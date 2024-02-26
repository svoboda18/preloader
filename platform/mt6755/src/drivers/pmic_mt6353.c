#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>
#include <mt6311.h>
#include <sec_devinfo.h>

#define EFUSE_DEBUG 0
#define PMIC_32K_LESS_DETECT_V1 0
U32 efuse_data[0x20]={0};
U32 upmu_get_reg_value(kal_uint32 reg);
int PMIC_AUXADC_INIT_SETTING_V1(void);
//==============================================================================
// PMIC access API
//==============================================================================
U32 pmic_read_interface (U32 RegNum, U32 *val, U32 MASK, U32 SHIFT)
{
    U32 return_value = 0;
    U32 pmic_reg = 0;
    U32 rdata = 0;

    //mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        print("[PMIC_PRELOADER] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    //print("[pmic_read_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

    pmic_reg &= (MASK << SHIFT);
    *val = (pmic_reg >> SHIFT);
    //print("[pmic_read_interface] val=0x%x\n", *val);

    return return_value;
}

U32 pmic_get_register_value(U32 RegNum, U32 MASK, U32 SHIFT)
{
	U32 val = 0;
	U32 ret;

	ret=pmic_read_interface (RegNum,&val,MASK,SHIFT);

	return val;
}

U32 pmic_config_interface (U32 RegNum, U32 val, U32 MASK, U32 SHIFT)
{
    U32 return_value = 0;
    U32 pmic_reg = 0;
    U32 rdata = 0;

    //1. mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        print("[PMIC_PRELOADER] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    //print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

    pmic_reg &= ~(MASK << SHIFT);
    pmic_reg |= (val << SHIFT);

    //2. mt_write_byte(RegNum, pmic_reg);
    return_value= pwrap_wacs2(1, (RegNum), pmic_reg, &rdata);
    if(return_value!=0)
    {
        print("[PMIC_PRELOADER] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    //print("[pmic_config_interface] write Reg[%x]=0x%x\n", RegNum, pmic_reg);

#if 0
    //3. Double Check
    //mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        print("[pmic_config_interface] Reg[%x]= pmic_wrap write data fail\n", RegNum);
        return return_value;
    }
    print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);
#endif

    return return_value;
}

void upmu_set_reg_value(kal_uint32 reg, kal_uint32 reg_val)
{
    U32 ret=0;

    ret=pmic_config_interface(reg, reg_val, 0xFFFF, 0x0);
}

//==============================================================================
// PMIC Usage APIs
//==============================================================================
U32 get_MT6353_PMIC_chip_version (void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6353_SWCID),
                           (&val),
                           (U32)(PMIC_SWCID_MASK),
                           (U32)(PMIC_SWCID_SHIFT)
	                       );

    return val;
}

int pmic_detect_powerkey(void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6353_TOPSTATUS),
                           (&val),
                           (U32)(PMIC_PWRKEY_DEB_MASK),
                           (U32)(PMIC_PWRKEY_DEB_SHIFT)
	                       );

    if (val==1){
        print("[PMIC_PRELOADER] pl pmic powerkey Release\n");
        return 0;
    }else{
        print("[PMIC_PRELOADER] pl pmic powerkey Press\n");
        return 1;
    }
}

int pmic_detect_homekey(void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6353_TOPSTATUS),
                           (&val),
                           (U32)(PMIC_HOMEKEY_DEB_MASK),
                           (U32)(PMIC_HOMEKEY_DEB_SHIFT)
	                       );

    if (val==1){
        print("[PMIC_PRELOADER] pl pmic FCHRKEY Release\n");
        return 0;
    }else{
        print("[PMIC_PRELOADER] pl pmic FCHRKEY Press\n");
        return 1;
    }
}

U32 pmic_IsUsbCableIn (void)
{
    U32 ret=0;
    U32 val=0;

#if CFG_EVB_PLATFORM
    val = 1; // for bring up
    //print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);
#else
    ret=pmic_read_interface( (U32)(MT6353_CHR_CON0),
                           (&val),
                           (U32)(PMIC_RGS_CHRDET_MASK),
                           (U32)(PMIC_RGS_CHRDET_SHIFT)
	                       );
    print("[PMIC_PRELOADER] pmic_IsUsbCableIn %d\n", val);
#endif

    if(val)
        return PMIC_CHRDET_EXIST;
    else
        return PMIC_CHRDET_NOT_EXIST;
}

void mt6353_upmu_set_baton_tdet_en(U32 val)
{
    U32 ret=0;
    ret=pmic_config_interface( (U32)(PMIC_BATON_TDET_EN_ADDR),
                             (U32)(val),
                             (U32)(PMIC_BATON_TDET_EN_MASK),
                             (U32)(PMIC_BATON_TDET_EN_SHIFT)
	                         );
}

void mt6353_upmu_set_rg_baton_en(U32 val)
{
    U32 ret=0;
    ret=pmic_config_interface( (U32)(PMIC_RG_BATON_EN_ADDR),
                             (U32)(val),
                             (U32)(PMIC_RG_BATON_EN_MASK),
                             (U32)(PMIC_RG_BATON_EN_SHIFT)
	                         );
}

U32 mt6353_upmu_get_rgs_baton_undet(void)
{
    U32 ret=0;
    U32 val=0;
    ret=pmic_read_interface( (U32)(PMIC_RGS_BATON_UNDET_ADDR),
                           (&val),
                           (U32)(PMIC_RGS_BATON_UNDET_MASK),
                           (U32)(PMIC_RGS_BATON_UNDET_SHIFT)
	                       );
   return val;
}

kal_uint32 upmu_is_chr_det(void)
{
  kal_uint32 ret=0;
  kal_uint32 val=0;
#if CFG_EVB_PLATFORM
    val = 1; // for bring up
    //print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);
#else
  ret=pmic_read_interface( (kal_uint32)(MT6353_CHR_CON0),
                           (&val),
                           (kal_uint32)(PMIC_RGS_CHRDET_MASK),
                           (kal_uint32)(PMIC_RGS_CHRDET_SHIFT)
	                       );
#endif
  return val;
}

void PMIC_enable_long_press_reboot(void)
{
#ifdef SLT
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#else
#if !CFG_FPGA_PLATFORM
#if !CFG_EVB_PLATFORM
#if KPD_PMIC_LPRST_TD!=0
	#if ONEKEY_REBOOT_NORMAL_MODE_PL
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x01, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6353_TOP_RST_MISC, (U32)KPD_PMIC_LPRST_TD, PMIC_RG_PWRKEY_RST_TD_MASK, PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#else
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x01, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x01, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6353_TOP_RST_MISC, (U32)KPD_PMIC_LPRST_TD, PMIC_RG_PWRKEY_RST_TD_MASK, PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#endif
#else
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
#endif /* !CFG_EVB_PLATFORM */
#else
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif /* !CFG_FPGA_PLATFORM */
#endif /* SLT */
}

U32 PMIC_VUSB_EN(void)
{
	int ret=0;

	ret = pmic_config_interface( (kal_uint32)(MT6353_LDO1_VUSB33_CON0),
			(kal_uint32)(1),
			(kal_uint32)(PMIC_LDO_VUSB33_EN_MASK),
			(kal_uint32)(PMIC_LDO_VUSB33_EN_SHIFT)
			);

	return ret;
}

U32 upmu_get_reg_value(kal_uint32 reg)
{
    U32 ret=0;
    U32 reg_val=0;

    ret=pmic_read_interface(reg, &reg_val, 0xFFFF, 0x0);

    return reg_val;
}

kal_uint16 pmic_read_efuse(kal_uint16 addr)
{
	kal_uint32 i, j;
	kal_uint32 ret, reg_val = 0;

	pmic_config_interface(PMIC_RG_OTP_PA_ADDR, addr*2, PMIC_RG_OTP_PA_MASK, PMIC_RG_OTP_PA_SHIFT);
	udelay(100);
	ret=pmic_read_interface(PMIC_RG_OTP_RD_TRIG_ADDR, &reg_val, PMIC_RG_OTP_RD_TRIG_MASK, PMIC_RG_OTP_RD_TRIG_SHIFT);

	if(reg_val==0)
	{
		pmic_config_interface(PMIC_RG_OTP_RD_TRIG_ADDR, 1, PMIC_RG_OTP_RD_TRIG_MASK, PMIC_RG_OTP_RD_TRIG_SHIFT);
	}
	else
	{
		pmic_config_interface(PMIC_RG_OTP_RD_TRIG_ADDR, 0, PMIC_RG_OTP_RD_TRIG_MASK, PMIC_RG_OTP_RD_TRIG_SHIFT);
	}

	/* MT6328 HW bug, need to wait 3T 75K */
        udelay(300);
	do
	{
	ret=pmic_read_interface(PMIC_RG_OTP_RD_BUSY_ADDR, &reg_val, PMIC_RG_OTP_RD_BUSY_MASK, PMIC_RG_OTP_RD_BUSY_SHIFT);
	}while(reg_val==1);
	udelay(1000);
	ret=pmic_read_interface(PMIC_RG_OTP_DOUT_SW_ADDR, &reg_val, PMIC_RG_OTP_DOUT_SW_MASK, PMIC_RG_OTP_DOUT_SW_SHIFT);

	return reg_val;

}


void pmic_efuse_check(void)
{
	/* print("[0x%x]=0x%x ", 0x0CF6, upmu_get_reg_value(0x0CF6)); */
	print("[0x%x]=0x%x ", 0x0A98, upmu_get_reg_value(0x0A98));
	print("[0x%x]=0x%x ", 0x0A9C, upmu_get_reg_value(0x0A9C));
	print("[0x%x]=0x%x \n", 0x0AA0, upmu_get_reg_value(0x0AA0));
	
	print("[0x%x]=0x%x ", 0x0ABE, upmu_get_reg_value(0x0ABE));
	print("[0x%x]=0x%x ", 0x0ABC, upmu_get_reg_value(0x0ABC));
	print("[0x%x]=0x%x ", 0x0AAA, upmu_get_reg_value(0x0AAA));
	print("[0x%x]=0x%x \n", 0x0AAC, upmu_get_reg_value(0x0AAC));
	
	print("[0x%x]=0x%x ", 0x0AD8, upmu_get_reg_value(0x0AD8));
	print("[0x%x]=0x%x ", 0x0AD6, upmu_get_reg_value(0x0AD6));
    print("[0x%x]=0x%x ", 0x0ACC, upmu_get_reg_value(0x0ACC));
	print("[0x%x]=0x%x \n", 0x0ACE, upmu_get_reg_value(0x0ACE));
	
    print("[0x%x]=0x%x ", 0x0ACA, upmu_get_reg_value(0x0ACA));
	print("[0x%x]=0x%x ", 0x0AAE, upmu_get_reg_value(0x0AAE));
	print("[0x%x]=0x%x ", 0x043A, upmu_get_reg_value(0x043A));
	print("[0x%x]=0x%x \n", 0x0442, upmu_get_reg_value(0x0442));

	print("[0x%x]=0x%x ", 0x0454, upmu_get_reg_value(0x0454));
	print("[0x%x]=0x%x ", 0x0458, upmu_get_reg_value(0x0458));
	print("[0x%x]=0x%x ", 0x045E, upmu_get_reg_value(0x045E));
	print("[0x%x]=0x%x \n ", 0x0460, upmu_get_reg_value(0x0460));

	print("[0x%x]=0x%x ", 0x044A, upmu_get_reg_value(0x044A));
	print("[0x%x]=0x%x ", 0x044E, upmu_get_reg_value(0x044E));
	print("[0x%x]=0x%x ", 0x046C, upmu_get_reg_value(0x046C));
	print("[0x%x]=0x%x \n ", 0x0470, upmu_get_reg_value(0x0470));

	print("[0x%x]=0x%x ", 0x043C, upmu_get_reg_value(0x043C));
	print("[0x%x]=0x%x ", 0x0458, upmu_get_reg_value(0x0458));
	print("[0x%x]=0x%x ", 0x0460, upmu_get_reg_value(0x0460));
	/* print("[0x%x]=0x%x \n", 0x044E, upmu_get_reg_value(0x044E)); */
	
	print("[0x%x]=0x%x ", 0x044C, upmu_get_reg_value(0x044C));
	print("[0x%x]=0x%x ", 0x046E, upmu_get_reg_value(0x046E));
	print("[0x%x]=0x%x ", 0x0456, upmu_get_reg_value(0x0456));
	print("[0x%x]=0x%x \n", 0x043E, upmu_get_reg_value(0x043E));
	
	print("[0x%x]=0x%x ", 0x0460, upmu_get_reg_value(0x0460));
	print("[0x%x]=0x%x \n", 0x045E, upmu_get_reg_value(0x045E));
   
}

void pmic_efuse_check_bit(void)
{
	U32 val_reg=0;
	print("pmic_efuse_check_bit\n");
	
	/* pmic_read_interface(0x0CF6,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,3   ,val_reg,(efuse_data[0x1 ] >>12 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,4   ,val_reg,(efuse_data[0x1 ] >>13 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,5   ,val_reg,(efuse_data[0x1 ] >>14 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,6   ,val_reg,(efuse_data[0x1 ] >>15 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,7   ,val_reg,(efuse_data[0x2 ] >>0  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,8   ,val_reg,(efuse_data[0x2 ] >>1  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,9   ,val_reg,(efuse_data[0x2 ] >>2  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,10  ,val_reg,(efuse_data[0x2 ] >>3  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,11  ,val_reg,(efuse_data[0x2 ] >>4  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,12 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,12  ,val_reg,(efuse_data[0x2 ] >>5  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,13 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,13  ,val_reg,(efuse_data[0x2 ] >>6  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,14 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,14  ,val_reg,(efuse_data[0x2 ] >>7  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,15 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,15  ,val_reg,(efuse_data[0x2 ] >>8  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,3   ,val_reg,(efuse_data[0x2 ] >>9  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,4   ,val_reg,(efuse_data[0x2 ] >>10 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,5   ,val_reg,(efuse_data[0x2 ] >>11 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,6   ,val_reg,(efuse_data[0x2 ] >>12 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,7   ,val_reg,(efuse_data[0x2 ] >>13 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,8   ,val_reg,(efuse_data[0x2 ] >>14 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,9   ,val_reg,(efuse_data[0x2 ] >>15 )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,10  ,val_reg,(efuse_data[0x3 ] >>0  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,12 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,12  ,val_reg,(efuse_data[0x3 ] >>1  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,13 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,13  ,val_reg,(efuse_data[0x3 ] >>2  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,14 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,14  ,val_reg,(efuse_data[0x3 ] >>3  )&1);
	pmic_read_interface(0x0CF6,&val_reg,0x1,15 );print("[0x%x:%d]:%d =%d  \n", 0x0CF6,15  ,val_reg,(efuse_data[0x3 ] >>4  )&1); */

	pmic_read_interface(0x0A98,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0A98,0   ,val_reg,(efuse_data[0x5 ] >>13 )&1);
	pmic_read_interface(0x0A98,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0A98,1   ,val_reg,(efuse_data[0x5 ] >>14 )&1);
	pmic_read_interface(0x0A98,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0A98,2   ,val_reg,(efuse_data[0x5 ] >>15 )&1);
	pmic_read_interface(0x0A98,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0A98,3   ,val_reg,(efuse_data[0x6 ] >>0  )&1);

	pmic_read_interface(0x0A9C,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0A9C,0   ,val_reg,(efuse_data[0x6 ] >>1  )&1);
	pmic_read_interface(0x0A9C,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0A9C,1   ,val_reg,(efuse_data[0x6 ] >>2  )&1);
	pmic_read_interface(0x0A9C,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0A9C,2   ,val_reg,(efuse_data[0x6 ] >>3  )&1);
	pmic_read_interface(0x0A9C,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0A9C,3   ,val_reg,(efuse_data[0x6 ] >>4  )&1);

	pmic_read_interface(0x0AA0,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0AA0,0   ,val_reg,(efuse_data[0x6 ] >>5  )&1);
	pmic_read_interface(0x0AA0,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0AA0,1   ,val_reg,(efuse_data[0x6 ] >>6  )&1);
	pmic_read_interface(0x0AA0,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0AA0,2   ,val_reg,(efuse_data[0x6 ] >>7  )&1);
	pmic_read_interface(0x0AA0,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0AA0,3   ,val_reg,(efuse_data[0x6 ] >>8  )&1);

	pmic_read_interface(0x0ABE,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0ABE,0   ,val_reg,(efuse_data[0x6 ] >>9  )&1);
	pmic_read_interface(0x0ABE,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0ABE,1   ,val_reg,(efuse_data[0x6 ] >>10 )&1);
	pmic_read_interface(0x0ABE,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0ABE,2   ,val_reg,(efuse_data[0x6 ] >>11 )&1);
	pmic_read_interface(0x0ABE,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0ABE,3   ,val_reg,(efuse_data[0x6 ] >>12 )&1);

	pmic_read_interface(0x0ABC,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0ABC,0   ,val_reg,(efuse_data[0x6 ] >>13 )&1);
	pmic_read_interface(0x0ABC,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0ABC,1   ,val_reg,(efuse_data[0x6 ] >>14 )&1);
	pmic_read_interface(0x0ABC,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0ABC,2   ,val_reg,(efuse_data[0x6 ] >>15 )&1);
	pmic_read_interface(0x0ABC,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0ABC,3   ,val_reg,(efuse_data[0x7 ] >>0  )&1);

	pmic_read_interface(0x0AAA,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0AAA,0   ,val_reg,(efuse_data[0x7 ] >>1  )&1);
	pmic_read_interface(0x0AAA,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0AAA,1   ,val_reg,(efuse_data[0x7 ] >>2  )&1);
	pmic_read_interface(0x0AAA,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0AAA,2   ,val_reg,(efuse_data[0x7 ] >>3  )&1);
	pmic_read_interface(0x0AAA,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0AAA,3   ,val_reg,(efuse_data[0x7 ] >>4  )&1);

	pmic_read_interface(0x0AAC,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0AAC,0   ,val_reg,(efuse_data[0x7 ] >>5  )&1);
	pmic_read_interface(0x0AAC,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0AAC,1   ,val_reg,(efuse_data[0x7 ] >>6  )&1);
	pmic_read_interface(0x0AAC,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0AAC,2   ,val_reg,(efuse_data[0x7 ] >>7  )&1);
	pmic_read_interface(0x0AAC,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0AAC,3   ,val_reg,(efuse_data[0x7 ] >>8  )&1);

	pmic_read_interface(0x0AD8,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0AD8,0   ,val_reg,(efuse_data[0x7 ] >>9  )&1);
	pmic_read_interface(0x0AD8,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0AD8,1   ,val_reg,(efuse_data[0x7 ] >>10 )&1);
	pmic_read_interface(0x0AD8,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0AD8,2   ,val_reg,(efuse_data[0x7 ] >>11 )&1);
	pmic_read_interface(0x0AD8,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0AD8,3   ,val_reg,(efuse_data[0x7 ] >>12 )&1);

	pmic_read_interface(0x0AD6,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0AD6,0   ,val_reg,(efuse_data[0x7 ] >>13 )&1);
	pmic_read_interface(0x0AD6,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0AD6,1   ,val_reg,(efuse_data[0x7 ] >>14 )&1);
	pmic_read_interface(0x0AD6,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0AD6,2   ,val_reg,(efuse_data[0x7 ] >>15 )&1);
	pmic_read_interface(0x0AD6,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0AD6,3   ,val_reg,(efuse_data[0x8 ] >>0  )&1);

	pmic_read_interface(0x0ACC,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0ACC,0   ,val_reg,(efuse_data[0x8 ] >>1  )&1);
	pmic_read_interface(0x0ACC,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0ACC,1   ,val_reg,(efuse_data[0x8 ] >>2  )&1);
	pmic_read_interface(0x0ACC,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0ACC,2   ,val_reg,(efuse_data[0x8 ] >>3  )&1);
	pmic_read_interface(0x0ACC,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0ACC,3   ,val_reg,(efuse_data[0x8 ] >>4  )&1);

	pmic_read_interface(0x0ACE,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0ACE,0   ,val_reg,(efuse_data[0x8 ] >>5  )&1);
	pmic_read_interface(0x0ACE,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0ACE,1   ,val_reg,(efuse_data[0x8 ] >>6  )&1);
	pmic_read_interface(0x0ACE,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0ACE,2   ,val_reg,(efuse_data[0x8 ] >>7  )&1);
	pmic_read_interface(0x0ACE,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0ACE,3   ,val_reg,(efuse_data[0x8 ] >>8  )&1);

	pmic_read_interface(0x0ACA,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0ACA,0   ,val_reg,(efuse_data[0x8 ] >>9  )&1);
	pmic_read_interface(0x0ACA,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0ACA,1   ,val_reg,(efuse_data[0x8 ] >>10 )&1);
	pmic_read_interface(0x0ACA,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0ACA,2   ,val_reg,(efuse_data[0x8 ] >>11 )&1);
	pmic_read_interface(0x0ACA,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0ACA,3   ,val_reg,(efuse_data[0x8 ] >>12 )&1);

	pmic_read_interface(0x0AAE,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0AAE,0   ,val_reg,(efuse_data[0x9 ] >>2  )&1);
	pmic_read_interface(0x0AAE,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0AAE,1   ,val_reg,(efuse_data[0x9 ] >>3  )&1);
	pmic_read_interface(0x0AAE,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0AAE,2   ,val_reg,(efuse_data[0x9 ] >>4  )&1);
	pmic_read_interface(0x0AAE,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0AAE,3   ,val_reg,(efuse_data[0x9 ] >>5  )&1);

	pmic_read_interface(0x043A,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x043A,0   ,val_reg,(efuse_data[0x9 ] >>6  )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x043A,1   ,val_reg,(efuse_data[0x9 ] >>7  )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x043A,2   ,val_reg,(efuse_data[0x9 ] >>8  )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x043A,3   ,val_reg,(efuse_data[0x9 ] >>9  )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x043A,4   ,val_reg,(efuse_data[0x9 ] >>10 )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x043A,5   ,val_reg,(efuse_data[0x9 ] >>11 )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x043A,6   ,val_reg,(efuse_data[0x9 ] >>12 )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x043A,7   ,val_reg,(efuse_data[0x9 ] >>13 )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x043A,8   ,val_reg,(efuse_data[0x9 ] >>14 )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x043A,9   ,val_reg,(efuse_data[0x9 ] >>15 )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x043A,10  ,val_reg,(efuse_data[0xa ] >>0  )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x043A,11  ,val_reg,(efuse_data[0xa ] >>1  )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,12 );print("[0x%x:%d]:%d =%d  \n", 0x043A,12  ,val_reg,(efuse_data[0xa ] >>2  )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,13 );print("[0x%x:%d]:%d =%d  \n", 0x043A,13  ,val_reg,(efuse_data[0xa ] >>3  )&1);
	pmic_read_interface(0x043A,&val_reg,0x1,14 );print("[0x%x:%d]:%d =%d  \n", 0x043A,14  ,val_reg,(efuse_data[0xa ] >>4  )&1);

	pmic_read_interface(0x0442,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0442,0   ,val_reg,(efuse_data[0xa ] >>5  )&1);
	pmic_read_interface(0x0442,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0442,1   ,val_reg,(efuse_data[0xa ] >>6  )&1);
	pmic_read_interface(0x0442,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0442,2   ,val_reg,(efuse_data[0xa ] >>7  )&1);
	pmic_read_interface(0x0442,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0442,3   ,val_reg,(efuse_data[0xa ] >>8  )&1);
	pmic_read_interface(0x0442,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x0442,4   ,val_reg,(efuse_data[0xa ] >>9  )&1);
	pmic_read_interface(0x0442,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x0442,5   ,val_reg,(efuse_data[0xa ] >>10 )&1);
	
	pmic_read_interface(0x0454,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0454,3   ,val_reg,(efuse_data[0xa ] >>11 )&1);
	pmic_read_interface(0x0454,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x0454,4   ,val_reg,(efuse_data[0xa ] >>12 )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x0458,6   ,val_reg,(efuse_data[0xa ] >>13 )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x0458,7   ,val_reg,(efuse_data[0xa ] >>14 )&1);
	pmic_read_interface(0x045E,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x045E,3   ,val_reg,(efuse_data[0xa ] >>15 )&1);
	pmic_read_interface(0x045E,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x045E,4   ,val_reg,(efuse_data[0xb ] >>0  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x0460,6   ,val_reg,(efuse_data[0xb ] >>1  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x0460,7   ,val_reg,(efuse_data[0xb ] >>2  )&1);
	pmic_read_interface(0x044A,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x044A,3   ,val_reg,(efuse_data[0xb ] >>5  )&1);
	pmic_read_interface(0x044A,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x044A,4   ,val_reg,(efuse_data[0xb ] >>6  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x044E,6   ,val_reg,(efuse_data[0xb ] >>7  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x044E,7   ,val_reg,(efuse_data[0xb ] >>8  )&1);
	pmic_read_interface(0x046C,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x046C,3   ,val_reg,(efuse_data[0xb ] >>9  )&1);
	pmic_read_interface(0x046C,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x046C,4   ,val_reg,(efuse_data[0xb ] >>10 )&1);
	pmic_read_interface(0x0470,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x0470,6   ,val_reg,(efuse_data[0xb ] >>11 )&1);
	pmic_read_interface(0x0470,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x0470,7   ,val_reg,(efuse_data[0xb ] >>12 )&1);
	
	pmic_read_interface(0x043C,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x043C,3   ,val_reg,(efuse_data[0xc ] >>1  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x043C,4   ,val_reg,(efuse_data[0xc ] >>2  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x043C,5   ,val_reg,(efuse_data[0xc ] >>3  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x043C,6   ,val_reg,(efuse_data[0xc ] >>4  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x043C,7   ,val_reg,(efuse_data[0xc ] >>5  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x043C,8   ,val_reg,(efuse_data[0xc ] >>6  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x043C,9   ,val_reg,(efuse_data[0xc ] >>7  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x043C,10  ,val_reg,(efuse_data[0xc ] >>8  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x043C,11  ,val_reg,(efuse_data[0xc ] >>9  )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x043C,0   ,val_reg,(efuse_data[0xc ] >>10 )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x043C,1   ,val_reg,(efuse_data[0xc ] >>11 )&1);
	pmic_read_interface(0x043C,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x043C,2   ,val_reg,(efuse_data[0xc ] >>12 )&1);

	pmic_read_interface(0x0458,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0458,0   ,val_reg,(efuse_data[0xc ] >>13 )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0458,1   ,val_reg,(efuse_data[0xc ] >>14 )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0458,2   ,val_reg,(efuse_data[0xc ] >>15 )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0458,3   ,val_reg,(efuse_data[0xd ] >>0  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x0458,4   ,val_reg,(efuse_data[0xd ] >>1  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x0458,5   ,val_reg,(efuse_data[0xd ] >>2  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x0458,6   ,val_reg,(efuse_data[0xd ] >>3  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x0458,7   ,val_reg,(efuse_data[0xd ] >>4  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x0458,8   ,val_reg,(efuse_data[0xd ] >>5  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x0458,9   ,val_reg,(efuse_data[0xd ] >>6  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x0458,10  ,val_reg,(efuse_data[0xd ] >>7  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x0458,11  ,val_reg,(efuse_data[0xd ] >>8  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,12 );print("[0x%x:%d]:%d =%d  \n", 0x0458,12  ,val_reg,(efuse_data[0xd ] >>9  )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,13 );print("[0x%x:%d]:%d =%d  \n", 0x0458,13  ,val_reg,(efuse_data[0xd ] >>10 )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,14 );print("[0x%x:%d]:%d =%d  \n", 0x0458,14  ,val_reg,(efuse_data[0xd ] >>11 )&1);
	pmic_read_interface(0x0458,&val_reg,0x1,15 );print("[0x%x:%d]:%d =%d  \n", 0x0458,15  ,val_reg,(efuse_data[0xd ] >>12 )&1);

	/* pmic_read_interface(0x0460,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x0460,6   ,val_reg,(efuse_data[0xd ] >>13 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x0460,7   ,val_reg,(efuse_data[0xd ] >>14 )&1); */
	pmic_read_interface(0x0460,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x0460,8   ,val_reg,(efuse_data[0xd ] >>15 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x0460,9   ,val_reg,(efuse_data[0xe ] >>0  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x0460,10  ,val_reg,(efuse_data[0xe ] >>1  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x0460,11  ,val_reg,(efuse_data[0xe ] >>2  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,12 );print("[0x%x:%d]:%d =%d  \n", 0x0460,12  ,val_reg,(efuse_data[0xe ] >>3  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,13 );print("[0x%x:%d]:%d =%d  \n", 0x0460,13  ,val_reg,(efuse_data[0xe ] >>4  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,14 );print("[0x%x:%d]:%d =%d  \n", 0x0460,14  ,val_reg,(efuse_data[0xe ] >>5  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,15 );print("[0x%x:%d]:%d =%d  \n", 0x0460,15  ,val_reg,(efuse_data[0xe ] >>6  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,16 );print("[0x%x:%d]:%d =%d  \n", 0x0460,16  ,val_reg,(efuse_data[0xe ] >>7  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,17 );print("[0x%x:%d]:%d =%d  \n", 0x0460,17  ,val_reg,(efuse_data[0xe ] >>8  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,18 );print("[0x%x:%d]:%d =%d  \n", 0x0460,18  ,val_reg,(efuse_data[0xe ] >>9  )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,19 );print("[0x%x:%d]:%d =%d  \n", 0x0460,19  ,val_reg,(efuse_data[0xe ] >>10 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,20 );print("[0x%x:%d]:%d =%d  \n", 0x0460,20  ,val_reg,(efuse_data[0xe ] >>11 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,21 );print("[0x%x:%d]:%d =%d  \n", 0x0460,21  ,val_reg,(efuse_data[0xe ] >>12 )&1);

	/* pmic_read_interface(0x044E,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x044E,0   ,val_reg,(efuse_data[0xe ] >>13 )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x044E,1   ,val_reg,(efuse_data[0xe ] >>14 )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x044E,2   ,val_reg,(efuse_data[0xe ] >>15 )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x044E,3   ,val_reg,(efuse_data[0xf ] >>0  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x044E,4   ,val_reg,(efuse_data[0xf ] >>1  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x044E,5   ,val_reg,(efuse_data[0xf ] >>2  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x044E,6   ,val_reg,(efuse_data[0xf ] >>3  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x044E,7   ,val_reg,(efuse_data[0xf ] >>4  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x044E,8   ,val_reg,(efuse_data[0xf ] >>5  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x044E,9   ,val_reg,(efuse_data[0xf ] >>6  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x044E,10  ,val_reg,(efuse_data[0xf ] >>7  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x044E,11  ,val_reg,(efuse_data[0xf ] >>8  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,12 );print("[0x%x:%d]:%d =%d  \n", 0x044E,12  ,val_reg,(efuse_data[0xf ] >>9  )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,13 );print("[0x%x:%d]:%d =%d  \n", 0x044E,13  ,val_reg,(efuse_data[0xf ] >>10 )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,14 );print("[0x%x:%d]:%d =%d  \n", 0x044E,14  ,val_reg,(efuse_data[0xf ] >>11 )&1);
	pmic_read_interface(0x044E,&val_reg,0x1,15 );print("[0x%x:%d]:%d =%d  \n", 0x044E,15  ,val_reg,(efuse_data[0xf ] >>12 )&1); */

	pmic_read_interface(0x044C,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x044C,6   ,val_reg,(efuse_data[0xf ] >>13 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x044C,7   ,val_reg,(efuse_data[0xf ] >>14 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x044C,8   ,val_reg,(efuse_data[0xf ] >>15 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x044C,9   ,val_reg,(efuse_data[0x10 ] >>0 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x044C,10  ,val_reg,(efuse_data[0x10 ] >>1 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x044C,11  ,val_reg,(efuse_data[0x10 ] >>2 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x044C,0   ,val_reg,(efuse_data[0x10 ] >>3 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x044C,1   ,val_reg,(efuse_data[0x10 ] >>4 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x044C,2   ,val_reg,(efuse_data[0x10 ] >>5 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x044C,3   ,val_reg,(efuse_data[0x10 ] >>6 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x044C,4   ,val_reg,(efuse_data[0x10 ] >>7 )&1);
	pmic_read_interface(0x044C,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x044C,5   ,val_reg,(efuse_data[0x10 ] >>8 )&1);

	pmic_read_interface(0x046E,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x046E,6   ,val_reg,(efuse_data[0x10 ] >>9  )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x046E,7   ,val_reg,(efuse_data[0x10 ] >>10 )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x046E,8   ,val_reg,(efuse_data[0x10 ] >>11 )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x046E,9   ,val_reg,(efuse_data[0x10 ] >>12 )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x046E,10  ,val_reg,(efuse_data[0x10 ] >>13 )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x046E,11  ,val_reg,(efuse_data[0x10 ] >>14 )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x046E,0   ,val_reg,(efuse_data[0x10 ] >>15 )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x046E,1   ,val_reg,(efuse_data[0x11 ] >>0  )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x046E,2   ,val_reg,(efuse_data[0x11 ] >>1  )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x046E,3   ,val_reg,(efuse_data[0x11 ] >>2  )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x046E,4   ,val_reg,(efuse_data[0x11 ] >>3  )&1);
	pmic_read_interface(0x046E,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x046E,5   ,val_reg,(efuse_data[0x11 ] >>4  )&1);

	pmic_read_interface(0x0456,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x0456,6   ,val_reg,(efuse_data[0x11 ] >>5 )&1);
	pmic_read_interface(0x0456,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x0456,7   ,val_reg,(efuse_data[0x11 ] >>6 )&1);
	pmic_read_interface(0x0456,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x0456,8   ,val_reg,(efuse_data[0x11 ] >>7 )&1);
	pmic_read_interface(0x0456,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x0456,9   ,val_reg,(efuse_data[0x11 ] >>8 )&1);
	pmic_read_interface(0x0456,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x0456,10  ,val_reg,(efuse_data[0x11 ] >>9 )&1);
	pmic_read_interface(0x0456,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x0456,11  ,val_reg,(efuse_data[0x11 ] >>10)&1);
	pmic_read_interface(0x0456,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0456,1   ,val_reg,(efuse_data[0x11 ] >>11)&1);
	pmic_read_interface(0x0456,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0456,2   ,val_reg,(efuse_data[0x11 ] >>12)&1);
	pmic_read_interface(0x0456,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0456,3   ,val_reg,(efuse_data[0x11 ] >>13)&1);
	pmic_read_interface(0x0456,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x0456,4   ,val_reg,(efuse_data[0x11 ] >>14)&1);
	pmic_read_interface(0x0456,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x0456,5   ,val_reg,(efuse_data[0x11 ] >>15)&1);
	pmic_read_interface(0x0456,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0456,0   ,val_reg,(efuse_data[0x12 ] >>0 )&1);

	pmic_read_interface(0x043E,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x043E,1   ,val_reg,(efuse_data[0x12 ] >>1 )&1);
	pmic_read_interface(0x043E,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x043E,2   ,val_reg,(efuse_data[0x12 ] >>2 )&1);

	pmic_read_interface(0x0460,&val_reg,0x1,0  );print("[0x%x:%d]:%d =%d  \n", 0x0460,0   ,val_reg,(efuse_data[0x16 ] >>4 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,1  );print("[0x%x:%d]:%d =%d  \n", 0x0460,1   ,val_reg,(efuse_data[0x16 ] >>5 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,2  );print("[0x%x:%d]:%d =%d  \n", 0x0460,2   ,val_reg,(efuse_data[0x16 ] >>6 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,3  );print("[0x%x:%d]:%d =%d  \n", 0x0460,3   ,val_reg,(efuse_data[0x16 ] >>7 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,4  );print("[0x%x:%d]:%d =%d  \n", 0x0460,4   ,val_reg,(efuse_data[0x16 ] >>8 )&1);
	pmic_read_interface(0x0460,&val_reg,0x1,5  );print("[0x%x:%d]:%d =%d  \n", 0x0460,5   ,val_reg,(efuse_data[0x16 ] >>9 )&1);

	pmic_read_interface(0x045E,&val_reg,0x1,6  );print("[0x%x:%d]:%d =%d  \n", 0x045E,6   ,val_reg,(efuse_data[0x16 ] >>10)&1);
	pmic_read_interface(0x045E,&val_reg,0x1,7  );print("[0x%x:%d]:%d =%d  \n", 0x045E,7   ,val_reg,(efuse_data[0x16 ] >>11)&1);
	pmic_read_interface(0x045E,&val_reg,0x1,8  );print("[0x%x:%d]:%d =%d  \n", 0x045E,8   ,val_reg,(efuse_data[0x16 ] >>12)&1);
	pmic_read_interface(0x045E,&val_reg,0x1,9  );print("[0x%x:%d]:%d =%d  \n", 0x045E,9   ,val_reg,(efuse_data[0x16 ] >>13)&1);
	pmic_read_interface(0x045E,&val_reg,0x1,10 );print("[0x%x:%d]:%d =%d  \n", 0x045E,10  ,val_reg,(efuse_data[0x16 ] >>14)&1);
	pmic_read_interface(0x045E,&val_reg,0x1,11 );print("[0x%x:%d]:%d =%d  \n", 0x045E,11  ,val_reg,(efuse_data[0x16 ] >>15)&1);
}

void pmic_efuse_management(void)
{
    int i=0;
    int is_efuse_trimed=0;
    u16 status = 0;
#if 1
    is_efuse_trimed = ((upmu_get_reg_value(PMIC_RG_OTP_DOUT_496_511_ADDR))>>15)&0x0001;

    print("[PMIC_PRELOADER][6353] is_efuse_trimed=0x%x,[0x%x]=0x%x\n", is_efuse_trimed, PMIC_RG_OTP_DOUT_496_511_ADDR, upmu_get_reg_value(PMIC_RG_OTP_DOUT_496_511_ADDR));

    if(is_efuse_trimed == 1)
    {
		//get efuse data
		//turn on efuse clock
		pmic_config_interface(PMIC_CLK_EFUSE_CK_PDN_HWEN_ADDR, 0x00, PMIC_CLK_EFUSE_CK_PDN_HWEN_MASK, PMIC_CLK_EFUSE_CK_PDN_HWEN_SHIFT);
		pmic_config_interface(PMIC_CLK_EFUSE_CK_PDN_ADDR, 0x00, PMIC_CLK_EFUSE_CK_PDN_MASK, PMIC_CLK_EFUSE_CK_PDN_SHIFT);
		pmic_config_interface(PMIC_RG_OTP_RD_SW_ADDR, 0x01, PMIC_RG_OTP_RD_SW_MASK, PMIC_RG_OTP_RD_SW_SHIFT);

		for(i=0;i<0x1f;i++)
		{
			efuse_data[i]=pmic_read_efuse(i);
		}

		//dump efuse data for check
		for(i=0x0;i<=0x1f;i++)
		print("[PMIC_PRELOADER][6353] efuse_data[0x%x]=0x%x\n", i, efuse_data[i]);


#if EFUSE_DEBUG
		print("Before apply pmic efuse\n");
		pmic_efuse_check();
#endif
		//------------------------------------------
		/* Ref file: MT6353_efuse_table_Final_20151215_Trim Status */
		/* pmic_config_interface(0x0CF6,((efuse_data[0x1 ] >>12 )&0x0001),0x1,3 );
		pmic_config_interface(0x0CF6,((efuse_data[0x1 ] >>13 )&0x0001),0x1,4 );
		pmic_config_interface(0x0CF6,((efuse_data[0x1 ] >>14 )&0x0001),0x1,5 );
		pmic_config_interface(0x0CF6,((efuse_data[0x1 ] >>15 )&0x0001),0x1,6 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>0  )&0x0001),0x1,7 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>1  )&0x0001),0x1,8 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>2  )&0x0001),0x1,9 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>3  )&0x0001),0x1,10);
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>4  )&0x0001),0x1,11);
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>5  )&0x0001),0x1,12);
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>6  )&0x0001),0x1,13);
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>7  )&0x0001),0x1,14);
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>8  )&0x0001),0x1,15);
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>9  )&0x0001),0x1,3 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>10 )&0x0001),0x1,4 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>11 )&0x0001),0x1,5 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>12 )&0x0001),0x1,6 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>13 )&0x0001),0x1,7 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>14 )&0x0001),0x1,8 );
		pmic_config_interface(0x0CF6,((efuse_data[0x2 ] >>15 )&0x0001),0x1,9 );
		pmic_config_interface(0x0CF6,((efuse_data[0x3 ] >>0  )&0x0001),0x1,10);
		pmic_config_interface(0x0CF6,((efuse_data[0x3 ] >>1  )&0x0001),0x1,12);
		pmic_config_interface(0x0CF6,((efuse_data[0x3 ] >>2  )&0x0001),0x1,13);
		pmic_config_interface(0x0CF6,((efuse_data[0x3 ] >>3  )&0x0001),0x1,14);
		pmic_config_interface(0x0CF6,((efuse_data[0x3 ] >>4  )&0x0001),0x1,15); */
		
		pmic_config_interface(0x0A98,((efuse_data[0x5 ] >>13 )&0x0001),0x1,0 );
		pmic_config_interface(0x0A98,((efuse_data[0x5 ] >>14 )&0x0001),0x1,1 );
		pmic_config_interface(0x0A98,((efuse_data[0x5 ] >>15 )&0x0001),0x1,2 );
		pmic_config_interface(0x0A98,((efuse_data[0x6 ] >>0  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0A9C,((efuse_data[0x6 ] >>1  )&0x0001),0x1,0 );
		pmic_config_interface(0x0A9C,((efuse_data[0x6 ] >>2  )&0x0001),0x1,1 );
		pmic_config_interface(0x0A9C,((efuse_data[0x6 ] >>3  )&0x0001),0x1,2 );
		pmic_config_interface(0x0A9C,((efuse_data[0x6 ] >>4  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0AA0,((efuse_data[0x6 ] >>5  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AA0,((efuse_data[0x6 ] >>6  )&0x0001),0x1,1 );
		pmic_config_interface(0x0AA0,((efuse_data[0x6 ] >>7  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AA0,((efuse_data[0x6 ] >>8  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0ABE,((efuse_data[0x6 ] >>9  )&0x0001),0x1,0 );
		pmic_config_interface(0x0ABE,((efuse_data[0x6 ] >>10 )&0x0001),0x1,1 );
		pmic_config_interface(0x0ABE,((efuse_data[0x6 ] >>11 )&0x0001),0x1,2 );
		pmic_config_interface(0x0ABE,((efuse_data[0x6 ] >>12 )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0ABC,((efuse_data[0x6 ] >>13 )&0x0001),0x1,0 );
		pmic_config_interface(0x0ABC,((efuse_data[0x6 ] >>14 )&0x0001),0x1,1 );
		pmic_config_interface(0x0ABC,((efuse_data[0x6 ] >>15 )&0x0001),0x1,2 );
		pmic_config_interface(0x0ABC,((efuse_data[0x7 ] >>0  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0AAA,((efuse_data[0x7 ] >>1  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AAA,((efuse_data[0x7 ] >>2  )&0x0001),0x1,1 );
		pmic_config_interface(0x0AAA,((efuse_data[0x7 ] >>3  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AAA,((efuse_data[0x7 ] >>4  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0AAC,((efuse_data[0x7 ] >>5  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AAC,((efuse_data[0x7 ] >>6  )&0x0001),0x1,1 );
		pmic_config_interface(0x0AAC,((efuse_data[0x7 ] >>7  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AAC,((efuse_data[0x7 ] >>8  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0AD8,((efuse_data[0x7 ] >>9  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AD8,((efuse_data[0x7 ] >>10 )&0x0001),0x1,1 );
		pmic_config_interface(0x0AD8,((efuse_data[0x7 ] >>11 )&0x0001),0x1,2 );
		pmic_config_interface(0x0AD8,((efuse_data[0x7 ] >>12 )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0AD6,((efuse_data[0x7 ] >>13 )&0x0001),0x1,0 );
		pmic_config_interface(0x0AD6,((efuse_data[0x7 ] >>14 )&0x0001),0x1,1 );
		pmic_config_interface(0x0AD6,((efuse_data[0x7 ] >>15 )&0x0001),0x1,2 );
		pmic_config_interface(0x0AD6,((efuse_data[0x8 ] >>0  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0ACC,((efuse_data[0x8 ] >>1  )&0x0001),0x1,0 );
		pmic_config_interface(0x0ACC,((efuse_data[0x8 ] >>2  )&0x0001),0x1,1 );
		pmic_config_interface(0x0ACC,((efuse_data[0x8 ] >>3  )&0x0001),0x1,2 );
		pmic_config_interface(0x0ACC,((efuse_data[0x8 ] >>4  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0ACE,((efuse_data[0x8 ] >>5  )&0x0001),0x1,0 );
		pmic_config_interface(0x0ACE,((efuse_data[0x8 ] >>6  )&0x0001),0x1,1 );
		pmic_config_interface(0x0ACE,((efuse_data[0x8 ] >>7  )&0x0001),0x1,2 );
		pmic_config_interface(0x0ACE,((efuse_data[0x8 ] >>8  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0ACA,((efuse_data[0x8 ] >>9  )&0x0001),0x1,0 );
		pmic_config_interface(0x0ACA,((efuse_data[0x8 ] >>10 )&0x0001),0x1,1 );
		pmic_config_interface(0x0ACA,((efuse_data[0x8 ] >>11 )&0x0001),0x1,2 );
		pmic_config_interface(0x0ACA,((efuse_data[0x8 ] >>12 )&0x0001),0x1,3 );
		
		pmic_config_interface(0x0AAE,((efuse_data[0x9 ] >>2  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AAE,((efuse_data[0x9 ] >>3  )&0x0001),0x1,1 );
		pmic_config_interface(0x0AAE,((efuse_data[0x9 ] >>4  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AAE,((efuse_data[0x9 ] >>5  )&0x0001),0x1,3 );
		
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>6  )&0x0001),0x1,0 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>7  )&0x0001),0x1,1 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>8  )&0x0001),0x1,2 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>9  )&0x0001),0x1,3 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>10 )&0x0001),0x1,4 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>11 )&0x0001),0x1,5 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>12 )&0x0001),0x1,6 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>13 )&0x0001),0x1,7 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>14 )&0x0001),0x1,8 );
		pmic_config_interface(0x043A,((efuse_data[0x9 ] >>15 )&0x0001),0x1,9 );
		pmic_config_interface(0x043A,((efuse_data[0xa ] >>0  )&0x0001),0x1,10);
		pmic_config_interface(0x043A,((efuse_data[0xa ] >>1  )&0x0001),0x1,11);
		pmic_config_interface(0x043A,((efuse_data[0xa ] >>2  )&0x0001),0x1,12);
		pmic_config_interface(0x043A,((efuse_data[0xa ] >>3  )&0x0001),0x1,13);
		pmic_config_interface(0x043A,((efuse_data[0xa ] >>4  )&0x0001),0x1,14);
		
		pmic_config_interface(0x0442,((efuse_data[0xa ] >>5  )&0x0001),0x1,0 );
		pmic_config_interface(0x0442,((efuse_data[0xa ] >>6  )&0x0001),0x1,1 );
		pmic_config_interface(0x0442,((efuse_data[0xa ] >>7  )&0x0001),0x1,2 );
		pmic_config_interface(0x0442,((efuse_data[0xa ] >>8  )&0x0001),0x1,3 );
		pmic_config_interface(0x0442,((efuse_data[0xa ] >>9  )&0x0001),0x1,4 );
		pmic_config_interface(0x0442,((efuse_data[0xa ] >>10 )&0x0001),0x1,5 );
		
		pmic_config_interface(0x0454,((efuse_data[0xa ] >>11 )&0x0001),0x1,3 );
		pmic_config_interface(0x0454,((efuse_data[0xa ] >>12 )&0x0001),0x1,4 );
		pmic_config_interface(0x0458,((efuse_data[0xa ] >>13 )&0x0001),0x1,6 );
		pmic_config_interface(0x0458,((efuse_data[0xa ] >>14 )&0x0001),0x1,7 );
		pmic_config_interface(0x045E,((efuse_data[0xa ] >>15 )&0x0001),0x1,3 );
		pmic_config_interface(0x045E,((efuse_data[0xb ] >>0  )&0x0001),0x1,4 );
		pmic_config_interface(0x0460,((efuse_data[0xb ] >>1  )&0x0001),0x1,6 );
		pmic_config_interface(0x0460,((efuse_data[0xb ] >>2  )&0x0001),0x1,7 );
		pmic_config_interface(0x044A,((efuse_data[0xb ] >>5  )&0x0001),0x1,3 );
		pmic_config_interface(0x044A,((efuse_data[0xb ] >>6  )&0x0001),0x1,4 );
		pmic_config_interface(0x044E,((efuse_data[0xb ] >>7  )&0x0001),0x1,6 );
		pmic_config_interface(0x044E,((efuse_data[0xb ] >>8  )&0x0001),0x1,7 );
		pmic_config_interface(0x046C,((efuse_data[0xb ] >>9  )&0x0001),0x1,3 );
		pmic_config_interface(0x046C,((efuse_data[0xb ] >>10 )&0x0001),0x1,4 );
		pmic_config_interface(0x0470,((efuse_data[0xb ] >>11 )&0x0001),0x1,6 );
		pmic_config_interface(0x0470,((efuse_data[0xb ] >>12 )&0x0001),0x1,7 );
			
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>1  )&0x0001),0x1,3 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>2  )&0x0001),0x1,4 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>3  )&0x0001),0x1,5 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>4  )&0x0001),0x1,6 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>5  )&0x0001),0x1,7 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>6  )&0x0001),0x1,8 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>7  )&0x0001),0x1,9 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>8  )&0x0001),0x1,10);
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>9  )&0x0001),0x1,11);
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>10 )&0x0001),0x1,0 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>11 )&0x0001),0x1,1 );
		pmic_config_interface(0x043C,((efuse_data[0xc ] >>12 )&0x0001),0x1,2 );

		pmic_config_interface(0x0458,((efuse_data[0xc ] >>13 )&0x0001),0x1,0 );
		pmic_config_interface(0x0458,((efuse_data[0xc ] >>14 )&0x0001),0x1,1 );
		pmic_config_interface(0x0458,((efuse_data[0xc ] >>15 )&0x0001),0x1,2 );
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>0  )&0x0001),0x1,3 );
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>1  )&0x0001),0x1,4 );
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>2  )&0x0001),0x1,5 );
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>3  )&0x0001),0x1,6 );
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>4  )&0x0001),0x1,7 );
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>5  )&0x0001),0x1,8 );
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>6  )&0x0001),0x1,9 );
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>7  )&0x0001),0x1,10);
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>8  )&0x0001),0x1,11);
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>9  )&0x0001),0x1,12);
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>10 )&0x0001),0x1,13);
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>11 )&0x0001),0x1,14);
		pmic_config_interface(0x0458,((efuse_data[0xd ] >>12 )&0x0001),0x1,15);
		
		/* pmic_config_interface(0x0460,((efuse_data[0xd ] >>13 )&0x0001),0x1,6 );
		pmic_config_interface(0x0460,((efuse_data[0xd ] >>14 )&0x0001),0x1,7 ); */
		pmic_config_interface(0x0460,((efuse_data[0xd ] >>15 )&0x0001),0x1,8 );
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>0  )&0x0001),0x1,9 );
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>1  )&0x0001),0x1,10);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>2  )&0x0001),0x1,11);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>3  )&0x0001),0x1,12);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>4  )&0x0001),0x1,13);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>5  )&0x0001),0x1,14);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>6  )&0x0001),0x1,15);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>7  )&0x0001),0x1,16);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>8  )&0x0001),0x1,17);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>9  )&0x0001),0x1,18);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>10 )&0x0001),0x1,19);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>11 )&0x0001),0x1,20);
		pmic_config_interface(0x0460,((efuse_data[0xe ] >>12 )&0x0001),0x1,21);
		
		/* pmic_config_interface(0x044E,((efuse_data[0xe ] >>13 )&0x0001),0x1,0 );
		pmic_config_interface(0x044E,((efuse_data[0xe ] >>14 )&0x0001),0x1,1 );
		pmic_config_interface(0x044E,((efuse_data[0xe ] >>15 )&0x0001),0x1,2 );
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>0  )&0x0001),0x1,3 );
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>1  )&0x0001),0x1,4 );
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>2  )&0x0001),0x1,5 );
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>3  )&0x0001),0x1,6 );
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>4  )&0x0001),0x1,7 );
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>5  )&0x0001),0x1,8 );
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>6  )&0x0001),0x1,9 );
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>7  )&0x0001),0x1,10);
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>8  )&0x0001),0x1,11);
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>9  )&0x0001),0x1,12);
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>10 )&0x0001),0x1,13);
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>11 )&0x0001),0x1,14);
		pmic_config_interface(0x044E,((efuse_data[0xf ] >>12 )&0x0001),0x1,15); */
				
		pmic_config_interface(0x044C,((efuse_data[0xf ] >>13 )&0x0001),0x1,6 );
		pmic_config_interface(0x044C,((efuse_data[0xf ] >>14 )&0x0001),0x1,7 );
		pmic_config_interface(0x044C,((efuse_data[0xf ] >>15 )&0x0001),0x1,8 );
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>0  )&0x0001),0x1,9 );
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>1  )&0x0001),0x1,10);
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>2  )&0x0001),0x1,11);
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>3  )&0x0001),0x1,0 );
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>4  )&0x0001),0x1,1 );
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>5  )&0x0001),0x1,2 );
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>6  )&0x0001),0x1,3 );
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>7  )&0x0001),0x1,4 );
		pmic_config_interface(0x044C,((efuse_data[0x10 ] >>8  )&0x0001),0x1,5 );
		
		pmic_config_interface(0x046E,((efuse_data[0x10 ] >>9  )&0x0001),0x1,6 );
		pmic_config_interface(0x046E,((efuse_data[0x10 ] >>10 )&0x0001),0x1,7 );
		pmic_config_interface(0x046E,((efuse_data[0x10 ] >>11 )&0x0001),0x1,8 );
		pmic_config_interface(0x046E,((efuse_data[0x10 ] >>12 )&0x0001),0x1,9 );
		pmic_config_interface(0x046E,((efuse_data[0x10 ] >>13 )&0x0001),0x1,10);
		pmic_config_interface(0x046E,((efuse_data[0x10 ] >>14 )&0x0001),0x1,11);
		pmic_config_interface(0x046E,((efuse_data[0x10 ] >>15 )&0x0001),0x1,0 );
		pmic_config_interface(0x046E,((efuse_data[0x11 ] >>0  )&0x0001),0x1,1 );
		pmic_config_interface(0x046E,((efuse_data[0x11 ] >>1  )&0x0001),0x1,2 );
		pmic_config_interface(0x046E,((efuse_data[0x11 ] >>2  )&0x0001),0x1,3 );
		pmic_config_interface(0x046E,((efuse_data[0x11 ] >>3  )&0x0001),0x1,4 );
		pmic_config_interface(0x046E,((efuse_data[0x11 ] >>4  )&0x0001),0x1,5 );
		
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>5  )&0x0001),0x1,6 );
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>6  )&0x0001),0x1,7 );
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>7  )&0x0001),0x1,8 );
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>8  )&0x0001),0x1,9 );
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>9  )&0x0001),0x1,10);
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>10 )&0x0001),0x1,11);
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>11 )&0x0001),0x1,1 );
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>12 )&0x0001),0x1,2 );
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>13 )&0x0001),0x1,3 );
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>14 )&0x0001),0x1,4 );
		pmic_config_interface(0x0456,((efuse_data[0x11 ] >>15 )&0x0001),0x1,5 );
		pmic_config_interface(0x0456,((efuse_data[0x12 ] >>0  )&0x0001),0x1,0 );
		
		pmic_config_interface(0x043E,((efuse_data[0x12 ] >>1 )&0x0001),0x1,1 );
		pmic_config_interface(0x043E,((efuse_data[0x12 ] >>2 )&0x0001),0x1,2 );
				
		pmic_config_interface(0x0460,((efuse_data[0x16 ] >>4 )&0x0001),0x1,0 );
		pmic_config_interface(0x0460,((efuse_data[0x16 ] >>5 )&0x0001),0x1,1 );
		pmic_config_interface(0x0460,((efuse_data[0x16 ] >>6 )&0x0001),0x1,2 );
		pmic_config_interface(0x0460,((efuse_data[0x16 ] >>7 )&0x0001),0x1,3 );
		pmic_config_interface(0x0460,((efuse_data[0x16 ] >>8 )&0x0001),0x1,4 );
		pmic_config_interface(0x0460,((efuse_data[0x16 ] >>9 )&0x0001),0x1,5 );
		
		pmic_config_interface(0x045E,((efuse_data[0x16 ] >>10 )&0x0001),0x1,6 );
		pmic_config_interface(0x045E,((efuse_data[0x16 ] >>11 )&0x0001),0x1,7 );
		pmic_config_interface(0x045E,((efuse_data[0x16 ] >>12 )&0x0001),0x1,8 );
		pmic_config_interface(0x045E,((efuse_data[0x16 ] >>13 )&0x0001),0x1,9 );
		pmic_config_interface(0x045E,((efuse_data[0x16 ] >>14 )&0x0001),0x1,10);
		pmic_config_interface(0x045E,((efuse_data[0x16 ] >>15 )&0x0001),0x1,11);

		//turn off efuse clock
		pmic_config_interface(PMIC_CLK_EFUSE_CK_PDN_HWEN_ADDR, 0x01, PMIC_CLK_EFUSE_CK_PDN_HWEN_MASK, PMIC_CLK_EFUSE_CK_PDN_HWEN_SHIFT);
		pmic_config_interface(PMIC_CLK_EFUSE_CK_PDN_ADDR, 0x01, PMIC_CLK_EFUSE_CK_PDN_MASK, PMIC_CLK_EFUSE_CK_PDN_SHIFT);

    //------------------------------------------


#if EFUSE_DEBUG
	print("After apply pmic efuse\n");
	pmic_efuse_check();
	pmic_efuse_check_bit();
#endif
    }
#endif
}

void pmic_disable_usbdl_wo_battery(void)
{
	print("[PMIC_PRELOADER] turn off usbdl wo battery..................\n");
    pmic_config_interface(PMIC_RG_USBDL_SET_ADDR, 0x0000, PMIC_RG_USBDL_SET_MASK, PMIC_RG_USBDL_SET_SHIFT);
    pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 0x0001, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);
}

void pmic_auxadc_debug(int index)
{
    int ret_val = 0, val = 0, val1 = 0, val2 = 0, val3 = 0, val4 = 0;
    ret_val=pmic_read_interface( (U32)(0x0240),
                           (&val),
                           (0xffff),
                           0);
    ret_val=pmic_read_interface( (U32)(0x0282),
                           (&val1),
                           (0xffff),
                           0);
    ret_val=pmic_read_interface( (U32)(0x025e),
                           (&val2),
                           (0xffff),
                           0);
    ret_val=pmic_read_interface( (U32)(0x023a),
                           (&val3),
                           (0xffff),
                           0);
    ret_val=pmic_read_interface( (U32)(0x0ea2),
                           (&val4),
                           (0xffff),
                           0);
	print("[PMIC_PRELOADER] [pmic_init]%d 0x0240:0x%x, 0x0282:0x%x, 0x025e:0x%x , 0x023a:0x%x , 0x0ea2:0x%x\n",index, val, val1, val2, val3, val4);
	ret_val = pmic_read_interface(MT6353_CLK_CKPDN_CON2, &val, PMIC_CLK_CKPDN_CON2_CLR_MASK, PMIC_CLK_CKPDN_CON2_CLR_SHIFT);

	ret_val = pmic_read_interface(PMIC_RG_BATON_HT_EN_RSV0_ADDR, &val1, 0xFFFF, 0x0);
	ret_val = pmic_read_interface(PMIC_LDO_BATON_HT_EN_ADDR, &val1, 0xFFFF, 0x0);
#if MT6351
	ret_val = pmic_read_interface(MT6353_LDO_VBIF28_CON0, &val2, 0xFFFF, 0x0);
	ret_val = pmic_read_interface(MT6353_TOP_CKHWEN_CON0, &val3, 0xFFFF, 0x0);
#endif /* MT6351 */
	ret_val = pmic_read_interface(MT6353_TOP_RST_MISC, &val4, 0xFFFF, 0x0);
	print("[PMIC_init] _CLK_CKPDN_CON2:0x%x BIF_BAT_CON0:0x%x LDO_VBIF28:0x%x 0x282:0x%x 0x2b6:0x%x\n" ,val ,val1 ,val2, val3, val4);


}

#if !CFG_EVB_PLATFORM || defined(MTK_EFUSE_WRITER_SUPPORT)
kal_int32 count_time_out=10;
#define VOLTAGE_FULL_RANGE     	1800
#define ADC_PRECISE		32768 	// 15 bits

kal_uint32 PMIC_IMM_GetOneChannelValue(kal_uint8 dwChannel, int deCount, int trimd)
{
	kal_int32 ret=0;
	kal_int32 ret_data;
	kal_int32 r_val_temp=0;
	kal_int32 adc_result=0;
	int count=0;
	kal_uint32 busy;
	kal_uint32 val = 0, val1 = 0, val2 = 0, val3 = 0, val4 = 0;
	/*
	CH0: BATSNS
	CH1: ISENSE
	CH2: VCDT
	CH3: BAT ON
	CH4: PMIC TEMP
	CH5: ACCDET
	CH6:
	CH7: TSX
	CH8:
	CH9:
	CH10:
	CH11:
	CH12:
	CH13:
	CH14:
	CH15:
	BATSNS 3v-4.5v
	ISENSE 1.5-4.5v
	BATON  0-1.8v
	VCDT   4v-14v
	ACCDET 1.8v
	GPS    1.8v

	*/

	if(dwChannel>15)
		return -1;

	ret = pmic_read_interface(MT6353_CLK_CKPDN_CON2, &val, PMIC_CLK_CKPDN_CON2_CLR_MASK, PMIC_CLK_CKPDN_CON2_CLR_SHIFT);
	ret = pmic_read_interface(PMIC_RG_BATON_HT_EN_RSV0_ADDR, &val1, 0xFFFF, 0x0);
	ret = pmic_read_interface(PMIC_LDO_BATON_HT_EN_ADDR, &val1, 0xFFFF, 0x0);
#if MT6351
	ret = pmic_read_interface(MT6353_LDO_VBIF28_CON0, &val2, 0xFFFF, 0x0);
	ret = pmic_read_interface(MT6353_TOP_CKHWEN_CON0, &val3, 0xFFFF, 0x0);
#endif /* MT6351 */
	ret = pmic_read_interface(MT6353_TOP_RST_MISC, &val4, 0xFFFF, 0x0);
	/* ch3 bug, wei-lin request us to add above code */
	print("[PMIC_init] _CLK_CKPDN_CON2:0x%x BIF_BAT_CON0:0x%x LDO_VBIF28:0x%x 0x282:0x%x 0x2b6:0x%x\n" ,val ,val1 ,val2, val3, val4);
	//ret=pmic_config_interface(MT6351_TOP_CLKSQ_SET,(1<<2),0xffff,0);
	ret=pmic_config_interface(MT6353_AUXADC_RQST0_SET,(1<<dwChannel),0xffff,0);

	busy=upmu_get_reg_value(MT6353_AUXADC_STA0);
	udelay(50);

	switch(dwChannel){
	case 0:
		while(pmic_get_register_value(PMIC_AUXADC_ADC_RDY_CH0_BY_AP_ADDR,PMIC_AUXADC_ADC_RDY_CH0_BY_AP_MASK,PMIC_AUXADC_ADC_RDY_CH0_BY_AP_SHIFT) != 1 )
		{
			mdelay(1);
			if( (count++) > count_time_out)
			{
			print( "[PMIC_PRELOADER][IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
			break;
			}
		}
		//ret_data = mt6351_get_register_value(PMIC_AUXADC_ADC_OUT_CH0_BY_AP);
		ret_data=pmic_get_register_value(PMIC_AUXADC_ADC_OUT_CH0_BY_AP_ADDR,PMIC_AUXADC_ADC_OUT_CH0_BY_AP_MASK,PMIC_AUXADC_ADC_OUT_CH0_BY_AP_SHIFT);
	break;
	case 1:
		while(pmic_get_register_value(PMIC_AUXADC_ADC_RDY_CH1_BY_AP_ADDR,PMIC_AUXADC_ADC_RDY_CH1_BY_AP_MASK,PMIC_AUXADC_ADC_RDY_CH1_BY_AP_SHIFT) != 1 )
		{
			mdelay(1);
			if( (count++) > count_time_out)
			{
			print( "[PMIC_PRELOADER][IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
			break;
			}
		}
		ret_data = pmic_get_register_value(PMIC_AUXADC_ADC_OUT_CH1_BY_AP_ADDR,PMIC_AUXADC_ADC_OUT_CH1_BY_AP_MASK,PMIC_AUXADC_ADC_OUT_CH1_BY_AP_SHIFT);
	break;
	case 2:
		while(pmic_get_register_value(PMIC_AUXADC_ADC_RDY_CH2_ADDR,PMIC_AUXADC_ADC_RDY_CH2_MASK,PMIC_AUXADC_ADC_RDY_CH2_SHIFT) != 1 )
		{
			mdelay(1);
			if( (count++) > count_time_out)
			{
			print( "[PMIC_PRELOADER][IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
			break;
			}
		}

		ret_data = pmic_get_register_value(PMIC_AUXADC_ADC_OUT_CH2_ADDR,PMIC_AUXADC_ADC_OUT_CH2_MASK,PMIC_AUXADC_ADC_OUT_CH2_SHIFT);
	break;
	case 3:
		while(pmic_get_register_value(PMIC_AUXADC_ADC_RDY_CH3_ADDR,PMIC_AUXADC_ADC_RDY_CH3_MASK,PMIC_AUXADC_ADC_RDY_CH3_SHIFT) != 1 )
		{
			mdelay(1);
			if( (count++) > count_time_out)
			{
			print( "[PMIC_PRELOADER][IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
			break;
			}
		}
		ret_data = pmic_get_register_value(PMIC_AUXADC_ADC_OUT_CH3_ADDR,PMIC_AUXADC_ADC_OUT_CH3_MASK,PMIC_AUXADC_ADC_OUT_CH3_SHIFT);
	break;


	default:
		print( "[PMIC_PRELOADER][AUXADC] Invalid channel value(%d,%d)\n", dwChannel, trimd);

	return -1;
	break;
	}

    switch(dwChannel){
        case 0:
            r_val_temp = 3;
            adc_result = (ret_data*r_val_temp*VOLTAGE_FULL_RANGE)/32768;
            break;
        case 1:
            r_val_temp = 3;
            adc_result = (ret_data*r_val_temp*VOLTAGE_FULL_RANGE)/32768;
            break;
        case 2:
            r_val_temp = 1;
            adc_result = (ret_data*r_val_temp*VOLTAGE_FULL_RANGE)/4096;
            break;
        case 3:
            r_val_temp = 1;
            adc_result = (ret_data*r_val_temp*VOLTAGE_FULL_RANGE)/4096;
            break;

        default:
            print( "[PMIC_PRELOADER][AUXADC] Invalid channel value(%d,%d)\n", dwChannel, trimd);

            return -1;
            break;
    }
	print( "[PMIC_PRELOADER][AUXADC] ch=%d raw=%d data=%d \n", dwChannel, ret_data,adc_result);

	return adc_result;

}

#endif

/******************************************************************************************************
* Adjust BUCK voltage
******************************************************************************************************/
#define PMIC_BUCK_GEN1(_name, _vosel, _ctrl, _da_ni_vosel, _da_ni_vosel_sync,_sleep,_on)	\
{	\
	.name = #_name,	\
	.vosel = (_vosel),	\
	.ctrl = (_ctrl),	\
	.da_ni_vosel = (_da_ni_vosel),	\
	.da_ni_vosel_sync = (_da_ni_vosel_sync),	\
	.sleep = (_sleep),	\
	.on = (_on),	\
}

struct mtk_buck_t {
	const char *name;
	kal_uint32 vosel;
	kal_uint32 ctrl;
	kal_uint32 da_ni_vosel;
	kal_uint32 da_ni_vosel_sync;
	kal_uint32 sleep;
	kal_uint32 on;
};

struct mtk_buck_t mtk_buck[] = {
	PMIC_BUCK_GEN1(VPROC,PMIC_BUCK_VPROC_VOSEL_ADDR,
		                      PMIC_BUCK_VPROC_VOSEL_CTRL_ADDR,
		                      PMIC_DA_NI_VPROC_VOSEL_ADDR,
		                      PMIC_DA_NI_VPROC_VOSEL_ADDR,
		                      PMIC_BUCK_VPROC_VOSEL_SLEEP_ADDR,
		                      PMIC_BUCK_VPROC_VOSEL_ON_ADDR),
	PMIC_BUCK_GEN1(VCORE,PMIC_BUCK_VCORE_VOSEL_ADDR,
		                      PMIC_BUCK_VCORE_VOSEL_CTRL_ADDR,
		                      PMIC_DA_NI_VCORE_VOSEL_ADDR,
		                      PMIC_DA_NI_VCORE_VOSEL_ADDR,
		                      PMIC_BUCK_VCORE_VOSEL_SLEEP_ADDR,
		                      PMIC_BUCK_VCORE_VOSEL_ON_ADDR),
	PMIC_BUCK_GEN1(VCORE2,PMIC_BUCK_VCORE2_VOSEL_ADDR,
		                      PMIC_BUCK_VCORE2_VOSEL_CTRL_ADDR,
		                      PMIC_DA_NI_VCORE2_VOSEL_ADDR,
		                      PMIC_DA_NI_VCORE2_VOSEL_ADDR,
		                      PMIC_BUCK_VCORE2_VOSEL_SLEEP_ADDR,
		                      PMIC_BUCK_VCORE2_VOSEL_ON_ADDR),
	PMIC_BUCK_GEN1(VS1,PMIC_BUCK_VS1_VOSEL_ADDR,
		                      PMIC_BUCK_VS1_VOSEL_CTRL_ADDR,
		                      PMIC_DA_NI_VS1_VOSEL_ADDR,
		                      PMIC_DA_NI_VS1_VOSEL_ADDR,
		                      PMIC_BUCK_VS1_VOSEL_SLEEP_ADDR,
		                      PMIC_BUCK_VS1_VOSEL_ON_ADDR),
};
U32 buck_adjust_voltage(BUCK_TYPE type, BUCK_VOLTAGE_UV voltage)
{
	U16 value = 0;
	kal_uint32 ret = 0;
	int reg_val = 0;

	if (type > VS1) {
		print("[PMIC_PRELOADER][pmic_init]Wrong buck type\n");
		return -1;
	}
	if (voltage > PMIC_VOLT_01_400000_V) {
		print("[PMIC_PRELOADER][pmic_init]Wrong buck voltage\n");
		return -1;
	}

	value = (pmic_buck_voltages[voltage] - 600000)/6250;
	print("[PMIC_PRELOADER][pmic_init]%s Expected volt: 0x%x\n", mtk_buck[type].name, value);
	switch(type)
	{
		default:
			upmu_set_reg_value(mtk_buck[type].vosel ,value);
			upmu_set_reg_value(mtk_buck[type].on ,value);
			//4.delay 100us
			udelay(100);
			pmic_read_interface(mtk_buck[type].vosel, &reg_val,0x7F,0);
			print("[PMIC_PRELOADER][pmic_init] %s[0x%x]=0x%x\n",
				mtk_buck[type].name, mtk_buck[type].vosel, reg_val);
			pmic_read_interface(mtk_buck[type].da_ni_vosel_sync, &reg_val,0x7F,0);
			print("[PMIC_PRELOADER][pmic_init] %s[0x%x]=0x%x\n",
				mtk_buck[type].name, mtk_buck[type].da_ni_vosel_sync, reg_val);
			break;
	}
	return 0;
}

U32 buck_vcore_segment_voltage(void)
{
	kal_uint32 ret = 0;
	kal_uint32 segment, fab;

	segment = seclib_get_devinfo_with_index(17) & 0xFF;
	fab = seclib_get_devinfo_with_index(18) & 0x40000000;

	/*----MT6353_BUCK_VCORE_VOL_CON1 0x632----*/
	if (segment == 0x41 || segment == 0x45 || (segment == 0xC1) || (segment == 0xC5)) {
	/* 6750T, 6750 eng sample or 6750S */
		ret = pmic_config_interface(0x632,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 1.0V */
		ret = pmic_config_interface(0x634,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 1.0V */
	} else if (segment == 0x42 || segment == 0x46 || (segment == 0xC2) || (segment == 0xC6)) {
	/* 6750 normal or 6750N */
#ifdef USE_FHD_DRAM_CAL_FLOW
		ret = pmic_config_interface(0x632,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 1.0V */
		ret = pmic_config_interface(0x634,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 1.0V */
#else /*--!USE_FHD_DRAM_CAL_FLOW--*/
		if (fab) {
		/* UMC */
			ret = pmic_config_interface(0x632,0x38,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 0.95V */
			ret = pmic_config_interface(0x634,0x38,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 0.95V */
		} else {
		/* TSMC */
			ret = pmic_config_interface(0x632,0x30,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 0.9V */
			ret = pmic_config_interface(0x634,0x30,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 0.9V */
		}
#endif /*--USE_FHD_DRAM_CAL_FLOW--*/
	} else if (segment == 0x43 || segment == 0x4B) {
	/* 6738 */
#ifdef USE_FHD_DRAM_CAL_FLOW
		ret = pmic_config_interface(0x632,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 1.0V */
		ret = pmic_config_interface(0x634,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 1.0V */
#else /*--!USE_FHD_DRAM_CAL_FLOW--*/
		if (fab) {
			ret = pmic_config_interface(0x632,0x38,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 0.95V */
			ret = pmic_config_interface(0x634,0x38,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 0.95V */
		} else {
			ret = pmic_config_interface(0x632,0x30,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 0.9V */
			ret = pmic_config_interface(0x634,0x30,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 0.9V */
		}
#endif /*--USE_FHD_DRAM_CAL_FLOW--*/
	} else if (segment == 0x82 || segment == 0x86) {
	/* 6755S */
		/* MT6311 voltage set in kernel , same as 6755 */
		/* set VCORE to 1.0V */
		ret = pmic_config_interface(0x632,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 1.0V */
		ret = pmic_config_interface(0x634,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 1.0V */
		/* set VPROC to 1.125V for GPU */
		ret = pmic_config_interface(0x60A,0x54,0x7F,0);
		ret = pmic_config_interface(0x60C,0x54,0x7F,0);
	} else {
		ret = pmic_config_interface(0x632,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 1.0V */
		ret = pmic_config_interface(0x634,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 1.0V */
	}
	return ret;
}

int PMIC_AUXADC_INIT_SETTING_V1(void)
{
	int ret_val=0, val;
#if MT6351
	ret_val = pmic_config_interface(PMIC_RG_VBIF28_ON_CTRL_ADDR, 0, PMIC_RG_VBIF28_ON_CTRL_MASK, PMIC_RG_VBIF28_ON_CTRL_SHIFT);
	ret_val = pmic_config_interface(PMIC_RG_VBIF28_EN_ADDR, 1, PMIC_RG_VBIF28_EN_MASK, PMIC_RG_VBIF28_EN_SHIFT);
	udelay(500); /* delay 500us, wait ldo settle */
#endif /* MT6351 */
	ret_val = pmic_config_interface(PMIC_AUXADC_CK_AON_ADDR,0x1,PMIC_AUXADC_CK_AON_MASK,PMIC_AUXADC_CK_AON_SHIFT);
	ret_val = pmic_config_interface(PMIC_CLK_AUXADC_CK_PDN_ADDR,0,PMIC_CLK_AUXADC_CK_PDN_MASK,PMIC_CLK_AUXADC_CK_PDN_SHIFT);
/*	ret_val = pmic_config_interface(MT6351_PMIC_RG_AUXADC_CK_PDN_HWEN_ADDR,0,MT6351_PMIC_RG_AUXADC_CK_PDN_HWEN_MASK,MT6351_PMIC_RG_AUXADC_CK_PDN_HWEN_SHIFT);*/

	ret_val = pmic_config_interface(PMIC_CLK_AUXADC_CK_CKSEL_ADDR,0,PMIC_CLK_AUXADC_CK_CKSEL_MASK,PMIC_CLK_AUXADC_CK_CKSEL_SHIFT);
/*	ret_val = pmic_config_interface(MT6351_PMIC_RG_AUXADC_CK_CKSEL_HWEN_ADDR,0,MT6351_PMIC_RG_AUXADC_CK_CKSEL_HWEN_MASK,MT6351_PMIC_RG_AUXADC_CK_CKSEL_HWEN_SHIFT);*/

	ret_val = pmic_config_interface(PMIC_CLK_AUXADC_SMPS_CK_PDN_ADDR,0,PMIC_CLK_AUXADC_SMPS_CK_PDN_MASK,PMIC_CLK_AUXADC_SMPS_CK_PDN_SHIFT);
#if MT6351
	ret_val = pmic_config_interface(PMIC_TOP_CKHWEN_CON0_CLR_ADDR,0x818,PMIC_TOP_CKHWEN_CON0_CLR_MASK,PMIC_TOP_CKHWEN_CON0_CLR_SHIFT);
#endif /* MT6351 */
	return ret_val;
}
int PMIC_MD_INIT_SETTING_V1(void)
{
	unsigned int ret = 0;
	unsigned int pmic_reg = 0;

#if PMIC_32K_LESS_DETECT_V1

	/* 32k less crystal auto detect start */
	ret |= pmic_config_interface(0x701E, 0x1, 0x1, 0);
	ret |= pmic_config_interface(0x701E, 0x3, 0xF, 1);
	ret = pmic_read_interface(0x7000, &pmic_reg, 0xffff, 0);
	ret |= pmic_config_interface(0x701E, 0x0, 0x1, 0);
	ret = pmic_config_interface(0xA04, 0x1, 0x1, 3);
	if ((pmic_reg & 0x200) == 0x200) {
		/* VCTCXO on MT6176 , OFF XO on MT6351
		    HW control, use srclken_0 */

		ret = pmic_config_interface(0xA04, 0x0, 0x7, 11);
		print("[PMIC_PRELOADER] VCTCXO on MT6176 , OFF XO on MT6353\n");
	} else {
		ret = pmic_config_interface(PMIC_RG_CLKSQ_EN_AUX_AP_MODE_ADDR,0x1,
			PMIC_RG_CLKSQ_EN_AUX_AP_MODE_MASK,
			PMIC_RG_CLKSQ_EN_AUX_AP_MODE_SHIFT);
		/*  HW control, use srclken_1, for LP */
		ret = pmic_config_interface(0xA04, 0x1, 0x1, 4);
		ret = pmic_config_interface(0xA04, 0x1, 0x7, 11);
		print("[PMIC_PRELOADER] VCTCXO TSX on MT6353 0x7000=0x%x\n", pmic_reg);
	}
#endif
	return ret;
}
//==============================================================================
// PMIC Init Code
//==============================================================================
U32 pmic_init (void)
{
    U32 ret_code = PMIC_TEST_PASS;
    int ret_val=0, val;

    print("[PMIC_PRELOADER] Preloader Start..................\n");
    print("[PMIC_PRELOADER] MT6353 CHIP Code = 0x%x\n", get_MT6353_PMIC_chip_version());

    /*1.UVLO off*/
    print("[PMIC_PRELOADER][pmic_status] Reg[0x%x]=0x%x\n", MT6353_TOP_RST_STATUS, upmu_get_reg_value(MT6353_TOP_RST_STATUS));
    /*2.thermal shutdown 150*/
    print("[PMIC_PRELOADER][pmic_status] Reg[0x%x]=0x%x\n", MT6353_THERMALSTATUS, upmu_get_reg_value(MT6353_THERMALSTATUS));
    /*3.power not good*/
    print("[PMIC_PRELOADER][pmic_status] Reg[0x%x]=0x%x\n", MT6353_PGSTATUS0, upmu_get_reg_value(MT6353_PGSTATUS0));
    /*4.buck oc*/
    print("[PMIC_PRELOADER][pmic_status] Reg[0x%x]=0x%x\n", MT6353_OCSTATUS1, upmu_get_reg_value(MT6353_OCSTATUS1));
    print("[PMIC_PRELOADER][pmic_status] Reg[0x%x]=0x%x\n", MT6353_OCSTATUS2, upmu_get_reg_value(MT6353_OCSTATUS2));
    /*5.long press shutdown*/
    print("[PMIC_PRELOADER][pmic_status] Reg[0x%x]=0x%x\n", MT6353_STRUP_CON4, upmu_get_reg_value(MT6353_STRUP_CON4));
    /*6.WDTRST*/
    print("[PMIC_PRELOADER][pmic_status] Reg[0x%x]=0x%x\n", MT6353_TOP_RST_MISC, upmu_get_reg_value(MT6353_TOP_RST_MISC));
    /*7.CLK TRIM*/
    print("[PMIC_PRELOADER][pmic_status] Reg[0x%x]=0x%x\n", 0x250, upmu_get_reg_value(0x250));

    /* ask shin-shyu programming guide TBD */
    ret_val=pmic_config_interface(PMIC_RG_ULC_DET_EN_ADDR,1,PMIC_RG_ULC_DET_EN_MASK,PMIC_RG_ULC_DET_EN_SHIFT);

	/* for auxadc init */
	PMIC_AUXADC_INIT_SETTING_V1();
	/*pmic_auxadc_debug(1);*/
	PMIC_MD_INIT_SETTING_V1();
	/* detect V battery Drop   used by RTC to check power on reason  */
	/* ask waverly programming guide */
	pmic_DetectVbatDrop();

	/* ask shin-shyu programming guide TBD */
	if(hw_check_battery()==1)
	{
		pmic_disable_usbdl_wo_battery();
	}


    pmic_efuse_management();

    #if 1
    //Enable PMIC RST function (depends on main chip RST function)
    /* ask peter programming guide TBD */
    ret_val=pmic_config_interface(MT6353_TOP_RST_MISC_CLR, 0x0002, 0xFFFF, 0); //[1]=0, RG_WDTRSTB_MODE
    ret_val=pmic_config_interface(MT6353_TOP_RST_MISC_SET, 0x0001, 0xFFFF, 0); //[0]=1, RG_WDTRSTB_EN
    print("[PMIC_PRELOADER][pmic_init] Reg[0x%x]=0x%x\n", MT6353_TOP_RST_MISC, upmu_get_reg_value(MT6353_TOP_RST_MISC));
    #endif


    /* ask peter peng programming guide TBD */
    ret_val= pmic_config_interface(PMIC_RG_SMPS_TESTMODE_B_ADDR, 0x0001, PMIC_RG_SMPS_TESTMODE_B_MASK, PMIC_RG_SMPS_TESTMODE_B_SHIFT); //RG_SMPS_TESTMODE_B by luke

	 mt6311_driver_probe();

#if MT6351
	/* ask johnson programming guide TBD */
	if(is_mt6311_exist()==1)
	{
#if defined MT6328
/*
		1.	Vproc 1.0V -> 1.1V
		2.	Delay 100us
		3.	Vsram 1.2V -> 1.51V
		4.	Delay 100us  20us
		5.	Vproc 1.1V -> 1.15V
		6.	Delay 100us

		MT6311
		Voltage = 0.7+0.00651*step
		(0.7V~1.49375V)
*/
		//1.VPROC 1.0v -> 1.1v
		mt6311_config_interface(0x8D,0x40,0x7F,0); // [6:0]: VDVFS11_VOSEL; Setting by lower power ,20150305, Johsnon,_1.15V forD3T
		mt6311_config_interface(0x8E,0x40,0x7F,0); // [6:0]: VDVFS11_VOSEL_ON; Setting by lower power ,20150305, Johsnon,_1.15V forD3T
		//2.delay 100us
		udelay(100);
		//3.VSRAM 1.2v -> 1.51v
		upmu_set_reg_value(0x4b4,0x68); //vsram 1.51v
		upmu_set_reg_value(0xA88,0x68); //vsram 1.51v
		//4.delay 20us
		udelay(20);
		//5.VPROC 1.1V->1.15V
		mt6311_config_interface(0x8D,0x58,0x7F,0); // [6:0]: VDVFS11_VOSEL; Setting by lower power ,20150305, Johsnon,_1.15V forD3T
		mt6311_config_interface(0x8E,0x58,0x7F,0); // [6:0]: VDVFS11_VOSEL_ON; Setting by lower power ,20150305, Johsnon,_1.15V forD3T
		udelay(100);
#endif
		/* need to add jade e2 for MD
		<<    Default voltage >>
		Power Domain 	JADE E1 	JADE E2
		Vdd_sram_md 	1.0V 	1.0V
		Vdd_md1 	0.9V 	0.9V
		Vdd_modem 	1.0V 	0.9V

		<<    change before PLL >>
		Power Domain 	JADE E1  	JADE E2
		Vdd_sram_md 	1.0V 	1.1V
		Vdd_md1 	0.9V 	0.9V
		Vdd_modem 	1.0V 	1.0V
		*/

		if (mt_get_chip_sw_ver() == CHIP_SW_VER_02) {
			/*3.VSRAM_MD 1v -> 1.1v*/
			buck_adjust_voltage(VSRAM_MD, 1100000);
			/*upmu_set_reg_value(0x658,0x50);*/
			/*3.VMODEM 0.9v -> 1v*/
			buck_adjust_voltage(VMODEM, 1000000);
			/*upmu_set_reg_value(0x630,0x40);*/
		}
	}
#endif /* MT6351 */

	if(hw_check_battery()==1)
	{
#if !CFG_EVB_PLATFORM
		pl_check_bat_protect_status();
#endif
	}
   	/*ret_val=pmic_read_interface( (U32)(0x04de),
                           (&val),
                           (0xffff),
                           0);
	*/

	/*--- Vcore voltage by segment ---*/
	buck_vcore_segment_voltage();

	/*ret_val=pmic_config_interface(0x0A2E, 0x0, 0x1, 1);*/ // Disable RG_VMCH_EN
	print("[PMIC_PRELOADER] [pmic_init] Done...................\n");
	/*pmic_auxadc_debug(2);*/
    return ret_code;
}


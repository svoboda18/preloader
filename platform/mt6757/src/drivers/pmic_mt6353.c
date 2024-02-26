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
    U32 rdata;

    //mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        print("[PMIC_PL] Reg[%x]= pmic_wrap read data fail\n", RegNum);
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
	U32 val;
	U32 ret;

	ret=pmic_read_interface (RegNum,&val,MASK,SHIFT);

	return val;
}

U32 pmic_config_interface (U32 RegNum, U32 val, U32 MASK, U32 SHIFT)
{
    U32 return_value = 0;
    U32 pmic_reg = 0;
    U32 rdata;

    //1. mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        print("[PMIC_PL] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    //print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

    pmic_reg &= ~(MASK << SHIFT);
    pmic_reg |= (val << SHIFT);

    //2. mt_write_byte(RegNum, pmic_reg);
    return_value= pwrap_wacs2(1, (RegNum), pmic_reg, &rdata);
    if(return_value!=0)
    {
        print("[PMIC_PL] Reg[%x]= pmic_wrap read data fail\n", RegNum);
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
        print("[PMIC_PL] pl pmic powerkey Release\n");
        return 0;
    }else{
        print("[PMIC_PL] pl pmic powerkey Press\n");
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
        print("[PMIC_PL] pl pmic FCHRKEY Release\n");
        return 0;
    }else{
        print("[PMIC_PL] pl pmic FCHRKEY Press\n");
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
    print("[PMIC_PL] pmic_IsUsbCableIn %d\n", val);
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
#endif
#else
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6353_TOP_RST_MISC, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
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
	kal_uint32 i,j;
	kal_uint32 ret,reg_val;

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
#if MT6351
    print("[0x%x]=0x%x ", 0x0AC6, upmu_get_reg_value(0x0AC6));
    print("[0x%x]=0x%x ", 0x0ACC, upmu_get_reg_value(0x0ACC));
    print("[0x%x]=0x%x ", 0x0ACA, upmu_get_reg_value(0x0ACA));
    print("[0x%x]=0x%x \n", 0x0AD6, upmu_get_reg_value(0x0AD6));

    print("[0x%x]=0x%x ", 0x0B26, upmu_get_reg_value(0x0B26));
    print("[0x%x]=0x%x ", 0x0AE6, upmu_get_reg_value(0x0AE6));
    print("[0x%x]=0x%x ", 0x0AEE, upmu_get_reg_value(0x0AEE));
    print("[0x%x]=0x%x \n", 0x0AF2, upmu_get_reg_value(0x0AF2));

    print("[0x%x]=0x%x ", 0x0AF6, upmu_get_reg_value(0x0AF6));
    print("[0x%x]=0x%x ", 0x0B18, upmu_get_reg_value(0x0B18));
    print("[0x%x]=0x%x \n", 0x0B14, upmu_get_reg_value(0x0B14));

    print("[0x%x]=0x%x ", 0x0B1C, upmu_get_reg_value(0x0B1C));
    print("[0x%x]=0x%x ", 0x0B08, upmu_get_reg_value(0x0B08));
    print("[0x%x]=0x%x \n", 0x0AFC, upmu_get_reg_value(0x0AFC));

    print("[0x%x]=0x%x ", 0x0B0C, upmu_get_reg_value(0x0B0C));
    print("[0x%x]=0x%x ", 0x0B00, upmu_get_reg_value(0x0B00));
    print("[0x%x]=0x%x \n", 0x0ADA, upmu_get_reg_value(0x0ADA));

    print("[0x%x]=0x%x ", 0x0B28, upmu_get_reg_value(0x0B28));
    print("[0x%x]=0x%x ", 0x0AF8, upmu_get_reg_value(0x0AF8));
    print("[0x%x]=0x%x \n", 0x0AF0, upmu_get_reg_value(0x0AF0));

    print("[0x%x]=0x%x ", 0x0AF4, upmu_get_reg_value(0x0AF4));
    print("[0x%x]=0x%x ", 0x0B02, upmu_get_reg_value(0x0B02));
    print("[0x%x]=0x%x \n", 0x043A, upmu_get_reg_value(0x043A));

    print("[0x%x]=0x%x ", 0x04DE, upmu_get_reg_value(0x04DE));
    print("[0x%x]=0x%x ", 0x04CE, upmu_get_reg_value(0x04CE));
    print("[0x%x]=0x%x ", 0x046A, upmu_get_reg_value(0x046A));

    print("[0x%x]=0x%x ", 0x04BA, upmu_get_reg_value(0x04BA));
    print("[0x%x]=0x%x ", 0x0456, upmu_get_reg_value(0x0456));
    print("[0x%x]=0x%x \n", 0x04A6, upmu_get_reg_value(0x04A6));

    print("[0x%x]=0x%x ", 0x047E, upmu_get_reg_value(0x047E));
    print("[0x%x]=0x%x ", 0x0492, upmu_get_reg_value(0x0492));
    print("[0x%x]=0x%x \n", 0x04DE, upmu_get_reg_value(0x04DE));

    print("[0x%x]=0x%x ", 0x0448, upmu_get_reg_value(0x0448));
    print("[0x%x]=0x%x ", 0x043E, upmu_get_reg_value(0x043E));
    print("[0x%x]=0x%x \n", 0x0446, upmu_get_reg_value(0x0446));

    print("[0x%x]=0x%x \n", 0x043C, upmu_get_reg_value(0x043C));

    print("[0x%x]=0x%x ", 0x0444, upmu_get_reg_value(0x0444));
    print("[0x%x]=0x%x ", 0x0440, upmu_get_reg_value(0x0440));
    print("[0x%x]=0x%x ", 0x0442, upmu_get_reg_value(0x0442));
    print("[0x%x]=0x%x \n", 0x044A, upmu_get_reg_value(0x044A));

    print("[0x%x]=0x%x ", 0x04D6, upmu_get_reg_value(0x04D6));
    print("[0x%x]=0x%x ", 0x0472, upmu_get_reg_value(0x0472));
    print("[0x%x]=0x%x ", 0x04C2, upmu_get_reg_value(0x04C2));
    print("[0x%x]=0x%x \n", 0x045E, upmu_get_reg_value(0x045E));

    print("[0x%x]=0x%x \n", 0x04AE, upmu_get_reg_value(0x04AE));
    print("[0x%x]=0x%x \n", 0x0486, upmu_get_reg_value(0x0486));
    print("[0x%x]=0x%x \n", 0x049A, upmu_get_reg_value(0x049A));

    print("[0x%x]=0x%x ", 0x0456, upmu_get_reg_value(0x0456));
    print("[0x%x]=0x%x ", 0x0454, upmu_get_reg_value(0x0454));
    print("[0x%x]=0x%x ", 0x046A, upmu_get_reg_value(0x046A));
    print("[0x%x]=0x%x \n", 0x0468, upmu_get_reg_value(0x0468));
#endif
}

void pmic_efuse_check_bit(void)
{
	U32 val_reg=0;
	print("pmic_efuse_check_bit\n");

#if MT6351
pmic_read_interface(0x0AC6,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0AC6,0  ,val_reg,(efuse_data[0x5 ]>>9 )&1);
pmic_read_interface(0x0AC6,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0AC6,1  ,val_reg,(efuse_data[0x5 ]>>10)&1);
pmic_read_interface(0x0AC6,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0AC6,2  ,val_reg,(efuse_data[0x5 ]>>11)&1);
pmic_read_interface(0x0AC6,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0AC6,3  ,val_reg,(efuse_data[0x5 ]>>12)&1);
pmic_read_interface(0x0ACC,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0ACC,0  ,val_reg,(efuse_data[0x5 ]>>13)&1);
pmic_read_interface(0x0ACC,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0ACC,1  ,val_reg,(efuse_data[0x5 ]>>14)&1);
pmic_read_interface(0x0ACC,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0ACC,2  ,val_reg,(efuse_data[0x5 ]>>15)&1);
pmic_read_interface(0x0ACC,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0ACC,3  ,val_reg,(efuse_data[0x6 ]>>0 )&1);
pmic_read_interface(0x0ACA,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0ACA,0  ,val_reg,(efuse_data[0x6 ]>>1 )&1);
pmic_read_interface(0x0ACA,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0ACA,1  ,val_reg,(efuse_data[0x6 ]>>2 )&1);
pmic_read_interface(0x0ACA,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0ACA,2  ,val_reg,(efuse_data[0x6 ]>>3  )&1);
pmic_read_interface(0x0ACA,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0ACA,3  ,val_reg,(efuse_data[0x6 ]>>4  )&1);
pmic_read_interface(0x0AD6,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0AD6,0  ,val_reg,(efuse_data[0x6 ]>>5  )&1);
pmic_read_interface(0x0AD6,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0AD6,1  ,val_reg,(efuse_data[0x6 ]>>6  )&1);
pmic_read_interface(0x0AD6,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0AD6,2  ,val_reg,(efuse_data[0x6 ]>>7  )&1);
pmic_read_interface(0x0AD6,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0AD6,3  ,val_reg,(efuse_data[0x6 ]>>8  )&1);
pmic_read_interface(0x0B26,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0B26,0  ,val_reg,(efuse_data[0x6 ]>>9 )&1);
pmic_read_interface(0x0B26,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0B26,1  ,val_reg,(efuse_data[0x6 ]>>10)&1);
pmic_read_interface(0x0B26,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0B26,2  ,val_reg,(efuse_data[0x6 ]>>11)&1);
pmic_read_interface(0x0B26,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0B26,3  ,val_reg,(efuse_data[0x6 ]>>12)&1);
pmic_read_interface(0x0B26,&val_reg,0x1,4 );print("[0x%x:%d]:%d =%d  \n", 0x0B26,4  ,val_reg,(efuse_data[0x6 ]>>13)&1);
pmic_read_interface(0x0AE6,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0AE6,0  ,val_reg,(efuse_data[0x6 ]>>14)&1);
pmic_read_interface(0x0AE6,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0AE6,1  ,val_reg,(efuse_data[0x6 ]>>15)&1);
pmic_read_interface(0x0AE6,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0AE6,2  ,val_reg,(efuse_data[0x7 ]>>0 )&1);
pmic_read_interface(0x0AE6,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0AE6,3  ,val_reg,(efuse_data[0x7 ]>>1 )&1);
pmic_read_interface(0x0AEE,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0AEE,0  ,val_reg,(efuse_data[0x7 ]>>2 )&1);
pmic_read_interface(0x0AEE,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0AEE,1  ,val_reg,(efuse_data[0x7 ]>>3  )&1);
pmic_read_interface(0x0AEE,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0AEE,2  ,val_reg,(efuse_data[0x7 ]>>4  )&1);
pmic_read_interface(0x0AEE,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0AEE,3  ,val_reg,(efuse_data[0x7 ]>>5  )&1);
pmic_read_interface(0x0AF2,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0AF2,0  ,val_reg,(efuse_data[0x7 ]>>6  )&1);
pmic_read_interface(0x0AF2,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0AF2,1  ,val_reg,(efuse_data[0x7 ]>>7  )&1);
pmic_read_interface(0x0AF2,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0AF2,2  ,val_reg,(efuse_data[0x7 ]>>8  )&1);
pmic_read_interface(0x0AF2,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0AF2,3  ,val_reg,(efuse_data[0x7 ]>>9 )&1);
pmic_read_interface(0x0AF6,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0AF6,0  ,val_reg,(efuse_data[0x7 ]>>10)&1);
pmic_read_interface(0x0AF6,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0AF6,1  ,val_reg,(efuse_data[0x7 ]>>11)&1);
pmic_read_interface(0x0AF6,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0AF6,2  ,val_reg,(efuse_data[0x7 ]>>12)&1);
pmic_read_interface(0x0AF6,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0AF6,3  ,val_reg,(efuse_data[0x7 ]>>13)&1);
pmic_read_interface(0x0B18,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0B18,0  ,val_reg,(efuse_data[0x7 ]>>14)&1);
pmic_read_interface(0x0B18,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0B18,1  ,val_reg,(efuse_data[0x7 ]>>15)&1);
pmic_read_interface(0x0B18,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0B18,2  ,val_reg,(efuse_data[0x8 ]>>0 )&1);
pmic_read_interface(0x0B18,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0B18,3  ,val_reg,(efuse_data[0x8 ]>>1 )&1);
pmic_read_interface(0x0B14,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0B14,0  ,val_reg,(efuse_data[0x8 ]>>2 )&1);
pmic_read_interface(0x0B14,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0B14,1  ,val_reg,(efuse_data[0x8 ]>>3  )&1);
pmic_read_interface(0x0B14,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0B14,2  ,val_reg,(efuse_data[0x8 ]>>4  )&1);
pmic_read_interface(0x0B14,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0B14,3  ,val_reg,(efuse_data[0x8 ]>>5  )&1);
pmic_read_interface(0x0B1C,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0B1C,0  ,val_reg,(efuse_data[0x8 ]>>6  )&1);
pmic_read_interface(0x0B1C,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0B1C,1  ,val_reg,(efuse_data[0x8 ]>>7  )&1);
pmic_read_interface(0x0B1C,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0B1C,2  ,val_reg,(efuse_data[0x8 ]>>8  )&1);
pmic_read_interface(0x0B1C,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0B1C,3  ,val_reg,(efuse_data[0x8 ]>>9 )&1);
pmic_read_interface(0x0B08,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0B08,0  ,val_reg,(efuse_data[0x8 ]>>10)&1);
pmic_read_interface(0x0B08,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0B08,1  ,val_reg,(efuse_data[0x8 ]>>11)&1);
pmic_read_interface(0x0B08,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0B08,2  ,val_reg,(efuse_data[0x8 ]>>12)&1);
pmic_read_interface(0x0B08,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0B08,3  ,val_reg,(efuse_data[0x8 ]>>13)&1);
pmic_read_interface(0x0AFC,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0AFC,0  ,val_reg,(efuse_data[0x8 ]>>14)&1);
pmic_read_interface(0x0AFC,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0AFC,1  ,val_reg,(efuse_data[0x8 ]>>15)&1);
pmic_read_interface(0x0AFC,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0AFC,2  ,val_reg,(efuse_data[0x9 ]>>0 )&1);
pmic_read_interface(0x0AFC,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0AFC,3  ,val_reg,(efuse_data[0x9 ]>>1 )&1);
pmic_read_interface(0x0B0C,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0B0C,0  ,val_reg,(efuse_data[0x9 ]>>2 )&1);
pmic_read_interface(0x0B0C,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0B0C,1  ,val_reg,(efuse_data[0x9 ]>>3  )&1);
pmic_read_interface(0x0B0C,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0B0C,2  ,val_reg,(efuse_data[0x9 ]>>4  )&1);
pmic_read_interface(0x0B0C,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0B0C,3  ,val_reg,(efuse_data[0x9 ]>>5  )&1);
pmic_read_interface(0x0B00,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0B00,0  ,val_reg,(efuse_data[0x9 ]>>6  )&1);
pmic_read_interface(0x0B00,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0B00,1  ,val_reg,(efuse_data[0x9 ]>>7  )&1);
pmic_read_interface(0x0B00,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0B00,2  ,val_reg,(efuse_data[0x9 ]>>8  )&1);
pmic_read_interface(0x0B00,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0B00,3  ,val_reg,(efuse_data[0x9 ]>>9 )&1);
pmic_read_interface(0x0B00,&val_reg,0x1,4 );print("[0x%x:%d]:%d =%d  \n", 0x0B00,4  ,val_reg,(efuse_data[0x9 ]>>10 )&1);
/*
//pmic_read_interface(RSV   ,&val_reg,0x1,6);print("[0x%x:%d]:%d =%d  \n", 0RSV   66) ,val_reg,(efuse_data[4 ]>>2 )&1);
//pmic_read_interface(RSV   ,&val_reg,0x1,7);print("[0x%x:%d]:%d =%d  \n", 0RSV   77) ,val_reg,(efuse_data[4 ]>>3 )&1);
//pmic_read_interface(RSV   ,&val_reg,0x1,8);print("[0x%x:%d]:%d =%d  \n", 0RSV   88) ,val_reg,(efuse_data[4 ]>>4 )&1);
//pmic_read_interface(RSV   ,&val_reg,0x1,9);print("[0x%x:%d]:%d =%d  \n", 0RSV   99) ,val_reg,(efuse_data[4 ]>>5 )&1);
//pmic_read_interface(RSV   ,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", RSV   ,10 ,val_reg,(efuse_data[4 ]>>6 )&1);
*/
pmic_read_interface(0x0ADA,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0ADA,0  ,val_reg,(efuse_data[0xa ]>>0 )&1);
pmic_read_interface(0x0ADA,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0ADA,1  ,val_reg,(efuse_data[0xa ]>>1 )&1);
pmic_read_interface(0x0ADA,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0ADA,2  ,val_reg,(efuse_data[0xa ]>>2 )&1);
pmic_read_interface(0x0ADA,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0ADA,3  ,val_reg,(efuse_data[0xa ]>>3  )&1);
pmic_read_interface(0x0ADA,&val_reg,0x1,4 );print("[0x%x:%d]:%d =%d  \n", 0x0ADA,4  ,val_reg,(efuse_data[0xa ]>>4  )&1);
pmic_read_interface(0x0B28,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0B28,2  ,val_reg,(efuse_data[0xa ]>>5  )&1);
pmic_read_interface(0x0B28,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0B28,3  ,val_reg,(efuse_data[0xa ]>>6  )&1);
pmic_read_interface(0x0B28,&val_reg,0x1,4 );print("[0x%x:%d]:%d =%d  \n", 0x0B28,4  ,val_reg,(efuse_data[0xa ]>>7  )&1);
pmic_read_interface(0x0B28,&val_reg,0x1,5 );print("[0x%x:%d]:%d =%d  \n", 0x0B28,5  ,val_reg,(efuse_data[0xa ]>>8  )&1);
pmic_read_interface(0x0B28,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x0B28,6  ,val_reg,(efuse_data[0xa ]>>9 )&1);
pmic_read_interface(0x0B28,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x0B28,7  ,val_reg,(efuse_data[0xa ]>>10)&1);
pmic_read_interface(0x0AF8,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x0AF8,9  ,val_reg,(efuse_data[0xa ]>>11)&1);
pmic_read_interface(0x0AF8,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0AF8,10 ,val_reg,(efuse_data[0xa ]>>12)&1);
pmic_read_interface(0x0AF8,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0AF8,11 ,val_reg,(efuse_data[0xa ]>>13)&1);
pmic_read_interface(0x0AF8,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0AF8,12 ,val_reg,(efuse_data[0xa ]>>14)&1);
pmic_read_interface(0x0AF0,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x0AF0,9  ,val_reg,(efuse_data[0xa ]>>15)&1);
pmic_read_interface(0x0AF0,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0AF0,10 ,val_reg,(efuse_data[0xb ]>>0 )&1);
pmic_read_interface(0x0AF0,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0AF0,11 ,val_reg,(efuse_data[0xb ]>>1 )&1);
pmic_read_interface(0x0AF0,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0AF0,12 ,val_reg,(efuse_data[0xb ]>>2 )&1);
pmic_read_interface(0x0AF4,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x0AF4,9  ,val_reg,(efuse_data[0xb ]>>3  )&1);
pmic_read_interface(0x0AF4,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0AF4,10 ,val_reg,(efuse_data[0xb ]>>4  )&1);
pmic_read_interface(0x0AF4,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0AF4,11 ,val_reg,(efuse_data[0xb ]>>5  )&1);
pmic_read_interface(0x0AF4,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0AF4,12 ,val_reg,(efuse_data[0xb ]>>6  )&1);
pmic_read_interface(0x0B02,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0B02,10 ,val_reg,(efuse_data[0xb ]>>7  )&1);
pmic_read_interface(0x0B02,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0B02,11 ,val_reg,(efuse_data[0xb ]>>8  )&1);
pmic_read_interface(0x0B02,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0B02,12 ,val_reg,(efuse_data[0xb ]>>9 )&1);
pmic_read_interface(0x0B02,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x0B02,13 ,val_reg,(efuse_data[0xb ]>>10)&1);
pmic_read_interface(0x043A,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x043A,0  ,val_reg,(efuse_data[0xb ]>>11)&1);
pmic_read_interface(0x043A,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x043A,1  ,val_reg,(efuse_data[0xb ]>>12)&1);
pmic_read_interface(0x043A,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x043A,2  ,val_reg,(efuse_data[0xb ]>>13)&1);
pmic_read_interface(0x043A,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x043A,3  ,val_reg,(efuse_data[0xb ]>>14)&1);
pmic_read_interface(0x043A,&val_reg,0x1,4 );print("[0x%x:%d]:%d =%d  \n", 0x043A,4  ,val_reg,(efuse_data[0xb ]>>15)&1);
pmic_read_interface(0x043A,&val_reg,0x1,5 );print("[0x%x:%d]:%d =%d  \n", 0x043A,5  ,val_reg,(efuse_data[0xc ]>>0 )&1);
pmic_read_interface(0x043A,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x043A,6  ,val_reg,(efuse_data[0xc ]>>1 )&1);
pmic_read_interface(0x043A,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x043A,7  ,val_reg,(efuse_data[0xc ]>>2 )&1);
pmic_read_interface(0x043A,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x043A,8  ,val_reg,(efuse_data[0xc ]>>3  )&1);
pmic_read_interface(0x043A,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x043A,9  ,val_reg,(efuse_data[0xc ]>>4  )&1);
pmic_read_interface(0x043A,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x043A,10 ,val_reg,(efuse_data[0xc ]>>5  )&1);
pmic_read_interface(0x043A,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x043A,11 ,val_reg,(efuse_data[0xc ]>>6  )&1);
pmic_read_interface(0x043A,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x043A,12 ,val_reg,(efuse_data[0xc ]>>7  )&1);
pmic_read_interface(0x043A,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x043A,13 ,val_reg,(efuse_data[0xc ]>>8  )&1);
pmic_read_interface(0x043A,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x043A,14 ,val_reg,(efuse_data[0xc ]>>9 )&1);
pmic_read_interface(0x04DE,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,0  ,val_reg,(efuse_data[0xc ]>>10)&1);
pmic_read_interface(0x04DE,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,1  ,val_reg,(efuse_data[0xc ]>>11)&1);
pmic_read_interface(0x04DE,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,2  ,val_reg,(efuse_data[0xc ]>>12)&1);
pmic_read_interface(0x04DE,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,3  ,val_reg,(efuse_data[0xc ]>>13)&1);
pmic_read_interface(0x04DE,&val_reg,0x1,4 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,4  ,val_reg,(efuse_data[0xc ]>>14)&1);
pmic_read_interface(0x04DE,&val_reg,0x1,5 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,5  ,val_reg,(efuse_data[0xc ]>>15)&1);
pmic_read_interface(0x04DE,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,6  ,val_reg,(efuse_data[0xd ]>>0 )&1);
pmic_read_interface(0x04DE,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,7  ,val_reg,(efuse_data[0xd ]>>1 )&1);
pmic_read_interface(0x04CE,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x04CE,6  ,val_reg,(efuse_data[0xd ]>>2 )&1);
pmic_read_interface(0x04CE,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x04CE,7  ,val_reg,(efuse_data[0xd ]>>3  )&1);
pmic_read_interface(0x04CE,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x04CE,8  ,val_reg,(efuse_data[0xd ]>>4  )&1);
pmic_read_interface(0x04CE,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x04CE,9  ,val_reg,(efuse_data[0xd ]>>5  )&1);
pmic_read_interface(0x046A,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x046A,12 ,val_reg,(efuse_data[0xd ]>>6  )&1);
pmic_read_interface(0x046A,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x046A,13 ,val_reg,(efuse_data[0xd ]>>7  )&1);
pmic_read_interface(0x046A,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x046A,14 ,val_reg,(efuse_data[0xd ]>>8  )&1);
pmic_read_interface(0x046A,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x046A,15 ,val_reg,(efuse_data[0xd ]>>9 )&1);
pmic_read_interface(0x04BA,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x04BA,6  ,val_reg,(efuse_data[0xd ]>>10)&1);
pmic_read_interface(0x04BA,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x04BA,7  ,val_reg,(efuse_data[0xd ]>>11)&1);
pmic_read_interface(0x04BA,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x04BA,8  ,val_reg,(efuse_data[0xd ]>>12)&1);
pmic_read_interface(0x04BA,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x04BA,9  ,val_reg,(efuse_data[0xd ]>>13)&1);
pmic_read_interface(0x0456,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0456,12 ,val_reg,(efuse_data[0xd ]>>14)&1);
pmic_read_interface(0x0456,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x0456,13 ,val_reg,(efuse_data[0xd ]>>15)&1);
pmic_read_interface(0x0456,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x0456,14 ,val_reg,(efuse_data[0xe ]>>0 )&1);
pmic_read_interface(0x0456,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x0456,15 ,val_reg,(efuse_data[0xe ]>>1 )&1);
pmic_read_interface(0x04A6,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x04A6,6  ,val_reg,(efuse_data[0xe ]>>2 )&1);
pmic_read_interface(0x04A6,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x04A6,7  ,val_reg,(efuse_data[0xe ]>>3  )&1);
pmic_read_interface(0x04A6,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x04A6,8  ,val_reg,(efuse_data[0xe ]>>4  )&1);
pmic_read_interface(0x04A6,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x04A6,9  ,val_reg,(efuse_data[0xe ]>>5  )&1);
pmic_read_interface(0x047E,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x047E,6  ,val_reg,(efuse_data[0xe ]>>6 )&1);
pmic_read_interface(0x047E,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x047E,7  ,val_reg,(efuse_data[0xe ]>>7 )&1);
pmic_read_interface(0x047E,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x047E,8  ,val_reg,(efuse_data[0xe ]>>8 )&1);
pmic_read_interface(0x047E,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x047E,9  ,val_reg,(efuse_data[0xe ]>>9  )&1);
pmic_read_interface(0x0492,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x0492,6  ,val_reg,(efuse_data[0xe ]>>10 )&1);
pmic_read_interface(0x0492,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x0492,7  ,val_reg,(efuse_data[0xe ]>>11 )&1);
pmic_read_interface(0x0492,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x0492,8  ,val_reg,(efuse_data[0xe ]>>12 )&1);
pmic_read_interface(0x0492,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x0492,9  ,val_reg,(efuse_data[0xe ]>>13 )&1);
pmic_read_interface(0x04DE,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,8  ,val_reg,(efuse_data[0xe ]>>14 )&1);
pmic_read_interface(0x04DE,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x04DE,9  ,val_reg,(efuse_data[0xe ]>>15)&1);
pmic_read_interface(0x04DE,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x04DE,10 ,val_reg,(efuse_data[0xf ]>>0 )&1);
pmic_read_interface(0x04DE,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x04DE,11 ,val_reg,(efuse_data[0xf ]>>1 )&1);
pmic_read_interface(0x0448,&val_reg,0x1,5 );print("[0x%x:%d]:%d =%d  \n", 0x0448,5  ,val_reg,(efuse_data[0xf ]>>2 )&1);
pmic_read_interface(0x0448,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x0448,6  ,val_reg,(efuse_data[0xf ]>>3 )&1);
pmic_read_interface(0x0448,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x0448,7  ,val_reg,(efuse_data[0xf ]>>4 )&1);
pmic_read_interface(0x043E,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x043E,10 ,val_reg,(efuse_data[0xf ]>>5 )&1);
pmic_read_interface(0x043E,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x043E,11 ,val_reg,(efuse_data[0xf ]>>6 )&1);
pmic_read_interface(0x043E,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x043E,12 ,val_reg,(efuse_data[0xf ]>>7 )&1);
pmic_read_interface(0x0446,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0446,10 ,val_reg,(efuse_data[0xf ]>>8 )&1);
pmic_read_interface(0x0446,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0446,11 ,val_reg,(efuse_data[0xf ]>>9 )&1);
pmic_read_interface(0x0446,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0446,12 ,val_reg,(efuse_data[0xf ]>>10 )&1);
pmic_read_interface(0x043C,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x043C,10 ,val_reg,(efuse_data[0xf ]>>11 )&1);
pmic_read_interface(0x043C,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x043C,11 ,val_reg,(efuse_data[0xf ]>>12 )&1);
pmic_read_interface(0x043C,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x043C,12 ,val_reg,(efuse_data[0xf ]>>13 )&1);
pmic_read_interface(0x0444,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0444,10 ,val_reg,(efuse_data[0xf ]>>14 )&1);
pmic_read_interface(0x0444,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0444,11 ,val_reg,(efuse_data[0xf ]>>15 )&1);
pmic_read_interface(0x0444,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0444,12 ,val_reg,(efuse_data[0x10]>>0 )&1);
pmic_read_interface(0x0440,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0440,10 ,val_reg,(efuse_data[0x10]>>1 )&1);
pmic_read_interface(0x0440,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0440,11 ,val_reg,(efuse_data[0x10]>>2 )&1);
pmic_read_interface(0x0440,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0440,12 ,val_reg,(efuse_data[0x10]>>3 )&1);
pmic_read_interface(0x0442,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0442,10 ,val_reg,(efuse_data[0x10]>>4 )&1);
pmic_read_interface(0x0442,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0442,11 ,val_reg,(efuse_data[0x10]>>5 )&1);
pmic_read_interface(0x0442,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0442,12 ,val_reg,(efuse_data[0x10]>>6 )&1);
pmic_read_interface(0x044A,&val_reg,0x1,5 );print("[0x%x:%d]:%d =%d  \n", 0x044A,5  ,val_reg,(efuse_data[0x10]>>7 )&1);
pmic_read_interface(0x044A,&val_reg,0x1,6 );print("[0x%x:%d]:%d =%d  \n", 0x044A,6  ,val_reg,(efuse_data[0x10]>>8 )&1);
pmic_read_interface(0x044A,&val_reg,0x1,7 );print("[0x%x:%d]:%d =%d  \n", 0x044A,7  ,val_reg,(efuse_data[0x10]>>9 )&1);
pmic_read_interface(0x04D6,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x04D6,15 ,val_reg,(efuse_data[0x10]>>10 )&1);
pmic_read_interface(0x04D6,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x04D6,14 ,val_reg,(efuse_data[0x10]>>11 )&1);
pmic_read_interface(0x04D6,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x04D6,13 ,val_reg,(efuse_data[0x10]>>12 )&1);
pmic_read_interface(0x04D6,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x04D6,12 ,val_reg,(efuse_data[0x10]>>13)&1);
pmic_read_interface(0x04D6,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x04D6,11 ,val_reg,(efuse_data[0x10]>>14)&1);
pmic_read_interface(0x04D6,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x04D6,10 ,val_reg,(efuse_data[0x10]>>15)&1);
pmic_read_interface(0x04D6,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x04D6,9  ,val_reg,(efuse_data[0x11]>>0  )&1);
pmic_read_interface(0x04D6,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x04D6,8  ,val_reg,(efuse_data[0x11]>>1  )&1);
pmic_read_interface(0x0472,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x0472,15 ,val_reg,(efuse_data[0x11]>>2  )&1);
pmic_read_interface(0x0472,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x0472,14 ,val_reg,(efuse_data[0x11]>>3  )&1);
pmic_read_interface(0x0472,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x0472,13 ,val_reg,(efuse_data[0x11]>>4  )&1);
pmic_read_interface(0x0472,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0472,12 ,val_reg,(efuse_data[0x11]>>5  )&1);
pmic_read_interface(0x0472,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0472,11 ,val_reg,(efuse_data[0x11]>>6 )&1);
pmic_read_interface(0x0472,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0472,10 ,val_reg,(efuse_data[0x11]>>7 )&1);
pmic_read_interface(0x0472,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x0472,9  ,val_reg,(efuse_data[0x11]>>8 )&1);
pmic_read_interface(0x0472,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x0472,8  ,val_reg,(efuse_data[0x11]>>9 )&1);
pmic_read_interface(0x04C2,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x04C2,15 ,val_reg,(efuse_data[0x11]>>10)&1);
pmic_read_interface(0x04C2,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x04C2,14 ,val_reg,(efuse_data[0x11]>>11)&1);
pmic_read_interface(0x04C2,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x04C2,13 ,val_reg,(efuse_data[0x11]>>12)&1);
pmic_read_interface(0x04C2,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x04C2,12 ,val_reg,(efuse_data[0x11]>>13)&1);
pmic_read_interface(0x04C2,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x04C2,11 ,val_reg,(efuse_data[0x11]>>14)&1);
pmic_read_interface(0x04C2,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x04C2,10 ,val_reg,(efuse_data[0x11]>>15)&1);
pmic_read_interface(0x04C2,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x04C2,9  ,val_reg,(efuse_data[0x12]>>0 )&1);
pmic_read_interface(0x04C2,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x04C2,8  ,val_reg,(efuse_data[0x12]>>1  )&1);
pmic_read_interface(0x045E,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x045E,15 ,val_reg,(efuse_data[0x12]>>2  )&1);
pmic_read_interface(0x045E,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x045E,14 ,val_reg,(efuse_data[0x12]>>3  )&1);
pmic_read_interface(0x045E,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x045E,13 ,val_reg,(efuse_data[0x12]>>4  )&1);
pmic_read_interface(0x045E,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x045E,12 ,val_reg,(efuse_data[0x12]>>5  )&1);
pmic_read_interface(0x045E,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x045E,11 ,val_reg,(efuse_data[0x12]>>6  )&1);
pmic_read_interface(0x045E,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x045E,10 ,val_reg,(efuse_data[0x12]>>7 )&1);
pmic_read_interface(0x045E,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x045E,9  ,val_reg,(efuse_data[0x12]>>8 )&1);
pmic_read_interface(0x045E,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x045E,8  ,val_reg,(efuse_data[0x12]>>9 )&1);
pmic_read_interface(0x04AE,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x04AE,15 ,val_reg,(efuse_data[0x12]>>10)&1);
pmic_read_interface(0x04AE,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x04AE,14 ,val_reg,(efuse_data[0x12]>>11)&1);
pmic_read_interface(0x04AE,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x04AE,13 ,val_reg,(efuse_data[0x12]>>12)&1);
pmic_read_interface(0x04AE,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x04AE,12 ,val_reg,(efuse_data[0x12]>>13)&1);
pmic_read_interface(0x04AE,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x04AE,11 ,val_reg,(efuse_data[0x12]>>14)&1);
pmic_read_interface(0x04AE,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x04AE,10 ,val_reg,(efuse_data[0x12]>>15)&1);
pmic_read_interface(0x04AE,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x04AE,9  ,val_reg,(efuse_data[0x13]>>0 )&1);
pmic_read_interface(0x04AE,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x04AE,8  ,val_reg,(efuse_data[0x13]>>1  )&1);
pmic_read_interface(0x0486,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x0486,15 ,val_reg,(efuse_data[0x13]>>2  )&1);
pmic_read_interface(0x0486,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x0486,14 ,val_reg,(efuse_data[0x13]>>3  )&1);
pmic_read_interface(0x0486,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x0486,13 ,val_reg,(efuse_data[0x13]>>4 )&1);
pmic_read_interface(0x0486,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0486,12 ,val_reg,(efuse_data[0x13]>>5 )&1);
pmic_read_interface(0x0486,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0486,11 ,val_reg,(efuse_data[0x13]>>6 )&1);
pmic_read_interface(0x0486,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0486,10 ,val_reg,(efuse_data[0x13]>>7  )&1);
pmic_read_interface(0x0486,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x0486,9  ,val_reg,(efuse_data[0x13]>>8  )&1);
pmic_read_interface(0x0486,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x0486,8  ,val_reg,(efuse_data[0x13]>>9  )&1);
pmic_read_interface(0x049A,&val_reg,0x1,15);print("[0x%x:%d]:%d =%d  \n", 0x049A,15 ,val_reg,(efuse_data[0x13]>>10 )&1);
pmic_read_interface(0x049A,&val_reg,0x1,14);print("[0x%x:%d]:%d =%d  \n", 0x049A,14 ,val_reg,(efuse_data[0x13]>>11 )&1);
pmic_read_interface(0x049A,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x049A,13 ,val_reg,(efuse_data[0x13]>>12 )&1);
pmic_read_interface(0x049A,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x049A,12 ,val_reg,(efuse_data[0x13]>>13)&1);
pmic_read_interface(0x049A,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x049A,11 ,val_reg,(efuse_data[0x13]>>14)&1);
pmic_read_interface(0x049A,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x049A,10 ,val_reg,(efuse_data[0x13]>>15)&1);
pmic_read_interface(0x049A,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x049A,9  ,val_reg,(efuse_data[0x14]>>0 )&1);
pmic_read_interface(0x049A,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x049A,8  ,val_reg,(efuse_data[0x14]>>1 )&1);
pmic_read_interface(0x0456,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x0456,0  ,val_reg,(efuse_data[0x14]>>2 )&1);
pmic_read_interface(0x0456,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x0456,1  ,val_reg,(efuse_data[0x14]>>3 )&1);
pmic_read_interface(0x0456,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x0456,2  ,val_reg,(efuse_data[0x14]>>4 )&1);
pmic_read_interface(0x0456,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x0456,3  ,val_reg,(efuse_data[0x14]>>5 )&1);
pmic_read_interface(0x0456,&val_reg,0x1,4 );print("[0x%x:%d]:%d =%d  \n", 0x0456,4  ,val_reg,(efuse_data[0x14]>>6 )&1);
pmic_read_interface(0x0456,&val_reg,0x1,5 );print("[0x%x:%d]:%d =%d  \n", 0x0456,5  ,val_reg,(efuse_data[0x14]>>7 )&1);
pmic_read_interface(0x0454,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x0454,8  ,val_reg,(efuse_data[0x14]>>8 )&1);
pmic_read_interface(0x0454,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x0454,9  ,val_reg,(efuse_data[0x14]>>9 )&1);
pmic_read_interface(0x0454,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0454,10 ,val_reg,(efuse_data[0x14]>>10)&1);
pmic_read_interface(0x0454,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0454,11 ,val_reg,(efuse_data[0x14]>>11)&1);
pmic_read_interface(0x0454,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0454,12 ,val_reg,(efuse_data[0x14]>>12)&1);
pmic_read_interface(0x0454,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x0454,13 ,val_reg,(efuse_data[0x14]>>13)&1);
pmic_read_interface(0x046A,&val_reg,0x1,0 );print("[0x%x:%d]:%d =%d  \n", 0x046A,0  ,val_reg,(efuse_data[0x14]>>14 )&1);
pmic_read_interface(0x046A,&val_reg,0x1,1 );print("[0x%x:%d]:%d =%d  \n", 0x046A,1  ,val_reg,(efuse_data[0x14]>>15 )&1);
pmic_read_interface(0x046A,&val_reg,0x1,2 );print("[0x%x:%d]:%d =%d  \n", 0x046A,2  ,val_reg,(efuse_data[0x15]>>0  )&1);
pmic_read_interface(0x046A,&val_reg,0x1,3 );print("[0x%x:%d]:%d =%d  \n", 0x046A,3  ,val_reg,(efuse_data[0x15]>>1  )&1);
pmic_read_interface(0x046A,&val_reg,0x1,4 );print("[0x%x:%d]:%d =%d  \n", 0x046A,4  ,val_reg,(efuse_data[0x15]>>2  )&1);
pmic_read_interface(0x046A,&val_reg,0x1,5 );print("[0x%x:%d]:%d =%d  \n", 0x046A,5  ,val_reg,(efuse_data[0x15]>>3  )&1);
pmic_read_interface(0x0468,&val_reg,0x1,8 );print("[0x%x:%d]:%d =%d  \n", 0x0468,8  ,val_reg,(efuse_data[0x15]>>4 )&1);
pmic_read_interface(0x0468,&val_reg,0x1,9 );print("[0x%x:%d]:%d =%d  \n", 0x0468,9  ,val_reg,(efuse_data[0x15]>>5 )&1);
pmic_read_interface(0x0468,&val_reg,0x1,10);print("[0x%x:%d]:%d =%d  \n", 0x0468,10 ,val_reg,(efuse_data[0x15]>>6 )&1);
pmic_read_interface(0x0468,&val_reg,0x1,11);print("[0x%x:%d]:%d =%d  \n", 0x0468,11 ,val_reg,(efuse_data[0x15]>>7 )&1);
pmic_read_interface(0x0468,&val_reg,0x1,12);print("[0x%x:%d]:%d =%d  \n", 0x0468,12 ,val_reg,(efuse_data[0x15]>>8 )&1);
pmic_read_interface(0x0468,&val_reg,0x1,13);print("[0x%x:%d]:%d =%d  \n", 0x0468,13 ,val_reg,(efuse_data[0x15]>>9 )&1);
#endif
}

void pmic_efuse_management(void)
{
    int i=0;
    int is_efuse_trimed=0;
    u16 status = 0;
#if MT6351

#if MT6328
    u32 data32, data32_chk,data32_448_org,data32_472_org, data32_448,data32_472;
#endif
    is_efuse_trimed = ((upmu_get_reg_value(PMIC_RG_OTP_DOUT_496_511_ADDR))>>15)&0x0001;

    print("[PMIC_PL][6353] is_efuse_trimed=0x%x,[0x%x]=0x%x\n", is_efuse_trimed, PMIC_RG_OTP_DOUT_496_511_ADDR, upmu_get_reg_value(PMIC_RG_OTP_DOUT_496_511_ADDR));

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
		print("[PMIC_PL][6353] efuse_data[0x%x]=0x%x\n", i, efuse_data[i]);


#if EFUSE_DEBUG
		print("Before apply pmic efuse\n");
		pmic_efuse_check();
#endif
		//------------------------------------------
		/* check with peter peng, file: MT6353_EFUSE_TABLE_correlation.xlsx */
		pmic_config_interface(0x0AC6,((efuse_data[0x5 ] >>9  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AC6,((efuse_data[0x5 ] >>10 )&0x0001),0x1,1 );
		pmic_config_interface(0x0AC6,((efuse_data[0x5 ] >>11 )&0x0001),0x1,2 );
		pmic_config_interface(0x0AC6,((efuse_data[0x5 ] >>12 )&0x0001),0x1,3 );
		pmic_config_interface(0x0ACC,((efuse_data[0x5 ] >>13 )&0x0001),0x1,0 );
		pmic_config_interface(0x0ACC,((efuse_data[0x5 ] >>14 )&0x0001),0x1,1 );
		pmic_config_interface(0x0ACC,((efuse_data[0x5 ] >>15 )&0x0001),0x1,2 );
		pmic_config_interface(0x0ACC,((efuse_data[0x6 ] >>0  )&0x0001),0x1,3 );
		pmic_config_interface(0x0ACA,((efuse_data[0x6 ] >>1  )&0x0001),0x1,0 );
		pmic_config_interface(0x0ACA,((efuse_data[0x6 ] >>2  )&0x0001),0x1,1 );
		pmic_config_interface(0x0ACA,((efuse_data[0x6 ] >>3  )&0x0001),0x1,2 );
		pmic_config_interface(0x0ACA,((efuse_data[0x6 ] >>4  )&0x0001),0x1,3 );
		pmic_config_interface(0x0AD6,((efuse_data[0x6 ] >>5  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AD6,((efuse_data[0x6 ] >>6  )&0x0001),0x1,1 );
		pmic_config_interface(0x0AD6,((efuse_data[0x6 ] >>7  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AD6,((efuse_data[0x6 ] >>8  )&0x0001),0x1,3 );
		pmic_config_interface(0x0B26,((efuse_data[0x6 ] >>9  )&0x0001),0x1,0 );
		pmic_config_interface(0x0B26,((efuse_data[0x6 ] >>10 )&0x0001),0x1,1 );
		pmic_config_interface(0x0B26,((efuse_data[0x6 ] >>11 )&0x0001),0x1,2 );
		pmic_config_interface(0x0B26,((efuse_data[0x6 ] >>12 )&0x0001),0x1,3 );
		pmic_config_interface(0x0B26,((efuse_data[0x6 ] >>13 )&0x0001),0x1,4 );
		pmic_config_interface(0x0AE6,((efuse_data[0x6 ] >>14 )&0x0001),0x1,0 );
		pmic_config_interface(0x0AE6,((efuse_data[0x6 ] >>15 )&0x0001),0x1,1 );
		pmic_config_interface(0x0AE6,((efuse_data[0x7 ] >>0  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AE6,((efuse_data[0x7 ] >>1  )&0x0001),0x1,3 );
		pmic_config_interface(0x0AEE,((efuse_data[0x7 ] >>2  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AEE,((efuse_data[0x7 ] >>3  )&0x0001),0x1,1 );
		pmic_config_interface(0x0AEE,((efuse_data[0x7 ] >>4  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AEE,((efuse_data[0x7 ] >>5  )&0x0001),0x1,3 );
		pmic_config_interface(0x0AF2,((efuse_data[0x7 ] >>6  )&0x0001),0x1,0 );
		pmic_config_interface(0x0AF2,((efuse_data[0x7 ] >>7  )&0x0001),0x1,1 );
		pmic_config_interface(0x0AF2,((efuse_data[0x7 ] >>8  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AF2,((efuse_data[0x7 ] >>9  )&0x0001),0x1,3 );
		pmic_config_interface(0x0AF6,((efuse_data[0x7 ] >>10 )&0x0001),0x1,0 );
		pmic_config_interface(0x0AF6,((efuse_data[0x7 ] >>11 )&0x0001),0x1,1 );
		pmic_config_interface(0x0AF6,((efuse_data[0x7 ] >>12 )&0x0001),0x1,2 );
		pmic_config_interface(0x0AF6,((efuse_data[0x7 ] >>13 )&0x0001),0x1,3 );
		pmic_config_interface(0x0B18,((efuse_data[0x7 ] >>14 )&0x0001),0x1,0 );
		pmic_config_interface(0x0B18,((efuse_data[0x7 ] >>15 )&0x0001),0x1,1 );
		pmic_config_interface(0x0B18,((efuse_data[0x8 ] >>0  )&0x0001),0x1,2 );
		pmic_config_interface(0x0B18,((efuse_data[0x8 ] >>1  )&0x0001),0x1,3 );
		pmic_config_interface(0x0B14,((efuse_data[0x8 ] >>2  )&0x0001),0x1,0 );
		pmic_config_interface(0x0B14,((efuse_data[0x8 ] >>3  )&0x0001),0x1,1 );
		pmic_config_interface(0x0B14,((efuse_data[0x8 ] >>4  )&0x0001),0x1,2 );
		pmic_config_interface(0x0B14,((efuse_data[0x8 ] >>5  )&0x0001),0x1,3 );
		pmic_config_interface(0x0B1C,((efuse_data[0x8 ] >>6  )&0x0001),0x1,0 );
		pmic_config_interface(0x0B1C,((efuse_data[0x8 ] >>7  )&0x0001),0x1,1 );
		pmic_config_interface(0x0B1C,((efuse_data[0x8 ] >>8  )&0x0001),0x1,2 );
		pmic_config_interface(0x0B1C,((efuse_data[0x8 ] >>9  )&0x0001),0x1,3 );
		pmic_config_interface(0x0B08,((efuse_data[0x8 ] >>10 )&0x0001),0x1,0 );
		pmic_config_interface(0x0B08,((efuse_data[0x8 ] >>11 )&0x0001),0x1,1 );
		pmic_config_interface(0x0B08,((efuse_data[0x8 ] >>12 )&0x0001),0x1,2 );
		pmic_config_interface(0x0B08,((efuse_data[0x8 ] >>13 )&0x0001),0x1,3 );
		pmic_config_interface(0x0AFC,((efuse_data[0x8 ] >>14 )&0x0001),0x1,0 );
		pmic_config_interface(0x0AFC,((efuse_data[0x8 ] >>15 )&0x0001),0x1,1 );
		pmic_config_interface(0x0AFC,((efuse_data[0x9 ] >>0  )&0x0001),0x1,2 );
		pmic_config_interface(0x0AFC,((efuse_data[0x9 ] >>1  )&0x0001),0x1,3 );
		pmic_config_interface(0x0B0C,((efuse_data[0x9 ] >>2  )&0x0001),0x1,0 );
		pmic_config_interface(0x0B0C,((efuse_data[0x9 ] >>3  )&0x0001),0x1,1 );
		pmic_config_interface(0x0B0C,((efuse_data[0x9 ] >>4  )&0x0001),0x1,2 );
		pmic_config_interface(0x0B0C,((efuse_data[0x9 ] >>5  )&0x0001),0x1,3 );
		pmic_config_interface(0x0B00,((efuse_data[0x9 ] >>6  )&0x0001),0x1,0 );
		pmic_config_interface(0x0B00,((efuse_data[0x9 ] >>7  )&0x0001),0x1,1 );
		pmic_config_interface(0x0B00,((efuse_data[0x9 ] >>8  )&0x0001),0x1,2 );
		pmic_config_interface(0x0B00,((efuse_data[0x9 ] >>9  )&0x0001),0x1,3 );
		if (get_MT6353_PMIC_chip_version() == 0x5110) {
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>10 )&0x0001),0x1,  );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>11 )&0x0001),0x1,  );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>12 )&0x0001),0x1,  );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>13 )&0x0001),0x1,  );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>14 )&0x0001),0x1,  );
			pmic_config_interface(0x0ADA,((efuse_data[0x9 ] >>15 )&0x0001),0x1,0 );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>0  )&0x0001),0x1,1 );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>1  )&0x0001),0x1,2 );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>2  )&0x0001),0x1,3 );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>3  )&0x0001),0x1,4 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>4  )&0x0001),0x1,2 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>5  )&0x0001),0x1,3 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>6  )&0x0001),0x1,4 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>7  )&0x0001),0x1,5 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>8  )&0x0001),0x1,6 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>9  )&0x0001),0x1,7 );
			pmic_config_interface(0x0AF8,((efuse_data[0xa ] >>10 )&0x0001),0x1,9 );
			pmic_config_interface(0x0AF8,((efuse_data[0xa ] >>11 )&0x0001),0x1,10);
			pmic_config_interface(0x0AF8,((efuse_data[0xa ] >>12 )&0x0001),0x1,11);
			pmic_config_interface(0x0AF8,((efuse_data[0xa ] >>13 )&0x0001),0x1,12);
			pmic_config_interface(0x0AF0,((efuse_data[0xa ] >>14 )&0x0001),0x1,9 );
			pmic_config_interface(0x0AF0,((efuse_data[0xa ] >>15 )&0x0001),0x1,10);
			pmic_config_interface(0x0AF0,((efuse_data[0xb ] >>0  )&0x0001),0x1,11);
			pmic_config_interface(0x0AF0,((efuse_data[0xb ] >>1  )&0x0001),0x1,12);
			pmic_config_interface(0x0AF4,((efuse_data[0xb ] >>2  )&0x0001),0x1,9 );
			pmic_config_interface(0x0AF4,((efuse_data[0xb ] >>3  )&0x0001),0x1,10);
			pmic_config_interface(0x0AF4,((efuse_data[0xb ] >>4  )&0x0001),0x1,11);
			pmic_config_interface(0x0AF4,((efuse_data[0xb ] >>5  )&0x0001),0x1,12);
			pmic_config_interface(0x0B02,((efuse_data[0xb ] >>6  )&0x0001),0x1,10);
			pmic_config_interface(0x0B02,((efuse_data[0xb ] >>7  )&0x0001),0x1,11);
			pmic_config_interface(0x0B02,((efuse_data[0xb ] >>8  )&0x0001),0x1,12);
			pmic_config_interface(0x0B02,((efuse_data[0xb ] >>9  )&0x0001),0x1,13);
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>10 )&0x0001),0x1,0 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>11 )&0x0001),0x1,1 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>12 )&0x0001),0x1,2 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>13 )&0x0001),0x1,3 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>14 )&0x0001),0x1,4 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>15 )&0x0001),0x1,5 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>0  )&0x0001),0x1,6 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>1  )&0x0001),0x1,7 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>2  )&0x0001),0x1,8 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>3  )&0x0001),0x1,9 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>4  )&0x0001),0x1,10);
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>5  )&0x0001),0x1,11);
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>6  )&0x0001),0x1,12);
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>7  )&0x0001),0x1,13);
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>8  )&0x0001),0x1,14);
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>9  )&0x0001),0x1,0 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>10 )&0x0001),0x1,1 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>11 )&0x0001),0x1,2 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>12 )&0x0001),0x1,3 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>13 )&0x0001),0x1,4 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>14 )&0x0001),0x1,5 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>15 )&0x0001),0x1,6 );
			pmic_config_interface(0x04DE,((efuse_data[0xd ] >>0  )&0x0001),0x1,7 );
			pmic_config_interface(0x04CE,((efuse_data[0xd ] >>1  )&0x0001),0x1,6 );
			pmic_config_interface(0x04CE,((efuse_data[0xd ] >>2  )&0x0001),0x1,7 );
			pmic_config_interface(0x04CE,((efuse_data[0xd ] >>3  )&0x0001),0x1,8 );
			pmic_config_interface(0x04CE,((efuse_data[0xd ] >>4  )&0x0001),0x1,9 );
			pmic_config_interface(0x046A,((efuse_data[0xd ] >>5  )&0x0001),0x1,12);
			pmic_config_interface(0x046A,((efuse_data[0xd ] >>6  )&0x0001),0x1,13);
			pmic_config_interface(0x046A,((efuse_data[0xd ] >>7  )&0x0001),0x1,14);
			pmic_config_interface(0x046A,((efuse_data[0xd ] >>8  )&0x0001),0x1,15);
			pmic_config_interface(0x04BA,((efuse_data[0xd ] >>9  )&0x0001),0x1,6 );
			pmic_config_interface(0x04BA,((efuse_data[0xd ] >>10 )&0x0001),0x1,7 );
			pmic_config_interface(0x04BA,((efuse_data[0xd ] >>11 )&0x0001),0x1,8 );
			pmic_config_interface(0x04BA,((efuse_data[0xd ] >>12 )&0x0001),0x1,9 );
			pmic_config_interface(0x0456,((efuse_data[0xd ] >>13 )&0x0001),0x1,12);
			pmic_config_interface(0x0456,((efuse_data[0xd ] >>14 )&0x0001),0x1,13);
			pmic_config_interface(0x0456,((efuse_data[0xd ] >>15 )&0x0001),0x1,14);
			pmic_config_interface(0x0456,((efuse_data[0xe ] >>0  )&0x0001),0x1,15);
			pmic_config_interface(0x04A6,((efuse_data[0xe ] >>1  )&0x0001),0x1,6 );
			pmic_config_interface(0x04A6,((efuse_data[0xe ] >>2  )&0x0001),0x1,7 );
			pmic_config_interface(0x04A6,((efuse_data[0xe ] >>3  )&0x0001),0x1,8 );
			pmic_config_interface(0x04A6,((efuse_data[0xe ] >>4  )&0x0001),0x1,9 );
			pmic_config_interface(0x047E,((efuse_data[0xe ] >>5  )&0x0001),0x1,6 );
			pmic_config_interface(0x047E,((efuse_data[0xe ] >>6  )&0x0001),0x1,7 );
			pmic_config_interface(0x047E,((efuse_data[0xe ] >>7  )&0x0001),0x1,8 );
			pmic_config_interface(0x047E,((efuse_data[0xe ] >>8  )&0x0001),0x1,9 );
			pmic_config_interface(0x0492,((efuse_data[0xe ] >>9  )&0x0001),0x1,6 );
			pmic_config_interface(0x0492,((efuse_data[0xe ] >>10 )&0x0001),0x1,7 );
			pmic_config_interface(0x0492,((efuse_data[0xe ] >>11 )&0x0001),0x1,8 );
			pmic_config_interface(0x0492,((efuse_data[0xe ] >>12 )&0x0001),0x1,9 );
			pmic_config_interface(0x04DE,((efuse_data[0xe ] >>13 )&0x0001),0x1,8 );
			pmic_config_interface(0x04DE,((efuse_data[0xe ] >>14 )&0x0001),0x1,9 );
			pmic_config_interface(0x04DE,((efuse_data[0xe ] >>15 )&0x0001),0x1,10);
			pmic_config_interface(0x04DE,((efuse_data[0xf ] >>0  )&0x0001),0x1,11);
			pmic_config_interface(0x0448,((efuse_data[0xf ] >>1  )&0x0001),0x1,5 );
			pmic_config_interface(0x0448,((efuse_data[0xf ] >>2  )&0x0001),0x1,6 );
			pmic_config_interface(0x0448,((efuse_data[0xf ] >>3  )&0x0001),0x1,7 );
			pmic_config_interface(0x043E,((efuse_data[0xf ] >>4  )&0x0001),0x1,10);
			pmic_config_interface(0x043E,((efuse_data[0xf ] >>5  )&0x0001),0x1,11);
			pmic_config_interface(0x043E,((efuse_data[0xf ] >>6  )&0x0001),0x1,12);
			pmic_config_interface(0x0446,((efuse_data[0xf ] >>7  )&0x0001),0x1,10);
			pmic_config_interface(0x0446,((efuse_data[0xf ] >>8  )&0x0001),0x1,11);
			pmic_config_interface(0x0446,((efuse_data[0xf ] >>9  )&0x0001),0x1,12);
			pmic_config_interface(0x043C,((efuse_data[0xf ] >>10 )&0x0001),0x1,10);
			pmic_config_interface(0x043C,((efuse_data[0xf ] >>11 )&0x0001),0x1,11);
			pmic_config_interface(0x043C,((efuse_data[0xf ] >>12 )&0x0001),0x1,12);
			pmic_config_interface(0x0444,((efuse_data[0xf ] >>13 )&0x0001),0x1,10);
			pmic_config_interface(0x0444,((efuse_data[0xf ] >>14 )&0x0001),0x1,11);
			pmic_config_interface(0x0444,((efuse_data[0xf ] >>15 )&0x0001),0x1,12);
			pmic_config_interface(0x0440,((efuse_data[0x10] >>0  )&0x0001),0x1,10);
			pmic_config_interface(0x0440,((efuse_data[0x10] >>1  )&0x0001),0x1,11);
			pmic_config_interface(0x0440,((efuse_data[0x10] >>2  )&0x0001),0x1,12);
			pmic_config_interface(0x0442,((efuse_data[0x10] >>3  )&0x0001),0x1,10);
			pmic_config_interface(0x0442,((efuse_data[0x10] >>4  )&0x0001),0x1,11);
			pmic_config_interface(0x0442,((efuse_data[0x10] >>5  )&0x0001),0x1,12);
			pmic_config_interface(0x044A,((efuse_data[0x10] >>6  )&0x0001),0x1,5 );
			pmic_config_interface(0x044A,((efuse_data[0x10] >>7  )&0x0001),0x1,6 );
			pmic_config_interface(0x044A,((efuse_data[0x10] >>8  )&0x0001),0x1,7 );
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>9  )&0x0001),0x1,15);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>10 )&0x0001),0x1,14);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>11 )&0x0001),0x1,13);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>12 )&0x0001),0x1,12);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>13 )&0x0001),0x1,11);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>14 )&0x0001),0x1,10);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>15 )&0x0001),0x1,9 );
			pmic_config_interface(0x04D6,((efuse_data[0x11] >>0  )&0x0001),0x1,8 );
			pmic_config_interface(0x0472,((efuse_data[0x11] >>1  )&0x0001),0x1,15);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>2  )&0x0001),0x1,14);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>3  )&0x0001),0x1,13);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>4  )&0x0001),0x1,12);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>5  )&0x0001),0x1,11);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>6  )&0x0001),0x1,10);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>7  )&0x0001),0x1,9 );
			pmic_config_interface(0x0472,((efuse_data[0x11] >>8  )&0x0001),0x1,8 );
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>9  )&0x0001),0x1,15);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>10 )&0x0001),0x1,14);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>11 )&0x0001),0x1,13);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>12 )&0x0001),0x1,12);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>13 )&0x0001),0x1,11);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>14 )&0x0001),0x1,10);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>15 )&0x0001),0x1,9 );
			pmic_config_interface(0x04C2,((efuse_data[0x12] >>0  )&0x0001),0x1,8 );
			pmic_config_interface(0x045E,((efuse_data[0x12] >>1  )&0x0001),0x1,15);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>2  )&0x0001),0x1,14);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>3  )&0x0001),0x1,13);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>4  )&0x0001),0x1,12);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>5  )&0x0001),0x1,11);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>6  )&0x0001),0x1,10);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>7  )&0x0001),0x1,9 );
			pmic_config_interface(0x045E,((efuse_data[0x12] >>8  )&0x0001),0x1,8 );
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>9  )&0x0001),0x1,15);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>10 )&0x0001),0x1,14);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>11 )&0x0001),0x1,13);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>12 )&0x0001),0x1,12);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>13 )&0x0001),0x1,11);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>14 )&0x0001),0x1,10);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>15 )&0x0001),0x1,9 );
			pmic_config_interface(0x04AE,((efuse_data[0x13] >>0  )&0x0001),0x1,8 );
			pmic_config_interface(0x0486,((efuse_data[0x13] >>1  )&0x0001),0x1,15);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>2  )&0x0001),0x1,14);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>3  )&0x0001),0x1,13);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>4  )&0x0001),0x1,12);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>5  )&0x0001),0x1,11);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>6  )&0x0001),0x1,10);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>7  )&0x0001),0x1,9 );
			pmic_config_interface(0x0486,((efuse_data[0x13] >>8  )&0x0001),0x1,8 );
			pmic_config_interface(0x049A,((efuse_data[0x13] >>9  )&0x0001),0x1,15);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>10 )&0x0001),0x1,14);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>11 )&0x0001),0x1,13);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>12 )&0x0001),0x1,12);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>13 )&0x0001),0x1,11);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>14 )&0x0001),0x1,10);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>15 )&0x0001),0x1,9 );
			pmic_config_interface(0x049A,((efuse_data[0x14] >>0  )&0x0001),0x1,8 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>1  )&0x0001),0x1,0 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>2  )&0x0001),0x1,1 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>3  )&0x0001),0x1,2 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>4  )&0x0001),0x1,3 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>5  )&0x0001),0x1,4 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>6  )&0x0001),0x1,5 );
			pmic_config_interface(0x0454,((efuse_data[0x14] >>7  )&0x0001),0x1,8 );
			pmic_config_interface(0x0454,((efuse_data[0x14] >>8  )&0x0001),0x1,9 );
			pmic_config_interface(0x0454,((efuse_data[0x14] >>9  )&0x0001),0x1,10);
			pmic_config_interface(0x0454,((efuse_data[0x14] >>10 )&0x0001),0x1,11);
			pmic_config_interface(0x0454,((efuse_data[0x14] >>11 )&0x0001),0x1,12);
			pmic_config_interface(0x0454,((efuse_data[0x14] >>12 )&0x0001),0x1,13);
			pmic_config_interface(0x046A,((efuse_data[0x14] >>13 )&0x0001),0x1,0 );
			pmic_config_interface(0x046A,((efuse_data[0x14] >>14 )&0x0001),0x1,1 );
			pmic_config_interface(0x046A,((efuse_data[0x14] >>15 )&0x0001),0x1,2 );
			pmic_config_interface(0x046A,((efuse_data[0x15] >>0  )&0x0001),0x1,3 );
			pmic_config_interface(0x046A,((efuse_data[0x15] >>1  )&0x0001),0x1,4 );
			pmic_config_interface(0x046A,((efuse_data[0x15] >>2  )&0x0001),0x1,5 );
			pmic_config_interface(0x0468,((efuse_data[0x15] >>3  )&0x0001),0x1,8 );
			pmic_config_interface(0x0468,((efuse_data[0x15] >>4  )&0x0001),0x1,9 );
			pmic_config_interface(0x0468,((efuse_data[0x15] >>5  )&0x0001),0x1,10);
			pmic_config_interface(0x0468,((efuse_data[0x15] >>6  )&0x0001),0x1,11);
			pmic_config_interface(0x0468,((efuse_data[0x15] >>7  )&0x0001),0x1,12);
			pmic_config_interface(0x0468,((efuse_data[0x15] >>8  )&0x0001),0x1,13);
		}
		else if (get_MT6353_PMIC_chip_version() == 0x5120) {
			pmic_config_interface(0x0B00,((efuse_data[0x9 ] >>10 )&0x0001),0x1,4 );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>11 )&0x0001),0x1,  );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>12 )&0x0001),0x1,  );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>13 )&0x0001),0x1,  );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>14 )&0x0001),0x1,  );
	//		pmic_config_interface(RSV   ,((efuse_data[0x9 ] >>15 )&0x0001),0x1,  );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>0  )&0x0001),0x1,0 );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>1  )&0x0001),0x1,1 );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>2  )&0x0001),0x1,2 );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>3  )&0x0001),0x1,3 );
			pmic_config_interface(0x0ADA,((efuse_data[0xa ] >>4  )&0x0001),0x1,4 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>5  )&0x0001),0x1,2 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>6  )&0x0001),0x1,3 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>7  )&0x0001),0x1,4 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>8  )&0x0001),0x1,5 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>9  )&0x0001),0x1,6 );
			pmic_config_interface(0x0B28,((efuse_data[0xa ] >>10 )&0x0001),0x1,7 );
			pmic_config_interface(0x0AF8,((efuse_data[0xa ] >>11 )&0x0001),0x1,9 );
			pmic_config_interface(0x0AF8,((efuse_data[0xa ] >>12 )&0x0001),0x1,10);
			pmic_config_interface(0x0AF8,((efuse_data[0xa ] >>13 )&0x0001),0x1,11);
			pmic_config_interface(0x0AF8,((efuse_data[0xa ] >>14 )&0x0001),0x1,12);
			pmic_config_interface(0x0AF0,((efuse_data[0xa ] >>15 )&0x0001),0x1,9 );
			pmic_config_interface(0x0AF0,((efuse_data[0xb ] >>0  )&0x0001),0x1,10);
			pmic_config_interface(0x0AF0,((efuse_data[0xb ] >>1  )&0x0001),0x1,11);
			pmic_config_interface(0x0AF0,((efuse_data[0xb ] >>2  )&0x0001),0x1,12);
			pmic_config_interface(0x0AF4,((efuse_data[0xb ] >>3  )&0x0001),0x1,9 );
			pmic_config_interface(0x0AF4,((efuse_data[0xb ] >>4  )&0x0001),0x1,10);
			pmic_config_interface(0x0AF4,((efuse_data[0xb ] >>5  )&0x0001),0x1,11);
			pmic_config_interface(0x0AF4,((efuse_data[0xb ] >>6  )&0x0001),0x1,12);
			pmic_config_interface(0x0B02,((efuse_data[0xb ] >>7  )&0x0001),0x1,10);
			pmic_config_interface(0x0B02,((efuse_data[0xb ] >>8  )&0x0001),0x1,11);
			pmic_config_interface(0x0B02,((efuse_data[0xb ] >>9  )&0x0001),0x1,12);
			pmic_config_interface(0x0B02,((efuse_data[0xb ] >>10 )&0x0001),0x1,13);
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>11 )&0x0001),0x1,0 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>12 )&0x0001),0x1,1 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>13 )&0x0001),0x1,2 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>14 )&0x0001),0x1,3 );
			pmic_config_interface(0x043A,((efuse_data[0xb ] >>15 )&0x0001),0x1,4 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>0  )&0x0001),0x1,5 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>1  )&0x0001),0x1,6 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>2  )&0x0001),0x1,7 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>3  )&0x0001),0x1,8 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>4  )&0x0001),0x1,9 );
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>5  )&0x0001),0x1,10);
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>6  )&0x0001),0x1,11);
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>7  )&0x0001),0x1,12);
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>8  )&0x0001),0x1,13);
			pmic_config_interface(0x043A,((efuse_data[0xc ] >>9  )&0x0001),0x1,14);
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>10 )&0x0001),0x1,0 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>11 )&0x0001),0x1,1 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>12 )&0x0001),0x1,2 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>13 )&0x0001),0x1,3 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>14 )&0x0001),0x1,4 );
			pmic_config_interface(0x04DE,((efuse_data[0xc ] >>15 )&0x0001),0x1,5 );
			pmic_config_interface(0x04DE,((efuse_data[0xd ] >>0  )&0x0001),0x1,6 );
			pmic_config_interface(0x04DE,((efuse_data[0xd ] >>1  )&0x0001),0x1,7 );
			pmic_config_interface(0x04CE,((efuse_data[0xd ] >>2  )&0x0001),0x1,6 );
			pmic_config_interface(0x04CE,((efuse_data[0xd ] >>3  )&0x0001),0x1,7 );
			pmic_config_interface(0x04CE,((efuse_data[0xd ] >>4  )&0x0001),0x1,8 );
			pmic_config_interface(0x04CE,((efuse_data[0xd ] >>5  )&0x0001),0x1,9 );
			pmic_config_interface(0x046A,((efuse_data[0xd ] >>6  )&0x0001),0x1,12);
			pmic_config_interface(0x046A,((efuse_data[0xd ] >>7  )&0x0001),0x1,13);
			pmic_config_interface(0x046A,((efuse_data[0xd ] >>8  )&0x0001),0x1,14);
			pmic_config_interface(0x046A,((efuse_data[0xd ] >>9  )&0x0001),0x1,15);
			pmic_config_interface(0x04BA,((efuse_data[0xd ] >>10 )&0x0001),0x1,6 );
			pmic_config_interface(0x04BA,((efuse_data[0xd ] >>11 )&0x0001),0x1,7 );
			pmic_config_interface(0x04BA,((efuse_data[0xd ] >>12 )&0x0001),0x1,8 );
			pmic_config_interface(0x04BA,((efuse_data[0xd ] >>13 )&0x0001),0x1,9 );
			pmic_config_interface(0x0456,((efuse_data[0xd ] >>14 )&0x0001),0x1,12);
			pmic_config_interface(0x0456,((efuse_data[0xd ] >>15 )&0x0001),0x1,13);
			pmic_config_interface(0x0456,((efuse_data[0xe ] >>0  )&0x0001),0x1,14);
			pmic_config_interface(0x0456,((efuse_data[0xe ] >>1  )&0x0001),0x1,15);
			pmic_config_interface(0x04A6,((efuse_data[0xe ] >>2  )&0x0001),0x1,6 );
			pmic_config_interface(0x04A6,((efuse_data[0xe ] >>3  )&0x0001),0x1,7 );
			pmic_config_interface(0x04A6,((efuse_data[0xe ] >>4  )&0x0001),0x1,8 );
			pmic_config_interface(0x04A6,((efuse_data[0xe ] >>5  )&0x0001),0x1,9 );
			pmic_config_interface(0x047E,((efuse_data[0xe ] >>6  )&0x0001),0x1,6 );
			pmic_config_interface(0x047E,((efuse_data[0xe ] >>7  )&0x0001),0x1,7 );
			pmic_config_interface(0x047E,((efuse_data[0xe ] >>8  )&0x0001),0x1,8 );
			pmic_config_interface(0x047E,((efuse_data[0xe ] >>9  )&0x0001),0x1,9 );
			pmic_config_interface(0x0492,((efuse_data[0xe ] >>10 )&0x0001),0x1,6 );
			pmic_config_interface(0x0492,((efuse_data[0xe ] >>11 )&0x0001),0x1,7 );
			pmic_config_interface(0x0492,((efuse_data[0xe ] >>12 )&0x0001),0x1,8 );
			pmic_config_interface(0x0492,((efuse_data[0xe ] >>13 )&0x0001),0x1,9 );
			pmic_config_interface(0x04DE,((efuse_data[0xe ] >>14 )&0x0001),0x1,8 );
			pmic_config_interface(0x04DE,((efuse_data[0xe ] >>15 )&0x0001),0x1,9 );
			pmic_config_interface(0x04DE,((efuse_data[0xf ] >>0  )&0x0001),0x1,10);
			pmic_config_interface(0x04DE,((efuse_data[0xf ] >>1  )&0x0001),0x1,11);
			pmic_config_interface(0x0448,((efuse_data[0xf ] >>2  )&0x0001),0x1,5 );
			pmic_config_interface(0x0448,((efuse_data[0xf ] >>3  )&0x0001),0x1,6 );
			pmic_config_interface(0x0448,((efuse_data[0xf ] >>4  )&0x0001),0x1,7 );
			pmic_config_interface(0x043E,((efuse_data[0xf ] >>5  )&0x0001),0x1,10);
			pmic_config_interface(0x043E,((efuse_data[0xf ] >>6  )&0x0001),0x1,11);
			pmic_config_interface(0x043E,((efuse_data[0xf ] >>7  )&0x0001),0x1,12);
			pmic_config_interface(0x0446,((efuse_data[0xf ] >>8  )&0x0001),0x1,10);
			pmic_config_interface(0x0446,((efuse_data[0xf ] >>9  )&0x0001),0x1,11);
			pmic_config_interface(0x0446,((efuse_data[0xf ] >>10 )&0x0001),0x1,12);
			pmic_config_interface(0x043C,((efuse_data[0xf ] >>11 )&0x0001),0x1,10);
			pmic_config_interface(0x043C,((efuse_data[0xf ] >>12 )&0x0001),0x1,11);
			pmic_config_interface(0x043C,((efuse_data[0xf ] >>13 )&0x0001),0x1,12);
			pmic_config_interface(0x0444,((efuse_data[0xf ] >>14 )&0x0001),0x1,10);
			pmic_config_interface(0x0444,((efuse_data[0xf ] >>15 )&0x0001),0x1,11);
			pmic_config_interface(0x0444,((efuse_data[0x10] >>0  )&0x0001),0x1,12);
			pmic_config_interface(0x0440,((efuse_data[0x10] >>1  )&0x0001),0x1,10);
			pmic_config_interface(0x0440,((efuse_data[0x10] >>2  )&0x0001),0x1,11);
			pmic_config_interface(0x0440,((efuse_data[0x10] >>3  )&0x0001),0x1,12);
			pmic_config_interface(0x0442,((efuse_data[0x10] >>4  )&0x0001),0x1,10);
			pmic_config_interface(0x0442,((efuse_data[0x10] >>5  )&0x0001),0x1,11);
			pmic_config_interface(0x0442,((efuse_data[0x10] >>6  )&0x0001),0x1,12);
			pmic_config_interface(0x044A,((efuse_data[0x10] >>7  )&0x0001),0x1,5 );
			pmic_config_interface(0x044A,((efuse_data[0x10] >>8  )&0x0001),0x1,6 );
			pmic_config_interface(0x044A,((efuse_data[0x10] >>9  )&0x0001),0x1,7 );
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>10 )&0x0001),0x1,15);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>11 )&0x0001),0x1,14);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>12 )&0x0001),0x1,13);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>13 )&0x0001),0x1,12);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>14 )&0x0001),0x1,11);
			pmic_config_interface(0x04D6,((efuse_data[0x10] >>15 )&0x0001),0x1,10);
			pmic_config_interface(0x04D6,((efuse_data[0x11] >>0  )&0x0001),0x1,9 );
			pmic_config_interface(0x04D6,((efuse_data[0x11] >>1  )&0x0001),0x1,8 );
			pmic_config_interface(0x0472,((efuse_data[0x11] >>2  )&0x0001),0x1,15);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>3  )&0x0001),0x1,14);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>4  )&0x0001),0x1,13);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>5  )&0x0001),0x1,12);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>6  )&0x0001),0x1,11);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>7  )&0x0001),0x1,10);
			pmic_config_interface(0x0472,((efuse_data[0x11] >>8  )&0x0001),0x1,9 );
			pmic_config_interface(0x0472,((efuse_data[0x11] >>9  )&0x0001),0x1,8 );
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>10 )&0x0001),0x1,15);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>11 )&0x0001),0x1,14);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>12 )&0x0001),0x1,13);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>13 )&0x0001),0x1,12);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>14 )&0x0001),0x1,11);
			pmic_config_interface(0x04C2,((efuse_data[0x11] >>15 )&0x0001),0x1,10);
			pmic_config_interface(0x04C2,((efuse_data[0x12] >>0  )&0x0001),0x1,9 );
			pmic_config_interface(0x04C2,((efuse_data[0x12] >>1  )&0x0001),0x1,8 );
			pmic_config_interface(0x045E,((efuse_data[0x12] >>2  )&0x0001),0x1,15);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>3  )&0x0001),0x1,14);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>4  )&0x0001),0x1,13);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>5  )&0x0001),0x1,12);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>6  )&0x0001),0x1,11);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>7  )&0x0001),0x1,10);
			pmic_config_interface(0x045E,((efuse_data[0x12] >>8  )&0x0001),0x1,9 );
			pmic_config_interface(0x045E,((efuse_data[0x12] >>9  )&0x0001),0x1,8 );
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>10 )&0x0001),0x1,15);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>11 )&0x0001),0x1,14);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>12 )&0x0001),0x1,13);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>13 )&0x0001),0x1,12);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>14 )&0x0001),0x1,11);
			pmic_config_interface(0x04AE,((efuse_data[0x12] >>15 )&0x0001),0x1,10);
			pmic_config_interface(0x04AE,((efuse_data[0x13] >>0  )&0x0001),0x1,9 );
			pmic_config_interface(0x04AE,((efuse_data[0x13] >>1  )&0x0001),0x1,8 );
			pmic_config_interface(0x0486,((efuse_data[0x13] >>2  )&0x0001),0x1,15);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>3  )&0x0001),0x1,14);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>4  )&0x0001),0x1,13);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>5  )&0x0001),0x1,12);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>6  )&0x0001),0x1,11);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>7  )&0x0001),0x1,10);
			pmic_config_interface(0x0486,((efuse_data[0x13] >>8  )&0x0001),0x1,9 );
			pmic_config_interface(0x0486,((efuse_data[0x13] >>9  )&0x0001),0x1,8 );
			pmic_config_interface(0x049A,((efuse_data[0x13] >>10 )&0x0001),0x1,15);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>11 )&0x0001),0x1,14);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>12 )&0x0001),0x1,13);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>13 )&0x0001),0x1,12);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>14 )&0x0001),0x1,11);
			pmic_config_interface(0x049A,((efuse_data[0x13] >>15 )&0x0001),0x1,10);
			pmic_config_interface(0x049A,((efuse_data[0x14] >>0  )&0x0001),0x1,9 );
			pmic_config_interface(0x049A,((efuse_data[0x14] >>1  )&0x0001),0x1,8 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>2  )&0x0001),0x1,0 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>3  )&0x0001),0x1,1 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>4  )&0x0001),0x1,2 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>5  )&0x0001),0x1,3 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>6  )&0x0001),0x1,4 );
			pmic_config_interface(0x0456,((efuse_data[0x14] >>7  )&0x0001),0x1,5 );
			pmic_config_interface(0x0454,((efuse_data[0x14] >>8  )&0x0001),0x1,8 );
			pmic_config_interface(0x0454,((efuse_data[0x14] >>9  )&0x0001),0x1,9 );
			pmic_config_interface(0x0454,((efuse_data[0x14] >>10 )&0x0001),0x1,10);
			pmic_config_interface(0x0454,((efuse_data[0x14] >>11 )&0x0001),0x1,11);
			pmic_config_interface(0x0454,((efuse_data[0x14] >>12 )&0x0001),0x1,12);
			pmic_config_interface(0x0454,((efuse_data[0x14] >>13 )&0x0001),0x1,13);
			pmic_config_interface(0x046A,((efuse_data[0x14] >>14 )&0x0001),0x1,0 );
			pmic_config_interface(0x046A,((efuse_data[0x14] >>15 )&0x0001),0x1,1 );
			pmic_config_interface(0x046A,((efuse_data[0x15] >>0  )&0x0001),0x1,2 );
			pmic_config_interface(0x046A,((efuse_data[0x15] >>1  )&0x0001),0x1,3 );
			pmic_config_interface(0x046A,((efuse_data[0x15] >>2  )&0x0001),0x1,4 );
			pmic_config_interface(0x046A,((efuse_data[0x15] >>3  )&0x0001),0x1,5 );
			pmic_config_interface(0x0468,((efuse_data[0x15] >>4  )&0x0001),0x1,8 );
			pmic_config_interface(0x0468,((efuse_data[0x15] >>5  )&0x0001),0x1,9 );
			pmic_config_interface(0x0468,((efuse_data[0x15] >>6  )&0x0001),0x1,10);
			pmic_config_interface(0x0468,((efuse_data[0x15] >>7  )&0x0001),0x1,11);
			pmic_config_interface(0x0468,((efuse_data[0x15] >>8  )&0x0001),0x1,12);
			pmic_config_interface(0x0468,((efuse_data[0x15] >>9  )&0x0001),0x1,13);
		}

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

#endif /* MT6351 */
}

void pmic_disable_usbdl_wo_battery(void)
{
	print("[PMIC_PL] turn off usbdl wo battery...\n");
    pmic_config_interface(PMIC_RG_USBDL_SET_ADDR, 0x0000, PMIC_RG_USBDL_SET_MASK, PMIC_RG_USBDL_SET_SHIFT);
    pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 0x0001, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);
}

void pmic_auxadc_debug(int index)
{
    int ret_val=0, val, val1, val2, val3, val4;
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
	print("[PMIC_PL] [pmic_init]%d 0x0240:0x%x, 0x0282:0x%x, 0x025e:0x%x , 0x023a:0x%x , 0x0ea2:0x%x\n",index, val, val1, val2, val3, val4);
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
	kal_uint32 val=0 ,val1=0 ,val2=0 ,val3=0 ,val4;
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
			print( "[PMIC_PL][IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
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
			print( "[PMIC_PL][IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
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
			print( "[PMIC_PL][IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
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
			print( "[PMIC_PL][IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
			break;
			}
		}
		ret_data = pmic_get_register_value(PMIC_AUXADC_ADC_OUT_CH3_ADDR,PMIC_AUXADC_ADC_OUT_CH3_MASK,PMIC_AUXADC_ADC_OUT_CH3_SHIFT);
	break;


	default:
		print( "[PMIC_PL][AUXADC] Invalid channel value(%d,%d)\n", dwChannel, trimd);

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
            print( "[PMIC_PL][AUXADC] Invalid channel value(%d,%d)\n", dwChannel, trimd);

            return -1;
            break;
    }
	print( "[PMIC_PL][AUXADC] ch=%d raw=%d data=%d \n", dwChannel, ret_data,adc_result);

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
/*
	PMIC_BUCK_GEN1(VPA,PMIC_BUCK_VPA_VOSEL_ADDR,
		                      PMIC_BUCK_VPA_VOSEL_CTRL_ADDR,
		                      PMIC_DA_NI_VPA_VOSEL_GRAY_ADDR,
		                      PMIC_DA_NI_VPA_VOSEL_ADDR,
		                      PMIC_BUCK_VPA_VOSEL_SLEEP_ADDR,
		                      PMIC_BUCK_VPA_VOSEL_ON_ADDR),
*/
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
		print("[PMIC_PL][pmic_init]Wrong buck type\n");
		return -1;
	}
	if (voltage > 1400000) {
		print("[PMIC_PL][pmic_init]Wrong buck voltage\n");
		return -1;
	}

	value = (pmic_buck_voltages[voltage] - 600000)/6250;
	print("[PMIC_PL][pmic_init]%s Expected volt: 0x%x\n", mtk_buck[type].name, value);
	switch(type)
	{
#if MT6351
		case VPROC:
			/* [6:0]: VDVFS11_VOSEL; Setting by lower power ,20150305, Johsnon,_1.15V forD3T */
			mt6311_config_interface(mtk_buck[type].vosel ,value,0x7F,0);
			/* [6:0]: VDVFS11_VOSEL_ON; Setting by lower power ,20150305, Johsnon,_1.15V forD3T */
			mt6311_config_interface(mtk_buck[type].on ,value,0x7F,0);
			//2.delay 100us
			udelay(100);
			mt6311_read_byte(mtk_buck[type].vosel, &reg_val);
			print("[PMIC_PL][pmic_init]%s Reg[0x%x]=0x%x\n", mtk_buck[type].name, mtk_buck[type].vosel, reg_val);
			mt6311_read_byte(mtk_buck[type].on, &reg_val);
			print("[PMIC_PL][pmic_init]%s Reg[0x%x]=0x%x\n", mtk_buck[type].name, mtk_buck[type].on, reg_val);
			break;
#endif /* MT6351 */
		default:
			upmu_set_reg_value(mtk_buck[type].vosel ,value);
			upmu_set_reg_value(mtk_buck[type].on ,value);
			//4.delay 100us
			udelay(100);
			pmic_read_interface(mtk_buck[type].vosel, &reg_val,0x7F,0);
			print("[PMIC_PL][pmic_init] %s[0x%x]=0x%x\n",
				mtk_buck[type].name, mtk_buck[type].vosel, reg_val);
			pmic_read_interface(mtk_buck[type].da_ni_vosel_sync, &reg_val,0x7F,0);
			print("[PMIC_PL][pmic_init] %s[0x%x]=0x%x\n",
				mtk_buck[type].name, mtk_buck[type].da_ni_vosel_sync, reg_val);
			break;
	}
	return 0;
}

U32 buck_vcore_segment_voltage(void)
{
	kal_uint32 ret = 0;
	kal_uint32 segment;

	segment = seclib_get_devinfo_with_index(17) & 0xFF;

	/*----MT6353_BUCK_VCORE_VOL_CON1 0x632----*/
	if (segment == 0x41) {
		ret = pmic_config_interface(0x632,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 1.0V */
		ret = pmic_config_interface(0x634,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 1.0V */
	} else if (segment == 0x42) {
#ifdef USE_FHD_DRAM_CAL_FLOW
		ret = pmic_config_interface(0x632,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 1.0V */
		ret = pmic_config_interface(0x634,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 1.0V */
#else
		ret = pmic_config_interface(0x632,0x30,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 0.9V */
		ret = pmic_config_interface(0x634,0x30,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 0.9V */
#endif
	} else if (segment == 0x43) {
#ifdef USE_FHD_DRAM_CAL_FLOW
		ret = pmic_config_interface(0x632,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 1.0V */
		ret = pmic_config_interface(0x634,0x40,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 1.0V */
#else
		ret = pmic_config_interface(0x632,0x30,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL; Jimmy, 12/14, Reserve SW mode 0.9V */
		ret = pmic_config_interface(0x634,0x30,0x7F,0); /* [6:0]: BUCK_VCORE_VOSEL_ON; Jimmy, 12/14, Reserve HW mode 0.9V */
#endif
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
		print("[PMIC_PL] VCTCXO on MT6176 , OFF XO on MT6353\n");
	} else {
		ret = pmic_config_interface(PMIC_RG_CLKSQ_EN_AUX_AP_MODE_ADDR,0x1,
			PMIC_RG_CLKSQ_EN_AUX_AP_MODE_MASK,
			PMIC_RG_CLKSQ_EN_AUX_AP_MODE_SHIFT);
		/*  HW control, use srclken_1, for LP */
		ret = pmic_config_interface(0xA04, 0x1, 0x1, 4);
		ret = pmic_config_interface(0xA04, 0x1, 0x7, 11);
		print("[PMIC_PL] VCTCXO TSX on MT6353 0x7000=0x%x\n", pmic_reg);
	}
#endif
}
//==============================================================================
// PMIC Init Code
//==============================================================================
U32 pmic_init (void)
{
    U32 ret_code = PMIC_TEST_PASS;
    int ret_val=0, val;

    print("[PMIC_PL] Preloader Start...\n");
    print("[PMIC_PL] MT6353 CHIP Code = 0x%x\n", get_MT6353_PMIC_chip_version());

    /*1.UVLO off*/
    print("[PMIC_PL][pmic_status] Reg[0x%x]=0x%x\n", MT6353_TOP_RST_STATUS, upmu_get_reg_value(MT6353_TOP_RST_STATUS));
    /*2.thermal shutdown 150*/
    print("[PMIC_PL][pmic_status] Reg[0x%x]=0x%x\n", MT6353_THERMALSTATUS, upmu_get_reg_value(MT6353_THERMALSTATUS));
    /*3.power not good*/
    print("[PMIC_PL][pmic_status] Reg[0x%x]=0x%x\n", MT6353_PGSTATUS0, upmu_get_reg_value(MT6353_PGSTATUS0));
    /*4.buck oc*/
    print("[PMIC_PL][pmic_status] Reg[0x%x]=0x%x\n", MT6353_OCSTATUS1, upmu_get_reg_value(MT6353_OCSTATUS1));
    print("[PMIC_PL][pmic_status] Reg[0x%x]=0x%x\n", MT6353_OCSTATUS2, upmu_get_reg_value(MT6353_OCSTATUS2));
    /*5.long press shutdown*/
    print("[PMIC_PL][pmic_status] Reg[0x%x]=0x%x\n", MT6353_STRUP_CON4, upmu_get_reg_value(MT6353_STRUP_CON4));
    /*6.WDTRST*/
    print("[PMIC_PL][pmic_status] Reg[0x%x]=0x%x\n", MT6353_TOP_RST_MISC, upmu_get_reg_value(MT6353_TOP_RST_MISC));
    /*7.CLK TRIM*/
    print("[PMIC_PL][pmic_status] Reg[0x%x]=0x%x\n", 0x250, upmu_get_reg_value(0x250));

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
    print("[PMIC_PL][pmic_init] Reg[0x%x]=0x%x\n", MT6353_TOP_RST_MISC, upmu_get_reg_value(MT6353_TOP_RST_MISC));
    #endif


    /* ask peter peng programming guide TBD */
    ret_val= pmic_config_interface(PMIC_RG_SMPS_TESTMODE_B_ADDR, 0x0001, PMIC_RG_SMPS_TESTMODE_B_MASK, PMIC_RG_SMPS_TESTMODE_B_SHIFT); //RG_SMPS_TESTMODE_B by luke

#if MT6351
	 mt6311_driver_probe();
#endif /* MT6351 */

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

		if (mt_get_chip_hw_ver() >= 0xCA01) {  //CHIP_VER_E2
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

	/*--- Long Press Setting ---*/
	ret_val = pmic_config_interface(0x024,0x8041,0xFFFF,0);
	ret_val = pmic_config_interface(0x2A8,0x0300,0xFFFF,0);

	/*ret_val=pmic_config_interface(0x0A2E, 0x0, 0x1, 1);*/ // Disable RG_VMCH_EN
	print("[PMIC_PL] [pmic_init] Done...\n");
	/*pmic_auxadc_debug(2);*/
    return ret_code;
}


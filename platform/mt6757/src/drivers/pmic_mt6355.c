#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>
#include <regulator.h>
#include <mt6355.h>
#include <sec_devinfo.h>
#include <rtc.h>

/* External Config */
/* #define EXTERNAL_BUCK_xxxxxx */
#if defined(EXTERNAL_BUCK_xxxxxx)
#include <xxxxxx.h>
#endif

#define DEBUGSTS 1
#define EFUSE_DEBUG 0
#define PMIC_32K_LESS_DETECT_V1 0
U32 efuse_data[0x20]={0};
U32 upmu_get_reg_value(kal_uint32 reg);
int pmic_auxadc_init_setting_v1(void);

//==============================================================================
// PMIC access API
//==============================================================================
U32 pmic_read_interface (U32 RegNum, U32 *val, U32 MASK, U32 SHIFT)
{
    U32 return_value = 0;
    U32 pmic_reg = 0;
    U32 rdata = 0;

    //1. mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        print("[PMIC] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    //print("[pmic_read_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

    pmic_reg &= (MASK << SHIFT);
    *val = (pmic_reg >> SHIFT);
    //print("[pmic_read_interface] val=0x%x\n", *val);

    return return_value;
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
        print("[PMIC] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    //print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

    pmic_reg &= ~(MASK << SHIFT);
    pmic_reg |= (val << SHIFT);

    //2. mt_write_byte(RegNum, pmic_reg);
    return_value= pwrap_wacs2(1, (RegNum), pmic_reg, &rdata);
    if(return_value!=0)
    {
        print("[PMIC] Reg[%x]= pmic_wrap read data fail\n", RegNum);
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

U32 pmic_get_register_value(pmuflag pmureg)
{
	U32 val = 0;
	U32 ret;

	ret = pmic_read_interface (pmureg.addr, &val, pmureg.mask, pmureg.shift);

	return val;
}

U32 pmic_set_register_value(pmuflag pmureg, U32 val)
{
	U32 ret;

	ret = pmic_config_interface (pmureg.addr, val, pmureg.mask, pmureg.shift);

	return ret;
}

void upmu_set_reg_value(kal_uint32 reg, kal_uint32 reg_val)
{
    U32 ret=0;

    ret=pmic_config_interface(reg, reg_val, 0xFFFF, 0x0);
}

U32 upmu_get_reg_value(kal_uint32 reg)
{
    U32 ret=0;
    U32 reg_val=0;

    ret=pmic_read_interface(reg, &reg_val, 0xFFFF, 0x0);

    return reg_val;
}

//==============================================================================
// PMIC Usage APIs
//==============================================================================
U32 get_MT6355_PMIC_chip_version (void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6355_SWCID),
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

    ret=pmic_read_interface( (U32)(MT6355_TOPSTATUS),
                           (&val),
                           (U32)(PMIC_PWRKEY_DEB_MASK),
                           (U32)(PMIC_PWRKEY_DEB_SHIFT)
	                       );

    if (val==1){
        print("[PMIC] pl pmic powerkey Release\n");
        return 0;
    }else{
        print("[PMIC] pl pmic powerkey Press\n");
        return 1;
    }
}

int pmic_detect_homekey(void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6355_TOPSTATUS),
                           (&val),
                           (U32)(PMIC_HOMEKEY_DEB_MASK),
                           (U32)(PMIC_HOMEKEY_DEB_SHIFT)
	                       );

    if (val==1){
        print("[PMIC] pl pmic FCHRKEY Release\n");
        return 0;
    }else{
        print("[PMIC] pl pmic FCHRKEY Press\n");
        return 1;
    }
}

U32 pmic_IsUsbCableIn (void)
{
    U32 ret=0;
    U32 val=0;

#if CFG_EVB_PLATFORM
    val = 1; /*for bring up*/
    /*print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);*/
#else
    ret=pmic_read_interface( (U32)(MT6355_CHR_CON0),
                           (&val),
                           (U32)(PMIC_RGS_CHRDET_MASK),
                           (U32)(PMIC_RGS_CHRDET_SHIFT)
	                       );
    print("[PMIC] pmic_IsUsbCableIn %d\n", val);
#endif

    if(val)
        return PMIC_CHRDET_EXIST;
    else
        return PMIC_CHRDET_NOT_EXIST;
}

void mt6355_upmu_set_baton_tdet_en(U32 val)
{
    U32 ret=0;
    ret=pmic_config_interface( (U32)(PMIC_BATON_TDET_EN_ADDR),
                             (U32)(val),
                             (U32)(PMIC_BATON_TDET_EN_MASK),
                             (U32)(PMIC_BATON_TDET_EN_SHIFT)
	                         );
}

void mt6355_upmu_set_rg_baton_en(U32 val)
{
    U32 ret=0;
    ret=pmic_config_interface( (U32)(PMIC_RG_BATON_EN_ADDR),
                             (U32)(val),
                             (U32)(PMIC_RG_BATON_EN_MASK),
                             (U32)(PMIC_RG_BATON_EN_SHIFT)
	                         );
}

U32 mt6355_upmu_get_rgs_baton_undet(void)
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
  ret=pmic_read_interface( (kal_uint32)(MT6355_CHR_CON0),
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
	pmic_config_interface(MT6355_TOP_RST_MISC, 0x01, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6355_TOP_RST_MISC, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6355_TOP_RST_MISC, (U32)KPD_PMIC_LPRST_TD, PMIC_RG_PWRKEY_RST_TD_MASK, PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#else
	pmic_config_interface(MT6355_TOP_RST_MISC, 0x01, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6355_TOP_RST_MISC, 0x01, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6355_TOP_RST_MISC, (U32)KPD_PMIC_LPRST_TD, PMIC_RG_PWRKEY_RST_TD_MASK, PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#endif
#else
	pmic_config_interface(MT6355_TOP_RST_MISC, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6355_TOP_RST_MISC, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
#endif
#else
	pmic_config_interface(MT6355_TOP_RST_MISC, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6355_TOP_RST_MISC, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
}

U32 PMIC_VUSB_EN(void)
{
	int ret=0;

	ret = pmic_config_interface( (kal_uint32)(MT6355_LDO_VUSB33_CON0_0),
			(kal_uint32)(1),
			(kal_uint32)(PMIC_RG_LDO_VUSB33_EN_0_MASK),
			(kal_uint32)(PMIC_RG_LDO_VUSB33_EN_0_SHIFT)
			);

	return ret;
}

unsigned int wk_auxadc_vsen_tdet_ctrl(unsigned char en_check)
{
	int ret=0;

        print("[PMIC] wk_auxadc_vsen_tdet_ctrl...................\n");
	ret = pmic_config_interface(PMIC_RG_ADCIN_VSEN_MUX_EN_ADDR, 1, PMIC_RG_ADCIN_VSEN_MUX_EN_MASK, PMIC_RG_ADCIN_VSEN_MUX_EN_SHIFT);
	ret = pmic_config_interface(PMIC_BATON_TDET_EN_ADDR, 0, PMIC_BATON_TDET_EN_MASK, PMIC_BATON_TDET_EN_SHIFT);
	return ret;
}

unsigned int wk_gpio_initial(void)
{
	/*GPIO low power setting*/
	U32 vm_mode = 0;
	pmic_read_interface(PMIC_VM_MODE_ADDR, &vm_mode, PMIC_VM_MODE_MASK, PMIC_VM_MODE_SHIFT);
	vm_mode &= 0x3;
	if (vm_mode == 3) {
		print("[PMIC] set VOW_CLK_MISO/GPIO13 pull high in LPDDR3 type.\n");
		/*TYPE_LPDDR3*/
		/*GPIO13: VOW_CLK_MISO pull enable*/
		pmic_config_interface((unsigned int)(MT6355_GPIO_PULLEN0),  1, 1, 13);
		/*GPIO13: VOW_CLK_MISO pull select to high*/
		pmic_config_interface((unsigned int)(MT6355_GPIO_PULLSEL0),  1, 1, 13);
		/*GPIO13: select to GPIO mode. default is VOW_CLK_MISO*/
		pmic_config_interface((unsigned int)(MT6355_GPIO_MODE2),  0, 0x07, 9);
	}
	return 0;
}

unsigned int wk_dram_sleep_voltage(void)
{
	U32 vm_mode = 0;
	pmic_read_interface(PMIC_VM_MODE_ADDR, &vm_mode, PMIC_VM_MODE_MASK, PMIC_VM_MODE_SHIFT);
	vm_mode &= 0x3;
	if (vm_mode == 3) {
		print("[PMIC]LP3 type\n");
		/*TYPE_LPDDR3*/
		pmic_config_interface(0x10A8,0x71,0x7F,0);
		pmic_config_interface(0x1206,0x1,0x1,13);
	}
	return 0;
}

unsigned int wk_vs1_chk(void)
{
	int ret=0;
	unsigned int edata = 0;
	unsigned int vdata = 0;

	ret = pmic_read_interface(PMIC_RG_OTP_DOUT_1120_1135_ADDR, &edata, PMIC_RG_OTP_DOUT_1120_1135_MASK, PMIC_RG_OTP_DOUT_1120_1135_SHIFT);
        print("[PMIC] wk_vs1_chk..................%d\n",(((edata & 0xFFFF) >> 12) & 0x1));
	if ((((edata & 0xFFFF) >> 12) & 0x1)) {
		ret = pmic_read_interface(PMIC_RG_BUCK_VS1_VOSEL_ADDR, &vdata, PMIC_RG_BUCK_VS1_VOSEL_MASK, PMIC_RG_BUCK_VS1_VOSEL_SHIFT);
		ret = pmic_config_interface(PMIC_RG_BUCK_VS1_VOSEL_ADDR, (vdata+2), PMIC_RG_BUCK_VS1_VOSEL_MASK, PMIC_RG_BUCK_VS1_VOSEL_SHIFT);
		print("[PMIC] wk_vs1_chk..................%x\n", vdata);
	}
	return ret;
}

unsigned int wk_vs2_chk(void)
{
	int ret=0;
	unsigned int edata = 0;
	unsigned int vdata = 0;

	ret = pmic_read_interface(PMIC_RG_OTP_DOUT_1120_1135_ADDR, &edata, PMIC_RG_OTP_DOUT_1120_1135_MASK, PMIC_RG_OTP_DOUT_1120_1135_SHIFT);
        print("[PMIC] wk_vs2_chk..................%d\n",(((edata & 0xFFFF) >> 11) & 0x1));
	if ((((edata & 0xFFFF) >> 11) & 0x1)) {
		ret = pmic_read_interface(PMIC_RG_BUCK_VS2_VOSEL_ADDR, &vdata, PMIC_RG_BUCK_VS2_VOSEL_MASK, PMIC_RG_BUCK_VS2_VOSEL_SHIFT);
		ret = pmic_config_interface(PMIC_RG_BUCK_VS2_VOSEL_ADDR, (vdata+1), PMIC_RG_BUCK_VS2_VOSEL_MASK, PMIC_RG_BUCK_VS2_VOSEL_SHIFT);
        	print("[PMIC] wk_vs2_chk..................%x\n", vdata);
	}
	return ret;
}

int pmic_init_setting_array(void)
{
	unsigned int i,init_size;
    	unsigned int ret=0;

	struct pmu_init_s pisac[] = {
	};

	init_size = ARRAY_SIZE(pisac);
	for (i = 0; i < init_size; i++) {
		ret = pmic_config_interface(pisac[i].addr, pisac[i].val, pisac[i].mask, pisac[i].shift);
	}

	return 0;
}

void mt6355_dump_register(void)
{
	unsigned short i = 0;

	print("dump PMIC 6355 register\n");

	for (i = 0; i <= 0x3A00; i = i + 10) {
		print
		    ("Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x\n",
		     i, upmu_get_reg_value(i), i + 2, upmu_get_reg_value(i + 2), i + 4, upmu_get_reg_value(i + 4),
		     i + 6, upmu_get_reg_value(i + 6), i + 8, upmu_get_reg_value(i + 8));
	}

}

void pmic_init_setting(void)
{
    	U32 ret=0;
	print("[PMIC] pmic_init_setting\n");
#if 0
	pmic_init_setting_array();
#else
	ret = pmic_config_interface(0x200,0x1,0x1,5); // [5:5]: RG_SRCLKEN_IN0_HW_MODE; initial, 6/14, Juinn-Ting
	ret = pmic_config_interface(0x200,0x1,0x1,6); // [6:6]: RG_SRCLKEN_IN1_HW_MODE; initial, 6/14, Juinn-Ting
	ret = pmic_config_interface(0x200,0x1,0x1,7); // [7:7]: RG_OSC_SEL_HW_MODE;
	ret = pmic_config_interface(0x204,0x1F,0x1F,8); // [12:8]: RG_MON_GRP_SEL;
	ret = pmic_config_interface(0x21C,0x1,0x1,0); // [0:0]: RG_SMT_WDTRSTB_IN; initial, 6/14, Kim
	ret = pmic_config_interface(0x21C,0x1,0x1,1); // [1:1]: RG_SMT_HOMEKEY; initial, 6/14, Kim
	ret = pmic_config_interface(0x406,0x1,0x1,8); // [8:8]: RG_TRIM_75K_CK_PDN;
	ret = pmic_config_interface(0x412,0x1,0x1,1); // [1:1]: RG_EFUSE_CK_PDN;
	ret = pmic_config_interface(0x418,0x1,0x1,0); // [0:0]: RG_RTCDET_CK_PDN;
	ret = pmic_config_interface(0x418,0x1,0x1,1); // [1:1]: RG_RTC_75K_CK_PDN;
	ret = pmic_config_interface(0x41E,0x1,0x1,3); // [3:3]: RG_75K_32K_SEL;
	ret = pmic_config_interface(0x430,0x1,0x1,7); // [7:7]: RG_REG_CK_PDN_HWEN;
	ret = pmic_config_interface(0x43C,0x3,0x3,0); // [1:0]: RG_VPROC11_FREQ_SEL; SY, 9/19
	ret = pmic_config_interface(0x43C,0x3,0x3,2); // [3:2]: RG_VPROC12_FREQ_SEL; SY, 9/19
	ret = pmic_config_interface(0x43C,0x3,0x3,4); // [5:4]: RG_VCORE_FREQ_SEL; SY, 9/19
	ret = pmic_config_interface(0x43C,0x3,0x3,6); // [7:6]: RG_VGPU_FREQ_SEL; SY, 9/19
	ret = pmic_config_interface(0x60E,0x1,0x1,5); // [5:5]: RG_WDTRSTB_DEB; 6/14, Kim
	ret = pmic_config_interface(0xC00,0x1,0x1,0); // [0:0]: RG_SLP_RW_EN;
	ret = pmic_config_interface(0xE0C,0x1,0x1,3); // [3:3]: RG_STRUP_AUXADC_RSTB_SEL;
	ret = pmic_config_interface(0xE18,0x1,0x1,15); // [15:15]: RG_STRUP_ENVTEM_CTRL; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,0); // [0:0]: RG_STRUP_VA12_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,1); // [1:1]: RG_STRUP_VA10_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,2); // [2:2]: RG_STRUP_VSRAM_GPU_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,3); // [3:3]: RG_STRUP_VSRAM_MD_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,4); // [4:4]: RG_STRUP_VSRAM_CORE_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,5); // [5:5]: RG_STRUP_VA18_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,6); // [6:6]: RG_STRUP_BUCK_RSV_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,7); // [7:7]: RG_STRUP_VDRAM2_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,8); // [8:8]: RG_STRUP_VDRAM1_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,9); // [9:9]: RG_STRUP_VPROC12_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,10); // [10:10]: RG_STRUP_VPROC11_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,11); // [11:11]: RG_STRUP_VS1_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,12); // [12:12]: RG_STRUP_VMODEM_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,13); // [13:13]: RG_STRUP_VGPU_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,14); // [14:14]: RG_STRUP_VCORE_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1C,0x1,0x1,15); // [15:15]: RG_STRUP_VS2_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1E,0x1,0x1,11); // [11:11]: RG_STRUP_EXT_PMIC_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1E,0x1,0x1,12); // [12:12]: RG_STRUP_VUSB33_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1E,0x1,0x1,13); // [13:13]: RG_STRUP_VSRAM_PROC_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1E,0x1,0x1,14); // [14:14]: RG_STRUP_VUFS18_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE1E,0x1,0x1,15); // [15:15]: RG_STRUP_VEMC_PG_H2L_EN; 6/14, kim
	ret = pmic_config_interface(0xE28,0x1,0x1,8); // [8:8]: RG_RTC_SPAR_DEB_EN; 6/14, Scott
	ret = pmic_config_interface(0xE28,0x1,0x1,9); // [9:9]: RG_RTC_ALARM_DEB_EN; 6/14, Scott
	ret = pmic_config_interface(0xE2C,0x1,0x1,6); // [6:6]: RG_RST_DRVSEL; 6/14, Kim
	ret = pmic_config_interface(0xE2C,0x1,0x1,7); // [7:7]: RG_EN_DRVSEL; 6/14, Kim
	ret = pmic_config_interface(0xE2E,0x1,0x1,0); // [0:0]: RG_PWRHOLD; 6/14, kim
	ret = pmic_config_interface(0xE3C,0x4729,0xFFFF,0); // [15:0]: RG_CPS_W_KEY; Joseph 2016/10/13, Please SW write 16'h0000 to lock it
	ret = pmic_config_interface(0xE5A,0x2,0x1F,0); // [4:0]: RG_VS2_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE5A,0x3,0x1F,8); // [12:8]: RG_VSRAM_CORE_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE5C,0x3,0x1F,0); // [4:0]: RG_VSRAM_MD_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE5C,0x1F,0x1F,8); // [12:8]: RG_VSRAM_GPU_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE5E,0x4,0x1F,0); // [4:0]: RG_VCORE_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE5E,0x4,0x1F,8); // [12:8]: RG_VGPU_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE60,0x6,0x1F,0); // [4:0]: RG_VMODEM_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE60,0x6,0x1F,8); // [12:8]: RG_VS1_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE62,0x7,0x1F,0); // [4:0]: RG_VA10_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE62,0x1F,0x1F,8); // [12:8]: RG_VA12_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE64,0x7,0x1F,0); // [4:0]: RG_VIO18_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE64,0x7,0x1F,8); // [12:8]: RG_VEMC_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE66,0x1F,0x1F,0); // [4:0]: RG_VUFS18_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE66,0x8,0x1F,8); // [12:8]: RG_VIO28_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE68,0x4,0x1F,0); // [4:0]: RG_VSRAM_PROC_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE68,0x6,0x1F,8); // [12:8]: RG_VPROC11_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE6A,0x6,0x1F,0); // [4:0]: RG_VPROC12_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE6A,0x6,0x1F,8); // [12:8]: RG_EXT_PMIC_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE6C,0x8,0x1F,0); // [4:0]: RG_VDRAM1_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE6C,0x8,0x1F,8); // [12:8]: RG_VDRAM2_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE6E,0x8,0x1F,0); // [4:0]: RG_VUSB33_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE6E,0x8,0x1F,8); // [12:8]: RG_VXO22_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0xE70,0x8,0x1F,0); // [4:0]: RG_VXO18_DSA; Joseph 2016/10/13
	ret = pmic_config_interface(0x1000,0x1,0x1,1); // [1:1]: RG_BUCK_DCM_MODE;
	ret = pmic_config_interface(0x101E,0x1F,0x7F,0); // [6:0]: RG_BUCK_VPROC11_VOSEL_SLEEP; R2R for Kibo+, 0.6V  (7'b0011111)
	ret = pmic_config_interface(0x1020,0xF,0x7F,0); // [6:0]: RG_BUCK_VPROC11_SFCHG_FRATE; SY, 9/19
	ret = pmic_config_interface(0x1020,0x4,0x7F,8); // [14:8]: RG_BUCK_VPROC11_SFCHG_RRATE; SY, 9/19
	ret = pmic_config_interface(0x1022,0x3,0x3,4); // [5:4]: RG_BUCK_VPROC11_DVS_EN_CTRL; SY, 9/19
	ret = pmic_config_interface(0x1022,0x3,0x3,12); // [13:12]: RG_BUCK_VPROC11_DVS_DOWN_CTRL; SY, 9/19
	ret = pmic_config_interface(0x1040,0x1F,0x7F,0); // [6:0]: RG_BUCK_VPROC12_VOSEL_SLEEP; R2R for Kibo+, 0.6V  (7'b0011111)
	ret = pmic_config_interface(0x1062,0x27,0x7F,0); // [6:0]: RG_BUCK_VCORE_VOSEL_SLEEP; R2R for Kibo+,  0.65V (7'b0100111)
	ret = pmic_config_interface(0x1064,0x11,0x7F,0); // [6:0]: RG_BUCK_VCORE_SFCHG_FRATE; request from Kashi
	ret = pmic_config_interface(0x1064,0xB,0x7F,8); // [14:8]: RG_BUCK_VCORE_SFCHG_RRATE; request from Kashi
	ret = pmic_config_interface(0x1066,0x3,0x3,4); // [5:4]: RG_BUCK_VCORE_DVS_EN_CTRL; SY, 9/19
	ret = pmic_config_interface(0x1066,0x3,0x3,12); // [13:12]: RG_BUCK_VCORE_DVS_DOWN_CTRL; SY, 9/19
	ret = pmic_config_interface(0x1086,0x1A,0x7F,0); // [6:0]: RG_BUCK_VGPU_VOSEL_SLEEP; PDN, R2R for Kibo+, 0.56875V
	ret = pmic_config_interface(0x1088,0xF,0x7F,0); // [6:0]: RG_BUCK_VGPU_SFCHG_FRATE; SY, 9/19
	ret = pmic_config_interface(0x1088,0x4,0x7F,8); // [14:8]: RG_BUCK_VGPU_SFCHG_RRATE; SY, 9/19
	ret = pmic_config_interface(0x108A,0x3,0x3,4); // [5:4]: RG_BUCK_VGPU_DVS_EN_CTRL; SY, 9/19
	ret = pmic_config_interface(0x108A,0x3,0x3,12); // [13:12]: RG_BUCK_VGPU_DVS_DOWN_CTRL; SY, 9/19
	ret = pmic_config_interface(0x10CC,0x11,0x7F,0); // [6:0]: RG_BUCK_VDRAM2_SFCHG_FRATE; Lan
	ret = pmic_config_interface(0x10CC,0x3,0x7F,8); // [14:8]: RG_BUCK_VDRAM2_SFCHG_RRATE; Lan
	ret = pmic_config_interface(0x10CE,0x1,0x3,0); // [1:0]: RG_BUCK_VDRAM2_DVS_EN_TD; 9/9,Lan
	ret = pmic_config_interface(0x10CE,0x3,0x3,4); // [5:4]: RG_BUCK_VDRAM2_DVS_EN_CTRL; 9/9,Lan
	ret = pmic_config_interface(0x10CE,0x1,0x3,8); // [9:8]: RG_BUCK_VDRAM2_DVS_DOWN_TD; 9/9,Lan
	ret = pmic_config_interface(0x10CE,0x3,0x3,12); // [13:12]: RG_BUCK_VDRAM2_DVS_DOWN_CTRL; 9/9,Lan
	ret = pmic_config_interface(0x10EC,0x20,0x7F,0); // [6:0]: RG_BUCK_VMODEM_VOSEL_SLEEP; 6/15,Mason 0.6V for Kibo+ (7'b0100000)
	ret = pmic_config_interface(0x10EE,0x8,0x7F,0); // [6:0]: RG_BUCK_VMODEM_SFCHG_FRATE; Mason,11/14 modified again
	ret = pmic_config_interface(0x10EE,0x4,0x7F,8); // [14:8]: RG_BUCK_VMODEM_SFCHG_RRATE; Hsu-Cheng Ho
	ret = pmic_config_interface(0x10F0,0x1,0x3,4); // [5:4]: RG_BUCK_VMODEM_DVS_EN_CTRL; Hsu-Cheng Ho
	ret = pmic_config_interface(0x10F0,0x0,0x3,8); // [9:8]: RG_BUCK_VMODEM_DVS_DOWN_TD; Hsu-Cheng Ho
	ret = pmic_config_interface(0x1102,0x0,0x3,2); // [3:2]: RG_BUCK_VMODEM_OC_WND; 9/9, Hsu-chen
	ret = pmic_config_interface(0x1116,0x40,0x7F,0); // [6:0]: RG_BUCK_VS1_VOSEL_SLEEP; 2V, R2R for Kibo+
	ret = pmic_config_interface(0x1118,0x23,0x7F,0); // [6:0]: RG_BUCK_VS1_SFCHG_FRATE; 9/9,Lan
	ret = pmic_config_interface(0x1118,0x8,0x7F,8); // [14:8]: RG_BUCK_VS1_SFCHG_RRATE; 9/9,Lan
	ret = pmic_config_interface(0x111A,0x1,0x3,0); // [1:0]: RG_BUCK_VS1_DVS_EN_TD; 9/9,Lan
	ret = pmic_config_interface(0x111A,0x2,0x3,4); // [5:4]: RG_BUCK_VS1_DVS_EN_CTRL; 12/2, Lan for E2
	ret = pmic_config_interface(0x111A,0x1,0x3,8); // [9:8]: RG_BUCK_VS1_DVS_DOWN_TD; 9/9,Lan
	ret = pmic_config_interface(0x111A,0x2,0x3,12); // [13:12]: RG_BUCK_VS1_DVS_DOWN_CTRL; 12/2, Lan for E2
	ret = pmic_config_interface(0x113A,0x35,0x7F,0); // [6:0]: RG_BUCK_VS1_VOTER_VOSEL; 1.86V, Lan
	ret = pmic_config_interface(0x1140,0xC,0x7F,0); // [6:0]: RG_BUCK_VS2_VOSEL_SLEEP; 1.35V, R2R for Kibo+
	ret = pmic_config_interface(0x1142,0x23,0x7F,0); // [6:0]: RG_BUCK_VS2_SFCHG_FRATE; 9/9,Lan
	ret = pmic_config_interface(0x1142,0x8,0x7F,8); // [14:8]: RG_BUCK_VS2_SFCHG_RRATE; 9/9,Lan
	ret = pmic_config_interface(0x1144,0x1,0x3,0); // [1:0]: RG_BUCK_VS2_DVS_EN_TD; 9/9,Lan
	ret = pmic_config_interface(0x1144,0x2,0x3,4); // [5:4]: RG_BUCK_VS2_DVS_EN_CTRL; 12/2, Lan for E2
	ret = pmic_config_interface(0x1144,0x1,0x3,8); // [9:8]: RG_BUCK_VS2_DVS_DOWN_TD; 9/9,Lan
	ret = pmic_config_interface(0x1144,0x2,0x3,12); // [13:12]: RG_BUCK_VS2_DVS_DOWN_CTRL; 12/2, Lan for E2
	ret = pmic_config_interface(0x1164,0x8,0x7F,0); // [6:0]: RG_BUCK_VS2_VOTER_VOSEL; Lan, set at 1.3V
	ret = pmic_config_interface(0x116A,0x1,0x7F,0); // [6:0]: RG_BUCK_VPA_SFCHG_FRATE; JL
	ret = pmic_config_interface(0x116A,0x1,0x7F,8); // [14:8]: RG_BUCK_VPA_SFCHG_RRATE; JL
	ret = pmic_config_interface(0x116C,0x0,0x3,0); // [1:0]: RG_BUCK_VPA_DVS_TRANST_TD; JL
	ret = pmic_config_interface(0x116C,0x3,0x3,4); // [5:4]: RG_BUCK_VPA_DVS_TRANST_CTRL; JL
	ret = pmic_config_interface(0x116C,0x1,0x1,6); // [6:6]: RG_BUCK_VPA_DVS_TRANST_ONCE; JL
	ret = pmic_config_interface(0x116E,0x0,0x1,1); // [1:1]: RG_BUCK_VPA_OC_DEG_EN; JL
	ret = pmic_config_interface(0x1176,0x10,0x3F,0); // [5:0]: RG_BUCK_VPA_VOSEL_DLC011; 6/15, Jian-lung
	ret = pmic_config_interface(0x1176,0x28,0x3F,8); // [13:8]: RG_BUCK_VPA_VOSEL_DLC111; 6/15, Jian-lung
	ret = pmic_config_interface(0x1178,0x8,0x3F,8); // [13:8]: RG_BUCK_VPA_VOSEL_DLC001; 6/15, Jian-lung
	ret = pmic_config_interface(0x117C,0x1,0x1,0); // [0:0]: RG_BUCK_VPA_MSFG_EN; JL
	ret = pmic_config_interface(0x117E,0x5,0x3F,0); // [5:0]: RG_BUCK_VPA_MSFG_RDELTA2GO; JL
	ret = pmic_config_interface(0x117E,0x5,0x3F,8); // [13:8]: RG_BUCK_VPA_MSFG_FDELTA2GO; JL
	ret = pmic_config_interface(0x1180,0x1,0x1F,0); // [4:0]: RG_BUCK_VPA_MSFG_RRATE0; JL
	ret = pmic_config_interface(0x1180,0x1,0x1F,8); // [12:8]: RG_BUCK_VPA_MSFG_RRATE1; JL
	ret = pmic_config_interface(0x1182,0x1,0x1F,0); // [4:0]: RG_BUCK_VPA_MSFG_RRATE2; JL
	ret = pmic_config_interface(0x1182,0x1,0x1F,8); // [12:8]: RG_BUCK_VPA_MSFG_RRATE3; JL
	ret = pmic_config_interface(0x1184,0x1,0x1F,0); // [4:0]: RG_BUCK_VPA_MSFG_RRATE4; JL
	ret = pmic_config_interface(0x1184,0x1,0x1F,8); // [12:8]: RG_BUCK_VPA_MSFG_RRATE5; JL
	ret = pmic_config_interface(0x1186,0xA,0x3F,0); // [5:0]: RG_BUCK_VPA_MSFG_RTHD0; JL
	ret = pmic_config_interface(0x1186,0x14,0x3F,8); // [13:8]: RG_BUCK_VPA_MSFG_RTHD1; JL
	ret = pmic_config_interface(0x1188,0x1E,0x3F,0); // [5:0]: RG_BUCK_VPA_MSFG_RTHD2; JL
	ret = pmic_config_interface(0x1188,0x28,0x3F,8); // [13:8]: RG_BUCK_VPA_MSFG_RTHD3; JL
	ret = pmic_config_interface(0x118A,0x32,0x3F,0); // [5:0]: RG_BUCK_VPA_MSFG_RTHD4; JL
	ret = pmic_config_interface(0x118C,0x1,0x1F,0); // [4:0]: RG_BUCK_VPA_MSFG_FRATE0; JL
	ret = pmic_config_interface(0x118C,0x1,0x1F,8); // [12:8]: RG_BUCK_VPA_MSFG_FRATE1; JL
	ret = pmic_config_interface(0x118E,0x1,0x1F,0); // [4:0]: RG_BUCK_VPA_MSFG_FRATE2; JL
	ret = pmic_config_interface(0x118E,0x1,0x1F,8); // [12:8]: RG_BUCK_VPA_MSFG_FRATE3; JL
	ret = pmic_config_interface(0x1190,0x1,0x1F,0); // [4:0]: RG_BUCK_VPA_MSFG_FRATE4; JL
	ret = pmic_config_interface(0x1190,0x17,0x1F,8); // [12:8]: RG_BUCK_VPA_MSFG_FRATE5; JL
	ret = pmic_config_interface(0x1192,0x32,0x3F,0); // [5:0]: RG_BUCK_VPA_MSFG_FTHD0; JL
	ret = pmic_config_interface(0x1192,0x28,0x3F,8); // [13:8]: RG_BUCK_VPA_MSFG_FTHD1; JL
	ret = pmic_config_interface(0x1194,0x1E,0x3F,0); // [5:0]: RG_BUCK_VPA_MSFG_FTHD2; JL
	ret = pmic_config_interface(0x1194,0x14,0x3F,8); // [13:8]: RG_BUCK_VPA_MSFG_FTHD3; JL
	ret = pmic_config_interface(0x1196,0xA,0x3F,0); // [5:0]: RG_BUCK_VPA_MSFG_FTHD4; JL
	ret = pmic_config_interface(0x1200,0x2,0x3,12); // [13:12]: RG_VPA_BURSTH; JL
	ret = pmic_config_interface(0x1204,0x2,0x3,13); // [14:13]: RG_VCORE_SLEEP_VOLTAGE; 2'h2 for Kibo+
	ret = pmic_config_interface(0x1208,0x6,0x7,13); // [15:13]: RG_VMODEM_SLEEP_VOLTAGE; 3'h6 for Kibo+
	ret = pmic_config_interface(0x120C,0x1,0x1,13); // [13:13]: RG_VS1_SLEEP_VOLTAGE; for Kibo+
	ret = pmic_config_interface(0x1210,0x1,0x1,13); // [13:13]: RG_VDRAM2_SLEEP_VOLTAGE; 12/1 DRAM comment
	ret = pmic_config_interface(0x1212,0x2,0x3,13); // [14:13]: RG_VPROC11_SLEEP_VOLTAGE; 2'h2 for Kibo+
	ret = pmic_config_interface(0x1214,0x2,0x3,13); // [14:13]: RG_VPROC12_SLEEP_VOLTAGE; 2'h2 for Kibo+
	ret = pmic_config_interface(0x1216,0x0,0x3,13); // [14:13]: RG_VSRAM_PROC_SLEEP_VOLTAGE; 2'h0 for Kibo+
	ret = pmic_config_interface(0x1218,0x0,0x7,13); // [15:13]: RG_VSRAM_CORE_SLEEP_VOLTAGE; 3'h0 for Kibo+
	ret = pmic_config_interface(0x121A,0x0,0x3,13); // [14:13]: RG_VSRAM_GPU_SLEEP_VOLTAGE; 2'h0 for Kibo+
	ret = pmic_config_interface(0x121C,0x0,0x7,13); // [15:13]: RG_VSRAM_MD_SLEEP_VOLTAGE; 3'h0 for Kibo+, Kashi 12/28
	ret = pmic_config_interface(0x1222,0x1,0x7,11); // [13:11]: RG_VPROC11_RAMP_SLP; 11/24, E2 Hua-Chiang
	ret = pmic_config_interface(0x1224,0x1,0x7,11); // [13:11]: RG_VPROC12_RAMP_SLP; 11/24, E2 Hua-Chiang
	ret = pmic_config_interface(0x1226,0x1,0x7,3); // [5:3]: RG_VPROC12_RCS; 8/29, Hua-Chiang
	ret = pmic_config_interface(0x1228,0x6,0x7,0); // [2:0]: RG_VPROC11_CSP_TRIM; SY, 9/19
	ret = pmic_config_interface(0x1228,0x6,0x7,3); // [5:3]: RG_VPROC12_CSP_TRIM; SY, 9/19
	ret = pmic_config_interface(0x1228,0x6,0x7,6); // [8:6]: RG_VPROC11_CSN_TRIM; SY, 9/19
	ret = pmic_config_interface(0x1228,0x6,0x7,9); // [11:9]: RG_VPROC12_CSN_TRIM; SY, 9/19
	ret = pmic_config_interface(0x122A,0x5,0xF,0); // [3:0]: RG_VPROC11_NLIM_TRIM; SY, 9/19
	ret = pmic_config_interface(0x122A,0x5,0xF,4); // [7:4]: RG_VPROC12_NLIM_TRIM; SY, 9/19
	ret = pmic_config_interface(0x122A,0x0,0x7,8); // [10:8]: RG_VPROC11_RPSI1_TRIM; SY, 9/19
	ret = pmic_config_interface(0x122A,0x0,0x7,11); // [13:11]: RG_VPROC12_RPSI1_TRIM; SY, 9/19
	ret = pmic_config_interface(0x122C,0x0,0x3,0); // [1:0]: RG_VPROC11_TB_WIDTH; 11/24, E2 hua-chiang
	ret = pmic_config_interface(0x122C,0x0,0x3,2); // [3:2]: RG_VPROC12_TB_WIDTH; 11/24, E2 hua-chiang
	ret = pmic_config_interface(0x122C,0x2,0x3,4); // [5:4]: RG_VPROC11_UG_SR; hua-chiang
	ret = pmic_config_interface(0x122C,0x3,0x3,6); // [7:6]: RG_VPROC11_LG_SR; hua-chiang
	ret = pmic_config_interface(0x122C,0x2,0x3,8); // [9:8]: RG_VPROC12_UG_SR; hua-chiang
	ret = pmic_config_interface(0x122C,0x3,0x3,10); // [11:10]: RG_VPROC12_LG_SR; hua-chiang
	ret = pmic_config_interface(0x122C,0x2,0x7,12); // [14:12]: RG_VPROC11_PFM_TON; hua-chiang
	ret = pmic_config_interface(0x122E,0x0,0x7,0); // [2:0]: RG_VPROC12_PFM_TON; hua-chiang
	ret = pmic_config_interface(0x122E,0x2B,0x3F,3); // [8:3]: RG_VPROC11_TON_TRIM; hua-chiang
	ret = pmic_config_interface(0x122E,0x2B,0x3F,9); // [14:9]: RG_VPROC12_TON_TRIM; hua-chiang
	ret = pmic_config_interface(0x1236,0x3,0x7,0); // [2:0]: RG_VPROC11_COTRAMP_SLP; hua-chiang
	ret = pmic_config_interface(0x1236,0x3,0x7,3); // [5:3]: RG_VPROC12_COTRAMP_SLP; hua-chiang
	ret = pmic_config_interface(0x1236,0x1,0x3,6); // [7:6]: RG_VPROC11_SLEEP_TIME; hua-chiang
	ret = pmic_config_interface(0x1236,0x1,0x3,8); // [9:8]: RG_VPROC12_SLEEP_TIME; hua-chiang
	ret = pmic_config_interface(0x1236,0x0,0x3,10); // [11:10]: RG_VPROC11_VREFTB; 12/8, E2 corner hua-chiang
	ret = pmic_config_interface(0x1236,0x0,0x3,12); // [13:12]: RG_VPROC12_VREFTB; 12/8, E2 corner hua-chiang
	ret = pmic_config_interface(0x123C,0x208,0xFFFF,0); // [15:0]: RG_VPROC11_RSV; hua-chiang, 11/24, for E2
	ret = pmic_config_interface(0x123E,0x208,0xFFFF,0); // [15:0]: RG_VPROC12_RSV; hua-chiang, 11/24, for E2
	ret = pmic_config_interface(0x1244,0x1,0x3,9); // [10:9]: RG_VCORE_CCOMP1; 11/24, E2 huachiang
	ret = pmic_config_interface(0x1244,0x1,0x7,11); // [13:11]: RG_VCORE_RAMP_SLP; 11/24, E2 huachiang
	ret = pmic_config_interface(0x1246,0x1,0x3,9); // [10:9]: RG_VGPU_CCOMP1; 11/24, E2 hua-chiang
	ret = pmic_config_interface(0x1246,0x1,0x7,11); // [13:11]: RG_VGPU_RAMP_SLP; 11/24, E2 hua-chiang
	ret = pmic_config_interface(0x124A,0x6,0x7,0); // [2:0]: RG_VCORE_CSP_TRIM; SY, 9/19
	ret = pmic_config_interface(0x124A,0x6,0x7,6); // [8:6]: RG_VCORE_CSN_TRIM; SY, 9/19
	ret = pmic_config_interface(0x124A,0x1,0x7,9); // [11:9]: RG_VGPU_CSN_TRIM; SY, 9/19
	ret = pmic_config_interface(0x124C,0x5,0xF,0); // [3:0]: RG_VCORE_NLIM_TRIM; SY, 9/19
	ret = pmic_config_interface(0x124C,0x5,0xF,4); // [7:4]: RG_VGPU_NLIM_TRIM; SY, 9/19
	ret = pmic_config_interface(0x124C,0x0,0x7,8); // [10:8]: RG_VCORE_RPSI1_TRIM; SY, 9/19
	ret = pmic_config_interface(0x124C,0x0,0x7,11); // [13:11]: RG_VGPU_RPSI1_TRIM; SY, 9/19
	ret = pmic_config_interface(0x124E,0x0,0x3,0); // [1:0]: RG_VCORE_TB_WIDTH; 11/24, E2 hua-chiang
	ret = pmic_config_interface(0x124E,0x0,0x3,2); // [3:2]: RG_VGPU_TB_WIDTH; 11/24, E2 hua-chiang
	ret = pmic_config_interface(0x124E,0x2,0x3,4); // [5:4]: RG_VCORE_UG_SR; Hua-Chiang
	ret = pmic_config_interface(0x124E,0x3,0x3,6); // [7:6]: RG_VCORE_LG_SR; Hua-Chiang
	ret = pmic_config_interface(0x124E,0x2,0x3,8); // [9:8]: RG_VGPU_UG_SR; Hua-Chiang
	ret = pmic_config_interface(0x124E,0x3,0x3,10); // [11:10]: RG_VGPU_LG_SR; Hua-Chiang
	ret = pmic_config_interface(0x124E,0x2,0x7,12); // [14:12]: RG_VCORE_PFM_TON; Hua-Chiang
	ret = pmic_config_interface(0x1250,0x2,0x7,0); // [2:0]: RG_VGPU_PFM_TON; 11/24, E2 hua-chiang
	ret = pmic_config_interface(0x1250,0x2B,0x3F,3); // [8:3]: RG_VCORE_TON_TRIM; hua-chiang
	ret = pmic_config_interface(0x1250,0x2B,0x3F,9); // [14:9]: RG_VGPU_TON_TRIM; hua-chiang
	ret = pmic_config_interface(0x1258,0x3,0x7,0); // [2:0]: RG_VCORE_COTRAMP_SLP; hua-chiang
	ret = pmic_config_interface(0x1258,0x3,0x7,3); // [5:3]: RG_VGPU_COTRAMP_SLP; hua-chiang
	ret = pmic_config_interface(0x1258,0x1,0x3,6); // [7:6]: RG_VCORE_SLEEP_TIME; hua-chiang
	ret = pmic_config_interface(0x1258,0x1,0x3,8); // [9:8]: RG_VGPU_SLEEP_TIME; hua-chiang
	ret = pmic_config_interface(0x1258,0x2,0x3,10); // [11:10]: RG_VCORE_VREFTB; 11/24, E2 hua-chiang
	ret = pmic_config_interface(0x1258,0x0,0x3,12); // [13:12]: RG_VGPU_VREFTB; 1/4, E2 SY imrpove overshoot
	ret = pmic_config_interface(0x125E,0x60A,0xFFFF,0); // [15:0]: RG_VCORE_RSV; hua-chiang, 11/24 for ECO
	ret = pmic_config_interface(0x1260,0x60A,0xFFFF,0); // [15:0]: RG_VGPU_RSV; hua-chiang, 11/24 for ECO
	ret = pmic_config_interface(0x1264,0x1,0x1,1); // [1:1]: RG_VDRAM1_FCOT; Lan, 11/15 E2 change
	ret = pmic_config_interface(0x1264,0x9,0xF,2); // [5:2]: RG_VDRAM1_RCOMP; Lucas
	ret = pmic_config_interface(0x1266,0x0,0x7,0); // [2:0]: RG_VDRAM1_PFM_TON; Lucas
	ret = pmic_config_interface(0x1266,0x4,0x7,3); // [5:3]: RG_VDRAM1_PWMRAMP_SLP; Lucas
	ret = pmic_config_interface(0x1266,0x6,0x7,6); // [8:6]: RG_VDRAM1_COTRAMP_SLP; Lucas
	ret = pmic_config_interface(0x1268,0x0,0x7,0); // [2:0]: RG_VDRAM1_CSN_TRIM; Lan, 11/15 E2 change
	ret = pmic_config_interface(0x1268,0x0,0x7,3); // [5:3]: RG_VDRAM1_CSP_TRIM; Lan, 11/15 E2 change
	ret = pmic_config_interface(0x1268,0x2,0x7,6); // [8:6]: RG_VDRAM1_RPSI_TRIM; 8/29, Lucas
	ret = pmic_config_interface(0x126A,0x1,0x3,7); // [8:7]: RG_VDRAM1_VREFUP; 8/29,Lucas
	ret = pmic_config_interface(0x126A,0x0,0x3,9); // [10:9]: RG_VDRAM1_TB_WIDTH; Lucas
	ret = pmic_config_interface(0x126A,0x3,0x3,11); // [12:11]: RG_VDRAM1_UG_SR; Lucas
	ret = pmic_config_interface(0x126A,0x3,0x3,13); // [14:13]: RG_VDRAM1_LG_SR; Lucas
	ret = pmic_config_interface(0x126E,0x2,0xFFFF,0); // [15:0]: RG_VDRAM1_RSV; Lucas, 11/4 for E2
	ret = pmic_config_interface(0x1274,0x1,0x1,1); // [1:1]: RG_VDRAM2_FCOT; Lucas
	ret = pmic_config_interface(0x1274,0x5,0xF,2); // [5:2]: RG_VDRAM2_RCOMP; Lucas
	ret = pmic_config_interface(0x1274,0x1,0x1,6); // [6:6]: RG_VDRAM2_TB_DIS; Lucas
	ret = pmic_config_interface(0x1274,0x3,0x3,9); // [10:9]: RG_VDRAM2_ZC_TRIM; Lucas
	ret = pmic_config_interface(0x1276,0x1,0x7,0); // [2:0]: RG_VDRAM2_PFM_TON; Lan, 11/15 E2 change
	ret = pmic_config_interface(0x1276,0x4,0x7,3); // [5:3]: RG_VDRAM2_PWMRAMP_SLP; Lucas
	ret = pmic_config_interface(0x1276,0x6,0x7,6); // [8:6]: RG_VDRAM2_COTRAMP_SLP; Lucas
	ret = pmic_config_interface(0x1278,0x0,0x7,0); // [2:0]: RG_VDRAM2_CSN_TRIM; Lan, 11/15 E2 change
	ret = pmic_config_interface(0x1278,0x0,0x7,3); // [5:3]: RG_VDRAM2_CSP_TRIM; Lan, 11/15 E2 change
	ret = pmic_config_interface(0x127A,0x3,0x3,7); // [8:7]: RG_VDRAM2_VREFUP; Lucas
	ret = pmic_config_interface(0x127A,0x0,0x3,9); // [10:9]: RG_VDRAM2_TB_WIDTH; Lucas
	ret = pmic_config_interface(0x127A,0x3,0x3,11); // [12:11]: RG_VDRAM2_UG_SR; Lucas
	ret = pmic_config_interface(0x127A,0x3,0x3,13); // [14:13]: RG_VDRAM2_LG_SR; Lucas
	ret = pmic_config_interface(0x127E,0x3,0xFFFF,0); // [15:0]: RG_VDRAM2_RSV; 9/9,Lan, 11/4 for E2
	ret = pmic_config_interface(0x1286,0x1,0xF,3); // [6:3]: RG_VMODEM_RZSEL1; Mason ,HC 11/3
	ret = pmic_config_interface(0x1286,0x1,0x3,7); // [8:7]: RG_VMODEM_CCSEL0; Hsu-Cheng Ho
	ret = pmic_config_interface(0x1286,0x1,0x3,9); // [10:9]: RG_VMODEM_CCSEL1; Hsu-Cheng Ho
	ret = pmic_config_interface(0x1288,0x5,0x7,0); // [2:0]: RG_VMODEM_SLP; 12/22, reduce overshoot
	ret = pmic_config_interface(0x1288,0x0,0x1,3); // [3:3]: RG_VMODEM_ADRC_FEN; 9/9, Hsu-Cheng Ho
	ret = pmic_config_interface(0x1288,0x0,0x1,4); // [4:4]: RG_VMODEM_VC_CAP_CLAMP_FEN; Hsu-Cheng Ho
	ret = pmic_config_interface(0x1288,0x0,0x1,5); // [5:5]: RG_VMODEM_VC_CLAMP_FEN; Hsu-Cheng Ho
	ret = pmic_config_interface(0x1288,0x1,0x7,9); // [11:9]: RG_VMODEM_CSR; 11/14, E2 Mason
	ret = pmic_config_interface(0x128E,0x1,0x3,2); // [3:2]: RG_VMODEM_SR_N; Hsu-Cheng Ho,Load Transient ring overshoot
	ret = pmic_config_interface(0x128E,0x2,0x7,4); // [6:4]: RG_VMODEM_PFM_RIP; Hsu-Cheng Ho
	ret = pmic_config_interface(0x128E,0x7,0x3F,7); // [12:7]: RG_VMODEM_TRAN_BST; 9/9, Hsu-Cheng Ho
	ret = pmic_config_interface(0x1298,0x1,0x7,4); // [6:4]: RG_VS1_RZSEL; Chc
	ret = pmic_config_interface(0x1298,0x5,0x7,7); // [9:7]: RG_VS1_CSR; Lan, 12/2 for E2
	ret = pmic_config_interface(0x129A,0x2,0x7,0); // [2:0]: RG_VS1_SLP; Lan, 12/2 for E2
	ret = pmic_config_interface(0x12A2,0x2,0x3,5); // [6:5]: RG_VS1_SR_P; Chc
	ret = pmic_config_interface(0x12A2,0x2,0x3,7); // [8:7]: RG_VS1_SR_N; Chc
	ret = pmic_config_interface(0x12A2,0x4,0x7,9); // [11:9]: RG_VS1_BURST; Lan, 12/13 for E2
	ret = pmic_config_interface(0x12A6,0x3,0x7,4); // [6:4]: RG_VS2_RZSEL; Chc
	ret = pmic_config_interface(0x12A6,0x1,0x7,7); // [9:7]: RG_VS2_CSR; Lan, 11/15 E2 change
	ret = pmic_config_interface(0x12B0,0x2,0x3,5); // [6:5]: RG_VS2_SR_P; Chc
	ret = pmic_config_interface(0x12B0,0x2,0x3,7); // [8:7]: RG_VS2_SR_N; Chc
	ret = pmic_config_interface(0x12B0,0x5,0x7,9); // [11:9]: RG_VS2_BURST; Chc,9/21
	ret = pmic_config_interface(0x12B6,0x1,0x3,4); // [5:4]: RG_VPA_CSMIR; JL
	ret = pmic_config_interface(0x12B6,0x1,0x3,6); // [7:6]: RG_VPA_CSL; JL, improve DVFS setting time
	ret = pmic_config_interface(0x12B6,0x0,0x1,10); // [10:10]: RG_VPA_AZC_EN; JL
	ret = pmic_config_interface(0x12B8,0x5,0xF,8); // [11:8]: RG_VPA_NLIM_SEL; Mason/JL, 11/16 E2
	ret = pmic_config_interface(0x12B8,0x3,0x3,14); // [15:14]: RG_VPA_SLEW; JL
	ret = pmic_config_interface(0x12BA,0x3,0x3,0); // [1:0]: RG_VPA_SLEW_NMOS; JL
	ret = pmic_config_interface(0x12BA,0x0,0x3,2); // [3:2]: RG_VPA_MIN_ON; JL
	ret = pmic_config_interface(0x12BA,0x0,0x3,14); // [15:14]: RG_VPA_MIN_PK; JL
	ret = pmic_config_interface(0x12BC,0x86,0xFF,0); // [7:0]: RG_VPA_RSV1; JL
	ret = pmic_config_interface(0x12BC,0x88,0xFF,8); // [15:8]: RG_VPA_RSV2; JL
	ret = pmic_config_interface(0x1610,0x1,0x1,9); // [9:9]: RG_LDO_VIO28_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1624,0x1,0x1,9); // [9:9]: RG_LDO_VIO18_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1638,0x1,0x1,9); // [9:9]: RG_LDO_VUFS18_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x164E,0x0,0x1,1); // [1:1]: RG_LDO_VA10_SLEEP_MODE; Teddy
	ret = pmic_config_interface(0x164E,0x1,0x1,9); // [9:9]: RG_LDO_VA10_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1662,0x1,0x1,9); // [9:9]: RG_LDO_VA12_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1676,0x1,0x1,9); // [9:9]: RG_LDO_VA18_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x168C,0x1,0x1,9); // [9:9]: RG_LDO_VUSB33_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x16A0,0x1,0x1,9); // [9:9]: RG_LDO_VEMC_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x16B4,0x1,0x1,9); // [9:9]: RG_LDO_VXO22_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x16C8,0x1,0x1,9); // [9:9]: RG_LDO_VXO18_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x16DC,0x1,0x1,9); // [9:9]: RG_LDO_VSIM1_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x16F0,0x1,0x1,9); // [9:9]: RG_LDO_VSIM2_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1704,0x1,0x1,9); // [9:9]: RG_LDO_VCAMD1_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1718,0x1,0x1,9); // [9:9]: RG_LDO_VCAMD2_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x172C,0x1,0x1,9); // [9:9]: RG_LDO_VCAMIO_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1740,0x1,0x1,9); // [9:9]: RG_LDO_VMIPI_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1754,0x1,0x1,9); // [9:9]: RG_LDO_VGP_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x176A,0x1,0x1,9); // [9:9]: RG_LDO_VCN33_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x177E,0x1,0x1,9); // [9:9]: RG_LDO_VCN18_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1792,0x1,0x1,9); // [9:9]: RG_LDO_VCN28_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x17A6,0x1,0x1,9); // [9:9]: RG_LDO_VBIF28_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x17BA,0x1,0x1,9); // [9:9]: RG_LDO_VTCXO24_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x17D0,0x1,0x1,9); // [9:9]: RG_LDO_VLDO28_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x17E4,0x1,0x1,9); // [9:9]: RG_LDO_VGP2_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1810,0x1,0x1,9); // [9:9]: RG_LDO_VFE28_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1824,0x1,0x1,9); // [9:9]: RG_LDO_VMCH_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1838,0x1,0x1,9); // [9:9]: RG_LDO_VMC_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x184C,0x1,0x1,9); // [9:9]: RG_LDO_VRF18_1_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1860,0x1,0x1,9); // [9:9]: RG_LDO_VRF18_2_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1874,0x1,0x1,9); // [9:9]: RG_LDO_VRF12_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1888,0x1,0x1,9); // [9:9]: RG_LDO_VCAMA1_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x189C,0x1,0x1,9); // [9:9]: RG_LDO_VCAMA2_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x18AC,0xD,0x7F,0); // [6:0]: RG_LDO_VSRAM_PROC_VOSEL_SLEEP; Fandy, 7'hd R2R for (0.6V, Kibo+)
	ret = pmic_config_interface(0x18AE,0xF,0x7F,0); // [6:0]: RG_LDO_VSRAM_PROC_SFCHG_FRATE; 9/13, KY_Fang
	ret = pmic_config_interface(0x18AE,0x4,0x7F,8); // [14:8]: RG_LDO_VSRAM_PROC_SFCHG_RRATE; Teddy
	ret = pmic_config_interface(0x18C0,0x1,0x1,9); // [9:9]: RG_LDO_VSRAM_PROC_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x18CC,0xD,0x7F,0); // [6:0]: RG_LDO_VSRAM_CORE_VOSEL_SLEEP; Chi-Huan, R2R sleep(0.6V, Kibo+),9/23
	ret = pmic_config_interface(0x18CE,0x11,0x7F,0); // [6:0]: RG_LDO_VSRAM_CORE_SFCHG_FRATE; 10/7, by Kashi
	ret = pmic_config_interface(0x18CE,0xB,0x7F,8); // [14:8]: RG_LDO_VSRAM_CORE_SFCHG_RRATE; 9/13, KY_Fang
	ret = pmic_config_interface(0x18E0,0x1,0x1,9); // [9:9]: RG_LDO_VSRAM_CORE_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x18EC,0xD,0x7F,0); // [6:0]: RG_LDO_VSRAM_GPU_VOSEL_SLEEP; Chi-Huan, R2R sleep(0.6V, Kibo+)
	ret = pmic_config_interface(0x18EE,0xF,0x7F,0); // [6:0]: RG_LDO_VSRAM_GPU_SFCHG_FRATE; 9/13, KY_Fang
	ret = pmic_config_interface(0x18EE,0x4,0x7F,8); // [14:8]: RG_LDO_VSRAM_GPU_SFCHG_RRATE; Teddy
	ret = pmic_config_interface(0x1900,0x1,0x1,9); // [9:9]: RG_LDO_VSRAM_GPU_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x190C,0xD,0x7F,0); // [6:0]: RG_LDO_VSRAM_MD_VOSEL_SLEEP; 12/28, Kashi change to 7'h0d R2R for (0.6V, Kibo+)
	ret = pmic_config_interface(0x190E,0x8,0x7F,0); // [6:0]: RG_LDO_VSRAM_MD_SFCHG_FRATE; 11/13, modified follow MODEM
	ret = pmic_config_interface(0x190E,0x4,0x7F,8); // [14:8]: RG_LDO_VSRAM_MD_SFCHG_RRATE; 9/13, KY_Fang
	ret = pmic_config_interface(0x1920,0x1,0x1,9); // [9:9]: RG_LDO_VSRAM_MD_OCFB_EN; 9/13, Chi-Huan
	ret = pmic_config_interface(0x1928,0x0,0x1,0); // [0:0]: RG_LDO_VSRAM_CORE_TRACK_SLEEP_CTRL; enable SRAM_CORE HW auto tracking, only for Alaska Kibo+ should be 1'b0
	ret = pmic_config_interface(0x1928,0x0,0x1,1); // [1:1]: RG_LDO_VSRAM_CORE_TRACK_ON_CTRL; enable SRAM_CORE HW auto tracking, only for Alaska Kibo+ should be 1'b0
	ret = pmic_config_interface(0x1928,0x0,0x1,2); // [2:2]: RG_LDO_VSRAM_CORE_TRACK_VBUCK_ON_CTRL; enable SRAM_CORE HW auto tracking, only for Alaska Kibo+ should be 1'b0
	ret = pmic_config_interface(0x192A,0x4,0x7F,0); // [6:0]: RG_LDO_VSRAM_CORE_VOSEL_DELTA; Fandy (0.025V)
	ret = pmic_config_interface(0x192A,0x10,0x7F,8); // [14:8]: RG_LDO_VSRAM_CORE_VOSEL_OFFSET; Fandy (0.1V)
	ret = pmic_config_interface(0x192C,0xC,0x7F,0); // [6:0]: RG_LDO_VSRAM_CORE_VOSEL_ON_LB; Fandy (0.6V)
	ret = pmic_config_interface(0x192C,0x7D,0x7F,8); // [14:8]: RG_LDO_VSRAM_CORE_VOSEL_ON_HB; Fandy (1.3V)
	ret = pmic_config_interface(0x192E,0x5,0x7F,0); // [6:0]: RG_LDO_VSRAM_CORE_VOSEL_SLEEP_LB; Fandy (0.55V)
	ret = pmic_config_interface(0x1930,0x1,0x1,0); // [0:0]: RG_LDO_DCM_MODE;
	ret = pmic_config_interface(0x1932,0x0,0x1,0); // [0:0]: RG_LDO_VIO28_CK_SW_MODE;
	ret = pmic_config_interface(0x1934,0x0,0x1,0); // [0:0]: RG_LDO_VIO18_CK_SW_MODE;
	ret = pmic_config_interface(0x1936,0x0,0x1,0); // [0:0]: RG_LDO_VUFS18_CK_SW_MODE;
	ret = pmic_config_interface(0x1938,0x0,0x1,0); // [0:0]: RG_LDO_VA10_CK_SW_MODE;
	ret = pmic_config_interface(0x193A,0x0,0x1,0); // [0:0]: RG_LDO_VA12_CK_SW_MODE;
	ret = pmic_config_interface(0x193C,0x0,0x1,0); // [0:0]: RG_LDO_VSRAM_PROC_CK_SW_MODE;
	ret = pmic_config_interface(0x193E,0x0,0x1,0); // [0:0]: RG_LDO_VSRAM_CORE_CK_SW_MODE;
	ret = pmic_config_interface(0x1940,0x0,0x1,0); // [0:0]: RG_LDO_VSRAM_GPU_CK_SW_MODE;
	ret = pmic_config_interface(0x1942,0x0,0x1,0); // [0:0]: RG_LDO_VSRAM_MD_CK_SW_MODE;
	ret = pmic_config_interface(0x1944,0x0,0x1,0); // [0:0]: RG_LDO_VA18_CK_SW_MODE;
	ret = pmic_config_interface(0x1946,0x0,0x1,0); // [0:0]: RG_LDO_VUSB33_CK_SW_MODE;
	ret = pmic_config_interface(0x1948,0x0,0x1,0); // [0:0]: RG_LDO_VEMC_CK_SW_MODE;
	ret = pmic_config_interface(0x194A,0x0,0x1,0); // [0:0]: RG_LDO_VXO22_CK_SW_MODE;
	ret = pmic_config_interface(0x194C,0x0,0x1,0); // [0:0]: RG_LDO_VXO18_CK_SW_MODE;
	ret = pmic_config_interface(0x194E,0x0,0x1,0); // [0:0]: RG_LDO_VSIM1_CK_SW_MODE;
	ret = pmic_config_interface(0x1950,0x0,0x1,0); // [0:0]: RG_LDO_VSIM2_CK_SW_MODE;
	ret = pmic_config_interface(0x1952,0x0,0x1,0); // [0:0]: RG_LDO_VCAMD1_CK_SW_MODE;
	ret = pmic_config_interface(0x1954,0x0,0x1,0); // [0:0]: RG_LDO_VCAMD2_CK_SW_MODE;
	ret = pmic_config_interface(0x1956,0x0,0x1,0); // [0:0]: RG_LDO_VCAMIO_CK_SW_MODE;
	ret = pmic_config_interface(0x1958,0x0,0x1,0); // [0:0]: RG_LDO_VMIPI_CK_SW_MODE;
	ret = pmic_config_interface(0x195A,0x0,0x1,0); // [0:0]: RG_LDO_VGP_CK_SW_MODE;
	ret = pmic_config_interface(0x195C,0x0,0x1,0); // [0:0]: RG_LDO_VCN33_CK_SW_MODE;
	ret = pmic_config_interface(0x195E,0x0,0x1,0); // [0:0]: RG_LDO_VCN18_CK_SW_MODE;
	ret = pmic_config_interface(0x1960,0x0,0x1,0); // [0:0]: RG_LDO_VCN28_CK_SW_MODE;
	ret = pmic_config_interface(0x1962,0x0,0x1,0); // [0:0]: RG_LDO_VGP2_CK_SW_MODE;
	ret = pmic_config_interface(0x1964,0x0,0x1,0); // [0:0]: RG_LDO_VBIF28_CK_SW_MODE;
	ret = pmic_config_interface(0x1966,0x0,0x1,0); // [0:0]: RG_LDO_VFE28_CK_SW_MODE;
	ret = pmic_config_interface(0x1968,0x0,0x1,0); // [0:0]: RG_LDO_VMCH_CK_SW_MODE;
	ret = pmic_config_interface(0x196A,0x0,0x1,0); // [0:0]: RG_LDO_VMC_CK_SW_MODE;
	ret = pmic_config_interface(0x196C,0x0,0x1,0); // [0:0]: RG_LDO_VRF18_1_CK_SW_MODE;
	ret = pmic_config_interface(0x196E,0x0,0x1,0); // [0:0]: RG_LDO_VRF18_2_CK_SW_MODE;
	ret = pmic_config_interface(0x1970,0x0,0x1,0); // [0:0]: RG_LDO_VTCXO24_CK_SW_MODE;
	ret = pmic_config_interface(0x1972,0x0,0x1,0); // [0:0]: RG_LDO_VLDO28_CK_SW_MODE;
	ret = pmic_config_interface(0x1974,0x0,0x1,0); // [0:0]: RG_LDO_VRF12_CK_SW_MODE;
	ret = pmic_config_interface(0x1976,0x0,0x1,0); // [0:0]: RG_LDO_VCAMA1_CK_SW_MODE;
	ret = pmic_config_interface(0x1978,0x0,0x1,0); // [0:0]: RG_LDO_VCAMA2_CK_SW_MODE;
	ret = pmic_config_interface(0x1A2E,0x1,0x1,12); // [12:12]: RG_VSIM1_STB_SEL; 5/2: Fandy for SIM issue
	ret = pmic_config_interface(0x1A32,0x1,0x1,12); // [12:12]: RG_VSIM2_STB_SEL; 5/2: Fandy for SIM issue
	ret = pmic_config_interface(0x2000,0x2,0x3,3); // [4:3]: XO_EXTBUF2_MODE; David
	ret = pmic_config_interface(0x2000,0x3,0x3,6); // [7:6]: XO_EXTBUF3_MODE; David
	ret = pmic_config_interface(0x2000,0x3,0x3,9); // [10:9]: XO_EXTBUF4_MODE; David
	ret = pmic_config_interface(0x2000,0x0,0x1,13); // [13:13]: XO_ENBB_MAN; David
	ret = pmic_config_interface(0x2008,0x1,0x3,7); // [8:7]: XO_AAC_MODE_LPM; David
	ret = pmic_config_interface(0x2008,0x1,0x3,9); // [10:9]: XO_AAC_MODE_FPM; David
	ret = pmic_config_interface(0x2016,0x4,0xF,5); // [8:5]: XO_AAC_VSEL_LPM; Chinchung Wang
	ret = pmic_config_interface(0x201A,0x3,0x3,8); // [9:8]: XO_EXTBUF6_MODE; David
	ret = pmic_config_interface(0x201A,0x3,0x3,11); // [12:11]: XO_EXTBUF7_MODE; David
	ret = pmic_config_interface(0x2024,0x1,0x1,12); // [12:12]: XO_EXTBUF2_CLKSEL_MAN; John_C, 11/11 E2
	ret = pmic_config_interface(0x2024,0x0,0x1,13); // [13:13]: XO_AUDIO_EN_M; David, 12/23 E2
	ret = pmic_config_interface(0x2028,0x1,0x3,0); // [1:0]: XO_EXTBUF1_ISET_M; John_Chiang
	ret = pmic_config_interface(0x2028,0x1,0x3,2); // [3:2]: XO_EXTBUF2_ISET_M; John_Chiang
	ret = pmic_config_interface(0x2028,0x1,0x3,4); // [5:4]: XO_EXTBUF3_ISET_M; John_Chiang
	ret = pmic_config_interface(0x2028,0x1,0x3,6); // [7:6]: XO_EXTBUF4_ISET_M; John_Chiang
	ret = pmic_config_interface(0x2028,0x1,0x3,10); // [11:10]: XO_EXTBUF6_ISET_M; John_Chiang
	ret = pmic_config_interface(0x2028,0x1,0x3,12); // [13:12]: XO_EXTBUF7_ISET_M; John_Chiang
	ret = pmic_config_interface(0x2216,0x2,0x1F,6); // [10:6]: RG_LBAT_INT_VTH;
	ret = pmic_config_interface(0x3072,0x1,0x1,6); // [6:6]: FG_RNG_EN_MODE; Wei-Lin
	ret = pmic_config_interface(0x3072,0x0,0x1,7); // [7:7]: FG_RNG_EN_SW; Wei-Lin
	ret = pmic_config_interface(0x32AC,0x0,0x1,15); // [15:15]: AUXADC_CK_AON; Peter-SW
	ret = pmic_config_interface(0x32B4,0x93,0xFFF,0); // [11:0]: AUXADC_AVG_NUM_SEL; Jyun-Jia, 9/29 for U8S
	ret = pmic_config_interface(0x32B4,0x1,0x1,15); // [15:15]: AUXADC_AVG_NUM_SEL_WAKEUP; Jyun-Jia
	ret = pmic_config_interface(0x32B6,0x20,0x3FF,0); // [9:0]: AUXADC_SPL_NUM_LARGE; Jyun-Jia
	ret = pmic_config_interface(0x32BA,0x1,0x1,14); // [14:14]: AUXADC_SPL_NUM_SEL_BAT_TEMP; Jyun-Jia
	ret = pmic_config_interface(0x32BC,0x7,0x3FF,0); // [9:0]: AUXADC_SPL_NUM_CH0; Jyun-Jia
	ret = pmic_config_interface(0x32BE,0x20,0x3FF,0); // [9:0]: AUXADC_SPL_NUM_CH3; Jyun-Jia
	ret = pmic_config_interface(0x32C0,0x7,0x3FF,0); // [9:0]: AUXADC_SPL_NUM_CH7; Jyun-Jia
	ret = pmic_config_interface(0x32C2,0x6,0x7,4); // [6:4]: AUXADC_AVG_NUM_CH7; Jyun-Jia
	ret = pmic_config_interface(0x32C2,0x6,0x7,12); // [14:12]: AUXADC_AVG_NUM_CH0; Jyun-Jia
	ret = pmic_config_interface(0x32C4,0x6,0x7,4); // [6:4]: AUXADC_AVG_NUM_DCXO; Jyun-Jia
	ret = pmic_config_interface(0x32C6,0x1,0x3,4); // [5:4]: AUXADC_TRIM_CH2_SEL; 6/17, Jyun-Jia
	ret = pmic_config_interface(0x32C6,0x3,0x3,6); // [7:6]: AUXADC_TRIM_CH3_SEL; 6/17, Jyun-Jia
	ret = pmic_config_interface(0x32C6,0x1,0x3,8); // [9:8]: AUXADC_TRIM_CH4_SEL; 6/17, Jyun-Jia
	ret = pmic_config_interface(0x32C6,0x1,0x3,10); // [11:10]: AUXADC_TRIM_CH5_SEL; 6/17, Jyun-Jia
	ret = pmic_config_interface(0x32C6,0x1,0x3,12); // [13:12]: AUXADC_TRIM_CH6_SEL; 6/17, Jyun-Jia
	ret = pmic_config_interface(0x32C6,0x2,0x3,14); // [15:14]: AUXADC_TRIM_CH7_SEL; 6/17, Jyun-Jia
	ret = pmic_config_interface(0x32C8,0x1,0x3,0); // [1:0]: AUXADC_TRIM_CH8_SEL; Jyun-Jia
	ret = pmic_config_interface(0x32C8,0x1,0x3,2); // [3:2]: AUXADC_TRIM_CH9_SEL; Jyun-Jia
	ret = pmic_config_interface(0x32C8,0x1,0x3,4); // [5:4]: AUXADC_TRIM_CH10_SEL; Jyun-Jia
	ret = pmic_config_interface(0x32C8,0x1,0x3,6); // [7:6]: AUXADC_TRIM_CH11_SEL; Jyun-Jia
	ret = pmic_config_interface(0x32CE,0x1,0x1,0); // [0:0]: AUXADC_RNG_EN; 9/29, Jyun-Jia
	ret = pmic_config_interface(0x32DA,0x1,0x1,14); // [14:14]: AUXADC_START_SHADE_EN; set in initial setting, Kashi
	ret = pmic_config_interface(0x32DE,0x0,0x3,0); // [1:0]: AUXADC_DATA_REUSE_SEL; Peter-SW
	ret = pmic_config_interface(0x32DE,0x1,0x1,8); // [8:8]: AUXADC_DATA_REUSE_EN; Peter-SW
	ret = pmic_config_interface(0x32F0,0x1,0x1,0); // [0:0]: AUXADC_ACCDET_AUTO_SPL; Peter-SW, 11/24 E2
	ret = pmic_config_interface(0x3336,0x40,0x3FF,0); // [9:0]: AUXADC_MDRT_DET_PRD; Peter-SW
	ret = pmic_config_interface(0x3336,0x1,0x1,15); // [15:15]: AUXADC_MDRT_DET_EN; Peter-SW
	ret = pmic_config_interface(0x333A,0x1,0x1,2); // [2:2]: AUXADC_MDRT_DET_WKUP_EN; Peter-SW
	ret = pmic_config_interface(0x333E,0x1,0x1,0); // [0:0]: AUXADC_MDRT_DET_START_SEL; L1, Modem, MY_Tu
	ret = pmic_config_interface(0x3370,0x0,0x1,0); // [0:0]: RG_AUXADC_IMP_CK_SW_MODE; Restore to default, Peter-SW 10/12
	ret = pmic_config_interface(0x3372,0x0,0x1,0); // [0:0]: RG_AUXADC_LBAT_CK_SW_MODE; Restore to default, Peter-SW 10/12
	ret = pmic_config_interface(0x3374,0x0,0x1,0); // [0:0]: RG_AUXADC_THR_CK_SW_MODE; Restore to default, Peter-SW 10/12
	ret = pmic_config_interface(0x3376,0x0,0x1,0); // [0:0]: RG_AUXADC_BAT_TEMP_CK_SW_MODE; Restore to default, Peter-SW 10/12
	ret = pmic_config_interface(0x3378,0x0,0x1,0); // [0:0]: RG_AUXADC_LBAT2_CK_SW_MODE; Restore to default, Peter-SW 10/12
	ret = pmic_config_interface(0x337A,0x0,0x1,0); // [0:0]: RG_AUXADC_JEITA_CK_SW_MODE; Restore to default, Peter-SW 10/12
	ret = pmic_config_interface(0x337C,0x0,0x1,0); // [0:0]: RG_AUXADC_NAG_CK_SW_MODE; Restore to default, Peter-SW 10/12
	print("[PMIC]pmic_init_setting end\n");
#endif
	/*--Since power off sequence unlock by writing 4729, we need to write 0 to lock it--*/
	ret = pmic_config_interface(0xE3C,0,0xFFFF,0); // [15:0]: RG_CPS_W_KEY; Joseph 2016/10/13, Please SW write 16'h0000 to lock it
	/*mt6355_dump_register();*/
	wk_vs1_chk();
	wk_vs2_chk();
	wk_dram_sleep_voltage();

	/*GPIO low power setting*/
	wk_gpio_initial();
}
/*
 * PMIC Export API
 */
static U32 g_sts_crst;

/* check if PMIC cold reset at previous power off */
U32 is_pmic_cold_reset(void)
{
	return g_sts_crst;
}

int PMIC_POWER_HOLD(unsigned int hold)
{
	int ret = 0;
	if (hold > 1) {
		print("[PMIC] PMIC_POWER_HOLD hold = %d only 0 or 1\n", hold);
		return -1;
	}

	if (hold)
		print("[PMIC] PMIC_POWER_HOLD ON\n");
	else
		print("[PMIC] PMIC_POWER_HOLD OFF\n");
    	/* MT6355 must keep power hold */
	ret=pmic_config_interface(PMIC_RG_PWRHOLD_ADDR, hold,
				PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);

	print("[PMIC] MT6355 PowerHold = 0x%x\n",
			upmu_get_reg_value(MT6355_PPCCTL0));
	return 0;
}

void pmic_disable_usbdl_wo_battery(void)
{
    	/* ask shin-shyu programming guide TBD */
	/* write 1 to ULC_DET_EN */
	/* write 0 to USB_DL_SET */
	/* write 1 to USB_DL_RST */
	print("[PMIC] turn off usbdl wo battery..................\n");
	/*pmic_config_interface(PMIC_RG_ULC_DET_EN_ADDR,1,PMIC_RG_ULC_DET_EN_MASK,PMIC_RG_ULC_DET_EN_SHIFT);*//*---MT6336 TBD---*/
	/*pmic_config_interface(PMIC_RG_USBDL_SET_ADDR, 0x0000, PMIC_RG_USBDL_SET_MASK, PMIC_RG_USBDL_SET_SHIFT);
	pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 0x0001, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);*/
}

void pmic_smps_testmode_b(void)
{
	/* To avoid buck is still enable, but ivegen is not exist */
	/* All buck en_anable will be en_vxxx= (~IVGENENB & IVGEN_READY )& AD_QI_VCORE_EN */
	/* ps. ivegen means reference current source */
	int ret_val=0;
	print("[PMIC] pmic_smps_testmode_b\n");
	ret_val= pmic_config_interface(PMIC_RG_SMPS_TESTMODE_B_ADDR, 0x0001,
		PMIC_RG_SMPS_TESTMODE_B_MASK, PMIC_RG_SMPS_TESTMODE_B_SHIFT); /*--RG_SMPS_TESTMODE_B by luke--*/
}

void pmic_pwrhold_info_to_gauge(void)
{
	/* It might not need beside MT6355 */
	/* read pwrhold reg val */
	/* write to specific register of rtc */
	/* Gauge user would get this value from this during kernel stage */
	U16 pwhold = 0;
	print("[PMIC] pmic_pwrhold_info_to_gauge\n");
	pwhold = upmu_get_reg_value(MT6355_PPCCTL0);
	print("[PMIC] MT6355 PPCCTL = 0x%x\n", pwhold);
	pwhold &= 0x1;
	rtc_rdwr_pwrhold_bits(&pwhold);
}

void pmic_dbg_status(unsigned char option)
{
	/*--option = 1--*/
	/* UVLO off TBD */
	/* Write 1 to clear PONSTS and POFFSTS TBD */
	/* power not good TBD */
	/* buck oc TBD */
	/* thermal shutdown 150 TBD */
	/* long press shutdown TBD */
	/* WDTRST TBD */
	/* CLK TRIM TBD */
	/*--option = 0--*/
	/* After writing 1, need to write 0 back TBD */

	/*--UVLO off--*/
	int ret_val = 0;
	unsigned int poff_sts = 0;

	if (option) {
	/*pwhold must set to 1 to update debug status*/
		PMIC_POWER_HOLD(1);
		print("[PMIC][status] TOP_RST_STATUS[0x%x]=0x%x\n",
			MT6355_TOP_RST_STATUS, upmu_get_reg_value(MT6355_TOP_RST_STATUS));
		print("[PMIC][status] PONSTS[0x%x]=0x%x\n",
			MT6355_PONSTS, upmu_get_reg_value(MT6355_PONSTS));
	/*special for cold rest*/
		poff_sts = upmu_get_reg_value(MT6355_POFFSTS);
		g_sts_crst = (poff_sts >> PMIC_STS_CRST_SHIFT) & PMIC_STS_CRST_MASK;
		print("[PMIC][status] POFFSTS[0x%x]=0x%x\n",
			MT6355_POFFSTS, poff_sts);
	/*power not good*/
		print("[PMIC][status] PGSTATUS0[0x%x]=0x%x\n",
			MT6355_PGSTATUS0, upmu_get_reg_value(MT6355_PGSTATUS0));
		print("[PMIC][status] PGSTATUS1[0x%x]=0x%x\n",
			MT6355_PGSTATUS1, upmu_get_reg_value(MT6355_PGSTATUS1));
	/*--buck oc--*/
		print("[PMIC][status] PSOCSTATUS[0x%x]=0x%x\n",
			MT6355_PSOCSTATUS, upmu_get_reg_value(MT6355_PSOCSTATUS));
		print("[PMIC][status] BUCK_OC_SDN_STATUS[0x%x]=0x%x\n",
			MT6355_BUCK_OC_CON0, upmu_get_reg_value(MT6355_BUCK_OC_CON0));
		print("[PMIC][status] BUCK_OC_SDN_EN[0x%x]=0x%x\n",
			MT6355_BUCK_OC_CON1, upmu_get_reg_value(MT6355_BUCK_OC_CON1));
	/*--thermal shutdown 150--*/
		print("[PMIC][status] THERMALSTATUS[0x%x]=0x%x\n",
			MT6355_THERMALSTATUS, upmu_get_reg_value(MT6355_THERMALSTATUS));
	/*--long press shutdown--*/
		print("[PMIC][status] STRUP_CON4[0x%x]=0x%x\n",
			MT6355_STRUP_CON4, upmu_get_reg_value(MT6355_STRUP_CON4));
	/*--WDTRST--*/
		print("[PMIC][status] TOP_RST_MISC[0x%x]=0x%x\n",
			MT6355_TOP_RST_MISC, upmu_get_reg_value(MT6355_TOP_RST_MISC));
	/*--CLK TRIM--*/
		print("[PMIC][status] TOP_CLK_TRIM[0x%x]=0x%x\n",
			MT6355_TOP_CLK_TRIM, upmu_get_reg_value(MT6355_TOP_CLK_TRIM));
	/*--Write 1 to clear PONSTS and POFFSTS--*/
		ret_val=pmic_config_interface(MT6355_PSTSCTL, 0x1, 0x1, 0);/*--clear PONSTS--*/
		ret_val=pmic_config_interface(MT6355_PSTSCTL, 0x1, 0x1, 8);/*--clear POFFSTS--*/
		ret_val=pmic_config_interface(MT6355_BUCK_OC_CON0, 0xFF, 0xFF, 0);/*--clear OC_SDN_STATUS--*/
	} else {
	/*--After writing 1, need to write 0 back--*/
		ret_val=pmic_config_interface(MT6355_PSTSCTL, 0x0, 0x1, 0);
		ret_val=pmic_config_interface(MT6355_PSTSCTL, 0x0, 0x1, 8);
	}
}

void pmic_wdt_set(void)
{
	int ret_val=0;
	/*--Reset digital only--*/
	/*--Enable WDT--*/
	print("[PMIC] pmic_wdt_set\n");
	ret_val=pmic_config_interface(MT6355_TOP_RST_MISC_CLR, 0x0002, 0xFFFF, 0); /*--[1]=0, RG_WDTRSTB_MODE--*/
	ret_val=pmic_config_interface(MT6355_TOP_RST_MISC_SET, 0x0001, 0xFFFF, 0); /*--[0]=1, RG_WDTRSTB_EN--*/
	print("[PMIC][pmic_init] Reg[0x%x]=0x%x\n",
			MT6355_TOP_RST_MISC, upmu_get_reg_value(MT6355_TOP_RST_MISC));
}

void pmic_default_voltage(void)
{
	int ret;
	struct regulator *reg_vproc11;
	struct regulator *reg_vsram_proc;
	struct regulator *reg_vmodem;
	struct regulator *reg_vsram_md;

	/*--Get regulator handle--*/
	reg_vproc11 = regulator_get(NULL, "vproc11");
	if (!reg_vproc11)
		print("regulator_get reg_vproc11 failed\n");

	reg_vsram_proc = regulator_get(NULL, "vsram_proc");
	if (!reg_vsram_proc)
		print("regulator_get reg_vsram_proc failed\n");

	reg_vmodem = regulator_get(NULL, "vmodem");
	if (!reg_vmodem)
		print("regulator_get reg_vmodem failed\n");

	reg_vsram_md = regulator_get(NULL, "vsram_md");
	if (!reg_vsram_md)
		print("regulator_get reg_vsram_md failed\n");

	/*--Set voltage--*/
	if (reg_vproc11) {
		ret  = regulator_set_voltage(reg_vproc11, 925000, 1193750);
		if (ret)
			print("regulator_set_voltage reg_vproc11 failed\n");
	}

	if (reg_vsram_proc) {
		ret  = regulator_set_voltage(reg_vsram_proc, 1025000, 1306250);
		if (ret)
			print("regulator_set_voltage reg_vsram_proc failed\n");
	}

	if (reg_vmodem) {
		ret  = regulator_set_voltage(reg_vmodem, 800000, 1187500);
		if (ret)
			print("regulator_set_voltage reg_vmodem failed\n");
	}

	if (reg_vsram_md) {
		ret  = regulator_set_voltage(reg_vsram_md, 931250, 1306250);
		if (ret)
			print("regulator_set_voltage reg_vsram_md failed\n");
	}

	/*--Get voltage & enable--*/
	if (reg_vproc11) {
		print("regulator_get_voltage reg_vproc11 = %d uV\n", regulator_get_voltage(reg_vproc11));
		ret = regulator_enable(reg_vproc11);
		if (ret)
			print("regulator_enable reg_vproc11 failed\n");
	}

	if (reg_vsram_proc) {
		print("regulator_get_voltage reg_vsram_proc = %d uV\n", regulator_get_voltage(reg_vsram_proc));
		ret = regulator_enable(reg_vsram_proc);
		if (ret)
			print("regulator_enable reg_vsram_proc failed\n");
	}

	if (reg_vmodem) {
		print("regulator_get_voltage reg_vmodem = %d uV\n", regulator_get_voltage(reg_vmodem));
		ret = regulator_enable(reg_vmodem);
		if (ret)
			print("regulator_enable reg_vmodem failed\n");
	}

	if (reg_vsram_md) {
		print("regulator_get_voltage reg_vsram_md = %d uV\n", regulator_get_voltage(reg_vsram_md));
		ret = regulator_enable(reg_vsram_md);
		if (ret)
			print("regulator_enable reg_vsram_md failed\n");
	}
}

/******************************************************************************************************
* AUXADC
******************************************************************************************************/
struct mtk_auxadc_t mtk_auxadc[] = {
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH0_BY_AP_ADDR, PMIC_AUXADC_ADC_OUT_CH0_BY_AP_ADDR, PMIC_AUXADC_RQST_CH0_ADDR, 0, 15, 3),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH2_ADDR, PMIC_AUXADC_ADC_OUT_CH2_ADDR, PMIC_AUXADC_RQST_CH2_ADDR, 2, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH3_ADDR, PMIC_AUXADC_ADC_OUT_CH3_ADDR, PMIC_AUXADC_RQST_CH3_ADDR, 3, 12, 2),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_BATID_ADDR, PMIC_AUXADC_ADC_OUT_BATID_ADDR, PMIC_AUXADC_RQST_BATID_ADDR, 3, 12, 2),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH11_ADDR, PMIC_AUXADC_ADC_OUT_CH11_ADDR, PMIC_AUXADC_RQST_CH11_ADDR, 11, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH4_ADDR, PMIC_AUXADC_ADC_OUT_CH4_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH4_ADDR, PMIC_AUXADC_ADC_OUT_CH4_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH5_ADDR, PMIC_AUXADC_ADC_OUT_CH5_ADDR, PMIC_AUXADC_RQST_CH5_ADDR, 5, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH7_ADDR, PMIC_AUXADC_ADC_OUT_CH7_ADDR, PMIC_AUXADC_RQST_CH7_ADDR, 7, 15, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH9_ADDR, PMIC_AUXADC_ADC_OUT_CH9_ADDR, PMIC_AUXADC_RQST_CH9_ADDR, 9, 12, 1),
};

#if !CFG_EVB_PLATFORM || defined(MTK_EFUSE_WRITER_SUPPORT)
kal_int32 count_time_out=10;
#define VOLTAGE_FULL_RANGE     	1800
#define ADC_PRECISE		32768 	// 15 bits

void pmic_auxadc_debug(int index)
{
	print("[PMIC][Dummy AP] pmic_auxadc_debug_none\n");
}

int pmic_get_auxadc_value(AUXADC_LIST list)
{
	int count = 0;
	signed int adc_result = 0, reg_val = 0;
	struct mtk_auxadc_t *auxadc_channel;

	if (list < AUXADC_LIST_BATADC && list > AUXADC_LIST_TSX) {
		printf("[%s] Invalid channel(%d)\n", __func__, list);
		return -1;
	}
	auxadc_channel = &mtk_auxadc[list];

	if (list == AUXADC_LIST_DCXO)
		pmic_config_interface(PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_ADDR, 1,
					PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_MASK,
					PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_SHIFT);
	if (list == AUXADC_LIST_CHIP_TEMP)
		pmic_config_interface(PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_ADDR, 0,
					PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_MASK,
					PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_SHIFT);

	pmic_set_register_value(auxadc_channel->rqst, 1);
	udelay(10);

	while (pmic_get_register_value(auxadc_channel->rdy) != 1) {
		udelay(1300);
		if ((count++) > count_time_out) {
			printf("[%s] (%d) Time out!\n", __func__, list);
			break;
		}
	}
	reg_val = pmic_get_register_value(auxadc_channel->out);

	if (auxadc_channel->resolution == 12)
		adc_result = (reg_val * auxadc_channel->r_val *
				VOLTAGE_FULL_RANGE) / 4096;
	else if (auxadc_channel->resolution == 15)
		adc_result = (reg_val * auxadc_channel->r_val *
				VOLTAGE_FULL_RANGE) / 32768;

	printf("[%s] reg_val = 0x%x, adc_result = %d\n",
				__func__, reg_val, adc_result);
	return adc_result;
}
#endif


//==============================================================================
// AUXADC Init Code
//==============================================================================
int pmic_auxadc_init_setting_v1(void)
{
	print("[PMIC] pmic_auxadc_init_setting_v1\n");
	/* set 15 bits sample times = 128*/
	pmic_config_interface(PMIC_AUXADC_AVG_NUM_LARGE_ADDR, 6, 0x7, 3); /* 1.28 ms */
	/* set 12 bits sample times  = 8 */
	pmic_config_interface(PMIC_AUXADC_AVG_NUM_SMALL_ADDR, 2, 0x7, 0); /* 0.08 ms */

	/* set channel 0, 7 as 15 bits, others = 12 bits  000001000001*/
	pmic_config_interface(PMIC_AUXADC_AVG_NUM_SEL_ADDR, 0x41, 0xFFF, 0);

	pmic_config_interface(PMIC_AUXADC_CK_AON_ADDR, 0, 0x1, 15);
	pmic_config_interface(PMIC_AUXADC_CK_AON_MD_ADDR, 0, 0x1, 14);
	pmic_config_interface(PMIC_AUXADC_CK_AON_GPS_ADDR, 0, 0x1, 13);
	pmic_config_interface(PMIC_AUXADC_DATA_REUSE_SEL_ADDR, 0, 0x3, 0);
	pmic_config_interface(PMIC_AUXADC_DATA_REUSE_EN_ADDR, 1, 0x1, 8);

	/* setting of ADC not being reset in Sleep mode */
	pmic_config_interface(PMIC_RG_STRUP_AUXADC_RSTB_SW_ADDR, 1, 0x1, 1);
	pmic_config_interface(PMIC_RG_STRUP_AUXADC_RSTB_SEL_ADDR, 1, 0x1, 3);

	return 0;
}
//==============================================================================
// MD Init Code
//==============================================================================
int pmic_md_init_setting_v1(void)
{
	print("[PMIC][Dummy AP] pmic_md_init_setting_v1\n");
	return 0;

}
//==============================================================================
// PMIC Init Code
//==============================================================================
U32 pmic_init (void)
{
    U32 ret_code = PMIC_TEST_PASS;
    U16 pwhold = 0;
    int ret_val=0, val;

    print("[PMIC] Preloader Start..................\n");

    print("[PMIC] MT6355 CHIP Code = 0x%x\n",
			get_MT6355_PMIC_chip_version());

    /* keep power hold value for gauge*/
    pmic_pwrhold_info_to_gauge();
    /* Boot debug status TBD*/
    pmic_dbg_status(1);
    /* for auxadc init */
    pmic_auxadc_init_setting_v1();
    /*pmic_auxadc_debug(1);*/
    pmic_md_init_setting_v1();
    /* detect V battery Drop   used by RTC to check power on reason  */
    /* ask waverly programming guide */
    pmic_DetectVbatDrop();

    /* ask shin-shyu programming guide TBD */
    if(hw_check_battery()==1)
    {
	pmic_disable_usbdl_wo_battery();
    }

    /* Enable PMIC WDTRST function (depends on main chip RST function) TBD */
    pmic_wdt_set();
    /* ask peter programming guide RG_SMPS_TESTMODE_B by luke TBD */
    pmic_smps_testmode_b();

#if defined(EXTERNAL_BUCK_xxxxxx)
	xxxxxx_driver_probe();
#endif

	pl_battery_init(false);

    if(hw_check_battery()==1)
    {
#if !CFG_EVB_PLATFORM
	 pl_check_bat_protect_status();
#endif
    }

    pmic_dbg_status(0);

    /*regulator_init();*/
    mt6355_probe();

    pmic_default_voltage();

    wk_auxadc_vsen_tdet_ctrl(1);

    print("[PMIC] [pmic_init] Done...................\n");

    return ret_code;
}

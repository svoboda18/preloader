#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <regulator/mtk_regulator.h>
#include <mt6357.h>
#include <pmic.h>
#include <sec_devinfo.h>
#include <rtc.h>

/*#define	EXTERNAL_BUCK_RT5738	1*/
#if defined(EXTERNAL_BUCK_RT5738)
#include <rt5738.h>
#endif

/*
 * CONFIG OPTION SET
 */
/* Enable this option when pmic need efuse sw load */
/* if enable, please also check pmic_efuse.c or pmic_efuse_xxxx.c */
#define EFUSE_SW_LOAD 0
/* Enable this option when platform need smart reset*/

/*
 * PMIC Access APIs
 */
U32 pmic_read_interface (U32 RegNum, U32 *val, U32 MASK, U32 SHIFT)
{
	U32 return_value = 0;
	U32 pmic_reg = 0;
	U32 rdata = 0;

	return_value = pwrap_read(RegNum, &rdata);
	pmic_reg = rdata;
	if (return_value != 0) {
		print("[PMIC]Reg[0x%x]= pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	/*print("[pmic_read_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);*/

	pmic_reg &= (MASK << SHIFT);
	*val = (pmic_reg >> SHIFT);
	/*print("[pmic_read_interface] val=0x%x\n", *val);*/

	return return_value;
}

U32 pmic_config_interface (U32 RegNum, U32 val, U32 MASK, U32 SHIFT)
{
	U32 return_value = 0;
	U32 pmic_reg = 0;
	U32 rdata = 0;

	return_value = pwrap_read(RegNum, &rdata);
	pmic_reg = rdata;
	if(return_value!=0)
	{
		print("[PMIC]Reg[%x]= pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	/*print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);*/

	pmic_reg &= ~(MASK << SHIFT);
	pmic_reg |= (val << SHIFT);

	/* 2. mt_write_byte(RegNum, pmic_reg); */
	return_value = pwrap_write(RegNum, pmic_reg);
	if (return_value != 0) {
		print("[PMIC]Reg[%x]= pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	/*print("[pmic_config_interface] write Reg[%x]=0x%x\n", RegNum, pmic_reg);*/

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

U32 upmu_get_reg_value(U32 reg)
{
	U32 ret = 0;
	U32 reg_val = 0;

	ret = pmic_read_interface(reg, &reg_val, 0xFFFF, 0x0);

	return reg_val;
}

U32 upmu_set_reg_value(U32 reg, U32 reg_val)
{
	U32 ret = 0;

	ret = pmic_config_interface(reg, reg_val, 0xFFFF, 0x0);

	return ret;
}

/*
 * PMIC Usage APIs
 */
/* For ZION/CERVINO distinguish dram type in MT6357, not common API */
U32 get_dram_type(void)
{
	U32 val = 0;
	pmic_read_interface(PMIC_VM_MODE_ADDR, &val,
		PMIC_VM_MODE_MASK, PMIC_VM_MODE_SHIFT);

	return val;
}

U32 get_PMIC_chip_version (void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_SWCID_ADDR), (&val),
		(U32)(PMIC_SWCID_MASK),
		(U32)(PMIC_SWCID_SHIFT));

	return val;
}

int pmic_detect_powerkey(void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_PWRKEY_DEB_ADDR), (&val),
		(U32)(PMIC_PWRKEY_DEB_MASK),
		(U32)(PMIC_PWRKEY_DEB_SHIFT));

	return (1 - val);
}

int pmic_detect_homekey(void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_HOMEKEY_DEB_ADDR), (&val),
		(U32)(PMIC_HOMEKEY_DEB_MASK),
		(U32)(PMIC_HOMEKEY_DEB_SHIFT));

	return (1 - val);
}

U32 pmic_IsUsbCableIn (void)
{
#if CFG_EVB_PLATFORM
	return PMIC_CHRDET_EXIST;	/* for bring up */
	/*print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);*/
#else
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_RGS_CHRDET_ADDR), (&val),
		(U32)(PMIC_RGS_CHRDET_MASK),
		(U32)(PMIC_RGS_CHRDET_SHIFT));
	print("[PMIC]IsUsbCableIn %d\n", val);
	if (val)
		return PMIC_CHRDET_EXIST;
	else
		return PMIC_CHRDET_NOT_EXIST;
#endif
}

U32 pmic_upmu_set_baton_tdet_en(U32 val)
{
	U32 ret = 0;

	ret = pmic_config_interface((U32)(PMIC_BATON_TDET_EN_ADDR), (U32)(val),
		(U32)(PMIC_BATON_TDET_EN_MASK),
		(U32)(PMIC_BATON_TDET_EN_SHIFT));

	return ret;
}

U32 pmic_upmu_set_rg_baton_en(U32 val)
{
	U32 ret = 0;

	ret = pmic_config_interface((U32)(PMIC_RG_BATON_EN_ADDR), (U32)(val),
		(U32)(PMIC_RG_BATON_EN_MASK),
		(U32)(PMIC_RG_BATON_EN_SHIFT));

	return ret;
}

U32 pmic_upmu_get_rgs_baton_undet(void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_RGS_BATON_UNDET_ADDR), (&val),
		(U32)(PMIC_RGS_BATON_UNDET_MASK),
		(U32)(PMIC_RGS_BATON_UNDET_SHIFT));

	return val;
}

U32 upmu_is_chr_det(void)
{
	U32 ret = 0;
	U32 val = 0;
#if CFG_EVB_PLATFORM
	val = 1; /* for bring up */
	/*print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);*/
#else
	ret = pmic_read_interface((U32)(PMIC_RGS_CHRDET_ADDR), (&val),
		(U32)(PMIC_RGS_CHRDET_MASK),
		(U32)(PMIC_RGS_CHRDET_SHIFT));
#endif
	return val;
}

void PMIC_enable_long_press_reboot(void)
{
#if !CFG_FPGA_PLATFORM
#if !CFG_EVB_PLATFORM
#if KPD_PMIC_LPRST_TD!=0
	#if ONEKEY_REBOOT_NORMAL_MODE_PL
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x01, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_HOMEKEY_RST_EN_ADDR, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_PWRKEY_RST_TD_ADDR, (U32)KPD_PMIC_LPRST_TD,
		PMIC_RG_PWRKEY_RST_TD_MASK, PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#else
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x01, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_HOMEKEY_RST_EN_ADDR, 0x01, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_PWRKEY_RST_TD_ADDR, (U32)KPD_PMIC_LPRST_TD, PMIC_RG_PWRKEY_RST_TD_MASK, PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#endif
#else
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_HOMEKEY_RST_EN_ADDR, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
#endif
#else
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_HOMEKEY_RST_EN_ADDR, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
}

U32 PMIC_VUSB_EN(void)
{
	int ret = 0;

	ret = pmic_config_interface((U32)(PMIC_RG_LDO_VUSB33_EN_0_ADDR), 1,
		(U32)(PMIC_RG_LDO_VUSB33_EN_0_MASK),
		(U32)(PMIC_RG_LDO_VUSB33_EN_0_SHIFT));

	return ret;
}

#if 0 /* remove unused function to save space of log string */
void mt6357_dump_register(void)
{
	unsigned short i = 0;

	print("dump PMIC 6357 register\n");

	for (i = 0; i <= 0x2340; i = i + 10) {
		print("Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x\n",
			i, upmu_get_reg_value(i),
			i + 2, upmu_get_reg_value(i + 2),
			i + 4, upmu_get_reg_value(i + 4),
			i + 6, upmu_get_reg_value(i + 6),
			i + 8, upmu_get_reg_value(i + 8));
	}
}
#endif /* remove unused function to save space of log string */

/*
 * PMIC Export API
 */
static U32 g_sts_rtca;
static U32 g_sts_spar;
static U32 g_sts_crst;
static U32 g_just_rst;
static U32 g_pmic_boot_status;

/* check if RTC Alarm happened at power on */
U32 is_pmic_rtc_alarm(void)
{
	return g_sts_rtca;
}
/* check if RTC SPAR happened at power on */
U32 is_pmic_spar(void)
{
	return g_sts_spar;
}
/* check if PMIC cold reset at previous power off */
U32 is_pmic_cold_reset(void)
{
	return g_sts_crst;
}

U32 is_pmic_long_press_reset(void)
{
	return g_just_rst;
}

U32 is_pmic_mrv(void)
{
	U32 g_is_mrv = 0;

	pmic_read_interface(PMIC_RG_TOP2_RSV0_ADDR,
			    &g_is_mrv, 0x1, 15);

	return g_is_mrv;
}

U32 get_pmic_boot_status(void)
{
	return g_pmic_boot_status;
}

int PMIC_POWER_HOLD(unsigned int hold)
{
	if (hold > 1) {
		print("[PMIC] POWER_HOLD error\n");
		return -1;
	}

	/* MT6357 must keep power hold */
	pmic_config_interface(PMIC_RG_PWRHOLD_ADDR, hold, PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);

	/* hold:  1(ON)  0(OFF) */
	print("[PMIC]POWER_HOLD :0x%x\n", upmu_get_reg_value(MT6357_PPCCTL0));
	return 0;
}

static void record_pmic_boot_status(unsigned int pon_sts, unsigned int poff_sts)
{
	if (poff_sts & 0x6)
		g_pmic_boot_status = 1; /* OC/PG */
	else if (poff_sts & 0x40 || g_just_rst)
		g_pmic_boot_status = 2; /* Long press */
	else if (poff_sts & 0x1 || poff_sts & 0x200)
		g_pmic_boot_status = 3; /* UVLO/DDLO */
}

static void pmic_check_rst(unsigned int poff_sts)
{
	unsigned int val;

	/*
	 * TOP_RST_STATUS is used to indicate which reset happened
	 * If a reset happened, the corresponding bit will be clear
	 */
	val = upmu_get_reg_value(MT6357_TOP_RST_STATUS);
	if (val != 0x7F) {
		if (val == 0)
			print("[%s] PORSTB\n", __func__);
		else if (((val >> 1) & 1) == 0)
			print("[%s] DDLO_RSTB\n", __func__);
		else if (((val >> 2) & 1) == 0)
			print("[%s] UVLO_RSTB\n", __func__);
#if 0 /* TBD */
		if (((val >> 3) & 1) == 0)
			print("[%s] RTC_DDLO_RSTB\n", __func__);
		if (((val >> 4) & 1) == 0)
			print("[%s] CHRWDT_REG_RSTB\n", __func__);
		if (((val >> 5) & 1) == 0)
			print("[%s] CHRDET_REG_RSTB\n", __func__);
#endif
		/* bit[2:0] will be keep to LK */
		val = upmu_set_reg_value(MT6357_TOP_RST_STATUS, 0x78 | val);
	}
	if ((poff_sts >> 8) & 1)
		print("[%s] BWDT\n", __func__);
	if ((poff_sts >> 6) & 1)
		print("[%s] Long press shutdown\n", __func__);
	if ((poff_sts >> 5) & 1)
		print("[%s] Cold Reset\n", __func__);
	if ((poff_sts >> 10) & 1)
		print("[%s] AP Watchdog\n", __func__);
	if ((poff_sts >> 4) & 1)
		print("[%s] WARM Reset\n", __func__);

	/* Long press shutdown status */
	pmic_read_interface(PMIC_JUST_PWRKEY_RST_ADDR, &g_just_rst,
		PMIC_JUST_PWRKEY_RST_MASK, PMIC_JUST_PWRKEY_RST_SHIFT);
	print("[PMIC]just_rst = %d\n", g_just_rst);
	/* LK mtk_key.c will clear long press shutdown status */
}

void pmic_dbg_status(unsigned char option)
{
	/*--option = 1--*/
	/* UVLO off */
	/* Write 1 to clear PONSTS and POFFSTS */
	/* power not good */
	/* buck oc */
	/* thermal shutdown 150 */
	/* long press shutdown */
	/* WDTRST */
	/* CLK TRIM */
	/*--option = 0--*/
	/* After writing 1, need to write 0 back */

	int ret_val = 0;
	unsigned int pon_sts = 0;
	unsigned int poff_sts = 0;

	if (option) {
	/* pwhold must set to 1 to update debug status */
		PMIC_POWER_HOLD(1);
	/*--UVLO off--*/
		print("[PMIC]TOP_RST_STATUS[0x%x]=0x%x\n",
			MT6357_TOP_RST_STATUS, upmu_get_reg_value(MT6357_TOP_RST_STATUS));
	/*special for RTC Alarm and SPAR*/
		pon_sts = upmu_get_reg_value(MT6357_PONSTS);
		g_sts_rtca = (pon_sts >> PMIC_STS_RTCA_SHIFT) & PMIC_STS_RTCA_MASK;
		g_sts_spar = (pon_sts >> PMIC_STS_SPAR_SHIFT) & PMIC_STS_SPAR_MASK;
		print("[PMIC]PONSTS[0x%x]=0x%x\n",
			MT6357_PONSTS, pon_sts);
	/*special for cold rest*/
		poff_sts = upmu_get_reg_value(MT6357_POFFSTS);
		g_sts_crst = (poff_sts >> PMIC_STS_CRST_SHIFT) & PMIC_STS_CRST_MASK;
		print("[PMIC]POFFSTS[0x%x]=0x%x\n",
			MT6357_POFFSTS, poff_sts);
	/*--power not good--*/
		print("[PMIC]PGSTATUS0[0x%x]=0x%x\n",
			MT6357_PG_SDN_STS0, upmu_get_reg_value(MT6357_PG_SDN_STS0));
	/*--buck oc--*/
		print("[PMIC]PSOCSTATUS[0x%x]=0x%x\n",
			MT6357_OC_SDN_STS0, upmu_get_reg_value(MT6357_OC_SDN_STS0));
		print("[PMIC]BUCK_OC_SDN_STATUS[0x%x]=0x%x\n",
			MT6357_BUCK_TOP_OC_CON0, upmu_get_reg_value(MT6357_BUCK_TOP_OC_CON0));
		print("[PMIC]BUCK_OC_SDN_EN[0x%x]=0x%x\n",
			MT6357_BUCK_TOP_ELR0, upmu_get_reg_value(MT6357_BUCK_TOP_ELR0));
	/*--thermal shutdown 150--*/
		print("[PMIC]THERMALSTATUS[0x%x]=0x%x\n",
			MT6357_THERMALSTATUS, upmu_get_reg_value(MT6357_THERMALSTATUS));
	/*--long press shutdown--*/
		print("[PMIC]STRUP_CON4[0x%x]=0x%x\n",
			MT6357_STRUP_CON4, upmu_get_reg_value(MT6357_STRUP_CON4));
	/*--WDTRST--*/
		print("[PMIC]TOP_RST_MISC[0x%x]=0x%x\n",
			MT6357_TOP_RST_MISC, upmu_get_reg_value(MT6357_TOP_RST_MISC));
	/*--CLK TRIM--*/
		print("[PMIC]TOP_CLK_TRIM[0x%x]=0x%x\n",
			MT6357_TOP_CLK_TRIM, upmu_get_reg_value(MT6357_TOP_CLK_TRIM));
	/*--Write 1 to clear PONSTS and POFFSTS(include PG status and BUCK OC status)--*/
		ret_val = pmic_config_interface(PMIC_RG_POFFSTS_CLR_ADDR, 0x1,
			PMIC_RG_POFFSTS_CLR_MASK, PMIC_RG_POFFSTS_CLR_SHIFT);/*--clear PONSTS--*/
		ret_val = pmic_config_interface(PMIC_RG_PONSTS_CLR_ADDR, 0x1,
			PMIC_RG_PONSTS_CLR_MASK, PMIC_RG_PONSTS_CLR_SHIFT);/*--clear POFFSTS--*/
		ret_val = pmic_config_interface(MT6357_BUCK_TOP_OC_CON0, 0xFF, 0xFF, 0);/*--clear OC_SDN_STATUS--*/
	/*--Write 1 to clear thermal shutdown--*/
		ret_val = pmic_config_interface(PMIC_RG_STRUP_THR_CLR_ADDR, 0x1,
			PMIC_RG_STRUP_THR_CLR_MASK, PMIC_RG_STRUP_THR_CLR_SHIFT);
	/*--Long press shutdown will be clear by pmic_check_rst()--*/

	/*--WDTRSTB_STATUS will be clear in kernel--*/
		mt6357_wdtdbg_vosel();

	/*--Check PMIC reset reason--*/
		pmic_check_rst(poff_sts);

		record_pmic_boot_status(pon_sts, poff_sts);
	} else {
	/*--After writing 1, need to write 0 back--*/
		ret_val = pmic_config_interface(PMIC_RG_POFFSTS_CLR_ADDR, 0x0,
			PMIC_RG_POFFSTS_CLR_MASK, PMIC_RG_POFFSTS_CLR_SHIFT);
		ret_val = pmic_config_interface(PMIC_RG_PONSTS_CLR_ADDR, 0x0,
			PMIC_RG_PONSTS_CLR_MASK, PMIC_RG_PONSTS_CLR_SHIFT);
		ret_val = pmic_config_interface(PMIC_RG_STRUP_THR_CLR_ADDR, 0x0,
			PMIC_RG_STRUP_THR_CLR_MASK, PMIC_RG_STRUP_THR_CLR_SHIFT);
	}
}

void pmic_disable_usbdl_wo_battery(void)
{
	print("[PMIC]disable usbdl wo battery\n");

	/* ask shin-shyu programming guide TBD */
	pmic_config_interface(PMIC_RG_ULC_DET_EN_ADDR,1,PMIC_RG_ULC_DET_EN_MASK,PMIC_RG_ULC_DET_EN_SHIFT);
	pmic_config_interface(PMIC_RG_USBDL_SET_ADDR, 0x0000, PMIC_RG_USBDL_SET_MASK, PMIC_RG_USBDL_SET_SHIFT);
	pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 0x0001, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);
}

void pmic_wdt_set(void)
{
	unsigned int ret_val = 0;
	/*--Reset digital only--*/
	/*--Enable WDT--*/
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_SET_ADDR, 0x0020, 0xFFFF, 0); /*--[5]=1, RG_WDTRSTB_DEB--*/
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_CLR_ADDR, 0x0002, 0xFFFF, 0); /*--[1]=0, RG_WDTRSTB_MODE--*/
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_SET_ADDR, 0x0001, 0xFFFF, 0); /*--[0]=1, RG_WDTRSTB_EN--*/
	print("[PMIC]%s Reg[0x%x]=0x%x\n", __func__,
			MT6357_TOP_RST_MISC, upmu_get_reg_value(MT6357_TOP_RST_MISC));
}

/******************************************************************************************************
* AUXADC
******************************************************************************************************/
struct mtk_auxadc_t mtk_auxadc[] = {
	/* BATADC */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH0_BY_AP_ADDR,
		PMIC_AUXADC_ADC_OUT_CH0_BY_AP_ADDR, PMIC_AUXADC_RQST_CH0_ADDR, 0, 15, 3),
	/* VCDT */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH2_ADDR,
		PMIC_AUXADC_ADC_OUT_CH2_ADDR, PMIC_AUXADC_RQST_CH2_ADDR, 2, 12, 1),
	/* BAT TEMP */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH3_ADDR,
		PMIC_AUXADC_ADC_OUT_CH3_ADDR, PMIC_AUXADC_RQST_CH3_ADDR, 3, 12, 1),
	/* BATID */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_BATID_ADDR,
		PMIC_AUXADC_ADC_OUT_BATID_ADDR, PMIC_AUXADC_RQST_BATID_ADDR, 3, 12, 1),
	/* VBIF */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH11_ADDR,
		PMIC_AUXADC_ADC_OUT_CH11_ADDR, PMIC_AUXADC_RQST_CH11_ADDR, 11, 12, 1),
	/* CHIP TEMP */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH4_ADDR,
		PMIC_AUXADC_ADC_OUT_CH4_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
	/* DCXO */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_DCXO_BY_AP_ADDR,
		PMIC_AUXADC_ADC_OUT_DCXO_BY_AP_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 15, 1),
	/* ACCDET */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH5_ADDR,
		PMIC_AUXADC_ADC_OUT_CH5_ADDR, PMIC_AUXADC_RQST_CH5_ADDR, 5, 12, 1),
	/* TSX */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH7_ADDR,
		PMIC_AUXADC_ADC_OUT_CH7_ADDR, PMIC_AUXADC_RQST_CH7_ADDR, 7, 15, 1),
	/* HP OFFSET CAL */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH9_ADDR,
		PMIC_AUXADC_ADC_OUT_CH9_ADDR, PMIC_AUXADC_RQST_CH9_ADDR, 9, 15, 1),
	/* ISENSE */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH1_BY_AP_ADDR,
		PMIC_AUXADC_ADC_OUT_CH1_BY_AP_ADDR, PMIC_AUXADC_RQST_CH1_ADDR, 1, 15, 3),
	/* TS_BUCK1 */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH4_BY_THR1_ADDR,
		PMIC_AUXADC_ADC_OUT_CH4_BY_THR1_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
	/* TS_BUCK2 */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH4_BY_THR2_ADDR,
		PMIC_AUXADC_ADC_OUT_CH4_BY_THR2_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
};

#if !CFG_EVB_PLATFORM || defined(MTK_EFUSE_WRITER_RESERVE_CODESIZE)
kal_int32 count_time_out = 10;
#define VOLTAGE_FULL_RANGE	1800
#define ADC_PRECISE		32768 /* 15 bits */

int pmic_get_auxadc_value(AUXADC_LIST list)
{
	int count = 0;
	signed int adc_result = 0, reg_val = 0;
	struct mtk_auxadc_t *auxadc_channel;

	if (list < AUXADC_LIST_BATADC || list >= AUXADC_LIST_BOTTOM) {
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
			printf("[%s] (%d), ch %d Time out!\n",
				__func__, list, auxadc_channel->rqst_shift);
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

/*
 * PMIC EFUSE
 */
/* PMIC EFUSE SW load need to check EFUSE_TABLE */
static void pmic_efuse_sw_load(void)
{
#if EFUSE_SW_LOAD
	unsigned int efuse_data = 0;

	efuse_data = pmic_read_efuse_nolock(2); /* check pmic_efuse related code */
	pmic_config_interface(0x0D3A, (efuse_data >> 8) & 1, 0x1, 9);
	pmic_config_interface(0x0D46, (efuse_data >> 9) & 1, 0x1, 9);
	pmic_config_interface(0x0D2A, (efuse_data >> 10) & 0x7, 0x7, 6);
#else
	print("No EFUSE SW Load\n");
#endif
}

/*
 * PMIC Init Code
 */

U32 pmic_init (void)
{
	U32 ret_code = PMIC_TEST_PASS;
	int ret_val = 0;

	print("[PMIC]Preloader Start\n");

	print("[PMIC]MT6357 CHIP Code = 0x%x, mrv=%d\n"
	      , get_PMIC_chip_version()
	      , is_pmic_mrv());

	/* Boot debug status */
	pmic_dbg_status(1);
	/* Auxadc init will be set in PMIC initial setting, removed by Jeter */
	/*pmic_auxadc_init_setting();*/

	/* PMIC SW load EFUSE to target register */
	pmic_efuse_sw_load();

	/* ask shin-shyu programming guide TBD */
	if(hw_check_battery() == 1)
		pmic_disable_usbdl_wo_battery();

	/* Enable PMIC WDTRST function (depends on main chip RST function) TBD */
	pmic_wdt_set();

#if defined(EXTERNAL_BUCK_RT5738)
	rt5738_driver_probe();
#endif

	pmic_dbg_status(0);

	/*regulator_init();*/
	mt6357_probe();

	print("[PMIC]Init done\n");

	return ret_code;
}

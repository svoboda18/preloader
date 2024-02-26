#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <regulator/mtk_regulator.h>
#include <pmic.h>
#include <sec_devinfo.h>
#include <rtc.h>
#include <mt6359.h>
#ifdef MT6360_PMIC
#include <mt6360_pmic.h> /* for mt6360_get_hw_trapping_type */
#endif /* MT6360_PMIC */
#ifdef MT6360_LDO
#include <mt6360_ldo.h>
#endif /* MT6360_LDO */


/* show vcore for MD before MD boot up */
static const struct mtk_regulator empty_regulator;
void wk_vcore_check(void)
{
	struct mtk_regulator reg_vcore = empty_regulator;
	int ret = 0;

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		pal_log_err("mtk_regulator_get reg_vcore failed\n");
	pal_log_warn("vcore = %d uV\n", mtk_regulator_get_voltage(&reg_vcore));
}

#if 0 /* remove unused function */
/* used for modify power down sequence */
static void wk_power_down_seq(void)
{
	pmic_config_interface(PMIC_RG_CPS_W_KEY_ADDR, 0x4729,
			      PMIC_RG_CPS_W_KEY_MASK,
			      PMIC_RG_CPS_W_KEY_SHIFT);
	/* set VPROC12 sequence to VA12 */
	pmic_config_interface(PMIC_RG_VPROC12_DSA_ADDR, 0xA,
		PMIC_RG_VPROC12_DSA_MASK, PMIC_RG_VPROC12_DSA_SHIFT);
	pmic_config_interface(PMIC_RG_CPS_W_KEY_ADDR, 0,
			      PMIC_RG_CPS_W_KEY_MASK,
			      PMIC_RG_CPS_W_KEY_SHIFT);
}
#endif

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
		pal_log_err("[PMIC]Reg[0x%x] pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	/*pal_log_info("[pmic_read_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);*/

	pmic_reg &= (MASK << SHIFT);
	*val = (pmic_reg >> SHIFT);
	/*pal_log_info("[pmic_read_interface] val=0x%x\n", *val);*/

	return return_value;
}

U32 pmic_config_interface (U32 RegNum, U32 val, U32 MASK, U32 SHIFT)
{
	U32 return_value = 0;
	U32 pmic_reg = 0;
	U32 rdata = 0;

	return_value = pwrap_read(RegNum, &rdata);
	pmic_reg = rdata;
	if (return_value != 0) {
		pal_log_err("[PMIC]Reg[0x%x] pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	/*pal_log_info("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);*/

	pmic_reg &= ~(MASK << SHIFT);
	pmic_reg |= (val << SHIFT);

	/* 2. mt_write_byte(RegNum, pmic_reg); */
	return_value = pwrap_write(RegNum, pmic_reg);
	if (return_value != 0) {
		pal_log_err("[PMIC]Reg[0x%x] pmic_wrap write 0x%x fail\n", RegNum, pmic_reg);
		return return_value;
	}
	/*pal_log_info("[pmic_config_interface] write Reg[%x]=0x%x\n", RegNum, pmic_reg);*/

	return return_value;
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

static U32 pmic_get_register_value(struct pmuflag_t pmureg)
{
	U32 val = 0;
	U32 ret;

	ret = pmic_read_interface(pmureg.addr, &val, pmureg.mask, pmureg.shift);

	return val;
}

static U32 pmic_set_register_value(struct pmuflag_t pmureg, U32 val)
{
	U32 ret;

	ret = pmic_config_interface(pmureg.addr, val, pmureg.mask, pmureg.shift);

	return ret;
}
/*
 * PMIC Usage APIs
 */
U32 get_dram_type(void)
{
	U32 val = 0;

#ifdef MT6360_PMIC
	val = mt6360_get_hw_trapping_type();
#else
	pmic_read_interface(PMIC_VM_MODE_ADDR, &val,
		PMIC_VM_MODE_MASK, PMIC_VM_MODE_SHIFT);
#endif
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

U32 pmic_upmu_set_baton_tdet_en(U32 val)
{
	U32 ret = 0;

	ret = pmic_config_interface((U32)(PMIC_RG_BATON_TDET_EN_ADDR), (U32)(val),
		(U32)(PMIC_RG_BATON_TDET_EN_MASK),
		(U32)(PMIC_RG_BATON_TDET_EN_SHIFT));

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

	ret = pmic_read_interface((U32)(PMIC_AD_BATON_UNDET_ADDR), (&val),
		(U32)(PMIC_AD_BATON_UNDET_MASK),
		(U32)(PMIC_AD_BATON_UNDET_SHIFT));

	return val;
}

U32 upmu_is_chr_det(void)
{
	U32 ret = 0;
	U32 val = 0;

	if (drdi_get_hw_ver() == HW_VER_V0) /* EVB */
		return 1;
	ret = pmic_read_interface((U32)(PMIC_RGS_CHRDET_ADDR), (&val),
		(U32)(PMIC_RGS_CHRDET_MASK),
		(U32)(PMIC_RGS_CHRDET_SHIFT));
	pal_log_info("[PMIC]IsUsbCableIn %d\n", val);
	return val;
}

void PMIC_enable_long_press_reboot(void)
{
#if CFG_LPRST_SUPPORT
	#if ONEKEY_REBOOT_NORMAL_MODE_PL
	/* PWRKEY */
	pmic_config_interface(PMIC_RG_PWRKEY_KEY_MODE_ADDR, 0x00,
			      PMIC_RG_PWRKEY_KEY_MODE_MASK,
			      PMIC_RG_PWRKEY_KEY_MODE_SHIFT);
	#else
	/* PWRKEY + HOMEKEY */
	pmic_config_interface(PMIC_RG_PWRKEY_KEY_MODE_ADDR, 0x01,
			      PMIC_RG_PWRKEY_KEY_MODE_MASK,
			      PMIC_RG_PWRKEY_KEY_MODE_SHIFT);
	#endif /* ONEKEY_REBOOT_NORMAL_MODE_PL */
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x01,
			      PMIC_RG_PWRKEY_RST_EN_MASK,
			      PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_PWRKEY_RST_TD_ADDR,
			      (U32)KPD_PMIC_LPRST_TD,
			      PMIC_RG_PWRKEY_RST_TD_MASK,
			      PMIC_RG_PWRKEY_RST_TD_SHIFT);
#else
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x00,
			      PMIC_RG_PWRKEY_RST_EN_MASK,
			      PMIC_RG_PWRKEY_RST_EN_SHIFT);
#endif /* CFG_LPRST_SUPPORT */
}

U32 PMIC_VUSB_EN(void)
{
	int ret = 0;

	ret = pmic_config_interface((U32)(PMIC_RG_LDO_VUSB_EN_0_ADDR), 1,
		(U32)(PMIC_RG_LDO_VUSB_EN_0_MASK),
		(U32)(PMIC_RG_LDO_VUSB_EN_0_SHIFT));

	return ret;
}

/*
 * PMIC Export API
 */
static U32 g_sts_rtca;
static U32 g_sts_spar;
static U32 g_sts_crst;
static U32 g_just_rst;
static U32 g_pwrkey_release;
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

U32 get_pmic_boot_status(void)
{
	return g_pmic_boot_status;
}

/*
 * used for measure long press counter time which unit is 32ms
 * unit of T is ms
 */
#define LONG_PRESS_COUNT_TIME(T) (T / 32)
/*
 * if detect PWRKEY pressed, check is PWRKEY short pressed
 * return 1 means PWRKEY is short pressed, it is not a valid boot up
 */
U32 is_pwrkey_short_press(void)
{
	U32 val = 0;

	/* if detect PWRKEY pressed, check is PWRKEY short pressed */
	pmic_read_interface(PMIC_PWRKEY_LONG_PRESS_COUNT_ADDR, &val,
		PMIC_PWRKEY_LONG_PRESS_COUNT_MASK, PMIC_PWRKEY_LONG_PRESS_COUNT_SHIFT);
	pal_log_info("[%s] pwrkey_release=%d, LONG_PRESS_COUNT=%d\n",
		__func__, g_pwrkey_release, val << 5);
	if (g_pwrkey_release) {
		/* PWRKEY released during power up and press time not enough */
		if (val < LONG_PRESS_COUNT_TIME(800))
			return 1;
	}
	return 0;
}

int PMIC_POWER_HOLD(unsigned int hold)
{
	unsigned int val = 0;

	if (hold > 1) {
		pal_log_err("[PMIC] POWER_HOLD error\n");
		return -1;
	}

	pmic_config_interface(PMIC_RG_PWRHOLD_ADDR, hold,
		PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);

	/* hold:  1(ON)  0(OFF) */
	pmic_read_interface(PMIC_RG_PWRHOLD_ADDR, &val,
		PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);
	if (hold == 1)
		pal_log_info("[PMIC]POWER_HOLD :0x%x\n", val);
	return 0;
}

static void pmic_check_rst(unsigned int poff_sts)
{
	unsigned int val;

	/*
	 * TOP_RST_STATUS is used to indicate which reset happened
	 * If a reset happened, the corresponding bit will be clear
	 */
	val = upmu_get_reg_value(MT6359_TOP_RST_STATUS);
	if (val != 0x7F) {
		if (val == 0)
			pal_log_warn("[%s] PORSTB\n", __func__);
		else if (((val >> 1) & 1) == 0 && (poff_sts >> 9) & 1)
			pal_log_warn("[%s] DDLO_RSTB\n", __func__);
		else if (((val >> 2) & 1) == 0 && (poff_sts >> 0) & 1)
			pal_log_warn("[%s] UVLO_RSTB\n", __func__);
		val = upmu_set_reg_value(MT6359_TOP_RST_STATUS_SET, 0x78 | val);
	}
	if ((poff_sts >> 8) & 1)
		pal_log_warn("[%s] BWDT\n", __func__);
	if ((poff_sts >> 6) & 1)
		pal_log_warn("[%s] Long press shutdown\n", __func__);
	if ((poff_sts >> 5) & 1)
		pal_log_warn("[%s] Cold Reset\n", __func__);
	if ((poff_sts >> 13) & 1)
		pal_log_warn("[%s] PWRKEY short press\n", __func__);
	if ((poff_sts >> 10) & 1)
		pal_log_warn("[%s] AP Watchdog\n", __func__);

	/* Long press shutdown status */
	pmic_read_interface(PMIC_JUST_PWRKEY_RST_ADDR, &g_just_rst,
		PMIC_JUST_PWRKEY_RST_MASK, PMIC_JUST_PWRKEY_RST_SHIFT);
	pal_log_warn("[PMIC]just_rst = %d\n", g_just_rst);
	/* LK mtk_key will clear long press status */
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

void pmic_dbg_status(unsigned char option)
{
	/*--option = 1--*/
	/* UVLO off */
	/* power not good */
	/* buck oc */
	/* thermal shutdown 150 */
	/* long press shutdown */
	/* WDTRST */
	/* CLK TRIM */
	/* WDTDBG_VOSEL */
	/*--option = 0--*/
	/* Clear PONSTS, POFFSTS and other exception status */
	int ret_val = 0;
	unsigned int pon_sts = 0;
	unsigned int poff_sts = 0;

	if (option) {
	/* pwhold must set to 1 to update debug status */
		PMIC_POWER_HOLD(1);
	/*--Check if PWRKEY released during boot up--*/
		pmic_read_interface(PMIC_PUP_PKEY_RELEASE_ADDR, &g_pwrkey_release,
			PMIC_PUP_PKEY_RELEASE_MASK, PMIC_PUP_PKEY_RELEASE_SHIFT);
	/*--UVLO off--*/
		pal_log_warn("[PMIC]TOP_RST_STATUS[0x%x]=0x%x\n",
			MT6359_TOP_RST_STATUS, upmu_get_reg_value(MT6359_TOP_RST_STATUS));
	/*special for RTC Alarm and SPAR*/
		pon_sts = upmu_get_reg_value(MT6359_PONSTS);
		g_sts_rtca = (pon_sts >> PMIC_STS_RTCA_SHIFT) & PMIC_STS_RTCA_MASK;
		g_sts_spar = (pon_sts >> PMIC_STS_SPAR_SHIFT) & PMIC_STS_SPAR_MASK;
		pal_log_warn("[PMIC]PONSTS[0x%x]=0x%x\n",
			MT6359_PONSTS, pon_sts);
	/*special for cold rest*/
		poff_sts = upmu_get_reg_value(MT6359_POFFSTS);
		g_sts_crst = (poff_sts >> PMIC_STS_CRST_SHIFT) & PMIC_STS_CRST_MASK;
		pal_log_warn("[PMIC]POFFSTS[0x%x]=0x%x\n",
			MT6359_POFFSTS, poff_sts);
	/*--power not good--*/
		pal_log_warn("[PMIC]PG_SDN_STS0[0x%x]=0x%x\n",
			MT6359_PG_SDN_STS0, upmu_get_reg_value(MT6359_PG_SDN_STS0));
		pal_log_warn("[PMIC]PG_SDN_STS1[0x%x]=0x%x\n",
			MT6359_PG_SDN_STS1, upmu_get_reg_value(MT6359_PG_SDN_STS1));
	/*--buck oc--*/
		pal_log_warn("[PMIC]OC_SDN_STS0[0x%x]=0x%x\n",
			MT6359_OC_SDN_STS0, upmu_get_reg_value(MT6359_OC_SDN_STS0));
		pal_log_warn("[PMIC]OC_SDN_STS1[0x%x]=0x%x\n",
			MT6359_OC_SDN_STS1, upmu_get_reg_value(MT6359_OC_SDN_STS1));
		pal_log_warn("[PMIC]BUCK_OC_SDN_STATUS[0x%x]=0x%x\n",
			MT6359_BUCK_TOP_OC_CON0, upmu_get_reg_value(MT6359_BUCK_TOP_OC_CON0));
		pal_log_warn("[PMIC]BUCK_OC_SDN_EN[0x%x]=0x%x\n",
			MT6359_BUCK_TOP_ELR0, upmu_get_reg_value(MT6359_BUCK_TOP_ELR0));
	/*--thermal shutdown 150--*/
		pal_log_warn("[PMIC]THERMALSTATUS[0x%x]=0x%x\n",
			MT6359_THERMALSTATUS, upmu_get_reg_value(MT6359_THERMALSTATUS));
	/*--long press shutdown--*/
		pal_log_warn("[PMIC]STRUP_CON4[0x%x]=0x%x\n",
			MT6359_STRUP_CON4, upmu_get_reg_value(MT6359_STRUP_CON4));
	/*--WDTRST--*/
		pal_log_warn("[PMIC]TOP_RST_MISC[0x%x]=0x%x\n",
			MT6359_TOP_RST_MISC, upmu_get_reg_value(MT6359_TOP_RST_MISC));
	/*--CLK TRIM--*/
		pal_log_warn("[PMIC]TOP_CLK_TRIM[0x%x]=0x%x\n",
			MT6359_TOP_CLK_TRIM, upmu_get_reg_value(MT6359_TOP_CLK_TRIM));
	/*--WDTRSTB_STATUS will be clear in kernel--*/
		mt6359_wdtdbg_vosel();
	/*--Check PMIC reset reason--*/
		pmic_check_rst(poff_sts);

		record_pmic_boot_status(pon_sts, poff_sts);
	} else {
	/*--Clear PONSTS and POFFSTS(include PG status and BUCK OC status)--*/
		ret_val = pmic_config_interface(PMIC_RG_POFFSTS_CLR_ADDR, 0x1,
			PMIC_RG_POFFSTS_CLR_MASK, PMIC_RG_POFFSTS_CLR_SHIFT);
		ret_val = pmic_config_interface(PMIC_RG_PONSTS_CLR_ADDR, 0x1,
			PMIC_RG_PONSTS_CLR_MASK, PMIC_RG_PONSTS_CLR_SHIFT);
		ret_val = pmic_config_interface(PMIC_RG_POFFSTS_CLR_ADDR, 0x0,
			PMIC_RG_POFFSTS_CLR_MASK, PMIC_RG_POFFSTS_CLR_SHIFT);
		ret_val = pmic_config_interface(PMIC_RG_PONSTS_CLR_ADDR, 0x0,
			PMIC_RG_PONSTS_CLR_MASK, PMIC_RG_PONSTS_CLR_SHIFT);
	/*--clear OC_SDN_STATUS--*/
		ret_val = pmic_config_interface(MT6359_BUCK_TOP_OC_CON0, 0xFF, 0xFF, 0);
	/*--Clear thermal shutdown--*/
		ret_val = pmic_config_interface(PMIC_RG_STRUP_THR_CLR_ADDR, 0x1,
			PMIC_RG_STRUP_THR_CLR_MASK, PMIC_RG_STRUP_THR_CLR_SHIFT);
		ret_val = pmic_config_interface(PMIC_RG_STRUP_THR_CLR_ADDR, 0x0,
			PMIC_RG_STRUP_THR_CLR_MASK, PMIC_RG_STRUP_THR_CLR_SHIFT);
	/*--Long press shutdown will be clear by pmic_check_rst()--*/
	}
}

void pmic_disable_usbdl_wo_battery(void)
{
#if 0
	pal_log_info("[PMIC]disable usbdl wo battery\n");

	/* ask shin-shyu programming guide TBD */
	pmic_config_interface(PMIC_RG_ULC_DET_EN_ADDR,1,PMIC_RG_ULC_DET_EN_MASK,PMIC_RG_ULC_DET_EN_SHIFT);
	pmic_config_interface(PMIC_RG_USBDL_SET_ADDR, 0x0000, PMIC_RG_USBDL_SET_MASK, PMIC_RG_USBDL_SET_SHIFT);
	pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 0x0001, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);
#endif
}

void pmic_wdt_set(void)
{
	unsigned int ret_val = 0;
	/*--Reset digital only--*/
	/*--Enable WDT--*/
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_SET_ADDR, 0x0020, 0xFFFF, 0); /*--[5]=1, RG_WDTRSTB_DEB--*/
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_CLR_ADDR, 0x0002, 0xFFFF, 0); /*--[1]=0, RG_WDTRSTB_MODE--*/
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_SET_ADDR, 0x0001, 0xFFFF, 0); /*--[0]=1, RG_WDTRSTB_EN--*/
	pal_log_info("[%s] TOP_RST_MISC=0x%x\n", __func__,
			upmu_get_reg_value(MT6359_TOP_RST_MISC));
}

/******************************************************************************************************
* AUXADC
******************************************************************************************************/
/* extend r_val to 10 times from PMIC MT6359 */
struct pmic_auxadc_channel_new pmic_auxadc_channel[] = {
	/* BATADC */
	PMIC_AUXADC_GEN(15, 35, 0, PMIC_AUXADC_RQST_CH0,
		PMIC_AUXADC_ADC_RDY_CH0_BY_AP, PMIC_AUXADC_ADC_OUT_CH0_BY_AP),
	/* BAT TEMP */
	PMIC_AUXADC_GEN(12, 25, 3, PMIC_AUXADC_RQST_CH3,
		PMIC_AUXADC_ADC_RDY_CH3, PMIC_AUXADC_ADC_OUT_CH3),
	/* CHIP TEMP */
	PMIC_AUXADC_GEN(12, 10, 4, PMIC_AUXADC_RQST_CH4,
		PMIC_AUXADC_ADC_RDY_CH4, PMIC_AUXADC_ADC_OUT_CH4),
	/* VCORE_TEMP */
	PMIC_AUXADC_GEN(12, 10, 4, PMIC_AUXADC_RQST_CH4_BY_THR1,
		PMIC_AUXADC_ADC_RDY_CH4_BY_THR1, PMIC_AUXADC_ADC_OUT_CH4_BY_THR1),
	/* VPROC_TEMP */
	PMIC_AUXADC_GEN(12, 10, 4, PMIC_AUXADC_RQST_CH4_BY_THR2,
		PMIC_AUXADC_ADC_RDY_CH4_BY_THR2, PMIC_AUXADC_ADC_OUT_CH4_BY_THR2),
	/* VGPU_TEMP */
	PMIC_AUXADC_GEN(12, 10, 4, PMIC_AUXADC_RQST_CH4_BY_THR3,
		PMIC_AUXADC_ADC_RDY_CH4_BY_THR3, PMIC_AUXADC_ADC_OUT_CH4_BY_THR3),
	/* ACCDET Multi-Key */
	PMIC_AUXADC_GEN(12, 10, 5, PMIC_AUXADC_RQST_CH5,
		PMIC_AUXADC_ADC_RDY_CH5, PMIC_AUXADC_ADC_OUT_CH5),
	/* DCXO voltage */
	PMIC_AUXADC_GEN(12, 15, 6, PMIC_AUXADC_RQST_CH6,
		PMIC_AUXADC_ADC_RDY_CH6, PMIC_AUXADC_ADC_OUT_CH6),
	/* TSX TEMP(GPS) */
	PMIC_AUXADC_GEN(15, 10, 7, PMIC_AUXADC_RQST_CH7_BY_GPS,
		PMIC_AUXADC_ADC_RDY_CH7_BY_GPS, PMIC_AUXADC_ADC_OUT_CH7_BY_GPS),
	/* HP OFFSET CAL */
	PMIC_AUXADC_GEN(15, 10, 9, PMIC_AUXADC_RQST_CH9,
		PMIC_AUXADC_ADC_RDY_CH9, PMIC_AUXADC_ADC_OUT_CH9),
	/* DCXO TEMP */
	PMIC_AUXADC_GEN(15, 10, 10, PMIC_AUXADC_RQST_DCXO_BY_GPS,
		PMIC_AUXADC_ADC_RDY_DCXO_BY_GPS, PMIC_AUXADC_ADC_OUT_DCXO_BY_GPS),
	/* VBIF */
	PMIC_AUXADC_GEN(12, 25, 11, PMIC_AUXADC_RQST_CH11,
		PMIC_AUXADC_ADC_RDY_CH11, PMIC_AUXADC_ADC_OUT_CH11),
};

static int count_time_out = 15;
#define VOLT_FULL	1800

int pmic_get_auxadc_value(unsigned short channel)
{
	int count = 0;
	signed int adc_result = 0, reg_val = 0;
	struct pmic_auxadc_channel_new *auxadc_channel;

	if (channel < AUXADC_LIST_BATADC || channel >= AUXADC_LIST_MAX) {
		pal_log_err("[%s] Invalid channel(%d)\n", __func__, channel);
		return -1;
	}
	auxadc_channel = &pmic_auxadc_channel[channel];

	pmic_set_register_value(auxadc_channel->channel_rqst, 1);
	udelay(10);

	while (pmic_get_register_value(auxadc_channel->channel_rdy) != 1) {
		udelay(1300);
		count++;
		if (count > count_time_out) {
			pal_log_err("[%s] (%d) Time out!\n",
				__func__, auxadc_channel->ch_num);
			break;
		}
	}
	reg_val = pmic_get_register_value(auxadc_channel->channel_out);

	/* Audio request HPOFS to return raw data */
	/* V = (reg_val * r_val * 1800 / 10) >> (resolution bits) */
	if (channel == AUXADC_LIST_HPOFS_CAL)
		adc_result = reg_val;
	else
		adc_result = ((reg_val * auxadc_channel->r_val * VOLT_FULL) / 10)
				>> auxadc_channel->resolution;

	pal_log_info("[%s] channel = %d, reg_val = 0x%x, adc_result = %d\n",
		__func__, auxadc_channel->ch_num, reg_val, adc_result);
	return adc_result;
}

static void pwrkey_dbg_status(void)
{
	U32 val = 0;

	pmic_read_interface(PMIC_PWRKEY_LONG_PRESS_COUNT_ADDR, &val,
			    PMIC_PWRKEY_LONG_PRESS_COUNT_MASK,
			    PMIC_PWRKEY_LONG_PRESS_COUNT_SHIFT);

	pal_log_info("[%s] powerkey:%d count=%d(ms) g_pwrkey_release=%d\n"
		     , __func__
		     , pmic_detect_powerkey()
		     , val << 5
		     , g_pwrkey_release);
}

/*
 * PMIC Init Code
 */
U32 pmic_init (void)
{
	U32 ret_code = PMIC_TEST_PASS;
	int ret_val = 0, val;

	pal_log_info("[PMIC]Preloader Start\n");

	pal_log_info("[PMIC] CHIP Code = 0x%x\n",
		get_PMIC_chip_version());

	/* Boot debug status */
	pmic_dbg_status(1);

	pwrkey_dbg_status();

	/* ask shin-shyu programming guide TBD */
	if(hw_check_battery() == 1)
		pmic_disable_usbdl_wo_battery();

	/* Enable PMIC WDTRST function (depends on main chip RST function) TBD */
	pmic_wdt_set();

	mt6359_probe();
#ifdef MT6360_PMIC
	mt6360_pmic_i2c_probe();
#endif /* MT6360_PMIC */
#ifdef MT6360_LDO
	mt6360_ldo_i2c_probe();
#endif /* MT6360_LDO */

	pal_log_info("[PMIC]Init done\n");

	/* wk_power_down_seq(); */

#if 0 /* Test PMIC AUXADC */
	pmic_get_auxadc_value(AUXADC_LIST_BATADC);
	pmic_get_auxadc_value(AUXADC_LIST_VCDT);
	pmic_get_auxadc_value(AUXADC_LIST_BATTEMP);
	pmic_get_auxadc_value(AUXADC_LIST_VBIF);
	pmic_get_auxadc_value(AUXADC_LIST_TSX_TEMP);
#endif
	/* turn on VIO28 for bring up */
	pmic_config_interface(PMIC_RG_LDO_VIO28_EN_ADDR, 1,
		PMIC_RG_LDO_VIO28_EN_MASK, PMIC_RG_LDO_VIO28_EN_SHIFT);
	/* default disable smart reset, enablt it by RGU driver */
	pmic_config_interface(PMIC_RG_SMART_RST_MODE_ADDR, 0x0,
			      PMIC_RG_SMART_RST_MODE_MASK,
			      PMIC_RG_SMART_RST_MODE_SHIFT);
	pmic_config_interface(PMIC_RG_SMART_RST_SDN_EN_ADDR, 0x0,
			      PMIC_RG_SMART_RST_SDN_EN_MASK,
			      PMIC_RG_SMART_RST_SDN_EN_SHIFT);

	return ret_code;
}


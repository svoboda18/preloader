#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <regulator/mtk_regulator.h>
#include <mt6356.h>
#include <pmic.h>
#include <sec_devinfo.h>
#include <rtc.h>

/*#define EXTERNAL_BUCK_MT6311*/
/*#define EXTERNAL_BUCK_RT5738*/

/* External BUCK Config */
#if defined(EXTERNAL_BUCK_MT6311)
#include <mt6311.h>
#endif

#if defined(EXTERNAL_BUCK_RT5738)
#include <rt5738.h>
#endif /* EXTERNAL_BUCK_RT5738) */

#define DEBUGSTS 1
#define EFUSE_DEBUG 0
#define SMART_RESET 0

/* Bianco only API which is used to check VCORE not lower 0.8V */
void wk_vcore_check(int check_vol)
{
	struct mtk_regulator reg_vcore;
	int vcore_vol = 0;
	int ret = 0;

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		print("mtk_regulator_get reg_vcore failed\n");
	vcore_vol = mtk_regulator_get_voltage(&reg_vcore);
	if (vcore_vol < check_vol) {
		print("vcore = %d < %d, ASSERT!\n", vcore_vol, check_vol);
		ASSERT(0);
	}
}

//==============================================================================
// PMIC access API
//==============================================================================
U32 pmic_read_interface (U32 RegNum, U32 *val, U32 MASK, U32 SHIFT)
{
	U32 return_value = 0;
	U32 pmic_reg = 0;
	U32 rdata;

	return_value = pwrap_read(RegNum, &rdata);
	pmic_reg = rdata;
	if (return_value != 0) {
		print("[PMIC]Reg[0x%x]= pmic_wrap read data fail\n", RegNum);
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
	U32 rdata;

	return_value = pwrap_read(RegNum, &rdata);
	pmic_reg = rdata;
	if(return_value!=0)
	{
		print("[PMIC]Reg[%x]= pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	//print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

	pmic_reg &= ~(MASK << SHIFT);
	pmic_reg |= (val << SHIFT);

	//2. mt_write_byte(RegNum, pmic_reg);
	return_value = pwrap_write(RegNum, pmic_reg);
	if (return_value != 0) {
		print("[PMIC]Reg[%x]= pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	//print("[pmic_config_interface] write Reg[%x]=0x%x\n", RegNum, pmic_reg);

	return return_value;
}

U32 pmic_get_register_value(pmuflag pmureg)
{
	U32 val;
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

//==============================================================================
// PMIC Usage APIs
//==============================================================================
/* For Bianco distinguish dram type in MT6356, not common API */
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
	return PMIC_CHRDET_EXIST;	// for bring up
	//print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);
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
	val = 1; // for bring up
	//print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);
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

	ret = pmic_config_interface((U32)(PMIC_RG_LDO_VUSB_EN_0_ADDR), 1,
		(U32)(PMIC_RG_LDO_VUSB_EN_0_MASK),
		(U32)(PMIC_RG_LDO_VUSB_EN_0_SHIFT));

	return ret;
}

#if 0 //remove unused function to save space of log string
void mt6356_dump_register(void)
{
	unsigned short i = 0;

	print("dump PMIC 6356 register\n");

	for (i = 0; i <= 0x1706; i = i + 10) {
		print("Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x\n",
			i, upmu_get_reg_value(i),
			i + 2, upmu_get_reg_value(i + 2),
			i + 4, upmu_get_reg_value(i + 4),
			i + 6, upmu_get_reg_value(i + 6),
			i + 8, upmu_get_reg_value(i + 8));
	}
}
#endif //remove unused function to save space of log string

/*
 * PMIC Export API
 */
static U16 g_sts_crst;

/* check if PMIC cold reset at previous power off */
U16 is_pmic_cold_reset(void)
{
	return g_sts_crst;
}

int PMIC_POWER_HOLD(unsigned int hold)
{
	if (hold > 1) {
		print("[PMIC] hold = %d, only 0 or 1\n", hold);
		return -1;
	}

	if (hold)
		print("[PMIC]POWER_HOLD ON\n");
	else
		print("[PMIC]POWER_HOLD OFF\n");
	/* MT6356 must keep power hold */
	pmic_config_interface(PMIC_RG_PWRHOLD_ADDR, hold, PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);

	print("[PMIC]PowerHold = 0x%x\n", upmu_get_reg_value(MT6356_PPCCTL0));
	return 0;
}

static int vbat_status = PMIC_VBAT_NOT_DROP;
void pmic_DetectVbatDrop (void)
{
	U32 ret = 0;
	U32 just_rst = 0;

	/* Long press shutdown status */
	pmic_read_interface(PMIC_JUST_PWRKEY_RST_ADDR, (&just_rst),
		PMIC_JUST_PWRKEY_RST_MASK, PMIC_JUST_PWRKEY_RST_SHIFT);
	pmic_config_interface(PMIC_CLR_JUST_RST_ADDR, 1,
		PMIC_CLR_JUST_RST_MASK, PMIC_CLR_JUST_RST_SHIFT);
	udelay(62);
	pmic_config_interface(PMIC_CLR_JUST_RST_ADDR, 0,
		PMIC_CLR_JUST_RST_MASK, PMIC_CLR_JUST_RST_SHIFT);

	print("[PMIC]just_rst = %d\n", just_rst);
	if (just_rst)
		vbat_status = PMIC_VBAT_DROP;
	else
		vbat_status = PMIC_VBAT_NOT_DROP;

}

int pmic_IsVbatDrop(void)
{
	/* used by RTC to check power on reason */
	return vbat_status;
}

void pmic_check_rst(unsigned int poff_sts)
{
	unsigned int val;

	/*
	 * TOP_RST_STATUS is used to indicate which reset happened
	 * If a reset happened, the corresponding bit will be clear
	 */
	val = upmu_get_reg_value(MT6356_TOP_RST_STATUS);
	if (val != 0x7F) {
		if (((val >> 0) & 1) == 0)
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
		val = upmu_set_reg_value(MT6356_TOP_RST_STATUS, 0x7F);
	}
	if ((poff_sts >> 8) & 1)
		print("[%s] PMIC BWDT\n", __func__);
	if ((poff_sts >> 6) & 1)
		print("[%s] Long press shutdown\n", __func__);
	if ((poff_sts >> 5) & 1)
		print("[%s] PMIC Cold Reset\n", __func__);
	if ((poff_sts >> 10) & 1)
		print("[%s] AP Watchdog\n", __func__);
	if ((poff_sts >> 4) & 1)
		print("[%s] PMIC WARM Reset\n", __func__);

#if SMART_RESET
	/* Smart reset status */
	pmic_read_interface(PMIC_JUST_SMART_RST_ADDR, &val,
		PMIC_JUST_SMART_RST_MASK, PMIC_JUST_SMART_RST_SHIFT);
	if (val) {
		print("[%s] PMIC Smart Reset\n", __func__);
		pmic_config_interface(PMIC_RG_CLR_JUST_SMART_RST_ADDR, 1,
			PMIC_RG_CLR_JUST_SMART_RST_MASK, PMIC_RG_CLR_JUST_SMART_RST_SHIFT);
		udelay(62);
		pmic_config_interface(PMIC_RG_CLR_JUST_SMART_RST_ADDR, 0,
			PMIC_RG_CLR_JUST_SMART_RST_MASK, PMIC_RG_CLR_JUST_SMART_RST_SHIFT);
	}
#endif
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
	unsigned int poff_sts = 0;

	if (option) {
	/*pwhold must set to 1 to update debug status*/
		PMIC_POWER_HOLD(1);
	/*--UVLO off--*/
		print("[PMIC]TOP_RST_STATUS[0x%x]=0x%x\n",
			MT6356_TOP_RST_STATUS, upmu_get_reg_value(MT6356_TOP_RST_STATUS));
		print("[PMIC]PONSTS[0x%x]=0x%x\n",
			MT6356_PONSTS, upmu_get_reg_value(MT6356_PONSTS));
	/*special for cold rest*/
		poff_sts = upmu_get_reg_value(MT6356_POFFSTS);
		g_sts_crst = (poff_sts >> PMIC_STS_CRST_SHIFT) & PMIC_STS_CRST_MASK;
		print("[PMIC]POFFSTS[0x%x]=0x%x\n",
			MT6356_POFFSTS, poff_sts);
	/*--power not good--*/
		print("[PMIC]PGSTATUS0[0x%x]=0x%x\n",
			MT6356_PG_SDN_STS0, upmu_get_reg_value(MT6356_PG_SDN_STS0));
		print("[PMIC]PGSTATUS1[0x%x]=0x%x\n",
			MT6356_PG_SDN_STS1, upmu_get_reg_value(MT6356_PG_SDN_STS1));
	/*--buck oc--*/
		print("[PMIC]PSOCSTATUS[0x%x]=0x%x\n",
			MT6356_OC_SDN_STS0, upmu_get_reg_value(MT6356_OC_SDN_STS0));
		print("[PMIC]BUCK_OC_SDN_STATUS[0x%x]=0x%x\n",
			MT6356_BUCK_TOP_OC_CON0, upmu_get_reg_value(MT6356_BUCK_TOP_OC_CON0));
		print("[PMIC]BUCK_OC_SDN_EN[0x%x]=0x%x\n",
			MT6356_BUCK_TOP_ELR0, upmu_get_reg_value(MT6356_BUCK_TOP_ELR0));
	/*--thermal shutdown 150--*/
		print("[PMIC]THERMALSTATUS[0x%x]=0x%x\n",
			MT6356_THERMALSTATUS, upmu_get_reg_value(MT6356_THERMALSTATUS));
	/*--long press shutdown--*/
		print("[PMIC]STRUP_CON4[0x%x]=0x%x\n",
			MT6356_STRUP_CON4, upmu_get_reg_value(MT6356_STRUP_CON4));
	/*--WDTRST--*/
		print("[PMIC]TOP_RST_MISC[0x%x]=0x%x\n",
			MT6356_TOP_RST_MISC, upmu_get_reg_value(MT6356_TOP_RST_MISC));
	/*--CLK TRIM--*/
		print("[PMIC]TOP_CLK_TRIM[0x%x]=0x%x\n",
			MT6356_TOP_CLK_TRIM, upmu_get_reg_value(MT6356_TOP_CLK_TRIM));
	/*--Write 1 to clear PONSTS and POFFSTS(include PG status and BUCK OC status)--*/
		ret_val = pmic_config_interface(PMIC_RG_POFFSTS_CLR_ADDR, 0x1,
			PMIC_RG_POFFSTS_CLR_MASK, PMIC_RG_POFFSTS_CLR_SHIFT);/*--clear PONSTS--*/
		ret_val = pmic_config_interface(PMIC_RG_PONSTS_CLR_ADDR, 0x1,
			PMIC_RG_PONSTS_CLR_MASK, PMIC_RG_PONSTS_CLR_SHIFT);/*--clear POFFSTS--*/
		ret_val = pmic_config_interface(MT6356_BUCK_TOP_OC_CON0, 0xFF, 0xFF, 0);/*--clear OC_SDN_STATUS--*/
	/*--Write 1 to clear thermal shutdown--*/
		ret_val = pmic_config_interface(PMIC_RG_STRUP_THR_CLR_ADDR, 0x1,
			PMIC_RG_STRUP_THR_CLR_MASK, PMIC_RG_STRUP_THR_CLR_SHIFT);
	/*--WDTRSTB_STATUS will be clear in kernel--*/

	/*--Long press shutdown will be clear by pmic_DetectVbatDrop()--*/

	/*--Check PMIC reset reason--*/
		pmic_check_rst(poff_sts);
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
	/*pmic_config_interface(PMIC_RG_ULC_DET_EN_ADDR,1,PMIC_RG_ULC_DET_EN_MASK,PMIC_RG_ULC_DET_EN_SHIFT);*//*---MT6336 TBD---*/

	pmic_config_interface(PMIC_RG_USBDL_SET_ADDR, 0x0000, PMIC_RG_USBDL_SET_MASK, PMIC_RG_USBDL_SET_SHIFT);
	pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 0x0001, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);
}

void pmic_wdt_set(void)
{
	unsigned int ret_val = 0;
	/*--Reset digital only--*/
	/*--Enable WDT--*/
	print("[PMIC]%s\n", __func__);
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_SET_ADDR, 0x0020, 0xFFFF, 0); /*--[5]=1, RG_WDTRSTB_DEB--*/
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_CLR_ADDR, 0x0002, 0xFFFF, 0); /*--[1]=0, RG_WDTRSTB_MODE--*/
	ret_val = pmic_config_interface(PMIC_TOP_RST_MISC_SET_ADDR, 0x0001, 0xFFFF, 0); /*--[0]=1, RG_WDTRSTB_EN--*/
	print("[PMIC]%s Reg[0x%x]=0x%x\n", __func__,
			MT6356_TOP_RST_MISC, upmu_get_reg_value(MT6356_TOP_RST_MISC));
}

void pmic_smps_testmode_b(void)
{
	/* To avoid buck is still enable, but ivegen is not exist */
	/* All buck en_anable will be en_vxxx= (~IVGENENB & IVGEN_READY )& AD_QI_VCORE_EN */
	/* ps. ivegen means reference current source */
	unsigned ret_val = 0;
	print("[PMIC] pmic_smps_testmode_b\n");
	ret_val= pmic_config_interface(PMIC_RG_SMPS_TESTMODE_B_ADDR, 0x0001,
		PMIC_RG_SMPS_TESTMODE_B_MASK, PMIC_RG_SMPS_TESTMODE_B_SHIFT); /*--RG_SMPS_TESTMODE_B by luke--*/
}

void pmic_default_voltage(void)
{
	int ret;
	struct mtk_regulator reg_vsram_proc;
	struct mtk_regulator reg_vsram_gpu;
	struct mtk_regulator reg_vsram_others;
	struct mtk_regulator reg_vproc;
	struct mtk_regulator reg_vcore;
	struct mtk_regulator reg_vmodem;
	int vsram_proc_vol = 0;
	int vsram_gpu_vol = 0;
	int vsram_others_vol = 0;
	int vproc_vol = 0;
	int vcore_vol = 0;
	int vmodem_vol = 0;

#if 1 /* Bianco+ and Bianco have the same default voltage */
	vsram_proc_vol = 900000;
	vsram_gpu_vol = 900000;
	vsram_others_vol = 900000;
	vproc_vol = 800000;
	vcore_vol = 800000;
	vmodem_vol = 775000;
#else
	if (is_mt6311_exist()) {
		/* with MT6311 */
		vsram_proc_vol = 900000;
		vsram_gpu_vol = 900000;
		vsram_others_vol = 900000;
		vproc_vol = 800000;
		vcore_vol = 800000;
		vmodem_vol = 775000;
	} else {
		/* without MT6311 */
		vsram_proc_vol = 900000;
		vsram_gpu_vol = 900000;
		vsram_others_vol = 900000;
		vproc_vol = 800000;
		vcore_vol = 800000;
		vmodem_vol = 775000;
	}
#endif

	/*--Get regulator handle--*/
	ret = mtk_regulator_get("vsram_proc", &reg_vsram_proc);
	if (ret)
		print("mtk_regulator_get reg_vsram_proc failed\n");

	ret = mtk_regulator_get("vsram_gpu", &reg_vsram_gpu);
	if (ret)
		print("mtk_regulator_get reg_vsram_gpu failed\n");

	ret = mtk_regulator_get("vsram_others", &reg_vsram_others);
	if (ret)
		print("mtk_regulator_get reg_vsram_others failed\n");

	ret = mtk_regulator_get("vproc", &reg_vproc);
	if (ret)
		print("mtk_regulator_get reg_vproc failed\n");

	ret = mtk_regulator_get("vcore", &reg_vcore);
	if (ret)
		print("mtk_regulator_get reg_vcore failed\n");

	ret = mtk_regulator_get("vmodem", &reg_vmodem);
	if (ret)
		print("mtk_regulator_get reg_vmodem failed\n");

	/*--Set voltage--*/
	/*
	 * To keep 100mV <= vsram_proc - vproc <= 250mV
	 * need to set their voltage step by step
	 */
#if defined(EXTERNAL_BUCK_MT6311)
	if (is_mt6311_exist()) {
	/* MT6311 = 0.9V -> vsram_proc = 1.0V -> MT6311 = 0.8V -> vsram_proc = 0.9V */
		ret = mt6311_config_interface(0x8E, 0x30, 0x7F, 0);
		udelay(50);
		ret = mtk_regulator_set_voltage(&reg_vsram_proc, 1000000, vsram_proc_max_uV);
		udelay(10);
		ret = mt6311_config_interface(0x8E, 0x20, 0x7F, 0);
		udelay(50);
		ret = mtk_regulator_set_voltage(&reg_vsram_proc, vsram_proc_vol, vsram_proc_max_uV);
		if (ret)
			print("mtk_regulator_set_voltage reg_vsram_proc failed\n");
	} else {
#else
	{
#endif
	/* vsram_proc = 1.0V -> vproc = 0.8V -> vsram_proc = 0.9V */
		ret = mtk_regulator_set_voltage(&reg_vsram_proc, 1000000, vsram_proc_max_uV);
		udelay(10);
		ret  = mtk_regulator_set_voltage(&reg_vproc, vproc_vol, vproc_max_uV);
		udelay(20);
		if (ret)
			print("mtk_regulator_set_voltage reg_vproc failed\n");
		ret = mtk_regulator_set_voltage(&reg_vsram_proc, vsram_proc_vol, vsram_proc_max_uV);
		if (ret)
			print("mtk_regulator_set_voltage reg_vsram_proc failed\n");
	}

	ret  = mtk_regulator_set_voltage(&reg_vsram_gpu, vsram_gpu_vol, vsram_gpu_max_uV);
	if (ret)
		print("mtk_regulator_set_voltage reg_vsram_gpu failed\n");

	ret  = mtk_regulator_set_voltage(&reg_vsram_others, vsram_others_vol, vsram_others_max_uV);
	if (ret)
		print("mtk_regulator_set_voltage reg_vsram_others failed\n");

	ret  = mtk_regulator_set_voltage(&reg_vcore, vcore_vol, vcore_max_uV);
	if (ret)
		print("mtk_regulator_set_voltage reg_vcore failed\n");

	ret  = mtk_regulator_set_voltage(&reg_vmodem, vmodem_vol, vmodem_max_uV);
	if (ret)
		print("mtk_regulator_set_voltage reg_vmodem failed\n");

	/*--Get voltage--*/
	print("vsram_proc = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc));
	print("vsram_gpu = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_gpu));
	print("vsram_others = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_others));
	print("vproc = %d uV\n", mtk_regulator_get_voltage(&reg_vproc));
	print("vcore = %d uV\n", mtk_regulator_get_voltage(&reg_vcore));
	print("vmodem = %d uV\n", mtk_regulator_get_voltage(&reg_vmodem));

	/*--enable--*/
	ret = mtk_regulator_enable(&reg_vsram_proc, 1);
	if (ret)
		print("enable reg_vsram_proc failed\n");

#if defined(EXTERNAL_BUCK_MT6311)
	if (is_mt6311_exist()) {
		ret = mtk_regulator_enable(&reg_vsram_gpu, 1);
		if (ret)
			print("enable reg_vsram_gpu failed\n");
	} else {
#else
	{
#endif
		/* MT6763 w/o MT6311, no need to enable vsram_gpu since GPU uses vsram_others */
		/*ret = mtk_regulator_enable(&reg_vsram_gpu, 0);
		 *if (ret)
			print("disable reg_vsram_gpu failed\n");
		 */
	}

	ret = mtk_regulator_enable(&reg_vsram_others, 1);
	if (ret)
		print("enable reg_vsram_others failed\n");

	ret = mtk_regulator_enable(&reg_vproc, 1);
	if (ret)
		print("enable reg_vproc11 failed\n");

	ret = mtk_regulator_enable(&reg_vcore, 1);
	if (ret)
		print("enable reg_vcore failed\n");

	ret = mtk_regulator_enable(&reg_vmodem, 1);
	if (ret)
		print("enable reg_vmodem failed\n");
}

/******************************************************************************************************
* AUXADC
******************************************************************************************************/
struct mtk_auxadc_t mtk_auxadc[] = {
	/* BATADC */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH0_BY_AP_ADDR, PMIC_AUXADC_ADC_OUT_CH0_BY_AP_ADDR, PMIC_AUXADC_RQST_CH0_ADDR, 0, 15, 3),
	/* VCDT */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH2_ADDR, PMIC_AUXADC_ADC_OUT_CH2_ADDR, PMIC_AUXADC_RQST_CH2_ADDR, 2, 12, 1),
	/* BAT TEMP */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH3_ADDR, PMIC_AUXADC_ADC_OUT_CH3_ADDR, PMIC_AUXADC_RQST_CH3_ADDR, 3, 12, 2),
	/* BATID */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_BATID_ADDR, PMIC_AUXADC_ADC_OUT_BATID_ADDR, PMIC_AUXADC_RQST_BATID_ADDR, 3, 12, 2),
	/* VBIF */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH11_ADDR, PMIC_AUXADC_ADC_OUT_CH11_ADDR, PMIC_AUXADC_RQST_CH11_ADDR, 11, 12, 1),
	/* CHIP TEMP */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH4_ADDR, PMIC_AUXADC_ADC_OUT_CH4_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
	/* DCXO */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_DCXO_BY_AP_ADDR, PMIC_AUXADC_ADC_OUT_DCXO_BY_AP_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
	/* ACCDET */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH5_ADDR, PMIC_AUXADC_ADC_OUT_CH5_ADDR, PMIC_AUXADC_RQST_CH5_ADDR, 5, 12, 1),
	/* TSX */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH7_ADDR, PMIC_AUXADC_ADC_OUT_CH7_ADDR, PMIC_AUXADC_RQST_CH7_ADDR, 7, 15, 1),
	/* HP OFFSET CAL */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH9_ADDR, PMIC_AUXADC_ADC_OUT_CH9_ADDR, PMIC_AUXADC_RQST_CH9_ADDR, 9, 12, 1),
	/* ISENSE */
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH1_BY_AP_ADDR, PMIC_AUXADC_ADC_OUT_CH1_BY_AP_ADDR, PMIC_AUXADC_RQST_CH1_ADDR, 1, 15, 3),
};

#if !CFG_EVB_PLATFORM || defined(MTK_EFUSE_WRITER_RESERVE_CODESIZE)
kal_int32 count_time_out = 10;
#define VOLTAGE_FULL_RANGE	1800
#define ADC_PRECISE		32768 // 15 bits

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


//==============================================================================
// AUXADC Init Code
//==============================================================================
int pmic_auxadc_init_setting_v1(void)
{
	print("[PMIC] %s\n", __func__);

	/* Setting of Dynamic CLK Management */
	pmic_config_interface(PMIC_AUXADC_CK_AON_ADDR, 0,
		PMIC_AUXADC_CK_AON_MASK, PMIC_AUXADC_CK_AON_SHIFT);
	pmic_config_interface(PMIC_AUXADC_CK_AON_MD_ADDR, 0,
		PMIC_AUXADC_CK_AON_MD_MASK, PMIC_AUXADC_CK_AON_MD_SHIFT);
	pmic_config_interface(PMIC_AUXADC_CK_AON_GPS_ADDR, 0,
		PMIC_AUXADC_CK_AON_GPS_MASK, PMIC_AUXADC_CK_AON_GPS_SHIFT);

	/* Setting of ADC not being reset in Sleep mode */
	pmic_config_interface(PMIC_RG_STRUP_AUXADC_RSTB_SW_ADDR, 1,
		PMIC_RG_STRUP_AUXADC_RSTB_SW_MASK, PMIC_RG_STRUP_AUXADC_RSTB_SW_SHIFT);
	pmic_config_interface(PMIC_RG_STRUP_AUXADC_RSTB_SEL_ADDR, 1,
		PMIC_RG_STRUP_AUXADC_RSTB_SEL_MASK, PMIC_RG_STRUP_AUXADC_RSTB_SEL_SHIFT);

	/* Settings of ADC receiving resume start from STRUP */
	pmic_config_interface(PMIC_RG_STRUP_AUXADC_START_SEL_ADDR, 0,
		PMIC_RG_STRUP_AUXADC_START_SEL_MASK, PMIC_RG_STRUP_AUXADC_START_SEL_SHIFT);

	/* Settings of ADC receiving resume start from STRUP */
	pmic_config_interface(PMIC_AUXADC_DATA_REUSE_SEL_ADDR, 0,
		PMIC_AUXADC_DATA_REUSE_SEL_MASK, PMIC_AUXADC_DATA_REUSE_SEL_SHIFT);
	pmic_config_interface(PMIC_AUXADC_DATA_REUSE_EN_ADDR, 1,
		PMIC_AUXADC_DATA_REUSE_EN_MASK, PMIC_AUXADC_DATA_REUSE_EN_SHIFT);

	return 0;
}

//==============================================================================
// PMIC EFUSE
//==============================================================================
/* PMIC EFUSE SW load need to check EFUSE_TABLE */
static void pmic_efuse_sw_load(void)
{
	unsigned int efuse_data = 0;

	efuse_data = pmic_read_efuse_nolock(2);
	pmic_config_interface(0x0D3A, (efuse_data >> 8) & 1, 0x1, 9);
	pmic_config_interface(0x0D46, (efuse_data >> 9) & 1, 0x1, 9);
	pmic_config_interface(0x0D2A, (efuse_data >> 10) & 0x7, 0x7, 6);
}

//==============================================================================
// PMIC Init Code
//==============================================================================
void __attribute__ ((weak)) pl_battery_init(bool force_init)
{
	return;
}

U32 pmic_init (void)
{
	U32 ret_code = PMIC_TEST_PASS;
	U32 poff_sts = 0;
	U16 pwhold = 0;
	int ret_val = 0, val;

	print("[PMIC]Preloader Start\n");

	print("[PMIC]MT6356 CHIP Code = 0x%x\n",
		get_PMIC_chip_version());

	/* Boot debug status */
	pmic_dbg_status(1);
	/* for auxadc init */
	pmic_auxadc_init_setting_v1();
	/*pmic_auxadc_debug(1);*/
	/* detect V battery Drop   used by RTC to check power on reason  */
	/* ask waverly programming guide */
	pmic_DetectVbatDrop();

	/* PMIC SW load EFUSE to target register */
	pmic_efuse_sw_load();

	/* ask shin-shyu programming guide TBD */
	if(hw_check_battery() == 1)
		pmic_disable_usbdl_wo_battery();

	/* Enable PMIC WDTRST function (depends on main chip RST function) TBD */
	pmic_wdt_set();
	/* ask peter programming guide RG_SMPS_TESTMODE_B by luke TBD */
	pmic_smps_testmode_b();

#if defined(EXTERNAL_BUCK_MT6311)
	mt6311_driver_probe();
	if (is_mt6311_exist() == 1)
		/* use TOP_MDB_CONF0 Reg[0x20E][15] as the flag of MT6311 exists */
		pmic_config_interface(MT6356_TOP_MDB_CONF0, 0x1, 0x1, 15);
#endif

#if defined(EXTERNAL_BUCK_RT5738)
        rt5738_driver_probe();
#endif /* EXTERNAL_BUCK_RT5738 */

	pl_battery_init(false);

	if (hw_check_battery() == 1) {
#if !CFG_EVB_PLATFORM
		pl_check_bat_protect_status();
#endif
	}

	pmic_dbg_status(0);

	/*regulator_init();*/
	mt6356_probe();

	pmic_default_voltage();
#if SMART_RESET
	/* Enable Smart reset in MT6763, if keep pressing key for 8 sec, it will shutdown */
	pmic_config_interface(PMIC_RG_SMART_RST_MODE_ADDR, 1,
		PMIC_RG_SMART_RST_MODE_MASK, PMIC_RG_SMART_RST_MODE_SHIFT);
	pmic_config_interface(PMIC_RG_SMART_RST_SDN_EN_ADDR, 1,
		PMIC_RG_SMART_RST_SDN_EN_MASK, PMIC_RG_SMART_RST_SDN_EN_SHIFT);
#endif
	print("Reg[0x384] = 0x%x\n", upmu_get_reg_value(MT6356_STRUP_CON12));

	print("[pmic_init]Done\n");

	return ret_code;
}

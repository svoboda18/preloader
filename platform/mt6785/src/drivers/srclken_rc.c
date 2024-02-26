#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include "ufs_aio_platform.h"
#include "srclken_rc.h"
#include "spm.h"

#define IS_TRUE		(1)
#define IS_FALSE		(0)

/* calibration setting */
#define ULPOSC_CAL_START_VALUE		16
#define ULPOSC_CAL_MAX_VALUE		RG_OSC_CALI_MSK
#define FREQ_METER_ABIST_AD_OSC_CK	38
#define FREQ_260MHZ			260	/* ulposc1 freq et to 262Mhz */

/* RC settle time setting */
#define NO_DCXO_XO_CHN		((1 << CHN_COANT) | (1 << CHN_SCP) | (1 << CHN_RESERVE))
#define BT_ONLY_HW_MODE	((1 << CHN_BT))	/* only BT use HW side-band signal */
#define FULL_SET_HW_MODE	0	/* dcxo mode use pmrc_en */
#define VCORE_SETTLE_TIME	1	/* ~= 30us */
#define ULPOSC_SETTLE_TIME	5	/* ~= 150us */
#if MTK_SRCLKEN_RC_VFE28_ONLY
#define XO_SETTLE_TIME		0x10	/* 2^(step_sz+5) * 0x4 *30.77ns~=31.5us */
#define DCXO_SETTLE_TIME		0x10	/* 2^(step_sz+5) * 0x4 *30.77ns~= 31.5us */
#else
#define XO_SETTLE_TIME		0x33	/* 2^(step_sz+5) * 0x33 *30.77ns~=400us */
#define DCXO_SETTLE_TIME		0x87	/* 2^(step_sz+5) * 0x87 *30.77ns~= 1063us */
#endif
#define CENTROL_CNT_STEP	0x3	/* Fix in 3, central align with Mxx Channel */

/* RC central setting */
#define RC_CENTRAL_ENABLE	1
#define RC_CENTRAL_DISABLE	0
#if (MTK_SRCLKEN_RC_BT_ONLY || MTK_SRCLKEN_RC_VFE28_ONLY)
#define SPI_TRIG_MODE		VREQ_MODE	/* use vreq to set vcore */
#define IS_SPI_DONE_RELEASE	IS_TRUE	/* release vcore when spi request done */
#define IS_SPI2PMIC_SET_CLR	IS_TRUE	/* register direct write */
#define SRCLKENO_0_CTRL_M	BYPASS_OTHER_MODE | ASYNC_MODE /* bypass spm */
#define VREQ_CTRL_M		MERGE_OR_MODE /* merge with vreq */
#define ULPOSC_CTRL_M		MERGE_OR_MODE /* merge with ulposc */
#define PWRAP_CTRL_M		MERGE_OR_MODE	/* merge with pwrap_scp */
#elif MTK_SRCLKEN_RC_FULL_SET
#define SPI_TRIG_MODE		SRCLKENAO_MODE	/* use srlckenao to set vcore */
#define IS_SPI_DONE_RELEASE	IS_FALSE	/* release vcore when spi request done */
#define IS_SPI2PMIC_SET_CLR	IS_FALSE	/* register direct write */
#define SRCLKENO_0_CTRL_M	MERGE_OR_MODE /* merge with spm */
/* need to check this! */
#define VREQ_CTRL_M		BYPASS_RC_MODE	/* merge with vreq */
#define ULPOSC_CTRL_M		MERGE_OR_MODE /* merge with ulposc */
#define PWRAP_CTRL_M		MERGE_OR_MODE	/* merge with pwrap_scp */
#endif
#define SPI_CLK_SRC		RC_32K	/* pmic spec under 200us */

/* other setting */
#define PMRC_CON0		0x1A6	/* default use this reg direct write */
#define PMRC_CON0_SET		0x1A8
#define PMRC_CON0_CLR		0x1AA
#if MTK_SRCLKEN_RC_VFE28_ONLY
#define DCXO_FPM_CTRL_MODE	BYPASS_MODE /* bypass all */
#else
#define DCXO_FPM_CTRL_MODE	MERGE_OR_MODE | ASYNC_MODE /* merge with spm */
#endif

#define PWRAP_TMOUT_VAL	0xA	/* 31us * 0xa ~= 310us */

#define SUB_CTRL(_id, _mode, _req) {	\
		.id = _id,		\
		.mode = _mode,	\
		.req = _req,	\
	}

#if (MTK_SRCLKEN_RC_BT_ONLY || MTK_SRCLKEN_RC_VFE28_ONLY)
#if MTK_SRCLKEN_RC_VFE28_ONLY
#define FPM_MSK_B		FULL_SET_HW_MODE
#define MD0_SRCLKENO_0_MASK_B	IS_FALSE		/* md0 control by srclkeno_0 */

static struct subsys_rc_ctrl rc_ctrl[MAX_CHN_NUM] = {
	SUB_CTRL(CHN_SUSPEND, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_RF, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_DEEPIDLE, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_MD, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_GPS, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_BT, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_WIFI, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_MCU, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_COANT, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_NFC, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_UFS, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_SCP, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_RESERVE, SW_MODE, NO_REQ),
};

#else
#define FPM_MSK_B		BT_ONLY_HW_MODE
#define MD0_SRCLKENO_0_MASK_B	IS_TRUE		/* md0 control by srclkeno_0 */

static struct subsys_rc_ctrl rc_ctrl[MAX_CHN_NUM] = {
	SUB_CTRL(CHN_SUSPEND, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_RF, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_DEEPIDLE, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_MD, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_GPS, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_BT, SW_MODE, BBLPM_REQ),
	SUB_CTRL(CHN_WIFI, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_MCU, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_COANT, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_NFC, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_UFS, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_SCP, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_RESERVE, SW_MODE, NO_REQ),
};

#endif

#define SUB_BBLPM_SET		(1 << CHN_SUSPEND | 1 << CHN_DEEPIDLE | 1 <<CHN_BT \
				| 1 << CHN_COANT | 1 << CHN_SCP)
#define SUB_FPM_SET		(1 << CHN_RF | 1 << CHN_MD | 1 << CHN_GPS \
				| 1 << CHN_WIFI | 1 << CHN_MCU \
				| 1 << CHN_NFC | 1 << CHN_UFS)
#elif MTK_SRCLKEN_RC_FULL_SET
#define FPM_MSK_B		FULL_SET_HW_MODE
#define MD0_SRCLKENO_0_MASK_B	IS_FALSE		/* md0 control by pmrc */

#define SUB_BBLPM_SET		(1 <<CHN_BT | 1 << CHN_COANT | 1 << CHN_SCP)
#define SUB_FPM_SET		(1 << CHN_SUSPEND | 1 << CHN_DEEPIDLE \
				| 1 << CHN_RF | 1 << CHN_MD | 1 << CHN_GPS \
				| 1 << CHN_WIFI | 1 << CHN_MCU \
				| 1 << CHN_NFC | 1 << CHN_UFS)

static struct subsys_rc_ctrl rc_ctrl[MAX_CHN_NUM] = {
	SUB_CTRL(CHN_SUSPEND, SW_MODE, FPM_REQ),
	SUB_CTRL(CHN_RF, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_DEEPIDLE, SW_MODE, FPM_REQ),
	SUB_CTRL(CHN_MD, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_GPS, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_BT, SW_MODE, BBLPM_REQ),
	SUB_CTRL(CHN_WIFI, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_MCU, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_COANT, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_NFC, SW_MODE, NO_REQ),
	SUB_CTRL(CHN_UFS, SW_MODE, FPM_REQ),
	SUB_CTRL(CHN_SCP, SW_MODE, BBLPM_REQ),
	SUB_CTRL(CHN_RESERVE, SW_MODE, NO_REQ),
};
#endif


/* ULPOSC1 calibratin flow and relative setting */
ulposc_con_t ulposc1_setting[1] = {
    {.osc_div = 12, .cali_val = ULPOSC_CAL_START_VALUE},
};

extern unsigned int mt_get_abist_freq(unsigned int ID);
u32 get_ulposc_clk_by_freqmeter(u32 cali_val)
{
	u32 result = 0;

	/* set calibration value */
	DRV_ClrReg32(PLL_ULPOSC_CON0, RG_OSC_CALI_MSK << RG_OSC_CALI_SHFT);
	DRV_SetReg32(PLL_ULPOSC_CON0, (cali_val & RG_OSC_CALI_MSK) << RG_OSC_CALI_SHFT);

	gpt_busy_wait_us(50);

	result = mt_get_abist_freq(FREQ_METER_ABIST_AD_OSC_CK);

	print("cali_val=0x%x, %d KHz\n",
		(u32)cali_val, result);

	return result / 1000;
}

int ulposc_cali_process(u32 target_clk)
{
	u32 target_val = 0, current_val = 0;
	u32 min = 0, max = ULPOSC_CAL_MAX_VALUE, middle;
	u32 diff_by_min = 0, diff_by_max = 0xffff;
	u32 cal_result = 0;

	do {
		middle = (min + max) / 2;
		if (middle == min)
			break;

		current_val = get_ulposc_clk_by_freqmeter(middle);

		if (current_val > target_clk)
			max = middle;
		else
			min = middle;
	} while (min <= max);

	current_val = get_ulposc_clk_by_freqmeter(min);
	if (current_val > target_clk)
		diff_by_min = current_val - target_clk;
	else
		diff_by_min = target_clk - current_val;

	current_val = get_ulposc_clk_by_freqmeter(max);
	if (current_val > target_clk)
		diff_by_max = current_val - target_clk;
	else
		diff_by_max = target_clk - current_val;

	if (diff_by_min < diff_by_max) {
		cal_result = min;
		current_val = get_ulposc_clk_by_freqmeter(min);
	} else {
		cal_result = max;
		current_val = get_ulposc_clk_by_freqmeter(max);
	}

	/* check if calibrated value is in the range of target value +- 15% */
	if (current_val > (target_clk*850/1000) && current_val < (target_clk*1150/1000))
		print("ULPOSC1 K done: %dM\n", current_val);
	else {
		print("ULPOSC1 K fail: %dM\n", current_val);

		return -1;
	}

	return 0;
}

void config_ulposc1_registers(void)
{
	/*
	**** Config ULPOSC_CON0 ***
	*/

	/* OSC_RSV= 00000000 */
	DRV_ClrReg32(PLL_ULPOSC_CON0, RG_OSC_RSV_MSK << RG_OSC_RSV_SHFT);

	/* OSC_CP_EN = 1 */
	DRV_SetReg32(PLL_ULPOSC_CON0, 0x1 << RG_OSC_CP_EN_SHFT);

	/* set OSC_DIV */
	DRV_ClrReg32(PLL_ULPOSC_CON0, RG_OSC_DIV_MSK << RG_OSC_DIV_SHFT);
	DRV_SetReg32(PLL_ULPOSC_CON0,
		(ulposc1_setting[0].osc_div & RG_OSC_DIV_MSK) << RG_OSC_DIV_SHFT);

	/* OSC_FBAND = 0000 */
	DRV_ClrReg32(PLL_ULPOSC_CON0, RG_OSC_FBAND_MSK << RG_OSC_FBAND_SHFT);

	/* OSC_IBAND = 0000100 */
	DRV_ClrReg32(PLL_ULPOSC_CON0, RG_OSC_IBAND_MSK << RG_OSC_IBAND_SHFT);
	DRV_SetReg32(PLL_ULPOSC_CON0, 0x4 << RG_OSC_IBAND_SHFT);

	/* set OSC_CALI */
	DRV_ClrReg32(PLL_ULPOSC_CON0, RG_OSC_CALI_MSK << RG_OSC_CALI_SHFT);
	DRV_SetReg32(PLL_ULPOSC_CON0,
		(ulposc1_setting[0].cali_val & RG_OSC_CALI_MSK) << RG_OSC_CALI_SHFT);

	/*
	**** Config ULPOSC_CON1 ***
	*/

	/* OSC_DIV2_EN = 1 */
	DRV_SetReg32(PLL_ULPOSC_CON1, 0x1 << RG_OSC_DIV2_EN_SHFT);

	/* OSC_MOD = 00 */
	DRV_ClrReg32(PLL_ULPOSC_CON1, RG_OSC_MOD_MSK << RG_OSC_MOD_SHFT);
}

void turn_onoff_ulposc(bool enable)
{

	if (DRV_Reg32(SPM_POWERON_CONFIG_SET) & SPM_REGWR_EN
			!= SPM_REGWR_EN)
		DRV_WriteReg32(SPM_POWERON_CONFIG_SET,
			SPM_REGWR_CFG_KEY | SPM_REGWR_EN);

	if (enable) {
		DRV_SetReg32(ULPOSC_CON, 0x1 << ULPOSC_EN_SHFT);
		gpt_busy_wait_us(1);
		DRV_SetReg32(ULPOSC_CON, 0x1 << ULPOSC_RST_SHFT);
		gpt_busy_wait_us(30);
		DRV_ClrReg32(ULPOSC_CON, 0x1 << ULPOSC_RST_SHFT);
		gpt_busy_wait_us(120);
		DRV_SetReg32(ULPOSC_CON, 0x1 << ULPOSC_CG_EN_SHFT);
	} else {
		DRV_ClrReg32(ULPOSC_CON,
				(0x1 << ULPOSC_RST_SHFT) |
				(0x1 << ULPOSC_RST_SHFT) |
				(0x1 << ULPOSC_CG_EN_SHFT));
	}

	print("%s: ULPOSC_CON: 0x%x\n", __func__, DRV_Reg32(ULPOSC_CON));
}

int ulposc_init(void)
{
	int ret = 0;
	/* calibrate ULPOSC1 */
	config_ulposc1_registers();

	turn_onoff_ulposc(true);

	ret = ulposc_cali_process(FREQ_260MHZ);

	turn_onoff_ulposc(false);

	return ret;
}

/* RC initial flow and relative setting */
static void __rc_ctrl_mode_switch(enum chn_id id, enum rc_ctrl_m mode)
{
#if MTK_SRCLKEN_RC_BRINGUP
	print("Bring-UP : skip srclken_rc switch mode\n");
#else
#ifdef MTK_SRCLKEN_RC_SUPPORT
	unsigned int bypass = ((1 << id & NO_DCXO_XO_CHN) ? 1 : 0) << BYPASS_CMD_EN_SHFT;
	unsigned int settle_time = 0;
	unsigned int eco_setting = 0;
	unsigned int ctrl_setting = 0;

	if (bypass) {
		if (id == CHN_RESERVE)
			#if MTK_SRCLKEN_RC_FULL_SET
			eco_setting = 1 <<PWRAP_SLP_TIMEOUT_CLR_SHFT |
				PWRAP_TMOUT_VAL << PWRAP_SLP_TIMEOUT_VAL_SHFT |
				0x1 << PL_M10_EN_SHFT |
				0x1 << PL_CHK_INTC_EN_SHFT |
				0x1 << PL_CHK_DEF_VAL_M10_SHFT;
			#else
			eco_setting = 1 <<PWRAP_SLP_TIMEOUT_CLR_SHFT |
				PWRAP_TMOUT_VAL << PWRAP_SLP_TIMEOUT_VAL_SHFT |
				0x1 << PL_M10_EN_SHFT |
				0x1 << PL_CHK_INTC_EN_SHFT;
			#endif
		else
			settle_time = CENTROL_CNT_STEP << CNT_PRD_STEP_SHFT;
	} else
		settle_time = DCXO_SETTLE_TIME << DCXO_STABLE_PRD_SHFT |
				XO_SETTLE_TIME << XO_STABLE_PRD_SHFT |
				CENTROL_CNT_STEP << CNT_PRD_STEP_SHFT;

	if (mode == INIT_MODE)
		ctrl_setting = SW_MODE | FPM_REQ;
	else
		ctrl_setting = rc_ctrl[id].req | mode;

	DRV_WriteReg32(RC_MXX_SRCLKEN_CFG + (0x4 * id),
			(settle_time | eco_setting) |
			0x1 << REQ_ACK_LOW_IMD_EN_SHFT |
			ctrl_setting |
			bypass);
	print("M0%d: 0x%x\n", id, DRV_Reg32(RC_MXX_SRCLKEN_CFG + (0x4 * id)));
#endif
#endif
}

void rc_ctrl_mode_switch_hw(enum chn_id id)
{
	__rc_ctrl_mode_switch(id, HW_MODE);
}

void rc_ctrl_mode_switch_sw(enum chn_id id)
{
	__rc_ctrl_mode_switch(id, SW_MODE);
}

void rc_ctrl_mode_switch_init(enum chn_id id)
{
	__rc_ctrl_mode_switch(id, INIT_MODE);
}

int srclken_rc_init(void)
{
	int ret = 0;

#if MTK_SRCLKEN_RC_BRINGUP
	print("Bring-UP : skip srclken_rc init\n");
#else
#ifndef MTK_SRCLKEN_RC_SUPPORT
	/* old co-clock architecture, disable ECO setting */
	DRV_WriteReg32(RC_M12_SRCLKEN_CFG,
			0x1 << PWRAP_SLP_TIMEOUT_VAL_SHFT |
			0x1 << PL_CHK_INTC_EN_SHFT |
			0x1 << SW_SRCLKEN_RC_SHFT);

	print("Disable srclken_rc with ECO - M12 : 0x%x\n", DRV_Reg32(RC_M12_SRCLKEN_CFG));
#else
	/* new co-clock architecture srclkenrc implement here */
	unsigned int chn_n;

	/* Enable ulposc1 first */
	ret = ulposc_init();

	/* Set SW RESET 1 */
	DRV_WriteReg32(SRCLKEN_RC_CFG, 1 << SW_RESET_SHFT);

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW CG 1 */
	DRV_SetReg32(SRCLKEN_RC_CFG,
		(1 << CG_32K_EN_SHFT) | (1 << CG_FCLK_EN_SHFT));

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set req_filter m00~m12 as default SW_FPM */
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++)
		rc_ctrl_mode_switch_init(chn_n);

	/* Set PMIC addr for SPI CMD */
	DRV_WriteReg32(RC_PMIC_RCEN_ADDR, PMRC_CON0);

	DRV_WriteReg32(RC_PMIC_RCEN_SET_CLR_ADDR,
			(PMRC_CON0_CLR << 16 | PMRC_CON0_SET));

	DRV_WriteReg32(RC_CMD_ARB_CFG, 0);

	/* CFG1 setting for spi cmd config */
	DRV_WriteReg32(RC_CENTRAL_CFG1,
		DCXO_SETTLE_TIME << DCXO_SETTLE_T_SHFT |
		XO_SETTLE_TIME << NON_DCXO_SETTLE_T_SHFT |
		ULPOSC_SETTLE_TIME << ULPOSC_SETTLE_T_SHFT |
		VCORE_SETTLE_TIME << VCORE_SETTLE_T_SHFT |
		IS_SPI2PMIC_SET_CLR << RCEN_ISSUE_M_SHFT |
		RC_CENTRAL_DISABLE << SRCLKEN_RC_EN_SHFT);

	/* CFG2 setting for signal mode of each control mux */
	DRV_WriteReg32(RC_CENTRAL_CFG2,
		SPI_CLK_SRC << PWRAP_SLP_MUX_SEL_SHFT |
		PWRAP_CTRL_M << PWRAP_SLP_CTRL_M_SHFT |
		ULPOSC_CTRL_M << ULPOSC_CTRL_M_SHFT |
		IS_SPI_DONE_RELEASE << SRCVOLTEN_VREQ_M_SHFT |
		SPI_TRIG_MODE << SRCVOLTEN_VREQ_SEL_SHFT |
		VREQ_CTRL_M << VREQ_CTRL_SHFT |
		SRCLKENO_0_CTRL_M << SRCVOLTEN_CTRL_SHFT);

	DRV_WriteReg32(RC_CENTRAL_CFG3,
		1 << TO_LPM_SETTLE_T_SHFT |
		0x9F << PL_CHK_EN_B_SHFT |
		1 << BCK_SPM_ARB_CMD_SHFT |
		1 << TO_LPM_SETTLE_EN_SHFT);
	/* Set srclkeno_0/conn_bt as factor to allow dcxo change to FPM */
	DRV_WriteReg32(RC_DCXO_FPM_CFG,
		FPM_MSK_B << SUB_SRCLKEN_FPM_MSK_B_SHFT |
		MD0_SRCLKENO_0_MASK_B << SRCVOLTEN_FPM_MSK_B_SHFT |
		DCXO_FPM_CTRL_MODE << DCXO_FPM_CTRL_M_SHFT);

	/* Set bblpm/fpm channel */
	DRV_WriteReg32(SUBSYS_INTF_CFG,
		SUB_BBLPM_SET << SRCLKEN_BBLPM_MASK_B_SHFT|
		SUB_FPM_SET << SRCLKEN_FPM_MASK_B_SHFT);

	/* Trigger srclken_rc enable */
	DRV_SetReg32(RC_CENTRAL_CFG1,
		RC_CENTRAL_ENABLE << SRCLKEN_RC_EN_SHFT);

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW RESET 0 */
	DRV_ClrReg32(SRCLKEN_RC_CFG, 1 << SW_RESET_SHFT);

	/* Wait 100us */
	gpt_busy_wait_us(100);

	/* Set SW CG 1 */
	DRV_ClrReg32(SRCLKEN_RC_CFG,
		(1 << CG_32K_EN_SHFT) | (1 << CG_FCLK_EN_SHFT));

	/* Wait 500us */
	gpt_busy_wait_us(500);

	/* Set req_filter m00~m12 */
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++) {
		rc_ctrl_mode_switch_sw(chn_n);
	}

	/* Polling ACK of Initial Subsys Input */
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++) {
		unsigned int chk_sta;
		int i = 0;

		if (rc_ctrl[chn_n].mode >> SW_SRCLKEN_RC_SHFT) {
			chk_sta = ((rc_ctrl[chn_n].req >>SW_SRCLKEN_FPM_SHFT) & 0x1) << 1 |
				((rc_ctrl[chn_n].req >>SW_SRCLKEN_BBLPM_SHFT) & 0x1) << 3;
		} else
			chk_sta = 0;

		while((DRV_Reg32(RC_MXX_REQ_STA_0 + chn_n * 4) & 0xa) != chk_sta) {
			udelay(10);
			i++;
			if(i > 100) {
				print("%s: polling M%d status fail.(0x%x)\n", __func__, chn_n,
					DRV_Reg32(RC_MXX_REQ_STA_0 + chn_n * 4));
				ret = -1;
				break;
			}
		}
	}
#if MTK_SRCLKEN_RC_FULL_SET
	DRV_SetReg32(SPM_POWER_ON_VAL1, 0x1 << 6);
	DRV_SetReg32(SPM_SPARE_CON_SET, 0x1 << 1);

	/* Set req_filter m00~m12 */
	for (chn_n = 0; chn_n < MAX_CHN_NUM; chn_n++)
		rc_ctrl_mode_switch_hw(chn_n);

#elif MTK_SRCLKEN_RC_VFE28_ONLY
	rc_ctrl_mode_switch_hw(CHN_COANT);
#endif
#ifdef SRCLKEN_DBG
	print("SRCLKEN_RC_CFG:0x%x\n", DRV_Reg32(SRCLKEN_RC_CFG));
	print("RC_CENTRAL_CFG1:0x%x\n", DRV_Reg32(RC_CENTRAL_CFG1));
	print("RC_CENTRAL_CFG2:0x%x\n", DRV_Reg32(RC_CENTRAL_CFG2));
	print("RC_CENTRAL_CFG3:0x%x\n", DRV_Reg32(RC_CENTRAL_CFG3));
	print("RC_DCXO_FPM_CFG:0x%x\n", DRV_Reg32(RC_DCXO_FPM_CFG));
	print("SUBSYS_INTF_CFG = 0x%x\n", DRV_Reg32(SUBSYS_INTF_CFG));
	print("RC_SPI_STA_0:0x%x\n", DRV_Reg32(RC_SPI_STA_0));
#endif
#endif
#endif
	return ret;
}

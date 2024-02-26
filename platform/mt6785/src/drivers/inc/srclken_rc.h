#ifndef _SRCLKEN_RC_H_
#define _SRCLKEN_RC_H_

/***********************************************************************************
** Definitions
************************************************************************************/

#define MTK_SRCLKEN_RC_BRINGUP		(1)
#ifdef MTK_SRCLKEN_RC_SUPPORT
#define MTK_SRCLKEN_RC_BT_ONLY		(1)
#define MTK_SRCLKEN_RC_FULL_SET		(0)
#define MTK_SRCLKEN_RC_VFE28_ONLY	(0)
#define SRCLKEN_DBG			(1)
#else
#define MTK_SRCLKEN_RC_BT_ONLY		(0)
#define MTK_SRCLKEN_RC_FULL_SET		(0)
#define MTK_SRCLKEN_RC_VFE28_ONLY	(0)
#define SRCLKEN_DBG			(0)
#endif

#define AP_BASE				(0x1000C000)
#define TOPCK_BASE			(0x10000000)
#define SPM_BASE			(0x10006000)
#define RC_BASE				(0x10006500)

/* SPM Register */
#define ULPOSC_CON			(SPM_BASE + 0x0440)
    #define ULPOSC_EN_SHFT		0
    #define ULPOSC_RST_SHFT		1
    #define ULPOSC_CG_EN_SHFT		2
    #define ULPOSC_CLK_SEL_SHFT		3

/* APMIXEDSYS Register */
/* PLL_ULPOSC_CON0 */
#define PLL_ULPOSC_CON0         	(AP_BASE + 0x0700)
    #define RG_OSC_CALI_MSK		0x3f
    #define RG_OSC_CALI_SHFT		0
    #define RG_OSC_IBAND_MSK		0x7f
    #define RG_OSC_IBAND_SHFT		6
    #define RG_OSC_FBAND_MSK		0xf
    #define RG_OSC_FBAND_SHFT		13
    #define RG_OSC_DIV_MSK		0x3f
    #define RG_OSC_DIV_SHFT		17
    #define RG_OSC_CP_EN_MSK		0x1
    #define RG_OSC_CP_EN_SHFT		23
    #define RG_OSC_RSV_MSK		0xff
    #define RG_OSC_RSV_SHFT		24

/* PLL_ULPOSC_CON1 */
#define PLL_ULPOSC_CON1			(AP_BASE + 0x0704)
    #define RG_OSC_MOD_MSK		0x3
    #define RG_OSC_MOD_SHFT		0
    #define RG_OSC_DIV2_EN_MSK		0x1
    #define RG_OSC_DIV2_EN_SHFT		2

/* FREQUENCY METER CONTROL */
#define CLK_MISC_CFG_0			(TOPCK_BASE + 0x0104)
#define CLK_DBG_CFG			(TOPCK_BASE + 0x010C)
#define CLK_SCP_CFG_0			(TOPCK_BASE + 0x0220)
#define CLK_SCP_CFG_1			(TOPCK_BASE + 0x0224)

#define SRCLKEN_RC_CFG			(RC_BASE + 0x0000)
    #define SW_RESET_MSK		0x1
    #define SW_RESET_SHFT		0
    #define CG_32K_EN_MSK		0x1
    #define CG_32K_EN_SHFT		1
    #define CG_FCLK_EN_MSK		0x1
    #define CG_FCLK_EN_SHFT		2

#define RC_CENTRAL_CFG1			(RC_BASE + 0x0004)
    #define SRCLKEN_RC_EN_MSK		0x1
    #define SRCLKEN_RC_EN_SHFT		0
    #define RCEN_ISSUE_M_MSK		0x1
    #define RCEN_ISSUE_M_SHFT		1
    #define RC_SPI_ACTIVE_MSK		0x1
    #define RC_SPI_ACTIVE_SHFT		2
    #define VCORE_SETTLE_T_MSK		0x7
    #define VCORE_SETTLE_T_SHFT		5
    #define ULPOSC_SETTLE_T_MSK		0xf
    #define ULPOSC_SETTLE_T_SHFT	8
    #define NON_DCXO_SETTLE_T_MSK	0x3ff
    #define NON_DCXO_SETTLE_T_SHFT	12
    #define DCXO_SETTLE_T_MSK		0x3ff
    #define DCXO_SETTLE_T_SHFT		22

#define RC_CENTRAL_CFG2			(RC_BASE + 0x0008)
    #define SRCVOLTEN_CTRL_MSK		0xf
    #define SRCVOLTEN_CTRL_SHFT		0
    #define VREQ_CTRL_MSK		0xf
    #define VREQ_CTRL_SHFT		4
    #define SRCVOLTEN_VREQ_SEL_MSK	0x1
    #define SRCVOLTEN_VREQ_SEL_SHFT	8
    #define SRCVOLTEN_VREQ_M_MSK	0x1
    #define SRCVOLTEN_VREQ_M_SHFT	9
    #define FORCE_SRCVOLTEN_OFF_MSK	0x1
    #define FORCE_SRCVOLTEN_OFF_SHFT	10
    #define FORCE_SRCVOLTEN_ON_MSK	0x1
    #define FORCE_SRCVOLTEN_ON_SHFT	11
    #define ULPOSC_CTRL_M_MSK		0xf
    #define ULPOSC_CTRL_M_SHFT		12
    #define FORCE_VCORE_RDY_MSK		0x1
    #define FORCE_VCORE_RDY_SHFT	16
    #define FORCE_ULPOSC2ON_MSK		0x1
    #define FORCE_ULPOSC2ON_SHFT	17
    #define FORCE_ULPOSC_CLK_EN_MSK	0x1
    #define FORCE_ULPOSC_CLK_EN_SHFT	18
    #define FORCE_ULPOSC_ON_MSK		0x1
    #define FORCE_ULPOSC_ON_SHFT	19
    #define DIS_ULPOSC_RDY_CHK_MSK	0x1
    #define DIS_ULPOSC_RDY_CHK_SHFT	20
    #define PWRAP_SLP_CTRL_M_MSK	0xf
    #define PWRAP_SLP_CTRL_M_SHFT	21
    #define PWRAP_SLP_MUX_SEL_MSK	0x1
    #define PWRAP_SLP_MUX_SEL_SHFT	25
    #define FORCE_PWRAP_ON_MSK		0x1
    #define FORCE_PWRAP_ON_SHFT		26
    #define FORCE_PWRAP_AWK_MSK  	0x1
    #define FORCE_PWRAP_AWK_SHFT	27
    #define NON_DCXO_REQ_FORCEON_MSK	0x1
    #define NON_DCXO_REQ_FORCEON_SHFT	28
    #define NON_DCXO_REQ_FORCEOFF_MSK	0x1
    #define NON_DCXO_REQ_FORCEOFF_SHFT	29
    #define DCXO_REQ_FORCEON_MSK	0x1
    #define DCXO_REQ_FORCEON_SHFT	30
    #define DCXO_REQ_FORCEOFF_MSK	0x1
    #define DCXO_REQ_FORCEOFF_SHFT	31
    
#define RC_CMD_ARB_CFG			(RC_BASE + 0x000C)
    #define SW_RC_EN_MSK		0x1fff
    #define SW_RC_EN_SHFT		0
    #define SW_RCEN_EN_MSK		0x1fff
    #define SW_RCEN_EN_SHFT		13
    #define SW_DCXO_M_EN_MSK		0x1
    #define SW_DCXO_M_EN_SHFT		28
    #define SW_DCXO_M_MSK		0x7
    #define SW_DCXO_M_SHFT		29

#define RC_PMIC_RCEN_ADDR		(RC_BASE + 0x0010)
#define RC_PMIC_RCEN_SET_CLR_ADDR	(RC_BASE + 0x0014)
#define RC_DCXO_FPM_CFG			(RC_BASE + 0x0018)
    #define DCXO_FPM_CTRL_M_MSK		0xf
    #define DCXO_FPM_CTRL_M_SHFT	0
    #define SRCVOLTEN_FPM_MSK_B_MSK	0x1
    #define SRCVOLTEN_FPM_MSK_B_SHFT	4
    #define SUB_SRCLKEN_FPM_MSK_B_MSK	0x1fff
    #define SUB_SRCLKEN_FPM_MSK_B_SHFT	16

#define RC_CENTRAL_CFG3			(RC_BASE + 0x001C)
    #define TO_LPM_SETTLE_EN_MSK	0x1
    #define TO_LPM_SETTLE_EN_SHFT	0
    #define BCK_SPM_ARB_CMD_MSK	0x1
    #define BCK_SPM_ARB_CMD_SHFT	1
    #define PL_CHK_EN_B_SHFT	4
    #define TO_LPM_SETTLE_T_MSK		0x2ff
    #define TO_LPM_SETTLE_T_SHFT	12

#define RC_MXX_SRCLKEN_CFG		(RC_BASE + 0x0020)
    #define SRCLKEN_BLK_EN_MSK		0x1
    #define SRCLKEN_BLK_EN_SHFT		0
    #define DCXO_SETTLE_BLK_EN_MSK	0x1
    #define DCXO_SETTLE_BLK_EN_SHFT	1
    #define BYPASS_CMD_EN_MSK		0x1
    #define BYPASS_CMD_EN_SHFT		2
    #define SW_SRCLKEN_RC_MSK		0x1
    #define SW_SRCLKEN_RC_SHFT		3
    #define SW_SRCLKEN_FPM_MSK		0x1
    #define SW_SRCLKEN_FPM_SHFT		4
    #define SW_SRCLKEN_BBLPM_MSK		0x1
    #define SW_SRCLKEN_BBLPM_SHFT		5
    #define XO_SOC_LINK_EN_MSK		0x1
    #define XO_SOC_LINK_EN_SHFT		6
    #define REQ_ACK_LOW_IMD_EN_MSK	0x1
    #define REQ_ACK_LOW_IMD_EN_SHFT	7
    #define SRCLKEN_TRACK_M_EN_MSK	0x1
    #define SRCLKEN_TRACK_M_EN_SHFT	8
    #define CNT_PRD_STEP_MSK		0x3
    #define CNT_PRD_STEP_SHFT		10
    #define XO_STABLE_PRD_MSK		0x3ff
    #define XO_STABLE_PRD_SHFT		12
    #define DCXO_STABLE_PRD_MSK		0x3ff
    #define DCXO_STABLE_PRD_SHFT	22

#define RC_M12_SRCLKEN_CFG		(RC_BASE + 0x0050)
    #define PL_M10_EN_SHFT		10
    #define PL_CHK_INTC_EN_SHFT		11
    #define PL_CHK_DEF_VAL_M04_SHFT	14
    #define PL_CHK_DEF_VAL_M05_SHFT	15
    #define PL_CHK_DEF_VAL_M06_SHFT	16
    #define PL_CHK_DEF_VAL_M07_SHFT	17
    #define PL_CHK_DEF_VAL_M08_SHFT	18
    #define PL_CHK_DEF_VAL_M10_SHFT	20
    #define PL_CHK_DEF_VAL_M11_SHFT	21
    #define PWRAP_SLP_TIMEOUT_VAL_SHFT	26
    #define PWRAP_SLP_TIMEOUT_CLR_SHFT	30
    #define PWRAP_SLP_TIMEOUT_EN_SHFT	31

#define RC_MXX_REQ_STA_0		(RC_BASE + 0x0080)

#define SUBSYS_INTF_CFG			(RC_BASE + 0x00BC)
    #define SRCLKEN_FPM_MASK_B_MSK	0x1fff
    #define SRCLKEN_FPM_MASK_B_SHFT	0
    #define SRCLKEN_BBLPM_MASK_B_MSK	0x1fff
    #define SRCLKEN_BBLPM_MASK_B_SHFT	16

#define RC_SPI_STA_0			(RC_BASE + 0x006C)

#define TEST_UFS				(UFS_HCI_BASE + 0x0144)

typedef struct {
    u32    osc_div;
    u32    cali_val;
} ulposc_con_t;

enum {
    ULPOSC_DIV_1 = 0,
    ULPOSC_DIV_4 = 1
};

enum chn_id {
	CHN_SUSPEND = 0,
	CHN_RF = 1,
	CHN_DEEPIDLE = 2,
	CHN_MD= 3,
	CHN_GPS = 4,
	CHN_BT = 5,
	CHN_WIFI = 6,
	CHN_MCU = 7,
	CHN_COANT = 8,
	CHN_NFC = 9,
	CHN_UFS = 10,
	CHN_SCP = 11,
	CHN_RESERVE = 12,
	MAX_CHN_NUM,
};

enum {
	SRCLKENAO_MODE,
	VREQ_MODE,
};

enum {
	MERGE_OR_MODE		= 0x0,
	BYPASS_MODE			= 0x1,
	MERGE_AND_MODE 		= 0x1 << 1,
	BYPASS_RC_MODE 		= 0x2 << 1,
	BYPASS_OTHER_MODE 	= 0x3 << 1,
	ASYNC_MODE		= 0x1 << 3,
};

enum {
	RC_32K = 0,
	RC_ULPOSC1,
};

enum rc_ctrl_m {
	HW_MODE = 0 << SW_SRCLKEN_RC_SHFT,
	SW_MODE = 1 << SW_SRCLKEN_RC_SHFT,
	INIT_MODE = 0xff,
};

enum rc_ctrl_r {
	NO_REQ = 0,
	FPM_REQ = 1 << SW_SRCLKEN_FPM_SHFT,
	BBLPM_REQ = 1 << SW_SRCLKEN_BBLPM_SHFT,
};

struct subsys_rc_ctrl {
	enum chn_id id;
	enum rc_ctrl_m mode;
	enum rc_ctrl_r req;
};

struct rc_subsys_con {
	unsigned int dcxo_prd;
	unsigned int xo_prd;
	unsigned int cnt_step;
	bool track_en;
	bool req_ack_imd_en;
	bool xo_soc_link_en;
	bool sw_bblpm;
	bool sw_fpm;
	bool sw_rc;
	bool bypass_settle;
};

#define RC_SUBSYS_SET(_dcxo_prd, _xo_prd, _cnt_step, _track_en, _req_ack_imd_en,	\
		_xo_soc_link_en, _sw_bblpm, _sw_fpm, _sw_rc, _bypass_settle) {	\
			.dcxo_prd = _dcxo_prd,		\
			.xo_prd = _xo_prd,		\
			.cnt_step = _cnt_step,		\
			.track_en = _track_en,		\
			.req_ack_imd_en = _req_ack_imd_en,	\
			.xo_soc_link_en = _xo_soc_link_en,	\
			.sw_bblpm = _sw_bblpm,		\
			.sw_fpm = _sw_fpm,		\
			.sw_rc = _sw_rc,		\
			.bypass_settle = _bypass_settle,	\
		}

extern void rc_ctrl_mode_switch_hw(enum chn_id id);
extern void rc_ctrl_mode_switch_sw(enum chn_id id);
extern int srclken_rc_init(void);

#endif	/* _SRCLKEN_RC_H_ */

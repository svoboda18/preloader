#ifndef _PL_MT_PMIC_6356_H_
#define _PL_MT_PMIC_6356_H_

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct pmu_init_s
{
	unsigned short addr;
	unsigned short val;
	unsigned short mask;
	unsigned char shift;
	unsigned char con;
};

struct pmic_setting {
	unsigned short addr;
	unsigned short val;
	unsigned short mask;
	unsigned char shift;
};

typedef struct pmuflag_t {
	unsigned int addr;
	unsigned int mask;
	unsigned int shift;
} pmuflag;

struct mtk_auxadc_t {
	pmuflag rdy;
	pmuflag out;
	pmuflag rqst;
	unsigned char rqst_shift;
	unsigned char resolution;
	unsigned char r_val;
};

#define PMIC_AUXADC_GEN1(_rdy, _out, _rqst, _rqst_shift, _rel, _r_val)	   \
	{	\
		.rdy = {		\
			.addr = (_rdy),	\
			.mask = 0x1,	\
			.shift = 15,	\
		},			\
		.out = {		\
			.addr = (_out),	\
			.mask = 0x7FFF,	\
			.shift = 0,	\
		},			\
		.rqst = {		\
			.addr = (_rqst),\
			.mask = 0x1,	\
			.shift = _rqst_shift,	\
		},			\
		.resolution = _rel,	\
		.r_val = _r_val,	\
	}

//==============================================================================
// The CHIP INFO
//==============================================================================
#define PMIC6356_E1_CID_CODE	0x5610
#define PMIC6356_E2_CID_CODE	0x5620
#define PMIC6356_E3_CID_CODE	0x5630
#define PMIC6356_E4_CID_CODE	0x5640
#define PMIC6356_E5_CID_CODE	0x5650
#define PMIC6356_E6_CID_CODE	0x5660


//==============================================================================
// PMIC define
//==============================================================================
typedef enum {
	CHARGER_UNKNOWN = 0,
	STANDARD_HOST,		// USB : 450mA
	CHARGING_HOST,
	NONSTANDARD_CHARGER,	// AC : 450mA~1A
	STANDARD_CHARGER,	// AC : ~1A
	APPLE_2_1A_CHARGER,	 // 2.1A apple charger
	APPLE_1_0A_CHARGER,	 // 1A apple charger
	APPLE_0_5A_CHARGER,	 // 0.5A apple charger
} CHARGER_TYPE;

//==============================================================================
// PMIC define
//==============================================================================
typedef enum {
	AUXADC_LIST_BATADC,
	AUXADC_LIST_VCDT,
	AUXADC_LIST_BATTEMP,
	AUXADC_LIST_BATID,
	AUXADC_LIST_VBIF,
	AUXADC_LIST_CHIP_TEMP,
	AUXADC_LIST_DCXO,
	AUXADC_LIST_ACCDET,
	AUXADC_LIST_TSX,
	AUXADC_LIST_HP,
	AUXADC_LIST_BOTTOM,
} AUXADC_LIST;

//==============================================================================
// PMIC BUCK/LDO control
//==============================================================================

//==============================================================================
// PMIC Register Index
//==============================================================================
//register number
#include <upmu_hw.h>


//==============================================================================
// PMIC Status Code
//==============================================================================
#define PMIC_TEST_PASS			0x0000
#define PMIC_TEST_FAIL			0xB001
#define PMIC_EXCEED_I2C_FIFO_LENGTH	0xB002
#define PMIC_CHRDET_EXIST		0xB003
#define PMIC_CHRDET_NOT_EXIST		0xB004
#define PMIC_VBAT_DROP			0xB005
#define PMIC_VBAT_NOT_DROP		0xB006
//==============================================================================
// PMIC Exported Function
//==============================================================================
extern CHARGER_TYPE mt_charger_type_detection(void);
extern U32 pmic_IsUsbCableIn (void);
extern int pmic_detect_powerkey(void);
extern int pmic_detect_homekey(void);
extern void hw_set_cc(int cc_val);
extern void pl_charging(int en_chr);
extern void pl_kick_chr_wdt(void);
extern void pl_close_pre_chr_led(void);
extern void pl_hw_ulc_det(void);
extern U32 pmic_init (void);
extern int pmic_get_auxadc_value(AUXADC_LIST list);
extern U16 is_pmic_cold_reset(void);
extern unsigned int pmic_read_efuse_nolock(int i);
extern void pmic_init_setting(void);
/* For Bianco distinguish dram type in MT6356, not common API */
extern U32 get_dram_type(void);

//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
extern int pmic_IsVbatDrop(void);
extern CHARGER_TYPE mt_charger_type_detection(void);
extern int hw_check_battery(void);
extern void pl_check_bat_protect_status(void);
extern void pmic_DetectVbatDrop (void);
extern int PMIC_POWER_HOLD(unsigned int hold);
void hw_set_cc(int cc_val);

/* PMIC CodeGen API */
extern U32 pmic_upmu_set_baton_tdet_en(U32 val);
extern U32 pmic_upmu_set_rg_baton_en(U32 val);
extern U32 pmic_upmu_get_rgs_baton_undet(void);
#endif

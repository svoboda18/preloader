#ifndef _PL_MT_PMIC_H_
#define _PL_MT_PMIC_H_

#include <mt6359.h>

struct pmic_setting {
	unsigned short addr;
	unsigned short val;
	unsigned short mask;
	unsigned char shift;
};

struct pmuflag_t {
	unsigned int addr;
	unsigned int mask;
	unsigned int shift;
};

struct pmic_auxadc_channel_new {
	u8 resolution;
	u8 r_val;
	u8 ch_num;
	struct pmuflag_t channel_rqst;
	struct pmuflag_t channel_rdy;
	struct pmuflag_t channel_out;
};

#define PMIC_AUXADC_GEN(_rel, _r_val, _ch_num, _rqst, _rdy, _out)	   \
	{	\
		.resolution = _rel,	\
		.r_val = _r_val,	\
		.ch_num = _ch_num,	\
		.channel_rqst = {			\
			.addr = _rqst##_ADDR,	\
			.mask = _rqst##_MASK,	\
			.shift = _rqst##_SHIFT,	\
		},	\
		.channel_rdy = {			\
			.addr = _rdy##_ADDR,	\
			.mask = _rdy##_MASK,	\
			.shift = _rdy##_SHIFT,	\
		},	\
		.channel_out = {			\
			.addr = _out##_ADDR,	\
			.mask = _out##_MASK,	\
			.shift = _out##_SHIFT,	\
		},	\
	}

//==============================================================================
// The CHIP INFO
//==============================================================================
#define PMIC6359_E1_CID_CODE	0x5910
#define PMIC6359_E2_CID_CODE	0x5920
#define PMIC6359_E3_CID_CODE	0x5930
#define PMIC6359_E4_CID_CODE	0x5940
#define PMIC6359_E5_CID_CODE	0x5950
#define PMIC6359_E6_CID_CODE	0x5960


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
enum {
	AUXADC_LIST_BATADC,
	AUXADC_LIST_BATTEMP,
	AUXADC_LIST_CHIP_TEMP,
	AUXADC_LIST_VCORE_TEMP,
	AUXADC_LIST_VPROC_TEMP,
	AUXADC_LIST_VGPU_TEMP,
	AUXADC_LIST_ACCDET,
	AUXADC_LIST_VDCXO,
	AUXADC_LIST_TSX_TEMP,
	AUXADC_LIST_HPOFS_CAL,
	AUXADC_LIST_DCXO_TEMP,
	AUXADC_LIST_VBIF,
	AUXADC_LIST_MAX,
	/* MT6359 not support ISENSE/VCDT/BATID */
	AUXADC_LIST_ISENSE,
	AUXADC_LIST_VCDT,
	AUXADC_LIST_BATID,
};

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
extern U32 pmic_read_interface(U32 RegNum, U32 *val, U32 MASK, U32 SHIFT);
extern U32 pmic_config_interface(U32 RegNum, U32 val, U32 MASK, U32 SHIFT);
extern CHARGER_TYPE mt_charger_type_detection(void);
extern int pmic_detect_powerkey(void);
extern int pmic_detect_homekey(void);
extern void hw_set_cc(int cc_val);
extern void pl_charging(int en_chr);
extern void pl_kick_chr_wdt(void);
extern void pl_close_pre_chr_led(void);
extern void pl_hw_ulc_det(void);
extern U32 pmic_init (void);
extern int pmic_get_auxadc_value(unsigned short channel);
extern U32 is_pmic_rtc_alarm(void);
extern U32 is_pmic_spar(void);
extern U32 is_pmic_cold_reset(void);
extern U32 is_pmic_long_press_reset(void);
extern U32 is_pwrkey_short_press(void);
extern U32 get_pmic_boot_status(void);
extern unsigned int pmic_read_efuse_nolock(int i);
extern void pmic_efuse_sw_load(void);
extern void pmic_init_setting(void);
extern void pmic_dbg_status(unsigned char option);
extern U32 get_dram_type(void);

//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
extern CHARGER_TYPE mt_charger_type_detection(void);
extern int hw_check_battery(void);
extern void pl_check_bat_protect_status(void);
extern int PMIC_POWER_HOLD(unsigned int hold);
void hw_set_cc(int cc_val);

/* PMIC CodeGen API */
extern U32 pmic_upmu_set_baton_tdet_en(U32 val);
extern U32 pmic_upmu_set_rg_baton_en(U32 val);
extern U32 pmic_upmu_get_rgs_baton_undet(void);
#endif


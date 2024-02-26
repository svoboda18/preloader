
#ifndef _PL_MT_PMIC_H_
#define _PL_MT_PMIC_H_

#if defined(PMIC_CHIP_MT6356)

#include "pmic_mt6356.h"

#elif defined(PMIC_CHIP_MT6357)

#include "pmic_mt6357.h"

#else

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
}pmuflag;

struct mtk_auxadc_t {
	pmuflag rdy;
	pmuflag out;
	pmuflag rqst;
	unsigned char rqst_shift;
	unsigned char resolution;
	unsigned char r_val;
};

#define PMIC_AUXADC_GEN1(_rdy, _out, _rqst, _rqst_shift, _rel, _r_val)	\
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

#define PMIC_BUCK_GEN1(_name, _en, _mode, _vosel, _da_qi_en, _da_ni_vosel, min, max, step, _stb)	\
	{	\
		.name = #_name,		\
		.min_uV = (min),	\
		.max_uV = (max),	\
		.uV_step = (step),	\
		.en = {			\
			.addr = (_en),	\
			.mask = 0x1,	\
			.shift = 0,	\
		},			\
		.mode = {		\
			.addr = (_mode),\
			.mask = 0x1,	\
			.shift = 1,	\
		},			\
		.vosel = {		\
			.addr = (_vosel),	\
			.mask = 0x7F,	\
			.shift = 0,	\
		},			\
		.da_qi_en = {		\
			.addr = (_da_qi_en),	\
			.mask = 0x1,	\
			.shift = 0	\
		},			\
		.da_ni_vosel = {	\
			.addr = (_da_ni_vosel),	\
			.mask = 0x7F,	\
		},			\
		.stb = (_stb),		\
	}

struct mtk_bucks_t {
	const char *name;
	unsigned int min_uV;
	unsigned int max_uV;
	unsigned int uV_step;
	unsigned int stb;
	pmuflag en;
	pmuflag mode;
	pmuflag vosel;
	pmuflag da_qi_en;
	pmuflag da_ni_vosel;
};

//==============================================================================
// The CHIP INFO
//==============================================================================
#define PMIC6335_E1_CID_CODE	0x3510
#define PMIC6335_E2_CID_CODE	0x3520
#define PMIC6335_E3_CID_CODE	0x3530
#define PMIC6335_E4_CID_CODE	0x3540
#define PMIC6335_E5_CID_CODE	0x3550
#define PMIC6335_E6_CID_CODE	0x3560


//==============================================================================
// PMIC define
//==============================================================================
typedef enum {
	CHARGER_UNKNOWN = 0,
	STANDARD_HOST,			// USB : 450mA
	CHARGING_HOST,
	NONSTANDARD_CHARGER,	// AC : 450mA~1A
	STANDARD_CHARGER,		// AC : ~1A
	APPLE_2_1A_CHARGER,		// 2.1A apple charger
	APPLE_1_0A_CHARGER,		// 1A apple charger
	APPLE_0_5A_CHARGER,		// 0.5A apple charger
} CHARGER_TYPE;

//==============================================================================
// PMIC define
//==============================================================================
typedef enum {
	VCORE,
	VDRAM,
	VMODEM,
	VMD1,
	VS1,
	VS2,
	VPA1,
	VIMVO,
	VSRAM_DVFS1,
	VSRAM_DVFS2,
	VSRAM_VGPU,
	VSRAM_VCORE,
	VSRAM_VMD,
} BUCK_TYPE;

typedef enum {
	PMIC_VOLT_00_700000_V,
	PMIC_VOLT_00_725000_V,
	PMIC_VOLT_00_750000_V,
	PMIC_VOLT_00_775000_V,
	PMIC_VOLT_00_800000_V,
	PMIC_VOLT_00_825000_V,
	PMIC_VOLT_00_850000_V,
	PMIC_VOLT_00_875000_V,
	PMIC_VOLT_00_900000_V,
	PMIC_VOLT_00_925000_V,
	PMIC_VOLT_00_950000_V,
	PMIC_VOLT_00_975000_V,
	PMIC_VOLT_01_000000_V,
	PMIC_VOLT_01_025000_V,
	PMIC_VOLT_01_050000_V,
	PMIC_VOLT_01_075000_V,
	PMIC_VOLT_01_100000_V,
	PMIC_VOLT_01_125000_V,
	PMIC_VOLT_01_150000_V,
	PMIC_VOLT_01_175000_V,
	PMIC_VOLT_01_200000_V,
	PMIC_VOLT_01_225000_V,
	PMIC_VOLT_01_250000_V,
	PMIC_VOLT_01_275000_V,
	PMIC_VOLT_01_300000_V,
	PMIC_VOLT_01_325000_V,
	PMIC_VOLT_01_350000_V,
	PMIC_VOLT_01_375000_V,
	PMIC_VOLT_01_400000_V,
} BUCK_VOLTAGE_UV;

static const int pmic_buck_voltages[] = {
	700000,
	725000,
	750000,
	775000,
	800000,
	825000,
	850000,
	875000,
	900000,
	925000,
	950000,
	975000,
	1000000,
	1025000,
	1050000,
	1075000,
	1100000,
	1125000,
	1150000,
	1175000,
	1200000,
	1225000,
	1250000,
	1275000,
	1300000,
	1325000,
	1350000,
	1375000,
	1400000,
};

typedef enum {
	AUXADC_LIST_BATADC,
	AUXADC_LIST_VCDT,
	AUXADC_LIST_BATTEMP,
	AUXADC_LIST_BATID,
	AUXADC_LIST_VBIF,
	AUXADC_LIST_CHIP_TEMP,
	AUXADC_LIST_DCXO,
	AUXADC_LIST_TSX,
} AUXADC_LIST;

//==============================================================================
// PMIC Register Index
//==============================================================================
//register number
#include <upmu_hw.h>


//==============================================================================
// PMIC Status Code
//==============================================================================
#define PMIC_TEST_PASS				 0x0000
#define PMIC_TEST_FAIL				 0xB001
#define PMIC_EXCEED_I2C_FIFO_LENGTH	 0xB002
#define PMIC_CHRDET_EXIST			 0xB003
#define PMIC_CHRDET_NOT_EXIST		 0xB004
#define PMIC_VBAT_DROP		0xB005
#define PMIC_VBAT_NOT_DROP	0xB006
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

//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
extern int pmic_IsVbatDrop(void);
extern CHARGER_TYPE mt_charger_type_detection(void);
extern int hw_check_battery(void);
extern void pl_check_bat_protect_status(void);
extern void pmic_DetectVbatDrop (void);
extern int buck_is_enabled(BUCK_TYPE type);
extern int buck_enable(BUCK_TYPE type, unsigned char en);
extern int buck_set_mode(BUCK_TYPE type, unsigned char pmode);
extern int buck_set_voltage(BUCK_TYPE type, unsigned int voltage);
extern unsigned int buck_get_voltage(BUCK_TYPE type);
extern void PMIC_POWER_HOLD(unsigned int hold);
void hw_set_cc(int cc_val);

/* PMIC CodeGen API */
extern U32 pmic_upmu_set_baton_tdet_en(U32 val);
extern U32 pmic_upmu_set_rg_baton_en(U32 val);
extern U32 pmic_upmu_get_rgs_baton_undet(void);
#endif

#endif

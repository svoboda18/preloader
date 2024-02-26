#ifndef _PL_MT_PMIC_6355_H_
#define _PL_MT_PMIC_6355_H_

#define BUCK_ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

struct pmu_init_s
{
	unsigned short addr;
	unsigned short val;
	unsigned short mask;
	unsigned char shift;
	unsigned char con;
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

#define PMIC_AUXADC_GEN1(_rdy, _out, _rqst, _rqst_shift, _rel, _r_val)       \
        {       \
                .rdy = {   \
                        .addr = (_rdy),        \
                        .mask = 0x1,        \
                        .shift = 15,      \
                },      \
                .out = {        \
                        .addr = (_out),     \
                        .mask = 0x7FFF,     \
                        .shift = 0,   \
                },      \
		.rqst = {	\
			.addr = (_rqst),	\
			.mask = 0x1,	\
			.shift = _rqst_shift,	\
		},	\
		.resolution = _rel,	\
		.r_val = _r_val,	\
        }

//==============================================================================
// The CHIP INFO
//==============================================================================
#define PMIC6355_E1_CID_CODE    0x5510
#define PMIC6355_E2_CID_CODE    0x5520
#define PMIC6355_E3_CID_CODE    0x5530
#define PMIC6355_E4_CID_CODE    0x5540
#define PMIC6355_E5_CID_CODE    0x5550
#define PMIC6355_E6_CID_CODE    0x5560


//==============================================================================
// PMIC define
//==============================================================================
typedef enum {
    CHARGER_UNKNOWN = 0,
    STANDARD_HOST,          // USB : 450mA
    CHARGING_HOST,
    NONSTANDARD_CHARGER,    // AC : 450mA~1A
    STANDARD_CHARGER,       // AC : ~1A
    APPLE_2_1A_CHARGER,     // 2.1A apple charger
    APPLE_1_0A_CHARGER,     // 1A apple charger
    APPLE_0_5A_CHARGER,     // 0.5A apple charger
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
} AUXADC_LIST;

//==============================================================================
// PMIC BUCK/LDO control
//==============================================================================
#if 0
#define vdram2_vol_reg  (0x10C8)
#define vdram2_vol_mask (0x7F)
#define vdram2_vol_shift (0)
#define vdram2_enable_reg (0x10C6)
#define vdram2_enable_bit (0)
#define vdram2_mode_reg (0x1274)
#define vdram2_mode_bit (8)
#define vdram2_min_uV 400000
#define vdram2_max_uV 1193750
#define vdram2_uV_step 6250
#define vdram2_stb 1000

#define vs1_vol_reg  (0x1114)
#define vs1_vol_mask (0x7F)
#define vs1_vol_shift (0)
#define vs1_enable_reg (0x1112)
#define vs1_enable_bit (0)
#define vs1_mode_reg (0x129E)
#define vs1_mode_bit (8)
#define vs1_min_uV 1200000
#define vs1_max_uV 2787500
#define vs1_uV_step 12500
#define vs1_stb 1000

#define vdram1_vol_reg (0x10A6)
#define vdram1_vol_mask (0x7F)
#define vdram1_vol_shift (0)
#define vdram1_enable_reg (0x10A4)
#define vdram1_enable_bit (0)
#define vdram1_mode_reg (0x1264)
#define vdram1_mode_bit (8)
#define vdram1_min_uV 518750
#define vdram1_max_uV 1312500
#define vdram1_uV_step 6250
#define vdram1_stb 1000

#define vmodem_vol_reg (0x10EA)
#define vmodem_vol_mask (0x7F)
#define vmodem_vol_shift (0)
#define vmodem_enable_reg (0x10E8)
#define vmodem_enable_bit (0)
#define vmodem_mode_reg (0x1284)
#define vmodem_mode_bit (1)
#define vmodem_min_uV 400000
#define vmodem_max_uV 1193750
#define vmodem_uV_step 6250
#define vmodem_stb 1000

#define vcore_vol_reg (0x1060)
#define vcore_vol_mask (0x7F)
#define vcore_vol_shift (0)
#define vcore_enable_reg (0x105E)
#define vcore_enable_bit (0)
#define vcore_mode_reg (0x1242)
#define vcore_mode_bit (1)
#define vcore_min_uV 406250
#define vcore_max_uV 1200000
#define vcore_uV_step 6250
#define vcore_stb 1000

#define vs2_vol_reg (0x113E)
#define vs2_vol_mask (0x7F)
#define vs2_vol_shift (0)
#define vs2_enable_reg (0x113C)
#define vs2_enable_bit (0)
#define vs2_mode_reg (0x12AC)
#define vs2_mode_bit (1)
#define vs2_min_uV 1200000
#define vs2_max_uV 2787500
#define vs2_uV_step 12500
#define vs2_stb 1000

#define vpa_vol_reg (0x1168)
#define vpa_vol_mask (0x3F)
#define vpa_vol_shift (0)
#define vpa_enable_reg (0x1166)
#define vpa_enable_bit (0)
#define vpa_mode_reg (0x12B4)
#define vpa_mode_bit (3)
#define vpa_min_uV 500000
#define vpa_max_uV 36500000
#define vpa_uV_step 50000
#define vpa_stb 1000

#define vproc12_vol_reg (0x103E)
#define vproc12_vol_mask (0x7F)
#define vproc12_vol_shift (0)
#define vproc12_enable_reg (0x103C)
#define vproc12_enable_bit (0)
#define vproc12_mode_reg (0x1220)
#define vproc12_mode_bit (2)
#define vproc12_min_uV 406250
#define vproc12_max_uV 1200000
#define vproc12_uV_step 6250
#define vproc12_stb 1000

#define vgpu_vol_reg (0x1084)
#define vgpu_vol_mask (0x7F)
#define vgpu_vol_shift (0)
#define vgpu_enable_reg (0x1082)
#define vgpu_enable_bit (0)
#define vgpu_mode_reg (0x1242)
#define vgpu_mode_bit (2)
#define vgpu_min_uV 406250
#define vgpu_max_uV 1200000
#define vgpu_uV_step 6250
#define vgpu_stb 1000

#define vproc11_vol_reg (0x101C)
#define vproc11_vol_mask (0x7F)
#define vproc11_vol_shift (0)
#define vproc11_enable_reg (0x101A)
#define vproc11_enable_bit (0)
#define vproc11_mode_reg (0x1220)
#define vproc11_mode_bit (1)
#define vproc11_min_uV 406250
#define vproc11_max_uV 1200000
#define vproc11_uV_step 6250
#define vproc11_stb 1000

#define vsram_gpu_vol_reg (0x18EA)
#define vsram_gpu_vol_mask (0x7F)
#define vsram_gpu_vol_shift (0)
#define vsram_gpu_enable_reg (0x18E8)
#define vsram_gpu_enable_bit (0)
#define vsram_gpu_mode_reg (0)
#define vsram_gpu_mode_bit (0)
#define vsram_gpu_min_uV 518750
#define vsram_gpu_max_uV 1312500
#define vsram_gpu_uV_step 6250
#define vsram_gpu_stb 110

#define vsram_core_vol_reg (0x18CA)
#define vsram_core_vol_mask (0x7F)
#define vsram_core_vol_shift (0)
#define vsram_core_enable_reg (0x18C8)
#define vsram_core_enable_bit (0)
#define vsram_core_mode_reg (0)
#define vsram_core_mode_bit (0)
#define vsram_core_min_uV 518750
#define vsram_core_max_uV 1312500
#define vsram_core_uV_step 6250
#define vsram_core_stb 110

#define vsram_proc_vol_reg (0x18AA)
#define vsram_proc_vol_mask (0x7F)
#define vsram_proc_vol_shift (0)
#define vsram_proc_enable_reg (0x18A8)
#define vsram_proc_enable_bit (0)
#define vsram_proc_mode_reg (0)
#define vsram_proc_mode_bit (0)
#define vsram_proc_min_uV 518750
#define vsram_proc_max_uV 1312500
#define vsram_proc_uV_step 6250
#define vsram_proc_stb 110

#define vsram_md_vol_reg (0x190A)
#define vsram_md_vol_mask (0x7F)
#define vsram_md_vol_shift (0)
#define vsram_md_enable_reg (0x1908)
#define vsram_md_enable_bit (0)
#define vsram_md_mode_reg (0)
#define vsram_md_mode_bit (0)
#define vsram_md_min_uV 518750
#define vsram_md_max_uV 1312500
#define vsram_md_uV_step 6250
#define vsram_md_stb 110

#define vcamd1_vol_reg (0x1A70)
#define vcamd1_vol_mask (0xF)
#define vcamd1_vol_shift (8)
#define vcamd1_enable_reg (0x16F4)
#define vcamd1_enable_bit (0)
#define vcamd1_cal_reg (0x1A70)
#define vcamd1_cal_mask (0xF)
#define vcamd1_cal_shift (0)
#define vcamd1_min_uV  900000
#define vcamd1_max_uV 1200000
#define vcamd1_stb 264

#define va10_vol_reg (0x163E)
#define va10_vol_mask (0xF)
#define va10_vol_shift (8)
#define va10_enable_reg (0x163C)
#define va10_enable_bit (0)
#define va10_cal_reg (0x1A78)
#define va10_cal_mask (0xF)
#define va10_cal_shift (0)
#define va10_min_uV 600000
#define va10_max_uV 2100000
#define va10_stb 264

#define vio18_enable_reg (0x1614)
#define vio18_enable_bit (0)
#define vio18_cal_reg (0x1A66)
#define vio18_cal_mask (0xF)
#define vio18_cal_shift (0)
#define vio18_min_uV 1800000
#define vio18_max_uV 1800000
#define vio18_stb 264

#define vio28_enable_reg (0x1600)
#define vio28_enable_bit (0)
#define vio28_cal_reg (0x1A3A)
#define vio28_cal_mask (0xF)
#define vio28_cal_shift (0)
#define vio28_min_uV 2800000
#define vio28_max_uV 2800000
#define vio28_stb 264

#define vusb33_enable_reg (0x167A)
#define vusb33_enable_bit (0)
#define vusb33_cal_reg (0x1A1E)
#define vusb33_cal_mask (0xF)
#define vusb33_cal_shift (0)
#define vusb33_min_uV 3000000
#define vusb33_max_uV 3000000
#define vusb33_stb 264
#else
#include <mt6355.h>
#endif

//==============================================================================
// PMIC Register Index
//==============================================================================
//register number
#include <upmu_hw.h>


//==============================================================================
// PMIC Status Code
//==============================================================================
#define PMIC_TEST_PASS               0x0000
#define PMIC_TEST_FAIL               0xB001
#define PMIC_EXCEED_I2C_FIFO_LENGTH  0xB002
#define PMIC_CHRDET_EXIST            0xB003
#define PMIC_CHRDET_NOT_EXIST        0xB004
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

//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
extern int pmic_IsVbatDrop(void);
extern CHARGER_TYPE mt_charger_type_detection(void);
extern int hw_check_battery(void);
extern void pl_check_bat_protect_status();
extern void pmic_DetectVbatDrop (void);
extern int PMIC_POWER_HOLD(unsigned int hold);
void hw_set_cc(int cc_val);
#endif

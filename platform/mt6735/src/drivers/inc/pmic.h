
#ifndef _PL_MT_PMIC_H_
#define _PL_MT_PMIC_H_

//==============================================================================
// The CHIP INFO
//==============================================================================
#define PMIC6325_E1_CID_CODE    0x2510
#define PMIC6325_E2_CID_CODE    0x2520
#define PMIC6325_E3_CID_CODE    0x2530


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
extern int hw_check_battery(void);
extern void pl_charging(int en_chr);
extern void pl_kick_chr_wdt(void);
extern void pl_close_pre_chr_led(void);
extern void pl_hw_ulc_det(void);
extern U32 pmic_init (void);

#endif 


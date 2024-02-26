#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>
#include <mt6336_upmu_hw.h>
#include "pal_log.h"

#define PMIC_TYPE_DETECTION

CHARGER_TYPE g_ret = CHARGER_UNKNOWN;
int g_charger_in_flag = 0;
int g_first_check=0;


#ifndef PMIC_TYPE_DETECTION
/*
 * Force to USB Standard Host
 */
int hw_charger_type_detection(void)
{
    pal_log_info("force STANDARD_HOST\r\n");
    return STANDARD_HOST;
}
#else

/*
 * Charger Type Detection
 */
extern void Charger_Detect_Init(void);
extern void Charger_Detect_Release(void);
extern void mdelay (unsigned long msec);
extern unsigned int mt6336_config_interface(unsigned int RegNum, unsigned char val, unsigned char MASK, unsigned char SHIFT);
extern unsigned int mt6336_read_interface(unsigned int RegNum, unsigned char *val, unsigned char MASK, unsigned char SHIFT);


static void hw_bc12_init(void)
{
	int timeout = 200;

	/* RG_BC12_BB_CTRL = 1 */
	mt6336_config_interface(MT6336_RG_A_BC12_BB_CTRL_ADDR, 1, MT6336_RG_A_BC12_BB_CTRL_MASK, MT6336_RG_A_BC12_BB_CTRL_SHIFT);
	/* RG_BC12_RST = 1 */
	mt6336_config_interface(MT6336_RG_BC12_RST_ADDR, 1, MT6336_RG_BC12_RST_MASK, MT6336_RG_BC12_RST_SHIFT);
	/* RG_A_BC12_IBIAS_EN = 1 */
	mt6336_config_interface(MT6336_RG_A_BC12_IBIAS_EN_ADDR, 1, MT6336_RG_A_BC12_IBIAS_EN_MASK, MT6336_RG_A_BC12_IBIAS_EN_SHIFT);
	/* Delay 10ms */
	mdelay(10);
	/* RG_A_BC12_VSRC_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_VSRC_EN_ADDR, 0, MT6336_RG_A_BC12_VSRC_EN_MASK, MT6336_RG_A_BC12_VSRC_EN_SHIFT);
	/* RG_A_BC12_VREF_VTH_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_VREF_VTH_EN_ADDR, 0, MT6336_RG_A_BC12_VREF_VTH_EN_MASK, MT6336_RG_A_BC12_VREF_VTH_EN_SHIFT);
	/* RG_A_BC12_CMP_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_CMP_EN_ADDR, 0, MT6336_RG_A_BC12_CMP_EN_MASK, MT6336_RG_A_BC12_CMP_EN_SHIFT);
	/* RG_A_BC12_IPU_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPU_EN_ADDR, 0, MT6336_RG_A_BC12_IPU_EN_MASK, MT6336_RG_A_BC12_IPU_EN_SHIFT);
	/* RG_A_BC12_IPD_EN[1:0]= 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_EN_ADDR, 0, MT6336_RG_A_BC12_IPD_EN_MASK, MT6336_RG_A_BC12_IPD_EN_SHIFT);
	/* RG_A_BC12_IPD_HALF_EN = 0 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_HALF_EN_ADDR, 0, MT6336_RG_A_BC12_IPD_HALF_EN_MASK, MT6336_RG_A_BC12_IPD_HALF_EN_SHIFT);
/*
#if defined(CONFIG_PROJECT_PHY)
	Charger_Detect_Init();
#endif
*/
}


static unsigned int hw_bc12_step_1(void)
{
	unsigned int wChargerAvail = 0;
	unsigned int tmp = 0;

	/* RG_A_BC12_VSRC_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_VSRC_EN_ADDR, 0, MT6336_RG_A_BC12_VSRC_EN_MASK, MT6336_RG_A_BC12_VSRC_EN_SHIFT);
	/* RG_A_BC12_VREF_VTH_EN[1:0] = 01 */
	mt6336_config_interface(MT6336_RG_A_BC12_VREF_VTH_EN_ADDR, 1, MT6336_RG_A_BC12_VREF_VTH_EN_MASK, MT6336_RG_A_BC12_VREF_VTH_EN_SHIFT);
	/* RG_A_BC12_CMP_EN[1:0] = 10 */
	mt6336_config_interface(MT6336_RG_A_BC12_CMP_EN_ADDR, 2, MT6336_RG_A_BC12_CMP_EN_MASK, MT6336_RG_A_BC12_CMP_EN_SHIFT);
	/* RG_A_BC12_IPU_EN[1:0] = 10 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPU_EN_ADDR, 2, MT6336_RG_A_BC12_IPU_EN_MASK, MT6336_RG_A_BC12_IPU_EN_SHIFT);
	/* RG_A_BC12_IPD_EN[1:0]= 01 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_EN_ADDR, 1, MT6336_RG_A_BC12_IPD_EN_MASK, MT6336_RG_A_BC12_IPD_EN_SHIFT);
	/* RG_A_BC12_IPD_HALF_EN = 0 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_HALF_EN_ADDR, 0, MT6336_RG_A_BC12_IPD_HALF_EN_MASK, MT6336_RG_A_BC12_IPD_HALF_EN_SHIFT);

	/* Delay 10ms */
	mdelay(10);

	/* RGS_BC12_CMP_OUT */
	/* Check BC 1.2 comparator output
	 * 0: Jump to 2-a
	 * 1: Jump to 2-b
	 */
	mt6336_read_interface(MT6336_AD_QI_BC12_CMP_OUT_ADDR, &wChargerAvail, MT6336_AD_QI_BC12_CMP_OUT_MASK, MT6336_AD_QI_BC12_CMP_OUT_SHIFT);

	/* Delay 10ms */
	mdelay(10);

	return wChargerAvail;
}

static unsigned int hw_bc12_step_2a(void)
{
	unsigned int wChargerAvail = 0;
	unsigned int tmp = 0;

	/* RG_A_BC12_VSRC_EN[1:0] = 10 */
	mt6336_config_interface(MT6336_RG_A_BC12_VSRC_EN_ADDR, 2, MT6336_RG_A_BC12_VSRC_EN_MASK, MT6336_RG_A_BC12_VSRC_EN_SHIFT);
	/* RG_A_BC12_VREF_VTH_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_VREF_VTH_EN_ADDR, 0, MT6336_RG_A_BC12_VREF_VTH_EN_MASK, MT6336_RG_A_BC12_VREF_VTH_EN_SHIFT);
	/* RG_A_BC12_CMP_EN[1:0] = 01 */
	mt6336_config_interface(MT6336_RG_A_BC12_CMP_EN_ADDR, 1, MT6336_RG_A_BC12_CMP_EN_MASK, MT6336_RG_A_BC12_CMP_EN_SHIFT);
	/* RG_A_BC12_IPU_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPU_EN_ADDR, 0, MT6336_RG_A_BC12_IPU_EN_MASK, MT6336_RG_A_BC12_IPU_EN_SHIFT);
	/* RG_A_BC12_IPD_EN[1:0]= 01 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_EN_ADDR, 1, MT6336_RG_A_BC12_IPD_EN_MASK, MT6336_RG_A_BC12_IPD_EN_SHIFT);
	/* RG_A_BC12_IPD_HALF_EN = 0 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_HALF_EN_ADDR, 0, MT6336_RG_A_BC12_IPD_HALF_EN_MASK, MT6336_RG_A_BC12_IPD_HALF_EN_SHIFT);

	/* Delay 40ms */
	mdelay(40);

	/* RGS_BC12_CMP_OUT */
	/* Check BC 1.2 comparator output
	 * 0: SDP
	 * 1: Jump to 3-a
	 */
	mt6336_read_interface(MT6336_AD_QI_BC12_CMP_OUT_ADDR, &wChargerAvail, MT6336_AD_QI_BC12_CMP_OUT_MASK, MT6336_AD_QI_BC12_CMP_OUT_SHIFT);

	/* Delay 20ms */
	mdelay(20);

	return wChargerAvail;
}

static unsigned int hw_bc12_step_3a(void)
{
	unsigned int wChargerAvail = 0;

	/* RG_A_BC12_VSRC_EN[1:0] = 01 */
	mt6336_config_interface(MT6336_RG_A_BC12_VSRC_EN_ADDR, 1, MT6336_RG_A_BC12_VSRC_EN_MASK, MT6336_RG_A_BC12_VSRC_EN_SHIFT);
	/* RG_A_BC12_VREF_VTH_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_VREF_VTH_EN_ADDR, 0, MT6336_RG_A_BC12_VREF_VTH_EN_MASK, MT6336_RG_A_BC12_VREF_VTH_EN_SHIFT);
	/* RG_A_BC12_CMP_EN[1:0] = 10 */
	mt6336_config_interface(MT6336_RG_A_BC12_CMP_EN_ADDR, 2, MT6336_RG_A_BC12_CMP_EN_MASK, MT6336_RG_A_BC12_CMP_EN_SHIFT);
	/* RG_A_BC12_IPU_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPU_EN_ADDR, 0, MT6336_RG_A_BC12_IPU_EN_MASK, MT6336_RG_A_BC12_IPU_EN_SHIFT);
	/* RG_A_BC12_IPD_EN[1:0]= 10 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_EN_ADDR, 2, MT6336_RG_A_BC12_IPD_EN_MASK, MT6336_RG_A_BC12_IPD_EN_SHIFT);
	/* RG_A_BC12_IPD_HALF_EN = 0 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_HALF_EN_ADDR, 0, MT6336_RG_A_BC12_IPD_HALF_EN_MASK, MT6336_RG_A_BC12_IPD_HALF_EN_SHIFT);
	/* Delay 40ms */
	mdelay(40);

	/* RGS_BC12_CMP_OUT */
	/* Check BC 1.2 comparator output
	 * 0: CDP
	 * 1: DCP
	 */
	mt6336_read_interface(MT6336_AD_QI_BC12_CMP_OUT_ADDR, &wChargerAvail, MT6336_AD_QI_BC12_CMP_OUT_MASK, MT6336_AD_QI_BC12_CMP_OUT_SHIFT);
	/* Delay 20ms */
	mdelay(20);

	return wChargerAvail;
}



static void hw_bc12_done(void)
{
/*
#if defined(CONFIG_PROJECT_PHY)
	Charger_Detect_Release();
#endif
*/
	/* RG_BC12_BB_CTRL = 0 */
	mt6336_config_interface(MT6336_RG_A_BC12_BB_CTRL_ADDR, 0, MT6336_RG_A_BC12_BB_CTRL_MASK, MT6336_RG_A_BC12_BB_CTRL_SHIFT);
	/* RG_BC12_RST = 1 */
	mt6336_config_interface(MT6336_RG_BC12_RST_ADDR, 1, MT6336_RG_BC12_RST_MASK, MT6336_RG_BC12_RST_SHIFT);
	/* RG_A_BC12_IBIAS_EN = 0 */
	mt6336_config_interface(MT6336_RG_A_BC12_IBIAS_EN_ADDR, 0, MT6336_RG_A_BC12_IBIAS_EN_MASK, MT6336_RG_A_BC12_IBIAS_EN_SHIFT);
	/* RG_A_BC12_VSRC_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_VSRC_EN_ADDR, 0, MT6336_RG_A_BC12_VSRC_EN_MASK, MT6336_RG_A_BC12_VSRC_EN_SHIFT);
	/* RG_A_BC12_VREF_VTH_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_VREF_VTH_EN_ADDR, 0, MT6336_RG_A_BC12_VREF_VTH_EN_MASK, MT6336_RG_A_BC12_VREF_VTH_EN_SHIFT);
	/* RG_A_BC12_CMP_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_CMP_EN_ADDR, 0, MT6336_RG_A_BC12_CMP_EN_MASK, MT6336_RG_A_BC12_CMP_EN_SHIFT);
	/* RG_A_BC12_IPU_EN[1:0] = 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPU_EN_ADDR, 0, MT6336_RG_A_BC12_IPU_EN_MASK, MT6336_RG_A_BC12_IPU_EN_SHIFT);
	/* RG_A_BC12_IPD_EN[1:0]= 00 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_EN_ADDR, 0, MT6336_RG_A_BC12_IPD_EN_MASK, MT6336_RG_A_BC12_IPD_EN_SHIFT);
	/* RG_A_BC12_IPD_HALF_EN = 0 */
	mt6336_config_interface(MT6336_RG_A_BC12_IPD_HALF_EN_ADDR, 0, MT6336_RG_A_BC12_IPD_HALF_EN_MASK, MT6336_RG_A_BC12_IPD_HALF_EN_SHIFT);
}

static void dump_charger_name(CHARGER_TYPE type)
{
	switch (type) {
	case CHARGER_UNKNOWN:
		pal_log_info("charger type: %d, CHARGER_UNKNOWN\n", type);
		break;
	case STANDARD_HOST:
		pal_log_info("charger type: %d, Standard USB Host\n", type);
		break;
	case CHARGING_HOST:
		pal_log_info("charger type: %d, Charging USB Host\n", type);
		break;
	case NONSTANDARD_CHARGER:
		pal_log_info("charger type: %d, Non-standard Charger\n", type);
		break;
	case STANDARD_CHARGER:
		pal_log_info("charger type: %d, Standard Charger\n", type);
		break;
	default:
		pal_log_info("charger type: %d, Not Defined!!!\n", type);
		break;
	}
}

int hw_charger_type_detection(void)
{
	unsigned int out = 0;

	CHARGER_TYPE CHR_Type_num = CHARGER_UNKNOWN;

	hw_bc12_init();

	if (hw_bc12_step_1()) {
		CHR_Type_num = NONSTANDARD_CHARGER;
	} else {
		if (hw_bc12_step_2a()) {
			if (hw_bc12_step_3a())
				CHR_Type_num = STANDARD_CHARGER;
			else
				CHR_Type_num = CHARGING_HOST;
		} else
			CHR_Type_num = STANDARD_HOST;
	}

	dump_charger_name(CHR_Type_num);

	hw_bc12_done();

	return CHR_Type_num;
}

#endif

CHARGER_TYPE mt_charger_type_detection(void)
{
    if( g_first_check == 0 )
    {
        g_first_check = 1;
        if (upmu_is_chr_det())
        	g_ret = hw_charger_type_detection();
    }
    else
    {
        pal_log_info("[mt_charger_type_detection] charger type: %d\r\n", g_ret);
    }

    return g_ret;
}



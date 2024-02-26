#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>
#include "cust_usb.h"
#include "pal_log.h"

//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
#define V_CHARGER_MAX 6500				// 6.5 V
#define SWCHR_POWER_PATH 1
#ifdef MT6360_PMU
extern int mtk_get_vbus_adc(int *vbus);
#endif

#if SWCHR_POWER_PATH
#define BATTERY_LOWVOL_THRESOLD 0
#else
#define BATTERY_LOWVOL_THRESOLD 3300
#endif

CHARGER_TYPE g_ret = CHARGER_UNKNOWN;
int g_charger_in_flag = 0;
int g_first_check = 0;
static int vbat_status = PMIC_VBAT_NOT_DROP;
#ifdef PMIC_FORCE_CHARGER_TYPE

int hw_charger_type_detection(void)
{
    pal_log_info("force STANDARD_HOST\r\n");
    return STANDARD_HOST;
}

#else

extern void Charger_Detect_Init(void);
extern void Charger_Detect_Release(void);
extern void mdelay (unsigned long msec);

static void hw_bc11_dump_register(void)
{
/*
	kal_uint32 reg_val = 0;
	kal_uint32 reg_num = CHR_CON18;
	kal_uint32 i = 0;

	for(i=reg_num ; i<=CHR_CON19 ; i+=2)
	{
		reg_val = upmu_get_reg_value(i);
		pal_log_info("Chr Reg[0x%x]=0x%x \r\n", i, reg_val);
	}
*/
}

static void hw_bc11_init(void)
{
    mdelay(200);
    Charger_Detect_Init();
#if TBD
    //RG_bc11_BIAS_EN=1
    pmic_config_interface(PMIC_RG_BC11_BIAS_EN_ADDR, 1, PMIC_RG_BC11_BIAS_EN_MASK, PMIC_RG_BC11_BIAS_EN_SHIFT);
    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR,0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //bc11_RST=1
    pmic_config_interface(PMIC_RG_BC11_RST_ADDR, 1, PMIC_RG_BC11_RST_MASK, PMIC_RG_BC11_RST_SHIFT);
    //bc11_BB_CTRL=1
    pmic_config_interface(PMIC_RG_BC11_BB_CTRL_ADDR, 1, PMIC_RG_BC11_BB_CTRL_MASK, PMIC_RG_BC11_BB_CTRL_SHIFT);
#endif
    mdelay(50);
}

static U32 hw_bc11_DCD(void)
{
    U32 wChargerAvail = 0;

#if TBD
    //RG_bc11_IPU_EN[1.0] = 10
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x2, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x1, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=01
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x1, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 10
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x2, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, PMIC_RGS_BC11_CMP_OUT_MASK, PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x0, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
#endif /* TBD */
    return wChargerAvail;
}

static U32 hw_bc11_stepA1(void)
{
    U32 wChargerAvail = 0;

#if TBD
    //RG_bc11_IPD_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x1, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x1, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, PMIC_RGS_BC11_CMP_OUT_MASK, PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
#endif /* TBD */

    return  wChargerAvail;
}


static U32 hw_bc11_stepA2(void)
{
    U32 wChargerAvail = 0;

#if TBD
    //RG_bc11_VSRC_EN[1.0] = 10
    pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0x2, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_IPD_EN[1:0] = 01
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x1, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x1, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, PMIC_RGS_BC11_CMP_OUT_MASK, PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0x0, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
#endif /* TBD */
    return  wChargerAvail;
}


static U32 hw_bc11_stepB2(void)
{
    U32 wChargerAvail = 0;

#if TBD
    //RG_bc11_IPU_EN[1:0]=10
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x2, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=01
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x1, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x1, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, PMIC_RGS_BC11_CMP_OUT_MASK, PMIC_RGS_BC11_CMP_OUT_SHIFT);

    if (!wChargerAvail)
    {
        //RG_bc11_VSRC_EN[1.0] = 10
        //mt6325_upmu_set_rg_bc11_vsrc_en(0x2);
        pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0x2, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    }
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x0, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
#endif /* TBD */
    return  wChargerAvail;
}


static void hw_bc11_done(void)
{
#if TBD
    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(PMIC_RG_BC11_VSRC_EN_ADDR, 0x0, PMIC_RG_BC11_VSRC_EN_MASK, PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=0
    pmic_config_interface(PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, PMIC_RG_BC11_VREF_VTH_MASK, PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_CMP_EN_ADDR, 0x0, PMIC_RG_BC11_CMP_EN_MASK, PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPU_EN_ADDR, 0x0, PMIC_RG_BC11_IPU_EN_MASK, PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(PMIC_RG_BC11_IPD_EN_ADDR, 0x0, PMIC_RG_BC11_IPD_EN_MASK, PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_BIAS_EN=0
    pmic_config_interface(PMIC_RG_BC11_BIAS_EN_ADDR, 0x0, PMIC_RG_BC11_BIAS_EN_MASK, PMIC_RG_BC11_BIAS_EN_SHIFT);
#endif /* TBD */
    Charger_Detect_Release();
}

void pmic_DetectVbatDrop (void)
{

	U32 ret=0;
	U32 just_rst=0;

	pmic_read_interface(PMIC_JUST_PWRKEY_RST_ADDR, (&just_rst), PMIC_JUST_PWRKEY_RST_MASK, PMIC_JUST_PWRKEY_RST_SHIFT);
	pmic_config_interface(PMIC_CLR_JUST_RST_ADDR, 1, PMIC_CLR_JUST_RST_MASK, PMIC_CLR_JUST_RST_SHIFT);

	pal_log_info("[PMIC]just_rst = %d\n", just_rst);
	if(just_rst)
		vbat_status = PMIC_VBAT_DROP;
	else
		vbat_status = PMIC_VBAT_NOT_DROP;

}

int pmic_IsVbatDrop(void)
{
   /* used by RTC to check power on reason */
   return vbat_status;
}


void hw_set_cc(int cc_val)
{
    //TBD
}

int hw_charger_type_detection(void)
{
    CHARGER_TYPE charger_tye = CHARGER_UNKNOWN;

    /********* Step initial  ***************/
    hw_bc11_init();

    /********* Step DCD ***************/
    if(1 == hw_bc11_DCD())
    {
         /********* Step A1 ***************/
         if(1 == hw_bc11_stepA1())
         {
             charger_tye = APPLE_2_1A_CHARGER;
             pal_log_info("step A1 : Apple 2.1A CHARGER!\r\n");
         }
         else
         {
             charger_tye = NONSTANDARD_CHARGER;
             pal_log_info("step A1 : Non STANDARD CHARGER!\r\n");
         }
    }
    else
    {
         /********* Step A2 ***************/
         if(1 == hw_bc11_stepA2())
         {
             /********* Step B2 ***************/
             if(1 == hw_bc11_stepB2())
             {
                 charger_tye = STANDARD_CHARGER;
                 pal_log_info("step B2 : STANDARD CHARGER!\r\n");
             }
             else
             {
                 charger_tye = CHARGING_HOST;
                 pal_log_info("step B2 :  Charging Host!\r\n");
             }
         }
         else
         {
             charger_tye = STANDARD_HOST;
             pal_log_info("step A2 : Standard USB Host!\r\n");
         }
    }

    /********* Finally setting *******************************/
    hw_bc11_done();

    return charger_tye;
}
#endif

CHARGER_TYPE mt_charger_type_detection(void)
{
    if( g_first_check == 0 )
    {
        g_first_check = 1;

        if(upmu_is_chr_det() == KAL_FALSE)
            return CHARGER_UNKNOWN;

#ifdef MTK_EXT_CHGDET_SUPPORT
        mtk_ext_chgdet(&g_ret);
#else
        g_ret = hw_charger_type_detection();
#endif
    }
    else
    {
        pal_log_info("[mt_charger_type_detection] Got data !!, %d, %d\r\n", g_charger_in_flag, g_first_check);
    }

    return g_ret;
}

int hw_check_battery(void)
{
	/* ask shin-shyu programming guide */

	U32 val=0;
	U32 ret_val;
	#if CFG_FPGA_PLATFORM
		pal_log_info("FPGA ignore bat check !\n");
        return 1;
	#endif

	ret_val=pmic_config_interface( (U32)(PMIC_RG_LDO_VBIF28_EN_ADDR),
							 (U32)(1),
							 (U32)(PMIC_RG_LDO_VBIF28_EN_MASK),
							 (U32)(PMIC_RG_LDO_VBIF28_EN_SHIFT)
							 );
    #ifdef MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
        pal_log_info("ignore bat check !\n");
        return 1;
    #else
	if(drdi_get_hw_ver() == HW_VER_V0)
	{
		pal_log_info("ignore bat check\n");
		return 1;
	}
	else
	{
	pmic_upmu_set_rg_baton_en(1);
	/*mt6355_upmu_set_baton_tdet_en(1);*//*--After 6335, it's unnecessary--*/
	/*mdelay(100);*/
	val = pmic_upmu_get_rgs_baton_undet();

	if(val == 0)
	{
			pal_log_info("bat is exist.\n");
		return 1;
	}
	else
	{
			pal_log_info("bat NOT exist.\n");
		return 0;
	}
	}
    #endif

}

void pl_charging(int en_chr)
{
    //TBD
}

void pl_kick_chr_wdt(void)
{
    //TBD
}

void pl_close_pre_chr_led(void)
{
    //no charger feature
}

int get_bat_sense_volt(int times)
{
	return pmic_get_auxadc_value(AUXADC_LIST_BATADC);
}

int get_i_sense_volt(int times)
{
	/*TODO: MT6355 doesn't have isense auxadc */
	return pmic_get_auxadc_value(AUXADC_LIST_BATADC);
}

#define R_CHARGER_1 330
#define R_CHARGER_2 39


int get_charger_volt(int times)
{
	int val = 0;
#ifdef MT6360_PMU
	mtk_get_vbus_adc(&val);
	val /= 1000;
#else
	val = pmic_get_auxadc_value(AUXADC_LIST_VCDT);
	val = (((R_CHARGER_1+R_CHARGER_2)*100*val)/R_CHARGER_2)/100;
#endif
	return val;
}

void kick_charger_wdt(void)
{
#if TBD
	pmic_config_interface(PMIC_RG_CHRWDT_TD_ADDR,3,PMIC_RG_CHRWDT_TD_MASK,PMIC_RG_CHRWDT_TD_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_WR_ADDR,1,PMIC_RG_CHRWDT_WR_MASK,PMIC_RG_CHRWDT_WR_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_INT_EN_ADDR,1,PMIC_RG_CHRWDT_INT_EN_MASK,PMIC_RG_CHRWDT_INT_EN_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_EN_ADDR,1,PMIC_RG_CHRWDT_EN_MASK,PMIC_RG_CHRWDT_EN_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_FLAG_WR_ADDR,1,PMIC_RG_CHRWDT_FLAG_WR_MASK,PMIC_RG_CHRWDT_FLAG_WR_SHIFT);
#endif /* TBD */
/*
	pmic_set_register_value(PMIC_RG_CHRWDT_TD,3);  // CHRWDT_TD, 32s for keep charging for lk to kernel
	pmic_set_register_value(PMIC_RG_CHRWDT_WR,1); // CHRWDT_WR
	pmic_set_register_value(PMIC_RG_CHRWDT_INT_EN,1);	// CHRWDT_INT_EN
	pmic_set_register_value(PMIC_RG_CHRWDT_EN,1);		// CHRWDT_EN
	pmic_set_register_value(PMIC_RG_CHRWDT_FLAG_WR,1);// CHRWDT_WR
*/
}

void pchr_turn_on_charging(kal_bool bEnable)
{
	/*pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 1, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);*/
	//pmic_set_register_value(PMIC_RG_USBDL_RST,1);//force leave USBDL mode
	//mt6325_upmu_set_rg_usbdl_rst(1);       //force leave USBDL mode
#if TBD
	pmic_config_interface(PMIC_RG_BC11_RST_ADDR, 1, PMIC_RG_BC11_RST_MASK, PMIC_RG_BC11_RST_SHIFT);

	kick_charger_wdt();

	pmic_config_interface(PMIC_RG_CS_VTH_ADDR,0xc,PMIC_RG_CS_VTH_MASK,PMIC_RG_CS_VTH_SHIFT);
#if MT6351
	pmic_config_interface(PMIC_RG_NORM_CS_VTH_ADDR,0xc,PMIC_RG_NORM_CS_VTH_MASK,PMIC_RG_NORM_CS_VTH_SHIFT);
#endif

	//pmic_set_register_value(PMIC_RG_CS_VTH,0xC);	// CS_VTH, 450mA
	//mt6325_upmu_set_rg_cs_vth(0xC);             // CS_VTH, 450mA

	pmic_config_interface(PMIC_RG_CSDAC_EN_ADDR,1,PMIC_RG_CSDAC_EN_MASK,PMIC_RG_CSDAC_EN_SHIFT);
	//pmic_set_register_value(PMIC_RG_CSDAC_EN,1);
	//mt6325_upmu_set_rg_csdac_en(1);				// CSDAC_EN

	pmic_config_interface(PMIC_RG_CHR_EN_ADDR,1,PMIC_RG_CHR_EN_MASK,PMIC_RG_CHR_EN_SHIFT);

#if MT6351
	pmic_config_interface(PMIC_RG_NORM_CHR_EN_ADDR,1,PMIC_RG_NORM_CHR_EN_MASK,PMIC_RG_NORM_CHR_EN_SHIFT);
#endif
	//pmic_set_register_value(PMIC_RG_CHR_EN,1);
	//mt6325_upmu_set_rg_chr_en(1);				// CHR_EN

        pmic_config_interface(PMIC_RG_CSDAC_MODE_ADDR,1,PMIC_RG_CSDAC_MODE_MASK,PMIC_RG_CSDAC_MODE_SHIFT);
        pmic_config_interface(PMIC_RG_CSDAC_EN_ADDR,1,PMIC_RG_CSDAC_EN_MASK,PMIC_RG_CSDAC_EN_SHIFT);
#endif /* TBD */
}

#define CUST_R_SENSE         68
int get_charging_current(int times)
{
	int ret;
	kal_int32 ADC_I_SENSE=1;   // 1 measure time
	kal_int32 ADC_BAT_SENSE=1; // 1 measure time
	int ICharging=0;

	ADC_I_SENSE=get_i_sense_volt(1);
	ADC_BAT_SENSE=get_bat_sense_volt(1);

	ICharging = (ADC_I_SENSE - ADC_BAT_SENSE )*1000/CUST_R_SENSE;

	return ICharging;
}


void pl_check_bat_protect_status(void)
{
    kal_int32 bat_val = 0;
	int current,chr_volt,cnt=0,i;

#if SWCHR_POWER_PATH
    bat_val = get_i_sense_volt(5);
#else
    bat_val = get_bat_sense_volt(5);
#endif

    chr_volt= get_charger_volt(1);
    pal_log_info( "[%s]: check VBAT=%d mV with %d mV, VCHR %d mV ,VCHR_HV=%d  start charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD,chr_volt,V_CHARGER_MAX);


    //VCDT_HV_VTH, 7V
    //pmic_config_interface(PMIC_RG_VCDT_HV_VTH_ADDR, V_CHARGER_MAX, PMIC_RG_VCDT_HV_VTH_MASK, PMIC_RG_VCDT_HV_VTH_SHIFT);
    //VCDT_HV_EN=1
   	//pmic_config_interface(PMIC_RG_VCDT_HV_EN_ADDR, 0x01, PMIC_RG_VCDT_HV_EN_MASK, PMIC_RG_VCDT_HV_EN_SHIFT);


    while (bat_val < BATTERY_LOWVOL_THRESOLD)
    {
        mtk_wdt_restart();
        if(upmu_is_chr_det() == KAL_FALSE)
        {
            pal_log_info( "[PL][BATTERY] No Charger, Power OFF !\n");
            break;
        }

		chr_volt= get_charger_volt(1);
		if(chr_volt>V_CHARGER_MAX)
		{
            pal_log_info( "[PL][BATTERY] charger voltage is too high :%d , threshold is %d !\n",chr_volt,V_CHARGER_MAX);
            break;
		}


        pchr_turn_on_charging(KAL_TRUE);


		cnt=0;
		for(i=0;i<10;i++)
		{
			current=get_charging_current(1);
			chr_volt=get_charger_volt(1);
			if(current<100 && chr_volt<4400)
			{
				cnt++;
				pal_log_info( "[PL][BATTERY] charging current=%d charger volt=%d\n\r",current,chr_volt);
			}
			else
			{
				pal_log_info( "[PL][BATTERY] charging current=%d charger volt=%d\n\r",current,chr_volt);
				cnt=0;
			}
		}

		if(cnt>=8)
		{

	            pal_log_info( "[PL][BATTERY] charging current and charger volt too low !! \n\r",cnt);

	            pchr_turn_on_charging(KAL_FALSE);
	   			break;
		}
		mdelay(50);

#if SWCHR_POWER_PATH
		bat_val = get_i_sense_volt(5);
#else
		bat_val = get_bat_sense_volt(5);
#endif
		 pal_log_info( "[%s]: check VBAT=%d mV  chr=%d\n", __FUNCTION__, bat_val, chr_volt);
    }

    pal_log_info( "[%s]: check VBAT=%d mV with %d mV, stop charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD);
}

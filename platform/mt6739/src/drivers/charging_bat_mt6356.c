#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>

//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
#define V_CHARGER_MAX 6500				// 6.5 V
#define SWCHR_POWER_PATH 1

#if SWCHR_POWER_PATH
#define BATTERY_LOWVOL_THRESOLD 0
#else
#define BATTERY_LOWVOL_THRESOLD 3300
#endif

CHARGER_TYPE g_ret = CHARGER_UNKNOWN;
int g_charger_in_flag = 0;
int g_first_check=0;
static int vbat_status = PMIC_VBAT_NOT_DROP;
#define PMIC_TYPE_DETECTION
#ifndef PMIC_TYPE_DETECTION

int hw_charger_type_detection(void)
{
    print("force STANDARD_HOST\r\n");
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
		print("Chr Reg[0x%x]=0x%x \r\n", i, reg_val);
	}
*/
}

static void hw_bc11_init(void)
{
    mdelay(200);
    Charger_Detect_Init();

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

    mdelay(50);
}

static U32 hw_bc11_DCD(void)
{
    U32 wChargerAvail = 0;

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

    return wChargerAvail;
}

static U32 hw_bc11_stepA1(void)
{
    U32 wChargerAvail = 0;

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

    return  wChargerAvail;
}


static U32 hw_bc11_stepA2(void)
{
    U32 wChargerAvail = 0;

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

    return  wChargerAvail;
}


static U32 hw_bc11_stepB2(void)
{
    U32 wChargerAvail = 0;

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

    return  wChargerAvail;
}


static void hw_bc11_done(void)
{
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

    Charger_Detect_Release();
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
             print("step A1 : Apple 2.1A CHARGER!\r\n");
         }
         else
         {
             charger_tye = NONSTANDARD_CHARGER;
             print("step A1 : Non STANDARD CHARGER!\r\n");
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
                 print("step B2 : STANDARD CHARGER!\r\n");
             }
             else
             {
                 charger_tye = CHARGING_HOST;
                 print("step B2 :  Charging Host!\r\n");
             }
         }
         else
         {
             charger_tye = STANDARD_HOST;
             print("step A2 : Standard USB Host!\r\n");
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
        g_ret = hw_charger_type_detection();
    }
    else
    {
        print("[mt_charger_type_detection] Got data !!, %d, %d\r\n", g_charger_in_flag, g_first_check);
    }

    return g_ret;
}

int hw_check_battery(void)
{
#ifdef MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
	print("ignore bat check\n");
	return 1;
#else

#if CFG_EVB_PLATFORM
	print("ignore bat check\n");
	return 1;
#else
	U32 val = 0;
	U32 ret_val;

	ret_val = pmic_config_interface(PMIC_RG_LDO_VBIF28_EN_ADDR, 1,
					PMIC_RG_LDO_VBIF28_EN_MASK,
					PMIC_RG_LDO_VBIF28_EN_SHIFT);

	pmic_upmu_set_rg_baton_en(1);
	/*mdelay(100);*/
	val = pmic_upmu_get_rgs_baton_undet();


	if(val == 0) {
		print("battery exists\n");
		return 1;
	} else {
		print("battery doesn't exist\n");
		return 0;
	}
#endif /* !CFG_EVB_PLATFORM */
#endif /* !MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION */
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

#if !CFG_EVB_PLATFORM
int get_i_sense_volt(int times)
{
	/* TODO: Check whether MT6356 has isense auxadc */
	return pmic_get_auxadc_value(AUXADC_LIST_BATADC);
}

#define R_CHARGER_1 330
#define R_CHARGER_2 39


int get_charger_volt(int times)
{
	kal_int32 val;
	val = pmic_get_auxadc_value(AUXADC_LIST_VCDT);
	val = (((R_CHARGER_1+R_CHARGER_2)*100*val)/R_CHARGER_2)/100;
	return val;
}

void kick_charger_wdt(void)
{
	pmic_config_interface(PMIC_RG_CHRWDT_TD_ADDR,3,PMIC_RG_CHRWDT_TD_MASK,PMIC_RG_CHRWDT_TD_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_WR_ADDR,1,PMIC_RG_CHRWDT_WR_MASK,PMIC_RG_CHRWDT_WR_SHIFT);
	pmic_config_interface(PMIC_RG_INT_EN_WATCHDOG_ADDR,1,PMIC_RG_INT_EN_WATCHDOG_MASK,PMIC_RG_INT_EN_WATCHDOG_SHIFT);
	pmic_config_interface(PMIC_RG_CHRWDT_EN_ADDR,1,PMIC_RG_CHRWDT_EN_MASK,PMIC_RG_CHRWDT_EN_SHIFT);
	/* pmic_config_interface(PMIC_RG_CHRWDT_FLAG_WR_ADDR,1,PMIC_RG_CHRWDT_FLAG_WR_MASK,PMIC_RG_CHRWDT_FLAG_WR_SHIFT); */

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
	pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 1, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);
	//pmic_set_register_value(PMIC_RG_USBDL_RST,1);//force leave USBDL mode
	//mt6325_upmu_set_rg_usbdl_rst(1);       //force leave USBDL mode
	pmic_config_interface(PMIC_RG_BC11_RST_ADDR, 1, PMIC_RG_BC11_RST_MASK, PMIC_RG_BC11_RST_SHIFT);

	kick_charger_wdt();

	pmic_config_interface(PMIC_RG_CS_VTH_ADDR,0xc,PMIC_RG_CS_VTH_MASK,PMIC_RG_CS_VTH_SHIFT);
	//pmic_set_register_value(PMIC_RG_CS_VTH,0xC);	// CS_VTH, 450mA
	//mt6325_upmu_set_rg_cs_vth(0xC);             // CS_VTH, 450mA

	pmic_config_interface(PMIC_RG_CSDAC_EN_ADDR,1,PMIC_RG_CSDAC_EN_MASK,PMIC_RG_CSDAC_EN_SHIFT);
	//pmic_set_register_value(PMIC_RG_CSDAC_EN,1);
	//mt6325_upmu_set_rg_csdac_en(1);				// CSDAC_EN

	pmic_config_interface(PMIC_RG_CHR_EN_ADDR,1,PMIC_RG_CHR_EN_MASK,PMIC_RG_CHR_EN_SHIFT);
	//pmic_set_register_value(PMIC_RG_CHR_EN,1);
	//mt6325_upmu_set_rg_chr_en(1);				// CHR_EN

        pmic_config_interface(PMIC_RG_CSDAC_MODE_ADDR,1,PMIC_RG_CSDAC_MODE_MASK,PMIC_RG_CSDAC_MODE_SHIFT);
        pmic_config_interface(PMIC_RG_CSDAC_EN_ADDR,1,PMIC_RG_CSDAC_EN_MASK,PMIC_RG_CSDAC_EN_SHIFT);
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

    bat_val = get_bat_sense_volt(5);

    chr_volt= get_charger_volt(1);
    print( "[%s]: check VBAT=%d mV with %d mV, VCHR %d mV ,VCHR_HV=%d  start charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD,chr_volt,V_CHARGER_MAX);


    //VCDT_HV_VTH, 7V
    //pmic_config_interface(PMIC_RG_VCDT_HV_VTH_ADDR, V_CHARGER_MAX, PMIC_RG_VCDT_HV_VTH_MASK, PMIC_RG_VCDT_HV_VTH_SHIFT);
    //VCDT_HV_EN=1
	//pmic_config_interface(PMIC_RG_VCDT_HV_EN_ADDR, 0x01, PMIC_RG_VCDT_HV_EN_MASK, PMIC_RG_VCDT_HV_EN_SHIFT);


    while (bat_val < BATTERY_LOWVOL_THRESOLD)
    {
        mtk_wdt_restart();
        if(upmu_is_chr_det() == KAL_FALSE)
        {
            print( "[BAT] No Charger, Power OFF !\n");
            break;
        }

		chr_volt= get_charger_volt(1);
		if(chr_volt>V_CHARGER_MAX)
		{
            print( "[BAT] charger voltage is too high :%d , threshold is %d !\n",chr_volt,V_CHARGER_MAX);
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
				print( "[BAT] charging current=%d charger volt=%d\n\r",current,chr_volt);
			}
			else
			{
				print( "[BAT] charging current=%d charger volt=%d\n\r",current,chr_volt);
				cnt=0;
			}
		}

		if(cnt>=8)
		{

	            print( "[BAT] charging current and charger volt too low !!\n\r",cnt);

	            pchr_turn_on_charging(KAL_FALSE);
	   			break;
		}
		mdelay(50);

		bat_val = get_bat_sense_volt(5);
		print("[%s]: check VBAT=%dmV, Vchr=%dmV\n", __FUNCTION__, bat_val, chr_volt);
    }

    print( "[%s]: check VBAT=%d mV with %d mV, stop charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD);
}
#endif



#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>
#include <mt6311.h>



#define V_CHARGER_MAX 6500				// 6.5 V
#define BATTERY_LOWVOL_THRESOLD 3300

const static unsigned int mt6328_ovp_trim[] = {
	0x05,	0x06,	0x07,	0x07,	0x07,	0x07,
	0x07,	0x07,	0x0d,	0x0e,	0x0f,	0x0,
	0x01,	0x02,	0x03,	0x04
};


//==============================================================================
// PMIC access API
//==============================================================================
U32 pmic_read_interface (U32 RegNum, U32 *val, U32 MASK, U32 SHIFT)
{
    U32 return_value = 0;
    U32 pmic_reg = 0;
    U32 rdata = 0;

    //mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        /*print("[pmic_read_interface] Reg[%x]= pmic_wrap read data fail\n", RegNum);*/
        return return_value;
    }
    //print("[pmic_read_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

    pmic_reg &= (MASK << SHIFT);
    *val = (pmic_reg >> SHIFT);
    //print("[pmic_read_interface] val=0x%x\n", *val);

    return return_value;
}

U32 pmic_get_register_value(U32 RegNum, U32 MASK, U32 SHIFT)
{
	U32 val = 0;
	U32 ret;

	ret=pmic_read_interface (RegNum,&val,MASK,SHIFT);

	return val;
}





U32 pmic_config_interface (U32 RegNum, U32 val, U32 MASK, U32 SHIFT)
{
    U32 return_value = 0;
    U32 pmic_reg = 0;
    U32 rdata = 0;

    //1. mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        /*print("[pmic_config_interface] Reg[%x]= pmic_wrap read data fail\n", RegNum);*/
        return return_value;
    }
    //print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

    pmic_reg &= ~(MASK << SHIFT);
    pmic_reg |= (val << SHIFT);

    //2. mt_write_byte(RegNum, pmic_reg);
    return_value= pwrap_wacs2(1, (RegNum), pmic_reg, &rdata);
    if(return_value!=0)
    {
        /*print("[pmic_config_interface] Reg[%x]= pmic_wrap read data fail\n", RegNum);*/
        return return_value;
    }
    //print("[pmic_config_interface] write Reg[%x]=0x%x\n", RegNum, pmic_reg);    

#if 0
    //3. Double Check
    //mt_read_byte(RegNum, &pmic_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic_reg=rdata;
    if(return_value!=0)
    {
        print("[pmic_config_interface] Reg[%x]= pmic_wrap write data fail\n", RegNum);
        return return_value;
    }
    print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);
#endif

    return return_value;
}

void upmu_set_reg_value(kal_uint32 reg, kal_uint32 reg_val)
{
    U32 ret=0;

    ret=pmic_config_interface(reg, reg_val, 0xFFFF, 0x0);
}


//==============================================================================
// PMIC-Charger Type Detection
//==============================================================================
CHARGER_TYPE g_ret = CHARGER_UNKNOWN;
int g_charger_in_flag = 0;
int g_first_check=0;

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
    pmic_config_interface(MT6328_PMIC_RG_BC11_BIAS_EN_ADDR, 1, MT6328_PMIC_RG_BC11_BIAS_EN_MASK, MT6328_PMIC_RG_BC11_BIAS_EN_SHIFT);
    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(MT6328_PMIC_RG_BC11_VSRC_EN_ADDR, 0, MT6328_PMIC_RG_BC11_VSRC_EN_MASK, MT6328_PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(MT6328_PMIC_RG_BC11_VREF_VTH_ADDR,0, MT6328_PMIC_RG_BC11_VREF_VTH_MASK, MT6328_PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPU_EN_ADDR, 0, MT6328_PMIC_RG_BC11_IPU_EN_MASK, MT6328_PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPD_EN_ADDR, 0, MT6328_PMIC_RG_BC11_IPD_EN_MASK, MT6328_PMIC_RG_BC11_IPD_EN_SHIFT);
    //bc11_RST=1
    pmic_config_interface(MT6328_PMIC_RG_BC11_RST_ADDR, 1, MT6328_PMIC_RG_BC11_RST_MASK, MT6328_PMIC_RG_BC11_RST_SHIFT);
    //bc11_BB_CTRL=1
    pmic_config_interface(MT6328_PMIC_RG_BC11_BB_CTRL_ADDR, 1, MT6328_PMIC_RG_BC11_BB_CTRL_MASK, MT6328_PMIC_RG_BC11_BB_CTRL_SHIFT);

    mdelay(50);
}

static U32 hw_bc11_DCD(void)
{
    U32 wChargerAvail = 0;

    //RG_bc11_IPU_EN[1.0] = 10
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPU_EN_ADDR, 0x2, MT6328_PMIC_RG_BC11_IPU_EN_MASK, MT6328_PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 01
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPD_EN_ADDR, 0x1, MT6328_PMIC_RG_BC11_IPD_EN_MASK, MT6328_PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=01
    pmic_config_interface(MT6328_PMIC_RG_BC11_VREF_VTH_ADDR, 0x1, MT6328_PMIC_RG_BC11_VREF_VTH_MASK, MT6328_PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 10
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x2, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(MT6328_PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, MT6328_PMIC_RGS_BC11_CMP_OUT_MASK, MT6328_PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPU_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_IPU_EN_MASK, MT6328_PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPD_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_IPD_EN_MASK, MT6328_PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(MT6328_PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, MT6328_PMIC_RG_BC11_VREF_VTH_MASK, MT6328_PMIC_RG_BC11_VREF_VTH_SHIFT);

    return wChargerAvail;
}

static U32 hw_bc11_stepA1(void)
{
    U32 wChargerAvail = 0;

    //RG_bc11_IPD_EN[1.0] = 01
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPD_EN_ADDR, 0x1, MT6328_PMIC_RG_BC11_IPD_EN_MASK, MT6328_PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(MT6328_PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, MT6328_PMIC_RG_BC11_VREF_VTH_MASK, MT6328_PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x1, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(MT6328_PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, MT6328_PMIC_RGS_BC11_CMP_OUT_MASK, MT6328_PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPD_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_IPD_EN_MASK, MT6328_PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);

    return  wChargerAvail;
}


static U32 hw_bc11_stepA2(void)
{
    U32 wChargerAvail = 0;

    //RG_bc11_VSRC_EN[1.0] = 10
    pmic_config_interface(MT6328_PMIC_RG_BC11_VSRC_EN_ADDR, 0x2, MT6328_PMIC_RG_BC11_VSRC_EN_MASK, MT6328_PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_IPD_EN[1:0] = 01
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPD_EN_ADDR, 0x1, MT6328_PMIC_RG_BC11_IPD_EN_MASK, MT6328_PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(MT6328_PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, MT6328_PMIC_RG_BC11_VREF_VTH_MASK, MT6328_PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x1, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(MT6328_PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, MT6328_PMIC_RGS_BC11_CMP_OUT_MASK, MT6328_PMIC_RGS_BC11_CMP_OUT_SHIFT);

    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(MT6328_PMIC_RG_BC11_VSRC_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_VSRC_EN_MASK, MT6328_PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPD_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_IPD_EN_MASK, MT6328_PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);

    return  wChargerAvail;
}


static U32 hw_bc11_stepB2(void)
{
    U32 wChargerAvail = 0;

    //RG_bc11_IPU_EN[1:0]=10
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPU_EN_ADDR, 0x2, MT6328_PMIC_RG_BC11_IPU_EN_MASK, MT6328_PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=01
    pmic_config_interface(MT6328_PMIC_RG_BC11_VREF_VTH_ADDR, 0x1, MT6328_PMIC_RG_BC11_VREF_VTH_MASK, MT6328_PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 01
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x1, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);

    mdelay(80);

    pmic_read_interface(MT6328_PMIC_RGS_BC11_CMP_OUT_ADDR, &wChargerAvail, MT6328_PMIC_RGS_BC11_CMP_OUT_MASK, MT6328_PMIC_RGS_BC11_CMP_OUT_SHIFT);

    if (!wChargerAvail)
    {
        //RG_bc11_VSRC_EN[1.0] = 10
        //mt6325_upmu_set_rg_bc11_vsrc_en(0x2);
        pmic_config_interface(MT6328_PMIC_RG_BC11_VSRC_EN_ADDR, 0x2, MT6328_PMIC_RG_BC11_VSRC_EN_MASK, MT6328_PMIC_RG_BC11_VSRC_EN_SHIFT);
    }
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPU_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_IPU_EN_MASK, MT6328_PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=00
    pmic_config_interface(MT6328_PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, MT6328_PMIC_RG_BC11_VREF_VTH_MASK, MT6328_PMIC_RG_BC11_VREF_VTH_SHIFT);

    return  wChargerAvail;
}


static void hw_bc11_done(void)
{
    //RG_bc11_VSRC_EN[1:0]=00
    pmic_config_interface(MT6328_PMIC_RG_BC11_VSRC_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_VSRC_EN_MASK, MT6328_PMIC_RG_BC11_VSRC_EN_SHIFT);
    //RG_bc11_VREF_VTH = [1:0]=0
    pmic_config_interface(MT6328_PMIC_RG_BC11_VREF_VTH_ADDR, 0x0, MT6328_PMIC_RG_BC11_VREF_VTH_MASK, MT6328_PMIC_RG_BC11_VREF_VTH_SHIFT);
    //RG_bc11_CMP_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_CMP_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_CMP_EN_MASK, MT6328_PMIC_RG_BC11_CMP_EN_SHIFT);
    //RG_bc11_IPU_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPU_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_IPU_EN_MASK, MT6328_PMIC_RG_BC11_IPU_EN_SHIFT);
    //RG_bc11_IPD_EN[1.0] = 00
    pmic_config_interface(MT6328_PMIC_RG_BC11_IPD_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_IPD_EN_MASK, MT6328_PMIC_RG_BC11_IPD_EN_SHIFT);
    //RG_bc11_BIAS_EN=0
    pmic_config_interface(MT6328_PMIC_RG_BC11_BIAS_EN_ADDR, 0x0, MT6328_PMIC_RG_BC11_BIAS_EN_MASK, MT6328_PMIC_RG_BC11_BIAS_EN_SHIFT);

    Charger_Detect_Release();
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

//==============================================================================
// PMIC Usage APIs
//==============================================================================
U32 get_MT6328_PMIC_chip_version (void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6328_SWCID),
                           (&val),
                           (U32)(MT6328_PMIC_SWCID_MASK),
                           (U32)(MT6328_PMIC_SWCID_SHIFT)
	                       );

    return val;
}

int pmic_detect_powerkey(void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6328_TOPSTATUS),
                           (&val),
                           (U32)(MT6328_PMIC_PWRKEY_DEB_MASK),
                           (U32)(MT6328_PMIC_PWRKEY_DEB_SHIFT)
	                       );

    if (val==1){
        print("pl pmic powerkey Release\n");
        return 0;
    }else{
        print("pl pmic powerkey Press\n");
        return 1;
    }
}

int pmic_detect_homekey(void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6328_TOPSTATUS),
                           (&val),
                           (U32)(MT6328_PMIC_HOMEKEY_DEB_MASK),
                           (U32)(MT6328_PMIC_HOMEKEY_DEB_SHIFT)
	                       );

    if (val==1){
        print("pl pmic FCHRKEY Release\n");
        return 0;
    }else{
        print("pl pmic FCHRKEY Press\n");
        return 1;
    }
}

U32 pmic_IsUsbCableIn (void)
{
    U32 ret=0;
    U32 val=0;

#if CFG_EVB_PLATFORM
    val = 1; // for bring up
    //print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);
#else
    ret=pmic_read_interface( (U32)(MT6328_CHR_CON0),
                           (&val),
                           (U32)(MT6328_PMIC_RGS_CHRDET_MASK),
                           (U32)(MT6328_PMIC_RGS_CHRDET_SHIFT)
	                       );
    print("[pmic_IsUsbCableIn] %d\n", val);
#endif

    if(val)
        return PMIC_CHRDET_EXIST;
    else
        return PMIC_CHRDET_NOT_EXIST;
}

static int vbat_status = PMIC_VBAT_NOT_DROP;
static void pmic_DetectVbatDrop (void)
{

	U32 ret=0;
	U32 just_rst=0;

	pmic_read_interface( MT6328_STRUP_CON9, (&just_rst), MT6328_PMIC_JUST_PWRKEY_RST_MASK, MT6328_PMIC_JUST_PWRKEY_RST_SHIFT );
	pmic_config_interface(MT6328_STRUP_CON9, 1, MT6328_PMIC_CLR_JUST_RST_MASK, MT6328_PMIC_CLR_JUST_RST_SHIFT);

	print("just_rst = %d\n", just_rst);
	if(just_rst)
		vbat_status = PMIC_VBAT_DROP;
	else
		vbat_status = PMIC_VBAT_NOT_DROP;

}

int pmic_IsVbatDrop(void)
{
   return vbat_status;
}


void hw_set_cc(int cc_val)
{
    //TBD
}

void mt6325_upmu_set_baton_tdet_en(U32 val)
{
    U32 ret=0;
    ret=pmic_config_interface( (U32)(MT6328_CHR_CON7),
                             (U32)(val),
                             (U32)(MT6328_PMIC_BATON_TDET_EN_MASK),
                             (U32)(MT6328_PMIC_BATON_TDET_EN_SHIFT)
	                         );
}

void mt6325_upmu_set_rg_baton_en(U32 val)
{
    U32 ret=0;
    ret=pmic_config_interface( (U32)(MT6328_CHR_CON7),
                             (U32)(val),
                             (U32)(MT6328_PMIC_RG_BATON_EN_MASK),
                             (U32)(MT6328_PMIC_RG_BATON_EN_SHIFT)
	                         );
}

U32 mt6325_upmu_get_rgs_baton_undet(void)
{
    U32 ret=0;
    U32 val=0;
    ret=pmic_read_interface( (U32)(MT6328_CHR_CON41),
                           (&val),
                           (U32)(MT6328_PMIC_RGS_BATON_UNDET_MASK),
                           (U32)(MT6328_PMIC_RGS_BATON_UNDET_SHIFT)
	                       );
   return val;
}

int hw_check_battery(void)
{
    #ifdef MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
        print("ignore bat check !\n");
        return 1;
    #else
        #if CFG_EVB_PLATFORM
            print("ignore bat check\n");
            return 1;
        #else
            U32 val=0;
			U32 ret_val;

			ret_val=pmic_config_interface( (U32)(MT6328_VTREF_CON0),
									 (U32)(1),
									 (U32)(MT6328_PMIC_RG_TREF_EN_MASK),
									 (U32)(MT6328_PMIC_RG_TREF_EN_SHIFT)
									 );

			ret_val=pmic_config_interface( (U32)(MT6328_VTREF_CON0),
									 (U32)(1),
									 (U32)(MT6328_PMIC_RG_TREF_ON_CTRL_MASK),
									 (U32)(MT6328_PMIC_RG_TREF_ON_CTRL_SHIFT)
									 );



            mt6325_upmu_set_baton_tdet_en(1);
            mt6325_upmu_set_rg_baton_en(1);
            val = mt6325_upmu_get_rgs_baton_undet();

            if(val==0)
            {
                print("bat is exist\n");
                return 1;
            }
            else
            {
                print("bat NOT exist\n");
                return 0;
            }
        #endif
    #endif
}

void PMIC_enable_long_press_reboot(void)
{
#if !CFG_FPGA_PLATFORM
#if !CFG_EVB_PLATFORM
#if KPD_PMIC_LPRST_TD!=0
	#if ONEKEY_REBOOT_NORMAL_MODE_PL
	pmic_config_interface(MT6328_TOP_RST_MISC, 0x01, MT6328_PMIC_RG_PWRKEY_RST_EN_MASK, MT6328_PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6328_TOP_RST_MISC, 0x00, MT6328_PMIC_RG_HOMEKEY_RST_EN_MASK, MT6328_PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6328_TOP_RST_MISC, (U32)KPD_PMIC_LPRST_TD, MT6328_PMIC_RG_PWRKEY_RST_TD_MASK, MT6328_PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#else
	pmic_config_interface(MT6328_TOP_RST_MISC, 0x01, MT6328_PMIC_RG_PWRKEY_RST_EN_MASK, MT6328_PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6328_TOP_RST_MISC, 0x01, MT6328_PMIC_RG_HOMEKEY_RST_EN_MASK, MT6328_PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6328_TOP_RST_MISC, (U32)KPD_PMIC_LPRST_TD, MT6328_PMIC_RG_PWRKEY_RST_TD_MASK, MT6328_PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#endif
#else
	pmic_config_interface(MT6328_TOP_RST_MISC, 0x00, MT6328_PMIC_RG_PWRKEY_RST_EN_MASK, MT6328_PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6328_TOP_RST_MISC, 0x00, MT6328_PMIC_RG_HOMEKEY_RST_EN_MASK, MT6328_PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
#endif
#else
	pmic_config_interface(MT6328_TOP_RST_MISC, 0x00, MT6328_PMIC_RG_PWRKEY_RST_EN_MASK, MT6328_PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(MT6328_TOP_RST_MISC, 0x00, MT6328_PMIC_RG_HOMEKEY_RST_EN_MASK, MT6328_PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
}

U32 PMIC_VUSB_EN(void)
{
	int ret=0;

	ret = pmic_config_interface( (kal_uint32)(MT6328_VUSB33_CON0),
			(kal_uint32)(1),
			(kal_uint32)(MT6328_PMIC_RG_VUSB33_EN_MASK),
			(kal_uint32)(MT6328_PMIC_RG_VUSB33_EN_SHIFT)
			);

	return ret;
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

U32 upmu_get_reg_value(kal_uint32 reg)
{
    U32 ret=0;
    U32 reg_val=0;

    ret=pmic_read_interface(reg, &reg_val, 0xFFFF, 0x0);

    return reg_val;
}



kal_uint16 pmic_read_efuse(kal_uint16 addr)
{
	kal_uint32 i, j;
	kal_uint32 ret, reg_val = 0;

	pmic_config_interface(MT6328_PMIC_RG_OTP_PA_ADDR, addr*2, MT6328_PMIC_RG_OTP_PA_MASK, MT6328_PMIC_RG_OTP_PA_SHIFT);
	udelay(100);
	ret=pmic_read_interface(MT6328_PMIC_RG_OTP_RD_TRIG_ADDR, &reg_val, MT6328_PMIC_RG_OTP_RD_TRIG_MASK, MT6328_PMIC_RG_OTP_RD_TRIG_SHIFT);

	if(reg_val==0)
	{
		pmic_config_interface(MT6328_PMIC_RG_OTP_RD_TRIG_ADDR, 1, MT6328_PMIC_RG_OTP_RD_TRIG_MASK, MT6328_PMIC_RG_OTP_RD_TRIG_SHIFT);
	}
	else
	{
		pmic_config_interface(MT6328_PMIC_RG_OTP_RD_TRIG_ADDR, 0, MT6328_PMIC_RG_OTP_RD_TRIG_MASK, MT6328_PMIC_RG_OTP_RD_TRIG_SHIFT);
	}

        udelay(100);
	do
	{
	ret=pmic_read_interface(MT6328_PMIC_RG_OTP_RD_BUSY_ADDR, &reg_val, MT6328_PMIC_RG_OTP_RD_BUSY_MASK, MT6328_PMIC_RG_OTP_RD_BUSY_SHIFT);
	}while(reg_val==1);
        udelay(1000);

	ret=pmic_read_interface(MT6328_PMIC_RG_OTP_DOUT_SW_ADDR, &reg_val, MT6328_PMIC_RG_OTP_DOUT_SW_MASK, MT6328_PMIC_RG_OTP_DOUT_SW_SHIFT);

	return reg_val;

}

/*
void pmic_6328_efuse_check(void)
{
    print("[0x%x]=0x%x\n", 0x0434, upmu_get_reg_value(0x0434));
    print("[0x%x]=0x%x\n", 0x0438, upmu_get_reg_value(0x0438));
    print("[0x%x]=0x%x\n", 0x0464, upmu_get_reg_value(0x0464));


    print("[0x%x]=0x%x\n", 0x0438, upmu_get_reg_value(0x0438));
    print("[0x%x]=0x%x\n", 0x046e, upmu_get_reg_value(0x046e));

    print("[0x%x]=0x%x\n", 0x0444, upmu_get_reg_value(0x0444));
    print("[0x%x]=0x%x\n", 0x0458, upmu_get_reg_value(0x0458));
    print("[0x%x]=0x%x\n", 0x044e, upmu_get_reg_value(0x044e));


    print("[0x%x]=0x%x\n", 0x0a52, upmu_get_reg_value(0x0a52));
    print("[0x%x]=0x%x\n", 0x0a56, upmu_get_reg_value(0x0a56));
    print("[0x%x]=0x%x\n", 0x0a58, upmu_get_reg_value(0x0a58));

    print("[0x%x]=0x%x\n", 0x0a7c, upmu_get_reg_value(0x0a7c));
    print("[0x%x]=0x%x\n", 0x0a7e, upmu_get_reg_value(0x0a7e));



    print("[0x%x]=0x%x\n", 0x0a60, upmu_get_reg_value(0x0a60));
    print("[0x%x]=0x%x\n", 0x0a62, upmu_get_reg_value(0x0a62));
    print("[0x%x]=0x%x\n", 0x0a66, upmu_get_reg_value(0x0a66));
    print("[0x%x]=0x%x\n", 0x0a64, upmu_get_reg_value(0x0a64));

    print("[0x%x]=0x%x\n", 0x0a72, upmu_get_reg_value(0x0a72));
    print("[0x%x]=0x%x\n", 0x0a84, upmu_get_reg_value(0x0a84));
    print("[0x%x]=0x%x\n", 0x0a7a, upmu_get_reg_value(0x0a7a));
    print("[0x%x]=0x%x\n", 0x0a5c, upmu_get_reg_value(0x0a5c));
    print("[0x%x]=0x%x\n", 0x0a6a, upmu_get_reg_value(0x0a6a));
    print("[0x%x]=0x%x\n", 0x0a6c, upmu_get_reg_value(0x0a6c));

    print("[0x%x]=0x%x\n", 0x043e, upmu_get_reg_value(0x043e));

    print("[0x%x]=0x%x\n", 0x0470, upmu_get_reg_value(0x0470));
    print("[0x%x]=0x%x\n", 0x046c, upmu_get_reg_value(0x046c));
    print("[0x%x]=0x%x\n", 0x0466, upmu_get_reg_value(0x0466));
    print("[0x%x]=0x%x\n", 0x0442, upmu_get_reg_value(0x0442));

    print("[0x%x]=0x%x\n", 0x045a, upmu_get_reg_value(0x045a));
    print("[0x%x]=0x%x\n", 0x0456, upmu_get_reg_value(0x0456));

    print("[0x%x]=0x%x\n", 0x0450, upmu_get_reg_value(0x0450));
    print("[0x%x]=0x%x\n", 0x044c, upmu_get_reg_value(0x044c));

}
*/


U32 efuse_data[0x20]={0};

void pmic_6328_efuse_management(void)
{

    int i=0;
    int is_efuse_trimed=0;
    u16 status = 0;
    u32 data32 = 0, data32_chk = 0, data32_448_org = 0;
    u32 data32_472_org = 0, data32_448 = 0, data32_472 = 0;

    is_efuse_trimed = ((upmu_get_reg_value(0xC5C))>>15)&0x0001;

    /*print("[6328] is_efuse_trimed=0x%x,[0x%x]=0x%x\n", is_efuse_trimed, 0xC5C, upmu_get_reg_value(0xC5C));*/

    if(is_efuse_trimed == 1)
    {
			//get efuse data
			//turn on efuse clock
			pmic_config_interface(MT6328_PMIC_RG_EFUSE_CK_PDN_HWEN_ADDR, 0x00, MT6328_PMIC_RG_EFUSE_CK_PDN_HWEN_MASK, MT6328_PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
			pmic_config_interface(MT6328_PMIC_RG_EFUSE_CK_PDN_ADDR, 0x00, MT6328_PMIC_RG_EFUSE_CK_PDN_MASK, MT6328_PMIC_RG_EFUSE_CK_PDN_SHIFT);
			pmic_config_interface(MT6328_PMIC_RG_OTP_RD_SW_ADDR, 0x01, MT6328_PMIC_RG_OTP_RD_SW_MASK, MT6328_PMIC_RG_OTP_RD_SW_SHIFT);
			
			for(i=0;i<=0x1f;i++)
			{
				efuse_data[i]=pmic_read_efuse(i);
			}
			
			//dump efuse data for check
			for(i=0x0;i<=0x1f;i++)
			    print("[6328]e-data[0x%x]=0x%x\n", i, efuse_data[i]);
			
			
			//print("Before apply pmic efuse\n");
			//pmic_6328_efuse_check();
			
			//------------------------------------------
	pmic_config_interface(0x0434,((efuse_data[0] >>0 )&0x0001),0x1,4);
	pmic_config_interface(0x0434,((efuse_data[0] >>1 )&0x0001),0x1,5);
	pmic_config_interface(0x0434,((efuse_data[0] >>2 )&0x0001),0x1,6);
	pmic_config_interface(0x0434,((efuse_data[0] >>3 )&0x0001),0x1,7);
	pmic_config_interface(0x0434,((efuse_data[0] >>4 )&0x0001),0x1,8);
	pmic_config_interface(0x0434,((efuse_data[0] >>5 )&0x0001),0x1,9);
	pmic_config_interface(0x0434,((efuse_data[0] >>6 )&0x0001),0x1,10);
	pmic_config_interface(0x0434,((efuse_data[0] >>7 )&0x0001),0x1,11);
	pmic_config_interface(0x0434,((efuse_data[0] >>8 )&0x0001),0x1,12);
	pmic_config_interface(0x0434,((efuse_data[0] >>9 )&0x0001),0x1,13);
	pmic_config_interface(0x0438,((efuse_data[0] >>10 )&0x0001),0x1,0);
	pmic_config_interface(0x0438,((efuse_data[0] >>11 )&0x0001),0x1,1);
	pmic_config_interface(0x0438,((efuse_data[0] >>12 )&0x0001),0x1,2);
	pmic_config_interface(0x0438,((efuse_data[0] >>13 )&0x0001),0x1,3);
	pmic_config_interface(0x0438,((efuse_data[0] >>14 )&0x0001),0x1,4);
	pmic_config_interface(0x0464,((efuse_data[0] >>15 )&0x0001),0x1,0);
	pmic_config_interface(0x0464,((efuse_data[1] >>0 )&0x0001),0x1,1);
	pmic_config_interface(0x0464,((efuse_data[1] >>1 )&0x0001),0x1,2);
	pmic_config_interface(0x0464,((efuse_data[1] >>2 )&0x0001),0x1,3);
	pmic_config_interface(0x0464,((efuse_data[1] >>3 )&0x0001),0x1,4);
	pmic_config_interface(0x0464,((efuse_data[1] >>4 )&0x0001),0x1,5);
	pmic_config_interface(0x0438,((efuse_data[1] >>5 )&0x0001),0x1,11);
	pmic_config_interface(0x0438,((efuse_data[1] >>6 )&0x0001),0x1,12);
	pmic_config_interface(0x0438,((efuse_data[1] >>7 )&0x0001),0x1,13);
	pmic_config_interface(0x0438,((efuse_data[1] >>8 )&0x0001),0x1,8);
	pmic_config_interface(0x0438,((efuse_data[1] >>9 )&0x0001),0x1,9);
	pmic_config_interface(0x0438,((efuse_data[1] >>10 )&0x0001),0x1,10);
	pmic_config_interface(0x0438,((efuse_data[1] >>11 )&0x0001),0x1,5);
	pmic_config_interface(0x0438,((efuse_data[1] >>12 )&0x0001),0x1,6);
	pmic_config_interface(0x0438,((efuse_data[1] >>13 )&0x0001),0x1,7);
	pmic_config_interface(0x046E,((efuse_data[1] >>14 )&0x0001),0x1,6);
	pmic_config_interface(0x046E,((efuse_data[1] >>15 )&0x0001),0x1,7);
	pmic_config_interface(0x046E,((efuse_data[2] >>0 )&0x0001),0x1,8);
	pmic_config_interface(0x046E,((efuse_data[2] >>1 )&0x0001),0x1,9);
	pmic_config_interface(0x046E,((efuse_data[2] >>2 )&0x0001),0x1,10);
	pmic_config_interface(0x046E,((efuse_data[2] >>3 )&0x0001),0x1,11);
	pmic_config_interface(0x046E,((efuse_data[2] >>4 )&0x0001),0x1,0);
	pmic_config_interface(0x046E,((efuse_data[2] >>5 )&0x0001),0x1,1);
	pmic_config_interface(0x046E,((efuse_data[2] >>6 )&0x0001),0x1,2);
	pmic_config_interface(0x046E,((efuse_data[2] >>7 )&0x0001),0x1,3);
	pmic_config_interface(0x046E,((efuse_data[2] >>8 )&0x0001),0x1,4);
	pmic_config_interface(0x046E,((efuse_data[2] >>9 )&0x0001),0x1,5);
	pmic_config_interface(0x0444,((efuse_data[2] >>10 )&0x0001),0x1,6);
	pmic_config_interface(0x0444,((efuse_data[2] >>11 )&0x0001),0x1,7);
	pmic_config_interface(0x0444,((efuse_data[2] >>12 )&0x0001),0x1,8);
	pmic_config_interface(0x0444,((efuse_data[2] >>13 )&0x0001),0x1,9);
	pmic_config_interface(0x0444,((efuse_data[2] >>14 )&0x0001),0x1,10);
	pmic_config_interface(0x0444,((efuse_data[2] >>15 )&0x0001),0x1,11);
	pmic_config_interface(0x0444,((efuse_data[3] >>0 )&0x0001),0x1,0);
	pmic_config_interface(0x0444,((efuse_data[3] >>1 )&0x0001),0x1,1);
	pmic_config_interface(0x0444,((efuse_data[3] >>2 )&0x0001),0x1,2);
	pmic_config_interface(0x0444,((efuse_data[3] >>3 )&0x0001),0x1,3);
	pmic_config_interface(0x0444,((efuse_data[3] >>4 )&0x0001),0x1,4);
	pmic_config_interface(0x0444,((efuse_data[3] >>5 )&0x0001),0x1,5);
	pmic_config_interface(0x0458,((efuse_data[3] >>6 )&0x0001),0x1,7);
	pmic_config_interface(0x0458,((efuse_data[3] >>7 )&0x0001),0x1,8);
	pmic_config_interface(0x0458,((efuse_data[3] >>8 )&0x0001),0x1,9);
	pmic_config_interface(0x0458,((efuse_data[3] >>9 )&0x0001),0x1,10);
	pmic_config_interface(0x0458,((efuse_data[3] >>10 )&0x0001),0x1,11);
	pmic_config_interface(0x0458,((efuse_data[3] >>11 )&0x0001),0x1,6);
	pmic_config_interface(0x0458,((efuse_data[3] >>12 )&0x0001),0x1,1);
	pmic_config_interface(0x0458,((efuse_data[3] >>13 )&0x0001),0x1,2);
	pmic_config_interface(0x0458,((efuse_data[3] >>14 )&0x0001),0x1,3);
	pmic_config_interface(0x0458,((efuse_data[3] >>15 )&0x0001),0x1,4);
	pmic_config_interface(0x0458,((efuse_data[4] >>0 )&0x0001),0x1,5);
	pmic_config_interface(0x0458,((efuse_data[4] >>1 )&0x0001),0x1,0);
	pmic_config_interface(0x044E,((efuse_data[4] >>2 )&0x0001),0x1,6);
	pmic_config_interface(0x044E,((efuse_data[4] >>3 )&0x0001),0x1,7);
	pmic_config_interface(0x044E,((efuse_data[4] >>4 )&0x0001),0x1,8);
	pmic_config_interface(0x044E,((efuse_data[4] >>5 )&0x0001),0x1,9);
	pmic_config_interface(0x044E,((efuse_data[4] >>6 )&0x0001),0x1,10);
	pmic_config_interface(0x044E,((efuse_data[4] >>7 )&0x0001),0x1,11);
	pmic_config_interface(0x044E,((efuse_data[4] >>8 )&0x0001),0x1,0);
	pmic_config_interface(0x044E,((efuse_data[4] >>9 )&0x0001),0x1,1);
	pmic_config_interface(0x044E,((efuse_data[4] >>10 )&0x0001),0x1,2);
	pmic_config_interface(0x044E,((efuse_data[4] >>11 )&0x0001),0x1,3);
	pmic_config_interface(0x044E,((efuse_data[4] >>12 )&0x0001),0x1,4);
	pmic_config_interface(0x044E,((efuse_data[4] >>13 )&0x0001),0x1,5);
	pmic_config_interface(0x0A52,((efuse_data[4] >>14 )&0x0001),0x1,8);
	pmic_config_interface(0x0A52,((efuse_data[4] >>15 )&0x0001),0x1,9);
	pmic_config_interface(0x0A52,((efuse_data[5] >>0 )&0x0001),0x1,10);
	pmic_config_interface(0x0A52,((efuse_data[5] >>1 )&0x0001),0x1,11);
	pmic_config_interface(0x0A56,((efuse_data[5] >>2 )&0x0001),0x1,8);
	pmic_config_interface(0x0A56,((efuse_data[5] >>3 )&0x0001),0x1,9);
	pmic_config_interface(0x0A56,((efuse_data[5] >>4 )&0x0001),0x1,10);
	pmic_config_interface(0x0A56,((efuse_data[5] >>5 )&0x0001),0x1,11);
	pmic_config_interface(0x0A58,((efuse_data[5] >>6 )&0x0001),0x1,8);
	pmic_config_interface(0x0A58,((efuse_data[5] >>7 )&0x0001),0x1,9);
	pmic_config_interface(0x0A58,((efuse_data[5] >>8 )&0x0001),0x1,10);
	pmic_config_interface(0x0A58,((efuse_data[5] >>9 )&0x0001),0x1,11);
	pmic_config_interface(0x0A7C,((efuse_data[5] >>10 )&0x0001),0x1,8);
	pmic_config_interface(0x0A7C,((efuse_data[5] >>11 )&0x0001),0x1,9);
	pmic_config_interface(0x0A7C,((efuse_data[5] >>12 )&0x0001),0x1,10);
	pmic_config_interface(0x0A7C,((efuse_data[5] >>13 )&0x0001),0x1,11);
	pmic_config_interface(0x0A7E,((efuse_data[5] >>14 )&0x0001),0x1,8);
	pmic_config_interface(0x0A7E,((efuse_data[5] >>15 )&0x0001),0x1,9);
	pmic_config_interface(0x0A7E,((efuse_data[6] >>0 )&0x0001),0x1,10);
	pmic_config_interface(0x0A7E,((efuse_data[6] >>1 )&0x0001),0x1,11);
	pmic_config_interface(0x0A60,((efuse_data[6] >>2 )&0x0001),0x1,8);
	pmic_config_interface(0x0A60,((efuse_data[6] >>3 )&0x0001),0x1,9);
	pmic_config_interface(0x0A60,((efuse_data[6] >>4 )&0x0001),0x1,10);
	pmic_config_interface(0x0A60,((efuse_data[6] >>5 )&0x0001),0x1,11);
	pmic_config_interface(0x0A62,((efuse_data[6] >>6 )&0x0001),0x1,8);
	pmic_config_interface(0x0A62,((efuse_data[6] >>7 )&0x0001),0x1,9);
	pmic_config_interface(0x0A62,((efuse_data[6] >>8 )&0x0001),0x1,10);
	pmic_config_interface(0x0A62,((efuse_data[6] >>9 )&0x0001),0x1,11);
	pmic_config_interface(0x0A66,((efuse_data[6] >>10 )&0x0001),0x1,8);
	pmic_config_interface(0x0A66,((efuse_data[6] >>11 )&0x0001),0x1,9);
	pmic_config_interface(0x0A66,((efuse_data[6] >>12 )&0x0001),0x1,10);
	pmic_config_interface(0x0A66,((efuse_data[6] >>13 )&0x0001),0x1,11);
	pmic_config_interface(0x0A64,((efuse_data[6] >>14 )&0x0001),0x1,8);
	pmic_config_interface(0x0A64,((efuse_data[6] >>15 )&0x0001),0x1,9);
	pmic_config_interface(0x0A64,((efuse_data[7] >>0 )&0x0001),0x1,10);
	pmic_config_interface(0x0A64,((efuse_data[7] >>1 )&0x0001),0x1,11);
	pmic_config_interface(0x0A72,((efuse_data[7] >>2 )&0x0001),0x1,8);
	pmic_config_interface(0x0A72,((efuse_data[7] >>3 )&0x0001),0x1,9);
	pmic_config_interface(0x0A72,((efuse_data[7] >>4 )&0x0001),0x1,10);
	pmic_config_interface(0x0A72,((efuse_data[7] >>5 )&0x0001),0x1,11);
	pmic_config_interface(0x0A84,((efuse_data[7] >>6 )&0x0001),0x1,8);
	pmic_config_interface(0x0A84,((efuse_data[7] >>7 )&0x0001),0x1,9);
	pmic_config_interface(0x0A84,((efuse_data[7] >>8 )&0x0001),0x1,10);
	pmic_config_interface(0x0A84,((efuse_data[7] >>9 )&0x0001),0x1,11);
	pmic_config_interface(0x0A7A,((efuse_data[7] >>10 )&0x0001),0x1,8);
	pmic_config_interface(0x0A7A,((efuse_data[7] >>11 )&0x0001),0x1,9);
	pmic_config_interface(0x0A7A,((efuse_data[7] >>12 )&0x0001),0x1,10);
	pmic_config_interface(0x0A7A,((efuse_data[7] >>13 )&0x0001),0x1,11);
	pmic_config_interface(0x0A5C,((efuse_data[7] >>14 )&0x0001),0x1,9);
	pmic_config_interface(0x0A5C,((efuse_data[7] >>15 )&0x0001),0x1,10);
	pmic_config_interface(0x0A5C,((efuse_data[8] >>0 )&0x0001),0x1,11);
	pmic_config_interface(0x0A5C,((efuse_data[8] >>1 )&0x0001),0x1,12);
	pmic_config_interface(0x0A6A,((efuse_data[8] >>2 )&0x0001),0x1,8);
	pmic_config_interface(0x0A6A,((efuse_data[8] >>3 )&0x0001),0x1,9);
	pmic_config_interface(0x0A6A,((efuse_data[8] >>4 )&0x0001),0x1,10);
	pmic_config_interface(0x0A6A,((efuse_data[8] >>5 )&0x0001),0x1,11);
	pmic_config_interface(0x0A6C,((efuse_data[8] >>6 )&0x0001),0x1,8);
	pmic_config_interface(0x0A6C,((efuse_data[8] >>7 )&0x0001),0x1,9);
	pmic_config_interface(0x0A6C,((efuse_data[8] >>8 )&0x0001),0x1,10);
	pmic_config_interface(0x0A6C,((efuse_data[8] >>9 )&0x0001),0x1,11);
	pmic_config_interface(0x043E,((efuse_data[8] >>10 )&0x0001),0x1,10);
	pmic_config_interface(0x043E,((efuse_data[8] >>11 )&0x0001),0x1,11);
	pmic_config_interface(0x043E,((efuse_data[8] >>12 )&0x0001),0x1,12);


	pmic_config_interface(0x0470,((efuse_data[12] >>7 )&0x0001),0x1,6);
	pmic_config_interface(0x0470,((efuse_data[12] >>8 )&0x0001),0x1,7);
	pmic_config_interface(0x046C,((efuse_data[12] >>9 )&0x0001),0x1,3);
	pmic_config_interface(0x046C,((efuse_data[12] >>10 )&0x0001),0x1,4);
	pmic_config_interface(0x0466,((efuse_data[12] >>11 )&0x0001),0x1,6);
	pmic_config_interface(0x0466,((efuse_data[12] >>12 )&0x0001),0x1,7);
	pmic_config_interface(0x0442,((efuse_data[12] >>13 )&0x0001),0x1,3);
	pmic_config_interface(0x0442,((efuse_data[12] >>14 )&0x0001),0x1,4);
	pmic_config_interface(0x045A,((efuse_data[12] >>15 )&0x0001),0x1,6);
	pmic_config_interface(0x045A,((efuse_data[13] >>0 )&0x0001),0x1,7);
	pmic_config_interface(0x0456,((efuse_data[13] >>1 )&0x0001),0x1,3);
	pmic_config_interface(0x0456,((efuse_data[13] >>2 )&0x0001),0x1,4);
	pmic_config_interface(0x0450,((efuse_data[13] >>3 )&0x0001),0x1,6);
	pmic_config_interface(0x0450,((efuse_data[13] >>4 )&0x0001),0x1,7);
	pmic_config_interface(0x044C,((efuse_data[13] >>5 )&0x0001),0x1,7);
	pmic_config_interface(0x044C,((efuse_data[13] >>6 )&0x0001),0x1,8);

		efuse_data[0x11]=pmic_read_efuse(0x11);
		/* check bit 784 if equal to zero */
		if ((efuse_data[0x11] & 0x1) == 0)
		{
			/* read ovp original trim value */
			status = (u16)pmic_read_interface(MT6328_PMIC_RG_OVP_TRIM_ADDR, &data32, 0xffff, 0);
			/* remap to new value and write back */
			pmic_config_interface(MT6328_PMIC_RG_OVP_TRIM_ADDR,mt6328_ovp_trim[data32], MT6328_PMIC_RG_OVP_TRIM_MASK,MT6328_PMIC_RG_OVP_TRIM_SHIFT);
			/* read ovp original trim value */
			status = (u16)pmic_read_interface(MT6328_PMIC_RG_OVP_TRIM_ADDR, &data32_chk, 0xffff, 0);
			print("[pmic_6328_efuse_management]4.27 org_ovp_trim:0x%x ovp_trim[0x%x]:0x%x new_ovp_trim:0x%x\r\n", data32, MT6328_PMIC_RG_OVP_TRIM_ADDR, mt6328_ovp_trim[data32], data32_chk);
		}
		else
			print("[pmic_6328_efuse_management]4.27 efuse_data[0x11]:0x%x\r\n", efuse_data[0x11]);
	
		if ((efuse_data[0x11] & 0x2) == 0x2)
		{
			/* read 448 value */
			status = (u16)pmic_read_interface(0x0448, &data32_448_org, 0xffff, 0);
			pmic_config_interface(0x0448,0,0x1,0);
			status = (u16)pmic_read_interface(0x0448, &data32_448, 0xffff, 0);
			/* read 472 value */
			status = (u16)pmic_read_interface(0x0472, &data32_472_org, 0xffff, 0);
			pmic_config_interface(0x0472,0,0x1,0);
			status = (u16)pmic_read_interface(0x0472, &data32_472, 0xffff, 0);
			print("[pmic_6328_efuse_management]4.27 data32_448_org:0x%x data32_472_org:0x%x\r\n", data32_448_org, data32_472_org);
			print("[pmic_6328_efuse_management]4.27 data32_448:0x%x data32_472:0x%x\r\n", data32_448, data32_472);
	
		}
		else
			print("[pmic_6328_efuse_management]4.27 efuse_data[0x11][448:472]:0x%x\r\n", efuse_data[0x11]);

        //------------------------------------------

        //print("After apply pmic efuse\n");
        //pmic_6328_efuse_check();

		//turn off efuse clock
		pmic_config_interface(MT6328_PMIC_RG_EFUSE_CK_PDN_HWEN_ADDR, 0x01, MT6328_PMIC_RG_EFUSE_CK_PDN_HWEN_MASK, MT6328_PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
		pmic_config_interface(MT6328_PMIC_RG_EFUSE_CK_PDN_ADDR, 0x01, MT6328_PMIC_RG_EFUSE_CK_PDN_MASK, MT6328_PMIC_RG_EFUSE_CK_PDN_SHIFT);

    }
}

const static unsigned char mt6328_VIO_1_84[] = {
	14,	15,	0,	1,	2,	3,	4,	5,	8,	8,	8,	9,	10,	11,	12,	13
};

static unsigned char vio18_cal;

void upmu_set_rg_vio18_184(void)
{
	/*print("[upmu_set_rg_vio18_184] old cal=%d new cal=%d.\r\n", vio18_cal,mt6328_VIO_1_84[vio18_cal]);*/
	pmic_config_interface(MT6328_PMIC_RG_VIO18_CAL_ADDR, mt6328_VIO_1_84[vio18_cal], MT6328_PMIC_RG_VIO18_CAL_MASK, MT6328_PMIC_RG_VIO18_CAL_SHIFT);
    }

/*
const static unsigned char mt6328_VMC_1_86[] = {
	13,	14,	15,	0,	1,	2,	3,	4,	5,	8,	8,	9,	10,	11,	12,	13
};


static unsigned char vmc_cal;

void upmu_set_rg_vmc_186(void)
{
	print("[upmu_set_rg_vio18_184] old cal=%d new cal=%d.\r\n", vmc_cal,mt6328_VMC_1_86[vmc_cal]);
	pmic_config_interface(MT6328_PMIC_RG_VMC_CAL_ADDR, mt6328_VMC_1_86[vmc_cal], MT6328_PMIC_RG_VMC_CAL_MASK, MT6328_PMIC_RG_VMC_CAL_SHIFT);
}
*/


void pmic_disable_usbdl_wo_battery(void)
{
    int ret_val = 0;
    /*print("[pmic_init] turn off usbdl wo battery..................\n");*/
    ret_val=pmic_config_interface(MT6328_PMIC_RG_ULC_DET_EN_ADDR,1,MT6328_PMIC_RG_ULC_DET_EN_MASK,MT6328_PMIC_RG_ULC_DET_EN_SHIFT);
    pmic_config_interface(MT6328_PMIC_RG_USBDL_SET_ADDR, 0x0000, MT6328_PMIC_RG_USBDL_SET_MASK, MT6328_PMIC_RG_USBDL_SET_SHIFT); //[1]=0, RG_WDTRSTB_MODE
    pmic_config_interface(MT6328_PMIC_RG_USBDL_RST_ADDR, 0x0001, MT6328_PMIC_RG_USBDL_RST_MASK, MT6328_PMIC_RG_USBDL_RST_SHIFT); //[0]=1, RG_WDTRSTB_EN

}

#if !CFG_EVB_PLATFORM || defined(MTK_EFUSE_WRITER_SUPPORT)
kal_int32 count_time_out=10;
#define VOLTAGE_FULL_RANGE     	1800
#define ADC_PRECISE		32768 	// 15 bits

kal_uint32 PMIC_IMM_GetOneChannelValue(kal_uint8 dwChannel, int deCount, int trimd)
{
	kal_int32 ret=0;
	kal_int32 ret_data;
	kal_int32 r_val_temp=0;
	kal_int32 adc_result=0;
	int count=0;
	kal_uint32 busy;
	/*
	CH0: BATSNS
	CH1: ISENSE
	CH2: VCDT
	CH3: BAT ON
	CH4: PMIC TEMP
	CH5: ACCDET
	CH6:
	CH7: TSX
	CH8:
	CH9:
	CH10:
	CH11:
	CH12:
	CH13:
	CH14:
	CH15:
	BATSNS 3v-4.5v
	ISENSE 1.5-4.5v
	BATON  0-1.8v
	VCDT   4v-14v
	ACCDET 1.8v
	GPS    1.8v

	*/

	if(dwChannel>15)
		return -1;

	upmu_set_reg_value(0x0a44,0x010a);
	upmu_set_reg_value(0x0cec,0x0000);
	upmu_set_reg_value(0x0d00,0x0010);
	upmu_set_reg_value(0x0f14,0x1290);


	//ret=pmic_config_interface(MT6328_TOP_CLKSQ_SET,(1<<2),0xffff,0);
	ret=pmic_config_interface(MT6328_AUXADC_RQST0_SET,(1<<dwChannel),0xffff,0);

	pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_RDY_CH0_BY_AP_ADDR,MT6328_PMIC_AUXADC_ADC_RDY_CH0_BY_AP_MASK,MT6328_PMIC_AUXADC_ADC_RDY_CH0_BY_AP_SHIFT);
	busy=upmu_get_reg_value(0E84);
	udelay(50);

	switch(dwChannel){
	case 0:
		while(pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_RDY_CH0_BY_AP_ADDR,MT6328_PMIC_AUXADC_ADC_RDY_CH0_BY_AP_MASK,MT6328_PMIC_AUXADC_ADC_RDY_CH0_BY_AP_SHIFT) != 1 )
		{
			mdelay(1);
			if( (count++) > count_time_out)
			{
			/*print( "[IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);*/
			break;
			}
		}
		//ret_data = mt6328_get_register_value(PMIC_AUXADC_ADC_OUT_CH0_BY_AP);
		ret_data=pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_OUT_CH0_BY_AP_ADDR,MT6328_PMIC_AUXADC_ADC_OUT_CH0_BY_AP_MASK,MT6328_PMIC_AUXADC_ADC_OUT_CH0_BY_AP_SHIFT);
	break;
	case 1:
		while(pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_RDY_CH1_BY_AP_ADDR,MT6328_PMIC_AUXADC_ADC_RDY_CH1_BY_AP_MASK,MT6328_PMIC_AUXADC_ADC_RDY_CH1_BY_AP_SHIFT) != 1 )
		{
			mdelay(1);
			if( (count++) > count_time_out)
			{
			//print( "[IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
			break;
			}
		}
		ret_data = pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_OUT_CH1_BY_AP_ADDR,MT6328_PMIC_AUXADC_ADC_OUT_CH1_BY_AP_MASK,MT6328_PMIC_AUXADC_ADC_OUT_CH1_BY_AP_SHIFT);
	break;
	case 2:
		while(pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_RDY_CH2_ADDR,MT6328_PMIC_AUXADC_ADC_RDY_CH2_MASK,MT6328_PMIC_AUXADC_ADC_RDY_CH2_SHIFT) != 1 )
		{
			mdelay(1);
			if( (count++) > count_time_out)
			{
			//print( "[IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
			break;
			}
		}

		ret_data = pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_OUT_CH2_ADDR,MT6328_PMIC_AUXADC_ADC_OUT_CH2_MASK,MT6328_PMIC_AUXADC_ADC_OUT_CH2_SHIFT);
	break;
	case 3:
		while(pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_RDY_CH3_ADDR,MT6328_PMIC_AUXADC_ADC_RDY_CH3_MASK,MT6328_PMIC_AUXADC_ADC_RDY_CH3_SHIFT) != 1 )
		{
			mdelay(1);
			if( (count++) > count_time_out)
			{
			//print( "[IMM_GetOneChannelValue_PMIC] (%d) Time out!\n", dwChannel);
			break;
			}
		}
		ret_data = pmic_get_register_value(MT6328_PMIC_AUXADC_ADC_OUT_CH3_ADDR,MT6328_PMIC_AUXADC_ADC_OUT_CH3_MASK,MT6328_PMIC_AUXADC_ADC_OUT_CH3_SHIFT);
	break;


	default:
		//print( "[AUXADC] Invalid channel value(%d,%d)\n", dwChannel, trimd);

	return -1;
	break;
	}

    switch(dwChannel){
        case 0:
            r_val_temp = 3;
            adc_result = (ret_data*r_val_temp*VOLTAGE_FULL_RANGE)/32768;
            break;
        case 1:
            r_val_temp = 3;
            adc_result = (ret_data*r_val_temp*VOLTAGE_FULL_RANGE)/32768;
            break;
        case 2:
            r_val_temp = 1;
            adc_result = (ret_data*r_val_temp*VOLTAGE_FULL_RANGE)/4096;
            break;
        case 3:
            r_val_temp = 1;
            adc_result = (ret_data*r_val_temp*VOLTAGE_FULL_RANGE)/4096;
            break;

        default:
            //print( "[AUXADC] Invalid channel value(%d,%d)\n", dwChannel, trimd);

            return -1;
            break;
    }



       //print( "[AUXADC] ch=%d raw=%d data=%d \n", dwChannel, ret_data,adc_result);

	//return ret_data;
	return adc_result;

}

int get_bat_sense_volt(int times)
{
	return PMIC_IMM_GetOneChannelValue(0,times,1);
}
#endif


#if !CFG_EVB_PLATFORM
int get_i_sense_volt(int times)
{
	return PMIC_IMM_GetOneChannelValue(1,times,1);
}

#define R_CHARGER_1 330
#define R_CHARGER_2 39


int get_charger_volt(int times)
{
	kal_int32 val;
	val=PMIC_IMM_GetOneChannelValue(2,times,1);
	val = (((R_CHARGER_1+R_CHARGER_2)*100*val)/R_CHARGER_2)/100;
	return val;
}


kal_uint32 upmu_is_chr_det(void)
{
  kal_uint32 ret=0;
  kal_uint32 val=0;


  ret=pmic_read_interface( (kal_uint32)(MT6328_CHR_CON0),
                           (&val),
                           (kal_uint32)(MT6328_PMIC_RGS_CHRDET_MASK),
                           (kal_uint32)(MT6328_PMIC_RGS_CHRDET_SHIFT)
	                       );


  return val;
}


void kick_charger_wdt(void)
{
	pmic_config_interface(MT6328_PMIC_RG_CHRWDT_TD_ADDR,3,MT6328_PMIC_RG_CHRWDT_TD_MASK,MT6328_PMIC_RG_CHRWDT_TD_SHIFT);
	pmic_config_interface(MT6328_PMIC_RG_CHRWDT_WR_ADDR,1,MT6328_PMIC_RG_CHRWDT_WR_MASK,MT6328_PMIC_RG_CHRWDT_WR_SHIFT);
	pmic_config_interface(MT6328_PMIC_RG_CHRWDT_INT_EN_ADDR,1,MT6328_PMIC_RG_CHRWDT_INT_EN_MASK,MT6328_PMIC_RG_CHRWDT_INT_EN_SHIFT);
	pmic_config_interface(MT6328_PMIC_RG_CHRWDT_EN_ADDR,1,MT6328_PMIC_RG_CHRWDT_EN_MASK,MT6328_PMIC_RG_CHRWDT_EN_SHIFT);
	pmic_config_interface(MT6328_PMIC_RG_CHRWDT_FLAG_WR_ADDR,1,MT6328_PMIC_RG_CHRWDT_FLAG_WR_MASK,MT6328_PMIC_RG_CHRWDT_FLAG_WR_SHIFT);

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
	pmic_config_interface(MT6328_PMIC_RG_USBDL_RST_ADDR, 1, MT6328_PMIC_RG_USBDL_RST_MASK, MT6328_PMIC_RG_USBDL_RST_SHIFT);
	//pmic_set_register_value(PMIC_RG_USBDL_RST,1);//force leave USBDL mode
	//mt6325_upmu_set_rg_usbdl_rst(1);       //force leave USBDL mode
	pmic_config_interface(MT6328_PMIC_RG_BC11_RST_ADDR, 1, MT6328_PMIC_RG_BC11_RST_MASK, MT6328_PMIC_RG_BC11_RST_SHIFT);

	kick_charger_wdt();

	pmic_config_interface(MT6328_PMIC_RG_CS_VTH_ADDR,0xc,MT6328_PMIC_RG_CS_VTH_MASK,MT6328_PMIC_RG_CS_VTH_SHIFT);
	//pmic_set_register_value(PMIC_RG_CS_VTH,0xC);	// CS_VTH, 450mA
	//mt6325_upmu_set_rg_cs_vth(0xC);             // CS_VTH, 450mA

	pmic_config_interface(MT6328_PMIC_RG_CSDAC_EN_ADDR,1,MT6328_PMIC_RG_CSDAC_EN_MASK,MT6328_PMIC_RG_CSDAC_EN_SHIFT);
	//pmic_set_register_value(PMIC_RG_CSDAC_EN,1);
	//mt6325_upmu_set_rg_csdac_en(1);				// CSDAC_EN

	pmic_config_interface(MT6328_PMIC_RG_CHR_EN_ADDR,1,MT6328_PMIC_RG_CHR_EN_MASK,MT6328_PMIC_RG_CHR_EN_SHIFT);
	//pmic_set_register_value(PMIC_RG_CHR_EN,1);
	//mt6325_upmu_set_rg_chr_en(1);				// CHR_EN

        pmic_config_interface(MT6328_PMIC_RG_CSDAC_MODE_ADDR,1,MT6328_PMIC_RG_CSDAC_MODE_MASK,MT6328_PMIC_RG_CSDAC_MODE_SHIFT);
        pmic_config_interface(MT6328_PMIC_RG_CSDAC_EN_ADDR,1,MT6328_PMIC_RG_CSDAC_EN_MASK,MT6328_PMIC_RG_CSDAC_EN_SHIFT);
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
    print( "VBAT=%d mV with %d mV, VCHR %d mV ,VCHR_HV=%d \n", bat_val, BATTERY_LOWVOL_THRESOLD,chr_volt,V_CHARGER_MAX);


    while (bat_val < BATTERY_LOWVOL_THRESOLD)
    {
        mtk_wdt_restart();
        if(upmu_is_chr_det() == KAL_FALSE)
        {
            //print( "[PL][BATTERY] No Charger, Power OFF !\n");
            break;
        }

		chr_volt= get_charger_volt(1);
		if(chr_volt>V_CHARGER_MAX)
		{
            //print( "[PL][BATTERY] charger voltage is too high :%d , threshold is %d !\n",chr_volt,V_CHARGER_MAX);
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
				//print( "[PL][BATTERY] charging current=%d charger volt=%d\n\r",current,chr_volt);
			}
			else
			{
				//print( "[PL][BATTERY] charging current=%d charger volt=%d\n\r",current,chr_volt);
				cnt=0;
			}
		}

		if(cnt>=8)
		{

	            //print( "[PL][BATTERY] charging current and charger volt too low !! \n\r",cnt);

	            pchr_turn_on_charging(KAL_FALSE);
	   			break;
		}
		mdelay(50);

         bat_val = get_bat_sense_volt(5);
		 print( "check VBAT=%d mV  \n", bat_val);
    }

    //print( "[%s]: check VBAT=%d mV with %d mV, stop charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD);
}
#endif

//==============================================================================
// PMIC Init Code
//==============================================================================
U32 pmic_init (void)
{
    U32 ret_code = PMIC_TEST_PASS;
    kal_uint32 val=0;
    int ret_val=0;
    int reg_val=0;

    ret_val=pmic_config_interface(MT6328_PMIC_BIAS_GEN_EN_SEL_ADDR, 0x0001, MT6328_PMIC_BIAS_GEN_EN_SEL_MASK, MT6328_PMIC_BIAS_GEN_EN_SEL_SHIFT);
    ret_val=pmic_config_interface(MT6328_PMIC_BIAS_GEN_EN_ADDR, 0x0000, MT6328_PMIC_BIAS_GEN_EN_MASK, MT6328_PMIC_BIAS_GEN_EN_SHIFT);

    print("[pmic_init] Preloader Start,MT6328 CHIP Code = 0x%x\n", get_MT6328_PMIC_chip_version());
   
   /* move to usb_dl_wo_batt to avoid some condition that < 70mA */
   /* ret_val=pmic_config_interface(MT6328_PMIC_RG_ULC_DET_EN_ADDR,1,MT6328_PMIC_RG_ULC_DET_EN_MASK,MT6328_PMIC_RG_ULC_DET_EN_SHIFT);*/

	//detect V battery Drop
	pmic_DetectVbatDrop();

	if(hw_check_battery()==1)
	{
		pmic_disable_usbdl_wo_battery();
	}


    pmic_6328_efuse_management();

	pmic_read_interface(MT6328_PMIC_RG_VIO18_CAL_ADDR, &val, MT6328_PMIC_RG_VIO18_CAL_MASK, MT6328_PMIC_RG_VIO18_CAL_SHIFT);
	vio18_cal = (unsigned char)val;
	upmu_set_rg_vio18_184();
	//pmic_read_interface(MT6328_PMIC_RG_VMC_CAL_ADDR, &vmc_cal, MT6328_PMIC_RG_VMC_CAL_MASK, MT6328_PMIC_RG_VMC_CAL_SHIFT);
	//upmu_set_rg_vmc_186();

    #if 1
    //Enable PMIC RST function (depends on main chip RST function)
    ret_val=pmic_config_interface(MT6328_TOP_RST_MISC_CLR, 0x0002, 0xFFFF, 0); //[1]=0, RG_WDTRSTB_MODE
    ret_val=pmic_config_interface(MT6328_TOP_RST_MISC_SET, 0x0001, 0xFFFF, 0); //[0]=1, RG_WDTRSTB_EN
    //print("[pmic_init] Reg[0x%x]=0x%x\n", MT6328_TOP_RST_MISC, upmu_get_reg_value(MT6328_TOP_RST_MISC));
    #endif


	 ret_val= pmic_config_interface(MT6328_PMIC_RG_SMPS_TESTMODE_B_ADDR, 0x0001, MT6328_PMIC_RG_SMPS_TESTMODE_B_MASK, MT6328_PMIC_RG_SMPS_TESTMODE_B_SHIFT); //RG_SMPS_TESTMODE_B by luke

     ret_val = pmic_config_interface(0xA44,0x1,0x1,1); // [1:1]: RG_TREF_EN; Tim

	 mt6311_driver_probe();

    #if defined(MACH_TYPE_MT6753)
	if(is_mt6311_exist()==1)
	{
		/* disable vproc oc */
		ret_val=pmic_config_interface(MT6328_PMIC_VPROC_PG_ENB_ADDR, 0x0001, MT6328_PMIC_VPROC_PG_ENB_MASK, MT6328_PMIC_VPROC_PG_ENB_SHIFT);
		ret_val=pmic_config_interface(MT6328_PMIC_VPROC_OC_ENB_ADDR, 0x0001, MT6328_PMIC_VPROC_OC_ENB_MASK, MT6328_PMIC_VPROC_OC_ENB_SHIFT);
		ret_val=pmic_config_interface(MT6328_PMIC_VPROC_EN_ADDR, 0x0001, MT6328_PMIC_VPROC_EN_MASK, MT6328_PMIC_VPROC_EN_SHIFT);
		ret_val=pmic_config_interface(MT6328_PMIC_VPROC_SFCHG_FEN_ADDR, 0x0000, MT6328_PMIC_VPROC_SFCHG_FEN_MASK, MT6328_PMIC_VPROC_SFCHG_FEN_SHIFT);
		ret_val=pmic_config_interface(MT6328_PMIC_VPROC_SFCHG_REN_ADDR, 0x0000, MT6328_PMIC_VPROC_SFCHG_REN_MASK, MT6328_PMIC_VPROC_SFCHG_REN_SHIFT);
#if 0 /* if need debug, enable it */
		print("[pmic_init] VPROC_PG_ENB Reg[0x%x]=0x%x\n", MT6328_PMIC_VPROC_PG_ENB_ADDR, upmu_get_reg_value(MT6328_PMIC_VPROC_PG_ENB_ADDR));
		print("[pmic_init] VPROC_OC_ENB Reg[0x%x]=0x%x\n", MT6328_PMIC_VPROC_OC_ENB_ADDR, upmu_get_reg_value(MT6328_PMIC_VPROC_OC_ENB_ADDR));
		print("[pmic_init] VPROC_EN Reg[0x%x]=0x%x\n", MT6328_PMIC_VPROC_EN_ADDR, upmu_get_reg_value(MT6328_PMIC_VPROC_EN_ADDR));
		print("[pmic_init] VPROC_SFCHG_FEN Reg[0x%x]=0x%x\n", MT6328_PMIC_VPROC_SFCHG_FEN_ADDR, upmu_get_reg_value(MT6328_PMIC_VPROC_SFCHG_FEN_ADDR));
		print("[pmic_init] VPROC_SFCHG_REN Reg[0x%x]=0x%x\n", MT6328_PMIC_VPROC_SFCHG_REN_ADDR, upmu_get_reg_value(MT6328_PMIC_VPROC_SFCHG_REN_ADDR));
#endif
/*
		1.	Vproc 1.0V -> 1.1V
		2.	Delay 100us
		3.	Vsram 1.2V -> 1.25V
		4.	Delay 100us  20us
		5.	Vproc 1.1V -> 1.15V
		6.	Delay 100us

		MT6311
		Voltage = 0.7+0.00625*step
		(0.7V~1.49375V)
*/

		//1.VPROC 1.0v -> 1.1v
		mt6311_config_interface(0x8D,0x40,0x7F,0); // [6:0]: VDVFS11_VOSEL; Setting by lower power ,20150305, Johsnon,_1.15V forD3T
		mt6311_config_interface(0x8E,0x40,0x7F,0); // [6:0]: VDVFS11_VOSEL_ON; Setting by lower power ,20150305, Johsnon,_1.15V forD3T
		//2.delay 100us
		udelay(100);
		//3.VSRAM 1.2v -> 1.25v
		upmu_set_reg_value(0x4b4,0x68); //vsram 1.25v
		upmu_set_reg_value(0xA88,0x68); //vsram 1.25v
		//4.delay 20us
		udelay(20);
		//5.VPROC 1.1V->1.15V
		mt6311_config_interface(0x8D,0x58,0x7F,0); // [6:0]: VDVFS11_VOSEL; Setting by lower power ,20150305, Johsnon,_1.15V forD3T
		mt6311_config_interface(0x8E,0x58,0x7F,0); // [6:0]: VDVFS11_VOSEL_ON; Setting by lower power ,20150305, Johsnon,_1.15V forD3T
		udelay(100);

	}
	else
	{
		upmu_set_reg_value(0x4b4,0x68); //vsram 1.25v
		upmu_set_reg_value(0xA88,0x68); //RG_VSRAM_VOSEL; vsram 1.25v
	}
    pmic_config_interface(MT6328_PMIC_VCORE1_VOSEL_ADDR, 0x68, MT6328_PMIC_VCORE1_VOSEL_MASK, MT6328_PMIC_VCORE1_VOSEL_SHIFT);
    pmic_config_interface(MT6328_PMIC_VCORE1_VOSEL_ON_ADDR, 0x68, MT6328_PMIC_VCORE1_VOSEL_ON_MASK, MT6328_PMIC_VCORE1_VOSEL_ON_SHIFT);
	#else
	upmu_set_reg_value(0x4b4,0x68); //vsram 1.25v
	upmu_set_reg_value(0xA88,0x68); //vsram 1.25v
    #endif


		if(hw_check_battery()==1)
		{
#if !CFG_EVB_PLATFORM
			pl_check_bat_protect_status();
#endif
		}


    print("[pmic_init] Done...................\n");

    return ret_code;
}


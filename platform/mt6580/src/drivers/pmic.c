#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>


#define V_CHARGER_MAX 6500				// 6.5 V
#define BATTERY_LOWVOL_THRESOLD 3300
#define R_CHARGER_1 330
#define R_CHARGER_2 39

#ifndef PMIC_DEBUG
#define print(...) do {} while(0)
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// PMIC access API
//////////////////////////////////////////////////////////////////////////////////////////
U32 pmic_read_interface (U32 RegNum, U32 *val, U32 MASK, U32 SHIFT)
{
    U32 return_value = 0;
    U32 pmic6350_reg = 0;
    U32 rdata;

    //mt6350_read_byte(RegNum, &pmic6350_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic6350_reg=rdata;
    if(return_value!=0)
    {
        print("[pmic_read_interface] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    /*print("[pmic_read_interface] Reg[%x]=0x%x\n", RegNum, pmic6350_reg);*/

    pmic6350_reg &= (MASK << SHIFT);
    *val = (pmic6350_reg >> SHIFT);
    /*print("[pmic_read_interface] val=0x%x\n", *val);*/

    return return_value;
}

U32 pmic_config_interface (U32 RegNum, U32 val, U32 MASK, U32 SHIFT)
{
    U32 return_value = 0;
    U32 pmic6350_reg = 0;
    U32 rdata;

    //1. mt6350_read_byte(RegNum, &pmic6350_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic6350_reg=rdata;
    if(return_value!=0)
    {
        print("[pmic_config_interface] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    /*print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic6350_reg);*/

    pmic6350_reg &= ~(MASK << SHIFT);
    pmic6350_reg |= (val << SHIFT);

    //2. mt6350_write_byte(RegNum, pmic6350_reg);
    return_value= pwrap_wacs2(1, (RegNum), pmic6350_reg, &rdata);
    if(return_value!=0)
    {
        print("[pmic_config_interface] Reg[%x]= pmic_wrap read data fail\n", RegNum);
        return return_value;
    }
    /*print("[pmic_config_interface] write Reg[%x]=0x%x\n", RegNum, pmic6350_reg);*/

#if 0
    //3. Double Check
    //mt6350_read_byte(RegNum, &pmic6350_reg);
    return_value= pwrap_wacs2(0, (RegNum), 0, &rdata);
    pmic6350_reg=rdata;
    if(return_value!=0)
    {
        print("[pmic_config_interface] Reg[%x]= pmic_wrap write data fail\n", RegNum);
        return return_value;
    }
    print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic6350_reg);
#endif

    return return_value;
}

U32 upmu_get_reg_value(kal_uint32 reg)
{
    U32 ret=0;
    U32 reg_val=0;

    ret=pmic_read_interface(reg, &reg_val, 0xFFFF, 0x0);

    return reg_val;
}

//////////////////////////////////////////////////////////////////////////////////////////
// PMIC-Charger Type Detection
//////////////////////////////////////////////////////////////////////////////////////////
CHARGER_TYPE g_ret = CHARGER_UNKNOWN;
int g_charger_in_flag = 0;
int g_first_check=0;

extern void Charger_Detect_Init(void);
extern void Charger_Detect_Release(void);

void pmic_lock(void){
}

void pmic_unlock(void){
}

static int vbat_status = PMIC_VBAT_NOT_DROP;
static void pmic_DetectVbatDrop (void)
{

	U32 ret=0;
	U32 just_rst=0;

	pmic_read_interface( MT6350_PMIC_JUST_PWRKEY_RST_ADDR, (&just_rst), MT6350_PMIC_JUST_PWRKEY_RST_MASK, MT6350_PMIC_JUST_PWRKEY_RST_SHIFT );
	pmic_config_interface(MT6350_PMIC_CLR_JUST_RST_ADDR, 1, MT6350_PMIC_CLR_JUST_RST_MASK, MT6350_PMIC_CLR_JUST_RST_SHIFT);

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


void upmu_set_rg_bc11_bb_ctrl(kal_uint32 val)
{
  kal_uint32 ret=0;

  pmic_lock();
  ret=pmic_config_interface( (kal_uint32)(MT6350_CHR_CON18),
                             (kal_uint32)(val),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_BB_CTRL_MASK),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_BB_CTRL_SHIFT)
	                         );
  pmic_unlock();
}

void upmu_set_rg_bc11_rst(kal_uint32 val)
{
  kal_uint32 ret=0;

  pmic_lock();
  ret=pmic_config_interface( (kal_uint32)(MT6350_CHR_CON18),
                             (kal_uint32)(val),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_RST_MASK),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_RST_SHIFT)
	                         );
  pmic_unlock();
}

void upmu_set_rg_bc11_vsrc_en(kal_uint32 val)
{
  kal_uint32 ret=0;

  pmic_lock();
  ret=pmic_config_interface( (kal_uint32)(MT6350_CHR_CON18),
                             (kal_uint32)(val),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_VSRC_EN_MASK),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_VSRC_EN_SHIFT)
	                         );
  pmic_unlock();
}

kal_uint32 upmu_get_rgs_bc11_cmp_out(void)
{
  kal_uint32 ret=0;
  kal_uint32 val=0;

  pmic_lock();
  ret=pmic_read_interface( (kal_uint32)(MT6350_CHR_CON18),
                           (&val),
                           (kal_uint32)(MT6350_PMIC_RGS_BC11_CMP_OUT_MASK),
                           (kal_uint32)(MT6350_PMIC_RGS_BC11_CMP_OUT_SHIFT)
	                       );
  pmic_unlock();

  return val;
}

void upmu_set_rg_bc11_vref_vth(kal_uint32 val)
{
  kal_uint32 ret=0;

  pmic_lock();
  ret=pmic_config_interface( (kal_uint32)(MT6350_CHR_CON19),
                             (kal_uint32)(val),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_VREF_VTH_MASK),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_VREF_VTH_SHIFT)
	                         );
  pmic_unlock();
}

void upmu_set_rg_bc11_cmp_en(kal_uint32 val)
{
  kal_uint32 ret=0;

  pmic_lock();
  ret=pmic_config_interface( (kal_uint32)(MT6350_CHR_CON19),
                             (kal_uint32)(val),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_CMP_EN_MASK),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_CMP_EN_SHIFT)
	                         );
  pmic_unlock();
}

void upmu_set_rg_bc11_ipd_en(kal_uint32 val)
{
  kal_uint32 ret=0;

  pmic_lock();
  ret=pmic_config_interface( (kal_uint32)(MT6350_CHR_CON19),
                             (kal_uint32)(val),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_IPD_EN_MASK),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_IPD_EN_SHIFT)
	                         );
  pmic_unlock();
}

void upmu_set_rg_bc11_ipu_en(kal_uint32 val)
{
  kal_uint32 ret=0;

  pmic_lock();
  ret=pmic_config_interface( (kal_uint32)(MT6350_CHR_CON19),
                             (kal_uint32)(val),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_IPU_EN_MASK),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_IPU_EN_SHIFT)
	                         );
  pmic_unlock();
}

void upmu_set_rg_bc11_bias_en(kal_uint32 val)
{
  kal_uint32 ret=0;

  pmic_lock();
  ret=pmic_config_interface( (kal_uint32)(MT6350_CHR_CON19),
                             (kal_uint32)(val),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_BIAS_EN_MASK),
                             (kal_uint32)(MT6350_PMIC_RG_BC11_BIAS_EN_SHIFT)
	                         );
  pmic_unlock();
}


void hw_bc11_init(void)
 {
	 Charger_Detect_Init();

	 //RG_BC11_BIAS_EN=1
	 upmu_set_rg_bc11_bias_en(0x1);
	 //RG_BC11_VSRC_EN[1:0]=00
	 upmu_set_rg_bc11_vsrc_en(0x0);
	 //RG_BC11_VREF_VTH = [1:0]=00
	 upmu_set_rg_bc11_vref_vth(0x0);
	 //RG_BC11_CMP_EN[1.0] = 00
	 upmu_set_rg_bc11_cmp_en(0x0);
	 //RG_BC11_IPU_EN[1.0] = 00
	 upmu_set_rg_bc11_ipu_en(0x0);
	 //RG_BC11_IPD_EN[1.0] = 00
	 upmu_set_rg_bc11_ipd_en(0x0);
	   //BC11_RST=1
	 upmu_set_rg_bc11_rst(0x1);
	 //BC11_BB_CTRL=1
	 upmu_set_rg_bc11_bb_ctrl(0x1);

	 mdelay(100);

 }


 U32 hw_bc11_DCD(void)
 {
	 U32 wChargerAvail = 0;

	  //RG_BC11_IPU_EN[1.0] = 10
	 upmu_set_rg_bc11_ipu_en(0x2);
	   //RG_BC11_IPD_EN[1.0] = 01
	 upmu_set_rg_bc11_ipd_en(0x1);
	  //RG_BC11_VREF_VTH = [1:0]=01
	 upmu_set_rg_bc11_vref_vth(0x1);
	  //RG_BC11_CMP_EN[1.0] = 10
	 upmu_set_rg_bc11_cmp_en(0x2);

	 mdelay(400);

 	 wChargerAvail = upmu_get_rgs_bc11_cmp_out();

	 //RG_BC11_IPU_EN[1.0] = 00
	 upmu_set_rg_bc11_ipu_en(0x0);
	  //RG_BC11_IPD_EN[1.0] = 00
	 upmu_set_rg_bc11_ipd_en(0x0);
	  //RG_BC11_CMP_EN[1.0] = 00
	 upmu_set_rg_bc11_cmp_en(0x0);
	  //RG_BC11_VREF_VTH = [1:0]=00
	 upmu_set_rg_bc11_vref_vth(0x0);

	 return wChargerAvail;
 }


 U32 hw_bc11_stepA1(void)
 {
	 U32 wChargerAvail = 0;

	  //RG_BC11_IPU_EN[1.0] = 10
	 upmu_set_rg_bc11_ipu_en(0x2);
	   //RG_BC11_VREF_VTH = [1:0]=10
	 upmu_set_rg_bc11_vref_vth(0x2);
	  //RG_BC11_CMP_EN[1.0] = 10
	 upmu_set_rg_bc11_cmp_en(0x2);

	 mdelay(80);

     wChargerAvail = upmu_get_rgs_bc11_cmp_out();

	 //RG_BC11_IPU_EN[1.0] = 00
	 upmu_set_rg_bc11_ipu_en(0x0);
	  //RG_BC11_CMP_EN[1.0] = 00
	 upmu_set_rg_bc11_cmp_en(0x0);

	 return  wChargerAvail;
 }


 U32 hw_bc11_stepB1(void)
 {
	 U32 wChargerAvail = 0;

	  //RG_BC11_IPU_EN[1.0] = 01
	 upmu_set_rg_bc11_ipu_en(0x1);
	  //RG_BC11_VREF_VTH = [1:0]=10
	 upmu_set_rg_bc11_vref_vth(0x2);
	  //RG_BC11_CMP_EN[1.0] = 01
	 upmu_set_rg_bc11_cmp_en(0x1);

	 mdelay(80);

     wChargerAvail = upmu_get_rgs_bc11_cmp_out();

	 //RG_BC11_IPU_EN[1.0] = 00
	 upmu_set_rg_bc11_ipu_en(0x0);
	  //RG_BC11_CMP_EN[1.0] = 00
	 upmu_set_rg_bc11_cmp_en(0x0);
	   //RG_BC11_VREF_VTH = [1:0]=00
	 upmu_set_rg_bc11_vref_vth(0x0);

	 return  wChargerAvail;
 }


 U32 hw_bc11_stepC1(void)
 {
	 U32 wChargerAvail = 0;

	  //RG_BC11_IPU_EN[1.0] = 01
	 upmu_set_rg_bc11_ipu_en(0x1);
	   //RG_BC11_VREF_VTH = [1:0]=10
	 upmu_set_rg_bc11_vref_vth(0x2);
	  //RG_BC11_CMP_EN[1.0] = 01
	 upmu_set_rg_bc11_cmp_en(0x1);

	 mdelay(80);

     wChargerAvail = upmu_get_rgs_bc11_cmp_out();

	 //RG_BC11_IPU_EN[1.0] = 00
	 upmu_set_rg_bc11_ipu_en(0x0);
	  //RG_BC11_CMP_EN[1.0] = 00
	 upmu_set_rg_bc11_cmp_en(0x0);
	   //RG_BC11_VREF_VTH = [1:0]=00
     upmu_set_rg_bc11_vref_vth(0x0);

	 return  wChargerAvail;
 }


 U32 hw_bc11_stepA2(void)
 {
	 U32 wChargerAvail = 0;

	 //RG_BC11_VSRC_EN[1.0] = 10
	 upmu_set_rg_bc11_vsrc_en(0x2);
	 //RG_BC11_IPD_EN[1:0] = 01
	 upmu_set_rg_bc11_ipd_en(0x1);
	 //RG_BC11_VREF_VTH = [1:0]=00
	 upmu_set_rg_bc11_vref_vth(0x0);
	 //RG_BC11_CMP_EN[1.0] = 01
	 upmu_set_rg_bc11_cmp_en(0x1);

	 mdelay(80);

     wChargerAvail = upmu_get_rgs_bc11_cmp_out();

	 //RG_BC11_VSRC_EN[1:0]=00
	 upmu_set_rg_bc11_vsrc_en(0x0);
	 //RG_BC11_IPD_EN[1.0] = 00
	 upmu_set_rg_bc11_ipd_en(0x0);
	 //RG_BC11_CMP_EN[1.0] = 00
	 upmu_set_rg_bc11_cmp_en(0x0);

	 return  wChargerAvail;
 }


 U32 hw_bc11_stepB2(void)
 {
	 U32 wChargerAvail = 0;

	//RG_BC11_IPU_EN[1:0]=10
	upmu_set_rg_bc11_ipu_en(0x2);
	//RG_BC11_VREF_VTH = [1:0]=01
	upmu_set_rg_bc11_vref_vth(0x1);
	//RG_BC11_CMP_EN[1.0] = 01
	upmu_set_rg_bc11_cmp_en(0x1);

	mdelay(80);

    wChargerAvail = upmu_get_rgs_bc11_cmp_out();

	//RG_BC11_IPU_EN[1.0] = 00
	upmu_set_rg_bc11_ipu_en(0x0);
	//RG_BC11_CMP_EN[1.0] = 00
	upmu_set_rg_bc11_cmp_en(0x0);
	//RG_BC11_VREF_VTH = [1:0]=00
	upmu_set_rg_bc11_vref_vth(0x0);

	 return  wChargerAvail;
 }


 void hw_bc11_done(void)
 {
	 //RG_BC11_VSRC_EN[1:0]=00
	 upmu_set_rg_bc11_vsrc_en(0x0);
	 //RG_BC11_VREF_VTH = [1:0]=0
	 upmu_set_rg_bc11_vref_vth(0x0);
	 //RG_BC11_CMP_EN[1.0] = 00
	 upmu_set_rg_bc11_cmp_en(0x0);
	 //RG_BC11_IPU_EN[1.0] = 00
	 upmu_set_rg_bc11_ipu_en(0x0);
	 //RG_BC11_IPD_EN[1.0] = 00
	 upmu_set_rg_bc11_ipd_en(0x0);
	 //RG_BC11_BIAS_EN=0
	 upmu_set_rg_bc11_bias_en(0x0);

	 Charger_Detect_Release();
 }

CHARGER_TYPE hw_charger_type_detection(void)
{
    CHARGER_TYPE charger_tye;
#if defined(CONFIG_POWER_EXT)
	 charger_tye = STANDARD_HOST;
#else

	/********* Step initial  ***************/
	hw_bc11_init();

	/********* Step DCD ***************/
	if(1 == hw_bc11_DCD())
	{
		 /********* Step A1 ***************/
		 if(1 == hw_bc11_stepA1())
		 {
			 /********* Step B1 ***************/
			 if(1 == hw_bc11_stepB1())
			 {
				 charger_tye = NONSTANDARD_CHARGER;
				  print("step B1 : Non STANDARD CHARGER!\r\n");
			 }
			 else
			 {
				 charger_tye = APPLE_2_1A_CHARGER;
				  print("step B1 : Apple 2.1A CHARGER!\r\n");
			 }
		 }
		 else
		 {
			 /********* Step C1 ***************/
			 if(1 == hw_bc11_stepC1())
			 {
				 charger_tye = APPLE_1_0A_CHARGER;
				  print("step C1 : Apple 1A CHARGER!\r\n");
			 }
			 else
			 {
				charger_tye = APPLE_0_5A_CHARGER;
				  print("step C1 : Apple 0.5A CHARGER!\r\n");
			 }
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
#endif
	 return charger_tye;
}

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
// PMIC6350 Usage APIs
//==============================================================================
U32 get_pmic6350_chip_version (void)
{
    U32 ret=0;
    U32 eco_version = 0;

    ret=pmic_read_interface( (U32)(MT6350_CID),
                             (&eco_version),
                             (U32)(MT6350_PMIC_CID_MASK),
                             (U32)(MT6350_PMIC_CID_SHIFT)
                             );

    return eco_version;
}

int pmic_detect_powerkey(void)
{
    U32 ret=0;
    U32 val=0;

    ret=pmic_read_interface( (U32)(MT6350_CHRSTATUS),
                             (&val),
                             (U32)(MT6350_PMIC_PWRKEY_DEB_MASK),
                             (U32)(MT6350_PMIC_PWRKEY_DEB_SHIFT)
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

    ret=pmic_read_interface( (U32)(MT6350_CHRSTATUS),
                           (&val),
                           (U32)(MT6350_PMIC_FCHRKEY_DEB_MASK),
                           (U32)(MT6350_PMIC_FCHRKEY_DEB_SHIFT)
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

    ret=pmic_read_interface( (U32)(MT6350_CHR_CON0),
                             (&val),
                             (U32)(MT6350_PMIC_RGS_CHRDET_MASK),
                             (U32)(MT6350_PMIC_RGS_CHRDET_SHIFT)
                             );

#ifdef SLT
    return PMIC_CHRDET_EXIST;
#endif
    if(val)
        return PMIC_CHRDET_EXIST;
    else
        return PMIC_CHRDET_NOT_EXIST;
}

void hw_set_cc(int cc_val)
{
	U32 ret_val=0;
    U32 reg_val=0;
    U32 i=0;
    U32 hw_charger_ov_flag=0;

    print("hw_set_cc: %d\r\n", cc_val);


    //CS_EN=1
    ret_val=pmic_config_interface(MT6350_CHR_CON2, 0x01, MT6350_PMIC_RG_CS_EN_MASK, MT6350_PMIC_RG_CS_EN_SHIFT);
    //CSDAC_MODE=1
    ret_val=pmic_config_interface(MT6350_CHR_CON23, 0x01, MT6350_PMIC_RG_CSDAC_MODE_MASK, MT6350_PMIC_RG_CSDAC_MODE_SHIFT);

    ret_val=pmic_read_interface(MT6350_CHR_CON0, &hw_charger_ov_flag, MT6350_PMIC_RGS_VCDT_HV_DET_MASK, MT6350_PMIC_RGS_VCDT_HV_DET_SHIFT);
    if(hw_charger_ov_flag == 1)
    {
        ret_val=pmic_config_interface(MT6350_CHR_CON0, 0x00, MT6350_PMIC_RG_CHR_EN_MASK, MT6350_PMIC_RG_CHR_EN_SHIFT);
        print("pl chargerov turn off charging \n");
        return;
    }

    // CS_VTH
    switch(cc_val){
        case 1600: ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x00, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 1500: ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x01, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 1400: ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x02, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 1300: ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x03, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 1200: ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x04, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 1100: ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x05, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 1000: ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x06, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 900:  ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x07, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 800:  ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x08, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 700:  ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x09, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 650:  ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x0A, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 550:  ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x0B, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 450:  ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x0C, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 300:  ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x0D, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 200:  ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x0E, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        case 70:   ret_val=pmic_config_interface(MT6350_CHR_CON4, 0x0F, MT6350_PMIC_RG_CS_VTH_MASK, MT6350_PMIC_RG_CS_VTH_SHIFT); break;
        default:
            print("hw_set_cc: argument invalid!!\r\n");
            break;
    }

    //upmu_chr_csdac_dly(0x4);                // CSDAC_DLY
    ret_val=pmic_config_interface(MT6350_CHR_CON21, 0x04, MT6350_PMIC_RG_CSDAC_DLY_MASK, MT6350_PMIC_RG_CSDAC_DLY_SHIFT);
    //upmu_chr_csdac_stp(0x1);                // CSDAC_STP
    ret_val=pmic_config_interface(MT6350_CHR_CON21, 0x01, MT6350_PMIC_RG_CSDAC_STP_MASK, MT6350_PMIC_RG_CSDAC_STP_SHIFT);
    //upmu_chr_csdac_stp_inc(0x1);            // CSDAC_STP_INC
    ret_val=pmic_config_interface(MT6350_CHR_CON20, 0x01, MT6350_PMIC_RG_CSDAC_STP_INC_MASK, MT6350_PMIC_RG_CSDAC_STP_INC_SHIFT);
    //upmu_chr_csdac_stp_dec(0x2);            // CSDAC_STP_DEC
    ret_val=pmic_config_interface(MT6350_CHR_CON20, 0x02, MT6350_PMIC_RG_CSDAC_STP_DEC_MASK, MT6350_PMIC_RG_CSDAC_STP_DEC_SHIFT);
    //upmu_chr_chrwdt_td(0x0);                // CHRWDT_TD, 4s
    ret_val=pmic_config_interface(MT6350_CHR_CON13, 0x00, MT6350_PMIC_RG_CHRWDT_TD_MASK, MT6350_PMIC_RG_CHRWDT_TD_SHIFT);
    //upmu_set_rg_chrwdt_wr(1);             // CHRWDT_FLAG_WR
    ret_val=pmic_config_interface(MT6350_CHR_CON13, 0x01, MT6350_PMIC_RG_CHRWDT_WR_MASK, MT6350_PMIC_RG_CHRWDT_WR_SHIFT);
    //upmu_chr_chrwdt_int_en(1);              // CHRWDT_INT_EN
    ret_val=pmic_config_interface(MT6350_CHR_CON15, 0x01, MT6350_PMIC_RG_CHRWDT_INT_EN_MASK, MT6350_PMIC_RG_CHRWDT_INT_EN_SHIFT);
    //upmu_chr_chrwdt_en(1);                  // CHRWDT_EN
    ret_val=pmic_config_interface(MT6350_CHR_CON13, 0x01, MT6350_PMIC_RG_CHRWDT_EN_MASK, MT6350_PMIC_RG_CHRWDT_EN_SHIFT);
    //upmu_chr_chrwdt_flag_wr(1);             // CHRWDT_FLAG
    ret_val=pmic_config_interface(MT6350_CHR_CON15, 0x01, MT6350_PMIC_RG_CHRWDT_FLAG_WR_MASK, MT6350_PMIC_RG_CHRWDT_FLAG_WR_SHIFT);
    //upmu_chr_csdac_enable(1);               // CSDAC_EN
    ret_val=pmic_config_interface(MT6350_CHR_CON0, 0x01, MT6350_PMIC_RG_CSDAC_EN_MASK, MT6350_PMIC_RG_CSDAC_EN_SHIFT);
    //upmu_set_rg_hwcv_en(1);                 // HWCV_EN
    ret_val=pmic_config_interface(MT6350_CHR_CON23, 0x01, MT6350_PMIC_RG_HWCV_EN_MASK, MT6350_PMIC_RG_HWCV_EN_SHIFT);
    //upmu_chr_enable(1);                     // CHR_EN
    ret_val=pmic_config_interface(MT6350_CHR_CON0, 0x01, MT6350_PMIC_RG_CHR_EN_MASK, MT6350_PMIC_RG_CHR_EN_SHIFT);

/*
    for(i=MT6350_CHR_CON0 ; i<=MT6350_CHR_CON29 ; i++)
    {
        ret_val=pmic_read_interface(i,&reg_val,0xFFFF,0x0);
        print("[0x%x]=0x%x\n", i, reg_val);
    }
*/
    print("hw_set_cc: done\r\n");
}

void pl_hw_ulc_det(void)
{
	U32 ret_val=0;

    //upmu_chr_ulc_det_en(1);            // RG_ULC_DET_EN=1
    ret_val=pmic_config_interface(MT6350_CHR_CON23, 0x01, MT6350_PMIC_RG_ULC_DET_EN_MASK, MT6350_PMIC_RG_ULC_DET_EN_SHIFT);
    //upmu_chr_low_ich_db(1);            // RG_LOW_ICH_DB=000001'b
    ret_val=pmic_config_interface(MT6350_CHR_CON22, 0x01, MT6350_PMIC_RG_LOW_ICH_DB_MASK, MT6350_PMIC_RG_LOW_ICH_DB_SHIFT);
}

int hw_check_battery(void)
{
#ifndef MTK_DISABLE_POWER_ON_OFF_VOLTAGE_LIMITATION
	U32 ret_val;
	U32 reg_val=0;

 	ret_val=pmic_config_interface(MT6350_CHR_CON7,0x01, MT6350_PMIC_RG_BATON_EN_MASK, MT6350_PMIC_RG_BATON_EN_SHIFT);      //BATON_EN=1
 	ret_val=pmic_config_interface(MT6350_CHR_CON7,    0x01, MT6350_PMIC_BATON_TDET_EN_MASK, MT6350_PMIC_BATON_TDET_EN_SHIFT);  //BATON_TDET_EN=1
	ret_val=pmic_read_interface(MT6350_CHR_CON7,&reg_val, MT6350_PMIC_RGS_BATON_UNDET_MASK, MT6350_PMIC_RGS_BATON_UNDET_SHIFT);

    if (reg_val == 1)
    {
        print("No Battery\n");

        ret_val=pmic_read_interface(MT6350_CHR_CON7,&reg_val,0xFFFF,0x0);
		print("[0x%x]=0x%x\n",MT6350_CHR_CON7,reg_val);

        return 0;
    }
    else
    {
        print("Battery exist\n");

        ret_val=pmic_read_interface(MT6350_CHR_CON7,&reg_val,0xFF,0x0);
		print("[0x%x]=0x%x\n",MT6350_CHR_CON7,reg_val);

        pl_hw_ulc_det();

        return 1;
    }
#else
	return 1;
#endif
}

void pl_charging(int en_chr)
{
	U32 ret_val=0;
    U32 reg_val=0;
    U32 i=0;

    if(en_chr == 1)
    {
        print("pl charging en\n");

        hw_set_cc(450);

        //USBDL set 1
        ret_val=pmic_config_interface(MT6350_CHR_CON16, 0x01, MT6350_PMIC_RG_USBDL_SET_MASK, MT6350_PMIC_RG_USBDL_SET_SHIFT);
    }
    else
    {
        print("pl charging dis\n");

        //USBDL set 0
        ret_val=pmic_config_interface(MT6350_CHR_CON16, 0x00, MT6350_PMIC_RG_USBDL_SET_MASK, MT6350_PMIC_RG_USBDL_SET_SHIFT);

        //upmu_set_rg_hwcv_en(0); // HWCV_EN
        ret_val=pmic_config_interface(MT6350_CHR_CON23, 0x00, MT6350_PMIC_RG_HWCV_EN_MASK, MT6350_PMIC_RG_HWCV_EN_SHIFT);
        //upmu_chr_enable(0); // CHR_EN
        ret_val=pmic_config_interface(MT6350_CHR_CON0, 0x00, MT6350_PMIC_RG_CHR_EN_MASK, MT6350_PMIC_RG_CHR_EN_SHIFT);
    }


}

void pl_kick_chr_wdt(void)
{
	 int ret_val=0;

    //upmu_chr_chrwdt_td(0x0);                // CHRWDT_TD
    ret_val=pmic_config_interface(MT6350_CHR_CON13, 0x03, MT6350_PMIC_RG_CHRWDT_TD_MASK, MT6350_PMIC_RG_CHRWDT_TD_SHIFT);
	 //upmu_set_rg_chrwdt_wr(1);            // CHRWDT_FLAG
    ret_val=pmic_config_interface(MT6350_CHR_CON15, 0x01, MT6350_PMIC_RG_CHRWDT_WR_MASK, MT6350_PMIC_RG_CHRWDT_WR_SHIFT);
    //upmu_chr_chrwdt_int_en(1);             // CHRWDT_INT_EN
    ret_val=pmic_config_interface(MT6350_CHR_CON15, 0x01, MT6350_PMIC_RG_CHRWDT_INT_EN_MASK, MT6350_PMIC_RG_CHRWDT_INT_EN_SHIFT);
    //upmu_chr_chrwdt_en(1);                   // CHRWDT_EN
    ret_val=pmic_config_interface(MT6350_CHR_CON13, 0x01, MT6350_PMIC_RG_CHRWDT_EN_MASK, MT6350_PMIC_RG_CHRWDT_EN_SHIFT);
    //upmu_chr_chrwdt_flag_wr(1);            // CHRWDT_FLAG_WR
    ret_val=pmic_config_interface(MT6350_CHR_CON15, 0x01, MT6350_PMIC_RG_CHRWDT_FLAG_WR_MASK, MT6350_PMIC_RG_CHRWDT_FLAG_WR_SHIFT);

    //print("[pl_kick_chr_wdt] done\n");
}

void pl_close_pre_chr_led(void)
{
	U32 ret_val=0;

    ret_val=pmic_config_interface(MT6350_CHR_CON22, 0x00, MT6350_PMIC_RG_CHRIND_ON_MASK, MT6350_PMIC_RG_CHRIND_ON_SHIFT);

    print("pl pmic close pre-chr LED\n");
}

void pmic_disable_usbdl_wo_battery(void)
{
        print("[pmic_init] turn off usbdl wo battery\n");
        pmic_config_interface(MT6350_PMIC_RG_USBDL_SET_ADDR, 0x0000, MT6350_PMIC_RG_USBDL_SET_MASK, MT6350_PMIC_RG_USBDL_SET_SHIFT); //[1]=0, RG_WDTRSTB_MODE
        pmic_config_interface(MT6350_PMIC_RG_USBDL_RST_ADDR, 0x0001, MT6350_PMIC_RG_USBDL_RST_MASK, MT6350_PMIC_RG_USBDL_RST_SHIFT); //[0]=1, RG_WDTRSTB_EN

}

U32 pmic_get_register_value(U32 RegNum, U32 MASK, U32 SHIFT)
{
	U32 val;
	U32 ret;

	ret=pmic_read_interface (RegNum,&val,MASK,SHIFT);

	return val;
}



#define VOLTAGE_FULL_RANGE     	1800
#define ADC_PRECISE		32768 	// 15 bits

int PMIC_IMM_GetOneChannelValue(int dwChannel, int deCount, int trimd)
{
	kal_int32 ret_data;
//	kal_int32 count=0;
	kal_int32 u4Sample_times = 0;
	kal_int32 u4channel=0;
	kal_int32 adc_result_temp=0;
	kal_int32 r_val_temp=0;
	kal_int32 adc_result=0;
    	U32 adc_reg_val=0;

        /*
        0 : BATON2
        1 : CH6
        2 : THR SENSE2
        3 : THR SENSE1
        4 : VCDT
        5 : BATON1
        6 : ISENSE
        7 : BATSNS
        8 : ACCDET
        */


         //pmic_set_register_value(PMIC_RG_VBUF_EN, 1);
		pmic_config_interface(MT6350_PMIC_RG_VBUF_EN_ADDR,1,MT6350_PMIC_RG_VBUF_EN_MASK,MT6350_PMIC_RG_VBUF_EN_SHIFT);

        //set 0
        pmic_read_interface(MT6350_AUXADC_CON22,&adc_reg_val, MT6350_PMIC_RG_AP_RQST_LIST_MASK, MT6350_PMIC_RG_AP_RQST_LIST_SHIFT);
        adc_reg_val = adc_reg_val & (~(1<<dwChannel));
        pmic_config_interface(MT6350_AUXADC_CON22,adc_reg_val, MT6350_PMIC_RG_AP_RQST_LIST_MASK, MT6350_PMIC_RG_AP_RQST_LIST_SHIFT);

        //set 1
        pmic_read_interface(MT6350_AUXADC_CON22,&adc_reg_val, MT6350_PMIC_RG_AP_RQST_LIST_MASK, MT6350_PMIC_RG_AP_RQST_LIST_SHIFT);
        adc_reg_val = adc_reg_val | (1<<dwChannel);
        pmic_config_interface(MT6350_AUXADC_CON22,adc_reg_val, MT6350_PMIC_RG_AP_RQST_LIST_MASK, MT6350_PMIC_RG_AP_RQST_LIST_SHIFT);
	udelay(300);
	do
	{
	    //count=0;
	    ret_data=0;

	    switch(dwChannel){
			/*
	        case 0:
	            while(mt6350_get_register_value(PMIC_RG_ADC_RDY_BATON2)!= 1 );
		    ret_data = mt6350_get_register_value(PMIC_RG_ADC_OUT_BATON2);
	            break;
	        case 1:
	            while(mt6350_get_register_value(PMIC_RG_ADC_RDY_CH6)!= 1 );
		    ret_data = mt6350_get_register_value(PMIC_RG_ADC_OUT_CH6);
	            break;
	        case 2:
	            while(mt6350_get_register_value(PMIC_RG_ADC_RDY_THR_SENSE2)!= 1 );
		    ret_data = mt6350_get_register_value(PMIC_RG_ADC_OUT_THR_SENSE2);
	            break;
	        case 3:
	            while(mt6350_get_register_value(PMIC_RG_ADC_RDY_THR_SENSE1)!= 1 );
		    ret_data = mt6350_get_register_value(PMIC_RG_ADC_OUT_THR_SENSE1);
	            break;
	        case 5:
	            while(mt6350_get_register_value(PMIC_RG_ADC_RDY_BATON1)!= 1 );
		    ret_data = mt6350_get_register_value(PMIC_RG_ADC_OUT_BATON1);
	            break;
	        case 6:
	            while(mt6350_get_register_value(PMIC_RG_ADC_RDY_ISENSE)!= 1 );
		    ret_data = mt6350_get_register_value(PMIC_RG_ADC_OUT_ISENSE);
	            break;
	            */
	        case 4:
	            while(pmic_get_register_value(MT6350_PMIC_RG_ADC_RDY_VCDT_ADDR,MT6350_PMIC_RG_ADC_RDY_VCDT_MASK,MT6350_PMIC_RG_ADC_RDY_VCDT_SHIFT) != 1 );
				    ret_data = pmic_get_register_value(MT6350_PMIC_RG_ADC_OUT_VCDT_ADDR,MT6350_PMIC_RG_ADC_OUT_VCDT_MASK,MT6350_PMIC_RG_ADC_OUT_VCDT_SHIFT);
	            break;

	        case 7:
	            while(pmic_get_register_value(MT6350_PMIC_RG_ADC_RDY_BATSNS_ADDR,MT6350_PMIC_RG_ADC_RDY_BATSNS_MASK,MT6350_PMIC_RG_ADC_RDY_BATSNS_SHIFT) != 1 );
				    ret_data = pmic_get_register_value(MT6350_PMIC_RG_ADC_OUT_BATSNS_ADDR,MT6350_PMIC_RG_ADC_OUT_BATSNS_MASK,MT6350_PMIC_RG_ADC_OUT_BATSNS_SHIFT);
	            break;
				/*
	        case 8:
	            while(mt6350_get_register_value(PMIC_RG_ADC_RDY_CH5)!= 1 );
		    ret_data = mt6350_get_register_value(PMIC_RG_ADC_OUT_CH5);
	            break;
	            */
	        default:
	            print("[AUXADC] Invalid channel value(%d,%d)\n", dwChannel, trimd);

	            return -1;
	            break;
	    }

	    u4channel += ret_data;

	    u4Sample_times++;

	   // if (Enable_BATDRV_LOG == 1)
	    {
	        //debug
	        print("[AUXADC] u4channel[%d]=%d.\n",
	            dwChannel, ret_data);
	    }

	}while (u4Sample_times < deCount);

    /* Value averaging  */
    adc_result_temp = u4channel / deCount;

    switch(dwChannel){
#if 0
        case 0:
            r_val_temp = 1;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
        case 1:
            r_val_temp = 1;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
        case 2:
            r_val_temp = 1;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
        case 3:
            r_val_temp = 1;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
        case 5:
            r_val_temp = 1;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
        case 6:
            r_val_temp = 4;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
#endif
        case 4:
            r_val_temp = 1;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
        case 7:
            r_val_temp = 4;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
        case 8:
            r_val_temp = 1;
            adc_result = (adc_result_temp*r_val_temp*VOLTAGE_FULL_RANGE)/ADC_PRECISE;
            break;
        default:
            print("[AUXADC] Invalid channel value(%d,%d)\n", dwChannel, trimd);

            return -1;
            break;
    }

        print("[AUXADC] adc_result_temp=%d, adc_result=%d, r_val_temp=%d.\n",
                adc_result_temp, adc_result, r_val_temp);

    return adc_result;
}

int get_bat_sense_volt(int times)
{
    return PMIC_IMM_GetOneChannelValue(7,1,1);
}

int get_charger_volt(int times)
{
	kal_int32 val;
	val=PMIC_IMM_GetOneChannelValue(4,times,1);
	val = (((R_CHARGER_1+R_CHARGER_2)*100*val)/R_CHARGER_2)/100;
	return val;
}

void pl_check_bat_protect_status(void)
{
    kal_int32 bat_val = 0;
	int current,chr_volt,cnt=0,i;

	chr_volt= get_charger_volt(1);
    bat_val = get_bat_sense_volt(5);

    print( "[%s]: check VBAT=%d mV with %d mV ,chr:%d  start charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD,chr_volt);


	while (bat_val < BATTERY_LOWVOL_THRESOLD)
	{
		mtk_wdt_restart();
		if(pmic_IsUsbCableIn() == PMIC_CHRDET_NOT_EXIST)
		{
			print( "[PL][BATTERY] No Charger\n");
			break;
		}

		chr_volt= get_charger_volt(1);
		if(chr_volt>V_CHARGER_MAX)
		{
			print( "[PL][BATTERY] charger voltage is too high :%d , threshold is %d !\n",chr_volt,V_CHARGER_MAX);
			break;
		}

		pl_charging(1);
		pl_kick_chr_wdt();

		mdelay(1000);

		bat_val = get_bat_sense_volt(5);
		print( "[PL][%s]: check VBAT=%d mV  \n", __FUNCTION__, bat_val);
	}

	print( "[PL][%s]: check VBAT=%d mV with %d mV, stop charging... \n", __FUNCTION__, bat_val, BATTERY_LOWVOL_THRESOLD);


}
//==============================================================================
// PMIC6350 Init Code
//==============================================================================
U32 pmic_init (void)
{
    U32 ret_code = PMIC_TEST_PASS;
    int ret_val=0;
    int reg_val=0;

	print("[pmic6350_init] Preloader Start..................\n");

	print("[pmic6350_init] PMIC CHIP Code = %d\n", get_pmic6350_chip_version());

    //VCDT_HV_VTH, 7V
    //pmic_config_interface(MT6350_CHR_CON1, V_CHARGER_MAX, MT6350_PMIC_RG_VCDT_HV_VTH_MASK, MT6350_PMIC_RG_VCDT_HV_VTH_SHIFT);
    //VCDT_HV_EN=1
    //pmic_config_interface(MT6350_CHR_CON0, 0x01, MT6350_PMIC_RG_VCDT_HV_EN_MASK, MT6350_PMIC_RG_VCDT_HV_EN_SHIFT);

    //put pmic hw initial setting here

    //hw_check_battery();
    //print("[pmic6350_init] hw_check_battery\n");

	// Detect V battery Drop
	pmic_DetectVbatDrop();

	if(hw_check_battery()==1)
	{
		pmic_disable_usbdl_wo_battery();
	}

	print("[pmic6350_init] powerKey = %d\n", pmic_detect_powerkey());

	print("[pmic6350_init] is USB in = %d\n", pmic_IsUsbCableIn());

     //Enable PMIC RST function (depends on main chip RST function)
    ret_val=pmic_config_interface(0x011A, 0x1, 0x1, 1);
    ret_val=pmic_config_interface(0x011A, 0x1, 0x1, 3);
#ifdef SLT
    ret_val=pmic_config_interface(0x011A, 0x1, 0x1, 6); // Disable long press shutdown
#endif

	if(hw_check_battery()==1)
	{
		pl_check_bat_protect_status();
	}
    print("[pmic6350_init] Done...................\n");

    return ret_code;
}

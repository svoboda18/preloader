/*****************************************************************************
*
* Filename:
* ---------
*   fan5405.h
*
* Project:
* --------
*   Android
*
* Description:
* ------------
*   fan5405 header file
*
* Author:
* -------
*
****************************************************************************/

#ifndef _fan5405_SW_H_
#define _fan5405_SW_H_

//#define HIGH_BATTERY_VOLTAGE_SUPPORT

#define fan5405_CON0      0x00
#define fan5405_CON1      0x01
#define fan5405_CON2      0x02

/**********************************************************
  *
  *   [MASK/SHIFT] 
  *
  *********************************************************/
    //CON0
#define CON0_BOOST_MASK     0x01
#define CON0_BOOST_SHIFT    3
    
    //CON1
#define CON1_OPA_MODE_MASK  0x01
#define CON1_OPA_MODE_SHIFT 0
    
    //CON2
#define CON2_OTG_EN_MASK    0x01
#define CON2_OTG_EN_SHIFT   0

/**********************************************************
  *
  *   [Extern Function] 
  *
  *********************************************************/
    //CON0----------------------------------------------------
extern kal_uint32 fan5405_get_otg_status(void);
//CON1----------------------------------------------------
extern void fan5405_set_opa_mode(kal_uint32 val);
//CON2----------------------------------------------------
extern void fan5405_set_otg_en(kal_uint32 val);

#endif // _fan5405_SW_H_


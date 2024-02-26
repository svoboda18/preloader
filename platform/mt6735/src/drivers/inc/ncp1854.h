/*****************************************************************************
*
* Filename:
* ---------
*   ncp1854.h
*
* Project:
* --------
*   Android
*
* Description:
* ------------
*   ncp1854 header file
*
* Author:
* -------
*
****************************************************************************/

#ifndef _NCP1854_SW_H_
#define _NCP1854_SW_H_

#define NCP1854_CON1      0x01

/**********************************************************
  *
  *   [MASK/SHIFT]
  *
  *********************************************************/

/*CON1*/
#define CON1_REG_RST_MASK 			0x01
#define CON1_REG_RST_SHIFT 			7

#define CON1_OTG_EN_MASK		 	0x01
#define CON1_OTG_EN_SHIFT			5

/*CON1*/
extern void ncp1854_set_otg_en(kal_uint32 val);
extern kal_uint32 ncp1854_get_otg_en(void);
extern void ncp1854_check_otg_status(void);

#endif /*_NCP1854_SW_H_*/


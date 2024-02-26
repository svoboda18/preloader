/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/******************************************************************************
 * pmic_wrap_init.c - MTK PMIC Wrapper Driver
 *
 * Copyright 2008-2009 MediaTek Co.,Ltd.
 *
 * DESCRIPTION:
 *     This file provides API for other drivers to access PMIC registers
 *
 ******************************************************************************/



//#include <mach/mt_reg_base.h>
//#include <mach/mt_typedefs.h>
#include <timer.h>
#include <typedefs.h>
#include <gpio.h>
#include "pmic_wrap_init.h"

//#include "reg_pmic.h"
//#include "reg_pmic_wrap.h"
//#include "mt_pmic_wrap.h"
//---start ---internal API--------------------------------------------------
static S32 _pwrap_wacs2_nochk( U32 write, U32 adr, U32 wdata, U32 *rdata );
static S32 _pwrap_reset_spislv(void);
static S32 _pwrap_init_dio( U32 dio_en );
static S32 _pwrap_init_cipher( void );
static S32 _pwrap_init_reg_clock( U32 regck_sel );
static BOOL _pwrap_timeout_ns (U64 start_time_ns, U64 timeout_time_ns);
static U64 _pwrap_get_current_time(void);
static U64 _pwrap_time2ns (U64 time_us);
static S32 pwrap_read_nochk( U32  adr, U32 *rdata );
static S32 pwrap_write_nochk( U32  adr, U32  wdata );
static S32 _pwrap_wacs2_nochk( U32 write, U32 adr, U32 wdata, U32 *rdata );
void pwrap_dump_ap_register(void);
//---end--internal API--------------------------------------------------

#ifdef PMIC_WRAP_NO_PMIC
S32 pwrap_wacs2( U32  write, U32  adr, U32  wdata, U32 *rdata )
{
	PWRAPERR("there is no PMIC real chip,PMIC_WRAP do Nothing\n");
	return 0;
}
//******************************************************************************
//--external API for pmic_wrap user-------------------------------------------------
//******************************************************************************
S32 pwrap_read( U32  adr, U32 *rdata )
{
  return pwrap_wacs2( 0, adr,0,rdata );
}

S32 pwrap_write( U32  adr, U32  wdata )
{
  return pwrap_wacs2( 1, adr,wdata,0 );
}

/*
 *pmic_wrap init,init wrap interface
 *
 */
S32 pwrap_init ( void )
{
	printf("[PMIC_WRAP]There is no PMIC real chip, PMIC_WRAP do Nothing.\n");
	return 0;
}
S32 pwrap_init_preloader ( void )
{
	u32 pwrap_ret=0,i=0;
	PWRAPFUC();
	for(i=0;i<3;i++)
	{
		pwrap_ret = pwrap_init();
		if(pwrap_ret!=0)
    		{
      			printf("[PMIC_WRAP]wrap_init fail,the return value=%x.\n",pwrap_ret);
    		}
		else
		{
		printf("[PMIC_WRAP]wrap_init pass,the return value=%x.\n",pwrap_ret);
		break;//init pass
		}
	}
#ifdef PWRAP_PRELOADER_PORTING
	//pwrap_init_for_early_porting();
#endif
 return 0;
}
#ifdef PWRAP_PRELOADER_PORTING
//--------------------------------------------------------
//    Function : _pwrap_status_update_test()
// Description :only for early porting
//   Parameter :
//      Return :
//--------------------------------------------------------
static S32 _pwrap_status_update_test_porting( void )
{
  return 0;
}

void pwrap_init_for_early_porting(void)
{

}
#endif //PWRAP_PRELOADER_PORTING

#else
/******************************************************************************
 wrapper timeout
******************************************************************************/
#define PWRAP_TIMEOUT
//use the same API name with kernel driver
//however,the timeout API in preloader use tick instead of ns
#ifdef PWRAP_TIMEOUT
static U64 _pwrap_get_current_time(void)
{
  return gpt4_get_current_tick();
}
//_pwrap_timeout_tick,use the same API name with kernel driver

static BOOL _pwrap_timeout_ns (U64 start_time, U64 elapse_time)
{
  return gpt4_timeout_tick(start_time, elapse_time);
}

//_pwrap_time2tick_us
static U64 _pwrap_time2ns (U64 time_us)
{
	return gpt4_time2tick_us(time_us);
}

#else
static U64 _pwrap_get_current_time(void)
{
	return 0;
}
static BOOL _pwrap_timeout_ns (U64 start_time, U64 elapse_time)//,U64 timeout_ns)
{
	return FALSE;
}
static U64 _pwrap_time2ns (U64 time_us)
{
	return 0;
}

#endif
//#####################################################################
//define macro and inline function (for do while loop)
//#####################################################################
typedef U32 (*loop_condition_fp)(U32);//define a function pointer

static inline U32 wait_for_fsm_idle(U32 x)
{
	return (GET_WACS0_FSM( x ) != WACS_FSM_IDLE );
}
static inline U32 wait_for_fsm_vldclr(U32 x)
{
	return (GET_WACS0_FSM( x ) != WACS_FSM_WFVLDCLR);
}
static inline U32 wait_for_sync(U32 x)
{
	return (GET_SYNC_IDLE0(x) != WACS_SYNC_IDLE);
}
static inline U32 wait_for_idle_and_sync(U32 x)
{
	return ((GET_WACS2_FSM(x) != WACS_FSM_IDLE) || (GET_SYNC_IDLE2(x) != WACS_SYNC_IDLE)) ;
}
static inline U32 wait_for_wrap_idle(U32 x)
{
	return ((GET_WRAP_FSM(x) != 0x0) || (GET_WRAP_CH_DLE_RESTCNT(x) != 0x0));
}
static inline U32 wait_for_wrap_state_idle(U32 x)
{
	return ( GET_WRAP_AG_DLE_RESTCNT( x ) != 0 ) ;
}
static inline U32 wait_for_man_idle_and_noreq(U32 x)
{
	return ( (GET_MAN_REQ(x) != MAN_FSM_NO_REQ ) || (GET_MAN_FSM(x) != MAN_FSM_IDLE) );
}
static inline U32 wait_for_man_vldclr(U32 x)
{
	return  (GET_MAN_FSM( x ) != MAN_FSM_WFVLDCLR) ;
}
static inline U32 wait_for_cipher_ready(U32 x)
{
	return (x!=1) ;
}
static inline U32 wait_for_stdupd_idle(U32 x)
{
	return ( GET_STAUPD_FSM(x) != 0x0) ;
}

static inline U32 wait_for_state_ready_init(loop_condition_fp fp,U32 timeout_us,U32 wacs_register,U32 *read_reg)
{

	U64 start_time_ns=0, timeout_ns=0;
	U32 reg_rdata=0x0;
	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(timeout_us);
	do
	{
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns))
		{
			PWRAPLOG("wait_for_state_ready_init timeout when waiting for idle\n");
			return E_PWR_WAIT_IDLE_TIMEOUT;
		}
		reg_rdata = WRAP_RD32(wacs_register);
	} while( fp(reg_rdata)); //IDLE State
	if(read_reg)
		*read_reg=reg_rdata;
	return 0;
}

static inline U32 wait_for_state_idle_init(loop_condition_fp fp,U32 timeout_us,U32 wacs_register,U32 wacs_vldclr_register,U32 *read_reg)
{

	U64 start_time_ns=0, timeout_ns=0;
	U32 reg_rdata;
	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(timeout_us);
	do
	{
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns))
		{
			PWRAPLOG("wait_for_state_idle_init timeout when waiting for idle\n");
			pwrap_dump_ap_register();
			//pwrap_trace_wacs2();
			//BUG_ON(1);
			return E_PWR_WAIT_IDLE_TIMEOUT;
		}
		reg_rdata = WRAP_RD32(wacs_register);
		//if last read command timeout,clear vldclr bit
		//read command state machine:FSM_REQ-->wfdle-->WFVLDCLR;write:FSM_REQ-->idle
		switch ( GET_WACS0_FSM( reg_rdata ) )
		{
			case WACS_FSM_WFVLDCLR:
				WRAP_WR32(wacs_vldclr_register , 1);
				PWRAPLOG("WACS_FSM = PMIC_WRAP_WACS_VLDCLR\n");
				break;
			case WACS_FSM_WFDLE:
				PWRAPLOG("WACS_FSM = WACS_FSM_WFDLE\n");
				break;
			case WACS_FSM_REQ:
				PWRAPLOG("WACS_FSM = WACS_FSM_REQ\n");
				break;
			default:
				break;
		}
	}while( fp(reg_rdata)); //IDLE State
	if(read_reg)
		*read_reg=reg_rdata;
	return 0;
}
static inline U32 wait_for_state_idle(loop_condition_fp fp,U32 timeout_us,U32 wacs_register,U32 wacs_vldclr_register,U32 *read_reg)
{

	U64 start_time_ns=0, timeout_ns=0;
	U32 reg_rdata;
	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(timeout_us);
	do
	{
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns))
		{
			PWRAPLOG("wait_for_state_idle timeout when waiting for idle\n");
			pwrap_dump_ap_register();
			// pwrap_trace_wacs2();
			// BUG_ON(1);
			return E_PWR_WAIT_IDLE_TIMEOUT;
		}
		reg_rdata = WRAP_RD32(wacs_register);
		if( GET_INIT_DONE0( reg_rdata ) != WACS_INIT_DONE)
		{
			PWRAPLOG("initialization isn't finished \n");
			return E_PWR_NOT_INIT_DONE;
		}
		//if last read command timeout,clear vldclr bit
		//read command state machine:FSM_REQ-->wfdle-->WFVLDCLR;write:FSM_REQ-->idle
		switch ( GET_WACS0_FSM( reg_rdata ) )
		{
			case WACS_FSM_WFVLDCLR:
				WRAP_WR32(wacs_vldclr_register , 1);
				PWRAPLOG("WACS_FSM = PMIC_WRAP_WACS_VLDCLR\n");
				break;
			case WACS_FSM_WFDLE:
				PWRAPLOG("WACS_FSM = WACS_FSM_WFDLE\n");
				break;
			case WACS_FSM_REQ:
				PWRAPLOG("WACS_FSM = WACS_FSM_REQ\n");
				break;
			default:
				break;
		}
	}while( fp(reg_rdata)); //IDLE State
	if(read_reg)
		*read_reg=reg_rdata;
	return 0;
}

static inline U32 wait_for_state_ready(loop_condition_fp fp,U32 timeout_us,U32 wacs_register,U32 *read_reg)
{

	U64 start_time_ns=0, timeout_ns=0;
	U32 reg_rdata;
	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(timeout_us);
	do
	{
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns))
		{
			PWRAPLOG("timeout when waiting for idle\n");
			pwrap_dump_ap_register();
			return E_PWR_WAIT_IDLE_TIMEOUT;
		}
		reg_rdata = WRAP_RD32(wacs_register);

		if( GET_INIT_DONE0( reg_rdata ) != WACS_INIT_DONE)
		{
			PWRAPLOG("initialization isn't finished \n");
			return E_PWR_NOT_INIT_DONE;
		}
	} while( fp(reg_rdata)); //IDLE State
	if(read_reg)
		*read_reg=reg_rdata;
	return 0;
}
//******************************************************************************
//--external API for pmic_wrap user-------------------------------------------------
//******************************************************************************
S32 pwrap_read( U32  adr, U32 *rdata )
{
  return pwrap_wacs2( 0, adr,0,rdata );
}

S32 pwrap_write( U32  adr, U32  wdata )
{
  return pwrap_wacs2( 1, adr,wdata,0 );
}
//--------------------------------------------------------
//    Function : pwrap_wacs2()
// Description :
//   Parameter :
//      Return :
//--------------------------------------------------------
S32 pwrap_wacs2( U32  write, U32  adr, U32  wdata, U32 *rdata )
{
	//U64 wrap_access_time=0x0;
	U32 reg_rdata=0;
	U32 wacs_write=0;
	U32 wacs_adr=0;
	U32 wacs_cmd=0;
	U32 return_value=0;

	// Check argument validation
	if( (write & ~(0x1))    != 0)  return E_PWR_INVALID_RW;
	if( (adr   & ~(0xffff)) != 0)  return E_PWR_INVALID_ADDR;
	if( (wdata & ~(0xffff)) != 0)  return E_PWR_INVALID_WDAT;

	// Check IDLE & INIT_DONE in advance
	return_value=wait_for_state_idle(wait_for_fsm_idle,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,PMIC_WRAP_WACS2_VLDCLR,0);
	if(return_value!=0)
	{
		PWRAPLOG("wait_for_fsm_idle fail,return_value=%d\n",return_value);
		goto FAIL;
	}
	wacs_write  = write << 31;
	wacs_adr    = (adr >> 1) << 16;
	wacs_cmd = wacs_write | wacs_adr | wdata;

	WRAP_WR32(PMIC_WRAP_WACS2_CMD,wacs_cmd);
	if( write == 0 )
	{
		if (NULL == rdata)
		{
			PWRAPLOG("rdata is a NULL pointer\n");
			return_value= E_PWR_INVALID_ARG;
			WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR, 1);
			goto FAIL;
		}
		return_value=wait_for_state_ready(wait_for_fsm_vldclr,TIMEOUT_READ,PMIC_WRAP_WACS2_RDATA,&reg_rdata);
		if(return_value!=0)
		{
			PWRAPLOG("wait_for_fsm_vldclr fail,return_value=%d\n",return_value);
			return_value+=1;//E_PWR_NOT_INIT_DONE_READ or E_PWR_WAIT_IDLE_TIMEOUT_READ
			goto FAIL;
		}

		*rdata = GET_WACS0_RDATA( reg_rdata );
		WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR , 1);
	}
FAIL:

	if(return_value!=0)
	{
		PWRAPLOG("pwrap_wacs2 fail,return_value=%d\n",return_value);
		PWRAPLOG("timeout:BUG_ON here\n");
		//BUG_ON(1);
	}
	//wrap_access_time=_pwrap_get_current_time();
	//pwrap_trace(wrap_access_time,return_value,write, adr, wdata,rdata);
	return return_value;
}
//******************************************************************************
//--internal API for pwrap_init-------------------------------------------------
//******************************************************************************

//--------------------------------------------------------
//    Function : _pwrap_wacs2_nochk()
// Description :
//   Parameter :
//      Return :
//--------------------------------------------------------
static S32 pwrap_read_nochk( U32  adr, U32 *rdata )
{
	return _pwrap_wacs2_nochk( 0, adr,  0, rdata );
}

static S32 pwrap_write_nochk( U32  adr, U32  wdata )
{
	return _pwrap_wacs2_nochk( 1, adr,wdata,0 );
}

static S32 _pwrap_wacs2_nochk( U32 write, U32 adr, U32 wdata, U32 *rdata )
{
	U32 reg_rdata=0x0;
	U32 wacs_write=0x0;
	U32 wacs_adr=0x0;
	U32 wacs_cmd=0x0;
	U32 return_value=0x0;
	//PWRAPFUC();
	// Check argument validation
	if( (write & ~(0x1))    != 0)  return E_PWR_INVALID_RW;
	if( (adr   & ~(0xffff)) != 0)  return E_PWR_INVALID_ADDR;
	if( (wdata & ~(0xffff)) != 0)  return E_PWR_INVALID_WDAT;

	// Check IDLE
	return_value=wait_for_state_ready_init(wait_for_fsm_idle,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,0);
	if(return_value!=0)
	{
		PWRAPLOG("_pwrap_wacs2_nochk write command fail,return_value=%x\n", return_value);
		return return_value;
	}

	wacs_write  = write << 31;
	wacs_adr    = (adr >> 1) << 16;
	wacs_cmd = wacs_write | wacs_adr | wdata;
	WRAP_WR32(PMIC_WRAP_WACS2_CMD,wacs_cmd);

	if( write == 0 )
	{
		if (NULL == rdata)
		{
			PWRAPLOG("rdata is a NULL pointer\n");
			WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR, 1);
			return E_PWR_INVALID_ARG;
		}
		return_value=wait_for_state_ready_init(wait_for_fsm_vldclr,TIMEOUT_READ,PMIC_WRAP_WACS2_RDATA,&reg_rdata);
		if(return_value!=0)
		{
			PWRAPLOG("wait_for_fsm_vldclr fail,return_value=%d\n",return_value);
			return_value+=1;//E_PWR_NOT_INIT_DONE_READ or E_PWR_WAIT_IDLE_TIMEOUT_READ
			return return_value;
		}
		*rdata = GET_WACS0_RDATA( reg_rdata );
		WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR , 1);
	}
	return 0;
}
//--------------------------------------------------------
//    Function : _pwrap_init_dio()
// Description :call it in pwrap_init,mustn't check init done
//   Parameter :
//      Return :
//--------------------------------------------------------
static S32 _pwrap_init_dio( U32 dio_en )
{
	U32 arb_en_backup=0x0;
	U32 rdata=0x0;
	U32 return_value=0;

	//PWRAPFUC();
	arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);
	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, WACS2); // only WACS2

#ifdef SLV_6350
	pwrap_write_nochk(MT6350_DEW_DIO_EN, dio_en);
#endif

	// Check IDLE & INIT_DONE in advance
	return_value=wait_for_state_ready_init(wait_for_idle_and_sync,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,0);
	if(return_value!=0)
	{
		PWRAPLOG("_pwrap_init_dio fail,return_value=%x\n", return_value);
		return return_value;
	}
	//enable AP DIO mode
	WRAP_WR32(PMIC_WRAP_DIO_EN , dio_en);
	// Read Test
#ifdef SLV_6350  
	pwrap_read_nochk(MT6350_DEW_READ_TEST, &rdata);
	if( rdata != MT6350_DEFAULT_VALUE_READ_TEST )
	{
		PWRAPLOG("[Dio_mode][Read Test] fail,dio_en = %x, READ_TEST rdata=%x, exp=0x5aa5\n", dio_en, rdata);
		return E_PWR_READ_TEST_FAIL;
	}
#endif

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , arb_en_backup);
	return 0;
}

//--------------------------------------------------------
//    Function : _pwrap_init_cipher()
// Description :
//   Parameter :
//      Return :
//--------------------------------------------------------
static S32 _pwrap_init_cipher( void )
{
	U32 arb_en_backup=0;
	U32 rdata=0;
	U32 return_value=0;
	U32 start_time_ns=0, timeout_ns=0;
	//PWRAPFUC();
	arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , WACS2); // only WACS0

	WRAP_WR32(PMIC_WRAP_CIPHER_SWRST , 1);
	WRAP_WR32(PMIC_WRAP_CIPHER_SWRST , 0);
	WRAP_WR32(PMIC_WRAP_CIPHER_KEY_SEL , 1);
	WRAP_WR32(PMIC_WRAP_CIPHER_IV_SEL  , 2);
	WRAP_WR32(PMIC_WRAP_CIPHER_EN   , 1);

	//Config CIPHER @ PMIC
#ifdef SLV_6350 
	pwrap_write_nochk(MT6350_DEW_CIPHER_SWRST, 0x1);
	pwrap_write_nochk(MT6350_DEW_CIPHER_SWRST, 0x0);
	pwrap_write_nochk(MT6350_DEW_CIPHER_KEY_SEL, 0x1);
	pwrap_write_nochk(MT6350_DEW_CIPHER_IV_SEL,  0x2);
	pwrap_write_nochk(MT6350_DEW_CIPHER_EN,  0x1);
#endif

	//wait for cipher data ready@AP
	return_value=wait_for_state_ready_init(wait_for_cipher_ready,TIMEOUT_WAIT_IDLE,PMIC_WRAP_CIPHER_RDY,0);
	if(return_value!=0)
	{
		PWRAPLOG("wait for cipher data ready@AP fail,return_value=%x\n", return_value);
		return return_value;
	}

	//wait for cipher data ready@PMIC
#ifdef SLV_6350
	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(0xFFFFFF);
	do
	{
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns))
		{
			PWRAPLOG("wait for cipher data ready@PMIC\n");
		}
		pwrap_read_nochk(MT6350_DEW_CIPHER_RDY,&rdata);
	} while( rdata != 0x1 ); //cipher_ready

	pwrap_write_nochk(MT6350_DEW_CIPHER_MODE, 0x1);
#endif
	//wait for cipher mode idle
	return_value=wait_for_state_ready_init(wait_for_idle_and_sync,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,0);
	if(return_value!=0)
	{
		PWRAPLOG("wait for cipher mode idle fail,return_value=%x\n", return_value);
		return return_value;
	}
	WRAP_WR32(PMIC_WRAP_CIPHER_MODE , 1);

	// Read Test
#ifdef SLV_6350
	  pwrap_read_nochk(MT6350_DEW_READ_TEST, &rdata);
	  if( rdata != MT6350_DEFAULT_VALUE_READ_TEST )
	  {
		PWRAPLOG("_pwrap_init_cipher,read test error,error code=%x, rdata=%x\n", 1, rdata);
		return E_PWR_READ_TEST_FAIL;
	  }
#endif

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , arb_en_backup);
	return 0;
}

//--------------------------------------------------------
//    Function : _pwrap_init_sistrobe()
// Description : Initialize SI_CK_CON and SIDLY
//   Parameter :
//      Return :
//--------------------------------------------------------
static S32 _pwrap_init_sistrobe( void )
{
	U32 arb_en_backup=0;
	U32 rdata=0;
	U32 i=0;
	S32 ind=0;
	U32 tmp1=0;
	U32 tmp2=0;
	U32 result_faulty=0;
	U32 result[2]={0,0};
	S32 leading_one[2]={-1,-1};
	S32 tailing_one[2]={-1,-1};

	arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN ,WACS2); // only WACS2

	//---------------------------------------------------------------------
	// Scan all possible input strobe by READ_TEST
	//---------------------------------------------------------------------
	for( ind=0; ind<24; ind++)  // 24 sampling clock edge
	{
		WRAP_WR32(PMIC_WRAP_SI_CK_CON , (ind >> 2) & 0x7);
		WRAP_WR32(PMIC_WRAP_SIDLY ,0x3 - (ind & 0x3));
#ifdef SLV_6350
		_pwrap_wacs2_nochk(0, MT6350_DEW_READ_TEST, 0, &rdata);
		if( rdata == MT6350_DEFAULT_VALUE_READ_TEST ) {
			PWRAPERR("_pwrap_init_sistrobe [Read Test of MT6350] pass,index=%d rdata=%x\n", ind,rdata);
			result[0] |= (0x1 << ind);
		}else {
			PWRAPERR("_pwrap_init_sistrobe [Read Test of MT6350] tuning,index=%d rdata=%x\n", ind,rdata);
		}
#endif
	}
#ifndef SLV_6350
	  result[0] = result[1];
#endif
#ifndef SLV_6332
	  result[1] = result[0];
#endif
	 //---------------------------------------------------------------------
  	// Locate the leading one and trailing one of PMIC 1/2
  	//---------------------------------------------------------------------
	for( ind=23 ; ind>=0 ; ind-- )
	{
	  if( (result[0] & (0x1 << ind)) && leading_one[0] == -1){
		  leading_one[0] = ind;
	  }
	  if(leading_one[0] > 0) { break;}
	}
	for( ind=23 ; ind>=0 ; ind-- )
	{
	  if( (result[1] & (0x1 << ind)) && leading_one[1] == -1){
		  leading_one[1] = ind;
	  }
	  if(leading_one[1] > 0) { break;}
	}

	for( ind=0 ; ind<24 ; ind++ )
	{
	  if( (result[0] & (0x1 << ind)) && tailing_one[0] == -1){
		  tailing_one[0] = ind;
	  }
	  if(tailing_one[0] > 0) { break;}
	}
	for( ind=0 ; ind<24 ; ind++ )
	{
	  if( (result[1] & (0x1 << ind)) && tailing_one[1] == -1){
		  tailing_one[1] = ind;
	  }
	  if(tailing_one[1] > 0) { break;}
	}

  	//---------------------------------------------------------------------
  	// Check the continuity of pass range
  	//---------------------------------------------------------------------
  	for( i=0; i<2; i++)
  	{
    		tmp1 = (0x1 << (leading_one[i]+1)) - 1;
    		tmp2 = (0x1 << tailing_one[i]) - 1;
    		if( (tmp1 - tmp2) != result[i] )
    		{
    			/*TERR = "[DrvPWRAP_InitSiStrobe] Fail at PMIC %d, result = %x, leading_one:%d, tailing_one:%d"
    	         	 , i+1, result[i], leading_one[i], tailing_one[i]*/
    	    		PWRAPERR("_pwrap_init_sistrobe Fail at PMIC %d, result = %x, leading_one:%d, tailing_one:%d\n", i+1, result[i], leading_one[i], tailing_one[i]);
      			result_faulty = 0x1;
    		}
  	}

	//---------------------------------------------------------------------
	// Config SICK and SIDLY to the middle point of pass range
	//---------------------------------------------------------------------
	if( result_faulty == 0 )
    {
  		// choose the best point in the interaction of PMIC1's pass range and PMIC2's pass range
    	ind = ( (leading_one[0] + tailing_one[0])/2 + (leading_one[1] + tailing_one[1])/2 )/2;
        /*TINFO = "The best point in the interaction area is %d, ind"*/
		WRAP_WR32(PMIC_WRAP_SI_CK_CON , (ind >> 2) & 0x7);
		WRAP_WR32(PMIC_WRAP_SIDLY , 0x3 - (ind & 0x3));
		//---------------------------------------------------------------------
		// Restore
		//---------------------------------------------------------------------
		WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , arb_en_backup);
		return 0;
	}
	else
	{
		PWRAPERR("_pwrap_init_sistrobe Fail,result_faulty=%x\n", result_faulty);
		return result_faulty;
	}
}

//--------------------------------------------------------
//    Function : _pwrap_reset_spislv()
// Description :
//   Parameter :
//      Return :
//--------------------------------------------------------
static S32 _pwrap_reset_spislv( void )
{
	U32 ret=0;
	U32 return_value=0;
	//PWRAPFUC();
	// This driver does not using _pwrap_switch_mux
	// because the remaining requests are expected to fail anyway

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , DISABLE_ALL);
	WRAP_WR32(PMIC_WRAP_WRAP_EN , DISABLE);
	WRAP_WR32(PMIC_WRAP_MUX_SEL , MANUAL_MODE);
	WRAP_WR32(PMIC_WRAP_MAN_EN ,ENABLE);
	WRAP_WR32(PMIC_WRAP_DIO_EN ,DISABLE);

	WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_CSL  << 8));//0x2100
	WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8)); //0x2800//to reset counter
	WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_CSH  << 8));//0x2000
	WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));
	WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));
	WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));
	WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));

	return_value=wait_for_state_ready_init(wait_for_sync,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,0);
	if(return_value!=0)
	{
		PWRAPLOG("_pwrap_reset_spislv fail,return_value=%x\n", return_value);
		ret=E_PWR_TIMEOUT;
		goto timeout;
	}

	WRAP_WR32(PMIC_WRAP_MAN_EN ,DISABLE);
	WRAP_WR32(PMIC_WRAP_MUX_SEL ,WRAPPER_MODE);

timeout:
	WRAP_WR32(PMIC_WRAP_MAN_EN, DISABLE);
	WRAP_WR32(PMIC_WRAP_MUX_SEL, WRAPPER_MODE);
	return ret;
}

static void __pwrap_soft_reset(void)
{
	PWRAPLOG("start reset wrapper\n");
	PWRAP_SOFT_RESET;
	PWRAPLOG("the reset register =%x\n",WRAP_RD32(INFRA_GLOBALCON_RST0));
	PWRAPLOG("PMIC_WRAP_STAUPD_GRPEN =0x%x,it should be equal to 0xc\n",WRAP_RD32(PMIC_WRAP_STAUPD_GRPEN));
	//clear reset bit
	PWRAP_CLEAR_SOFT_RESET_BIT;
	return;
}

static S32 _pwrap_init_reg_clock( U32 regck_sel )
{
	U32 wdata=0;
	U32 rdata=0;
	PWRAPFUC();
	
	if(regck_sel == 1) // not supported in 6323!!
		return E_PWR_INIT_REG_CLOCK;
	else
		wdata = 0x3;
    
	pwrap_write_nochk(MT6350_TOP_CKCON1_CLR, wdata);
	pwrap_read_nochk(MT6350_TOP_CKCON1,  &rdata);    
#if defined SLV_6350
	if( (rdata & 0x3) != 0)
	{
		PWRAPLOG("_pwrap_init_reg_clock,MT6350_TOP_CKCON1 Write [15]=1 Fail, rdata=%x\n",rdata);
		return E_PWR_WRITE_TEST_FAIL;
	}
	pwrap_write_nochk(MT6350_DEW_RDDMY_NO, 0x8);
	WRAP_WR32(PMIC_WRAP_RDDMY ,0x8);    
#endif  

	// Config SPI Waveform according to reg clk
	if( regck_sel == 1 ) { // 6MHz in 6323  => no support ; 18MHz in 6320
		WRAP_WR32(PMIC_WRAP_CSHEXT_WRITE , 0x4);
		WRAP_WR32(PMIC_WRAP_CSHEXT_READ	 , 0x5);  // wait data written into register => 3T_PMIC: consists of CSLEXT_END(1T) + CSHEXT(6T)
		WRAP_WR32(PMIC_WRAP_CSLEXT_START , 0x0);
		WRAP_WR32(PMIC_WRAP_CSLEXT_END   , 0x0);
	}else if(regck_sel == 2 )
	{ // 12MHz in 6323; 36MHz in 6320
#if defined SLV_6350
		WRAP_WR32(PMIC_WRAP_CSHEXT_READ, 0x0);
		WRAP_WR32(PMIC_WRAP_CSHEXT_WRITE, 0x7);  // wait data written into register => 3T_PMIC: consists of CSLEXT_END(1T) + CSHEXT(6T)
#endif
		WRAP_WR32(PMIC_WRAP_CSLEXT_START , 0x0);
		WRAP_WR32(PMIC_WRAP_CSLEXT_END   , 0x0);
	}
    else { //Safe mode
		WRAP_WR32(PMIC_WRAP_CSHEXT_WRITE , 0xf);
		WRAP_WR32(PMIC_WRAP_CSHEXT_READ	 , 0xf);
		WRAP_WR32(PMIC_WRAP_CSLEXT_START , 0xf);
		WRAP_WR32(PMIC_WRAP_CSLEXT_END   , 0xf);
	}

	return 0;
}

/*
 * pmic_wrap init,init wrap interface
 */
S32 pwrap_init(void)
{
	S32 sub_return=0;
	S32 sub_return1=0;
  	U32 clk_sel = 0;
	//S32 ret=0;
	U32 rdata=0x0;
	//U32 timeout=0;
	u32 cg_mask = 0;
	u32 backup = 0;

	PWRAPFUC();

	//###############################
	//toggle PMIC_WRAP and pwrap_spictl reset
	//###############################
	//   WRAP_SET_BIT(0x80,INFRA_GLOBALCON_RST0);
	//   WRAP_CLR_BIT(0x80,INFRA_GLOBALCON_RST0);
	//__pwrap_soft_reset();

	/* Turn off module clock */
	cg_mask = ((1 << 20) | (1 << 27) | (1 << 28) | (1 << 29));
	backup = (~WRAP_RD32(CLK_SWCG_1)) & cg_mask;
	WRAP_WR32(CLK_SETCG_1, cg_mask);
	/* dummy read to add latency (to wait clock turning off) */
	rdata = WRAP_RD32(PMIC_WRAP_SWRST);

	/* Toggle module reset */
	WRAP_WR32(PMIC_WRAP_SWRST, ENABLE);

	rdata = WRAP_RD32(WDT_FAST_SWSYSRST);
	WRAP_WR32(WDT_FAST_SWSYSRST, (rdata | (0x1 << 11)) | (0x88 << 24));
	WRAP_WR32(WDT_FAST_SWSYSRST, (rdata & (~(0x1 << 11))) | (0x88 << 24));
	WRAP_WR32(PMIC_WRAP_SWRST, DISABLE);

	/* Turn on module clock */
	WRAP_WR32(CLK_CLRCG_1, backup | (1 << 20)); // ensure cg for AP is off;

	/* Turn on module clock dcm (in global_con) */
	// WHQA_00014186: set PMIC bclk DCM default off due to HW issue
	WRAP_WR32(CLK_SETCG_3, (1 << 2) | (1 << 1));
	// WRAP_WR32(CLK_SETCG_3, (1 << 2));

	//###############################
	// Set SPI_CK_freq = 26MHz 
	//###############################
#ifndef CONFIG_MTK_FPGA
	clk_sel = WRAP_RD32(CLK_SEL_0);
	WRAP_WR32(CLK_SEL_0, clk_sel | (0x3 << 24));
#endif

	//###############################
	//Enable DCM
	//###############################
	WRAP_WR32(PMIC_WRAP_DCM_EN, 1);//enable CRC DCM and PMIC_WRAP DCM
	WRAP_WR32(PMIC_WRAP_DCM_DBC_PRD, DISABLE); //no debounce
	//###############################
	//Reset SPISLV
	//###############################
	sub_return=_pwrap_reset_spislv();
	if( sub_return != 0 )
	{
		PWRAPLOG("error,_pwrap_reset_spislv fail,sub_return=%x\n",sub_return);
		return E_PWR_INIT_RESET_SPI;
	}
	//###############################
	// Enable WACS2
	//###############################
	WRAP_WR32(PMIC_WRAP_WRAP_EN, ENABLE);//enable wrap
	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, WACS2); //Only WACS2
	WRAP_WR32(PMIC_WRAP_WACS2_EN, ENABLE);

	//###############################
    // Set Dummy cycle to make the cycle is the same at both AP and PMIC sides
    //###############################
  // (default value of 6320 dummy cycle is already 0x8)
#ifdef SLV_6350
	WRAP_WR32(PMIC_WRAP_RDDMY , 0xF);
#endif

	//###############################
	// Input data calibration flow;
	//###############################
	sub_return = _pwrap_init_sistrobe();
	if( sub_return != 0 )
	{
		PWRAPLOG("error,DrvPWRAP_InitSiStrobe fail,sub_return=%x\n",sub_return);
		return E_PWR_INIT_SIDLY;
	}

	//###############################
	// SPI Waveform Configuration
	//###############################
	//0:safe mode, 1:18MHz
	sub_return = _pwrap_init_reg_clock(2);
	if( sub_return != 0)
	{
		PWRAPLOG("error,_pwrap_init_reg_clock fail,sub_return=%x\n",sub_return);
		return E_PWR_INIT_REG_CLOCK;
	}

	//###############################
	// Enable DIO mode
	//###############################
	//PMIC2 dual io not ready

	sub_return = _pwrap_init_dio(1);
	if( sub_return != 0 )
	{
		PWRAPLOG("_pwrap_init_dio test error,error code=%x, sub_return=%x\n", 0x11, sub_return);
		return E_PWR_INIT_DIO;
	}

	//###############################
	// Enable Encryption
	//###############################

	sub_return = _pwrap_init_cipher();
	if( sub_return != 0 )
	{
		PWRAPLOG("Enable Encryption fail, return=%x\n", sub_return);
		return E_PWR_INIT_CIPHER;
	}

	//###############################
	// Write test using WACS2
	//###############################
	//check Wtiet test default value
	
#ifdef SLV_6350
	  sub_return = pwrap_write_nochk(MT6350_DEW_WRITE_TEST, MT6350_WRITE_TEST_VALUE);
	  sub_return1 = pwrap_read_nochk(MT6350_DEW_WRITE_TEST, &rdata);
	  if( rdata != MT6350_WRITE_TEST_VALUE )  {
		PWRAPLOG("write test error,rdata=0x%x,exp=0xa55a,sub_return=0x%x,sub_return1=0x%x\n", rdata,sub_return,sub_return1);
		return E_PWR_INIT_WRITE_TEST;
	  }
#endif
	
	//###############################
	// Signature Checking - Using CRC
	// should be the last to modify WRITE_TEST
	//###############################
	sub_return = pwrap_write_nochk(MT6350_DEW_CRC_EN, ENABLE);
	if( sub_return != 0 )
	{
		PWRAPLOG("Enable CRC fail, return=%x\n", sub_return);
		return E_PWR_INIT_ENABLE_CRC;
	}
	WRAP_WR32(PMIC_WRAP_CRC_EN, ENABLE);
	WRAP_WR32(PMIC_WRAP_SIG_MODE, CHECK_CRC);
	WRAP_WR32(PMIC_WRAP_SIG_ADR, MT6350_DEW_CRC_VAL);    

	//###############################
	// PMIC_WRAP enables
	//###############################
	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, 0x1ff);
	WRAP_WR32(PMIC_WRAP_WACS0_EN,ENABLE);
	WRAP_WR32(PMIC_WRAP_WACS1_EN,ENABLE);
	WRAP_WR32(PMIC_WRAP_STAUPD_PRD, 0x5);  //0x1:20us,for concurrence test,MP:0x5;  //100us
	WRAP_WR32(PMIC_WRAP_STAUPD_GRPEN, 0xff);	
	WRAP_WR32(PMIC_WRAP_WDT_UNIT,0xf);
	WRAP_WR32(PMIC_WRAP_WDT_SRC_EN,0xffffffff);
	WRAP_WR32(PMIC_WRAP_TIMER_EN,0x1);
	WRAP_WR32(PMIC_WRAP_INT_EN, 0x7ffffffd); 

	//###############################
	// GPS Initialization
	// set address of ready bit register and thermal data register
	//###############################
	WRAP_WR32(PMIC_WRAP_ADC_CMD_ADDR, MT6350_AUXADC_CON21);
	WRAP_WR32(PMIC_WRAP_PWRAP_ADC_CMD, 0x8000);
	WRAP_WR32(PMIC_WRAP_ADC_RDY_ADDR, MT6350_AUXADC_ADC12);
	WRAP_WR32(PMIC_WRAP_ADC_RDATA_ADDR1, MT6350_AUXADC_ADC13);
	WRAP_WR32(PMIC_WRAP_ADC_RDATA_ADDR2, MT6350_AUXADC_ADC14);

	//###############################
	// Initialization Done
	//###############################
	WRAP_WR32(PMIC_WRAP_INIT_DONE2 , ENABLE);

	//###############################
	//TBD: Should be configured by MD MCU
	//###############################
	WRAP_WR32(PMIC_WRAP_INIT_DONE0 ,ENABLE);
	WRAP_WR32(PMIC_WRAP_INIT_DONE1 , ENABLE);

	return 0;
}

/*-pwrap debug--------------------------------------------------------------------------*/
void pwrap_dump_ap_register(void)
{
	U32 i = 0;
	PWRAPREG("dump pwrap register, base=0x%p\n", PMIC_WRAP_BASE);
	PWRAPREG("address     :   3 2 1 0    7 6 5 4    B A 9 8    F E D C \n");
	for (i = 0; i <= PMIC_WRAP_REG_RANGE; i += 4)
	{
		PWRAPREG("offset 0x%.3x:0x%.8x 0x%.8x 0x%.8x 0x%.8x \n",i * 4,
				WRAP_RD32(PMIC_WRAP_BASE + (i * 4) + 0),
				WRAP_RD32(PMIC_WRAP_BASE + (i * 4) + 0x4),
				WRAP_RD32(PMIC_WRAP_BASE + (i * 4) + 0x8),
				WRAP_RD32(PMIC_WRAP_BASE + (i * 4) + 0xC));
	}
	//PWRAPREG("elapse_time=%llx(ns)\n",elapse_time);
}

static inline void pwrap_dump_pmic_register(void)
{
#if 0
	U32 i=0;
	U32 reg_addr=0;
	U32 reg_value=0;

	PWRAPREG("dump dewrap register\n");
	for(i=0;i<=14;i++)
	{
		reg_addr=(DEW_BASE+i*4);
		reg_value=pwrap_read_nochk(reg_addr,&reg_value);
		PWRAPREG("0x%x=0x%x\n",reg_addr,reg_value);
	}
#endif
	return;
}

static inline void pwrap_dump_all_register(void)
{
	pwrap_dump_ap_register();
	pwrap_dump_pmic_register();
	return;
}
S32 pwrap_init_preloader ( void )
{
	u32 pwrap_ret=0,i=0;
	PWRAPFUC();
	for(i=0;i<3;i++)
	{
		pwrap_ret = pwrap_init();
		if(pwrap_ret!=0)
		{
			printf("[PMIC_WRAP]wrap_init fail,the return value=%x.\n",pwrap_ret);
		}
		else
		{
			printf("[PMIC_WRAP]wrap_init pass,the return value=%x.\n",pwrap_ret);
			break;//init pass
		}
	}
#ifdef PWRAP_PRELOADER_PORTING
  //pwrap_init_for_early_porting();
#endif
	return 0;
}
#ifdef PWRAP_PRELOADER_PORTING

//--------------------------------------------------------
//    Function : _pwrap_status_update_test()
// Description :only for early porting
//   Parameter :
//      Return :
//--------------------------------------------------------
static S32 _pwrap_status_update_test_porting( void )
{
 	//U32 i, j;
 	U32 rdata;
 	volatile U32 delay=1000*1000*1;
 	PWRAPFUC();
 	//disable signature interrupt
 	WRAP_WR32(PMIC_WRAP_INT_EN,0x0);
#ifdef SLV_6350
	pwrap_write(MT6350_DEW_WRITE_TEST, MT6350_WRITE_TEST_VALUE);
	WRAP_WR32(PMIC_WRAP_SIG_ADR,MT6350_DEW_WRITE_TEST);
	WRAP_WR32(PMIC_WRAP_SIG_VALUE,0xAA55);
#endif
	WRAP_WR32(PMIC_WRAP_SIG_MODE, 0x1);

	while(delay--);

	rdata=WRAP_RD32(PMIC_WRAP_SIG_ERRVAL);
#ifdef SLV_6350
	if( (rdata&0xFFFF) != MT6350_WRITE_TEST_VALUE )
	{
		PWRAPLOG("MT6350 _pwrap_status_update_test error,error code=%x, rdata=%x\n", 1, (rdata&0xFFFF));
		//return 1;
	}
#endif
#ifdef SLV_6350
	WRAP_WR32(PMIC_WRAP_SIG_VALUE,MT6350_WRITE_TEST_VALUE);//tha same as write test
#endif

	//clear sig_error interrupt flag bit
	WRAP_WR32(PMIC_WRAP_INT_CLR,1<<1);

	//enable signature interrupt
	WRAP_WR32(PMIC_WRAP_INT_EN,0x7ffffff9);
	WRAP_WR32(PMIC_WRAP_SIG_MODE, 0x0);
	#ifdef SLV_6350
		WRAP_WR32(PMIC_WRAP_SIG_ADR, MT6350_DEW_CRC_VAL);
	#endif
	return 0;
}

void pwrap_init_for_early_porting(void)
{
	int ret = 0;
	U32 res=0;
	PWRAPFUC();

	ret=_pwrap_status_update_test_porting();
	if(ret==0)
	{
		PWRAPLOG("wrapper_StatusUpdateTest pass.\n");
	}
	else
	{
		PWRAPLOG("error:wrapper_StatusUpdateTest fail.\n");
		res+=1;
	}
}
#endif //PWRAP_PRELOADER_PORTING
#endif // PMIC_WRAP_NO_PMIC

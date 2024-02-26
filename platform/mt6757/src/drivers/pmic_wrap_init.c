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

#include <timer.h>
#include <typedefs.h>
#include <gpio.h>
#include "pmic_wrap_init.h"

/*********************start ---internal API**************************************/
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
/************* end--internal API************************************************/

#ifdef PMIC_WRAP_NO_PMIC
S32 pwrap_wacs2( U32  write, U32  adr, U32  wdata, U32 *rdata )
{
	PWRAPERR("there is no PMIC real chip,PMIC_WRAP do Nothing\n");
	return 0;
}

/**************external API for pmic_wrap user***************************/

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
        if(pwrap_ret!=0) {
            print("[PMIC_WRAP]wrap_init fail,return=%x.\n",pwrap_ret);
        } else {
            print("[PMIC_WRAP]wrap_init pass,return=%x.\n",pwrap_ret);
            break;
        }
    }

    return 0;
}

#else
/******************************************************************************
 wrapper timeout
******************************************************************************/
#define PWRAP_TIMEOUT
/*use the same API name with kernel driver
 *however,the timeout API in uboot use tick instead of ns */
#ifdef PWRAP_TIMEOUT
static U64 _pwrap_get_current_time(void)
{
    return gpt4_get_current_tick();
}

static BOOL _pwrap_timeout_ns (U64 start_time, U64 elapse_time)
{
    return gpt4_timeout_tick(start_time, elapse_time);
}

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

typedef U32 (*loop_condition_fp)(U32);

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
    return (x!=3) ;
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

    do {
        if (_pwrap_timeout_ns(start_time_ns, timeout_ns)) {
            PWRAPERR("wait_for_state_ready_init timeout when waiting for idle\n");
            return E_PWR_WAIT_IDLE_TIMEOUT;
        }
        reg_rdata = WRAP_RD32(wacs_register);
    } while( fp(reg_rdata));

    if(read_reg)
        *read_reg = reg_rdata;

    return 0;
}

static inline U32 wait_for_state_idle_init(loop_condition_fp fp,U32 timeout_us,U32 wacs_register,U32 wacs_vldclr_register,U32 *read_reg)
{

    U64 start_time_ns=0, timeout_ns=0;
    U32 reg_rdata;

    start_time_ns = _pwrap_get_current_time();
    timeout_ns = _pwrap_time2ns(timeout_us);

    do {
        if (_pwrap_timeout_ns(start_time_ns, timeout_ns))
            {
            PWRAPERR("wait_for_state_idle_init timeout when waiting for idle\n");
            pwrap_dump_ap_register();
            return E_PWR_WAIT_IDLE_TIMEOUT;
        }
        reg_rdata = WRAP_RD32(wacs_register);
    /*if last read command timeout,clear vldclr bit
       *read command state machine:FSM_REQ-->wfdle-->WFVLDCLR;write:FSM_REQ-->idle */
    switch ( GET_WACS0_FSM( reg_rdata ) )
    {
        case WACS_FSM_WFVLDCLR:
             WRAP_WR32(wacs_vldclr_register , 1);
             PWRAPERR("WACS_FSM = PMIC_WRAP_WACS_VLDCLR\n");
             break;
        case WACS_FSM_WFDLE:
             PWRAPERR("WACS_FSM = WACS_FSM_WFDLE\n");
             break;
        case WACS_FSM_REQ:
             PWRAPERR("WACS_FSM = WACS_FSM_REQ\n");
             break;
        default:
            break;
    }
    }while( fp(reg_rdata)); /* IDLE State */

    if(read_reg)
       *read_reg = reg_rdata;

    return 0;
}
static inline U32 wait_for_state_idle(loop_condition_fp fp,U32 timeout_us,U32 wacs_register,U32 wacs_vldclr_register,U32 *read_reg)
{

    U64 start_time_ns=0, timeout_ns=0;
    U32 reg_rdata;

    start_time_ns = _pwrap_get_current_time();
    timeout_ns = _pwrap_time2ns(timeout_us);

    do {
        if (_pwrap_timeout_ns(start_time_ns, timeout_ns)) {
            PWRAPERR("wait_for_state_idle timeout when waiting for idle\n");
            pwrap_dump_ap_register();
            return E_PWR_WAIT_IDLE_TIMEOUT;
        }
        reg_rdata = WRAP_RD32(wacs_register);
        if( GET_INIT_DONE0( reg_rdata ) != WACS_INIT_DONE) {
            PWRAPERR("initialization isn't finished \n");
            return E_PWR_NOT_INIT_DONE;
        }
    /* if last read command timeout,clear vldclr bit
        *read command state machine:FSM_REQ-->wfdle-->WFVLDCLR;write:FSM_REQ-->idle */
    switch ( GET_WACS0_FSM( reg_rdata ) )
    {
        case WACS_FSM_WFVLDCLR:
            WRAP_WR32(wacs_vldclr_register , 1);
            PWRAPERR("WACS_FSM = PMIC_WRAP_WACS_VLDCLR\n");
            break;
        case WACS_FSM_WFDLE:
            PWRAPERR("WACS_FSM = WACS_FSM_WFDLE\n");
            break;
        case WACS_FSM_REQ:
            PWRAPERR("WACS_FSM = WACS_FSM_REQ\n");
            break;
        default:
           break;
    }
    }while( fp(reg_rdata)); /*IDLE State*/

    if(read_reg)
        *read_reg = reg_rdata;

    return 0;
}

static inline U32 wait_for_state_ready(loop_condition_fp fp,U32 timeout_us,U32 wacs_register,U32 *read_reg)
{

    U64 start_time_ns=0, timeout_ns=0;
    U32 reg_rdata;

    start_time_ns = _pwrap_get_current_time();
    timeout_ns = _pwrap_time2ns(timeout_us);

	do {
        if (_pwrap_timeout_ns(start_time_ns, timeout_ns)) {
            PWRAPERR("timeout when waiting for idle\n");
            pwrap_dump_ap_register();
            return E_PWR_WAIT_IDLE_TIMEOUT;
        }
        reg_rdata = WRAP_RD32(wacs_register);
        if( GET_INIT_DONE0( reg_rdata ) != WACS_INIT_DONE) {
            PWRAPERR("initialization isn't finished \n");
            return E_PWR_NOT_INIT_DONE;
        }
    } while( fp(reg_rdata)); /*IDLE State */

    if(read_reg)
        *read_reg = reg_rdata;

    return 0;
}


/*********************** external API for pmic_wrap user ************************/

S32 pwrap_read( U32  adr, U32 *rdata )
{
    return pwrap_wacs2(0,adr,0,rdata);
}

S32 pwrap_write( U32  adr, U32  wdata )
{
    return pwrap_wacs2(1,adr,wdata,0);
}

/******************************************************
 * Function : pwrap_wacs2()
 * Description :
 * Parameter :
 * Return :
 ******************************************************/
S32 pwrap_wacs2( U32  write, U32  adr, U32  wdata, U32 *rdata )
{
    U64 wrap_access_time=0x0;
    U32 res=0;
    U32 reg_rdata=0;
    U32 wacs_write=0;
    U32 wacs_adr=0;
    U32 wacs_cmd=0;
    U32 return_value=0;
    unsigned long flags=0;

    /* Check argument validation */
    if( (write & ~(0x1))    != 0)  return E_PWR_INVALID_RW;
    if( (adr   & ~(0xffff)) != 0)  return E_PWR_INVALID_ADDR;
    if( (wdata & ~(0xffff)) != 0)  return E_PWR_INVALID_WDAT;

    /* Check IDLE & INIT_DONE in advance */
    return_value = wait_for_state_idle(wait_for_fsm_idle,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,PMIC_WRAP_WACS2_VLDCLR,0);
    if( return_value != 0 ) {
        PWRAPERR("wait_for_fsm_idle fail,return_value=%d\n",return_value);
        goto FAIL;
    }

    wacs_write  = write << 31;
    wacs_adr    = (adr >> 1) << 16;
    wacs_cmd = wacs_write | wacs_adr | wdata;
    WRAP_WR32(PMIC_WRAP_WACS2_CMD,wacs_cmd);

    if( write == 0 ) {
        if (NULL == rdata) {
            PWRAPERR("rdata is a NULL pointer\n");
            return_value= E_PWR_INVALID_ARG;
            goto FAIL;
        }
        return_value = wait_for_state_ready(wait_for_fsm_vldclr,TIMEOUT_READ,PMIC_WRAP_WACS2_RDATA,&reg_rdata);
        if( return_value != 0 ) {
            PWRAPERR("wait_for_fsm_vldclr fail,return_value=%d\n",return_value);
            return_value+=1;
            goto FAIL;
        }

		*rdata = GET_WACS0_RDATA( reg_rdata );
        WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR , 1);
    }
	
FAIL:
    if( return_value != 0 ) {
        PWRAPERR("pwrap_wacs2 fail,return_value=%d\n",return_value);
        PWRAPERR("timeout:BUG_ON here\n");
    }

    return return_value;
}


/*********************internal API for pwrap_init***************************/

/**********************************
 * Function : _pwrap_wacs2_nochk()
 * Description :
 * Parameter :
 * Return :
 ***********************************/
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

    /* Check argument validation */
    if( (write & ~(0x1))    != 0)  return E_PWR_INVALID_RW;
    if( (adr   & ~(0xffff)) != 0)  return E_PWR_INVALID_ADDR;
    if( (wdata & ~(0xffff)) != 0)  return E_PWR_INVALID_WDAT;

    /* Check IDLE */
    return_value=wait_for_state_ready_init(wait_for_fsm_idle,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,0);
    if( return_value != 0) {
        PWRAPERR("_pwrap_wacs2_nochk write command fail,return_value=%x\n", return_value);
        return return_value;
    }

    wacs_write  = write << 31;
    wacs_adr    = (adr >> 1) << 16;
    wacs_cmd= wacs_write | wacs_adr | wdata;
    WRAP_WR32(PMIC_WRAP_WACS2_CMD,wacs_cmd);

    if( write == 0 ) {
        if (NULL == rdata) {
            PWRAPERR("rdata is a NULL pointer\n");
            return_value= E_PWR_INVALID_ARG;
            return return_value;
        }
        return_value=wait_for_state_ready_init(wait_for_fsm_vldclr,TIMEOUT_READ,PMIC_WRAP_WACS2_RDATA,&reg_rdata);
        if( return_value != 0 ) {
            PWRAPERR("wait_for_fsm_vldclr fail,return_value=%d\n",return_value);
            return_value+=1;
            return return_value;
        }
        *rdata = GET_WACS0_RDATA(reg_rdata);
        WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR , 1);
    }

    return 0;
}

/************************************************
 * Function : _pwrap_init_dio()
 * Description :call it in pwrap_init,mustn't check init done
 * Parameter :
 * Return :
 ************************************************/
static S32 _pwrap_init_dio( U32 dio_en )
{
    U32 arb_en_backup=0x0;
    /* U32 rdata=0x0; */
    U32 return_value=0;

    arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);
    WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , WACS2);
#ifdef SLV_6351  
    pwrap_write_nochk(MT6351_DEW_DIO_EN, (dio_en));
#endif

    /* Check IDLE & INIT_DONE in advance */
    return_value=wait_for_state_ready_init(wait_for_idle_and_sync,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,0);
    if( return_value != 0 ) {
         PWRAPERR("_pwrap_init_dio fail,return_value=%x\n", return_value);
         return return_value;
     }
    /* enable AP DIO mode */
    WRAP_WR32(PMIC_WRAP_DIO_EN , dio_en);
    /* Read Test */
/* #ifdef SLV_6351  
    pwrap_read_nochk(MT6351_DEW_READ_TEST, &rdata);
    if( rdata != MT6351_DEFAULT_VALUE_READ_TEST ) {
         PWRAPERR("[Dio_mode][Read Test] fail,dio_en = %x, READ_TEST rdata=%x, exp=%x\n", dio_en, rdata,MT6351_DEFAULT_VALUE_READ_TEST);
         return E_PWR_READ_TEST_FAIL;
    }
#endif*/
    WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , arb_en_backup);

    return 0;
}

/***************************************************
 * Function : _pwrap_init_cipher()
 * Description :
 * Parameter :
 * Return :
 ****************************************************/
static S32 _pwrap_init_cipher( void )
{
    U32 arb_en_backup=0;
    U32 rdata=0;
    U32 return_value=0;
    U32 start_time_ns=0, timeout_ns=0;

    arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);
    WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN ,WACS2);
    WRAP_WR32(PMIC_WRAP_CIPHER_SWRST ,	1);
    WRAP_WR32(PMIC_WRAP_CIPHER_SWRST ,	0);
    WRAP_WR32(PMIC_WRAP_CIPHER_KEY_SEL , 1);
    WRAP_WR32(PMIC_WRAP_CIPHER_IV_SEL  , 2);
    WRAP_WR32(PMIC_WRAP_CIPHER_EN	, 1);
    /* Config CIPHER @ PMIC */
#ifdef SLV_6351 
    pwrap_write_nochk(MT6351_DEW_CIPHER_SWRST, 0x1);
    pwrap_write_nochk(MT6351_DEW_CIPHER_SWRST, 0x0);
    pwrap_write_nochk(MT6351_DEW_CIPHER_KEY_SEL, 0x1);
    pwrap_write_nochk(MT6351_DEW_CIPHER_IV_SEL,  0x2);
    pwrap_write_nochk(MT6351_DEW_CIPHER_EN,  0x1);
#endif

    /*wait for cipher data ready@AP */
    return_value=wait_for_state_ready_init(wait_for_cipher_ready,TIMEOUT_WAIT_IDLE,PMIC_WRAP_CIPHER_RDY,0);
    if( return_value != 0 ) {
        PWRAPERR("wait for cipher data ready@AP fail,return_value=%x\n", return_value);
        return return_value;
    }
    PWRAPLOG("cipher data ready@AP\n"); 

    /* wait for cipher data ready@PMIC */
#ifdef SLV_6351
    start_time_ns = _pwrap_get_current_time();
    timeout_ns = _pwrap_time2ns(0xFFFFFF);
    do {
        if (_pwrap_timeout_ns(start_time_ns, timeout_ns)) {
            PWRAPERR("wait for cipher data ready@PMIC\n");
        }
        pwrap_read_nochk(MT6351_DEW_CIPHER_RDY,&rdata);
     } while( rdata != 0x1 ); /* cipher_ready */ 

    return_value = pwrap_write_nochk(MT6351_DEW_CIPHER_MODE, 0x1);
    if ( return_value != 0 ) {
        PWRAPERR("write MT6351_DEW_CIPHER_MODE fail,return_value=%x\n", return_value);
        return return_value;
    }
#endif
    PWRAPLOG("cipher data ready@PMIC\n");

	/* wait for cipher mode idle */
    return_value=wait_for_state_ready_init(wait_for_idle_and_sync,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,0);
    if( return_value != 0 ) {
        PWRAPERR("wait for cipher mode idle fail,return_value=%x\n", return_value);
        return return_value;
    }
    WRAP_WR32(PMIC_WRAP_CIPHER_MODE , 1);

    /* Read Test */
#ifdef SLV_6351
    pwrap_read_nochk(MT6351_DEW_READ_TEST, &rdata);
    if( rdata != MT6351_DEFAULT_VALUE_READ_TEST ) {
        PWRAPERR("_pwrap_init_cipher,read test error,error code=%x, rdata=%x\n", 1, rdata);
        return E_PWR_READ_TEST_FAIL;
    }
#endif

    WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , arb_en_backup);

    return 0;
}

static void _pwrap_adc_set()
{
    /*pwrap_write_nochk(MT6351_DEW_CRC_EN, 0x1);*/

    WRAP_WR32(PMIC_WRAP_SIG_ADR, MT6351_DEW_CRC_VAL);
    WRAP_WR32(PMIC_WRAP_STAUPD_GRPEN, 0x75);
    /*WRAP_WR32(PMIC_WRAP_CRC_EN, 0x1);*/
    WRAP_WR32(PMIC_WRAP_EINT_STA0_ADR, MT6351_INT_STA);

    WRAP_WR32(PMIC_WRAP_ADC_CMD_ADDR, MT6351_AUXADC_RQST1_SET);
    WRAP_WR32(PMIC_WRAP_PWRAP_ADC_CMD, 0x0100);
    WRAP_WR32(PMIC_WRAP_ADC_RDATA_ADDR, MT6351_AUXADC_ADC16);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR_LATEST, MT6351_AUXADC_ADC32);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR_WP, MT6351_AUXADC_MDBG_1);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR0, MT6351_AUXADC_BUF0);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR1, MT6351_AUXADC_BUF1);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR2, MT6351_AUXADC_BUF2);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR3, MT6351_AUXADC_BUF3);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR4, MT6351_AUXADC_BUF4);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR5, MT6351_AUXADC_BUF5);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR6, MT6351_AUXADC_BUF6);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR7, MT6351_AUXADC_BUF7);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR8, MT6351_AUXADC_BUF8);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR9, MT6351_AUXADC_BUF9);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR10, MT6351_AUXADC_BUF10);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR11, MT6351_AUXADC_BUF11);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR12, MT6351_AUXADC_BUF12);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR13, MT6351_AUXADC_BUF13);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR14, MT6351_AUXADC_BUF14);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR15, MT6351_AUXADC_BUF15);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR16, MT6351_AUXADC_BUF16);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR17, MT6351_AUXADC_BUF17);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR18, MT6351_AUXADC_BUF18);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR19, MT6351_AUXADC_BUF19);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR20, MT6351_AUXADC_BUF20);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR21, MT6351_AUXADC_BUF21);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR22, MT6351_AUXADC_BUF22);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR23, MT6351_AUXADC_BUF23);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR24, MT6351_AUXADC_BUF24);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR25, MT6351_AUXADC_BUF25);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR26, MT6351_AUXADC_BUF26);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR27, MT6351_AUXADC_BUF27);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR28, MT6351_AUXADC_BUF28);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR29, MT6351_AUXADC_BUF29);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR30, MT6351_AUXADC_BUF30);
    WRAP_WR32(PMIC_WRAP_MD_ADC_RDATA_ADDR31, MT6351_AUXADC_BUF31);

    return;
}

static void _pwrap_starve_set()
{
    WRAP_WR32(PMIC_WRAP_HARB_HPRIO, 0x0007);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_0, 0x0402);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_1, 0x0402);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_2, 0x0403);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_3, 0x0414);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_4, 0x0420);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_5, 0x0420);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_6, 0x0420);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_7, 0x0428);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_8, 0x0428);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_9, 0x0417);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_10, 0x0563);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_11, 0x047C);
    WRAP_WR32(PMIC_WRAP_STARV_COUNTER_12, 0x0740);

    return;
}

static void _pwrap_enable()
{
    WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, 0x01fff);
    WRAP_WR32(PMIC_WRAP_WACS0_EN, 0x1);
    WRAP_WR32(PMIC_WRAP_WACS1_EN, 0x1);
    WRAP_WR32(PMIC_WRAP_WACS3_EN, 0x1);
    WRAP_WR32(PMIC_WRAP_STAUPD_PRD, 0x5);
    WRAP_WR32(PMIC_WRAP_WDT_UNIT, 0xf);
    WRAP_WR32(PMIC_WRAP_WDT_SRC_EN, 0xffffffff);
    WRAP_WR32(PMIC_WRAP_TIMER_EN, 0x1);
    WRAP_WR32(PMIC_WRAP_INT0_EN, 0xfffffffd);
    WRAP_WR32(PMIC_WRAP_INT1_EN, 0x0001ffff);

    return;
}

/************************************************
 * Function : _pwrap_init_sistrobe()
 * scription : Initialize SI_CK_CON and SIDLY
 * Parameter :
 * Return :
 ************************************************/
static S32 _pwrap_init_sistrobe( void )
{
    U32 arb_en_backup=0;
    U32 rdata=0;
    S32 ind=0,sidly=0;
    U32 result_faulty=0;
	U64 result = 0, inds = 0, indf = 0, tmp1 = 0, tmp2 = 0;
	S32 leading_one = 0;
	S32 tailing_one = 0;
	S32 i = 0, fail_cnt = 0;
	U32 test_data[30] = {0x6996,0x9669,0x6996,0x9669,0x6996,0x9669,0x6996,0x9669,0x6996,0x9669,
						 0x5AA5,0xA55A,0x5AA5,0xA55A,0x5AA5,0xA55A,0x5AA5,0xA55A,0x5AA5,0xA55A,
                         0x1B27,0x1B27,0x1B27,0x1B27,0x1B27,0x1B27,0x1B27,0x1B27,0x1B27,0x1B27};

    arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);
	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, WACS2);	/* only WACS2 */

	/* --------------------------------------------------------------------- */
	/* Scan all possible input strobe by READ_TEST */
	/* --------------------------------------------------------------------- */
	for (ind = 0; ind < 24; ind++) {	/* 24 sampling clock edge */
        WRAP_WR32(PMIC_WRAP_SI_CK_CON , (ind >> 2) & 0x7);
        WRAP_WR32(PMIC_WRAP_SIDLY ,0x3 - (ind & 0x3));
#ifdef SLV_6351
        /* _pwrap_wacs2_nochk(0, MT6351_DEW_READ_TEST, 0, &rdata);
        if( rdata == MT6351_DEFAULT_VALUE_READ_TEST ) {
			PWRAPERR("_pwrap_init_sistrobe [Read Test of MT6351] pass,index=%d rdata=%x\n",ind, rdata);
			result |= (0x1 << ind);
		} else {
			PWRAPERR("_pwrap_init_sistrobe [Read Test of MT6351] tuning,index=%d rdata=%x\n",ind, rdata);
		} */
		fail_cnt = 0;
		for(i = 0; i < 30; i++)
		{
			_pwrap_wacs2_nochk(1, MT6351_DEW_WRITE_TEST, test_data[i], &rdata);
			_pwrap_wacs2_nochk(0, MT6351_DEW_WRITE_TEST, 0, &rdata);
			
			if(rdata != test_data[i])
			{				
				fail_cnt++;
				PWRAPLOG("_pwrap_init_sistrobe [Read Test of MT6351] tuning,index=%d rdata=%x\n",ind, rdata);
				break;
			}
			else
				PWRAPLOG("_pwrap_init_sistrobe [Read Test of MT6351] pass,index=%d rdata=%x\n",ind, rdata);
		}
		if(fail_cnt == 0)
			result |= (0x1 << ind);
#endif
    }

	/* --------------------------------------------------------------------- */
    /* Locate the leading one and trailing one of PMIC 1/2 */
	/* --------------------------------------------------------------------- */
	for (ind = 23; ind >= 0; ind--) {
		if (result & (0x1 << ind))
		    break;

    }
	leading_one = ind;

	for (ind = 0; ind < 24; ind++) {
		if (result & (0x1 << ind))
        break;
    }
	tailing_one = ind;

	/* --------------------------------------------------------------------- */
    /* Check the continuity of pass range */
	/* --------------------------------------------------------------------- */
	tmp1 = (0x1 << (leading_one + 1)) - 1;
	tmp2 = (0x1 << tailing_one) - 1;
	if ((tmp1 - tmp2) != result) 
    {
		PWRAPERR("_pwrap_init_sistrobe Fail, result = %llx, leading_one:%d, tailing_one:%d\n",result, leading_one, tailing_one);
            result_faulty = 0x1;
        }
	/* --------------------------------------------------------------------- */
    /* Config SICK and SIDLY to the middle point of pass range */
	/* --------------------------------------------------------------------- */
    if( result_faulty == 0 ) {
		ind = (leading_one + tailing_one) / 2;
        WRAP_WR32(PMIC_WRAP_SI_CK_CON , (ind >> 2) & 0x7);
		sidly = 0x3 - (ind & 0x3);
		if(sidly != 0)
			sidly--;
        WRAP_WR32(PMIC_WRAP_SIDLY ,sidly);
		/* --------------------------------------------------------------------- */
        /* Restore */
		/* --------------------------------------------------------------------- */
#ifdef SLV_6351  
		pwrap_read_nochk(MT6351_DEW_READ_TEST, &rdata);
		if( rdata != MT6351_DEFAULT_VALUE_READ_TEST ) {
			PWRAPERR("[init_sistrobe][Read Test] fail,READ_TEST rdata=%x, exp=%x\n",rdata,MT6351_DEFAULT_VALUE_READ_TEST);
			return E_PWR_READ_TEST_FAIL;
		}
#endif
		
		
        WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , arb_en_backup);
        return 0;
    } else {
        PWRAPERR("_pwrap_init_sistrobe Fail,result_faulty=%x\n", result_faulty);
        return result_faulty;
    }
}

/******************************************************
 * Function : _pwrap_reset_spislv()
 * Description :
 * Parameter :
 * Return :
 ******************************************************/
static S32 _pwrap_reset_spislv( void )
{
    U32 ret=0;
	U32 return_value=0;

    WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN , DISABLE_ALL);
    WRAP_WR32(PMIC_WRAP_WRAP_EN , DISABLE);
    WRAP_WR32(PMIC_WRAP_MUX_SEL , MANUAL_MODE);
    WRAP_WR32(PMIC_WRAP_MAN_EN ,ENABLE);
    WRAP_WR32(PMIC_WRAP_DIO_EN ,DISABLE);

    WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_CSL  << 8));
    WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));
    WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_CSH  << 8));
    WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));
    WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));
    WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));
    WRAP_WR32(PMIC_WRAP_MAN_CMD , (OP_WR << 13) | (OP_OUTS << 8));

    return_value=wait_for_state_ready_init(wait_for_sync,TIMEOUT_WAIT_IDLE,PMIC_WRAP_WACS2_RDATA,0);
    if( return_value!=0 ) {
        PWRAPERR("_pwrap_reset_spislv fail,return_value=%x\n", return_value);
        ret = E_PWR_TIMEOUT;
        goto timeout;
    }

    WRAP_WR32(PMIC_WRAP_MAN_EN ,DISABLE);
    WRAP_WR32(PMIC_WRAP_MUX_SEL ,WRAPPER_MODE);

timeout:
    WRAP_WR32(PMIC_WRAP_MAN_EN ,DISABLE);
    WRAP_WR32(PMIC_WRAP_MUX_SEL ,WRAPPER_MODE);
    return ret;
}

static void __pwrap_soft_reset(void)
{
    PWRAPLOG("start reset wrapper\n");
    WRAP_WR32(INFRA_GLOBALCON_RST0,0x1);
    WRAP_WR32(INFRA_GLOBALCON_RST1,0x1);
    return;
}

static void __pwrap_spi_clk_set(void)
{
    WRAP_WR32(CLK_CFG_5_CLR,CLK_SPI_CK_26M);
    /*sys_ck cg enable*/
    WRAP_WR32(MODULE_SW_CG_0_SET,0x0000000F);
    WRAP_WR32(MODULE_SW_CG_0_CLR,0x0000000F);
    return;
}

static S32 _pwrap_init_reg_clock( U32 regck_sel )
{
/* Set Dummy cycle 6351 (assume 18MHz) */
#ifdef SLV_6351
    pwrap_write_nochk(MT6351_DEW_RDDMY_NO, 0x8);
#endif	
 
    WRAP_WR32(PMIC_WRAP_RDDMY ,0x8);

    /* Config SPI Waveform according to reg clk */
    if( regck_sel == 1 ) {	
        WRAP_WR32(PMIC_WRAP_CSHEXT_READ    , 0x0);
        WRAP_WR32(PMIC_WRAP_CSHEXT_WRITE   , 0x33);
        WRAP_WR32(PMIC_WRAP_CSLEXT_START   , 0x0);
        WRAP_WR32(PMIC_WRAP_CSLEXT_END     , 0x0);
	} else { /* Safe mode */
		WRAP_WR32(PMIC_WRAP_CSHEXT_WRITE   , 0xff);
		WRAP_WR32(PMIC_WRAP_CSHEXT_READ    , 0xff);
		WRAP_WR32(PMIC_WRAP_CSLEXT_START   , 0xf);
		WRAP_WR32(PMIC_WRAP_CSLEXT_END     , 0xf);
    }
    return 0;
}

static U32 pwrap_read_test(void)
{
    U32 rdata=0;
    U32 return_value=0;
	/* Read Test */
#ifdef SLV_6351
    return_value=pwrap_read(MT6351_DEW_READ_TEST,&rdata);
    if( rdata != MT6351_DEFAULT_VALUE_READ_TEST ) {
        PWRAPREG("Read Test fail,rdata=0x%x, exp=0x5aa5,return_value=0x%x\n", rdata,return_value);
        return E_PWR_READ_TEST_FAIL;
    } else {
        PWRAPREG("Read MT6351 Test pass,return_value=%d\n",return_value);
    }
#endif

    return 0;
}
static U32 pwrap_write_test(void)
{
    U32 rdata=0;
    U32 sub_return=0;
    U32 sub_return1=0;

    /* Write test using WACS2 */
#ifdef SLV_6351
    sub_return = pwrap_write(MT6351_DEW_WRITE_TEST, MT6351_WRITE_TEST_VALUE);
    PWRAPREG("after MT6351 pwrap_write\n");
    sub_return1 = pwrap_read(MT6351_DEW_WRITE_TEST,&rdata);
    if(( rdata != MT6351_WRITE_TEST_VALUE )||( sub_return != 0 )||( sub_return1 != 0 )) {
        PWRAPREG("write test error,rdata=0x%x,exp=0xa55a,sub_return=0x%x,sub_return1=0x%x\n", rdata,sub_return,sub_return1);
        return E_PWR_INIT_WRITE_TEST;
    } else {
        PWRAPREG("write MT6351 Test pass\n");
    }
#endif

    return 0;
}
static void pwrap_ut(U32 ut_test)
{
    switch(ut_test)
    {
        case 1:
            pwrap_write_test();
            break;
        case 2:
            pwrap_read_test();
            break;
       default:
           PWRAPREG ( "default test.\n" );
           break;
    }
    return;
}
S32 pwrap_init ( void )
{
    S32 sub_return=0;
    S32 sub_return1=0;
    U32 rdata=0x0;
    u32 cg_mask = 0;
    u32 backup = 0;

    PWRAPLOG("Preloader pwrap_init start!!!!!!!!!!!!! \n");

    /* toggle PMIC_WRAP and pwrap_spictl reset */
    __pwrap_soft_reset();
    PWRAPLOG("soft reset ok \n");

    /* Set SPI_CK_freq = 26MHz. It can marked when use fpga verify, because it has no address on Everest fpga */
#if CFG_FPGA_PLATFORM
#else
    __pwrap_spi_clk_set();
#endif
    PWRAPLOG("spi clk set ok \n");

    /* Enable DCM */
    WRAP_WR32(PMIC_WRAP_DCM_EN , 3);
    /* no debounce */
    WRAP_WR32(PMIC_WRAP_DCM_DBC_PRD ,DISABLE);
#if 1
	/* 0x10001074 bit[22] = 1¡¦b0 to disable pmic clock DCM */
    sub_return = WRAP_RD32(PMIC_CLOCK_DCM);
	print("PMIC_CLOCK_DCM=0x%x\n",sub_return);
    WRAP_WR32(PMIC_CLOCK_DCM, (sub_return & 0xFFBFFFFF));

    /* 0x10001F0C bit[25] = 1¡¦b1 to disable apb clock gating mechanism. */
    sub_return = WRAP_RD32(APB_CLOCK_GATING);
	print("APB_CLOCK_GATING=0x%x \n",sub_return);
    WRAP_WR32(APB_CLOCK_GATING, ((sub_return & 0xFDFFFFFF) | 0x02000000));
    print("PMIC_CLOCK_DCM=0x%x APB_CLOCK_GATING=0x%x ,enable DCM ok \n",WRAP_RD32(PMIC_CLOCK_DCM),WRAP_RD32(APB_CLOCK_GATING));
#endif
  
  
    /* Reset SPISLV */
    sub_return=_pwrap_reset_spislv();
    if( sub_return != 0 ) {
        PWRAPERR("error,_pwrap_reset_spislv fail,sub_return=%x\n",sub_return);
        return E_PWR_INIT_RESET_SPI;
    }
    PWRAPLOG("reset spislv ok \n");

    /* Enable WACS2 */
    WRAP_WR32(PMIC_WRAP_WRAP_EN,ENABLE);
    WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN,WACS2);
    WRAP_WR32(PMIC_WRAP_WACS2_EN,ENABLE);
    PWRAPLOG("enable WACS2 ok \n");

    /* SPI Waveform Configuration. 0:safe mode, 1:18MHz */
    sub_return = _pwrap_init_reg_clock(1);
    if( sub_return != 0) {
        PWRAPERR("error,_pwrap_init_reg_clock fail,sub_return=%x\n",sub_return);
        return E_PWR_INIT_REG_CLOCK;
    }
    PWRAPLOG("set reg clock ok \n");

    /* Enable DIO mode */
    sub_return = _pwrap_init_dio(1);
    if( sub_return != 0 ) {
        PWRAPERR("_pwrap_init_dio test error,error code=%x, sub_return=%x\n", 0x11, sub_return);
        return E_PWR_INIT_DIO;
    }
    PWRAPLOG("enable dio ok \n");
	
	/* Input data calibration flow; */
    sub_return = _pwrap_init_sistrobe();
    if( sub_return != 0 ) {
        PWRAPERR("error,DrvPWRAP_InitSiStrobe fail,sub_return=%x\n",sub_return);
        return E_PWR_INIT_SIDLY;
    }
    PWRAPLOG("sistrobe ok \n");

    /* Enable Encryption */
    sub_return = _pwrap_init_cipher();
    if( sub_return != 0 ) {
        PWRAPERR("Enable Encryption fail, return=%x\n", sub_return);
        return E_PWR_INIT_CIPHER;
    }
    PWRAPLOG("enable cipher ok \n");

    /*  Write test using WACS2.  check Wtiet test default value */
#ifdef SLV_6351
    sub_return = pwrap_write_nochk(MT6351_DEW_WRITE_TEST, MT6351_WRITE_TEST_VALUE);
    sub_return1 = pwrap_read_nochk(MT6351_DEW_WRITE_TEST, &rdata);
    if( rdata != MT6351_WRITE_TEST_VALUE ) {
        PWRAPERR("write test error,rdata=0x%x,exp=0xa55a,sub_return=0x%x,sub_return1=0x%x\n", rdata,sub_return,sub_return1);
        return E_PWR_INIT_WRITE_TEST;
    }
#endif	
    PWRAPLOG("write test ok \n");

    /* Status update function initialization
        * 1. Signature Checking using CRC (CRC 0 only) 
        * 2. EINT update
        * 3. Read back Auxadc thermal data for GPD
        * 4. Read back Auxadc thermal data for LTE
        */
    _pwrap_adc_set();
    PWRAPLOG("adc set ok \n");

    /* PMIC WRAP priority adjust */
    WRAP_WR32(PMIC_WRAP_PRIORITY_USER_SEL_0, 0x6543C210);
    WRAP_WR32(PMIC_WRAP_PRIORITY_USER_SEL_1, 0xFEDBA987);
    WRAP_WR32(PMIC_WRAP_ARBITER_OUT_SEL_0, 0x87654210);
    WRAP_WR32(PMIC_WRAP_ARBITER_OUT_SEL_1, 0xFED3CBA9);
    /* PMIC_WRAP starvation setting */
    _pwrap_starve_set();
    PWRAPLOG("starve set ok \n");

    /* PMIC_WRAP enables */
    _pwrap_enable();
    PWRAPLOG("enable ok \n");

    /* Initialization Done */
    WRAP_WR32(PMIC_WRAP_INIT_DONE0, 0x1);
    WRAP_WR32(PMIC_WRAP_INIT_DONE1, 0x1);
    WRAP_WR32(PMIC_WRAP_INIT_DONE2, 0x1);
    WRAP_WR32(PMIC_WRAP_INIT_DONE3, 0x1);

    PWRAPLOG("Preloader pwrap_init Done!!!!!!!!! \n");

   return 0;
}

/*-------------------pwrap debug---------------------*/
void pwrap_dump_ap_register(void)
{
    U32 i=0;
    U32 reg_addr=0;
    U32 reg_value=0;

    PWRAPREG("dump pwrap register\n");
    for(i=0;i<=PMIC_WRAP_REG_RANGE;i++)
    {
        reg_addr=(PMIC_WRAP_BASE+i*4);
        reg_value=WRAP_RD32(reg_addr);
        PWRAPREG("0x%x=0x%x\n",reg_addr,reg_value);
    }
}

S32 pwrap_init_preloader ( void )
{
    u32 pwrap_ret=0,i=0;

    PWRAPFUC();
    for(i=0;i<3;i++)
    {
        pwrap_ret = pwrap_init();
        if( pwrap_ret != 0 ) {
            print("[PMIC_WRAP]wrap_init fail,return=%x.\n",pwrap_ret);
            if (i >= 2)
               ASSERT(0);
        }
        else {
            print("[PMIC_WRAP]wrap_init pass,return=%x.\n",pwrap_ret);
            break;
        }
    }

    return 0;
}

#endif/*PMIC_WRAP_NO_PMIC*/

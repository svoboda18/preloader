/******************************************************************************
 * pmic_wrap_init.c - Linux pmic_wrapper Driver,hardware_dependent driver
 *
 *
 * DESCRIPTION:
 *     This file provid the other drivers PMIC wrapper relative functions
 *
 ******************************************************************************/

/*-----start-- Driver Config-------------------------------------------------*/
#define PMIC_WRAP_PRELOADER_DRIVER
/* #define PMIC_WRAP_LK_DRIVER */
/* #define PMIC_WRAP_KERNEL_DRIVER */
/*-----end -- Driver Config-------------------------------------------------*/

#ifdef PMIC_WRAP_PRELOADER_DRIVER
#include <timer.h>
#include <typedefs.h>
#include <gpio.h>
#include "pmic_wrap_init.h"
#endif

#ifdef PMIC_WRAP_LK_DRIVER
#include <platform/pmic_wrap_init.h>
#endif

#ifdef PMIC_WRAP_KERNEL_DRIVER
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/io.h>
#ifdef CONFIG_OF
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#endif
#include <mach/mt_pmic_wrap.h>
#include <mach/mt_typedefs.h>
#include "pwrap_hal.h"

#define PMIC_WRAP_DEVICE "pmic_wrap"

/*-----start-- global variable-----------------------------------------------*/
#ifdef CONFIG_OF
void __iomem *pwrap_base;
static void __iomem *topckgen_reg_base;
static void __iomem *infracfg_ao_reg_base;
static void __iomem *gpio_reg_base;
static void __iomem *sleep_reg_base;
#endif

static struct mt_pmic_wrap_driver *mt_wrp;

static DEFINE_SPINLOCK(wrp_lock);
static DEFINE_SPINLOCK(wrp_lock_isr);
#endif				/* End of #ifdef PMIC_WRAP_KERNEL_DRIVER */

/*-----start--Internal API---------------------------------------------------*/
static S32 _pwrap_init_dio(U32 dio_en);
static S32 _pwrap_init_cipher(void);
static S32 _pwrap_init_reg_clock(U32 regck_sel);
static S32 _pwrap_wacs2_nochk(U32 write, U32 adr, U32 wdata, U32 *rdata);
static S32 _pwrap_reset_spislv(void);

/*-----end --Internal API---------------------------------------------------*/

#ifdef PMIC_WRAP_KERNEL_DRIVER
#ifdef CONFIG_OF
static int pwrap_of_iomap(void);
static void pwrap_of_iounmap(void);
#endif
#endif				/* End of #ifdef PMIC_WRAP_KERNEL_DRIVER */

#ifdef PMIC_WRAP_NO_PMIC
/*-pwrap debug--------------------------------------------------------------------------*/
static inline void pwrap_dump_all_register(void)
{
	return;
}

#ifndef PMIC_WRAP_KERNEL_DRIVER
S32 pwrap_wacs2(U32 write, U32 adr, U32 wdata, U32 *rdata)
{
	PWRAPERR("there is no PMIC real chip,PMIC_WRAP do Nothing\n");
	return 0;
}
#endif

/*********************************************************************************/
/* extern API for PMIC driver, INT related control, this INT is for PMIC chip to AP */
/*********************************************************************************/
#ifndef PMIC_WRAP_KERNEL_DRIVER
S32 pwrap_read(U32 adr, U32 *rdata)
{
	return pwrap_wacs2(0, adr, 0, rdata);
}

S32 pwrap_write(U32 adr, U32 wdata)
{
	return pwrap_wacs2(1, adr, wdata, 0);
}
#else
U32 mt_pmic_wrap_eint_status(void)
{
	PWRAPLOG("[PMIC2]first of all PMIC_WRAP_STAUPD_GRPEN=0x%x\n",
		 WRAP_RD32(PMIC_WRAP_STAUPD_GRPEN));
	PWRAPLOG("[PMIC2]first of all PMIC_WRAP_EINT_STA=0x%x\n", WRAP_RD32(PMIC_WRAP_EINT_STA));
	return WRAP_RD32(PMIC_WRAP_EINT_STA);
}

void mt_pmic_wrap_eint_clr(int offset)
{
	if ((offset < 0) || (offset > 3))
		PWRAPERR("clear EINT flag error, only 0-3 bit\n");
	else
		WRAP_WR32(PMIC_WRAP_EINT_CLR, (1 << offset));

}

/* -------------------------------------------------------- */
/* Function : pwrap_wacs2_hal() */
/* Description : */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
static S32 pwrap_wacs2_hal(U32 write, U32 adr, U32 wdata, U32 *rdata)
{
	PWRAPERR("there is no PMIC real chip,PMIC_WRAP do Nothing\n");
	return 0;
}
#endif				/* End of #ifndef PMIC_WRAP_KERNEL_DRIVER */

/*
 *pmic_wrap init,init wrap interface
 *
 */
S32 pwrap_init(void)
{
	PWRAPERR("There is no PMIC real chip, PMIC_WRAP do Nothing.\n");
	return 0;
}

S32 pwrap_read_nochk(U32 adr, U32 *rdata)
{
	return pwrap_wacs2_hal(0, adr, 0, rdata);
}

S32 pwrap_write_nochk(U32 adr, U32 wdata)
{
	return pwrap_wacs2_hal(1, adr, wdata, 0);
}

#ifdef PMIC_WRAP_PRELOADER_DRIVER
S32 pwrap_init_preloader(void)
{
	u32 pwrap_ret = 0, i = 0;

	PWRAPFUC();
	for (i = 0; i < 3; i++) {
		pwrap_ret = pwrap_init();
		if (pwrap_ret != 0) {
			printf("[PMIC_WRAP]wrap_init fail,the return value=%x.\n", pwrap_ret);
		} else {
			printf("[PMIC_WRAP]wrap_init pass,the return value=%x.\n", pwrap_ret);
			break;	/* init pass */
		}
	}
#ifdef PWRAP_PRELOADER_PORTING
	/* pwrap_init_for_early_porting(); */
#endif
	return 0;
}

#ifdef PWRAP_PRELOADER_PORTING
/* -------------------------------------------------------- */
/* Function : _pwrap_status_update_test() */
/* Description :only for early porting */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
static S32 _pwrap_status_update_test_porting(void)
{
	return 0;

}

void pwrap_init_for_early_porting(void)
{

}
#endif				/* PWRAP_PRELOADER_PORTING */
#endif				/* End of #ifdef PMIC_WRAP_PRELOADER_DRIVER */

#ifdef PMIC_WRAP_KERNEL_DRIVER
/*Interrupt handler function*/
static irqreturn_t mt_pmic_wrap_irq(int irqno, void *dev_id)
{
	unsigned long flags = 0;

	PWRAPFUC();
	PWRAPREG("dump pwrap register\n");
	spin_lock_irqsave(&wrp_lock, flags);
	/* *----------------------------------------------------------------------- */
	pwrap_dump_all_register();
	/* raise the priority of WACS2 for AP */
	WRAP_WR32(PMIC_WRAP_HARB_HPRIO, WACS2);

	/* *----------------------------------------------------------------------- */
	/* clear interrupt flag */
	WRAP_WR32(PMIC_WRAP_INT0_CLR, 0xffffffff);
	WRAP_WR32(PMIC_WRAP_INT1_CLR, 0xffffffff);
	PWRAPREG("INT flag 0x%x\n", WRAP_RD32(PMIC_WRAP_INT0_FLG_RAW));
	PWRAPREG("INT flag 0x%x\n", WRAP_RD32(PMIC_WRAP_INT1_FLG_RAW));
	/* BUG_ON(1); */
	spin_unlock_irqrestore(&wrp_lock, flags);
	return IRQ_HANDLED;
}

static U32 pwrap_read_test(void)
{
	return 0;
}

static U32 pwrap_write_test(void)
{
	return 0;
}

static void pwrap_ut(U32 ut_test)
{
	switch (ut_test) {
	case 1:
		/* pwrap_wacs2_para_test(); */
		pwrap_write_test();
		break;
	case 2:
		/* pwrap_wacs2_para_test(); */
		pwrap_read_test();
		break;

	default:
		PWRAPREG("default test.\n");
		break;
	}
	return;
}

/*---------------------------------------------------------------------------*/
static S32 mt_pwrap_show_hal(char *buf)
{
	PWRAPFUC();
	return snprintf(buf, PAGE_SIZE, "%s\n", "no implement");
}

/*---------------------------------------------------------------------------*/
static S32 mt_pwrap_store_hal(const char *buf, size_t count)
{
	U32 reg_value = 0;
	U32 reg_addr = 0;
	U32 return_value = 0;
	U32 ut_test = 0;

	if (!strncmp(buf, "-h", 2)) {
		PWRAPREG
		    ("PWRAP debug: [-dump_reg][-trace_wacs2][-init][-rdpmic][-wrpmic][-readtest][-writetest]\n");
		PWRAPREG("PWRAP UT: [1][2]\n");
	}
	/* ----------pwrap debug---------- */
	else if (!strncmp(buf, "-dump_reg", 9)) {
		pwrap_dump_all_register();
	} else if (!strncmp(buf, "-trace_wacs2", 12)) {
		/* pwrap_trace_wacs2(); */
	} else if (!strncmp(buf, "-init", 5)) {
		return_value = pwrap_init();
		if (return_value == 0)
			PWRAPREG("pwrap_init pass,return_value=%d\n", return_value);
		else
			PWRAPREG("pwrap_init fail,return_value=%d\n", return_value);
	} else if (!strncmp(buf, "-rdpmic", 7) && (1 == sscanf(buf + 7, "%x", &reg_addr))) {
		/* pwrap_read_reg_on_pmic(reg_addr); */
	} else if (!strncmp(buf, "-wrpmic", 7)
		   && (2 == sscanf(buf + 7, "%x %x", &reg_addr, &reg_value))) {
		/* pwrap_write_reg_on_pmic(reg_addr,reg_value); */
	} else if (!strncmp(buf, "-readtest", 9)) {
		pwrap_read_test();
	} else if (!strncmp(buf, "-writetest", 10)) {
		pwrap_write_test();
	}
	/* ----------pwrap UT---------- */
	else if (!strncmp(buf, "-ut", 3) && (1 == sscanf(buf + 3, "%d", &ut_test))) {
		pwrap_ut(ut_test);
	} else {
		PWRAPREG("wrong parameter\n");
	}
	return count;
}
#endif				/* End of #ifdef PMIC_WRAP_KERNEL_DRIVER */
/*---------------------------------------------------------------------------*/
#else				/* Else of #ifdef PMIC_WRAP_NO_PMIC */
/*-pwrap debug--------------------------------------------------------------------------*/
static inline void pwrap_dump_ap_register(void)
{
	U32 i = 0;

	PWRAPREG("dump pwrap register, base=0x%p\n", PMIC_WRAP_BASE);
	PWRAPREG("address     :   3 2 1 0    7 6 5 4    B A 9 8    F E D C\n");
	for (i = 0; i <= PMIC_WRAP_REG_RANGE; i += 4) {
		PWRAPREG("offset 0x%.3x:0x%.8x 0x%.8x 0x%.8x 0x%.8x\n", i * 4,
			 WRAP_RD32(PMIC_WRAP_BASE + (i * 4) + 0),
			 WRAP_RD32(PMIC_WRAP_BASE + (i * 4) + 0x4),
			 WRAP_RD32(PMIC_WRAP_BASE + (i * 4) + 0x8),
			 WRAP_RD32(PMIC_WRAP_BASE + (i * 4) + 0xC));
	}
}

static inline void pwrap_dump_pmic_register(void)
{
#if 0
	U32 i = 0;
	U32 reg_addr = 0;
	U32 reg_value = 0;

	PWRAPREG("dump dewrap register\n");
	for (i = 0; i <= 14; i++) {
		reg_addr = (DEW_BASE + i * 4);
		reg_value = pwrap_read_nochk(reg_addr, &reg_value);
		PWRAPREG("0x%x=0x%x\n", reg_addr, reg_value);
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

/******************************************************************************
pmic wrapper timeout
******************************************************************************/
#define PWRAP_TIMEOUT
/* use the same API name with kernel driver */
/* however,the timeout API in preloader/lk use tick instead of ns */
#ifdef PWRAP_TIMEOUT
static U64 _pwrap_get_current_time(void)
{
#ifdef PMIC_WRAP_KERNEL_DRIVER
	return sched_clock();	/* /TODO: fix me */
#else
	return gpt4_get_current_tick();
#endif
}

static BOOL _pwrap_timeout_ns(U64 start_time_ns, U64 timeout_time_ns)
{
#ifdef PMIC_WRAP_KERNEL_DRIVER
	U64 cur_time = 0;
	U64 elapse_time = 0;

	/* get current tick */
	cur_time = _pwrap_get_current_time();	/* ns */

	/* avoid timer over flow exiting in FPGA env */
	if (cur_time < start_time_ns) {
		PWRAPERR("@@@@Timer overflow! start%lld cur timer%lld\n", start_time_ns, cur_time);
		start_time_ns = cur_time;
		timeout_time_ns = 10000 * 1000;	/* 10000us */
		PWRAPERR("@@@@reset timer! start%lld setting%lld\n", start_time_ns,
			 timeout_time_ns);
	}

	elapse_time = cur_time - start_time_ns;

	/* check if timeout */
	if (timeout_time_ns <= elapse_time) {
		/* timeout */
		PWRAPERR
		    ("@@@@Timeout: elapse time%lld, start%lld , current%lld, setting timer%lld\n",
		     elapse_time, start_time_ns, cur_time, timeout_time_ns);
		return TRUE;
	}
	return FALSE;
#else
	return gpt4_timeout_tick(start_time_ns, timeout_time_ns);
#endif
}

static U64 _pwrap_time2ns(U64 time_us)
{
#ifdef PMIC_WRAP_KERNEL_DRIVER
	return time_us * 1000;
#else
	return gpt4_time2tick_us(time_us);
#endif
}

#else
static U64 _pwrap_get_current_time(void)
{
	return 0;
}

static BOOL _pwrap_timeout_ns(U64 start_time_ns, U64 elapse_time)
{
	return FALSE;
}

static U64 _pwrap_time2ns(U64 time_us)
{
	return 0;
}

#endif				/* End of #ifdef PWRAP_TIMEOUT */

/* ##################################################################### */
/* define macro and inline function (for do while loop) */
/* ##################################################################### */
typedef U32(*loop_condition_fp) (U32);	/* define a function pointer */

static inline U32 wait_for_fsm_idle(U32 x)
{
	return GET_WACS0_FSM(x) != WACS_FSM_IDLE;
}

static inline U32 wait_for_fsm_vldclr(U32 x)
{
	return GET_WACS0_FSM(x) != WACS_FSM_WFVLDCLR;
}

static inline U32 wait_for_sync(U32 x)
{
	return GET_SYNC_IDLE0(x) != WACS_SYNC_IDLE;
}

static inline U32 wait_for_idle_and_sync(U32 x)
{
	return (GET_WACS2_FSM(x) != WACS_FSM_IDLE) || (GET_SYNC_IDLE2(x) != WACS_SYNC_IDLE);
}

static inline U32 wait_for_wrap_idle(U32 x)
{
	return (GET_WRAP_FSM(x) != 0x0) || (GET_WRAP_CH_DLE_RESTCNT(x) != 0x0);
}

static inline U32 wait_for_wrap_state_idle(U32 x)
{
	return GET_WRAP_AG_DLE_RESTCNT(x) != 0;
}

static inline U32 wait_for_man_idle_and_noreq(U32 x)
{
	return (GET_MAN_REQ(x) != MAN_FSM_NO_REQ) || (GET_MAN_FSM(x) != MAN_FSM_IDLE);
}

static inline U32 wait_for_man_vldclr(U32 x)
{
	return GET_MAN_FSM(x) != MAN_FSM_WFVLDCLR;
}

static inline U32 wait_for_cipher_ready(U32 x)
{
	return x != 3;
}

static inline U32 wait_for_stdupd_idle(U32 x)
{
	return GET_STAUPD_FSM(x) != 0x0;
}

#ifdef PMIC_WRAP_KERNEL_DRIVER
static inline U32 wait_for_state_ready_init(loop_condition_fp fp, U32 timeout_us,
					    void *wacs_register, U32 *read_reg)
#else
static inline U32 wait_for_state_ready_init(loop_condition_fp fp, U32 timeout_us, U32 wacs_register,
					    U32 *read_reg)
#endif
{
	U64 start_time_ns = 0, timeout_ns = 0;
	U32 reg_rdata = 0x0;

	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(timeout_us);
	do {
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns)) {
			PWRAPLOG("wait_for_state_ready_init timeout when waiting for idle\n");
			return E_PWR_WAIT_IDLE_TIMEOUT;
		}
		reg_rdata = WRAP_RD32(wacs_register);
	} while (fp(reg_rdata));	/* IDLE State */
	if (read_reg)
		*read_reg = reg_rdata;
	return 0;
}

#ifdef PMIC_WRAP_KERNEL_DRIVER
static inline U32 wait_for_state_idle_init(loop_condition_fp fp, U32 timeout_us,
					   void *wacs_register, void *wacs_vldclr_register,
					   U32 *read_reg)
#else
static inline U32 wait_for_state_idle_init(loop_condition_fp fp, U32 timeout_us, U32 wacs_register,
					   U32 wacs_vldclr_register, U32 *read_reg)
#endif
{
	U64 start_time_ns = 0, timeout_ns = 0;
	U32 reg_rdata;

	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(timeout_us);
	do {
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns)) {
			PWRAPLOG("wait_for_state_idle_init timeout when waiting for idle\n");
			pwrap_dump_ap_register();
			/* pwrap_trace_wacs2(); */
			/* BUG_ON(1); */
			return E_PWR_WAIT_IDLE_TIMEOUT;
		}
		reg_rdata = WRAP_RD32(wacs_register);
		/* if last read command timeout,clear vldclr bit */
		/* read command state machine:FSM_REQ-->wfdle-->WFVLDCLR;write:FSM_REQ-->idle */
		switch (GET_WACS0_FSM(reg_rdata)) {
		case WACS_FSM_WFVLDCLR:
			WRAP_WR32(wacs_vldclr_register, 1);
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
	} while (fp(reg_rdata));	/* IDLE State */
	if (read_reg)
		*read_reg = reg_rdata;
	return 0;
}

#ifdef PMIC_WRAP_KERNEL_DRIVER
static inline U32 wait_for_state_idle(loop_condition_fp fp, U32 timeout_us, void *wacs_register,
				      void *wacs_vldclr_register, U32 *read_reg)
#else
static inline U32 wait_for_state_idle(loop_condition_fp fp, U32 timeout_us, U32 wacs_register,
				      U32 wacs_vldclr_register, U32 *read_reg)
#endif
{
	U64 start_time_ns = 0, timeout_ns = 0;
	U64 loop_counter = 0;
	U32 reg_rdata;

	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(timeout_us);
	do {
		loop_counter++;
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns)) {
			PWRAPLOG
			    ("wait_for_state_idle timeout when waiting for idle, loop_counter = %lld\n",
			     loop_counter);
			pwrap_dump_ap_register();
			/* pwrap_trace_wacs2(); */
			/* BUG_ON(1); */
			return E_PWR_WAIT_IDLE_TIMEOUT;
		}
		reg_rdata = WRAP_RD32(wacs_register);
		if (GET_INIT_DONE0(reg_rdata) != WACS_INIT_DONE) {
			PWRAPLOG("initialization isn't finished\n");
			return E_PWR_NOT_INIT_DONE;
		}
		/* if last read command timeout,clear vldclr bit */
		/* read command state machine:FSM_REQ-->wfdle-->WFVLDCLR;write:FSM_REQ-->idle */
		switch (GET_WACS0_FSM(reg_rdata)) {
		case WACS_FSM_WFVLDCLR:
			WRAP_WR32(wacs_vldclr_register, 1);
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
	} while (fp(reg_rdata));	/* IDLE State */
	if (read_reg)
		*read_reg = reg_rdata;
	return 0;
}

#ifdef PMIC_WRAP_KERNEL_DRIVER
static inline U32 wait_for_state_ready(loop_condition_fp fp, U32 timeout_us, void *wacs_register,
				       U32 *read_reg)
#else
static inline U32 wait_for_state_ready(loop_condition_fp fp, U32 timeout_us, U32 wacs_register,
				       U32 *read_reg)
#endif
{
	U64 start_time_ns = 0, timeout_ns = 0;
	U64 loop_count = 0;
	U32 reg_rdata;

	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(timeout_us);
	do {
		loop_count++;
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns)) {
			PWRAPLOG("timeout when waiting for idle, loop_count = %lld\n", loop_count);
			pwrap_dump_ap_register();
			return E_PWR_WAIT_IDLE_TIMEOUT;
		}
		reg_rdata = WRAP_RD32(wacs_register);

		if (GET_INIT_DONE0(reg_rdata) != WACS_INIT_DONE) {
			PWRAPLOG("initialization isn't finished\n");
			return E_PWR_NOT_INIT_DONE;
		}
	} while (fp(reg_rdata));	/* IDLE State */
	if (read_reg)
		*read_reg = reg_rdata;
	return 0;
}

#ifndef PMIC_WRAP_KERNEL_DRIVER
/* ****************************************************************************** */
/* --external API for pmic_wrap user------------------------------------------------- */
/* ****************************************************************************** */
S32 pwrap_read(U32 adr, U32 *rdata)
{
	return pwrap_wacs2(0, adr, 0, rdata);
}

S32 pwrap_write(U32 adr, U32 wdata)
{
	return pwrap_wacs2(1, adr, wdata, 0);
}
#endif

#ifdef PMIC_WRAP_KERNEL_DRIVER
/********************************************************************************************/
/* extern API for PMIC driver, INT related control, this INT is for PMIC chip to AP */
/********************************************************************************************/
U32 mt_pmic_wrap_eint_status(void)
{
	return WRAP_RD32(PMIC_WRAP_EINT_STA);
}

void mt_pmic_wrap_eint_clr(int offset)
{
	if ((offset < 0) || (offset > 3))
		PWRAPERR("clear EINT flag error, only 0-3 bit\n");
	else
		WRAP_WR32(PMIC_WRAP_EINT_CLR, (1 << offset));

	PWRAPREG("clear EINT flag mt_pmic_wrap_eint_status=0x%x\n", WRAP_RD32(PMIC_WRAP_EINT_STA));
}
#endif
/* -------------------------------------------------------- */
/* Function : pwrap_wacs2() */
/* Description : */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
#ifdef PMIC_WRAP_KERNEL_DRIVER
static S32 pwrap_wacs2_hal(U32 write, U32 adr, U32 wdata, U32 *rdata)
#else
S32 pwrap_wacs2(U32 write, U32 adr, U32 wdata, U32 *rdata)
#endif
{
	/* U64 wrap_access_time=0x0; */
	U32 reg_rdata = 0;
	U32 wacs_write = 0;
	U32 wacs_adr = 0;
	U32 wacs_cmd = 0;
	U32 return_value = 0;
#ifdef PMIC_WRAP_KERNEL_DRIVER
	unsigned long flags = 0;
#endif

#ifdef PMIC_WRAP_KERNEL_DRIVER
	spin_lock_irqsave(&wrp_lock, flags);
#endif

	/* Check argument validation */
	if ((write & ~(0x1)) != 0)
		return E_PWR_INVALID_RW;
	if ((adr & ~(0xffff)) != 0)
		return E_PWR_INVALID_ADDR;
	if ((wdata & ~(0xffff)) != 0)
		return E_PWR_INVALID_WDAT;

	/* Check IDLE & INIT_DONE in advance */
	return_value =
	    wait_for_state_idle(wait_for_fsm_idle, TIMEOUT_WAIT_IDLE, PMIC_WRAP_WACS2_RDATA,
				PMIC_WRAP_WACS2_VLDCLR, 0);
	if (return_value != 0) {
		PWRAPLOG("wait_for_fsm_idle fail,return_value=%d\n", return_value);
		goto FAIL;
	}
	wacs_write = write << 31;
	wacs_adr = (adr >> 1) << 16;
	wacs_cmd = wacs_write | wacs_adr | wdata;

	WRAP_WR32(PMIC_WRAP_WACS2_CMD, wacs_cmd);
	if (write == 0) {
		if (NULL == rdata) {
			PWRAPLOG("rdata is a NULL pointer\n");
			return_value = E_PWR_INVALID_ARG;
			WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR, 1);
			goto FAIL;
		}
		return_value =
		    wait_for_state_ready(wait_for_fsm_vldclr, TIMEOUT_READ, PMIC_WRAP_WACS2_RDATA,
					 &reg_rdata);
		if (return_value != 0) {
			PWRAPLOG("wait_for_fsm_vldclr fail,return_value=%d\n", return_value);
			return_value += 1;	/* E_PWR_NOT_INIT_DONE_READ or E_PWR_WAIT_IDLE_TIMEOUT_READ */
			goto FAIL;
		}

		*rdata = GET_WACS0_RDATA(reg_rdata);
		WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR, 1);
	}

FAIL:
#ifdef PMIC_WRAP_KERNEL_DRIVER
	spin_unlock_irqrestore(&wrp_lock, flags);
#endif
	if (return_value != 0) {
		PWRAPLOG("pwrap_wacs2 fail,return_value=%d\n", return_value);
		PWRAPLOG("timeout:BUG_ON here\n");
	}

	return return_value;
}

/* ****************************************************************************** */
/* --internal API for pwrap_init------------------------------------------------- */
/* ****************************************************************************** */

/* -------------------------------------------------------- */
/* Function : _pwrap_wacs2_nochk() */
/* Description : */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
S32 pwrap_read_nochk(U32 adr, U32 *rdata)
{
	return _pwrap_wacs2_nochk(0, adr, 0, rdata);
}

S32 pwrap_write_nochk(U32 adr, U32 wdata)
{
	return _pwrap_wacs2_nochk(1, adr, wdata, 0);
}

static S32 _pwrap_wacs2_nochk(U32 write, U32 adr, U32 wdata, U32 *rdata)
{
	U32 reg_rdata = 0x0;
	U32 wacs_write = 0x0;
	U32 wacs_adr = 0x0;
	U32 wacs_cmd = 0x0;
	U32 return_value = 0x0;
	/* PWRAPFUC(); */
	/* Check argument validation */
	if ((write & ~(0x1)) != 0)
		return E_PWR_INVALID_RW;
	if ((adr & ~(0xffff)) != 0)
		return E_PWR_INVALID_ADDR;
	if ((wdata & ~(0xffff)) != 0)
		return E_PWR_INVALID_WDAT;

	/* Check IDLE */
	return_value =
	    wait_for_state_ready_init(wait_for_fsm_idle, TIMEOUT_WAIT_IDLE, PMIC_WRAP_WACS2_RDATA,
				      0);
	if (return_value != 0) {
		PWRAPLOG("_pwrap_wacs2_nochk write command fail,return_value=%x\n", return_value);
		return return_value;
	}

	wacs_write = write << 31;
	wacs_adr = (adr >> 1) << 16;
	wacs_cmd = wacs_write | wacs_adr | wdata;
	WRAP_WR32(PMIC_WRAP_WACS2_CMD, wacs_cmd);

	if (write == 0) {
		if (NULL == rdata) {
			PWRAPLOG("rdata is a NULL pointer\n");
			WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR, 1);
			return E_PWR_INVALID_ARG;
		}
		/* wait for read data ready */
		return_value =
		    wait_for_state_ready_init(wait_for_fsm_vldclr, TIMEOUT_READ,
					      PMIC_WRAP_WACS2_RDATA, &reg_rdata);
		if (return_value != 0) {
			PWRAPLOG("wait_for_fsm_vldclr fail, return_value=%d\n", return_value);
			return_value += 1;	/* E_PWR_NOT_INIT_DONE_READ or E_PWR_WAIT_IDLE_TIMEOUT_READ */
			return return_value;
		}
		*rdata = GET_WACS0_RDATA(reg_rdata);
		WRAP_WR32(PMIC_WRAP_WACS2_VLDCLR, 1);
	}
	return 0;
}

/* -------------------------------------------------------- */
/* Function : _pwrap_init_dio() */
/* Description :call it in pwrap_init,mustn't check init done */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
static S32 _pwrap_init_dio(U32 dio_en)
{
	U32 arb_en_backup = 0x0;
	U32 rdata = 0x0;
	U32 return_value = 0;

	/* PWRAPFUC(); */
	arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);
	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, WACS2);	/* only WACS2 */

#ifdef SLV_6351
	pwrap_write_nochk(MT6351_DEW_DIO_EN, dio_en);
#endif

	/* Check IDLE & INIT_DONE in advance */
	return_value =
	    wait_for_state_ready_init(wait_for_idle_and_sync, TIMEOUT_WAIT_IDLE,
				      PMIC_WRAP_WACS2_RDATA, 0);
	if (return_value != 0) {
		PWRAPLOG("_pwrap_init_dio fail,return_value=%x\n", return_value);
		return return_value;
	}
	/* Enable AP DIO mode */
	WRAP_WR32(PMIC_WRAP_DIO_EN, dio_en);
	/* Read Test */
#ifdef SLV_6351
	pwrap_read_nochk(MT6351_DEW_READ_TEST, &rdata);
	if (rdata != MT6351_DEFAULT_VALUE_READ_TEST) {
		PWRAPLOG("[Dio_mode][Read Test] fail,dio_en = %x, READ_TEST rdata=%x, exp=0x5aa5\n",
			 dio_en, rdata);
		return E_PWR_READ_TEST_FAIL;
	}
#endif

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, arb_en_backup);
	return 0;
}

/* -------------------------------------------------------- */
/* Function : _pwrap_init_cipher() */
/* Description : */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
static S32 _pwrap_init_cipher(void)
{
	U32 arb_en_backup = 0;
	U32 rdata = 0;
	U32 return_value = 0;
	U32 start_time_ns = 0, timeout_ns = 0;
	/* PWRAPFUC(); */
	arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, WACS2);	/* only WACS0 */

	WRAP_WR32(PMIC_WRAP_CIPHER_SWRST, 1);
	WRAP_WR32(PMIC_WRAP_CIPHER_SWRST, 0);
	WRAP_WR32(PMIC_WRAP_CIPHER_KEY_SEL, 1);
	WRAP_WR32(PMIC_WRAP_CIPHER_IV_SEL, 2);
	WRAP_WR32(PMIC_WRAP_CIPHER_EN, 1);

	/* Config CIPHER @ PMIC */
#ifdef SLV_6351
	pwrap_write_nochk(MT6351_DEW_CIPHER_SWRST, 0x1);
	pwrap_write_nochk(MT6351_DEW_CIPHER_SWRST, 0x0);
	pwrap_write_nochk(MT6351_DEW_CIPHER_KEY_SEL, 0x1);
	pwrap_write_nochk(MT6351_DEW_CIPHER_IV_SEL, 0x2);
	pwrap_write_nochk(MT6351_DEW_CIPHER_EN, 0x1);
#endif

	/* wait for cipher data ready@AP */
#ifndef CONFIG_MTK_FPGA
	return_value =
	    wait_for_state_ready_init(wait_for_cipher_ready, TIMEOUT_WAIT_IDLE,
				      PMIC_WRAP_CIPHER_RDY, 0);
#else
	return_value =
	    wait_for_state_ready_init(wait_for_cipher_ready, 0xFFFFFFFF, PMIC_WRAP_CIPHER_RDY, 0);
#endif
	if (return_value != 0) {
		PWRAPLOG("wait for cipher data ready@AP fail,return_value=%x\n", return_value);
		return return_value;
	}
	/* wait for cipher data ready@PMIC */
#ifdef SLV_6351
	start_time_ns = _pwrap_get_current_time();
	timeout_ns = _pwrap_time2ns(0xFFFFFF);
	do {
		if (_pwrap_timeout_ns(start_time_ns, timeout_ns))
			PWRAPLOG("wait for cipher data ready@PMIC\n");

		pwrap_read_nochk(MT6351_DEW_CIPHER_RDY, &rdata);
	} while (rdata != 0x1);	/* cipher_ready */

	pwrap_write_nochk(MT6351_DEW_CIPHER_MODE, 0x1);
#endif
	/* wait for cipher mode idle */
	return_value =
	    wait_for_state_ready_init(wait_for_idle_and_sync, TIMEOUT_WAIT_IDLE,
				      PMIC_WRAP_WACS2_RDATA, 0);
	if (return_value != 0) {
		PWRAPLOG("wait for cipher mode idle fail,return_value=%x\n", return_value);
		return return_value;
	}
	WRAP_WR32(PMIC_WRAP_CIPHER_MODE, 1);

	/* Read Test */
#ifdef SLV_6351
	pwrap_read_nochk(MT6351_DEW_READ_TEST, &rdata);
	if (rdata != MT6351_DEFAULT_VALUE_READ_TEST) {
		PWRAPLOG("_pwrap_init_cipher,read test error,error code=%x, rdata=%x\n", 1, rdata);
		return E_PWR_READ_TEST_FAIL;
	}
#endif

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, arb_en_backup);
	return 0;
}

/* -------------------------------------------------------- */
/* Function : _pwrap_init_sistrobe() */
/* Description : Initialize SI_CK_CON and SIDLY */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
static S32 _pwrap_init_sistrobe(void)
{
	U32 arb_en_backup = 0;
	U32 rdata = 0;
	S32 ind = 0;
	U32 result_faulty = 0;
	U64 result = 0, inds = 0, indf = 0, tmp1 = 0, tmp2 = 0;
	S32 leading_one = 0;
	S32 tailing_one = 0;

	arb_en_backup = WRAP_RD32(PMIC_WRAP_HIPRIO_ARB_EN);

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, WACS2);	/* only WACS2 */

#ifdef ULPOSC
	/* --------------------------------------------------------------------- */
	/* Scan all possible input strobe by READ_TEST */
	/* --------------------------------------------------------------------- */
	result = 0;
	result_faulty = 0;
	inds = 0;
	indf = 0;
	for (ind = 0; ind < 44; ind++) {	/* 44 sampling clock edge */
		WRAP_WR32(PMIC_WRAP_SI_CK_CON, (ind >> 2) & 0xf);
		WRAP_WR32(PMIC_WRAP_SIDLY, 0x3 - (ind & 0x3));
#ifdef SLV_6351
		_pwrap_wacs2_nochk(0, MT6351_DEW_READ_TEST, 0, &rdata);
		if (rdata == MT6351_DEFAULT_VALUE_READ_TEST) {
			PWRAPERR
			    ("_pwrap_init_sistrobe [Read Test of MT6351] pass,index=%d rdata=%x\n",
			     ind, rdata);
			result |= (0x1 << ind);
		} else {
			PWRAPERR
			    ("_pwrap_init_sistrobe [Read Test of MT6351] tuning,index=%d rdata=%x\n",
			     ind, rdata);
		}
#endif
	}

	/* --------------------------------------------------------------------- */
	/* Locate the leading one and trailing one */
	/* --------------------------------------------------------------------- */
#if 0
	for (ind = 43; ind >= 0; ind--) {
		/* if( result & (0x1 << ind) ) break; */
		if (ind > 20) {
			indf = (0x1 << 20);
			/* indf = (indf << ((ind - 20)%32)); */
		} else {
			indf = (0x1 << (ind % 32));
		}
		inds = result & indf;
		if (inds)
			break;
	}
	leading_one = ind;

	for (ind = 0; ind < 44; ind++) {
		/* if( result & (0x1 << ind) ) break; */
		if (ind > 20) {
			indf = (0x1 << 20);
			/* indf = (indf << ((ind - 20)%32)); */
		} else {
			indf = (0x1 << (ind % 32));
		}
		inds = result & indf;
		if (inds)
			break;
	}
	tailing_one = ind;
#else
	for (ind = 43; ind >= 0; ind--) {
		if (result & (0x1 << ind))
			break;
	}
	leading_one = ind;

	for (ind = 0; ind < 44; ind++) {
		if (result & (0x1 << ind))
			break;
	}
	tailing_one = ind;
#endif

	/* --------------------------------------------------------------------- */
	/* Check the continuity of pass range */
	/* --------------------------------------------------------------------- */
	tmp1 = (0x1 << (leading_one + 1)) - 1;
	tmp2 = (0x1 << tailing_one) - 1;
	if ((tmp1 - tmp2) != result) {
		/*TERR = "[DrvPWRAP_InitSiStrobe] Fail, tmp1:%d, tmp2:%d", tmp1, tmp2 */
		PWRAPERR
		    ("_pwrap_init_sistrobe Fail, result = %llx, leading_one:%d, tailing_one:%d\n",
		     result, leading_one, tailing_one);
		result_faulty = 0x1;
	}
	/* --------------------------------------------------------------------- */
	/* Config SICK and SIDLY to the middle point of pass range */
	/* --------------------------------------------------------------------- */
	ind = (leading_one + tailing_one) / 2;
	WRAP_WR32(PMIC_WRAP_SI_CK_CON, (ind >> 2) & 0xf);
	WRAP_WR32(PMIC_WRAP_SIDLY, 0x3 - (ind & 0x3));

#else				/* Else of #ifdef ULPOSC */

	/* --------------------------------------------------------------------- */
	/* Scan all possible input strobe by READ_TEST */
	/* --------------------------------------------------------------------- */
	for (ind = 0; ind < 24; ind++) {	/* 24 sampling clock edge */
		WRAP_WR32(PMIC_WRAP_SI_CK_CON, (ind >> 2) & 0x7);
		WRAP_WR32(PMIC_WRAP_SIDLY, 0x3 - (ind & 0x3));
#ifdef SLV_6351
		_pwrap_wacs2_nochk(0, MT6351_DEW_READ_TEST, 0, &rdata);
		if (rdata == MT6351_DEFAULT_VALUE_READ_TEST) {
			PWRAPERR
			    ("_pwrap_init_sistrobe [Read Test of MT6351] pass,index=%d rdata=%x\n",
			     ind, rdata);
			result |= (0x1 << ind);
		} else {
			PWRAPERR
			    ("_pwrap_init_sistrobe [Read Test of MT6351] tuning,index=%d rdata=%x\n",
			     ind, rdata);
		}
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
	if ((tmp1 - tmp2) != result) {
		/*TERR = "[PWRAP] Check Continuity Fail at PMIC %d, result = %x, leading_one:%d, tailing_one:%d"
		   , i+1, result[i], leading_one[i], tailing_one[i] */
		PWRAPERR
		    ("_pwrap_init_sistrobe Fail, result = %llx, leading_one:%d, tailing_one:%d\n",
		     result, leading_one, tailing_one);
		result_faulty = 0x1;
	}
	/* --------------------------------------------------------------------- */
	/* Config SICK and SIDLY to the middle point of pass range */
	/* --------------------------------------------------------------------- */
	if (result_faulty == 0) {
		ind = (leading_one + tailing_one) / 2;
		WRAP_WR32(PMIC_WRAP_SI_CK_CON, (ind >> 2) & 0x7);
		WRAP_WR32(PMIC_WRAP_SIDLY, 0x3 - (ind & 0x3));
		/* --------------------------------------------------------------------- */
		/* Restore */
		/* --------------------------------------------------------------------- */
		WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, arb_en_backup);
		return 0;
	} else {
		PWRAPERR("_pwrap_init_sistrobe Fail,result_faulty=%x\n", result_faulty);
		return result_faulty;
	}
#endif				/* End of #ifdef ULPOSC */

}

/* -------------------------------------------------------- */
/* Function : _pwrap_reset_spislv() */
/* Description : */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
static S32 _pwrap_reset_spislv(void)
{
	U32 ret = 0;
	U32 return_value = 0;
	/* PWRAPFUC(); */
	/* This driver does not using _pwrap_switch_mux */
	/* because the remaining requests are expected to fail anyway */

	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, DISABLE_ALL);
	WRAP_WR32(PMIC_WRAP_WRAP_EN, DISABLE);
	WRAP_WR32(PMIC_WRAP_MUX_SEL, MANUAL_MODE);
	WRAP_WR32(PMIC_WRAP_MAN_EN, ENABLE);
	WRAP_WR32(PMIC_WRAP_DIO_EN, DISABLE);

	WRAP_WR32(PMIC_WRAP_MAN_CMD, (OP_WR << 13) | (OP_CSL << 8));	/* 0x2100 */
	WRAP_WR32(PMIC_WRAP_MAN_CMD, (OP_WR << 13) | (OP_OUTS << 8));	/* 0x2800//to reset counter */
	WRAP_WR32(PMIC_WRAP_MAN_CMD, (OP_WR << 13) | (OP_CSH << 8));	/* 0x2000 */
	WRAP_WR32(PMIC_WRAP_MAN_CMD, (OP_WR << 13) | (OP_OUTS << 8));
	WRAP_WR32(PMIC_WRAP_MAN_CMD, (OP_WR << 13) | (OP_OUTS << 8));
	WRAP_WR32(PMIC_WRAP_MAN_CMD, (OP_WR << 13) | (OP_OUTS << 8));
	WRAP_WR32(PMIC_WRAP_MAN_CMD, (OP_WR << 13) | (OP_OUTS << 8));

	return_value =
	    wait_for_state_ready_init(wait_for_sync, TIMEOUT_WAIT_IDLE, PMIC_WRAP_WACS2_RDATA, 0);
	if (return_value != 0) {
		PWRAPLOG("_pwrap_reset_spislv fail,return_value=%x\n", return_value);
		ret = E_PWR_TIMEOUT;
		goto timeout;
	}

	WRAP_WR32(PMIC_WRAP_MAN_EN, DISABLE);
	WRAP_WR32(PMIC_WRAP_MUX_SEL, WRAPPER_MODE);

timeout:
	WRAP_WR32(PMIC_WRAP_MAN_EN, DISABLE);
	WRAP_WR32(PMIC_WRAP_MUX_SEL, WRAPPER_MODE);
	return ret;
}

#if 0
static void __pwrap_soft_reset(void)
{
	PWRAPLOG("start reset wrapper\n");
	PWRAP_SOFT_RESET;
	PWRAPLOG("the reset register =%x\n", WRAP_RD32(INFRA_GLOBALCON_RST0));
	PWRAPLOG("PMIC_WRAP_STAUPD_GRPEN =0x%x,it should be equal to 0xc\n",
		 WRAP_RD32(PMIC_WRAP_STAUPD_GRPEN));
	/* clear reset bit */
	PWRAP_CLEAR_SOFT_RESET_BIT;
	return;
}
#endif

static S32 _pwrap_init_reg_clock(U32 regck_sel)
{
	PWRAPFUC();

#ifdef SLV_6351
	pwrap_write_nochk(MT6351_DEW_RDDMY_NO, 0x8);
	WRAP_WR32(PMIC_WRAP_RDDMY, 0x08);
#endif

	/* Config SPI Waveform according to reg clk */
	if (regck_sel == 1) {	/* 18MHz in 6328, 26Mhz in BBCHip */
		WRAP_WR32(PMIC_WRAP_CSHEXT_READ, 0x0);
		/* Wait data written into register =>
		   4T_PMIC: consists of CSHEXT_WRITE_START+EXT_CK+CSHEXT_WRITE_END+CSLEXT_START */
		WRAP_WR32(PMIC_WRAP_CSHEXT_WRITE, 0x33);
		WRAP_WR32(PMIC_WRAP_CSLEXT_START, 0x0);
		WRAP_WR32(PMIC_WRAP_CSLEXT_END, 0x0);	/* for PMIC site synchronizer use */
	} else {		/* Safe mode */
		WRAP_WR32(PMIC_WRAP_CSHEXT_WRITE, 0xff);
		WRAP_WR32(PMIC_WRAP_CSHEXT_READ, 0xff);
		WRAP_WR32(PMIC_WRAP_CSLEXT_START, 0xf);
		WRAP_WR32(PMIC_WRAP_CSLEXT_END, 0xf);
	}

	return 0;
}

/*
 * pmic_wrap init,init wrap interface
 */
S32 pwrap_init(void)
{
	S32 sub_return = 0;
	S32 sub_return1 = 0;
	/* S32 ret=0; */
	U32 rdata = 0x0;
	/* U32 timeout=0; */
	u32 cg_mask = 0;
	u32 backup = 0;

	PWRAPFUC();
#ifdef PMIC_WRAP_KERNEL_DRIVER
#ifdef CONFIG_OF
	sub_return = pwrap_of_iomap();
	if (sub_return)
		return sub_return;
#endif
#endif

#ifndef CONFIG_MTK_FPGA

	/* ############################### */
	/* toggle PMIC_WRAP and pwrap_spictl reset */
	/* ############################### */
	WRAP_WR32(INFRA_GLOBALCON_RST2_SET, 0x1);	/* Write 1 set */
	WRAP_WR32(INFRA_GLOBALCON_RST2_CLR, 0x1);	/* Write 1 clr */

	/* ############################### */
	/* Switch GPIO to PWRAP mode => modes of PMIC_WRAP's IOs are PMIC_WRAP in default setting */
	/* ############################### */
	/* SET_GPIO_MODE (40, 1);  // MI */
	/* SET_GPIO_MODE (41, 1);  // MO */
	/* SET_GPIO_MODE (38, 1);  // CK */
	/* SET_GPIO_MODE (39, 1);  // CS */

	/* ############################### */
	/* Set SPI_CK_freq  52MHz or 26MHz */
	/* ############################### */
#ifdef ULPOSC
	WRAP_WR32(ULPOSC_CON, 0x0001);	/* Enable ulposc */
	udelay(1);
	WRAP_WR32(ULPOSC_CON, 0x0003);	/* Reset ulposc */
	udelay(30);
	WRAP_WR32(ULPOSC_CON, 0x0001);	/* Release reset ulposc */
	udelay(130);
	WRAP_WR32(ULPOSC_CON, 0x0005);
	WRAP_WR32(CLK_CFG_5_SET, 0x0002);	/* 52MHz [1:0] */
	udelay(20);
	/* WRAP_WR32(GPIO_DUMMY, 0x80000000);  /* remove by ECO */ */
	WRAP_WR32(PMICW_CLOCK_CTRL, 0x0000);	/* pmicw_sw_en */
	WRAP_WR32(CLK_CFG_UPDATE, 0x00080000);	/* Clock configure update */
	WRAP_WR32(MODULE_SW_CG_0_SET, 0x0000000F);	/* sys_ck cg enable */
	WRAP_WR32(MODULE_SW_CG_0_CLR, 0x0000000F);	/* sys_ck cg disable */
#else
	WRAP_WR32(CLK_CFG_5_CLR, 0x0001);	/* 26MHz [1:0] */
	WRAP_WR32(MODULE_SW_CG_0_SET, 0x0000000F);	/* sys_ck cg enable */
	WRAP_WR32(MODULE_SW_CG_0_CLR, 0x0000000F);	/* sys_ck cg disable */
#endif
#endif				/* End of #ifndef CONFIG_MTK_FPGA */

	/* ############################### */
	/* Enable DCM */
	/* ############################### */
	WRAP_WR32(PMIC_WRAP_DCM_EN, 3);	/* enable CRC DCM and PWRAP DCM */
	WRAP_WR32(PMIC_WRAP_DCM_DBC_PRD, DISABLE);	/* No debounce */
	/* 0x10001074 bit[22] = 1¡¦b0 to disable pmic clock DCM */
	sub_return = WRAP_RD32(PMIC_CLOCK_DCM);
	WRAP_WR32(PMIC_CLOCK_DCM, (sub_return & 0xFFBFFFFF));
	//PWRAPERR("Enable DCM before = %x, after= %x\n", sub_return, WRAP_RD32(PMIC_CLOCK_DCM));
	/* 0x10001F0C bit[25] = 1¡¦b1 to disable apb clock gating mechanism. */
	sub_return = WRAP_RD32(APB_CLOCK_GATING);
	WRAP_WR32(APB_CLOCK_GATING, ((sub_return & 0xFDFFFFFF) | 0x02000000));
	//PWRAPERR("disable apb clock gating before =gg %x, after= %x\n", sub_return, WRAP_RD32(APB_CLOCK_GATING));
	
	/* ############################### */
	/* Reset all SPISLV */
	/* ############################### */
	sub_return = _pwrap_reset_spislv();
	if (sub_return != 0) {
		PWRAPLOG("error,_pwrap_reset_spislv fail,sub_return=%x\n", sub_return);
		return E_PWR_INIT_RESET_SPI;
	}
	/* ############################### */
	/* Enable WACS2 */
	/* ############################### */
	WRAP_WR32(PMIC_WRAP_WRAP_EN, ENABLE);	/* enable wrap */
	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, WACS2);	/* Only WACS2 */
	WRAP_WR32(PMIC_WRAP_WACS2_EN, ENABLE);

	/* ############################### */
	/* Input data calibration flow */
	/* ############################### */
	sub_return = _pwrap_init_sistrobe();
	if (sub_return != 0) {
		PWRAPLOG("error,DrvPWRAP_InitSiStrobe fail,sub_return=%x\n", sub_return);
		return E_PWR_INIT_SIDLY;
	}
	/* ############################### */
	/* SPI Waveform Configuration */
	/* ############################### */
	/* 0:safe mode, 1:18MHz */
	sub_return = _pwrap_init_reg_clock(1);
	if (sub_return != 0) {
		PWRAPLOG("error,_pwrap_init_reg_clock fail,sub_return=%x\n", sub_return);
		return E_PWR_INIT_REG_CLOCK;
	}
	/* ############################### */
	/* Enable DIO mode */
	/* ############################### */
	sub_return = _pwrap_init_dio(1);
	if (sub_return != 0) {
		PWRAPLOG("_pwrap_init_dio test error,error code=%x, sub_return=%x\n", 0x11,
			 sub_return);
		return E_PWR_INIT_DIO;
	}
	/* ############################### */
	/* Enable Encryption */
	/* ############################### */
	sub_return = _pwrap_init_cipher();
	if (sub_return != 0) {
		PWRAPLOG("Enable Encryption fail, return=%x\n", sub_return);
		return E_PWR_INIT_CIPHER;
	}
	/* ############################### */
	/* Write test using WACS2 */
	/* ############################### */
#ifdef SLV_6351
	sub_return = pwrap_write_nochk(MT6351_DEW_WRITE_TEST, MT6351_WRITE_TEST_VALUE);
	sub_return1 = pwrap_read_nochk(MT6351_DEW_WRITE_TEST, &rdata);
	if (rdata != MT6351_WRITE_TEST_VALUE) {
		PWRAPLOG
		    ("write test error,rdata=0x%x,exp=0xa55a,sub_return=0x%x,sub_return1=0x%x\n",
		     rdata, sub_return, sub_return1);
		return E_PWR_INIT_WRITE_TEST;
	}
#endif

	/* ############################### */
	/* Status update function initialization */
	/* 1. Signature Checking using CRC (CRC 0 only) */
	/* 2. EINT update */
	/* 3. Read back Auxadc thermal data for GPD */
	/* 4. Read back Auxadc thermal data for LTE */
	/* ############################### */
#if 0
	sub_return = pwrap_write_nochk(MT6351_DEW_CRC_EN, ENABLE);
	if (sub_return != 0) {
		PWRAPLOG("Enable CRC fail, return=%x\n", sub_return);
		return E_PWR_INIT_ENABLE_CRC;
	}
	WRAP_WR32(PMIC_WRAP_WACS2_EN, MT6351_DEW_CRC_VAL);
#endif
	WRAP_WR32(PMIC_WRAP_STAUPD_GRPEN, 0x75);
#if 0
	WRAP_WR32(PMIC_WRAP_CRC_EN, ENABLE);
#endif
	WRAP_WR32(PMIC_WRAP_EINT_STA0_ADR, MT6351_INT_STA);

	WRAP_WR32(PMIC_WRAP_ADC_CMD_ADDR, MT6351_LDO_RSV_CON1);
	WRAP_WR32(PMIC_WRAP_PWRAP_ADC_CMD, 0x0100);
	WRAP_WR32(PMIC_WRAP_ADC_RDATA_ADDR, MT6351_AUXADC_ADC32);
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
	//###############################
	// PMIC_WRAP priority adjusted
	//###############################
	WRAP_WR32(PMIC_WRAP_PRIORITY_USER_SEL_0, 0x6543C210);
	WRAP_WR32(PMIC_WRAP_PRIORITY_USER_SEL_1, 0xFEDBA987);
	WRAP_WR32(PMIC_WRAP_ARBITER_OUT_SEL_0, 0x87654210);
	WRAP_WR32(PMIC_WRAP_ARBITER_OUT_SEL_1, 0xFED3CBA9);

	/* ############################### */
	/* PMIC_WRAP starvation setting */
	/* ############################### */
	/* latency is smaller than 10us, pop up to group"1" first */
	WRAP_WR32(PMIC_WRAP_HARB_HPRIO, 0x0007);	/* [0]:MD_HW [1]:C2K_HW [2]:MD_DVFS _HW */
	/* starvation enable */
	/* [0]=MD_HW, latency= 2.6us, target= (2.6/1.2)=2 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_0, 0x0402);	/* [10]: enable starvation, [9:0]: target setting */
	/* [1]=C2K_HW, latency= 2.6us, target= (2.6/1.2)=2 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_1, 0x0402);	/* [10]: enable starvation, [9:0]: target setting */
	/* [2]=MD_DVFS_HW,latency= 5.2us, target= (5.2/1.2)-1=3 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_2, 0x0403);	/* [10]: enable starvation, [9:0]: target setting */
	/* [3]=WACS1(MD_SW1), latency=20us, target= (20/1.2)-1=25 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_3, 0x0414);	/* [10]: enable starvation, [9:0]: target setting */
	/* [4]=WACS0(MD_SW0), latency=40us, target= (40/1.2)-1=32 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_4, 0x0420);	/* [10]: enable starvation, [9:0]: target setting */
	/* [5]=SPM_HW, latency=40us, target= (40/1.2)-1=32 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_5, 0x0420);	/* [10]: enable starvation, [9:0]: target setting */
	/* [6]=WACS3(C2K_SW), latency=40us, target= (40/1.2)-1=32 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_6, 0x0420);	/* [10]: enable starvation, [9:0]: target setting */
	/* [7]=DCXO_CONNINF, latency=50us, target= (50/1.2)-1=40 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_7, 0x0428);	/* [10]: enable starvation, [9:0]: target setting */
	/* [8]=DCXO_NFCINF, latency=50us, target= (50/1.2)-1=40 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_8, 0x0428);	/* [10]: enable starvation, [9:0]: target setting */
	/* [9]=MD_ADCINF, latency=29us, target= (29/1.2)-1= 23 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_9, 0x0417);	/* [10]: enable starvation, [9:0]: target setting */
	/* [10]=STAUPD, latency=428us, target= (428/1.2)-1=355 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_10, 0x0563);	/* [10]: enable starvation, [9:0]: target setting */
	/* [11]=GPS_HW, latency=150us, target= (150/1.2)-1=124 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_11, 0x047C);	/* [10]: enable starvation, [9:0]: target setting */
	/* [12]=WACS2(AP_SW), latency=1000us, target= (1000/1.2)-1=832 */
	WRAP_WR32(PMIC_WRAP_STARV_COUNTER_12, 0x0740);	/* [10]: enable starvation, [9:0]: target setting */

	/* ############################### */
	/* PMIC_WRAP enables */
	/* ############################### */
	WRAP_WR32(PMIC_WRAP_HIPRIO_ARB_EN, 0x01fff);
	WRAP_WR32(PMIC_WRAP_WACS0_EN, ENABLE);
	WRAP_WR32(PMIC_WRAP_WACS1_EN, ENABLE);
	WRAP_WR32(PMIC_WRAP_WACS3_EN, ENABLE);
	WRAP_WR32(PMIC_WRAP_STAUPD_PRD, 0x5);	/* 100us */
	WRAP_WR32(PMIC_WRAP_WDT_UNIT, 0xf);
	WRAP_WR32(PMIC_WRAP_WDT_SRC_EN, 0xffffffff);
	WRAP_WR32(PMIC_WRAP_TIMER_EN, 0x1);
	/* WRAP_WR32(PMIC_WRAP_INT0_EN, 0xffffffff); */
	WRAP_WR32(PMIC_WRAP_INT0_EN, 0xfffffffD);	/* FIX ME */
	WRAP_WR32(PMIC_WRAP_INT1_EN, 0x0001ffff);

	/* ############################### */
	/* Initialization Done */
	/* ############################### */
	WRAP_WR32(PMIC_WRAP_INIT_DONE0, ENABLE);	/* MD SW0 */
	WRAP_WR32(PMIC_WRAP_INIT_DONE1, ENABLE);	/* MD SW1 */
	WRAP_WR32(PMIC_WRAP_INIT_DONE2, ENABLE);	/* AP SW */
	WRAP_WR32(PMIC_WRAP_INIT_DONE3, ENABLE);	/* C2K SW */

#ifdef PMIC_WRAP_KERNEL_DRIVER
#ifdef CONFIG_OF
	pwrap_of_iounmap();
#endif
#endif

	return 0;
}

#ifdef PMIC_WRAP_KERNEL_DRIVER
static irqreturn_t mt_pmic_wrap_irq(int irqno, void *dev_id)
{
	unsigned long flags = 0;

	PWRAPFUC();
	PWRAPREG("dump pwrap register\n");
	spin_lock_irqsave(&wrp_lock_isr, flags);
	/* *----------------------------------------------------------------------- */
	pwrap_dump_all_register();
	/* raise the priority of WACS2 for AP */
	WRAP_WR32(PMIC_WRAP_HARB_HPRIO, WACS2);

	/* *----------------------------------------------------------------------- */
	PWRAPREG("INT flag 0x%x\n", WRAP_RD32(PMIC_WRAP_INT0_FLG_RAW));
	PWRAPREG("INT flag 0x%x\n", WRAP_RD32(PMIC_WRAP_INT1_FLG_RAW));
	/* clear interrupt flag */
	WRAP_WR32(PMIC_WRAP_INT0_CLR, 0xffffffff);
	WRAP_WR32(PMIC_WRAP_INT1_CLR, 0xffffffff);
	BUG_ON(1);
	spin_unlock_irqrestore(&wrp_lock_isr, flags);
	return IRQ_HANDLED;
}


static U32 pwrap_read_test(void)
{
	U32 rdata = 0;
	U32 return_value = 0;
	/* Read Test */
#ifdef SLV_6351
	return_value = pwrap_read(MT6351_DEW_READ_TEST, &rdata);
	if (rdata != MT6351_DEFAULT_VALUE_READ_TEST) {
		PWRAPREG("Read Test fail,rdata=0x%x, exp=0x5aa5,return_value=0x%x\n", rdata,
			 return_value);
		return E_PWR_READ_TEST_FAIL;
	} else {
		PWRAPREG("Read Test pass,return_value=%d\n", return_value);
		/* return 0; */
	}
#endif

	return 0;
}

static U32 pwrap_write_test(void)
{
	U32 rdata = 0;
	U32 sub_return = 0;
	U32 sub_return1 = 0;
	/* ############################### */
	/* Write test using WACS2 */
	/* ############################### */
#ifdef SLV_6351
	sub_return = pwrap_write(MT6351_DEW_WRITE_TEST, MT6351_WRITE_TEST_VALUE);
	PWRAPREG("after MT6351 pwrap_write\n");
	sub_return1 = pwrap_read(MT6351_DEW_WRITE_TEST, &rdata);
	if ((rdata != MT6351_WRITE_TEST_VALUE) || (sub_return != 0) || (sub_return1 != 0)) {
		PWRAPREG
		    ("write test error,rdata=0x%x,exp=0xa55a,sub_return=0x%x,sub_return1=0x%x\n",
		     rdata, sub_return, sub_return1);
		return E_PWR_INIT_WRITE_TEST;
	} else {
		PWRAPREG("write MT6351 Test pass\n");
		/* return 0; */
	}
#endif

	return 0;
}

static void pwrap_int_test(void)
{
	U32 rdata1 = 0;
	U32 rdata2 = 0;

	while (1) {
#ifdef SLV_6351
		rdata1 = WRAP_RD32(PMIC_WRAP_EINT_STA);
		pwrap_read(MT6351_INT_STA, &rdata2);
		PWRAPREG
		    ("Pwrap INT status check,PMIC_WRAP_EINT_STA=0x%x,MT6351_INT_STA[0x01B4]=0x%x\n",
		     rdata1, rdata2);
#endif
		msleep(500);
	}
}

#ifndef USER_BUILD_KERNEL
static void pwrap_read_reg_on_pmic(U32 reg_addr)
{
	U32 reg_value = 0;
	U32 return_value = 0;
	/* PWRAPFUC(); */
	return_value = pwrap_read(reg_addr, &reg_value);
	PWRAPREG("0x%x=0x%x,return_value=%x\n", reg_addr, reg_value, return_value);
}

static void pwrap_write_reg_on_pmic(U32 reg_addr, U32 reg_value)
{
	U32 return_value = 0;

	PWRAPREG("write 0x%x to register 0x%x\n", reg_value, reg_addr);
	return_value = pwrap_write(reg_addr, reg_value);
	return_value = pwrap_read(reg_addr, &reg_value);
	/* PWRAPFUC(); */
	PWRAPREG("the result:0x%x=0x%x,return_value=%x\n", reg_addr, reg_value, return_value);
}
#endif

static void pwrap_ut(U32 ut_test)
{
	switch (ut_test) {
	case 1:
		/* pwrap_wacs2_para_test(); */
		pwrap_write_test();
		break;
	case 2:
		/* pwrap_wacs2_para_test(); */
		pwrap_read_test();
		break;

	default:
		PWRAPREG("default test.\n");
		break;
	}
	return;
}

/*---------------------------------------------------------------------------*/
static S32 mt_pwrap_show_hal(char *buf)
{
	PWRAPFUC();
	return snprintf(buf, PAGE_SIZE, "%s\n", "no implement");
}

/*---------------------------------------------------------------------------*/
static S32 mt_pwrap_store_hal(const char *buf, size_t count)
{
#ifndef USER_BUILD_KERNEL
	U32 reg_value = 0;
	U32 reg_addr = 0;
#endif
	U32 return_value = 0;
	U32 ut_test = 0;

	if (!strncmp(buf, "-h", 2)) {
		PWRAPREG
		    ("PWRAP debug: [-dump_reg][-trace_wacs2][-init][-rdpmic][-wrpmic][-readtest][-writetest]\n");
		PWRAPREG("PWRAP UT: [1][2]\n");
	}
	/* ----------pwrap debug---------- */
	else if (!strncmp(buf, "-dump_reg", 9)) {
		pwrap_dump_all_register();
	} else if (!strncmp(buf, "-trace_wacs2", 12)) {
		/* pwrap_trace_wacs2(); */
	} else if (!strncmp(buf, "-init", 5)) {
		return_value = pwrap_init();
		if (return_value == 0)
			PWRAPREG("pwrap_init pass,return_value=%d\n", return_value);
		else
			PWRAPREG("pwrap_init fail,return_value=%d\n", return_value);
	}
#ifndef USER_BUILD_KERNEL
	else if (!strncmp(buf, "-rdpmic", 7) && (1 == sscanf(buf + 7, "%x", &reg_addr))) {
		pwrap_read_reg_on_pmic(reg_addr);
	} else if (!strncmp(buf, "-wrpmic", 7)
		   && (2 == sscanf(buf + 7, "%x %x", &reg_addr, &reg_value))) {
		pwrap_write_reg_on_pmic(reg_addr, reg_value);
	}
#endif
	else if (!strncmp(buf, "-readtest", 9)) {
		pwrap_read_test();
	} else if (!strncmp(buf, "-writetest", 10)) {
		pwrap_write_test();
	} else if (!strncmp(buf, "-int", 4)) {
		pwrap_int_test();
	}
	/* ----------pwrap UT---------- */
	else if (!strncmp(buf, "-ut", 3) && (1 == sscanf(buf + 3, "%d", &ut_test))) {
		pwrap_ut(ut_test);
	} else {
		PWRAPREG("wrong parameter\n");
	}
	return count;
}

/*---------------------------------------------------------------------------*/
#endif				/* End of #endif PMIC_WRAP_KERNEL_DRIVER */

#ifdef PMIC_WRAP_PRELOADER_DRIVER
S32 pwrap_init_preloader(void)
{
	u32 pwrap_ret = 0, i = 0;

	PWRAPFUC();
	for (i = 0; i < 3; i++) {
		pwrap_ret = pwrap_init();
		if (pwrap_ret != 0) {
			printf("[PMIC_WRAP]wrap_init fail,the return value=%x.\n", pwrap_ret);
		} else {
			printf("[PMIC_WRAP]wrap_init pass,the return value=%x.\n", pwrap_ret);
			break;	/* init pass */
		}
	}
#ifdef PWRAP_PRELOADER_PORTING
	/* pwrap_init_for_early_porting(); */
#endif
	return 0;
}

#ifdef PWRAP_PRELOADER_PORTING
/* -------------------------------------------------------- */
/* Function : _pwrap_status_update_test() */
/* Description :only for early porting */
/* Parameter : */
/* Return : */
/* -------------------------------------------------------- */
static S32 _pwrap_status_update_test_porting(void)
{
	/* U32 i, j; */
	U32 rdata;

	volatile U32 delay = 1000 * 1000 * 1;

	PWRAPFUC();
	/* disable signature interrupt */
	WRAP_WR32(PMIC_WRAP_INT_EN, 0x0);
#ifdef SLV_6351
	pwrap_write(MT6351_DEW_WRITE_TEST, MT6351_WRITE_TEST_VALUE);
	WRAP_WR32(PMIC_WRAP_SIG_ADR, MT6351_DEW_WRITE_TEST);
	WRAP_WR32(PMIC_WRAP_SIG_VALUE, 0xAA55);
#endif
	WRAP_WR32(PMIC_WRAP_SIG_MODE, 0x1);

	while (delay--)
	;

	rdata = WRAP_RD32(PMIC_WRAP_SIG_ERRVAL);
#ifdef SLV_6351
	if ((rdata & 0xFFFF) != MT6351_WRITE_TEST_VALUE) {
		PWRAPLOG("MT6351 _pwrap_status_update_test error,error code=%x, rdata=%x\n", 1,
			 (rdata & 0xFFFF));
		/* return 1; */
	}
#endif
#ifdef SLV_6351
	WRAP_WR32(PMIC_WRAP_SIG_VALUE, MT6351_WRITE_TEST_VALUE);	/* tha same as write test */
#endif

	/* clear sig_error interrupt flag bit */
	WRAP_WR32(PMIC_WRAP_INT_CLR, 1 << 1);

	/* enable signature interrupt */
	WRAP_WR32(PMIC_WRAP_INT_EN, 0x7ffffff9);
	WRAP_WR32(PMIC_WRAP_SIG_MODE, 0x0);
#ifdef SLV_6351
	WRAP_WR32(PMIC_WRAP_SIG_ADR, MT6351_DEW_CRC_VAL);
#endif
	return 0;
}

void pwrap_init_for_early_porting(void)
{
	int ret = 0;
	U32 res = 0;

	PWRAPFUC();

	ret = _pwrap_status_update_test_porting();
	if (ret == 0) {
		PWRAPLOG("wrapper_StatusUpdateTest pass.\n");
	} else {
		PWRAPLOG("error:wrapper_StatusUpdateTest fail.\n");
		res += 1;
	}
}
#endif				/* PWRAP_PRELOADER_PORTING */
#endif				/* End of #ifdef PMIC_WRAP_PRELOADER_DRIVER */

#ifdef PMIC_WRAP_LK_DRIVER
S32 pwrap_init_lk(void)
{
	u32 pwrap_ret = 0, i = 0;

	PWRAPFUC();
	for (i = 0; i < 3; i++) {
		pwrap_ret = pwrap_init();
		if (pwrap_ret != 0) {
			printf("[PMIC_WRAP]wrap_init fail,the return value=%x.\n", pwrap_ret);
		} else {
			printf("[PMIC_WRAP]wrap_init pass,the return value=%x.\n", pwrap_ret);
			break;	/* init pass */
		}
	}
#ifdef PWRAP_PRELOADER_PORTING
	/* pwrap_init_for_early_porting(); */
#endif
	return 0;
}
#endif

#endif				/* End of #if PMIC_WRAP_NO_PMIC */

#ifdef PMIC_WRAP_KERNEL_DRIVER
#ifdef CONFIG_OF
static int pwrap_of_iomap(void)
{
	/*
	 * Map the address of the following register base:
	 * TOPCKGEN, INFRACFG_AO, GPIO, SLEEP
	 */

	struct device_node *topckgen_node;
	struct device_node *infracfg_ao_node;
	struct device_node *gpio_node;
	struct device_node *sleep_node;

	topckgen_node = of_find_compatible_node(NULL, NULL, "mediatek,TOPCKGEN");
	if (!topckgen_node) {
		pr_info("get TOPCKGEN failed\n");
		return -ENODEV;
	}

	topckgen_reg_base = of_iomap(topckgen_node, 0);
	if (!topckgen_reg_base) {
		pr_info("TOPCKGEN iomap failed\n");
		return -ENOMEM;
	}

	infracfg_ao_node = of_find_compatible_node(NULL, NULL, "mediatek,INFRACFG_AO");
	if (!infracfg_ao_node) {
		pr_info("get INFRACFG_AO failed\n");
		return -ENODEV;
	}

	infracfg_ao_reg_base = of_iomap(infracfg_ao_node, 0);
	if (!infracfg_ao_reg_base) {
		pr_info("INFRACFG_AO iomap failed\n");
		return -ENOMEM;
	}

	gpio_node = of_find_compatible_node(NULL, NULL, "mediatek,GPIO");
	if (!gpio_node) {
		pr_info("get GPIO failed\n");
		return -ENODEV;
	}

	gpio_reg_base = of_iomap(gpio_node, 0);
	if (!gpio_reg_base) {
		pr_info("GPIO iomap failed\n");
		return -ENOMEM;
	}

	sleep_node = of_find_compatible_node(NULL, NULL, "mediatek,SLEEP");
	if (!sleep_node) {
		pr_info("get SLEEP failed\n");
		return -ENODEV;
	}

	sleep_reg_base = of_iomap(sleep_node, 0);
	if (!sleep_reg_base) {
		pr_info("SLEEp iomap failed\n");
		return -ENOMEM;
	}

	pr_info("TOPCKGEN reg: 0x%p\n", topckgen_reg_base);
	pr_info("INFRACFG_AO reg: 0x%p\n", infracfg_ao_reg_base);
	pr_info("GPIO reg: 0x%p\n", gpio_reg_base);
	pr_info("SLEEP reg: 0x%p\n", sleep_reg_base);

	return 0;
}

static void pwrap_of_iounmap(void)
{
	iounmap(topckgen_reg_base);
	iounmap(infracfg_ao_reg_base);
	iounmap(gpio_reg_base);
	iounmap(sleep_reg_base);
}
#endif

#define VERSION     "$Revision$"
static int is_pwrap_init_done(void)
{
	int ret = 0;

	ret = WRAP_RD32(PMIC_WRAP_INIT_DONE2);
	if (ret != 0)
		return 0;

	ret = pwrap_init();
	if (ret != 0) {
		PWRAPERR("init error (%d)\n", ret);
		pwrap_dump_all_register();
		return ret;
	}
	PWRAPLOG("init successfully done (%d)\n\n", ret);
	return ret;
}

static int __init pwrap_hal_init(void)
{
	S32 ret = 0;
#ifdef CONFIG_OF
	u32 pwrap_irq;
	struct device_node *pwrap_node;

	pwrap_node = of_find_compatible_node(NULL, NULL, "mediatek,PWRAP");
	if (!pwrap_node) {
		pr_info("PWRAP get node failed\n");
		return -ENODEV;
	}

	pwrap_base = of_iomap(pwrap_node, 0);
	if (!pwrap_base) {
		pr_info("PWRAP iomap failed\n");
		return -ENOMEM;
	}

	pwrap_irq = irq_of_parse_and_map(pwrap_node, 0);
	if (!pwrap_irq) {
		pr_info("PWRAP get irq fail\n");
		return -ENODEV;
	}
	pr_info("PWRAP reg: 0x%p,  irq: %d\n", pwrap_base, pwrap_irq);
#endif

	PWRAPLOG("HAL init: version %s\n", VERSION);
	mt_wrp = get_mt_pmic_wrap_drv();
	mt_wrp->store_hal = mt_pwrap_store_hal;
	mt_wrp->show_hal = mt_pwrap_show_hal;
	mt_wrp->wacs2_hal = pwrap_wacs2_hal;

	if (is_pwrap_init_done() == 0) {
#ifdef PMIC_WRAP_NO_PMIC
#else

		ret =
		    request_irq(MT_PMIC_WRAP_IRQ_ID, mt_pmic_wrap_irq, IRQF_TRIGGER_HIGH,
				PMIC_WRAP_DEVICE, 0);
#endif
		if (ret) {
			PWRAPERR("register IRQ failed (%d)\n", ret);
			return ret;
		}
	} else {
		PWRAPERR("not init (%d)\n", ret);
	}

	/* PWRAPERR("not init (%x)\n", is_pwrap_init_done); */

	return ret;
}
postcore_initcall(pwrap_hal_init);
#endif				/* End of #ifdef PMIC_WRAP_KERNEL_DRIVER */

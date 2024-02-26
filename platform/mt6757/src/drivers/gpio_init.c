/******************************************************************************
 * gpio_init.c - MT6516 Linux GPIO Device Driver
 * 
 * Copyright 2008-2009 MediaTek Co.,Ltd.
 * 
 * DESCRIPTION:
 *     default GPIO init
 *
 ******************************************************************************/

#include <gpio.h>

#if ((!CFG_FPGA_PLATFORM) || defined (FPGA_SIMULATION))
#include <cust_power.h>
#include <cust_gpio_boot.h>
#include <gpio_init.h>
#include <platform.h>
#ifdef SELF_TEST
void mt_gpio_self_test(void);
#endif
//#include <debug.h>
#define GPIO_INIT_DEBUG 1
/*----------------------------------------------------------------------------*/
#define GPIOTAG "[GPIO] "
#define GPIODBG(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)
#define GPIOERR(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)
#define GPIOVER(fmt, arg...)    printf(GPIOTAG "%s: " fmt, __FUNCTION__ ,##arg)

#define GPIO_WR32(addr, data)   DRV_WriteReg32(addr,data)
#define GPIO_RD32(addr)         DRV_Reg32(addr)

#define ADDR_BIT 0
#define VAL_BIT  1
#define MASK_BIT 2




void mt_gpio_set_default_chip(void)
{
    u32 idx;
    u32 val;
	//u32 *p;
	u32 mask;
	unsigned int g_fb_base;
    GPIO_REGS *pReg = (GPIO_REGS*)(GPIO_BASE);

	GPIOVER("fwq .... gpio.................................. 6797 real 0805\n"); 
#ifdef FPGA_SIMULATION
	//for debug
	GPIOVER("gpio debug base =%x\n", GPIO_RD32(GPIO_BASE));
	memset(GPIO_BASE,0 ,4096);
	//end
#endif

	unsigned pin = 0;
	for(pin = 0; pin < MT_GPIO_BASE_MAX; pin++)
	{
		/*GPIOVER("GPIO %d dump\n",pin); */
		/* set GPIOx_MODE*/
		mt_set_gpio_mode(0x80000000+pin ,gpio_array[pin].mode);

		/* set GPIOx_DIR*/
		mt_set_gpio_dir(0x80000000+pin ,gpio_array[pin].dir);

		/* set GPIOx_PULL*/
		mt_set_gpio_pull_select(0x80000000+pin ,gpio_array[pin].pull);	

		/* set GPIOx_PULLEN*/
		mt_set_gpio_pull_enable(0x80000000+pin ,gpio_array[pin].pullen);
		
		/* set GPIOx_DATAOUT*/
		mt_set_gpio_out(0x80000000+pin ,gpio_array[pin].dataout);
		
		/* set GPIO0_SMT */
		mt_set_gpio_smt(0x80000000+pin ,gpio_array[pin].smt);
		

		//GPIOVER("smt1-init %d\n",pin); 
	}
#ifdef SELF_TEST
    mt_gpio_self_test();
#endif
}

void mt_gpio_set_driving(void)
{
#if 0
    u32 val;
    u32 mask;

    /* [MT6571] for MD BSI request */
    mask = 0x3000;
    val = GPIO_RD32(IO_CFG_T_BASE+0xF0);
    val &= ~(mask);
    val |= (0x0000 & mask);
    GPIO_WR32(IO_CFG_T_BASE+0xF0, val);
    /* [MT6571] end */

    /* [MT6571] for desense request (AUD IO) */
    mask = 0x30;
    val = GPIO_RD32(IO_CFG_L_BASE+0xA0);
    val &= ~(mask);
    val |= (0x00 & mask);
    GPIO_WR32(IO_CFG_L_BASE+0xA0, val);
    /* [MT6571] end */
#endif
}

/*
void mt_gpio_set_power(void)
{
#if 0
    u32 val;

    val = GPIO_RD32(GPIO_BASE+0x510);
    
    if(((val & 0x100)>>8) == 1){
	GPIO_WR32(IO_CFG_L_BASE+0x48, 0x8000);
    }
    if(((val & 0x200)>>9) == 1){
	GPIO_WR32(IO_CFG_L_BASE+0x48, 0x10000);
    }
#endif
}
*/

void mt_gpio_set_power(u8 mc1_power,u8 mc2_power, u8 sim1_power, u8 sim2_power)
{
	
	u32 reg=0;
	if (mc1_power == GPIO_VIO28) {

		reg = GPIO_RD32(GPIO_BASE+0x0c28);
		reg |= 0xc30c000;
		GPIO_WR32(GPIO_BASE+0x0c28, reg);
		 
 	} 
	else
	{
		reg = GPIO_RD32(GPIO_BASE+0x0c28);
		reg &= ~(0x3ffff<<12);
		GPIO_WR32(GPIO_BASE+0x0c28, reg);
	}
	//MC2
	if (mc2_power == GPIO_VIO28) {

		reg = GPIO_RD32(GPIO_BASE+0x0828);
		reg |= 0xc30c0;
		GPIO_WR32(GPIO_BASE+0x0828, reg);
 	}
	else
	{
		reg = GPIO_RD32(GPIO_BASE+0x0828);
		reg &= ~(0x3ffff<<4);
		GPIO_WR32(GPIO_BASE+0x0828, reg);
	}
	//sim1
	if (sim1_power == GPIO_VIO28) {

		reg = GPIO_RD32(GPIO_BASE+0x0c28);
		reg |= (0x6000);
		GPIO_WR32(GPIO_BASE+0x0c28, reg);
 	}
	else
	{
	    reg = GPIO_RD32(GPIO_BASE+0x0c28);
		reg &= ~(0x3f<<6);
		GPIO_WR32(GPIO_BASE+0x0c28, reg);
	}
    //sim2
	if (sim2_power == GPIO_VIO28) {

		reg = GPIO_RD32(GPIO_BASE+0x0c28);
		reg |= 0x0c;
		GPIO_WR32(GPIO_BASE+0x0c28, reg);
 	}
	else
	{
	    reg = GPIO_RD32(GPIO_BASE+0x0c28);
		reg &= ~(0x3f);
		GPIO_WR32(GPIO_BASE+0x0c28, reg);
	}
	
}	


void mt_gpio_set_avoid_leakage(void)
{
#if 0
#ifdef MTK_EMMC_SUPPORT
    GPIO_WR32(IO_CFG_B_BASE+0x58, 0x220000);
#endif
#endif
}

void mt_gpio_set_default(void)
{
	//mt_gpio_set_avoid_leakage();
	mt_gpio_set_default_chip();
	//mt_gpio_set_driving();
//	mt_gpio_set_power(GPIO_DVDD28_MC1,GPIO_DVDD28_MC2,GPIO_DVDD28_SIM1,GPIO_DVDD28_SIM2);
	//mutex_init(&gpio_mutex);
	return;
}

#ifdef SELF_TEST
int smt_test (void)
{

    int i, val;
	s32 out;
	int res;
	GPIOVER("smt_test test+++++\n");
	
	for (i = 0; i < MT_GPIO_EXT_MAX; i++)
	{
		/*prepare test */
		res = mt_set_gpio_mode(i|0x80000000, 0);
		if(res)
			return -1;
		/*test*/
		for (val = 0; val < GPIO_SMT_MAX; val++) {
			GPIOVER("test gpio[%d],smt[%d]\n", i, val);
			if(-1 == mt_set_gpio_smt(i|0x80000000,val)) {
				GPIOERR(" set smt unsupport\n" );
				continue;
			}
			if ((res = mt_set_gpio_smt(i|0x80000000,val)) != RSUCCESS) {
				GPIOERR(" set smt[%d] fail: %d\n", val, res);
				return -1;
			} 
			if ( val != mt_get_gpio_smt(i|0x80000000) ) {
				GPIOERR(" get smt[%d] fail: real get %d\n", val, mt_get_gpio_smt(i|0x80000000));
				return -1;
			}
			if (mt_get_gpio_smt(i|0x80000000) > 1) {
				GPIOERR(" get smt[%d] value fail: real get %d\n", val, mt_get_gpio_smt(i|0x80000000));
				return -1;
			}
		}
		
	}
	GPIOVER("smt_test test----- PASS!\n");
	return 0;
			
}


int output_test (void)
{

    int i, val;
	s32 out;
	int res;
	GPIOVER("output test+++++\n");
	
	for (i = 0; i < MT_GPIO_EXT_MAX; i++)
	{
		/*prepare test */
		res = mt_set_gpio_mode(i|0x80000000, 0);
		if(res)
			return -1;
		res = mt_set_gpio_dir(i|0x80000000,GPIO_DIR_OUT);
		if(res)
			return -1;
		/*test*/
		for (val = 0; val < GPIO_OUT_MAX; val++) {
			GPIOVER("test gpio[%d],output[%d]\n", i, val);
			if ((res = mt_set_gpio_out(i|0x80000000,val)) != RSUCCESS) {
				GPIOERR(" set out[%d] fail: %d\n", val, res);
				return -1;
			} 
			if ( val != mt_get_gpio_out(i|0x80000000) ) {
				GPIOERR(" get out[%d] fail: real get %d\n", val, mt_get_gpio_out(i|0x80000000));
				return -1;
			}
			if (mt_get_gpio_out(i|0x80000000) > 1) {
				GPIOERR(" get out[%d] value fail: real get %d\n", val, mt_get_gpio_out(i|0x80000000));
				return -1;
			}
		}
		
	}
	GPIOVER("output test----- PASS!\n");
	return 0;
			
}

int direction_test(void)
{
	int i, val;
	s32 out;
	int res;
	GPIOVER("direction_test test+++++\n");
	for (i = 0; i < MT_GPIO_EXT_MAX; i++)
	{
		/*prepare test */
		res = mt_set_gpio_mode(i|0x80000000, 0);
		if(res)
			return -1;
		
		/*test*/
		for (val = 0; val < GPIO_DIR_MAX; val++) {
			GPIOVER("test gpio[%d],direction[%d]\n", i, val);
			if ((res = mt_set_gpio_dir(i|0x80000000,val)) != RSUCCESS) {
				GPIOERR(" set direction[%d] fail: %d\n", val, res);
				return -1;
			}
			if ( val != mt_get_gpio_dir(i|0x80000000) ) {
				GPIOERR(" get direction[%d] fail1 real get %d\n", val, mt_get_gpio_dir(i|0x80000000));
				return -1;
			}
			if ( mt_get_gpio_dir(i|0x80000000) > 1) {
				GPIOERR(" get direction[%d] value fail2 real get %d\n", val, mt_get_gpio_dir(i|0x80000000));
				return -1;
			}
		}
		
	}
	GPIOVER("direction_test----- PASS!\n");

	return 0;
}

int mode_test(void)
{
	int i, val;
	s32 out;
	int res;
	GPIOVER("mode_test test+++++\n");
	for (i = 0; i < MT_GPIO_EXT_MAX; i++)
	{
		
		
		/*test*/
		for (val = 0; val < GPIO_MODE_MAX; val++) {
			GPIOVER("test gpio[%d],dir[%d]\n", i, val);
			if ((res = mt_set_gpio_mode(i|0x80000000,val)) != RSUCCESS) {
				GPIOERR(" set mode[%d] fail: %d\n", val, res);
				return -1;
			} 
			if ( val != mt_get_gpio_mode(i|0x80000000) ) {
				GPIOERR(" get mode[%d] fail: real get %d\n", val, mt_get_gpio_mode(i|0x80000000));
				return -1;
			}
			if ( mt_get_gpio_mode(i|0x80000000) > 7) {
				GPIOERR(" get mode[%d] value fail: real get %d\n", val, mt_get_gpio_mode(i|0x80000000));
				return -1;
			}
		}
		
	}
	GPIOVER("mode_test----- PASS!\n");

	return 0;
}


int pullen_test(void)
{
	int i, val;
	s32 out;
	int res;
	GPIOVER("pullen_test  +++++\n");
	for (i = 0; i < MT_GPIO_EXT_MAX; i++)
	{
		/*prepare test */
		res = mt_set_gpio_mode(i|0x80000000, 0);
		if(res)
			return -1;
		
		/*test*/
		for (val = 0; val < GPIO_PULL_EN_MAX; val++) {
			GPIOVER("test gpio[%d],pullen[%d]\n", i, val);
			if(-1 == mt_set_gpio_pull_enable(i|0x80000000,val)) {
				GPIOERR(" set pull_enable unsupport\n" );
				continue;
			}
			if(GPIO_NOPULLDOWN == mt_set_gpio_pull_enable(i|0x80000000,val)) {
				GPIOERR(" set pull_down unsupport\n" );
				continue;
			}
			if(GPIO_NOPULLUP== mt_set_gpio_pull_enable(i|0x80000000,val)) {
				GPIOERR(" set pull_up unsupport\n" );
				continue;
			}			
			if ((res = mt_set_gpio_pull_enable(i|0x80000000,val)) != RSUCCESS) {
				GPIOERR(" set pull_enable[%d] fail1 %d\n", val, res);
				return -1;
			} 
			if ( val != mt_get_gpio_pull_enable(i|0x80000000) ) {
				GPIOERR(" get pull_enable[%d] fail2 real get %d\n", val, mt_get_gpio_pull_enable(i|0x80000000));
				return -1;
			}
			
			if ( mt_get_gpio_pull_enable(i|0x80000000) > 1) {
				GPIOERR(" get pull_enable[%d] value fail3: real get %d\n", val, mt_get_gpio_pull_enable(i|0x80000000));
				return -1;
			}
		}
		
	}
	GPIOVER("pullen_test----- PASS!\n");

	return 0;
}

int pullselect_test(void)
{
	int i, val;
	s32 out;
	int res;
	GPIOVER("pullselect_test  +++++\n");
	for (i = 0; i < MT_GPIO_EXT_MAX; i++)
	{
		/*prepare test */
		res = mt_set_gpio_mode(i|0x80000000, 0 );
		if(res)
			return -1;
		
		/*test*/
		for (val = 0; val < GPIO_PULL_MAX; val++) {
			GPIOVER("test gpio[%d],pull_select[%d]\n", i, val);
			res =mt_set_gpio_pull_select(i|0x80000000,val);
			if (GPIO_PULL_UNSUPPORTED == res 
				|| GPIO_NOPULLUP == res
				|| GPIO_NOPULLDOWN ==res) {
				GPIOERR(" set gpio[%d] pull_select[%d] unsupport\n",i,val);
				continue;
			}
			
			if ((res = mt_set_gpio_pull_select(i|0x80000000,val)) != RSUCCESS) {
				GPIOERR(" set pull_select[%d] fail1: %d\n", val, res);
				return -1;
			} 
			if ( val != mt_get_gpio_pull_select(i|0x80000000) ) {
				GPIOERR(" get pull_select[%d] fail2: real get %d\n", val, mt_get_gpio_pull_select(i|0x80000000));
				return -1;
			}
			if(-1 == mt_get_gpio_pull_select(i|0x80000000)) {
				GPIOERR(" set gpio[%d] pull_select not support\n",i);
			}
			else if (mt_get_gpio_pull_select(i|0x80000000) > 2) {
				GPIOERR(" get pull_select[%d] value fail: real get %d\n", val, mt_get_gpio_pull_select(i|0x80000000));
				return -1;
			}
		}
		
	}
	GPIOVER("pullselect_test----- PASS!\n");

	return 0;
}



void mt_gpio_self_test(void)
{
    int err=0;
    GPIOVER("GPIO self_test start\n");
	err = mode_test();
	if(err) {
		GPIOVER("GPIO self_test FAIL\n");
		return;
	}

	err = direction_test();
	if(err) {
		GPIOVER("GPIO self_test FAIL\n");
		return;
	}

	err = output_test();
	if(err) {
		GPIOVER("GPIO self_test FAIL\n");
		return;
	}

	err = smt_test();
	if(err) {
		GPIOVER("GPIO self_test FAIL\n");
		return;
	}

	err = pullen_test();
	if(err) {
		GPIOVER("GPIO self_test FAIL\n");
		return;
	}

	err = pullselect_test();
	if(err) {
		GPIOVER("GPIO self_test FAIL\n");
		return;
	}
	
    GPIOVER("GPIO self_test PASS\n");
}

#endif

#endif // end  #if (!defined(CFG_FPGA_PLATFORM) && defined (FPGA_SIMULATION))


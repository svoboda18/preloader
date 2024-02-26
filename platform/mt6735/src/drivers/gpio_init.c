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

#if !(CFG_FPGA_PLATFORM)
#include <cust_power.h>
#include <cust_gpio_boot.h>
#endif

#include <platform.h>

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
/*----------------------------------------------------------------------------*/
#if (CFG_FPGA_PLATFORM)
void mt_gpio_set_default(void)
{
	return;
}
void mt_gpio_set_default_dump(void)
{
    return;
}


#else

#include <gpio_init.h>
#if defined(GPIO_INIT_DEBUG)
static GPIO_REGS saved;
#endif

#define GPIO_BASE (0x10211000)

void gpio_dump(void)
{
	int i=0;
	int idx=0;
	u32 val=0;
    GPIODBG("fwq .... gpio dct config ++++++++++++++++++++++++++++\n"); 
/*
	for (idx = 0; idx < sizeof(gpio_init_dir_data)/(sizeof(UINT32)); idx++){
	    val = gpio_init_dir_data[idx];
		GPIOVER("gpio_init_dir_reg[%d],[0x%x]\n",idx,GPIO_RD32(GPIO_BASE+16*idx));
        
    }
*/
	for(i=0;i<204;i++)
	{
	   //GPIOVER(" \n"); 
	   GPIODBG("g[%d]\n",i); 
	   GPIODBG("g[%d], mode(%x)\n",i,mt_get_gpio_mode(0x80000000+i));
	   GPIODBG("g[%d], dir(%x)\n",i,mt_get_gpio_dir(0x80000000+i));
	   GPIODBG("g[%d], pull_en(%x)\n",i,mt_get_gpio_pull_enable(0x80000000+i));
	   GPIODBG("g[%d], pull_sel(%x)\n",i,mt_get_gpio_pull_select(0x80000000+i));
	   GPIODBG("g[%d], out(%x)\n",i,mt_get_gpio_out(0x80000000+i));
	   GPIODBG("g[%d], smt(%x)\n",i,mt_get_gpio_smt(0x80000000+i));
	  // GPIOVER("gpio[%d], ies(%x)\n",i,mt_get_gpio_ies(0x80000000+i));
	  // GPIOVER("gpio[%d], in(%x)\n",i,mt_get_gpio_in(0x80000000+i));
	   
	  
	}

	 GPIODBG("fwq .... gpio dct config ----------------------------\n"); 
}

void mt_gpio_set_default_dump(void)
{
      gpio_dump();
}

void mt_gpio_set_default_chip(void)
{
    u32 idx;
    u32 val;
    u32 mask;

   // GPIO_REGS *pReg = (GPIO_REGS*)(GPIO_BASE);
   
   

	//GPIOVER("fwq .... gpio.................................. 6735 real ooooo\n"); 
	#ifdef SIMULATION
	//for debug
	memset(GPIO_BASE,0 ,4096);
	//end
    #endif
	
	unsigned pin = 0;
	for(pin = 0; pin < MT_GPIO_BASE_MAX; pin++)
	{
		/* set GPIOx_MODE*/
		mt_set_gpio_mode(0x80000000+pin ,gpio_array[pin].mode);

		/* set GPIOx_DIR*/
		mt_set_gpio_dir(0x80000000+pin ,gpio_array[pin].dir);
		//GPIOVER("fwq2..........\n");

		/* set GPIOx_PULLEN*/
		mt_set_gpio_pull_enable(0x80000000+pin ,gpio_array[pin].pullen);
		//GPIOVER("fwq3..........\n"); 
		
		/* set GPIOx_PULL*/
		mt_set_gpio_pull_select(0x80000000+pin ,gpio_array[pin].pull);	
		
		/* set GPIOx_DATAOUT*/
		mt_set_gpio_out(0x80000000+pin ,gpio_array[pin].dataout);
		
		//GPIOVER("fwq5..........\n"); 
		/* set GPIO0_SMT */
		mt_set_gpio_smt(0x80000000+pin ,gpio_array[pin].smt);
		

		//GPIOVER("smt1-init %d\n",pin); 
	}
	//gpio_dump();
	
    //GPIOVER("mt_gpio_set_default() done\n");        
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
	mt_gpio_set_power(GPIO_DVDD28_MC1,GPIO_DVDD28_MC2,GPIO_DVDD28_SIM1,GPIO_DVDD28_SIM2);
	//mt_gpio_set_power();
	return;
}
/*----------------------------------------------------------------------------*/
//EXPORT_SYMBOL(mt_gpio_set_default);
/*----------------------------------------------------------------------------*/
#if 0
void mt_gpio_checkpoint_save(void)
{
#if defined(GPIO_INIT_DEBUG)    
    GPIO_REGS *pReg = (GPIO_REGS*)(GPIO_BASE);
    GPIO_REGS *cur = &saved;
    int idx;
    
    memset(cur, 0x00, sizeof(*cur));
    for (idx = 0; idx < sizeof(pReg->dir)/sizeof(pReg->dir[0]); idx++)
        cur->dir[idx].val = GPIO_RD32(&pReg->dir[idx]);
    for (idx = 0; idx < sizeof(pReg->pullen)/sizeof(pReg->pullen[0]); idx++)
        cur->pullen[idx].val = GPIO_RD32(&pReg->pullen[idx]);
    for (idx = 0; idx < sizeof(pReg->pullsel)/sizeof(pReg->pullsel[0]); idx++)
        cur->pullsel[idx].val =GPIO_RD32(&pReg->pullsel[idx]);
/*    for (idx = 0; idx < sizeof(pReg->dinv)/sizeof(pReg->dinv[0]); idx++)
        cur->dinv[idx].val =GPIO_RD32(&pReg->dinv[idx]);*/
    for (idx = 0; idx < sizeof(pReg->dout)/sizeof(pReg->dout[0]); idx++)
        cur->dout[idx].val = GPIO_RD32(&pReg->dout[idx]);
    for (idx = 0; idx < sizeof(pReg->mode)/sizeof(pReg->mode[0]); idx++)
        cur->mode[idx].val = GPIO_RD32(&pReg->mode[idx]);    
#endif     
}
/*----------------------------------------------------------------------------*/
EXPORT_SYMBOL(mt_gpio_checkpoint_save);
/*----------------------------------------------------------------------------*/
void mt_gpio_dump_diff(GPIO_REGS* pre, GPIO_REGS* cur)
{
#if defined(GPIO_INIT_DEBUG)        
    GPIO_REGS *pReg = (GPIO_REGS*)(GPIO_BASE);
    int idx;
    unsigned char* p = (unsigned char*)pre;
    unsigned char* q = (unsigned char*)cur;
    
    GPIOVER("------ dumping difference between %p and %p ------\n", pre, cur);
    for (idx = 0; idx < sizeof(pReg->dir)/sizeof(pReg->dir[0]); idx++) {
        if (pre->dir[idx].val != cur->dir[idx].val)
            GPIOVER("diff: dir[%2d]    : 0x%08X <=> 0x%08X\n", idx, pre->dir[idx].val, cur->dir[idx].val);
    }
    for (idx = 0; idx < sizeof(pReg->pullen)/sizeof(pReg->pullen[0]); idx++) {
        if (pre->pullen[idx].val != cur->pullen[idx].val)
            GPIOVER("diff: pullen[%2d] : 0x%08X <=> 0x%08X\n", idx, pre->pullen[idx].val, cur->pullen[idx].val);
    }
    for (idx = 0; idx < sizeof(pReg->pullsel)/sizeof(pReg->pullsel[0]); idx++) {
        if (pre->pullsel[idx].val != cur->pullsel[idx].val)
            GPIOVER("diff: pullsel[%2d]: 0x%08X <=> 0x%08X\n", idx, pre->pullsel[idx].val, cur->pullsel[idx].val);
    }
    for (idx = 0; idx < sizeof(pReg->dout)/sizeof(pReg->dout[0]); idx++) {
        if (pre->dout[idx].val != cur->dout[idx].val)
            GPIOVER("diff: dout[%2d]   : 0x%08X <=> 0x%08X\n", idx, pre->dout[idx].val, cur->dout[idx].val);
    }
    for (idx = 0; idx < sizeof(pReg->mode)/sizeof(pReg->mode[0]); idx++) {
        if (pre->mode[idx].val != cur->mode[idx].val)
            GPIOVER("diff: mode[%2d]   : 0x%08X <=> 0x%08X\n", idx, pre->mode[idx].val, cur->mode[idx].val);
    }
    
    for (idx = 0; idx < sizeof(*pre); idx++) {
        if (p[idx] != q[idx])
            GPIOVER("diff: raw[%2d]: 0x%02X <=> 0x%02X\n", idx, p[idx], q[idx]);
    }
    GPIOVER("memcmp(%p, %p, %d) = %d\n", p, q, sizeof(*pre), memcmp(p, q, sizeof(*pre)));
    GPIOVER("------ dumping difference end --------------------------------\n");
#endif 
}
/*----------------------------------------------------------------------------*/
void mt_gpio_checkpoint_compare(void)
{
#if defined(GPIO_INIT_DEBUG)        
    GPIO_REGS *pReg = (GPIO_REGS*)(GPIO_BASE);
    GPIO_REGS latest;
    GPIO_REGS *cur = &latest;
    int idx;
    
    memset(cur, 0x00, sizeof(*cur));
    for (idx = 0; idx < sizeof(pReg->dir)/sizeof(pReg->dir[0]); idx++)
        cur->dir[idx].val = GPIO_RD32(&pReg->dir[idx]);
    for (idx = 0; idx < sizeof(pReg->pullen)/sizeof(pReg->pullen[0]); idx++)
        cur->pullen[idx].val = GPIO_RD32(&pReg->pullen[idx]);
    for (idx = 0; idx < sizeof(pReg->pullsel)/sizeof(pReg->pullsel[0]); idx++)
        cur->pullsel[idx].val =GPIO_RD32(&pReg->pullsel[idx]);
/*    for (idx = 0; idx < sizeof(pReg->dinv)/sizeof(pReg->dinv[0]); idx++)
        cur->dinv[idx].val =GPIO_RD32(&pReg->dinv[idx]);*/
    for (idx = 0; idx < sizeof(pReg->dout)/sizeof(pReg->dout[0]); idx++)
        cur->dout[idx].val = GPIO_RD32(&pReg->dout[idx]);
    for (idx = 0; idx < sizeof(pReg->mode)/sizeof(pReg->mode[0]); idx++)
        cur->mode[idx].val = GPIO_RD32(&pReg->mode[idx]);    
 
    //mt_gpio_dump_diff(&latest, &saved);
    //GPIODBG("memcmp(%p, %p, %d) = %d\n", &latest, &saved, sizeof(GPIO_REGS), memcmp(&latest, &saved, sizeof(GPIO_REGS)));
    if (memcmp(&latest, &saved, sizeof(GPIO_REGS))) {
        GPIODBG("checkpoint compare fail!!\n");
        GPIODBG("dump checkpoint....\n");
        //mt_gpio_dump(&saved);
        GPIODBG("\n\n");
        GPIODBG("dump current state\n");
        //mt_gpio_dump(&latest);
        GPIODBG("\n\n");
        mt_gpio_dump_diff(&saved, &latest);        
        //WARN_ON(1);
    } else {
        GPIODBG("checkpoint compare success!!\n");
    }
#endif    
}
/*----------------------------------------------------------------------------*/
EXPORT_SYMBOL(mt_gpio_checkpoint_compare);
/*----------------------------------------------------------------------------*/
#endif
#endif

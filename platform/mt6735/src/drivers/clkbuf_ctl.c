/**
* @file    mt_clk_buf_ctl.c
* @brief   Driver for RF clock buffer control
*
*/

#define __MT_CLK_BUF_CTL_C__

/*=============================================================*/
// Include files
/*=============================================================*/

// system includes
#if 0

//XXX: only in kernel
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/string.h>

#include <mach/mt_spm.h>
#include <mach/mt_clkmgr.h>
#include <mach/mt_spm_sleep.h>
#include <mach/mt_gpio.h>
#include <mach/mt_gpio_core.h>
#include <mach/mt_clkbuf_ctl.h>

#include <linux/xlog.h>

//#define CLK_BUF_SPM_BSI
#define CLK_BUF_TAG     "Power/swap"

#define clk_buf_err(fmt, args...)       \
    xlog_printk(ANDROID_LOG_ERROR, CLK_BUF_TAG, fmt, ##args)
#define clk_buf_warn(fmt, args...)      \
    xlog_printk(ANDROID_LOG_WARN, CLK_BUF_TAG, fmt, ##args)
#define clk_buf_info(fmt, args...)      \
    xlog_printk(ANDROID_LOG_INFO, CLK_BUF_TAG, fmt, ##args)
#define clk_buf_dbg(fmt, args...)       \
    xlog_printk(ANDROID_LOG_DEBUG, CLK_BUF_TAG, fmt, ##args)
#define clk_buf_ver(fmt, args...)       \
    xlog_printk(ANDROID_LOG_VERBOSE, CLK_BUF_TAG, fmt, ##args)
#endif

#include <platform.h>
#include <spm.h>
#include <spm_mtcmos.h>
#include <spm_mtcmos_internal.h>
#include <timer.h>  //udelay
#include <clkbuf_ctl.h>

//
// LOCK
//
// system includes
#if 0
//XXX: only in kernel

DEFINE_MUTEX(clk_buf_ctrl_lock);

#define DEFINE_ATTR_RO(_name)			\
static struct kobj_attribute _name##_attr = {	\
	.attr	= {				\
		.name = #_name,			\
		.mode = 0444,			\
	},					\
	.show	= _name##_show,			\
}

#define DEFINE_ATTR_RW(_name)			\
static struct kobj_attribute _name##_attr = {	\
	.attr	= {				\
		.name = #_name,			\
		.mode = 0644,			\
	},					\
	.show	= _name##_show,			\
	.store	= _name##_store,		\
}

#define __ATTR_OF(_name)	(&_name##_attr.attr)
#endif
#if 0//K2 FIXME
static CLK_BUF_SWCTRL_STATUS_T  clk_buf_swctrl[CLKBUF_NUM]={
	CLK_BUF_SW_ENABLE,
	CLK_BUF_SW_DISABLE,
	CLK_BUF_SW_DISABLE,
	CLK_BUF_SW_DISABLE
};
#else
static CLK_BUF_SWCTRL_STATUS_T  clk_buf_swctrl[CLKBUF_NUM]={
	CLK_BUF_SW_ENABLE,
	CLK_BUF_SW_ENABLE,
	CLK_BUF_SW_ENABLE,
	CLK_BUF_SW_ENABLE
};
#endif
// system includes
/*
//XXX: only in kernel

extern struct kobject *power_kobj;
*/
/*===============================================================================*/
/*                Manual Config BSI Waveform API                                 */
/*===============================================================================*/
#define BSI_CW_CNT           30  //bits w DATA/READ bit
#define BSI_CLK_HALF_PERIOD  1   //us
#define BSI_CLK_PERIOD       (BSI_CLK_HALF_PERIOD<<1)   //us

#define BSI_DATA_BIT         0   //0:not tx data 1:tx data
#define BSI_READ_BIT         0   //0:write       1:read
/*--------------------------------------------------------------*/
/* Co-Clock Buffuer Setting Can configure HERE!!!               */
/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
#define BSI_CLK_MASK         0x7

#define BSI_CW_ADDR          252
#define BSI_CW_DEFAULT       0x01E8F


#define SET_GPIO_DOUT(cs, clk, d0, d1, d2)     \
{	mt_set_gpio_out(GPIO_RFIC0_BSI_CS,  cs);   \
	mt_set_gpio_out(GPIO_RFIC0_BSI_D0,  d0);   \
	mt_set_gpio_out(GPIO_RFIC0_BSI_D1,  d1);   \
	mt_set_gpio_out(GPIO_RFIC0_BSI_D2,  d2);   \
	mt_set_gpio_out(GPIO_RFIC0_BSI_CK,	clk);	\
}

#ifdef CLK_BUF_SPM_BSI
#define CLK_BUF_BSI_PAD_NUM 5
static unsigned int clk_buf_spm_cfg[CLK_BUF_BSI_PAD_NUM] = 
{
    0x00000000, //BSI_EN_SR:
    0x00000000, //BSI_CLK_SR:
    0x00000000, //BSI_D0_SR:
    0x00000000, //BSI_D1_SR:
    0x00000000, //BSI_D2_SR:
};
static unsigned int clk_buf_bit_ctrl = 0;
#endif

static unsigned int clk_buf_CW(CLK_BUF_SWCTRL_STATUS_T *status)
{
	unsigned int bsi_clk_setting,bsi_cw_data;
	bsi_clk_setting = ((status[3]<<2)|(status[2]<<1)|(status[1]<<0));
	bsi_cw_data = ( (BSI_CW_DEFAULT&(~(BSI_CLK_MASK<<4))) | ((bsi_clk_setting&BSI_CLK_MASK)<<4) );
	return (((BSI_DATA_BIT&0x1)<<29)| ((BSI_READ_BIT&0x1)<<28)| ((BSI_CW_ADDR &0xFF)<<20)| ((bsi_cw_data &0xFFFFF)<<0 ));
	
}


static void clk_buf_Send_BSI_CW(CLK_BUF_SWCTRL_STATUS_T *status)
{
	int  i;
	unsigned int cw;
#ifndef CLK_BUF_SPM_BSI		
	GPIO_OUT d0,d1,d2;
#else
	bool d0,d1,d2;
#endif

#if 1//remove for switch to MD part
	//config RFICx as GPIO
	mt_set_gpio_mode(GPIO_RFIC0_BSI_CK,  GPIO_MODE_GPIO); 
	mt_set_gpio_mode(GPIO_RFIC0_BSI_D0,  GPIO_MODE_GPIO);
	mt_set_gpio_mode(GPIO_RFIC0_BSI_D1,  GPIO_MODE_GPIO);
	mt_set_gpio_mode(GPIO_RFIC0_BSI_D2,  GPIO_MODE_GPIO);
	mt_set_gpio_mode(GPIO_RFIC0_BSI_CS,  GPIO_MODE_GPIO);
#endif

	//Pre-Set DOUT as LOW
	SET_GPIO_DOUT( GPIO_OUT_ZERO,	GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO ); /* input: GPIO_DOUT, CS, CLK, D0, D1, D2 */
	//must confirm DOUT default output down
	mt_set_gpio_dir(GPIO_RFIC0_BSI_CK,	GPIO_DIR_OUT);
	mt_set_gpio_dir(GPIO_RFIC0_BSI_D0,  GPIO_DIR_OUT);
	mt_set_gpio_dir(GPIO_RFIC0_BSI_D1,  GPIO_DIR_OUT);
	mt_set_gpio_dir(GPIO_RFIC0_BSI_D2,  GPIO_DIR_OUT);
	mt_set_gpio_dir(GPIO_RFIC0_BSI_CS,  GPIO_DIR_OUT);
#ifndef CLK_BUF_SPM_BSI	
	//Start to Generate BSI Wave Form
	SET_GPIO_DOUT( GPIO_OUT_ZERO,  GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO ); /* input: GPIO_DOUT, CS, CLK, D0, D1, D2 */
	SET_GPIO_DOUT( GPIO_OUT_ONE, GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO ); /* input: GPIO_DOUT, CS, CLK, D0, D1, D2 */
	udelay(BSI_CLK_PERIOD);//delay 1T 26MHz
#else
	//Start to Generate BSI Wave Form
	clk_buf_spm_cfg[BSI_EN_SR]=0x20000000;
	clk_buf_spm_cfg[BSI_CLK_SR]=0x00000000;
	clk_buf_spm_cfg[BSI_D0_SR]=0x00000000;
	clk_buf_spm_cfg[BSI_D1_SR]=0x00000000;
	clk_buf_spm_cfg[BSI_D2_SR]=0x00000000;
	clk_buf_bit_ctrl=29;
#endif

	cw = clk_buf_CW(status);

	for( i=(BSI_CW_CNT-1); i>=0; i=i-3 )
	{
#ifndef CLK_BUF_SPM_BSI	
		d0 = ((cw>>(i-2))&0x1) ? GPIO_OUT_ONE : GPIO_OUT_ZERO;
		d1 = ((cw>>(i-1))&0x1) ? GPIO_OUT_ONE : GPIO_OUT_ZERO;
		d2 = ((cw>>(i-0))&0x1) ? GPIO_OUT_ONE : GPIO_OUT_ZERO;
		SET_GPIO_DOUT( GPIO_OUT_ONE, GPIO_OUT_ZERO,  d0, d1, d2 );
		udelay(BSI_CLK_HALF_PERIOD);//delay 1T 26MHz
		SET_GPIO_DOUT( GPIO_OUT_ONE, GPIO_OUT_ONE, d0, d1, d2 );
		udelay(BSI_CLK_HALF_PERIOD);//delay 1T 26MHz
#else
		d0 = ((cw>>(i-2))&0x1) ? 1 : 0;
		d1 = ((cw>>(i-1))&0x1) ? 1 : 0;
		d2 = ((cw>>(i-0))&0x1) ? 1 : 0;
		clk_buf_bit_ctrl-=1;
		clk_buf_spm_cfg[BSI_EN_SR]|=(1<<clk_buf_bit_ctrl);
		clk_buf_spm_cfg[BSI_CLK_SR]&=~(1<<clk_buf_bit_ctrl);
		clk_buf_spm_cfg[BSI_D0_SR]|=(d0<<clk_buf_bit_ctrl);
		clk_buf_spm_cfg[BSI_D1_SR]|=(d1<<clk_buf_bit_ctrl);
		clk_buf_spm_cfg[BSI_D2_SR]|=(d2<<clk_buf_bit_ctrl);
		
		clk_buf_bit_ctrl-=1;
		clk_buf_spm_cfg[BSI_EN_SR]|=(1<<clk_buf_bit_ctrl);
		clk_buf_spm_cfg[BSI_CLK_SR]|=(1<<clk_buf_bit_ctrl);
		clk_buf_spm_cfg[BSI_D0_SR]|=(d0<<clk_buf_bit_ctrl);
		clk_buf_spm_cfg[BSI_D1_SR]|=(d1<<clk_buf_bit_ctrl);
		clk_buf_spm_cfg[BSI_D2_SR]|=(d2<<clk_buf_bit_ctrl);
#endif
	}

#ifndef CLK_BUF_SPM_BSI	
	SET_GPIO_DOUT( GPIO_OUT_ONE, GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO ); /* input: GPIO_DOUT, CS, CLK, D0, D1, D2 */
	udelay(BSI_CLK_HALF_PERIOD);//delay 1T 26MHz
	SET_GPIO_DOUT( GPIO_OUT_ZERO,  GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO, GPIO_OUT_ZERO ); /* input: GPIO_DOUT, CS, CLK, D0, D1, D2 */
#else
	clk_buf_bit_ctrl-=1;
	clk_buf_spm_cfg[BSI_EN_SR]|=(1<<clk_buf_bit_ctrl);
	clk_buf_spm_cfg[BSI_CLK_SR]&=~(1<<clk_buf_bit_ctrl);
	clk_buf_spm_cfg[BSI_D0_SR]&=~(1<<clk_buf_bit_ctrl);
	clk_buf_spm_cfg[BSI_D1_SR]&=~(1<<clk_buf_bit_ctrl);
	clk_buf_spm_cfg[BSI_D2_SR]&=~(1<<clk_buf_bit_ctrl);

	clk_buf_bit_ctrl-=1;
	clk_buf_spm_cfg[BSI_EN_SR]&=~(1<<clk_buf_bit_ctrl);
	clk_buf_spm_cfg[BSI_CLK_SR]&=~(1<<clk_buf_bit_ctrl);
	clk_buf_spm_cfg[BSI_D0_SR]&=~(1<<clk_buf_bit_ctrl);
	clk_buf_spm_cfg[BSI_D1_SR]&=~(1<<clk_buf_bit_ctrl);
	clk_buf_spm_cfg[BSI_D2_SR]&=~(1<<clk_buf_bit_ctrl);
	//clk_buf_err("BSI_EN_SR=0x%x, BSI_CLK_SR=0x%x, BSI_D0_SR=0x%x, BSI_D1_SR=0x%x, BSI_D2_SR=0x%x\n",clk_buf_spm_cfg[BSI_EN_SR],clk_buf_spm_cfg[BSI_CLK_SR],clk_buf_spm_cfg[BSI_D0_SR],clk_buf_spm_cfg[BSI_D1_SR],clk_buf_spm_cfg[BSI_D2_SR]);
	spm_ap_bsi_gen(clk_buf_spm_cfg);
#endif
	//End of Generate BSI Wave Form

	//Set GPIO68~72 DIR as DIN
	mt_set_gpio_dir(GPIO_RFIC0_BSI_CK,	GPIO_DIR_IN);
	mt_set_gpio_dir(GPIO_RFIC0_BSI_D0,  GPIO_DIR_IN);
	mt_set_gpio_dir(GPIO_RFIC0_BSI_D1,  GPIO_DIR_IN);
	mt_set_gpio_dir(GPIO_RFIC0_BSI_D2,  GPIO_DIR_IN);
	mt_set_gpio_dir(GPIO_RFIC0_BSI_CS,  GPIO_DIR_IN);

#if 1//remove for switch to MD part
	//config RFICx as RFIC function
	mt_set_gpio_mode(GPIO_RFIC0_BSI_CK,  GPIO_MODE_01); 
	mt_set_gpio_mode(GPIO_RFIC0_BSI_D0,  GPIO_MODE_01);
	mt_set_gpio_mode(GPIO_RFIC0_BSI_D1,  GPIO_MODE_01);
	mt_set_gpio_mode(GPIO_RFIC0_BSI_D2,  GPIO_MODE_01);
	mt_set_gpio_mode(GPIO_RFIC0_BSI_CS,  GPIO_MODE_01);
#endif


}

// system includes
#if 0
//XXX: only in kernel

static void spm_clk_buf_ctrl(CLK_BUF_SWCTRL_STATUS_T *status)
{
#if 0
	u32 spm_val;
	int i;
	
	spm_ap_mdsrc_req(1);

	spm_val = spm_read(SPM_SLEEP_MDBSI_CON)&~0x7;

	for(i=1;i<CLKBUF_NUM;i++)
		spm_val |=status[i]<<(i-1);
	
	spm_write(SPM_SLEEP_MDBSI_CON,spm_val);

	udelay(2);

	spm_ap_mdsrc_req(0);
#endif
}


bool clk_buf_ctrl(enum clk_buf_id id,bool onoff)
{    
    if( id>=CLK_BUF_INVALID )//TODO, need check DCT tool for CLK BUF SW control
        return false;  
	if((id==CLK_BUF_BB_MD)&&(CLK_BUF1_STATUS==CLOCK_BUFFER_HW_CONTROL))
		return false;

	if((id==CLK_BUF_CONN)&&(CLK_BUF2_STATUS==CLOCK_BUFFER_HW_CONTROL))
		return false;	

	if((id==CLK_BUF_NFC)&&(CLK_BUF3_STATUS==CLOCK_BUFFER_HW_CONTROL))
		return false;

	if((id==CLK_BUF_AUDIO)&&(CLK_BUF4_STATUS==CLOCK_BUFFER_HW_CONTROL))
		return false;
		
	mutex_lock(&clk_buf_ctrl_lock);

	clk_buf_swctrl[id]=onoff;
	
    if(subsys_is_on(SYS_MD1))
    {
        spm_clk_buf_ctrl(clk_buf_swctrl);
    }else
    {    	
    	/*TODO, GPIO control for Fight mode API provide API from CS.Wu*/
		clk_buf_Send_BSI_CW(clk_buf_swctrl);

    }
	mutex_unlock(&clk_buf_ctrl_lock);
	return true;
    
}


void clk_buf_get_swctrl_status(CLK_BUF_SWCTRL_STATUS_T *status)
{
	int i;
	for(i=0;i<CLKBUF_NUM;i++)
		status[i] =clk_buf_swctrl[i];
	
}

static ssize_t clk_buf_ctrl_store(struct kobject *kobj, struct kobj_attribute *attr,
				  const char *buf, size_t count)
{
	/*design for BSI wrapper command or by GPIO wavefrom*/
	u32 clk_buf_en[CLKBUF_NUM],i;
	char cmd[32];

	if (sscanf(buf, "%s %x %x %x %x", cmd, &clk_buf_en[0], &clk_buf_en[1], &clk_buf_en[2], &clk_buf_en[3]) != 5)
		return -EPERM;
	
	for(i=0;i<CLKBUF_NUM;i++)
		clk_buf_swctrl[i]=clk_buf_en[i];

	if (!strcmp(cmd, "bsi"))
	{
		spm_clk_buf_ctrl(clk_buf_swctrl);
		return count;
	}

	if (!strcmp(cmd, "gpio"))
	{	
		clk_buf_Send_BSI_CW(clk_buf_swctrl);
		return count;
	}
	return -EINVAL;

}

static ssize_t clk_buf_ctrl_show(struct kobject *kobj, struct kobj_attribute *attr,
				  const char *buf, size_t count)
{
    int len = 0;
    char *p = buf;
    
    p += sprintf(p, "********** clock buffer state **********\n");
	p += sprintf(p, "CKBUF1 SW(1)/HW(2) CTL: %d, Disable(0)/Enable(1): %d\n",CLK_BUF1_STATUS,clk_buf_swctrl[0]);
	p += sprintf(p, "CKBUF2 SW(1)/HW(2) CTL: %d, Disable(0)/Enable(1): %d\n",CLK_BUF2_STATUS,clk_buf_swctrl[1]);
	p += sprintf(p, "CKBUF3 SW(1)/HW(2) CTL: %d, Disable(0)/Enable(1): %d\n",CLK_BUF3_STATUS,clk_buf_swctrl[2]);
	p += sprintf(p, "CKBUF4 SW(1)/HW(2) CTL: %d, Disable(0)/Enable(1): %d\n",CLK_BUF4_STATUS,clk_buf_swctrl[3]);
	p += sprintf(p, "\n********** clock buffer command help **********\n");
	p += sprintf(p, "BSI  switch on/off: echo bsi en1 en2 en3 en4 > /sys/power/clk_buf/clk_buf_ctrl\n");
	p += sprintf(p, "GPIO switch on/off: echo gpio en1 en2 en3 en4 > /sys/power/clk_buf/clk_buf_ctrl\n");
	p += sprintf(p, "BB   :en1\n");
	p += sprintf(p, "6605 :en2\n");
	p += sprintf(p, "5193 :en3\n");
	p += sprintf(p, "AUDIO:en4\n");

    len = p - buf;
    return len;

}



DEFINE_ATTR_RW(clk_buf_ctrl);

static struct attribute *clk_buf_attrs[] = {

	/* for clock buffer control */
	__ATTR_OF(clk_buf_ctrl),

	/* must */
	NULL,
};

static struct attribute_group spm_attr_group = {
	.name	= "clk_buf",
	.attrs	= clk_buf_attrs,
};

static int clk_buf_fs_init(void)
{
	int r;
	//FIXME: K2 early porting
#if defined(CONFIG_PM)
	/* create /sys/power/clk_buf/xxx */
	r = sysfs_create_group(power_kobj, &spm_attr_group);
	if (r)
		clk_buf_err("FAILED TO CREATE /sys/power/clk_buf (%d)\n", r);
	return r;
#endif	
}

bool clk_buf_init(void)
{
	clk_buf_fs_init();
}
#else
void clk_buf_all_on(void)
{
	clk_buf_Send_BSI_CW(clk_buf_swctrl);
}

#endif



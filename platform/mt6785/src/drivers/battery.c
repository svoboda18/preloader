#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>
#include "rtc.h"
#include "pal_log.h"


//==============================================================================
// Battery defines
//==============================================================================
#define UNIT_TIME          (50)
#define PMIC_SHUTDOWN_TIME	30

static int boot_vbat;
static int shutdowntime;
static bool is_fg_init;

//==============================================================================
// Battery functions
//==============================================================================
#if !CFG_FPGA_PLATFORM 
static unsigned int fg_get_data_ready_status(void)
{
	unsigned int ret = 0;
	unsigned int temp_val = 0;

	if (drdi_get_hw_ver() == HW_VER_V0)
		return 0;

	ret = pmic_read_interface(PMIC_FG_LATCHDATA_ST_ADDR, &temp_val, 0xFFFF, 0x0);

	/*pal_log_info("[fg_get_data_ready_status] Reg[0x%x]=0x%x\r\n", PMIC_FG_LATCHDATA_ST_ADDR, temp_val);*/

	temp_val =
	(temp_val & (PMIC_FG_LATCHDATA_ST_MASK << PMIC_FG_LATCHDATA_ST_SHIFT))
	>> PMIC_FG_LATCHDATA_ST_SHIFT;

	return temp_val;
}


int fgauge_polling1(void)
{
	int m = 0;

	if (drdi_get_hw_ver() == HW_VER_V0)
		return 0;

	/*(2)    Keep i2c read when status = 1 (0x06) */
	while (fg_get_data_ready_status() == 0) {
		m++;
		if (m > 1000) {
			pal_log_err(
				 "[fg_get_time] fg_get_data_ready_status timeout 1 !\r\n");
			break;
		}
	}
	return m;
}

int fgauge_polling2(void)
{
	int m = 0;

	if (drdi_get_hw_ver() == HW_VER_V0)
		return 0;

	while (fg_get_data_ready_status() != 0) {
		m++;
		if (m > 1000) {
			pal_log_err(
				 "[fg_get_time] fg_get_data_ready_status timeout 2 !\r\n");
			break;
		}
	}
	return m;
}

signed int fgauge_get_time(void *data)
{
	unsigned int time_29_16 = 0, time_15_00 = 0, ret_time = 0;
	unsigned int ret = 0;
	unsigned long time = 0;
	int p1 = 0, p2 = 0;

	if (drdi_get_hw_ver() == HW_VER_V0)
		return 0;


	ret = pmic_config_interface((U32)(PMIC_FG_SW_READ_PRE_ADDR), (U32)(1),
		(U32)(PMIC_FG_SW_READ_PRE_MASK),
		(U32)(PMIC_FG_SW_READ_PRE_SHIFT));

	p1 = fgauge_polling1();


	/*time_15_00 = pmic_get_register_value(PMIC_FG_NTER_15_00);*/
	ret = pmic_read_interface((U32)(PMIC_FG_NTER_15_00_ADDR), (&time_15_00),
		(U32)(PMIC_FG_NTER_15_00_MASK),
		(U32)(PMIC_FG_NTER_15_00_SHIFT));

	/*time_29_16 = pmic_get_register_value(PMIC_FG_NTER_29_16);*/
	ret = pmic_read_interface((U32)(PMIC_FG_NTER_29_16_ADDR), (&time_29_16),
		(U32)(PMIC_FG_NTER_29_16_MASK),
		(U32)(PMIC_FG_NTER_29_16_SHIFT));

	time = time_15_00;
	time |= time_29_16 << 16;
	time = time * UNIT_TIME / 100;
	ret_time = time;

	ret = pmic_config_interface((U32)(PMIC_FG_SW_CLEAR_ADDR), (U32)(1),
		(U32)(PMIC_FG_SW_CLEAR_MASK),
		(U32)(PMIC_FG_SW_CLEAR_SHIFT));

	ret = pmic_config_interface((U32)(PMIC_FG_SW_READ_PRE_ADDR), (U32)(0),
		(U32)(PMIC_FG_SW_READ_PRE_MASK),
		(U32)(PMIC_FG_SW_READ_PRE_SHIFT));

	p2 = fgauge_polling2();

	ret = pmic_config_interface((U32)(PMIC_FG_SW_CLEAR_ADDR), (U32)(0),
		(U32)(PMIC_FG_SW_CLEAR_MASK),
		(U32)(PMIC_FG_SW_CLEAR_SHIFT));

	pal_log_info(
		 "[fg_get_time] low:0x%x high:0x%x rtime:0x%lx 0x%x p1=%d p2=%d!\r\n",
		 time_15_00, time_29_16, time, ret_time, p1, p2);


	*(unsigned int *) (data) = ret_time;

	return 0;
}

signed int fgauge_get_soff_time(void *data)
{
	unsigned int soff_time_29_16 = 0, soff_time_15_00 = 0, ret_time = 0;
	unsigned int ret = 0;
	unsigned long time = 0;
	int check_soff_valid = 0;

	if (drdi_get_hw_ver() == HW_VER_V0)
		return 0;

	/*soff_time_15_00 = pmic_get_register_value(PMIC_FG_SOFF_TIME_15_00);*/
	ret = pmic_read_interface((U32)(PMIC_FG_SOFF_TIME_15_00_ADDR), (&soff_time_15_00),
		(U32)(PMIC_FG_SOFF_TIME_15_00_MASK),
		(U32)(PMIC_FG_SOFF_TIME_15_00_SHIFT));

		/*soff_time_29_16 = pmic_get_register_value(PMIC_FG_SOFF_TIME_29_16);*/
	ret = pmic_read_interface((U32)(PMIC_FG_SOFF_TIME_29_16_ADDR), (&soff_time_29_16),
		(U32)(PMIC_FG_SOFF_TIME_29_16_MASK),
		(U32)(PMIC_FG_SOFF_TIME_29_16_SHIFT));

	time = soff_time_15_00;
	time |= soff_time_29_16 << 16;
	time = time * UNIT_TIME / 100;
	ret_time = time;

	/* set FG_SOFF_RST = 1 ->dealy->set FG_SOFF_RST = 0,clear FG_SOFF */

	ret = pmic_config_interface((U32)(PMIC_FG_SOFF_RST_ADDR), (U32)(1),
		(U32)(PMIC_FG_SOFF_RST_MASK),
		(U32)(PMIC_FG_SOFF_RST_SHIFT));

	mdelay(1);

	ret = pmic_config_interface((U32)(PMIC_FG_SOFF_RST_ADDR), (U32)(0),
		(U32)(PMIC_FG_SOFF_RST_MASK),
		(U32)(PMIC_FG_SOFF_RST_SHIFT));

	ret = pmic_read_interface((U32)(PMIC_FG_SOFF_ADDR), (&check_soff_valid),
		(U32)(PMIC_FG_SOFF_MASK),
		(U32)(PMIC_FG_SOFF_SHIFT));

	pal_log_info(
		 "[fg_get_soff_time] low:0x%x high:0x%x rtime:0x%lx 0x%x, check_valid:%d\r\n",
		 soff_time_15_00, soff_time_29_16, time, ret_time, check_soff_valid);


	*(unsigned int *) (data) = ret_time;

	return 0;
}

void fuel_gauge_init(void)
{
	int fg_reset_status = 0;
	U32 efuse_cal = 0;
	int fg_curr_time = 0;
	int shutdown_pmic_time = 0;
	int do_init_fgadc_reset = 0;
	int ret = 0;
	int hw_id = 0, sw_id = 0;
	U32 reset_sel = 0;
	unsigned int slp_en = 0;
	int b_moniter_pl_charg_bit = 0;
	int b_monitor_soff_validtime = 0;
	int plcharg_status = 0;
	int fg_soff_valid = 0;
	unsigned int bfgadc_con1 = 0, fgadc_con1 = 0;
	unsigned int rgspare = 0, newrgspare = 0;


	if (drdi_get_hw_ver() == HW_VER_V0)
		return;

	reset_sel = upmu_get_reg_value(PMIC_RG_FGADC_RST_SRC_SEL_ADDR);
	slp_en = upmu_get_reg_value(PMIC_FG_SOFF_SLP_EN_ADDR);

	/*pmic_set_register_value(PMIC_FG_SOFF_SLP_EN, 1);*/
	ret = pmic_config_interface((U32)(PMIC_FG_SOFF_SLP_EN_ADDR), (U32)(1),
		(U32)(PMIC_FG_SOFF_SLP_EN_MASK),
		(U32)(PMIC_FG_SOFF_SLP_EN_SHIFT));

	//only for GM 3.0 : set FGADC reset source selection = 1 ,GM3.0 reset
	ret = pmic_config_interface((U32)(PMIC_RG_FGADC_RST_SRC_SEL_ADDR), (U32)(1),
		(U32)(PMIC_RG_FGADC_RST_SRC_SEL_MASK),
		(U32)(PMIC_RG_FGADC_RST_SRC_SEL_SHIFT));


	/*fg_reset_status = pmic_get_register_value(PMIC_FG_RSTB_STATUS);*/
	ret = pmic_read_interface((U32)(PMIC_FG_RSTB_STATUS_ADDR), (&fg_reset_status),
		(U32)(PMIC_FG_RSTB_STATUS_MASK),
		(U32)(PMIC_FG_RSTB_STATUS_SHIFT));

	/* eFuse */
	/*efuse_cal = pmic_get_register_value(PMIC_RG_FGADC_GAINERR_CAL);*/
	ret = pmic_read_interface((U32)(PMIC_RG_FGADC_GAINERR_CAL_ADDR), (&efuse_cal),
		(U32)(PMIC_RG_FGADC_GAINERR_CAL_MASK),
		(U32)(PMIC_RG_FGADC_GAINERR_CAL_SHIFT));

	/*pmic_set_register_value(PMIC_FG_GAIN, efuse_cal);*/
	if (efuse_cal != 0) {
		ret = pmic_config_interface((U32)(PMIC_FG_GAIN_ADDR), (U32)(efuse_cal),
			(U32)(PMIC_FG_GAIN_MASK),
			(U32)(PMIC_FG_GAIN_SHIFT));
	} else {
		while (1) {
			pal_log_err("[PMIC ERROR]NO EFUSE!! %d\n", efuse_cal);
			mdelay(300);
			platform_wdt_all_kick();

			ret = pmic_read_interface((U32)(PMIC_RG_FGADC_GAINERR_CAL_ADDR), (&efuse_cal),
				(U32)(PMIC_RG_FGADC_GAINERR_CAL_MASK),
				(U32)(PMIC_RG_FGADC_GAINERR_CAL_SHIFT));
			if (efuse_cal != 0)
				break;
		}
	}


	/* fg_soff_valid = pmic_get_register_value(PMIC_FG_SOFF); */
	ret = pmic_read_interface((U32)(PMIC_FG_SOFF_ADDR), (&fg_soff_valid),
		(U32)(PMIC_FG_SOFF_MASK),
		(U32)(PMIC_FG_SOFF_SHIFT));

	ret = pmic_read_interface((U32)(PMIC_RG_SPARE_ADDR), (&rgspare),
		(U32)(PMIC_RG_SPARE_MASK),
		(U32)(PMIC_RG_SPARE_SHIFT));

	newrgspare = rgspare | 0x2;

	/* Set RG_SPARE[1] = 1’b1 */
	ret = pmic_config_interface((U32)(PMIC_RG_SPARE_ADDR), newrgspare,
		(U32)(PMIC_RG_SPARE_MASK),
		(U32)(PMIC_RG_SPARE_SHIFT));

	/* Set FG_RNG_EN_MODE = 1’b1 */
	ret = pmic_config_interface((U32)(PMIC_FG_RNG_EN_MODE_ADDR), (U32)(1),
		(U32)(PMIC_FG_RNG_EN_MODE_MASK),
		(U32)(PMIC_FG_RNG_EN_MODE_SHIFT));


	hw_id = upmu_get_reg_value(MT6359_HWCID);
	sw_id = upmu_get_reg_value(MT6359_SWCID);

	ret = fgauge_get_time(&fg_curr_time);

	if (fg_soff_valid == 1)
		ret = fgauge_get_soff_time(&shutdown_pmic_time);
	else
		shutdown_pmic_time = fg_curr_time;

	pmic_read_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, &b_monitor_soff_validtime, 0x0001, 0x5);
	if (b_monitor_soff_validtime == 1) {
		fg_soff_valid = 1;
		shutdown_pmic_time = fg_curr_time - ((PMIC_SHUTDOWN_TIME * 60) + 2);
		pal_log_err("[fg_init] b_monitor_soff_validtime:%d fg_soff_valid %d, shutdown_pmic_time %d",
			b_monitor_soff_validtime, fg_soff_valid, shutdown_pmic_time);
	}


	if (fg_reset_status == 0)
		do_init_fgadc_reset = 1;
	else if ((fg_soff_valid == 1) && (shutdown_pmic_time != 0) &&
		(fg_curr_time - shutdown_pmic_time > PMIC_SHUTDOWN_TIME * 60)) {
		do_init_fgadc_reset = 1;

		/* using b5 GAUGE_MONITOR_SOFF_VALIDTIME to record status */
		/* prevent already reset adc, but doesn't goto dod init 10 case */
		ret = pmic_config_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, 0x1, 0x0001, 0x5);
	} else
		do_init_fgadc_reset = 0;


	bfgadc_con1 = upmu_get_reg_value(MT6359_FGADC_CON1);
	if (do_init_fgadc_reset == 1) {
		/* reset CON1 */
		ret = pmic_config_interface(MT6359_FGADC_CON1, 0x1f30, 0x1f00, 0x0);
		fgadc_con1 = upmu_get_reg_value(MT6359_FGADC_CON1);

		pal_log_err("BATTERY PRELOADER:ret=%d, bef FGADC_CON1=0x%x ,af:0x%x\n", ret, bfgadc_con1, fgadc_con1);
		mdelay(1);
		ret = pmic_config_interface(MT6359_FGADC_CON1, 0x0030, 0x1f00, 0x0);

		fgadc_con1 = upmu_get_reg_value(MT6359_FGADC_CON1);
		pal_log_err("BATTERY PRELOADER:ret=%d, af FGADC_CON1:0x%x\n", ret, fgadc_con1);
	}

	boot_vbat = get_bat_sense_volt(1);
	shutdowntime = fg_curr_time - shutdown_pmic_time;

	/* if WDT reset from PL to KERNEL, b_moniter_pl_charg_bit will keep 1*/
	/* record PL charging status in bit1, mointer bit in bit2 */
	pmic_read_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, &b_moniter_pl_charg_bit, 0x0001, 0x2);
	if (b_moniter_pl_charg_bit == 0) {
		plcharg_status = upmu_is_chr_det();
		ret = pmic_config_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, 0x1, 0x0001, 0x2);
		ret = pmic_config_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, plcharg_status, 0x0001, 0x1);
	}

	pal_log_err(
		"[fg_init] fg_reset_status %d do_init_fgadc_reset %d fg_curr_time %d fg_soff_valid:%d \
		shutdown_pmic_time %d hw_id 0x%x sw_id 0x%x, %d %d 0x%x 0x%x %d %d efuse_cal %d,rgspare:[%d %d]\n",
			fg_reset_status, do_init_fgadc_reset, fg_curr_time, fg_soff_valid,
			shutdown_pmic_time, hw_id, sw_id,
			boot_vbat, shutdowntime, reset_sel, slp_en, b_moniter_pl_charg_bit, plcharg_status,
			efuse_cal, rgspare, newrgspare);
	is_fg_init = true;

	if (hw_check_battery() == 1) {
		if (drdi_get_hw_ver() == HW_VER_V1)
			pl_check_bat_protect_status();
	}

}

void fg_set_2sec_reboot(void)
{
	if (drdi_get_hw_ver() == HW_VER_V0)
		return;

	pmic_config_interface((U32)(PMIC_RG_SYSTEM_INFO_CON0_ADDR), (U32)(1),
		(U32)(0x0001),
		(U32)(0));
	pal_log_info("[fg_set_2sec_reboot]set PMIC_RG_SYSTEM_INFO_CON0_ADDR\n");
}

#else
void fg_set_2sec_reboot(void)
{
}
#endif


u32 fg_get_2sec_reboot(void)
{
	return 0;
}

int fg_get_vbat(void)
{
	return boot_vbat;
}

int fg_get_shutdowntime(void)
{
	return shutdowntime;
}

void pl_battery_init(bool force_init)
{
#if !CFG_FPGA_PLATFORM
	bool is_battery_exist;

	if (is_fg_init == true) {
		pal_log_info("[pl_battery_init] is_fg_init: %d , skip init\n", is_fg_init);
		return;
	}

	if (drdi_get_hw_ver() == HW_VER_V0)
		return;

	is_battery_exist = hw_check_battery();
	pal_log_info("[pl_battery_init] fg:1 is_fg_init:%d , force_init:%d bat:%d\n",
		is_fg_init, force_init, is_battery_exist);

	if (force_init == false && is_battery_exist == false) {
		pal_log_info("[pl_battery_init] is_fg_init:%d , force_init:%d bat:%d , skip init\n",
			is_fg_init, force_init, is_battery_exist);
		return;
	}

#if CFG_BATTERY_DETECT
	if (force_init == true) {
		while (hw_check_battery() == 0) {
			mdelay(300);
			platform_wdt_all_kick();
		}
	}
#endif

	fuel_gauge_init();

#endif
}

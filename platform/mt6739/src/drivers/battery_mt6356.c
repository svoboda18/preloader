#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>
#include "rtc.h"


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
//#if !CFG_EVB_PLATFORM && !CFG_FPGA_PLATFORM 
#if 1
static unsigned int fg_get_data_ready_status(void)
{
	unsigned int ret = 0;
	unsigned int temp_val = 0;

	ret = pmic_read_interface(PMIC_FG_LATCHDATA_ST_ADDR, &temp_val, 0xFFFF, 0x0);

	/*print("[fg_get_data_ready_status] Reg[0x%x]=0x%x\r\n", PMIC_FG_LATCHDATA_ST_ADDR, temp_val);*/

	temp_val =
	(temp_val & (PMIC_FG_LATCHDATA_ST_MASK << PMIC_FG_LATCHDATA_ST_SHIFT))
	>> PMIC_FG_LATCHDATA_ST_SHIFT;

	return temp_val;
}

signed int fgauge_get_time(void *data)
{
	unsigned int time_29_16, time_15_00, ret_time;
	int m = 0;
	unsigned int ret = 0;
	unsigned long time = 0;

	ret = pmic_config_interface(MT6356_FGADC_CON1, 0x0001, 0x1F05, 0x0);
	/*(2)    Keep i2c read when status = 1 (0x06) */
	m = 0;
	while (fg_get_data_ready_status() == 0) {
		m++;
		if (m > 10) {
			print(
				 "[fgauge_get_time] fg_get_data_ready_status timeout 1 !\r\n");
			break;
		}
	}

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

	print(
		 "[fgauge_get_time] low:0x%x high:0x%x rtime:0x%lx 0x%x!\r\n",
		 time_15_00, time_29_16, time, ret_time);


	ret = pmic_config_interface(MT6356_FGADC_CON1, 0x0008, 0x000F, 0x0);

	m = 0;
	while (fg_get_data_ready_status() != 0) {
		m++;
		if (m > 1000) {
			print(
				 "[fgauge_get_time] fg_get_data_ready_status timeout 2 !\r\n");
			break;
		}
	}
	/*(8)    Recover original settings */
	ret = pmic_config_interface(MT6356_FGADC_CON1, 0x0000, 0x000F, 0x0);

	*(unsigned int *) (data) = ret_time;

	return 0;
}

signed int fgauge_get_soff_time(void *data)
{
	unsigned int soff_time_29_16, soff_time_15_00, ret_time;
	int m = 0;
	unsigned int ret = 0;
	unsigned long time = 0;

	ret = pmic_config_interface(MT6356_FGADC_CON1, 0x0001, 0x1F05, 0x0);
	/*(2)    Keep i2c read when status = 1 (0x06) */
	m = 0;
	while (fg_get_data_ready_status() == 0) {
		m++;
		if (m > 1000) {
			print(
				 "[fgauge_get_soff_time] fg_get_data_ready_status timeout 1 !\r\n");
			break;
		}
	}

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

	print(
		 "[fgauge_get_soff_time] low:0x%x high:0x%x rtime:0x%lx 0x%x!\r\n",
		 soff_time_15_00, soff_time_29_16, time, ret_time);


	ret = pmic_config_interface(MT6356_FGADC_CON1, 0x0008, 0x000F, 0x0);

	m = 0;
	while (fg_get_data_ready_status() != 0) {
		m++;
		if (m > 1000) {
			print(
				 "[fgauge_get_soff_time] fg_get_data_ready_status timeout 2 !\r\n");
			break;
		}
	}
	/*(8)    Recover original settings */
	ret = pmic_config_interface(MT6356_FGADC_CON1, 0x0000, 0x000F, 0x0);

	*(unsigned int *) (data) = ret_time;

	return 0;
}

void fuel_gauge_init(void)
{
	int fg_reset_status;
	signed int efuse_cal;
	int fg_curr_time;
	int shutdown_pmic_time;
	int do_init_fgadc_reset;
	int ret;
	int hw_id, sw_id;
	U32 reset_sel;

	reset_sel = upmu_get_reg_value(PMIC_RG_FGADC_RST_SRC_SEL_ADDR);
	//only for GM 3.0 : set FGADC reset source selection = 1 ,GM3.0 reset
	ret = pmic_config_interface((U32)(PMIC_RG_FGADC_RST_SRC_SEL_ADDR), (U32)(1),
		(U32)(PMIC_RG_FGADC_RST_SRC_SEL_MASK),
		(U32)(PMIC_RG_FGADC_RST_SRC_SEL_SHIFT));

	/*fg_reset_status = pmic_get_register_value(PMIC_FG_RSTB_STATUS);*/
	ret = pmic_read_interface((U32)(PMIC_FG_RSTB_STATUS_ADDR), (&fg_reset_status),
		(U32)(PMIC_FG_RSTB_STATUS_MASK),
		(U32)(PMIC_FG_RSTB_STATUS_SHIFT));

	/* eFuse */
	/*efuse_cal = pmic_get_register_value(PMIC_RG_FGADC_GAINERROR_CAL);*/
	ret = pmic_read_interface((U32)(PMIC_RG_FGADC_GAINERROR_CAL_ADDR), (&efuse_cal),
		(U32)(PMIC_RG_FGADC_GAINERROR_CAL_MASK),
		(U32)(PMIC_RG_FGADC_GAINERROR_CAL_SHIFT));

	/*pmic_set_register_value(PMIC_FG_GAIN, efuse_cal);*/
	ret = pmic_config_interface((U32)(PMIC_FG_GAIN_ADDR), (U32)(efuse_cal),
		(U32)(PMIC_FG_GAIN_MASK),
		(U32)(PMIC_FG_GAIN_SHIFT));

	hw_id = upmu_get_reg_value(MT6356_HWCID);
	sw_id = upmu_get_reg_value(MT6356_SWCID);

	ret = fgauge_get_time(&fg_curr_time);

	ret = fgauge_get_soff_time(&shutdown_pmic_time);

	if (fg_reset_status == 0)
		do_init_fgadc_reset = 1;
	else if ((shutdown_pmic_time != 0) && (fg_curr_time - shutdown_pmic_time > PMIC_SHUTDOWN_TIME * 60))
		do_init_fgadc_reset = 1;
	else
		do_init_fgadc_reset = 0;

	if (do_init_fgadc_reset == 1) {
		/*pmic_set_register_value(PMIC_FG_SOFF_SLP_EN, 1);*/
		ret = pmic_config_interface((U32)(PMIC_FG_SOFF_SLP_EN_ADDR), (U32)(1),
			(U32)(PMIC_FG_SOFF_SLP_EN_MASK),
			(U32)(PMIC_FG_SOFF_SLP_EN_SHIFT));

		/* reset CON1 */
		ret = pmic_config_interface(MT6356_FGADC_CON1, 0x1F04, 0xFFFF, 0x0);
		mdelay(1);
		ret = pmic_config_interface(MT6356_FGADC_CON1, 0x0000, 0x1F04, 0x0);

	}

	/* Elbrus E1 */
	if (hw_id == 0x3510) {
		/*pmic_set_register_value(PMIC_RG_BATON_CHRDET_SW, 1);*/
		ret = pmic_config_interface((U32)(PMIC_RG_BATON_CHRDET_SW_ADDR), (U32)(1),
			(U32)(PMIC_RG_BATON_CHRDET_SW_MASK),
			(U32)(PMIC_RG_BATON_CHRDET_SW_SHIFT));

		/*pmic_set_register_value(PMIC_RG_BATON_CHRDET_TEST_MODE, 1);*/
		ret = pmic_config_interface((U32)(PMIC_RG_BATON_CHRDET_TEST_MODE_ADDR), (U32)(1),
			(U32)(PMIC_RG_BATON_CHRDET_TEST_MODE_MASK),
			(U32)(PMIC_RG_BATON_CHRDET_TEST_MODE_SHIFT));

		print("[fuel_gauge_init]0x01E0 = 0x%x  0x01DE = 0x%x before\n", upmu_get_reg_value(0x15E0), upmu_get_reg_value(0x15DE));
		upmu_set_reg_value(0x15E0, 0x1);
		upmu_set_reg_value(0x15DE, 0x1);
		mdelay(10);
		print("[fuel_gauge_init]0x01E0 = 0x%x  0x01DE = 0x%x after\n", upmu_get_reg_value(0x15E0), upmu_get_reg_value(0x15DE));

	}

	boot_vbat = get_bat_sense_volt(1);
	shutdowntime = fg_curr_time - shutdown_pmic_time;

	print(
		"[fuel_gauge_init] fg_reset_status %d do_init_fgadc_reset %d fg_curr_time %d shutdown_pmic_time %d hw_id 0x%x sw_id 0x%x, %d %d 0x%x\n",
			fg_reset_status, do_init_fgadc_reset, fg_curr_time, shutdown_pmic_time, hw_id, sw_id,
			boot_vbat, shutdowntime, reset_sel);
	is_fg_init = true;
	
}
#endif

int fg_get_vbat(void)
{
	return boot_vbat;
}

int fg_get_shutdowntime(void)
{
	return shutdowntime;
}

//extern U16 rtc_rdwr_fginit_bits(U16 *val);

void fg_set_2sec_reboot(void)
{

	pmic_config_interface((U32)(PMIC_RG_SYSTEM_INFO_CON0_ADDR), (U32)(1),
		(U32)(0x0001),
		(U32)(0));
	print("[fg_set_2sec_reboot]set system_info_con0\n");

/*
	U16 rvalue, wvalue;
	int hw_id = upmu_get_reg_value(MT6356_HWCID);
	U16 temp_value;
		
	rvalue = rtc_rdwr_fginit_bits(NULL);

	if ((hw_id & 0xff00) == 0x3500) {
		temp_value = 0x80;
		rtc_rdwr_fginit_bits(&temp_value);
		mdelay(1);
		temp_value = 0x00;
		rtc_rdwr_fginit_bits(&temp_value);
	} else {
		temp_value = 0x80;
		rtc_rdwr_fginit_bits(&temp_value);
		mdelay(1);
		temp_value = 0x20;
		rtc_rdwr_fginit_bits(&temp_value);
	}
	wvalue = rtc_rdwr_fginit_bits(NULL);
	print("[fg_set_2sec_reboot]before:0x%x after:0x%x\n", rvalue, wvalue);
*/
}

void pl_battery_init(bool force_init)
{
	#if !CFG_EVB_PLATFORM && !CFG_FPGA_PLATFORM 
	bool is_battery_exist;

	is_battery_exist = hw_check_battery();
	print("[pl_battery_init] is_fg_init:%d , force_init:%d bat:%d\n", 
		is_fg_init, force_init, is_battery_exist);

	if (is_fg_init == true) {
		print("[pl_battery_init] is_fg_init: %d , skip init\n", is_fg_init);
		return;
	}

	if (force_init == false && is_battery_exist == false) {
		print("[pl_battery_init] is_fg_init:%d , force_init:%d bat:%d , skip init\n", 
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

#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>

//==============================================================================
// Battery defines
//==============================================================================
#if 0 //to reduce code size
static int boot_vbat;
#endif


//==============================================================================
// Battery functions
//==============================================================================
#if !CFG_EVB_PLATFORM && !CFG_FPGA_PLATFORM
static unsigned int fg_get_data_ready_status(void)
{
	unsigned int ret = 0;
	unsigned int temp_val = 0;

	ret = pmic_read_interface(PMIC_FG_LATCHDATA_ST_ADDR, &temp_val, 0xFFFF, 0x0);

	temp_val =
	(temp_val & (PMIC_FG_LATCHDATA_ST_MASK << PMIC_FG_LATCHDATA_ST_SHIFT))
	>> PMIC_FG_LATCHDATA_ST_SHIFT;

	return temp_val;
}

void fuel_gauge_init(void)
{
	int m = 0, ret;
	int b_moniter_pl_charg_bit;
	int plcharg_status = 0;
	U32 gain_cal;


#if 0 //to reduce code size
	boot_vbat = get_bat_sense_volt(1);
#endif

	pmic_read_interface(PMIC_RG_FGADC_GAINERROR_CAL_ADDR, &gain_cal, PMIC_RG_FGADC_GAINERROR_CAL_MASK, PMIC_RG_FGADC_GAINERROR_CAL_SHIFT);
	pmic_config_interface(PMIC_FG_GAIN_ADDR, gain_cal, PMIC_FG_GAIN_MASK, PMIC_FG_GAIN_SHIFT);

	/*reset HW FG */
	pmic_config_interface(PMIC_FG_CHARGE_RST_ADDR, 1, PMIC_FG_CHARGE_RST_MASK, PMIC_FG_CHARGE_RST_SHIFT);
	pmic_config_interface(PMIC_FG_TIME_RST_ADDR, 1, PMIC_FG_TIME_RST_MASK, PMIC_FG_TIME_RST_SHIFT);
	pmic_config_interface(PMIC_FG_OFFSET_RST_ADDR, 1, PMIC_FG_OFFSET_RST_MASK, PMIC_FG_OFFSET_RST_SHIFT);
	pmic_config_interface(PMIC_FG_SW_CR_ADDR, 1, PMIC_FG_SW_CR_MASK, PMIC_FG_SW_CR_SHIFT);
	udelay(100);
	pmic_config_interface(PMIC_FG_CHARGE_RST_ADDR, 0, PMIC_FG_CHARGE_RST_MASK, PMIC_FG_CHARGE_RST_SHIFT);
	pmic_config_interface(PMIC_FG_TIME_RST_ADDR, 0, PMIC_FG_TIME_RST_MASK, PMIC_FG_TIME_RST_SHIFT);
	pmic_config_interface(PMIC_FG_OFFSET_RST_ADDR, 0, PMIC_FG_OFFSET_RST_MASK, PMIC_FG_OFFSET_RST_SHIFT);
	pmic_config_interface(PMIC_FG_SW_CR_ADDR, 0, PMIC_FG_SW_CR_MASK, PMIC_FG_SW_CR_SHIFT);

	pmic_config_interface(PMIC_FG_SW_READ_PRE_ADDR, 1, PMIC_FG_SW_READ_PRE_MASK, PMIC_FG_SW_READ_PRE_SHIFT);	
	m = 0;
	while (fg_get_data_ready_status() == 0) {
		udelay(100);
		m++;
		if (m > 5) {
			print("1.PMIC_FG_LATCHDATA_ST_SHIFT = 0\r\n");
			break;
		}
	}

	pmic_config_interface(PMIC_FG_SW_CLEAR_ADDR, 1, PMIC_FG_SW_CLEAR_MASK, PMIC_FG_SW_CLEAR_SHIFT);
	pmic_config_interface(PMIC_FG_SW_READ_PRE_ADDR, 0, PMIC_FG_SW_READ_PRE_MASK, PMIC_FG_SW_READ_PRE_SHIFT);
	m = 0;
	while (fg_get_data_ready_status() != 0) {
		udelay(100);
		m++;
		if (m > 5) {
			print("2.PMIC_FG_LATCHDATA_ST_SHIFT != 0\r\n");
			break;
		}
	}

	pmic_config_interface(PMIC_FG_SW_CLEAR_ADDR, 0, PMIC_FG_SW_CLEAR_MASK, PMIC_FG_SW_CLEAR_SHIFT);


	/* if WDT reset from PL to KERNEL, b_moniter_pl_charg_bit will keep 1*/
	/* record PL charging status in bit1, mointer bit in bit2 */
	pmic_read_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, &b_moniter_pl_charg_bit, 0x0001, 0x2);
	if (b_moniter_pl_charg_bit == 0) {
		plcharg_status = upmu_is_chr_det();
		ret = pmic_config_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, 0x1, 0x0001, 0x2);
		ret = pmic_config_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, plcharg_status, 0x0001, 0x1);
	}

	ret = pmic_read_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, &b_moniter_pl_charg_bit, 0x0001, 0x2);
	ret = pmic_read_interface(PMIC_RG_SYSTEM_INFO_CON0_ADDR, &plcharg_status, 0x0001, 0x1);

	print("pl chr:%d monitor:%d plchr:%d gain:%d\r\n",
		upmu_is_chr_det(),
		b_moniter_pl_charg_bit,
		plcharg_status,
		gain_cal);

	if (hw_check_battery() == 1) {
		pl_check_bat_protect_status();
	}

}
#endif

#if 0 //to reduce code size
int fg_get_vbat(void)
{
	return boot_vbat;
}

int fg_get_shutdowntime(void)
{
	return 0;
}
#endif

void pl_battery_init(bool force_init)
{
	if (force_init == true)
		return;

	#if !CFG_EVB_PLATFORM && !CFG_FPGA_PLATFORM
	fuel_gauge_init();
	#endif
}

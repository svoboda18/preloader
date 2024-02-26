#include <typedefs.h>
#include <platform.h>
#include <pmic_wrap_init.h>
#include <pmic.h>
#include <sec_devinfo.h>
#include <rtc.h>


/* External BUCK Config */
#define EXTERNAL_BUCK_RT5738

#if defined(EXTERNAL_BUCK_RT5738)
#include <rt5738.h>
#endif /* EXTERNAL_BUCK_RT5738) */

#define DEBUGSTS 1
#define EFUSE_DEBUG 0
#define PMIC_32K_LESS_DETECT_V1 0

/* Enable this option when pmic initial setting is verified */
#define INIT_SETTING_VERIFIED 1
/* Enable this option to apply pmic initial setting with array */
#define ARRAY_INIT_SETTING 0

int PMIC_AUXADC_INIT_SETTING_V1(void);


/* PMIC access API */
U32 pmic_read_interface(U32 RegNum, U32 *val, U32 MASK, U32 SHIFT)
{
	U32 return_value = 0;
	U32 pmic_reg = 0;
	U32 rdata = 0;

	return_value = pwrap_read(RegNum, &rdata);
	pmic_reg = rdata;
	if (return_value != 0) {
		print("[PMIC]Reg[0x%x]= pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	//print("[pmic_read_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

	pmic_reg &= (MASK << SHIFT);
	*val = (pmic_reg >> SHIFT);
	//print("[pmic_read_interface] val=0x%x\n", *val);

	return return_value;
}

U32 pmic_config_interface(U32 RegNum, U32 val, U32 MASK, U32 SHIFT)
{
	U32 return_value = 0;
	U32 pmic_reg = 0;
	U32 rdata = 0;

	return_value = pwrap_read(RegNum, &rdata);
	pmic_reg = rdata;
	if(return_value!=0)
	{
		print("[PMIC]Reg[%x]= pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	//print("[pmic_config_interface] Reg[%x]=0x%x\n", RegNum, pmic_reg);

	pmic_reg &= ~(MASK << SHIFT);
	pmic_reg |= (val << SHIFT);

	//2. mt_write_byte(RegNum, pmic_reg);
	return_value = pwrap_write(RegNum, pmic_reg);
	if (return_value != 0) {
		print("[PMIC]Reg[%x]= pmic_wrap read data fail\n", RegNum);
		return return_value;
	}
	//print("[pmic_config_interface] write Reg[%x]=0x%x\n", RegNum, pmic_reg);

	return return_value;
}

U32 pmic_get_register_value(pmuflag pmureg)
{
	U32 val = 0;
	U32 ret;

	ret = pmic_read_interface(pmureg.addr, &val, pmureg.mask, pmureg.shift);

	return val;
}

U32 pmic_set_register_value(pmuflag pmureg, U32 val)
{
	U32 ret;

	ret = pmic_config_interface(pmureg.addr, val, pmureg.mask, pmureg.shift);

	return ret;
}

U32 upmu_get_reg_value(U32 reg)
{
	U32 ret = 0;
	U32 reg_val = 0;

	ret = pmic_read_interface(reg, &reg_val, 0xFFFF, 0x0);

	return reg_val;
}

U32 upmu_set_reg_value(U32 reg, U32 reg_val)
{
	U32 ret = 0;

	ret = pmic_config_interface(reg, reg_val, 0xFFFF, 0x0);

	return ret;
}

//==============================================================================
// PMIC Usage APIs
//==============================================================================
U32 get_PMIC_chip_version (void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_SWCID_ADDR), (&val),
		(U32)(PMIC_SWCID_MASK),
		(U32)(PMIC_SWCID_SHIFT));

	return val;
}

int pmic_detect_powerkey(void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_PWRKEY_DEB_ADDR), (&val),
		(U32)(PMIC_PWRKEY_DEB_MASK),
		(U32)(PMIC_PWRKEY_DEB_SHIFT));

	if (val == 1){
		print("[PMIC]powerkey Release\n");
		return 0;
	} else {
		print("[PMIC]powerkey Press\n");
		return 1;
	}
}

int pmic_detect_homekey(void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_HOMEKEY_DEB_ADDR), (&val),
		(U32)(PMIC_HOMEKEY_DEB_MASK),
		(U32)(PMIC_HOMEKEY_DEB_SHIFT));

	if (val == 1) {
		print("[PMIC]FCHRKEY Release\n");
		return 0;
	} else {
		print("[PMIC]FCHRKEY Press\n");
		return 1;
	}
}

U32 pmic_IsUsbCableIn (void)
{
#if CFG_EVB_PLATFORM
	return PMIC_CHRDET_EXIST;	// for bring up
	//print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);
#else
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_RGS_CHRDET_ADDR), (&val),
		(U32)(PMIC_RGS_CHRDET_MASK),
		(U32)(PMIC_RGS_CHRDET_SHIFT));
	print("[PMIC]IsUsbCableIn %d\n", val);
	if (val)
		return PMIC_CHRDET_EXIST;
	else
		return PMIC_CHRDET_NOT_EXIST;
#endif
}

U32 pmic_upmu_set_baton_tdet_en(U32 val)
{
	U32 ret = 0;

	ret = pmic_config_interface((U32)(PMIC_BATON_TDET_EN_ADDR), (U32)(val),
		(U32)(PMIC_BATON_TDET_EN_MASK),
		(U32)(PMIC_BATON_TDET_EN_SHIFT));

	return ret;
}

U32 pmic_upmu_set_rg_baton_en(U32 val)
{
	U32 ret = 0;

	ret = pmic_config_interface((U32)(PMIC_RG_BATON_EN_ADDR), (U32)(val),
		(U32)(PMIC_RG_BATON_EN_MASK),
		(U32)(PMIC_RG_BATON_EN_SHIFT));

	return ret;
}

U32 pmic_upmu_get_rgs_baton_undet(void)
{
	U32 ret = 0;
	U32 val = 0;

	ret = pmic_read_interface((U32)(PMIC_RGS_BATON_UNDET_ADDR), (&val),
		(U32)(PMIC_RGS_BATON_UNDET_MASK),
		(U32)(PMIC_RGS_BATON_UNDET_SHIFT));

	return val;
}

U32 upmu_is_chr_det(void)
{
	U32 ret = 0;
	U32 val = 0;
#if CFG_EVB_PLATFORM
	val = 1; // for bring up
	//print("[pmic_IsUsbCableIn] have CFG_EVB_PLATFORM, %d\n", val);
#else
	ret = pmic_read_interface((U32)(PMIC_RGS_CHRDET_ADDR), (&val),
		(U32)(PMIC_RGS_CHRDET_MASK),
		(U32)(PMIC_RGS_CHRDET_SHIFT));
#endif
	return val;
}

void PMIC_enable_long_press_reboot(void)
{
#if !CFG_FPGA_PLATFORM
#if !CFG_EVB_PLATFORM
#if KPD_PMIC_LPRST_TD!=0
	#if ONEKEY_REBOOT_NORMAL_MODE_PL
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x01, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_HOMEKEY_RST_EN_ADDR, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_PWRKEY_RST_TD_ADDR, (U32)KPD_PMIC_LPRST_TD,
		PMIC_RG_PWRKEY_RST_TD_MASK, PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#else
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x01, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_HOMEKEY_RST_EN_ADDR, 0x01, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_PWRKEY_RST_TD_ADDR, (U32)KPD_PMIC_LPRST_TD, PMIC_RG_PWRKEY_RST_TD_MASK, PMIC_RG_PWRKEY_RST_TD_SHIFT);
	#endif
#else
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_HOMEKEY_RST_EN_ADDR, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
#endif
#else
	pmic_config_interface(PMIC_RG_PWRKEY_RST_EN_ADDR, 0x00, PMIC_RG_PWRKEY_RST_EN_MASK, PMIC_RG_PWRKEY_RST_EN_SHIFT);
	pmic_config_interface(PMIC_RG_HOMEKEY_RST_EN_ADDR, 0x00, PMIC_RG_HOMEKEY_RST_EN_MASK, PMIC_RG_HOMEKEY_RST_EN_SHIFT);
#endif
}

U32 PMIC_VUSB_EN(void)
{
	int ret = 0;

	ret = pmic_config_interface((U32)(PMIC_RG_VUSB33_SW_EN_ADDR), 1,
		(U32)(PMIC_RG_VUSB33_SW_EN_MASK),
		(U32)(PMIC_RG_VUSB33_SW_EN_SHIFT));

	return ret;
}

#if 0 //remove unused function to save space of log string
int set_pmic_setting(struct pmu_init_s pisac[], unsigned int init_size)
{
	int ret = 0;
	unsigned int i = 0, j = 0;
	unsigned int return_value = 0;
	unsigned int pmic_reg = 0;
	unsigned int rdata;

	for (i = 0; i < init_size; i += pisac[i].con) {
		ret = pwrap_read(pisac[i].addr, &rdata);
		pmic_reg = rdata;
		if (ret != 0) {
			print("[PMIC]Reg[0x%x]= pmic_wrap read data fail\n", (pisac[i].addr));
			return_value++;
		}

		for (j = 0; j < pisac[i].con; j++) {
			pmic_reg &= ~(pisac[i+j].mask << pisac[i+j].shift);
			pmic_reg |= (pisac[i+j].val << pisac[i+j].shift);
		}

		ret = pwrap_write(pisac[i].addr, pmic_reg);
		if (ret != 0) {
			print("[PMIC]Reg[%x]= pmic_wrap read data fail\n", (pisac[i].addr));
			return_value++;
		}
	}

	return return_value;
}

void mt6335_dump_register(void)
{
	unsigned short i = 0;

	print("dump PMIC 6335 register\n");

	for (i = 0; i <= (0x2C2E - 10); i = i + 10) {
		print("Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x Reg[0x%x]=0x%x\n",
			i, upmu_get_reg_value(i),
			i + 2, upmu_get_reg_value(i + 2),
			i + 4, upmu_get_reg_value(i + 4),
			i + 6, upmu_get_reg_value(i + 6),
			i + 8, upmu_get_reg_value(i + 8));
	}

}
#endif //remove unused function to save space of log string

#if ARRAY_INIT_SETTING
int pmic_config_setting(void)
{
	unsigned int init_size;
	struct pmu_init_s pisac[] = {
	};

	init_size = ARRAY_SIZE(pisac);
	set_pmic_setting(pisac, init_size);
	return 0;
}
#endif
struct pmic_setting init_setting[] = {
	{0xC, 0x1, 0x1, 3}, /* [3:3]: RG_STRUP_AUXADC_RSTB_SEL; From ZF's golden setting 20160129 */
	{0x18, 0x1, 0x1, 15}, /* [15:15]: RG_STRUP_ENVTEM_CTRL; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 0}, /* [0:0]: RG_STRUP_VSRAM_DVFS2_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 1}, /* [1:1]: RG_STRUP_VSRAM_DVFS1_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 2}, /* [2:2]: RG_STRUP_VSRAM_CORE_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 3}, /* [3:3]: RG_STRUP_VSRAM_GPU_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 4}, /* [4:4]: RG_STRUP_VSRAM_MD_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 5}, /* [5:5]: RG_STRUP_VUFS18_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 6}, /* [6:6]: RG_STRUP_VEMC_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 7}, /* [7:7]: RG_STRUP_VA12_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 8}, /* [8:8]: RG_STRUP_VA10_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 9}, /* [9:9]: RG_STRUP_VA18_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 10}, /* [10:10]: RG_STRUP_VDRAM_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 11}, /* [11:11]: RG_STRUP_VMODEM_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 12}, /* [12:12]: RG_STRUP_VMD1_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 13}, /* [13:13]: RG_STRUP_VS2_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 14}, /* [14:14]: RG_STRUP_VS1_PG_H2L_EN; 16/01/27 , Kim */
	{0x1C, 0x1, 0x1, 15}, /* [15:15]: RG_STRUP_VCORE_PG_H2L_EN; 16/01/27 , Kim */
	{0x1E, 0x1, 0x1, 13}, /* [13:13]: RG_STRUP_VIMVO_PG_H2L_EN; 16/06/29 , Nick */
	{0x1E, 0x1, 0x1, 14}, /* [14:14]: RG_STRUP_EXT_PMIC_PG_H2L_EN; 16/01/27 , Kim */
	{0x1E, 0x1, 0x1, 15}, /* [15:15]: RG_STRUP_VUSB33_PG_H2L_EN; 16/01/27 , Kim */
	{0x28, 0x1, 0x1, 8}, /* [8:8]: RG_RTC_SPAR_DEB_EN; Scott (16/01/14) */
	{0x28, 0x1, 0x1, 9}, /* [9:9]: RG_RTC_ALARM_DEB_EN; Scott (16/01/14) */
	{0x2A, 0x1, 0x1, 14}, /* [14:14]: RG_EN_DRVSEL; 16/01/27 , Kim */
	{0x2A, 0x1, 0x1, 15}, /* [15:15]: RG_RST_DRVSEL; 16/01/27 , Kim */
	{0x2E, 0x1, 0x1, 0}, /* [0:0]: RG_PWRHOLD; 16/01/27 , Kim, INIT_SET */
	{0x38, 0x1, 0xFF, 0}, /* [7:0]: RG_PSEQ_RSV0; 16/07/11 , Scott-CF, to improve SPAR stability */
	{0x3A, 0x1, 0x1, 0}, /* [0:0]: RG_BWDT_EN; 16/06/29 , Kim */
	{0x204, 0x1, 0x1, 5}, /* [5:5]: RG_SRCLKEN_IN0_HW_MODE; Nick, From MT6351 */
	{0x204, 0x1, 0x1, 6}, /* [6:6]: RG_SRCLKEN_IN1_HW_MODE; Nick, From MT6351 */
	{0x204, 0x1, 0x1, 7}, /* [7:7]: RG_OSC_SEL_HW_MODE; Nick, From MT6351 */
	{0x220, 0x1, 0x1, 0}, /* [0:0]: RG_SMT_WDTRSTB_IN; Nick, From MT6351 */
	{0x220, 0x1, 0x1, 1}, /* [1:1]: RG_SMT_HOMEKEY; 16/01/27 , Kim */
	{0x406, 0x1, 0x1, 6}, /* [6:6]: RG_LDO_CALI_75K_CK_PDN; Waverly */
	{0x406, 0x1, 0x1, 8}, /* [8:8]: RG_TRIM_75K_CK_PDN; Waverly */
	{0x40C, 0x1, 0x1, 2}, /* [2:2]: RG_RTCDET_CK_PDN; Waverly, From MT6351 */
	{0x40C, 0x1, 0x1, 3}, /* [3:3]: RG_RTC_75K_CK_PDN; Waverly, From MT6351 */
	{0x40C, 0x0, 0x1, 13}, /* [13:13]: RG_RTC32K_1V8_1_O_PDN; 5/30, KH, for MT6337 ACCDET function */
	{0x412, 0x1, 0x1, 1}, /* [1:1]: RG_EFUSE_CK_PDN; Waverly, From MT6351 */
	{0x418, 0x1, 0x1, 3}, /* [3:3]: RG_75K_32K_SEL; Waverly */
	{0x42A, 0x1, 0x1, 7}, /* [7:7]: RG_REG_CK_PDN_HWEN; Waverly */
	{0x606, 0x1, 0x1, 6}, /* [6:6]: RG_FGADC_RST_SRC_SEL; GM3.0 */
	{0x60E, 0x1, 0x1, 0}, /* [0:0]: RG_WDTRSTB_EN; Kim,2016/01/22 */
	{0x60E, 0x1, 0x1, 5}, /* [5:5]: RG_WDTRSTB_DEB; Kim,2016/01/22 */
	{0xC00, 0x1, 0x1, 0}, /* [0:0]: RG_SLP_RW_EN; Waverly */
	{0xE00, 0x1, 0x1, 1}, /* [1:1]: RG_BUCK_DCM_MODE; CC Lee, to improve low power performance */
	{0xE0C, 0x1, 0x1, 7}, /* [7:7]: RG_BUCK_VIMVO_OC_SDN_EN; 10/11, Mason, Align to non-VPA buck */
	{0xE1C, 0x1B, 0x7F, 0}, /* [6:0]: RG_BUCK_VCORE_VOSEL_SLEEP; 16/6/29, Nick, 0.56875V */
	{0xE1E, 0xA, 0x7F, 0}, /* [6:0]: RG_BUCK_VCORE_SFCHG_FRATE; 16/8/12, Nick, Enable falling slope of 5.12mV/us */
	{0xE1E, 0xA, 0x7F, 8}, /* [14:8]: RG_BUCK_VCORE_SFCHG_RRATE; 16/8/12, Nick, Slope decreased to 5.12mV/us */
	{0xE20, 0x1, 0x3, 4}, /* [5:4]: RG_BUCK_VCORE_DVS_TRANS_CTRL; 16/9/6, Freddy, force PWM enabled at falling slope */
	{0xE28, 0x1, 0x1, 8}, /* [8:8]: RG_BUCK_VCORE_ON_OP;  */
	{0xE28, 0x0, 0x1, 9}, /* [9:9]: RG_BUCK_VCORE_LP_OP;  */
	{0xE4A, 0x1, 0x1, 8}, /* [8:8]: RG_BUCK_VDRAM_ON_OP;  */
	{0xE4A, 0x0, 0x1, 9}, /* [9:9]: RG_BUCK_VDRAM_LP_OP;  */
	{0xE60, 0x1B, 0x7F, 0}, /* [6:0]: RG_BUCK_VMD1_VOSEL_SLEEP; 16/6/29, Nick, 0.56875V */
	{0xE62, 0xA, 0x7F, 0}, /* [6:0]: RG_BUCK_VMD1_SFCHG_FRATE; 16/8/15, Nick, Slope decreased to 5.12mV/us */
	{0xE62, 0xA, 0x7F, 8}, /* [14:8]: RG_BUCK_VMD1_SFCHG_RRATE; 16/8/15, Nick, Slope decreased to 5.12mV/us */
	{0xE64, 0x1, 0x3, 4}, /* [5:4]: RG_BUCK_VMD1_DVS_TRANS_CTRL; Tim */
	{0xE6C, 0x1, 0x1, 8}, /* [8:8]: RG_BUCK_VMD1_ON_OP;  */
	{0xE6C, 0x0, 0x1, 9}, /* [9:9]: RG_BUCK_VMD1_LP_OP;  */
	{0xE82, 0x1B, 0x7F, 0}, /* [6:0]: RG_BUCK_VMODEM_VOSEL_SLEEP; 16/6/29, Nick, 0.56875V */
	{0xE84, 0xA, 0x7F, 0}, /* [6:0]: RG_BUCK_VMODEM_SFCHG_FRATE; 16/8/15, Nick, Slope decreased to 5.12mV/us */
	{0xE84, 0xA, 0x7F, 8}, /* [14:8]: RG_BUCK_VMODEM_SFCHG_RRATE; 16/8/15, Nick, Slope decreased to 5.12mV/us */
	{0xE86, 0x1, 0x3, 4}, /* [5:4]: RG_BUCK_VMODEM_DVS_TRANS_CTRL; Tim */
	{0xE8E, 0x1, 0x1, 8}, /* [8:8]: RG_BUCK_VMODEM_ON_OP;  */
	{0xE8E, 0x0, 0x1, 9}, /* [9:9]: RG_BUCK_VMODEM_LP_OP;  */
	{0xEB0, 0x1, 0x1, 8}, /* [8:8]: RG_BUCK_VS1_ON_OP;  */
	{0xEB0, 0x0, 0x1, 9}, /* [9:9]: RG_BUCK_VS1_LP_OP;  */
	{0xED2, 0x1, 0x1, 8}, /* [8:8]: RG_BUCK_VS2_ON_OP;  */
	{0xED2, 0x0, 0x1, 9}, /* [9:9]: RG_BUCK_VS2_LP_OP;  */
	{0xEE8, 0x1, 0x7F, 0}, /* [6:0]: RG_BUCK_VPA1_SFCHG_FRATE; JL */
	{0xEE8, 0x1, 0x7F, 8}, /* [14:8]: RG_BUCK_VPA1_SFCHG_RRATE; JL */
	{0xEEA, 0x0, 0x3, 0}, /* [1:0]: RG_BUCK_VPA1_DVS_TRANS_TD; JL */
	{0xEEA, 0x3, 0x3, 4}, /* [5:4]: RG_BUCK_VPA1_DVS_TRANS_CTRL; JL */
	{0xEEA, 0x1, 0x1, 6}, /* [6:6]: RG_BUCK_VPA1_DVS_TRANS_ONCE; JL */
	{0xEEC, 0x1, 0x1, 6}, /* [6:6]: RG_BUCK_VPA1_DVS_BW_ONCE; JL */
	{0xEEE, 0x3, 0x3, 2}, /* [3:2]: RG_BUCK_VPA1_OC_WND; 2/24, Spec. from CW Cheng */
	{0xEF6, 0x10, 0x3F, 0}, /* [5:0]: RG_BUCK_VPA1_VOSEL_DLC011; */
	{0xEF6, 0x28, 0x3F, 8}, /* [13:8]: RG_BUCK_VPA1_VOSEL_DLC111; */
	{0xEF8, 0x2, 0x3F, 8}, /* [13:8]: RG_BUCK_VPA1_VOSEL_DLC001; 2/16, to solve VPA=0.85V current capability issue */
	{0xF18, 0x1B, 0x7F, 0}, /* [6:0]: RG_BUCK_VIMVO_VOSEL_SLEEP; 16/6/29, Nick, 0.56875V */
	{0xF1A, 0xA, 0x7F, 0}, /* [6:0]: RG_BUCK_VIMVO_SFCHG_FRATE; 16/8/12, Nick, Slope decreased to 5.12mV/us */
	{0xF1A, 0xA, 0x7F, 8}, /* [14:8]: RG_BUCK_VIMVO_SFCHG_RRATE; 16/8/12, Nick, Slope decreased to 5.12mV/us */
	{0xF1C, 0x1, 0x3, 4}, /* [5:4]: RG_BUCK_VIMVO_DVS_TRANS_CTRL; 16/6/29, Nick, copy from VMD1 */
	{0xF24, 0x1, 0x1, 8}, /* [8:8]: RG_BUCK_VIMVO_ON_OP; */
	{0xF24, 0x0, 0x1, 9}, /* [9:9]: RG_BUCK_VIMVO_LP_OP; */
	{0xF50, 0x0, 0x3, 0}, /* [1:0]: RG_VCORE_SLEEP_VOLTAGE; 16/6/29, Nick, 0.56875V */
	{0xF50, 0x2, 0x3, 3}, /* [4:3]: RG_VMD1_SLEEP_VOLTAGE; 16/6/29, Nick, 0.56875V */
	{0xF50, 0x2, 0x3, 5}, /* [6:5]: RG_VMODEM_SLEEP_VOLTAGE; 16/6/29, Nick, 0.56875V */
	{0xF50, 0x2, 0x3, 7}, /* [8:7]: RG_VIMVO_SLEEP_VOLTAGE; 16/6/29, Nick, 0.56875V */
	{0xF50, 0x3, 0x7, 13}, /* [15:13]: RG_VSRAM_DVFS1_SLEEP_VOLTAGE; 16/6/29, Nick, 0.65V */
	{0xF52, 0x3, 0x7, 0}, /* [2:0]: RG_VSRAM_DVFS2_SLEEP_VOLTAGE; 16/6/29, Nick, 0.65V */
	{0xF52, 0x3, 0x7, 3}, /* [5:3]: RG_VSRAM_VCORE_SLEEP_VOLTAGE; 16/6/29, Nick, 0.65V */
	{0xF52, 0x3, 0x7, 6}, /* [8:6]: RG_VSRAM_VMD_SLEEP_VOLTAGE; 16/6/29, Nick, 0.65V */
	{0xF52, 0x3, 0x7, 9}, /* [11:9]: RG_VSRAM_VGPU_SLEEP_VOLTAGE; 16/6/29, Nick, 0.65V */
	{0xF5A, 0x5, 0x7, 0}, /* [2:0]: RG_VCORE_SLP; 2/15, Improve jitter. Hung-Mu */
	{0xF5A, 0x0, 0x1, 3}, /* [3:3]: RG_VCORE_ADRC_FEN; For stability improvement, MT6351 issue. Hung-Mu */
	{0xF5A, 0x0, 0x1, 4}, /* [4:4]: RG_VCORE_VCCAP_CLAMP_FEN; 10/7, Nick, to improve transition overshoot from PFM to PWM */
	{0xF60, 0x1, 0x7, 4}, /* [6:4]: RG_VCORE_PFM_RIP; 2/16, Hung-Mu for AC spec. */
	{0xF66, 0x800, 0xFFFF, 0}, /* [15:0]: RG_VCORE_RSV; Hung-Mu, to improve low power performance */
	{0xF6E, 0x0, 0x1, 3}, /* [3:3]: RG_VDRAM_ADRC_FEN; For stability improvement, MT6351 issue. Hung-Mu */
	{0xF72, 0x0, 0xF, 6}, /* [9:6]: RG_VDRAM_NLIM_TRIMMING; 2/24, Terry-CJ, No FT on RG_DRAM_NLIM, it's trimmed with fixed code of 4'h8. So the register must be over-write to default code with 4'h0 */
	{0xF7A, 0x800, 0xFFFF, 0}, /* [15:0]: RG_VDRAM_RSV; Hung-Mu, to improve low power performance */
	{0xF80, 0x2, 0x7, 0}, /* [2:0]: RG_VMODEM_RZSEL0; 16/8/4, Hung-Mu, DVS down undershoot */
	{0xF80, 0x5, 0xF, 3}, /* [6:3]: RG_VMODEM_RZSEL1; '16/8/4, Hung-Mu, DVS down undershoot */
	{0xF80, 0x8, 0xF, 11}, /* [14:11]: RG_VMODEM_CSL; 2/13, Hung-Mu */
	{0xF82, 0x3, 0x7, 0}, /* [2:0]: RG_VMODEM_SLP; '16/8/4, Hung-Mu, DVS down undershoot */
	{0xF82, 0x0, 0x1, 3}, /* [3:3]: RG_VMODEM_ADRC_FEN; For stability improvement, MT6351 issue. Hung-Mu */
	{0xF82, 0x0, 0x1, 4}, /* [4:4]: RG_VMODEM_VCCAP_CLAMP_FEN; 16/8/4, Hung-Mu, Output overshoot @ mode transition */
	{0xF82, 0x0, 0x1, 5}, /* [5:5]: RG_VMODEM_VC_CLAMP_FEN; 2/14, Hung-Mu */
	{0xF88, 0x2, 0x7, 4}, /* [6:4]: RG_VMODEM_PFM_RIP; 2/13, Hung-Mu */
	{0xF8E, 0x800, 0xFFFF, 0}, /* [15:0]: RG_VMODEM_RSV; Hung-Mu, to improve low power performance */
	{0xF94, 0x2, 0x7, 0}, /* [2:0]: RG_VMD1_RZSEL0; 16/8/4, Hung-Mu, DVS down undershoot */
	{0xF94, 0x5, 0xF, 3}, /* [6:3]: RG_VMD1_RZSEL1; '16/8/4, Hung-Mu, DVS down undershoot */
	{0xF94, 0x8, 0xF, 11}, /* [14:11]: RG_VMD1_CSL; 2/13, Hung-Mu */
	{0xF96, 0x3, 0x7, 0}, /* [2:0]: RG_VMD1_SLP; '16/8/4, Hung-Mu, DVS down undershoot */
	{0xF96, 0x0, 0x1, 3}, /* [3:3]: RG_VMD1_ADRC_FEN; For stability improvement, MT6351 issue. Hung-Mu */
	{0xF96, 0x0, 0x1, 4}, /* [4:4]: RG_VMD1_VCCAP_CLAMP_FEN; '16/8/4, Hung-Mu, Output overshoot @ mode transition */
	{0xF9C, 0x2, 0x7, 4}, /* [6:4]: RG_VMD1_PFM_RIP; 2/13, Hung-Mu */
	{0xFA2, 0x800, 0xFFFF, 0}, /* [15:0]: RG_VMD1_RSV; Hung-Mu, to improve low power performance */
	{0xFA8, 0xC, 0xF, 11}, /* [14:11]: RG_VS1_CSL; 7/12, Hung-Mu, for VS1 OC ECO */
	{0xFAA,0x5,0x7, 6}, /* [8:6]: RG_VS1_PFMOC; 9/7,  Hung-Mu,  for VS1 PFMOC ECO (COHEC00014877) */
	{0xFB6, 0x800, 0xFFFF, 0}, /* [15:0]: RG_VS1_RSV; Hung-Mu, to improve low power performance */
	{0xFBC, 0xC, 0xF, 11}, /* [14:11]: RG_VS2_CSL; 7/12, Hung-Mu, for VS2 OC ECO */
	{0xFBE, 0x4, 0x7, 6}, /* [8:6]: RG_VS2_PFMOC; 7/12, Hung-Mu, for VS2 PFMOC ECO */
	{0xFCA, 0x800, 0xFFFF, 0}, /* [15:0]: RG_VS2_RSV; Hung-Mu, to improve low power performance */
	{0xFD0, 0x3, 0x3, 4}, /* [5:4]: RG_VPA1_CSMIR; Mason,Corner stability */
	{0xFD0, 0x1, 0x3, 6}, /* [7:6]: RG_VPA1_CSL; Mason,Corner current limit */
	{0xFD0, 0x2, 0x3, 8}, /* [9:8]: RG_VPA1_SLP; Mason,Corner stability */
	{0xFD0, 0x0, 0x1, 10}, /* [10:10]: RG_VPA1_AZC_EN; JL */
	{0xFD0, 0x1, 0x3, 14}, /* [15:14]: RG_VPA1_RZSEL; 2/15, JL, for VBAT = 3.0V */
	{0xFD2, 0x2, 0xF, 8}, /* [11:8]: RG_VPA1_NLIM_SEL; JL */
	{0xFD4, 0x1, 0x3, 2}, /* [3:2]: RG_VPA1_MIN_ON; Mason, Corner Accuracy */
	{0xFD4, 0x0, 0x3, 14}, /* [15:14]: RG_VPA1_MIN_PK; JL */
	{0xFD6, 0x2, 0xFF, 0}, /* [7:0]: RG_VPA1_RSV1; JL */
	{0xFD6, 0x88, 0xFF, 8}, /* [15:8]: RG_VPA1_RSV2; JL */
	{0xFDC, 0x2, 0x7, 0}, /* [2:0]: RG_VIMVO_RZSEL0; 16/8/4, Hung-Mu, DVS down undershoot */
	{0xFDC, 0x5, 0xF, 3}, /* [6:3]: RG_VIMVO_RZSEL1; 16/8/4, Hung-Mu, DVS down undershoot */
	{0xFDC, 0x8, 0xF, 11}, /* [14:11]: RG_VIMVO_CSL; 16/6/29, Nick, Copy from VMD1 */
	{0xFDE, 0x3, 0x7, 0}, /* [2:0]: RG_VIMVO_SLP; '16/8/4, Hung-Mu, DVS down undershoot */
	{0xFDE, 0x0, 0x1, 3}, /* [3:3]: RG_VIMVO_ADRC_FEN; 16/6/29, Nick, Copy from VMD1 */
	{0xFDE, 0x0, 0x1, 4}, /* [4:4]: RG_VIMVO_VCCAP_CLAMP_FEN; 16/8/4, Hung-Mu, Output overshoot @ mode transition */
	{0xFDE, 0x0, 0x1, 5}, /* [5:5]: RG_VIMVO_VC_CLAMP_FEN; 16/6/29, Nick, Copy from VMD1 */
	{0xFE4, 0x2, 0x7, 4}, /* [6:4]: RG_VIMVO_PFM_RIP; 16/6/29, Nick, Copy from VMD1 */
	{0xFEA, 0x800, 0xFFFF, 0}, /* [15:0]: RG_VIMVO_RSV; Hung-Mu, to improve low power performance */
	{0x1408, 0x1, 0x1, 8}, /* [8:8]: RG_VIO28_GO_ON_OP;  */
	{0x1408, 0x0, 0x1, 9}, /* [9:9]: RG_VIO28_GO_LP_OP;  */
	{0x141C, 0x1, 0x1, 8}, /* [8:8]: RG_VIO18_GO_ON_OP;  */
	{0x141C, 0x0, 0x1, 9}, /* [9:9]: RG_VIO18_GO_LP_OP;  */
	{0x1430, 0x1, 0x1, 8}, /* [8:8]: RG_VUFS18_GO_ON_OP;  */
	{0x1430, 0x0, 0x1, 9}, /* [9:9]: RG_VUFS18_GO_LP_OP;  */
	{0x1444, 0x1, 0x1, 8}, /* [8:8]: RG_VA10_GO_ON_OP;  */
	{0x1444, 0x0, 0x1, 9}, /* [9:9]: RG_VA10_GO_LP_OP;  */
	{0x1458, 0x1, 0x1, 8}, /* [8:8]: RG_VA12_GO_ON_OP;  */
	{0x1458, 0x0, 0x1, 9}, /* [9:9]: RG_VA12_GO_LP_OP;  */
	{0x146C, 0x1, 0x1, 8}, /* [8:8]: RG_VA18_GO_ON_OP;  */
	{0x146C, 0x0, 0x1, 9}, /* [9:9]: RG_VA18_GO_LP_OP;  */
	{0x147E, 0x1, 0x1, 8}, /* [8:8]: RG_VUSB33_GO_ON_OP;  */
	{0x147E, 0x0, 0x1, 9}, /* [9:9]: RG_VUSB33_GO_LP_OP;  */
	{0x1490, 0x1, 0x1, 8}, /* [8:8]: RG_VEMC_GO_ON_OP;  */
	{0x1490, 0x0, 0x1, 9}, /* [9:9]: RG_VEMC_GO_LP_OP;  */
	{0x14A4, 0x1, 0x1, 8}, /* [8:8]: RG_VXO22_GO_ON_OP;  */
	{0x14A4, 0x0, 0x1, 9}, /* [9:9]: RG_VXO22_GO_LP_OP;  */
	{0x14B6, 0x1, 0x1, 8}, /* [8:8]: RG_VEFUSE_GO_ON_OP;  */
	{0x14B6, 0x0, 0x1, 9}, /* [9:9]: RG_VEFUSE_GO_LP_OP;  */
	{0x14CA, 0x1, 0x1, 8}, /* [8:8]: RG_VSIM1_GO_ON_OP;  */
	{0x14CA, 0x0, 0x1, 9}, /* [9:9]: RG_VSIM1_GO_LP_OP;  */
	{0x14DE, 0x1, 0x1, 8}, /* [8:8]: RG_VSIM2_GO_ON_OP;  */
	{0x14DE, 0x0, 0x1, 9}, /* [9:9]: RG_VSIM2_GO_LP_OP;  */
	{0x14F2, 0x1, 0x1, 8}, /* [8:8]: RG_VCAMAF_GO_ON_OP;  */
	{0x14F2, 0x0, 0x1, 9}, /* [9:9]: RG_VCAMAF_GO_LP_OP;  */
	{0x1506, 0x1, 0x1, 8}, /* [8:8]: RG_VTOUCH_GO_ON_OP;  */
	{0x1506, 0x0, 0x1, 9}, /* [9:9]: RG_VTOUCH_GO_LP_OP;  */
	{0x151A, 0x1, 0x1, 8}, /* [8:8]: RG_VCAMD1_GO_ON_OP;  */
	{0x151A, 0x0, 0x1, 9}, /* [9:9]: RG_VCAMD1_GO_LP_OP;  */
	{0x152E, 0x1, 0x1, 8}, /* [8:8]: RG_VCAMD2_GO_ON_OP;  */
	{0x152E, 0x0, 0x1, 9}, /* [9:9]: RG_VCAMD2_GO_LP_OP;  */
	{0x1542, 0x1, 0x1, 8}, /* [8:8]: RG_VCAMIO_GO_ON_OP;  */
	{0x1542, 0x0, 0x1, 9}, /* [9:9]: RG_VCAMIO_GO_LP_OP;  */
	{0x1556, 0x1, 0x1, 8}, /* [8:8]: RG_VMIPI_GO_ON_OP;  */
	{0x1556, 0x0, 0x1, 9}, /* [9:9]: RG_VMIPI_GO_LP_OP;  */
	{0x156A, 0x1, 0x1, 8}, /* [8:8]: RG_VGP3_GO_ON_OP;  */
	{0x156A, 0x0, 0x1, 9}, /* [9:9]: RG_VGP3_GO_LP_OP;  */
	{0x157E, 0x1, 0x1, 8}, /* [8:8]: RG_VCN33_GO_ON_OP_BT;  */
	{0x157E, 0x0, 0x1, 9}, /* [9:9]: RG_VCN33_GO_LP_OP_BT;  */
	{0x158C, 0x1, 0x1, 8}, /* [8:8]: RG_VCN33_GO_ON_OP_WIFI;  */
	{0x158C, 0x0, 0x1, 9}, /* [9:9]: RG_VCN33_GO_LP_OP_WIFI;  */
	{0x15A0, 0x1, 0x1, 8}, /* [8:8]: RG_VCN18_GO_ON_OP_BT;  */
	{0x15A0, 0x0, 0x1, 9}, /* [9:9]: RG_VCN18_GO_LP_OP_BT;  */
	{0x15AE, 0x1, 0x1, 8}, /* [8:8]: RG_VCN18_GO_ON_OP_WIFI;  */
	{0x15AE, 0x0, 0x1, 9}, /* [9:9]: RG_VCN18_GO_LP_OP_WIFI;  */
	{0x15C2, 0x1, 0x1, 8}, /* [8:8]: RG_VCN28_GO_ON_OP;  */
	{0x15C2, 0x0, 0x1, 9}, /* [9:9]: RG_VCN28_GO_LP_OP;  */
	{0x15D4, 0x1, 0x1, 8}, /* [8:8]: RG_VIBR_GO_ON_OP;  */
	{0x15D4, 0x0, 0x1, 9}, /* [9:9]: RG_VIBR_GO_LP_OP;  */
	{0x15E8, 0x1, 0x1, 8}, /* [8:8]: RG_VBIF28_GO_ON_OP;  */
	{0x15E8, 0x0, 0x1, 9}, /* [9:9]: RG_VBIF28_GO_LP_OP;  */
	{0x1608, 0x1, 0x1, 8}, /* [8:8]: RG_VFE28_GO_ON_OP;  */
	{0x1608, 0x0, 0x1, 9}, /* [9:9]: RG_VFE28_GO_LP_OP;  */
	{0x161A, 0x1, 0x1, 8}, /* [8:8]: RG_VMCH_GO_ON_OP;  */
	{0x161A, 0x0, 0x1, 9}, /* [9:9]: RG_VMCH_GO_LP_OP;  */
	{0x162E, 0x1, 0x1, 8}, /* [8:8]: RG_VMC_GO_ON_OP;  */
	{0x162E, 0x0, 0x1, 9}, /* [9:9]: RG_VMC_GO_LP_OP;  */
	{0x1642, 0x1, 0x1, 8}, /* [8:8]: RG_VRF18_1_GO_ON_OP;  */
	{0x1642, 0x0, 0x1, 9}, /* [9:9]: RG_VRF18_1_GO_LP_OP;  */
	{0x1656, 0x1, 0x1, 8}, /* [8:8]: RG_VRF18_2_GO_ON_OP;  */
	{0x1656, 0x0, 0x1, 9}, /* [9:9]: RG_VRF18_2_GO_LP_OP;  */
	{0x166A, 0x1, 0x1, 8}, /* [8:8]: RG_VRF12_GO_ON_OP;  */
	{0x166A, 0x0, 0x1, 9}, /* [9:9]: RG_VRF12_GO_LP_OP;  */
	{0x169C, 0x28, 0x7F, 0}, /* [6:0]: RG_VSRAM_DVFS1_VOSEL_SLEEP; 16/6/29, Nick, 0.65V */
	{0x169E, 0x4, 0x7F, 0}, /* [6:0]: RG_VSRAM_DVFS1_SFCHG_FRATE;  */
	{0x169E, 0x4, 0x7F, 8}, /* [14:8]: RG_VSRAM_DVFS1_SFCHG_RRATE;  */
	{0x16A8, 0x1, 0x1, 8}, /* [8:8]: RG_VSRAM_DVFS1_GO_ON_OP;  */
	{0x16A8, 0x0, 0x1, 9}, /* [9:9]: RG_VSRAM_DVFS1_GO_LP_OP;  */
	{0x16BC, 0x28, 0x7F, 0}, /* [6:0]: RG_VSRAM_DVFS2_VOSEL_SLEEP; 16/6/29, Nick, 0.65V */
	{0x16BE, 0x4, 0x7F, 0}, /* [6:0]: RG_VSRAM_DVFS2_SFCHG_FRATE;  */
	{0x16BE, 0x4, 0x7F, 8}, /* [14:8]: RG_VSRAM_DVFS2_SFCHG_RRATE;  */
	{0x16C8, 0x1, 0x1, 8}, /* [8:8]: RG_VSRAM_DVFS2_GO_ON_OP;  */
	{0x16C8, 0x0, 0x1, 9}, /* [9:9]: RG_VSRAM_DVFS2_GO_LP_OP;  */
	{0x16DC, 0x28, 0x7F, 0}, /* [6:0]: RG_VSRAM_VCORE_VOSEL_SLEEP; 16/6/29, Nick, 0.65V */
	{0x16DE, 0xA, 0x7F, 0}, /* [6:0]: RG_VSRAM_VCORE_SFCHG_FRATE; 16/8/12, Nick, Slope decreased to 5.12mV/us */
	{0x16DE, 0xA, 0x7F, 8}, /* [14:8]: RG_VSRAM_VCORE_SFCHG_RRATE; 16/8/12, Nick, Slope decreased to 5.12mV/us */
	{0x16E8, 0x1, 0x1, 8}, /* [8:8]: RG_VSRAM_VCORE_GO_ON_OP;  */
	{0x16E8, 0x0, 0x1, 9}, /* [9:9]: RG_VSRAM_VCORE_GO_LP_OP;  */
	{0x16FC, 0x28, 0x7F, 0}, /* [6:0]: RG_VSRAM_VGPU_VOSEL_SLEEP; 16/6/29, Nick, 0.65V */
	{0x16FE, 0x4, 0x7F, 0}, /* [6:0]: RG_VSRAM_VGPU_SFCHG_FRATE;  */
	{0x16FE, 0x4, 0x7F, 8}, /* [14:8]: RG_VSRAM_VGPU_SFCHG_RRATE;  */
	{0x1708, 0x1, 0x1, 8}, /* [8:8]: RG_VSRAM_VGPU_GO_ON_OP;  */
	{0x1708, 0x0, 0x1, 9}, /* [9:9]: RG_VSRAM_VGPU_GO_LP_OP;  */
	{0x171C, 0x28, 0x7F, 0}, /* [6:0]: RG_VSRAM_VMD_VOSEL_SLEEP; 16/6/29, Nick, 0.65V */
	{0x171E, 0xA, 0x7F, 0}, /* [6:0]: RG_VSRAM_VMD_SFCHG_FRATE; 16/8/15, Nick, Slope decreased to 5.12mV/us */
	{0x171E, 0xA, 0x7F, 8}, /* [14:8]: RG_VSRAM_VMD_SFCHG_RRATE; 16/8/15, Nick, Slope decreased to 5.12mV/us */
	{0x1728, 0x1, 0x1, 8}, /* [8:8]: RG_VSRAM_VMD_GO_ON_OP;  */
	{0x1728, 0x0, 0x1, 9}, /* [9:9]: RG_VSRAM_VMD_GO_LP_OP;  */
	{0x1740, 0x1, 0x1, 0}, /* [0:0]: RG_DCM_MODE;  */
	{0x1742, 0x0, 0x1, 0}, /* [0:0]: RG_VIO28_CK_SW_MODE;  */
	{0x1744, 0x0, 0x1, 0}, /* [0:0]: RG_VIO18_CK_SW_MODE;  */
	{0x1746, 0x0, 0x1, 0}, /* [0:0]: RG_VUFS18_CK_SW_MODE;  */
	{0x1748, 0x0, 0x1, 0}, /* [0:0]: RG_VA10_CK_SW_MODE;  */
	{0x174A, 0x0, 0x1, 0}, /* [0:0]: RG_VA12_CK_SW_MODE;  */
	{0x174C, 0x0, 0x1, 0}, /* [0:0]: RG_VSRAM_DVFS1_CK_SW_MODE;  */
	{0x174E, 0x0, 0x1, 0}, /* [0:0]: RG_VSRAM_DVFS2_CK_SW_MODE;  */
	{0x1750, 0x0, 0x1, 0}, /* [0:0]: RG_VSRAM_VCORE_CK_SW_MODE;  */
	{0x1752, 0x0, 0x1, 0}, /* [0:0]: RG_VSRAM_VGPU_CK_SW_MODE;  */
	{0x1754, 0x0, 0x1, 0}, /* [0:0]: RG_VSRAM_VMD_CK_SW_MODE;  */
	{0x1756, 0x0, 0x1, 0}, /* [0:0]: RG_VA18_CK_SW_MODE;  */
	{0x1758, 0x0, 0x1, 0}, /* [0:0]: RG_VUSB33_CK_SW_MODE;  */
	{0x175A, 0x0, 0x1, 0}, /* [0:0]: RG_VEMC_CK_SW_MODE;  */
	{0x175C, 0x0, 0x1, 0}, /* [0:0]: RG_VXO22_CK_SW_MODE;  */
	{0x175E, 0x0, 0x1, 0}, /* [0:0]: RG_VEFUSE_CK_SW_MODE;  */
	{0x1760, 0x0, 0x1, 0}, /* [0:0]: RG_VSIM1_CK_SW_MODE;  */
	{0x1762, 0x0, 0x1, 0}, /* [0:0]: RG_VSIM2_CK_SW_MODE;  */
	{0x1764, 0x0, 0x1, 0}, /* [0:0]: RG_VCAMAF_CK_SW_MODE;  */
	{0x1766, 0x0, 0x1, 0}, /* [0:0]: RG_VTOUCH_CK_SW_MODE;  */
	{0x1768, 0x0, 0x1, 0}, /* [0:0]: RG_VCAMD1_CK_SW_MODE;  */
	{0x176A, 0x0, 0x1, 0}, /* [0:0]: RG_VCAMD2_CK_SW_MODE;  */
	{0x176C, 0x0, 0x1, 0}, /* [0:0]: RG_VCAMIO_CK_SW_MODE;  */
	{0x176E, 0x0, 0x1, 0}, /* [0:0]: RG_VMIPI_CK_SW_MODE;  */
	{0x1770, 0x0, 0x1, 0}, /* [0:0]: RG_VGP3_CK_SW_MODE;  */
	{0x1772, 0x0, 0x1, 0}, /* [0:0]: RG_VCN33_CK_SW_MODE;  */
	{0x1774, 0x0, 0x1, 0}, /* [0:0]: RG_VCN18_CK_SW_MODE;  */
	{0x1776, 0x0, 0x1, 0}, /* [0:0]: RG_VCN28_CK_SW_MODE;  */
	{0x1778, 0x0, 0x1, 0}, /* [0:0]: RG_VIBR_CK_SW_MODE;  */
	{0x177A, 0x0, 0x1, 0}, /* [0:0]: RG_VBIF28_CK_SW_MODE;  */
	{0x177C, 0x0, 0x1, 0}, /* [0:0]: RG_VFE28_CK_SW_MODE;  */
	{0x177E, 0x0, 0x1, 0}, /* [0:0]: RG_VMCH_CK_SW_MODE;  */
	{0x1780, 0x0, 0x1, 0}, /* [0:0]: RG_VMC_CK_SW_MODE;  */
	{0x1782, 0x0, 0x1, 0}, /* [0:0]: RG_VRF18_1_CK_SW_MODE;  */
	{0x1784, 0x0, 0x1, 0}, /* [0:0]: RG_VRF18_2_CK_SW_MODE;  */
	{0x1786, 0x0, 0x1, 0}, /* [0:0]: RG_VRF12_CK_SW_MODE;  */
	{0x1788, 0x0, 0x1, 0}, /* [0:0]: RG_VCAMA1_CK_SW_MODE;  */
	{0x178A, 0x0, 0x1, 0}, /* [0:0]: RG_VCAMA2_CK_SW_MODE;  */
	{0x183A, 0x1, 0x1, 6}, /* [6:6]: RG_VSRAM_DVFS1_STB_SEL; 7/12, Moris, to improve low power performance. Reuse original bits and Ignore the description. */
	{0x183A, 0x1, 0x1, 7}, /* [7:7]: RG_VSRAM_DVFS1_PLCUR_EN; 5/19. Default value ECO to 1'h0, and set to 1'h1 by PMIC init. */
	{0x183C, 0x1, 0x1, 6}, /* [6:6]: RG_VSRAM_DVFS2_STB_SEL; 7/12, Moris, to improve low power performance. Reuse original bits and Ignore the description. */
	{0x183C, 0x1, 0x1, 7}, /* [7:7]: RG_VSRAM_DVFS2_PLCUR_EN; 5/19. Default value ECO to 1'h0, and set to 1'h1 by PMIC init. */
	{0x183E, 0x1, 0x1, 6}, /* [6:6]: RG_VSRAM_VCORE_STB_SEL; 7/12, Moris, to improve low power performance. Reuse original bits and Ignore the description. */
	{0x183E, 0x1, 0x1, 7}, /* [7:7]: RG_VSRAM_VCORE_PLCUR_EN; 5/19. Default value ECO to 1'h0, and set to 1'h1 by PMIC init. */
	{0x1840, 0x1, 0x1, 6}, /* [6:6]: RG_VSRAM_VGPU_STB_SEL; 7/12, Moris, to improve low power performance. Reuse original bits and Ignore the description. */
	{0x1840, 0x1, 0x1, 7}, /* [7:7]: RG_VSRAM_VGPU_PLCUR_EN; 5/19. Default value ECO to 1'h0, and set to 1'h1 by PMIC init. */
	{0x1842, 0x1, 0x1, 6}, /* [6:6]: RG_VSRAM_VMD_STB_SEL; 7/12, Moris, to improve low power performance. Reuse original bits and Ignore the description. */
	{0x1842, 0x1, 0x1, 7}, /* [7:7]: RG_VSRAM_VMD_PLCUR_EN; 5/19. Default value ECO to 1'h0, and set to 1'h1 by PMIC init. */
	{0x1862, 0x1, 0x1, 15}, /* [15:15]: RG_VIO18_FT_DNMC_EN; */
	{0x24AC, 0x0, 0x1, 15}, /* [15:15]: AUXADC_CK_AON; From ZF's golden setting 20160129 */
	{0x24B4, 0x83, 0xFFF, 0}, /* [11:0]: AUXADC_AVG_NUM_SEL; 20160129 by wlchen */
	{0x24B4, 0x1, 0x1, 15}, /* [15:15]: AUXADC_AVG_NUM_SEL_WAKEUP; 20160129 by wlchen */
	{0x24B6, 0x20, 0x3FF, 0}, /* [9:0]: AUXADC_SPL_NUM_LARGE; 3/7, Wei-Lin, to fix BATID low V accuracy */
	{0x24BA, 0x1, 0x1, 14}, /* [14:14]: AUXADC_SPL_NUM_SEL_BAT_TEMP; 3/7, Wei-Lin, to fix BATID low V accuracy */
	{0x24BC, 0x7, 0x3FF, 0}, /* [9:0]: AUXADC_SPL_NUM_CH0; 20160129 by Wei-Lin */
	{0x24BE, 0x2B, 0x3FF, 0}, /* [9:0]: AUXADC_SPL_NUM_CH3; 20160707 by Wei-Lin */
	{0x24C0, 0x7, 0x3FF, 0}, /* [9:0]: AUXADC_SPL_NUM_CH7; 20160129 by Wei-Lin */
	{0x24C2, 0x6, 0x7, 12}, /* [14:12]: AUXADC_AVG_NUM_CH0; 20160129 by wlchen */
	{0x24C4, 0x1, 0x3, 4}, /* [5:4]: AUXADC_TRIM_CH2_SEL; 20160129 by wlchen */
	{0x24C4, 0x3, 0x3, 6}, /* [7:6]: AUXADC_TRIM_CH3_SEL; 20160129 by wlchen */
	{0x24C4, 0x1, 0x3, 8}, /* [9:8]: AUXADC_TRIM_CH4_SEL; 20160129 by wlchen */
	{0x24C4, 0x2, 0x3, 14}, /* [15:14]: AUXADC_TRIM_CH7_SEL; 20160129 by wlchen */
	{0x24C6, 0x3, 0x3, 6}, /* [7:6]: AUXADC_TRIM_CH11_SEL; 20160129 by wlchen */
	{0x24D8, 0x1, 0x1, 14}, /* [14:14]: AUXADC_START_SHADE_EN; 20160129 by wlchen */
	{0x24DC, 0x0, 0x3, 2}, /* [3:2]: AUXADC_CH0_DATA_REUSE_SEL; 2/26, ZF */
	{0x24DC, 0x1, 0x1, 9}, /* [9:9]: AUXADC_CH0_DATA_REUSE_EN; From ZF's golden setting 20160129 */
	{0x2534, 0x40, 0x3FF, 0}, /* [9:0]: AUXADC_MDRT_DET_PRD; From ZF's golden setting 20160129 */
	{0x2534, 0x1, 0x1, 15}, /* [15:15]: AUXADC_MDRT_DET_EN; From ZF's golden setting 20160129 */
	{0x2538, 0x1, 0x1, 2}, /* [2:2]: AUXADC_MDRT_DET_WKUP_EN; From ZF's golden setting 20160129 */
	{0x253C, 0x1, 0x1, 0}, /* [0:0]: AUXADC_MDRT_DET_START_SEL; From L1's request for AUXADC,My Tu,4/25 */
};

void PMIC_INIT_SETTING(void)
{
	U32 ret = 0;
	U16 i;

	print("[PMIC]INIT_SETTING\n");
#if INIT_SETTING_VERIFIED
#if ARRAY_INIT_SETTING
	/* execute pmic init setting with array */
	pmic_config_setting();
#else
	/* execute pmic init setting individually */
	for (i = 0; i < ARRAY_SIZE(init_setting); i++)
		ret = pmic_config_interface(init_setting[i].addr, init_setting[i].val, init_setting[i].mask, init_setting[i].shift);
#endif
	print("[PMIC]INIT_SETTING Done at 2016-10-24\n");
#endif	/* INIT_SETTING_VERIFIED */

}

void pmic_disable_usbdl_wo_battery(void)
{
	print("[PMIC]disable usbdl wo battery\n");

	/* ask shin-shyu programming guide TBD */
	/*pmic_config_interface(PMIC_RG_ULC_DET_EN_ADDR,1,PMIC_RG_ULC_DET_EN_MASK,PMIC_RG_ULC_DET_EN_SHIFT);*//*---MT6336 TBD---*/

	pmic_config_interface(PMIC_RG_USBDL_SET_ADDR, 0x0000, PMIC_RG_USBDL_SET_MASK, PMIC_RG_USBDL_SET_SHIFT);
	pmic_config_interface(PMIC_RG_USBDL_RST_ADDR, 0x0001, PMIC_RG_USBDL_RST_MASK, PMIC_RG_USBDL_RST_SHIFT);
}

/******************************************************************************************************
* AUXADC
******************************************************************************************************/
struct mtk_auxadc_t mtk_auxadc[] = {
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH0_BY_AP_ADDR, PMIC_AUXADC_ADC_OUT_CH0_BY_AP_ADDR, PMIC_AUXADC_RQST_CH0_ADDR, 0, 15, 3),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH2_ADDR, PMIC_AUXADC_ADC_OUT_CH2_ADDR, PMIC_AUXADC_RQST_CH2_ADDR, 2, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH3_ADDR, PMIC_AUXADC_ADC_OUT_CH3_ADDR, PMIC_AUXADC_RQST_CH3_ADDR, 3, 12, 2),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_BATID_ADDR, PMIC_AUXADC_ADC_OUT_BATID_ADDR, PMIC_AUXADC_RQST_BATID_ADDR, 3, 12, 2),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH11_ADDR, PMIC_AUXADC_ADC_OUT_CH11_ADDR, PMIC_AUXADC_RQST_CH11_ADDR, 11, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH4_ADDR, PMIC_AUXADC_ADC_OUT_CH4_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH4_ADDR, PMIC_AUXADC_ADC_OUT_CH4_ADDR, PMIC_AUXADC_RQST_CH4_ADDR, 4, 12, 1),
	PMIC_AUXADC_GEN1(PMIC_AUXADC_ADC_RDY_CH7_ADDR, PMIC_AUXADC_ADC_OUT_CH7_ADDR, PMIC_AUXADC_RQST_CH7_ADDR, 7, 15, 1),
};

#if !CFG_EVB_PLATFORM || defined(MTK_EFUSE_WRITER_RESERVE_CODESIZE)
kal_int32 count_time_out = 10;
#define VOLTAGE_FULL_RANGE	1800
#define ADC_PRECISE		32768 	// 15 bits

#if 0 //remove unused function to save space of log string
void pmic_auxadc_debug(int index)
{
	print("[PMIC]auxadc_debug_none\n");
}
#endif //remove unused function to save space of log string

int pmic_get_auxadc_value(AUXADC_LIST list)
{
	int count = 0;
	signed int adc_result = 0, reg_val = 0;
	struct mtk_auxadc_t *auxadc_channel;

	if (list < AUXADC_LIST_BATADC && list > AUXADC_LIST_TSX) {
		printf("[%s] Invalid channel(%d)\n", __func__, list);
		return -1;
	}
	auxadc_channel = &mtk_auxadc[list];

	if (list == AUXADC_LIST_DCXO)
		pmic_config_interface(PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_ADDR, 1,
					PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_MASK,
					PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_SHIFT);
	if (list == AUXADC_LIST_CHIP_TEMP)
		pmic_config_interface(PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_ADDR, 0,
					PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_MASK,
					PMIC_AUXADC_DCXO_CH4_MUX_AP_SEL_SHIFT);

	pmic_set_register_value(auxadc_channel->rqst, 1);
	udelay(10);

	while (pmic_get_register_value(auxadc_channel->rdy) != 1) {
		udelay(1300);
		if ((count++) > count_time_out) {
			printf("[%s] (%d) Time out!\n", __func__, list);
			break;
		}
	}
	reg_val = pmic_get_register_value(auxadc_channel->out);

	if (auxadc_channel->resolution == 12)
		adc_result = (reg_val * auxadc_channel->r_val * VOLTAGE_FULL_RANGE) / 4096;
	else if (auxadc_channel->resolution == 15)
		adc_result = (reg_val * auxadc_channel->r_val * VOLTAGE_FULL_RANGE) / 32768;

	printf("[%s] reg_val = 0x%x, adc_result = %d\n", __func__, reg_val, adc_result);
	return adc_result;
}

#endif

/******************************************************************************************************
* Adjust BUCK voltage
******************************************************************************************************/
#define PMU_COMMAND_MAX 0xFFFF

struct mtk_bucks_t mtk_bucks_class[] = {
	PMIC_BUCK_GEN1(VCORE, PMIC_RG_BUCK_VCORE_EN_ADDR, PMIC_RG_VCORE_MODESET_ADDR,
			PMIC_RG_BUCK_VCORE_VOSEL_ADDR, PMIC_DA_QI_VCORE_EN_ADDR, PMIC_DA_NI_VCORE_VOSEL_ADDR,
			400000, 1200000, 6250, 30),
	PMIC_BUCK_GEN1(VDRAM, PMIC_RG_BUCK_VDRAM_EN_ADDR, PMIC_RG_VDRAM_MODESET_ADDR,
			PMIC_RG_BUCK_VDRAM_VOSEL_ADDR, PMIC_DA_QI_VDRAM_EN_ADDR, PMIC_DA_NI_VDRAM_VOSEL_ADDR,
			600000, 1170000, 6250, 30),
	PMIC_BUCK_GEN1(VMODEM, PMIC_RG_BUCK_VMODEM_EN_ADDR, PMIC_RG_VMODEM_MODESET_ADDR,
			PMIC_RG_BUCK_VMODEM_VOSEL_ADDR, PMIC_DA_QI_VMODEM_EN_ADDR, PMIC_DA_NI_VMODEM_VOSEL_ADDR,
			400000, 1200000, 6250, 30),
	PMIC_BUCK_GEN1(VMD1, PMIC_RG_BUCK_VMD1_EN_ADDR, PMIC_RG_VMD1_MODESET_ADDR,
			PMIC_RG_BUCK_VMD1_VOSEL_ADDR, PMIC_DA_QI_VMD1_EN_ADDR, PMIC_DA_NI_VMD1_VOSEL_ADDR,
			400000, 1200000, 6250, 30),
	PMIC_BUCK_GEN1(VS1, PMIC_RG_BUCK_VS1_EN_ADDR, PMIC_RG_VS1_MODESET_ADDR,
			PMIC_RG_BUCK_VS1_VOSEL_ADDR, PMIC_DA_QI_VS1_EN_ADDR, PMIC_DA_NI_VS1_VOSEL_ADDR,
			1200000, 2050000, 6250, 30),
	PMIC_BUCK_GEN1(VS2, PMIC_RG_BUCK_VS2_EN_ADDR, PMIC_RG_VS2_MODESET_ADDR,
			PMIC_RG_BUCK_VS2_VOSEL_ADDR, PMIC_DA_QI_VS2_EN_ADDR, PMIC_DA_NI_VS2_VOSEL_ADDR,
			1200000, 1600000, 6250, 30),
	PMIC_BUCK_GEN1(VPA1, PMIC_RG_BUCK_VPA1_EN_ADDR, PMIC_RG_VPA1_MODESET_ADDR,
			PMIC_RG_BUCK_VPA1_VOSEL_ADDR, PMIC_DA_QI_VPA1_EN_ADDR, PMIC_DA_NI_VPA1_VOSEL_ADDR,
			500000, 1393750, 50000, 100),
	PMIC_BUCK_GEN1(VIMVO, PMIC_RG_BUCK_VIMVO_EN_ADDR, PMIC_RG_VIMVO_MODESET_ADDR,
			PMIC_RG_BUCK_VIMVO_VOSEL_ADDR, PMIC_DA_QI_VIMVO_EN_ADDR, PMIC_DA_NI_VIMVO_VOSEL_ADDR,
			400000, 1193750, 6250, 30),
	PMIC_BUCK_GEN1(VSRAM_DVFS1, PMIC_RG_VSRAM_DVFS1_SW_EN_ADDR, PMU_COMMAND_MAX,
			PMIC_RG_VSRAM_DVFS1_VOSEL_ADDR, PMIC_DA_QI_VSRAM_DVFS1_EN_ADDR, PMIC_DA_NI_VSRAM_DVFS1_VOSEL_ADDR,
			400000, 1200000, 6250, 30),
	PMIC_BUCK_GEN1(VSRAM_DVFS2, PMIC_RG_VSRAM_DVFS2_SW_EN_ADDR, PMU_COMMAND_MAX,
			PMIC_RG_VSRAM_DVFS2_VOSEL_ADDR, PMIC_DA_QI_VSRAM_DVFS2_EN_ADDR, PMIC_DA_NI_VSRAM_DVFS2_VOSEL_ADDR,
			400000, 1200000, 6250, 30),
	PMIC_BUCK_GEN1(VSRAM_VGPU, PMIC_RG_VSRAM_VGPU_SW_EN_ADDR, PMU_COMMAND_MAX,
			PMIC_RG_VSRAM_VGPU_VOSEL_ADDR, PMIC_DA_QI_VSRAM_VGPU_EN_ADDR, PMIC_DA_NI_VSRAM_VGPU_VOSEL_ADDR,
			400000, 1200000, 6250, 30),
	PMIC_BUCK_GEN1(VSRAM_VCORE, PMIC_RG_VSRAM_VCORE_SW_EN_ADDR, PMU_COMMAND_MAX,
			PMIC_RG_VSRAM_VCORE_VOSEL_ADDR, PMIC_DA_QI_VSRAM_VCORE_EN_ADDR, PMIC_DA_NI_VSRAM_VCORE_VOSEL_ADDR,
			400000, 1200000, 6250, 30),
	PMIC_BUCK_GEN1(VSRAM_VMD, PMIC_RG_VSRAM_VMD_SW_EN_ADDR, PMU_COMMAND_MAX,
			PMIC_RG_VSRAM_VMD_VOSEL_ADDR, PMIC_DA_QI_VSRAM_VMD_EN_ADDR, PMIC_DA_NI_VSRAM_VMD_VOSEL_ADDR,
			400000, 1200000, 6250, 30),
};

static unsigned int mtk_bucks_size = ARRAY_SIZE(mtk_bucks_class);

int buck_is_enabled(BUCK_TYPE type)
{
	if (type >= mtk_bucks_size) {
		print("[PMIC]Wrong buck type\n");
		return -1;
	}

	return pmic_get_register_value(mtk_bucks_class[type].da_qi_en);
}
/* en = 1 enable */
/* en = 0 disable */
int buck_enable(BUCK_TYPE type, unsigned char en)
{
	if (type >= mtk_bucks_size) {
		print("[PMIC]Wrong buck type for setting on-off\n");
		return -1;
	}

	if (type == VS1 || type == VS2) {
		if (!en) {
			print("[PMIC]VS1/VS2 can't off\n");
			return -1;
		}
	}

	if (en > 1) {
		print("[PMIC]Set Wrong en (en > 1)!! only 0 or 1\n");
		return -1;
	}

	if (en) {
		/* the setting to prevent from overshooting when enable */
		switch (type) {
		case VMD1:
			pmic_config_interface(0x0F9C, 0x1, 0x1, 12);    /* 0x0F9C[12] = 1 */
			break;
		case VMODEM:
			pmic_config_interface(0x0F88, 0x1, 0x1, 12);    /* 0x0F88[12] = 1 */
			break;
		case VIMVO:
			pmic_config_interface(0x0FE4, 0x1, 0x1, 12);    /* 0x0FE4[12] = 1 */
			break;
		default:
			break;
		}
		pmic_set_register_value(mtk_bucks_class[type].en, en);
		udelay(220);
		switch (type) {
		case VMD1:
			pmic_config_interface(0x0F9C, 0x0, 0x1, 12);    /* 0x0F9C[12] = 0 */
			break;
		case VMODEM:
			pmic_config_interface(0x0F88, 0x0, 0x1, 12);    /* 0x0F88[12] = 0 */
			break;
		case VIMVO:
			pmic_config_interface(0x0FE4, 0x0, 0x1, 12);    /* 0x0FE4[12] = 0 */
			break;
		default:
			break;
		}
	} else {
		pmic_set_register_value(mtk_bucks_class[type].en, en);
		udelay(220);
	}

	/*---Make sure BUCK <NAME> ON before setting---*/
	if (pmic_get_register_value(mtk_bucks_class[type].da_qi_en) == en)
		print("[PMIC]Set %s Votage on-off:%d success\n", mtk_bucks_class[type].name, en);
	else
		print("[PMIC]Set %s Votage on-off:%d fail\n", mtk_bucks_class[type].name, en);

	/*return pmic_get_register_value(mtk_bucks_class[type].da_qi_en);*/
	return buck_is_enabled(type);
}

/* pmode = 1 force PWM mode */
/* pmode = 0 auto mode */
int buck_set_mode(BUCK_TYPE type, unsigned char pmode)
{
	if (type >= mtk_bucks_size) {
		print("[PMIC]Wrong buck type for setting Mode\n");
		return -1;
	}

	if (mtk_bucks_class[type].mode.addr == PMU_COMMAND_MAX) {
		print("[PMIC]Can't set mode for %s\n",
			mtk_bucks_class[type].name);
		return -1;
	}

	if (pmode > 1) {
		print("[PMIC]Set Wrong mode (mode > 1)!! only 0 or 1\n");
		return -1;
	}

	if (type == VCORE) {
		if (pmode == 1) {
			pmic_config_interface(PMIC_RG_VCORE_CCSEL1_ADDR, 0x2,
				PMIC_RG_VCORE_CCSEL1_MASK, PMIC_RG_VCORE_CCSEL1_SHIFT);
			pmic_config_interface(PMIC_RG_VCORE_RZSEL0_ADDR, 0x0,
				PMIC_RG_VCORE_RZSEL0_MASK, PMIC_RG_VCORE_RZSEL0_SHIFT);
			pmic_config_interface(PMIC_RG_VCORE_VC_CLAMP_FEN_ADDR, 0x1,
				PMIC_RG_VCORE_VC_CLAMP_FEN_MASK, PMIC_RG_VCORE_VC_CLAMP_FEN_SHIFT);
			pmic_config_interface(PMIC_RG_VCORE_SLP_ADDR, 0x4,
				PMIC_RG_VCORE_SLP_MASK, PMIC_RG_VCORE_SLP_SHIFT);
			pmic_set_register_value(mtk_bucks_class[type].mode, pmode);
		} else {
			pmic_set_register_value(mtk_bucks_class[type].mode, pmode);
			pmic_config_interface(PMIC_RG_VCORE_CCSEL1_ADDR, 0x0,
				PMIC_RG_VCORE_CCSEL1_MASK, PMIC_RG_VCORE_CCSEL1_SHIFT);
			pmic_config_interface(PMIC_RG_VCORE_RZSEL0_ADDR, 0x4,
				PMIC_RG_VCORE_RZSEL0_MASK, PMIC_RG_VCORE_RZSEL0_SHIFT);
			pmic_config_interface(PMIC_RG_VCORE_VC_CLAMP_FEN_ADDR, 0x0,
				PMIC_RG_VCORE_VC_CLAMP_FEN_MASK, PMIC_RG_VCORE_VC_CLAMP_FEN_SHIFT);
			pmic_config_interface(PMIC_RG_VCORE_SLP_ADDR, 0x5,
				PMIC_RG_VCORE_SLP_MASK, PMIC_RG_VCORE_SLP_SHIFT);
		}
	} else if (type == VDRAM) {
		if (pmode == 1) {
			pmic_config_interface(0x0F6C, 0x1, 0x1, 4);
			pmic_config_interface(0x0F72, 0x0, 0x1, 1);
			pmic_set_register_value(mtk_bucks_class[type].mode, pmode);
		} else {
			pmic_set_register_value(mtk_bucks_class[type].mode, pmode);
			pmic_config_interface(0x0F6C, 0x0, 0x1, 4);
			pmic_config_interface(0x0F72, 0x1, 0x1, 1);
		}
	} else
		pmic_set_register_value(mtk_bucks_class[type].mode, pmode);

	if( pmic_get_register_value(mtk_bucks_class[type].mode) == pmode)
		print("[PMIC]Set %s Mode to %d pass\n", mtk_bucks_class[type].name, pmode);
	else
		print("[PMIC]Set %s Mode to %d fail\n", mtk_bucks_class[type].name, pmode);

	return pmic_get_register_value(mtk_bucks_class[type].mode);
}

int buck_set_voltage(BUCK_TYPE type, unsigned int voltage)
{
	unsigned short value = 0;

	if (type >= mtk_bucks_size) {
		print("[PMIC]Wrong buck type for setting voltage\n");
		return -1;
	}

	if (type >= VSRAM_DVFS1)
		mtk_bucks_class[type].da_ni_vosel.shift = 8;
	else
		mtk_bucks_class[type].da_ni_vosel.shift = 0;

	if (voltage > mtk_bucks_class[type].max_uV || voltage < mtk_bucks_class[type].min_uV) {
		print("[PMIC]Set Wrong buck voltage, range (%duV - %duV), \n",
			mtk_bucks_class[type].min_uV, mtk_bucks_class[type].max_uV);
		return -1;
	}

	if (type == VS1 || type == VS2) {
		voltage >>= 1 ;
		value = (voltage - (mtk_bucks_class[type].min_uV >> 1)) / (mtk_bucks_class[type].uV_step);
	} else
		value = (voltage - mtk_bucks_class[type].min_uV) / (mtk_bucks_class[type].uV_step);

	print("[PMIC]%s Expected volt step: 0x%x\n", mtk_bucks_class[type].name, value);

	/*---Make sure BUCK <NAME> ON before setting---*/
	if (pmic_get_register_value(mtk_bucks_class[type].da_qi_en)) {
		pmic_set_register_value(mtk_bucks_class[type].vosel, value);
		udelay(220);
		if( pmic_get_register_value(mtk_bucks_class[type].da_ni_vosel) == value)
			print("[PMIC]Set %s Voltage to %d pass\n", mtk_bucks_class[type].name, value);
		else
			print("[PMIC]Set %s Voltage to %d fail\n", mtk_bucks_class[type].name, value);
	} else
		print("[PMIC]Set %s Votage to %d fail, due to buck non-enable\n", mtk_bucks_class[type].name ,value);

	return pmic_get_register_value(mtk_bucks_class[type].da_ni_vosel);
}

unsigned int buck_get_voltage(BUCK_TYPE type)
{
	unsigned short value = 0;
	unsigned int voltage = 0;

	if (type >= mtk_bucks_size) {
		print("[PMIC]Wrong buck type for getting voltage\n");
		return 1;
	}

	if (type >= VSRAM_DVFS1)
		mtk_bucks_class[type].da_ni_vosel.shift = 8;
	else
		mtk_bucks_class[type].da_ni_vosel.shift = 0;

	value = pmic_get_register_value(mtk_bucks_class[type].da_ni_vosel);

	if (type == VS1 || type == VS2) {
		voltage = ((value * (mtk_bucks_class[type].uV_step)) + (mtk_bucks_class[type].min_uV >> 1));
		voltage <<= 1 ;
	} else
		voltage = ((value * (mtk_bucks_class[type].uV_step)) + mtk_bucks_class[type].min_uV);

	if (voltage > mtk_bucks_class[type].max_uV || voltage < mtk_bucks_class[type].min_uV) {
		print("[PMIC]Get Wrong buck voltage, range (%d - %d)\n",
			mtk_bucks_class[type].min_uV, mtk_bucks_class[type].max_uV);
		return 1;
	}

	return voltage;
}

void PMIC_POWER_HOLD(unsigned int hold)
{
	if (hold > 1) {
		print("[PMIC]POWER_HOLD hold = %d only 0 or 1\n", hold);
		return -1;
	}

	if (hold)
		print("[PMIC]POWER_HOLD ON\n");
	else
		print("[PMIC]POWER_HOLD OFF\n");
	/* MT6335 must keep power hold */
	pmic_config_interface(PMIC_RG_PWRHOLD_ADDR, hold, PMIC_RG_PWRHOLD_MASK, PMIC_RG_PWRHOLD_SHIFT);

	print("[PMIC]MT6335 PowerHold = 0x%x\n", upmu_get_reg_value(MT6335_PPCCTL0));
}

#if 0 //remove empty function to save space
int PMIC_AUXADC_INIT_SETTING_V1(void)
{
	print("[PMIC]AUXADC_INIT_SETTING_V1\n");
	return 0;
}

int PMIC_MD_INIT_SETTING_V1(void)
{
	unsigned int ret = 0;
	unsigned int pmic_reg = 0;

	print("[PMIC]MD_INIT_SETTING_V1\n");

#if PMIC_32K_LESS_DETECT_V1
	/* 32k less crystal auto detect start */
	ret |= pmic_config_interface(0x701E, 0x1, 0x1, 0);
	ret |= pmic_config_interface(0x701E, 0x3, 0xF, 1);
	ret = pmic_read_interface(0x7000, &pmic_reg, 0xffff, 0);
	ret |= pmic_config_interface(0x701E, 0x0, 0x1, 0);
	ret = pmic_config_interface(0xA04, 0x1, 0x1, 3);
	if ((pmic_reg & 0x200) == 0x200) {
		/* VCTCXO on MT6176 , OFF XO on MT6351
		 * HW control, use srclken_0
		 */

		ret = pmic_config_interface(0xA04, 0x0, 0x7, 11);
		print("[PMIC]VCTCXO on MT6176 , OFF XO on MT6335\n");
	} else {
		ret = pmic_config_interface(PMIC_RG_CLKSQ_EN_AUX_AP_MODE_ADDR,0x1,
			PMIC_RG_CLKSQ_EN_AUX_AP_MODE_MASK,
			PMIC_RG_CLKSQ_EN_AUX_AP_MODE_SHIFT);
		/*  HW control, use srclken_1, for LP */
		ret = pmic_config_interface(0xA04, 0x1, 0x1, 4);
		ret = pmic_config_interface(0xA04, 0x1, 0x7, 11);
		print("[PMIC]VCTCXO TSX on MT6335 0x7000=0x%x\n", pmic_reg);
	}
#endif
	return ret;
}
#endif //remove empty function to save space

/*
 * SPECIAL voltage config for bring up, provided by KL Tong(Low Power DE)
 */
static void boot_voltage_config(U8 config)
{
	U32 vio18_cal, va12_cal;
	U32 vcore_vol, vimvo_vol, vsram_vcore_vol;
	U32 vmd1_vol, vmodem_vol, vsram_vmd_vol;

	print("[%s]config num=%d\n", __func__, config);
	switch (config) {
	case 0:
	/* PMIC default */
		return;
	case 1:
	/* For DVT/bring up, if process issue */
		/* VIO18 +30 mV */
		pmic_read_interface(PMIC_RG_VIO18_CAL_ADDR, &vio18_cal,
			PMIC_RG_VIO18_CAL_MASK, PMIC_RG_VIO18_CAL_SHIFT);
		vio18_cal = (vio18_cal - 0x3) & 0xF;
		pmic_config_interface(PMIC_RG_VIO18_CAL_ADDR, vio18_cal,
			PMIC_RG_VIO18_CAL_MASK, PMIC_RG_VIO18_CAL_SHIFT);
		/* VA12 +(20/3)*4mV */
		pmic_read_interface(PMIC_RG_VA12_CAL_ADDR, &va12_cal,
			PMIC_RG_VA12_CAL_MASK, PMIC_RG_VA12_CAL_SHIFT);
		va12_cal = (va12_cal - 0x4) & 0xF;
		pmic_config_interface(PMIC_RG_VA12_CAL_ADDR, va12_cal,
			PMIC_RG_VA12_CAL_MASK, PMIC_RG_VA12_CAL_SHIFT);
		/* VA10 = 0.9V */
		pmic_config_interface(PMIC_RG_VA10_VOSEL_ADDR, 0x3,
			PMIC_RG_VA10_VOSEL_MASK, PMIC_RG_VA10_VOSEL_SHIFT);
		buck_set_voltage(VS2, 1412500);
		vcore_vol = 900000;
		vimvo_vol = 900000;
		vsram_vcore_vol = 1000000;
#if 0
		vmd1_vol = 850000;
		vmodem_vol = 900000;
		vsram_vmd_vol = 1000000;
#else /* MD request new setting */
		vmd1_vol = 800000;
		vmodem_vol = 800000;
		vsram_vmd_vol = 900000;
#endif
		break;
	case 2:
	/* For Android load, if process issue */
		/* VIO18 +30 mV */
		pmic_read_interface(PMIC_RG_VIO18_CAL_ADDR, &vio18_cal,
			PMIC_RG_VIO18_CAL_MASK, PMIC_RG_VIO18_CAL_SHIFT);
		vio18_cal = (vio18_cal - 0x3) & 0xF;
		pmic_config_interface(PMIC_RG_VIO18_CAL_ADDR, vio18_cal,
			PMIC_RG_VIO18_CAL_MASK, PMIC_RG_VIO18_CAL_SHIFT);
		/* VA12 +(20/3)*4mV */
		pmic_read_interface(PMIC_RG_VA12_CAL_ADDR, &va12_cal,
			PMIC_RG_VA12_CAL_MASK, PMIC_RG_VA12_CAL_SHIFT);
		va12_cal = (va12_cal - 0x4) & 0xF;
		pmic_config_interface(PMIC_RG_VA12_CAL_ADDR, va12_cal,
			PMIC_RG_VA12_CAL_MASK, PMIC_RG_VA12_CAL_SHIFT);
		/* VA10 = 0.9V */
		pmic_config_interface(PMIC_RG_VA10_VOSEL_ADDR, 0x3,
			PMIC_RG_VA10_VOSEL_MASK, PMIC_RG_VA10_VOSEL_SHIFT);
		buck_set_voltage(VS2, 1412500);
		vcore_vol = 800000;
		vimvo_vol = 800000;
		vsram_vcore_vol = 900000;
		vmd1_vol = 850000;
		vmodem_vol = 900000;
		vsram_vmd_vol = 1000000;
		break;
	case 3:
	/* For DVT/bring up, if NO process issue */
		vcore_vol = 850000;
		vimvo_vol = 850000;
		vsram_vcore_vol = 950000;
		vmd1_vol = 800000;
		vmodem_vol = 850000;
		vsram_vmd_vol = 950000;
		break;
	default:
		print("[%s]error config num=%d\n", __func__, config);
		return;
	}
	buck_set_voltage(VCORE, vcore_vol);
	buck_set_voltage(VIMVO, vimvo_vol);
	buck_set_voltage(VSRAM_VCORE, vsram_vcore_vol);
	buck_set_voltage(VMD1, vmd1_vol);
	buck_set_voltage(VMODEM, vmodem_vol);
	buck_set_voltage(VSRAM_VMD, vsram_vmd_vol);
}

/*
 * PMIC Export API
 */
g_sts_crst;

/* check if PMIC cold reset at previous power off */
U16 is_pmic_cold_reset(void)
{
	return g_sts_crst;
}


//==============================================================================
// PMIC Init Code
//==============================================================================
U32 pmic_init (void)
{
	U32 ret_code = PMIC_TEST_PASS;
	U32 poff_sts = 0;
	U16 pwhold = 0;
	int ret_val = 0, val;

	print("[PMIC]Preloader Start\n");

	print("[PMIC]MT6335 CHIP Code = 0x%x\n", get_PMIC_chip_version());

	/* MT6335 keep power hold value for gauge*/
	pwhold = upmu_get_reg_value(MT6335_PPCCTL0);
	print("[PMIC]MT6335 PPCCTL = 0x%x\n", pwhold);
	pwhold &= 0x1;
	rtc_rdwr_pwrhold_bits(&pwhold);

#if DEBUGSTS
	/*--pwhold must set to 1 to update debug status--*/
		PMIC_POWER_HOLD(1);
	/*--UVLO off--*/
	print("[PMIC]TOP_RST_STATUS[0x%x]=0x%x\n",
		MT6335_TOP_RST_STATUS, upmu_get_reg_value(MT6335_TOP_RST_STATUS));
	print("[PMIC]PONSTS[0x%x]=0x%x\n",
		MT6335_PONSTS, upmu_get_reg_value(MT6335_PONSTS));
	poff_sts = upmu_get_reg_value(MT6335_POFFSTS);
	g_sts_crst = (poff_sts >> PMIC_STS_CRST_SHIFT) & PMIC_STS_CRST_MASK;
	print("[PMIC]POFFSTS[0x%x]=0x%x\n",
		MT6335_POFFSTS, poff_sts);
	/*power not good*/
	print("[PMIC]PGSTATUS0[0x%x]=0x%x\n",
		MT6335_PGSTATUS0, upmu_get_reg_value(MT6335_PGSTATUS0));
	print("[PMIC]PGSTATUS1[0x%x]=0x%x\n",
		MT6335_PGSTATUS1, upmu_get_reg_value(MT6335_PGSTATUS1));
	/*--buck oc--*/
	print("[PMIC]PSOCSTATUS[0x%x]=0x%x\n",
		MT6335_PSOCSTATUS, upmu_get_reg_value(MT6335_PSOCSTATUS));
	print("[PMIC]BUCK_OC_SDN_STATUS[0x%x]=0x%x\n",
		MT6335_BUCK_OC_CON0, upmu_get_reg_value(MT6335_BUCK_OC_CON0));
	print("[PMIC]BUCK_OC_SDN_EN[0x%x]=0x%x\n",
		MT6335_BUCK_OC_CON1, upmu_get_reg_value(MT6335_BUCK_OC_CON1));
	/*--thermal shutdown 150--*/
	print("[PMIC]THERMALSTATUS[0x%x]=0x%x\n",
		MT6335_THERMALSTATUS, upmu_get_reg_value(MT6335_THERMALSTATUS));
	/*--long press shutdown--*/
	print("[PMIC]STRUP_CON4[0x%x]=0x%x\n",
		MT6335_STRUP_CON4, upmu_get_reg_value(MT6335_STRUP_CON4));
	/*--WDTRST--*/
	print("[PMIC]TOP_RST_MISC[0x%x]=0x%x\n",
		MT6335_TOP_RST_MISC, upmu_get_reg_value(MT6335_TOP_RST_MISC));
	/*--CLK TRIM--*/
	print("[PMIC]TOP_CLK_TRIM[0x%x]=0x%x\n",
		MT6335_TOP_CLK_TRIM, upmu_get_reg_value(MT6335_TOP_CLK_TRIM));

	/*--Write 1 to clear PONSTS and POFFSTS--*/
	ret_val = pmic_config_interface(PMIC_RG_POFFSTS_CLR_ADDR, 0x1,
		PMIC_RG_POFFSTS_CLR_MASK, PMIC_RG_POFFSTS_CLR_SHIFT);/*--clear PONSTS--*/

	ret_val = pmic_config_interface(PMIC_RG_PONSTS_CLR_ADDR, 0x1,
		PMIC_RG_PONSTS_CLR_MASK, PMIC_RG_PONSTS_CLR_SHIFT);/*--clear POFFSTS--*/

	ret_val = pmic_config_interface(MT6335_BUCK_OC_CON0, 0xFF, 0xFF, 0);/*--clear OC_SDN_STATUS--*/
#endif /*--#if DEBUGSTS--*/

	#if 0 //remove empty function to save space
	/* for auxadc init */
	PMIC_AUXADC_INIT_SETTING_V1();
	/*pmic_auxadc_debug(1);*/
	PMIC_MD_INIT_SETTING_V1();
	#endif //remove empty function to save space
	/* detect V battery Drop   used by RTC to check power on reason  */
	/* ask waverly programming guide */
	pmic_DetectVbatDrop();

	/* ask shin-shyu programming guide TBD */
	if(hw_check_battery() == 1)
		pmic_disable_usbdl_wo_battery();

	/* move to platform.c */
	/*PMIC_INIT_SETTING();*/

	/*Enable PMIC RST function (depends on main chip RST function)*/
	/* ask peter programming guide TBD */
	ret_val = pmic_config_interface(MT6335_TOP_RST_MISC_CLR, 0x0002, 0xFFFF, 0); //[1]=0, RG_WDTRSTB_MODE
	ret_val = pmic_config_interface(MT6335_TOP_RST_MISC_SET, 0x0001, 0xFFFF, 0); //[0]=1, RG_WDTRSTB_EN
	print("[PMIC][pmic_init]Reg[0x%x]=0x%x\n",
		MT6335_TOP_RST_MISC, upmu_get_reg_value(MT6335_TOP_RST_MISC));


	/* ask peter peng programming guide TBD */
	ret_val = pmic_config_interface(PMIC_RG_SMPS_TESTMODE_B_ADDR, 0x0001,
		PMIC_RG_SMPS_TESTMODE_B_MASK, PMIC_RG_SMPS_TESTMODE_B_SHIFT); //RG_SMPS_TESTMODE_B by luke

	battery_init();

	if (hw_check_battery() == 1) {
#if !CFG_EVB_PLATFORM
		pl_check_bat_protect_status();
#endif
	}

#if DEBUGSTS
	/*--After writing 1, need to write 0 back--*/
	ret_val = pmic_config_interface(PMIC_RG_POFFSTS_CLR_ADDR, 0x0,
		PMIC_RG_POFFSTS_CLR_MASK, PMIC_RG_POFFSTS_CLR_SHIFT);

	ret_val = pmic_config_interface(PMIC_RG_PONSTS_CLR_ADDR, 0x0,
		PMIC_RG_PONSTS_CLR_MASK, PMIC_RG_PONSTS_CLR_SHIFT);
#endif /*--#if DEBUGSTS--*/

	print("[pmic_init]Done\n");

	boot_voltage_config(CFG_BOOT_VOL);

#if defined(EXTERNAL_BUCK_RT5738)
	rt5738_driver_probe();
#endif /* EXTERNAL_BUCK_RT5738 */

	return ret_code;
}

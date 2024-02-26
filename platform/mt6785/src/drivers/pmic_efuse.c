#include <typedefs.h>
#include <platform.h>
#include <pmic.h>

#define pmic_set_register_value(flagname, val) \
	pmic_config_interface(flagname##_ADDR, (val), flagname##_MASK, flagname##_SHIFT)

#define pmic_get_register_value(flagname) \
({	\
	unsigned int val;	\
	pmic_read_interface(flagname##_ADDR, &val, flagname##_MASK, flagname##_SHIFT);	\
	val;	\
})

unsigned int pmic_read_efuse_nolock(int i)
{
	unsigned int efuse_data = 0;

	/* 1. enable efuse ctrl engine clock */
	pmic_set_register_value(PMIC_TOP_CKHWEN_CON0_CLR, 1 << PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	pmic_set_register_value(PMIC_TOP_CKPDN_CON0_CLR, 1 << PMIC_RG_EFUSE_CK_PDN_SHIFT);
	/* 2. */
	pmic_set_register_value(PMIC_RG_OTP_RD_SW, 1);
	/* 3. Set row to read */
	pmic_set_register_value(PMIC_RG_OTP_PA, i * 2);
	/* 4. Toggle RG_OTP_RD_TRIG */
	if (pmic_get_register_value(PMIC_RG_OTP_RD_TRIG) == 0)
		pmic_set_register_value(PMIC_RG_OTP_RD_TRIG, 1);
	else
		pmic_set_register_value(PMIC_RG_OTP_RD_TRIG, 0);
	/* 5. Polling RG_OTP_RD_BUSY = 0 */
	udelay(300);
	while (pmic_get_register_value(PMIC_RG_OTP_RD_BUSY) == 1)
		;
	/* 6. Read RG_OTP_DOUT_SW */
	udelay(100);
	efuse_data = pmic_get_register_value(PMIC_RG_OTP_DOUT_SW);
	/* 7. disable efuse ctrl engine clock */
	pmic_set_register_value(PMIC_TOP_CKHWEN_CON0_SET, 1 << PMIC_RG_EFUSE_CK_PDN_HWEN_SHIFT);
	pmic_set_register_value(PMIC_TOP_CKPDN_CON0_SET, 1 << PMIC_RG_EFUSE_CK_PDN_SHIFT);

	return efuse_data;
}

/* PMIC EFUSE SW load need to check EFUSE_TABLE */
void pmic_efuse_sw_load(void)
{
	int VAUX18, VBG12;
	unsigned int efuse_data = 0;

	/* bit 64 ~ 79 */
	efuse_data = pmic_read_efuse_nolock(4);
	pmic_config_interface(PMIC_RG_RSV_SWREG_ADDR, efuse_data & 0x8000,
			      PMIC_RG_RSV_SWREG_MASK & 0x8000,
			      PMIC_RG_RSV_SWREG_SHIFT);
	if (efuse_data & 0x4000) {
		/* restore VPA_DLC initial setting */
		upmu_set_reg_value(MT6359_BUCK_VPA_DLC_CON0, 0x2810);
		upmu_set_reg_value(MT6359_BUCK_VPA_DLC_CON1, 0x800);
	}

	/* bit 864 ~ 879 */
	efuse_data = pmic_read_efuse_nolock(54);
	pmic_config_interface(PMIC_RG_VGPU11_PFM_PEAK_ADDR, (efuse_data >> 6) & 0xF,
			      PMIC_RG_VGPU11_PFM_PEAK_MASK,
			      PMIC_RG_VGPU11_PFM_PEAK_SHIFT);
	pmic_config_interface(PMIC_RG_VGPU11_SONIC_PFM_PEAK_ADDR, (efuse_data >> 10) & 0xF,
			      PMIC_RG_VGPU11_SONIC_PFM_PEAK_MASK,
			      PMIC_RG_VGPU11_SONIC_PFM_PEAK_SHIFT);
	/* bit 880 ~ 895 */
	efuse_data = pmic_read_efuse_nolock(55);
	pmic_config_interface(PMIC_RG_VGPU12_PFM_PEAK_ADDR, (efuse_data >> 6) & 0xF,
			      PMIC_RG_VGPU12_PFM_PEAK_MASK,
			      PMIC_RG_VGPU12_PFM_PEAK_SHIFT);
	pmic_config_interface(PMIC_RG_VGPU12_SONIC_PFM_PEAK_ADDR, (efuse_data >> 10) & 0xF,
			      PMIC_RG_VGPU12_SONIC_PFM_PEAK_MASK,
			      PMIC_RG_VGPU12_SONIC_PFM_PEAK_SHIFT);
	/* bit 896 ~ 911 */
	efuse_data = pmic_read_efuse_nolock(56);
	pmic_config_interface(PMIC_RG_VCORE_PFM_PEAK_ADDR, (efuse_data >> 6) & 0xF,
			      PMIC_RG_VCORE_PFM_PEAK_MASK,
			      PMIC_RG_VCORE_PFM_PEAK_SHIFT);
	pmic_config_interface(PMIC_RG_VCORE_SONIC_PFM_PEAK_ADDR, (efuse_data >> 10) & 0xF,
			      PMIC_RG_VCORE_SONIC_PFM_PEAK_MASK,
			      PMIC_RG_VCORE_SONIC_PFM_PEAK_SHIFT);
	/* bit 992 ~ 1007 */
	efuse_data = pmic_read_efuse_nolock(62);
	pmic_config_interface(PMIC_RG_VPROC1_PFM_PEAK_ADDR, (efuse_data >> 6) & 0xF,
			      PMIC_RG_VPROC1_PFM_PEAK_MASK,
			      PMIC_RG_VPROC1_PFM_PEAK_SHIFT);
	pmic_config_interface(PMIC_RG_VPROC1_SONIC_PFM_PEAK_ADDR, (efuse_data >> 10) & 0xF,
			      PMIC_RG_VPROC1_SONIC_PFM_PEAK_MASK,
			      PMIC_RG_VPROC1_SONIC_PFM_PEAK_SHIFT);
	/* bit 1136 ~ 1151 */
	efuse_data = pmic_read_efuse_nolock(71);
	pmic_config_interface(PMIC_RG_VPROC2_PFM_PEAK_ADDR, (efuse_data >> 6) & 0xF,
			      PMIC_RG_VPROC2_PFM_PEAK_MASK,
			      PMIC_RG_VPROC2_PFM_PEAK_SHIFT);
	pmic_config_interface(PMIC_RG_VPROC2_SONIC_PFM_PEAK_ADDR, (efuse_data >> 10) & 0xF,
			      PMIC_RG_VPROC2_SONIC_PFM_PEAK_MASK,
			      PMIC_RG_VPROC2_SONIC_PFM_PEAK_SHIFT);
	/* bit 1184 ~ 1199 */
	efuse_data = pmic_read_efuse_nolock(74);
	pmic_config_interface(PMIC_RG_VMODEM_PFM_PEAK_ADDR, (efuse_data >> 6) & 0xF,
			      PMIC_RG_VMODEM_PFM_PEAK_MASK,
			      PMIC_RG_VMODEM_PFM_PEAK_SHIFT);
	pmic_config_interface(PMIC_RG_VMODEM_SONIC_PFM_PEAK_ADDR, (efuse_data >> 10) & 0xF,
			      PMIC_RG_VMODEM_SONIC_PFM_PEAK_MASK,
			      PMIC_RG_VMODEM_SONIC_PFM_PEAK_SHIFT);
	/* bit 1232 ~ 1247 */
	efuse_data = pmic_read_efuse_nolock(77);
	pmic_config_interface(PMIC_RG_VPU_PFM_PEAK_ADDR, (efuse_data >> 6) & 0xF,
			      PMIC_RG_VPU_PFM_PEAK_MASK,
			      PMIC_RG_VPU_PFM_PEAK_SHIFT);
	pmic_config_interface(PMIC_RG_VPU_SONIC_PFM_PEAK_ADDR, (efuse_data >> 10) & 0xF,
			      PMIC_RG_VPU_SONIC_PFM_PEAK_MASK,
			      PMIC_RG_VPU_SONIC_PFM_PEAK_SHIFT);
	/* bit 1280 ~ 1295 */
	efuse_data = pmic_read_efuse_nolock(80);
	pmic_config_interface(PMIC_RG_VS1_PFM_TON_ADDR, (efuse_data >> 8) & 0x7,
			      PMIC_RG_VS1_PFM_TON_MASK,
			      PMIC_RG_VS1_PFM_TON_SHIFT);
	pmic_config_interface(PMIC_RG_VS1_SONIC_PFM_TON_ADDR, (efuse_data >> 11) & 0x7,
			      PMIC_RG_VS1_SONIC_PFM_TON_MASK,
			      PMIC_RG_VS1_SONIC_PFM_TON_SHIFT);
	/* bit 1312 ~ 1327 */
	efuse_data = pmic_read_efuse_nolock(82);
	pmic_config_interface(PMIC_RG_VS2_PFM_TON_ADDR, (efuse_data >> 8) & 0x7,
			      PMIC_RG_VS2_PFM_TON_MASK,
			      PMIC_RG_VS2_PFM_TON_SHIFT);
	pmic_config_interface(PMIC_RG_VS2_SONIC_PFM_TON_ADDR, (efuse_data >> 11) & 0x7,
			      PMIC_RG_VS2_SONIC_PFM_TON_MASK,
			      PMIC_RG_VS2_SONIC_PFM_TON_SHIFT);

	/* bit 1792 ~ 1807 */
	efuse_data = pmic_read_efuse_nolock(112);
	pmic_config_interface(PMIC_FG_RNG_EN_SW_ADDR, (efuse_data >> 8) & 0x1,
			      PMIC_FG_RNG_EN_SW_MASK,
			      PMIC_FG_RNG_EN_SW_SHIFT);
	pmic_config_interface(PMIC_RG_FGINTMODE_ADDR, (efuse_data >> 9) & 0x1,
			      PMIC_RG_FGINTMODE_MASK,
			      PMIC_RG_FGINTMODE_SHIFT);
	pmic_config_interface(PMIC_RG_FGANALOGTEST_ADDR, (efuse_data >> 10) & 0x7,
			      PMIC_RG_FGANALOGTEST_MASK,
			      PMIC_RG_FGANALOGTEST_SHIFT);

	/* bit 1808 ~ 1823 */
	efuse_data = pmic_read_efuse_nolock(113);
	pmic_config_interface(PMIC_XO_AAC_VSEL_M_ADDR, (efuse_data >> 9) & 0xF,
			      PMIC_XO_AAC_VSEL_M_MASK,
			      PMIC_XO_AAC_VSEL_M_SHIFT);

	/* dump VAUX18/VBG12 TRIM value */
	VAUX18 = pmic_get_register_value(PMIC_AUXADC_EFUSE_VAUX18);
	VBG12 = pmic_get_register_value(PMIC_AUXADC_EFUSE_VBG12);
	pal_log_info("VAUX18=%d.%dmV (0x%x), VBG12=%d.%dmV (0x%x)\n",
		(18000 + VAUX18 * 5) / 10,
		(18000 + VAUX18 * 5) % 10,
		VAUX18,
		(VBG12 >> 6) ? (120500 + (VBG12 - 0x80) * 25) / 100 : (120500 + VBG12 * 25) / 100,
		(VBG12 >> 6) ? (120500 + (VBG12 - 0x80) * 25) % 100 : (120500 + VBG12 * 25) % 100,
		VBG12);
}

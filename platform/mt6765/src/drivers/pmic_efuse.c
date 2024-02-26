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
#if 0
	/* start from i == 104 */
	i += 104;
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
#endif
	return efuse_data;
}

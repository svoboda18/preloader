#include <typedefs.h>
#include <platform.h>
#include <mt6356.h>
#include <pmic.h>

#ifdef LDO_SUPPORT
static const unsigned int vdram_volts[] = {
	1120000,
	1240000,
};

static const unsigned int vsim_volts[] = {
	1700000,
	1800000,
	2700000,
	3000000,
	3100000,
};

static const unsigned int vmc_volts[] = {
	1800000,
	2900000,
	3000000,
	3300000,
};

static const unsigned int vdram_idxs[] = {
	5, 6,
}; /*--ldo_volt_table2--*/

static const unsigned int vsim_idxs[] = {
	3, 4, 8, 11, 12,
}; /*--ldo_volt_table2--*/

static const unsigned int vmc_idxs[] = {
	4, 10, 11, 13
}; /*--ldo_volt_table2--*/

static const unsigned int fix_idxs[] = {
	0,
};

static const int votrim_1_type[] = {
	0, (-1)*20000, (-1)*40000, (-1)*60000, (-1)*80000, (-1)*100000, (-1)*120000, (-1)*140000,
	160000, 140000, 1200000, 100000, 80000, 60000, 40000, 20000,
};

static const int votrim_2_type[] = {
	0, (-1)*10000, (-1)*20000, (-1)*30000, (-1)*40000, (-1)*50000, (-1)*60000, (-1)*70000,
	80000, 70000, 600000, 50000, 40000, 30000, 20000, 10000,
};

static unsigned int mt6356_ldo_convert_data(unsigned char id, unsigned int cnvdata, volseltran transtype);

#endif /*--LDO_SUPPORT--*/

#ifndef CFG_MTK_TINYSYS_SSPM_SUPPORT
static unsigned int step_uv[4] = {6250, 12500, 50000, 0xFFFF};
static unsigned char vo_shift[3] = {0, 8, 0xFF};
static unsigned char vo_mask[4] = {0x7F, 0xF, 0x3F, 0xFF};
static unsigned char en_bit[2] = {0, 0xFF};
static unsigned char mod_bit[5] = {1, 2, 3, 8, 0xFF};
#else
#define step_uV		6250
#define vol_shift	0
#define vol_mask	0x7F
#define enable_bit	0
#define enable_mask	1
#define mode_bit	0
#define mode_mask	1
#endif

#ifdef LDO_VOTRIM_SUPPORT
static struct mt6356_ldo_trim_info ldo_trim_info[] = {
	mt6356_ldo_trim_decl(vdram, votrim_1_type),
	mt6356_ldo_trim_decl(vsim1, votrim_1_type),
	mt6356_ldo_trim_decl(vsim2, votrim_1_type),
	mt6356_ldo_trim_decl(vmc, votrim_1_type),
};
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
static struct mt6356_ldo_info ldo_ext_info[] = {
	mt6356_ldo_decl(vdram_volts, vdram_idxs),
	mt6356_ldo_decl(vsim_volts, vsim_idxs),
	mt6356_ldo_decl(vsim_volts, vsim_idxs),
	mt6356_ldo_decl(vmc_volts, vmc_idxs),
};
#endif /*--LDO_SUPPORT--*/

static struct mt6356_regulator_info mt6356_regu_info[] = {
	mt6356_decl(vs1),
	mt6356_decl(vmodem),
	mt6356_decl(vcore),
	mt6356_decl(vsram_others),
	mt6356_decl(vsram_gpu),
	mt6356_decl(vproc),
	mt6356_decl(vs2),
	mt6356_decl(vsram_proc),
	mt6356_decl(vpa),
#ifdef LDO_SUPPORT
	mt6356_decl(vdram),
	mt6356_decl(vsim1),
	mt6356_decl(vsim2),
	mt6356_decl(vmc),
#endif /*--LDO_SUPPORT--*/
};

static unsigned char regu_size = ARRAY_SIZE(mt6356_regu_info);

static int mt6356_set_voltage(unsigned char id, unsigned int volt, unsigned int max_volt) {
	unsigned short selector = 0;
	int ret = 0;

#ifndef CFG_MTK_TINYSYS_SSPM_SUPPORT
	unsigned int step_uV = step_uv[(mt6356_regu_info[id].vtype)];
	unsigned short vol_shift = vo_shift[(mt6356_regu_info[id].vspos)];
	unsigned short vol_mask = vo_mask[(mt6356_regu_info[id].vmpos)];
#endif /*--NON SSPM MODE--*/

	if (volt > mt6356_regu_info[id].max_uV || volt < mt6356_regu_info[id].min_uV) {
		mreg_dbg_print("vp\n");
		return -1;
	}

	if (mt6356_regu_info[id].rtype == REGULAR_VOLTAGE)
		selector = (volt - mt6356_regu_info[id].min_uV) / (step_uV);
#ifdef LDO_SUPPORT
	else if (mt6356_regu_info[id].rtype == NON_REGULAR_VOLTAGE) {
		selector = mt6356_ldo_convert_data(id, volt, VOLTOSEL);
		if (selector == 0) {
			/* This check condition is not good, because LDO may has 0 vosel in others project(TBD) */
			mreg_dbg_print("vnf\n"); /* voltage not found */
			return -1;
		}
	} else if (mt6356_regu_info[id].rtype == FIXED_REGULAR_VOLTAGE) {
		if (mt6356_ldo_convert_data(id, volt, VOLTOSEL) == 0)
			return 0;
		else {
			mreg_dbg_print("vswf\n");
			return -1;
		}
	}
#else
	else {
		mreg_dbg_print("ldo not support\n");
		return -1;
	}
#endif /*--LDO_SUPPORT--*/

	mreg_dbg_print("1 %d,%d\n", id, selector);

	if (id == MTK_REGULATOR_VCORE)
		/* MT6356 VCORE VOSEL workaround */
		selector--;
	else if (id == MTK_REGULATOR_VSRAM_OTHERS)
		/* MT6356 VSRAM_OTHERS VOSEL workaround */
		selector++;

	ret = pmic_config_interface(mt6356_regu_info[id].vol_reg, selector, vol_mask, vol_shift);
	return ret;
}

static int mt6356_get_voltage(unsigned char id)
{
	unsigned int selector = 0;
	unsigned int volt = 0;
	int ret = 0;

#ifndef CFG_MTK_TINYSYS_SSPM_SUPPORT
	unsigned int step_uV = step_uv[(mt6356_regu_info[id].vtype)];
	unsigned short vol_shift = vo_shift[(mt6356_regu_info[id].vspos)];
	unsigned short vol_mask = vo_mask[(mt6356_regu_info[id].vmpos)];
#endif /*--NON SSPM MODE--*/

	if (id == MTK_REGULATOR_VSRAM_OTHERS)
		ret = pmic_read_interface(PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_RO_ADDR, &selector,
			PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_RO_MASK,	PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_RO_SHIFT);
	else
		ret = pmic_read_interface(mt6356_regu_info[id].vol_reg, &selector, vol_mask, vol_shift);
	if (ret)
		return -1;

	if (id == MTK_REGULATOR_VCORE)
		/* MT6356 VCORE VOSEL workaround */
		selector++;
	else if (id == MTK_REGULATOR_VSRAM_OTHERS)
		/* MT6356 VSRAM_OTHERS VOSEL workaround */
		selector--;

	if (mt6356_regu_info[id].rtype == REGULAR_VOLTAGE)
		volt = ((selector * step_uV) + mt6356_regu_info[id].min_uV);
#ifdef LDO_SUPPORT
	else if (mt6356_regu_info[id].rtype == NON_REGULAR_VOLTAGE)
		volt = mt6356_ldo_convert_data(id, selector, SELTOVOL);
	else if (mt6356_regu_info[id].rtype == FIXED_REGULAR_VOLTAGE) {
		volt = *((int *)(mt6356_regu_info[id].extinfo->pvoltages));
	}
#else
	else
		return -1;
#endif /*--LDO_SUPPORT--*/

	mreg_dbg_print("get volt %d, %d, %d\n", id, selector, volt);
	if (volt > mt6356_regu_info[id].max_uV || volt < mt6356_regu_info[id].min_uV) {
		mreg_dbg_print("vgw\n");
		return -1;
	}

	return volt;
}

static int mt6356_enable(unsigned char id, unsigned char en)
{
	int ret = 0;

#ifndef CFG_MTK_TINYSYS_SSPM_SUPPORT
	unsigned short enable_bit = en_bit[(mt6356_regu_info[id].ebpos)];
	unsigned short enable_mask = 1;
#endif /*--NON SSPM MODE--*/

	if (mt6356_regu_info[id].ebpos != EPOS_NONE) {
		mreg_dbg_print("2 %d,%d\n", id, en);
		ret = pmic_config_interface(mt6356_regu_info[id].enable_reg, en, enable_mask, enable_bit);
	} else
		return -1;

	return ret;
}

static int mt6356_is_enabled(unsigned char id)
{
	unsigned int en = 0;
	int ret = 0;

#ifndef CFG_MTK_TINYSYS_SSPM_SUPPORT
	unsigned short enable_bit = en_bit[(mt6356_regu_info[id].ebpos)];
	unsigned short enable_mask = 1;
#endif /*--NON SSPM MODE--*/

	if (mt6356_regu_info[id].ebpos != EPOS_NONE) {
		ret = pmic_read_interface(mt6356_regu_info[id].enable_reg, &en, enable_mask, enable_bit);
		mreg_dbg_print("3 %d,%d\n", id, en);
	} else
		return -1;

	return (ret ? ret : en) ;
}

static int mt6356_set_mode(unsigned char id, unsigned char mode)
{
	int ret = 0;

#ifndef CFG_MTK_TINYSYS_SSPM_SUPPORT
	unsigned short mode_bit = mod_bit[(mt6356_regu_info[id].mbpos)];
	unsigned short mode_mask = 1;
#endif /*--NON SSPM MODE--*/

	if (mt6356_regu_info[id].mbpos != MPOS_NONE) {
		mreg_dbg_print("4 %d,%d\n", id, mode);
		ret = pmic_config_interface(mt6356_regu_info[id].mode_reg, mode, mode_mask, mode_bit);
	} else
		return -1;

	return ret;
}

static int mt6356_get_mode(unsigned char id)
{
	unsigned int mode = 0;
	int ret = 0;

#ifndef CFG_MTK_TINYSYS_SSPM_SUPPORT
	unsigned short mode_bit = mod_bit[(mt6356_regu_info[id].mbpos)];
	unsigned short mode_mask = 1;
#endif /*--NON SSPM MODE--*/

	if (mt6356_regu_info[id].mbpos != MPOS_NONE) {
		ret = pmic_read_interface(mt6356_regu_info[id].mode_reg, &mode, mode_mask, mode_bit);
		mreg_dbg_print("5 %d,%d\n", id, mode);
	} else
		return -1;

	return (ret ? ret: mode);
}

#ifdef LDO_VOTRIM_SUPPORT
static int mt6356_ldo_votrim_convert_data(unsigned char id, int cnvdata, trimseltran transtype)
{
	int i = 0, trim_size = 0, choice = -1;
	const int *trim_Voltage;

	if ((mt6356_regu_info[id].triminfo->trim_voltages != NULL)) {
		trim_Voltage = mt6356_regu_info[id].triminfo->trim_voltages;
		trim_size = mt6356_regu_info[id].triminfo->trim_size; 
		mreg_dbg_print("votrim_size %d, cnvdata %d\n", trim_size, cnvdata);
		switch (transtype) {
		case TRIMTOSEL:
			for (i = 0; i <= trim_size; i++) {
				if (trim_Voltage[i] == cnvdata) {
					choice = i;
					break;
				}
			}
			break;
		case SELTOTRIM:
			choice = trim_Voltage[cnvdata];
			break;
		default:
			break;
		}
	}

	return choice;
}

static int mt6356_set_votrim(unsigned char id, int trim_volt) {
	unsigned short selector = 0;
	int ret = 0;

	selector = mt6356_ldo_votrim_convert_data(id, trim_volt, TRIMTOSEL);
	if (selector == -1)
		return -1;

	if ((mt6356_regu_info[id].triminfo->trim_voltages != NULL)) {
		mreg_dbg_print("6 %d,%d\n", id, selector);
		ret = pmic_config_interface(mt6356_regu_info[id].triminfo->trim_reg, selector,
			mt6356_regu_info[id].triminfo->trim_mask, mt6356_regu_info[id].triminfo->trim_shift);
	}

	return ret;
}

static int mt6356_get_votrim(unsigned char id)
{
	unsigned int selector = 0;
	int ret = 0;

	ret = pmic_read_interface(mt6356_regu_info[id].triminfo->trim_reg, &selector,
		mt6356_regu_info[id].triminfo->trim_mask, mt6356_regu_info[id].triminfo->trim_shift);
	if (ret)
		return -1;

	ret = mt6356_ldo_votrim_convert_data(id, selector, SELTOTRIM);

	mreg_dbg_print("7 %d,%d,%d\n", id, selector,ret);

	return ret;
}
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
static unsigned int mt6356_ldo_convert_data(unsigned char id, unsigned int cnvdata, volseltran transtype)
{
	int i = 0, n_size = 0;
	unsigned int choice = 0;
	const unsigned int *pVoltage;
	const unsigned int *iDx;

	if ((mt6356_regu_info[id].extinfo->pvoltages != NULL) && (mt6356_regu_info[id].extinfo->idxs != NULL)) {
		pVoltage = mt6356_regu_info[id].extinfo->pvoltages;
		iDx = mt6356_regu_info[id].extinfo->idxs;
		n_size = mt6356_regu_info[id].extinfo->n_size; 
		mreg_dbg_print("n_size %d\n", n_size);
		for (i = 0; i <= n_size; i++) {
			if (transtype == VOLTOSEL && pVoltage[i] == cnvdata) {
				choice = iDx[i];
				break;
			} else if (transtype == SELTOVOL && iDx[i] == cnvdata) {
				choice = pVoltage[i];
				break;
			}
		}
	}

	return choice;
}
#endif /*--LDO_SUPPORT--*/

static struct regulator_ctrl mt6356_regulator_ctrl = {
	.set_voltage = mt6356_set_voltage,
	.get_voltage = mt6356_get_voltage,
	.enable = mt6356_enable,
	.is_enabled = mt6356_is_enabled,
	.set_mode = mt6356_set_mode,
	.get_mode = mt6356_get_mode,
#ifdef LDO_VOTRIM_SUPPORT
	.set_votrim = mt6356_set_votrim,
	.get_votrim = mt6356_get_votrim,
#endif /*--LDO_VOTRIM_SUPPORT--*/
};

static struct mtk_regulator mt6356_regulator[] = {
	{
		.name = "vs1",
		.id = MTK_REGULATOR_VS1,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vmodem",
		.id = MTK_REGULATOR_VMODEM,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vcore",
		.id = MTK_REGULATOR_VCORE,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vsram_others",
		.id = MTK_REGULATOR_VSRAM_OTHERS,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vsram_gpu",
		.id = MTK_REGULATOR_VSRAM_GPU,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vproc",
		.id = MTK_REGULATOR_VPROC,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vs2",
		.id = MTK_REGULATOR_VS1,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vsram_proc",
		.id = MTK_REGULATOR_VSRAM_PROC,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vpa",
		.id = MTK_REGULATOR_VPA,
		.reg_ops = &mt6356_regulator_ctrl,
	},
#ifdef LDO_SUPPORT
	{
		.name = "vdram",
		.id = MTK_REGULATOR_VDRAM,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vsim1",
		.id = MTK_REGULATOR_VSIM1,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vsim2",
		.id = MTK_REGULATOR_VSIM2,
		.reg_ops = &mt6356_regulator_ctrl,
	},
	{
		.name = "vmc",
		.id = MTK_REGULATOR_VMC,
		.reg_ops = &mt6356_regulator_ctrl,
	},
#endif /*--LDO_SUPPORT--*/
};

/* ====================
 * Driver operations
 * ====================
 */
int mt6356_probe(void)
{
	int ret = 0;
	unsigned int i = 0;

	for (i = 0; i < MTK_REGULATOR_MAX_NR; i++) {
#ifdef LDO_SUPPORT
		if (mt6356_regu_info[i].rtype != REGULAR_VOLTAGE && i >= MTK_REGULATOR_LDO_SUPPORT)
			mt6356_regu_info[i].extinfo = &ldo_ext_info[(i-MTK_REGULATOR_LDO_SUPPORT)];
#endif /*--LDO_SUPPORT--*/
#ifdef LDO_VOTRIM_SUPPORT
		if (mt6356_regu_info[i].rtype != REGULAR_VOLTAGE  && i >= MTK_REGULATOR_LDO_SUPPORT)
			mt6356_regu_info[i].triminfo = &ldo_trim_info[(i-MTK_REGULATOR_LDO_SUPPORT)];
#endif /*--LDO_SUPPORT--*/
		ret = mtk_simple_regulator_register(&mt6356_regulator[i]);
		if (ret < 0) {
			/* register mtk regulator error */
			mreg_dbg_print("mt6356 regulator, %s\n", mt6356_regulator[i].name);
			return ret;
		}
	}

	return 0;
}

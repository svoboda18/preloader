#include <typedefs.h>
#include <platform.h>
#include <pmic.h>
#include <regulator/mtk_regulator.h>
#include <mt6359.h>
#include <dconfig_env.h>

/*
 * CONFIG OPTION SET
 */
/* Enable this option when pmic initial setting is verified */
#define INIT_SETTING_VERIFIED	1

/* Enable this option when pmic need efuse sw load */
/* if enable, please also check pmic_efuse.c or pmic_efuse_xxxx.c */
#define EFUSE_SW_LOAD		1


static void wk_vs2_voter_setting()
{
	/* clear VS2_VOTER, VS2_VOSEL = 1.4V */
	pmic_config_interface(PMIC_RG_BUCK_VS2_VOTER_EN_ADDR, 0,
		PMIC_RG_BUCK_VS2_VOTER_EN_MASK, PMIC_RG_BUCK_VS2_VOTER_EN_SHIFT);
	pmic_config_interface(PMIC_RG_BUCK_VS2_VOSEL_ADDR, 0x30,
		PMIC_RG_BUCK_VS2_VOSEL_MASK, PMIC_RG_BUCK_VS2_VOSEL_SHIFT);
}

static struct pmic_setting init_setting[] = {
	{0x20, 0xA, 0xA, 0},
	{0x24, 0x1F00, 0x1F00, 0},
	{0x30, 0x1, 0x1, 0},
	{0x32, 0x1, 0x1, 0},
	{0x94, 0x0, 0xFFFF, 0},
	{0x10C, 0x10, 0x10, 0},
	{0x112, 0x4, 0x4, 0},
	{0x118, 0x8, 0x8, 0},
	{0x14A, 0x20, 0x20, 0},
	{0x198, 0x0, 0x1FF, 0},
	{0x1B2, 0x3, 0x3, 0},
	{0x3B0, 0x0, 0x300, 0},
	{0x7A6, 0xE000, 0xE000, 0},
	{0x7A8, 0x0, 0x200, 0},
	{0x98A, 0x80, 0x80, 0},
	{0x992, 0xF00, 0xF00, 0},
	{0xA08, 0x1, 0x1, 0},
	{0xA12, 0x1E0, 0x41E0, 0},
	{0xA24, 0xFFFF, 0xFFFF, 0},
	{0xA26, 0xFFC0, 0xFFC0, 0},
	{0xA2C, 0xC0DF, 0xC0DF, 0},
	{0xA2E, 0xEBC0, 0xEBC0, 0},
	{0xA34, 0x8000, 0x8000, 0},
	{0xA40, 0x1400, 0x7C00, 0},
	{0xA9A, 0x2E11, 0xFF11, 0},
	{0xA9E, 0x4000, 0x4000, 0},
	{0xF8C, 0x115, 0x115, 0},
	{0x1188, 0x0, 0x8000, 0},
	{0x1198, 0x13, 0x3FF, 0},
	{0x119E, 0x6000, 0x7000, 0},
	{0x11D4, 0x0, 0x2, 0},
	{0x1212, 0x0, 0x2, 0},
	{0x1224, 0x0, 0x2, 0},
	{0x1238, 0x0, 0x2, 0},
	{0x124A, 0x0, 0x2, 0},
	{0x125C, 0x0, 0x2, 0},
	{0x125E, 0x8000, 0x8000, 0},
	{0x1260, 0x1, 0xFFF, 0},
	{0x1262, 0x4, 0x4, 0},
	{0x1412, 0x8, 0x8, 0},
	{0x1440, 0x200, 0x200, 0},
	{0x148E, 0x18, 0x7F, 0},
	{0x1492, 0x14, 0x7F, 0},
	{0x150E, 0x20, 0x7F, 0},
	{0x1592, 0x14, 0x7F, 0},
	{0x1612, 0x14, 0x7F, 0},
	{0x168E, 0x18, 0x7F, 0},
	{0x170E, 0x18, 0x7F, 0},
	{0x178E, 0x18, 0x7F, 0},
	{0x18B0, 0x2C, 0x7F, 0},
	{0x1918, 0x0, 0x3F3F, 0},
	{0x191A, 0x0, 0x3F00, 0},
	{0x191E, 0x1, 0x1, 0},
	{0x198A, 0x5004, 0x502C, 0},
	{0x198C, 0x11, 0x3F, 0},
	{0x198E, 0x1E0, 0x1E0, 0},
	{0x1990, 0xFB, 0xFF, 0},
	{0x1994, 0x10, 0x38, 0},
	{0x1996, 0x2004, 0xA02C, 0},
	{0x1998, 0x11, 0x3F, 0},
	{0x199A, 0xFB78, 0xFF78, 0},
	{0x199E, 0x2, 0x7, 0},
	{0x19A0, 0x1050, 0x50F1, 0},
	{0x19A2, 0x5535, 0xFF3F, 0},
	{0x19A4, 0xF, 0xF, 0},
	{0x19A6, 0x20, 0xFF, 0},
	{0x19A8, 0x200, 0x200, 0},
	{0x19AC, 0x4208, 0x46D8, 0},
	{0x19AE, 0x2ADC, 0x7FFE, 0},
	{0x19B0, 0x3C00, 0x3C00, 0},
	{0x19B4, 0x20FD, 0xFFFF, 0},
	{0x19B6, 0x200, 0x200, 0},
	{0x19BC, 0xC00, 0xE00, 0},
	{0x19C0, 0x10, 0x30, 0},
	{0x19C2, 0x10, 0x30, 0},
	{0x1A08, 0x4200, 0x4680, 0},
	{0x1A0A, 0x2ADC, 0x7FFE, 0},
	{0x1A0C, 0x3C00, 0x3C00, 0},
	{0x1A10, 0x20FD, 0xFFFF, 0},
	{0x1A14, 0x4208, 0x46D8, 0},
	{0x1A16, 0x2DC, 0x7FFE, 0},
	{0x1A18, 0x3C00, 0x3C00, 0},
	{0x1A1C, 0x2000, 0xFF00, 0},
	{0x1A1E, 0x200, 0x200, 0},
	{0x1A20, 0x4200, 0x4680, 0},
	{0x1A22, 0x2ACA, 0x7FFE, 0},
	{0x1A24, 0x3C00, 0x3C00, 0},
	{0x1A28, 0x2000, 0xFF00, 0},
	{0x1A2C, 0x20, 0x74, 0},
	{0x1A2E, 0x1E, 0x1E, 0},
	{0x1A30, 0x42, 0xFF, 0},
	{0x1A32, 0x480, 0x7E0, 0},
	{0x1A34, 0x20, 0x74, 0},
	{0x1A36, 0x1E, 0x1E, 0},
	{0x1A38, 0x42, 0xFF, 0},
	{0x1A3A, 0x480, 0x7E0, 0},
	{0x1A3C, 0x14C, 0x3CC, 0},
	{0x1A3E, 0x23C, 0x3FC, 0},
	{0x1A40, 0xC400, 0xFF00, 0},
	{0x1A42, 0x80, 0xFF, 0},
	{0x1A44, 0x702C, 0xFF2C, 0},
	{0x1A4E, 0x7000, 0xF000, 0},
	{0x1A66, 0x1C0, 0x3C0, 0},
	{0x1B0E, 0xF, 0xF, 0},
	{0x1B10, 0x1, 0x1, 0},
	{0x1B32, 0x8, 0x8, 0},
	{0x1B46, 0x8000, 0x8000, 0},
	{0x1B88, 0x2C, 0x802C, 0},
	{0x1B98, 0x20, 0x8020, 0},
	{0x1B9C, 0x4000, 0x4000, 0},
	{0x1BA8, 0x21, 0x8021, 0},
	{0x1BB8, 0x1420, 0x9C20, 0},
	{0x1BBC, 0x2, 0x2, 0},
	{0x1BC8, 0x2C, 0x802C, 0},
	{0x1BD8, 0x20, 0x8020, 0},
	{0x1C08, 0x20, 0x8020, 0},
	{0x1C1A, 0x20, 0x8020, 0},
	{0x1C2A, 0x20, 0x8020, 0},
	{0x1C3A, 0x20, 0x8020, 0},
	{0x1C4A, 0x68, 0x806C, 0},
	{0x1C5A, 0x20, 0x8020, 0},
	{0x1C88, 0x20, 0x8020, 0},
	{0x1C8C, 0x4000, 0x4000, 0},
	{0x1C98, 0x20, 0x8020, 0},
	{0x1CA8, 0x20, 0x8020, 0},
	{0x1CB8, 0x20, 0x8020, 0},
	{0x1CC8, 0x6C, 0x806C, 0},
	{0x1CD8, 0x6C, 0x806C, 0},
	{0x1D08, 0x20, 0x8020, 0},
	{0x1D1A, 0x20, 0x8020, 0},
	{0x1D2A, 0x20, 0x8020, 0},
	{0x1D3A, 0x60, 0x8060, 0},
	{0x1D3E, 0x4000, 0x4000, 0},
	{0x1D4A, 0x60, 0x8060, 0},
	{0x1D5A, 0x68, 0x806C, 0},
	{0x1D88, 0x20, 0x8020, 0},
	{0x1D98, 0x20, 0x8020, 0},
	{0x1E88, 0x20, 0x8020, 0},
	{0x1E8C, 0x10, 0x7F, 0},
	{0x1E90, 0xF14, 0x7F7F, 0},
	{0x1EA6, 0x20, 0x8020, 0},
	{0x1EAA, 0x10, 0x7F, 0},
	{0x1EAE, 0xF14, 0x7F7F, 0},
	{0x1F08, 0x20, 0x8020, 0},
	{0x1F0C, 0x3C, 0x7F, 0},
	{0x1F10, 0xF1F, 0x7F7F, 0},
	{0x1F2C, 0x20, 0x8020, 0},
	{0x1F34, 0x90F, 0x7F7F, 0},
	{0x200A, 0x8, 0xC, 0},
	{0x202C, 0x8, 0xC, 0},
	{0x209A, 0x4000, 0x4200, 0},
};

static int vproc1, vproc2;
static bool vproc1_track = true, vproc2_track = true;
static int vproc1_offset = 100000, vproc2_offset = 100000;
static int vproc1_lb = 850000, vproc1_hb = 1118750, vproc2_lb = 850000, vproc2_hb = 1118750;
static int vproc1_rrate, vproc1_frate, vproc2_rrate, vproc2_frate;

#define DOE_UPDATE(_item)	\
{	\
	doe_##_item = dconfig_getenv(#_item);	\
	if (doe_##_item)	\
		_item = atoi(doe_##_item);\
	pal_log_info(#_item " %d\n", _item);\
}

static void doe_load(void)
{
	char *doe_vproc1, *doe_vproc2;
	char *doe_vproc1_track, *doe_vproc2_track;
	char *doe_vproc1_offset, *doe_vproc2_offset;
	char *doe_vproc1_lb, *doe_vproc1_hb, *doe_vproc2_lb, *doe_vproc2_hb;
	char *doe_vproc1_rrate, *doe_vproc1_frate;
	char *doe_vproc2_rrate, *doe_vproc2_frate;

	DOE_UPDATE(vproc1);
	DOE_UPDATE(vproc2);
	DOE_UPDATE(vproc1_track);
	DOE_UPDATE(vproc2_track);
	DOE_UPDATE(vproc1_offset);
	DOE_UPDATE(vproc2_offset);
	DOE_UPDATE(vproc1_lb);
	DOE_UPDATE(vproc1_hb);
	DOE_UPDATE(vproc2_lb);
	DOE_UPDATE(vproc2_hb);

	DOE_UPDATE(vproc1_rrate);
	DOE_UPDATE(vproc1_frate);
	DOE_UPDATE(vproc2_rrate);
	DOE_UPDATE(vproc2_frate);
	if (vproc1_rrate) {
		pmic_config_interface(PMIC_RG_BUCK_VPROC1_SFCHG_RRATE_ADDR,
				      162500 / vproc1_rrate / 1000 - 1,
				      PMIC_RG_BUCK_VPROC1_SFCHG_RRATE_MASK,
				      PMIC_RG_BUCK_VPROC1_SFCHG_RRATE_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC1_SFCHG_RRATE_ADDR,
				      162500 / vproc1_rrate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC1_SFCHG_RRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC1_SFCHG_RRATE_SHIFT);
	}
	if (vproc1_frate) {
		pmic_config_interface(PMIC_RG_BUCK_VPROC1_SFCHG_FRATE_ADDR,
				      162500 / vproc1_frate / 1000 - 1,
				      PMIC_RG_BUCK_VPROC1_SFCHG_FRATE_MASK,
				      PMIC_RG_BUCK_VPROC1_SFCHG_FRATE_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC1_SFCHG_FRATE_ADDR,
				      162500 / vproc1_frate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC1_SFCHG_FRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC1_SFCHG_FRATE_SHIFT);
	}
	if (vproc2_rrate) {
		pmic_config_interface(PMIC_RG_BUCK_VPROC2_SFCHG_RRATE_ADDR,
				      162500 / vproc2_rrate / 1000 - 1,
				      PMIC_RG_BUCK_VPROC2_SFCHG_RRATE_MASK,
				      PMIC_RG_BUCK_VPROC2_SFCHG_RRATE_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC2_SFCHG_RRATE_ADDR,
				      162500 / vproc2_rrate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC2_SFCHG_RRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC2_SFCHG_RRATE_SHIFT);
	}
	if (vproc2_frate) {
		pmic_config_interface(PMIC_RG_BUCK_VPROC2_SFCHG_FRATE_ADDR,
				      162500 / vproc2_frate / 1000 - 1,
				      PMIC_RG_BUCK_VPROC2_SFCHG_FRATE_MASK,
				      PMIC_RG_BUCK_VPROC2_SFCHG_FRATE_SHIFT);
		pmic_config_interface(PMIC_RG_LDO_VSRAM_PROC2_SFCHG_FRATE_ADDR,
				      162500 / vproc2_frate / 1000 - 1,
				      PMIC_RG_LDO_VSRAM_PROC2_SFCHG_FRATE_MASK,
				      PMIC_RG_LDO_VSRAM_PROC2_SFCHG_FRATE_SHIFT);
	}
}

static const struct mtk_regulator empty_regulator;
void pmic_default_voltage(void)
{
	int ret = 0;
	struct mtk_regulator reg_vsram_others = empty_regulator;
	struct mtk_regulator reg_vsram_proc1 = empty_regulator;
	struct mtk_regulator reg_vsram_proc2 = empty_regulator;
	struct mtk_regulator reg_vsram_md = empty_regulator;
	struct mtk_regulator reg_vcore = empty_regulator;
	struct mtk_regulator reg_vproc1 = empty_regulator;
	struct mtk_regulator reg_vproc2 = empty_regulator;
	struct mtk_regulator reg_vgpu11 = empty_regulator;
	struct mtk_regulator reg_vpu = empty_regulator;
	struct mtk_regulator reg_vmodem = empty_regulator;
	int vgpu11_vol = 600000;
	int vsram_others_vol = 875000;
	int vmodem_vol = 875000;
	int vsram_md_vol = 993750;

	/*--Get regulator handle--*/
	if (mtk_regulator_get("vsram_others", &reg_vsram_others))
		ret |= (1 << MTK_REGULATOR_VSRAM_OTHERS);

	if (mtk_regulator_get("vsram_proc1", &reg_vsram_proc1))
		ret |= (1 << MTK_REGULATOR_VSRAM_PROC1);

	if (mtk_regulator_get("vsram_proc2", &reg_vsram_proc2))
		ret |= (1 << MTK_REGULATOR_VSRAM_PROC2);

	if (mtk_regulator_get("vsram_md", &reg_vsram_md))
		ret |= (1 << MTK_REGULATOR_VSRAM_MD);

	if (mtk_regulator_get("vcore", &reg_vcore))
		ret |= (1 << MTK_REGULATOR_VCORE);

	if (mtk_regulator_get("vproc1", &reg_vproc1))
		ret |= (1 << MTK_REGULATOR_VPROC1);

	if (mtk_regulator_get("vproc2", &reg_vproc2))
		ret |= (1 << MTK_REGULATOR_VPROC2);

	if (mtk_regulator_get("vgpu11", &reg_vgpu11))
		ret |= (1 << MTK_REGULATOR_VGPU11);

	if (mtk_regulator_get("vpu", &reg_vpu))
		ret |= (1 << MTK_REGULATOR_VPU);

	if (mtk_regulator_get("vmodem", &reg_vmodem))
		ret |= (1 << MTK_REGULATOR_VMODEM);

	if (ret) {
		pal_log_err("mtk_regulator_get failed.(0x%x)\n", ret);
		return;
	}

	/*--Set voltage--*/
	if (mtk_regulator_set_voltage(&reg_vsram_others, vsram_others_vol, vsram_others_max_uV))
		ret |= (1 << MTK_REGULATOR_VSRAM_OTHERS);
	if (mtk_regulator_set_voltage(&reg_vgpu11, vgpu11_vol, vgpu11_max_uV))
		ret |= (1 << MTK_REGULATOR_VGPU11);
	if (mtk_regulator_set_voltage(&reg_vsram_md, vsram_md_vol, vsram_md_max_uV))
		ret |= (1 << MTK_REGULATOR_VSRAM_MD);
	if (mtk_regulator_set_voltage(&reg_vmodem, vmodem_vol, vmodem_max_uV))
		ret |= (1 << MTK_REGULATOR_VMODEM);

	/* DoE setting */
	if (vproc1)
		mtk_regulator_set_voltage(&reg_vproc1, vproc1, vproc1_max_uV);
	if (vproc2)
		mtk_regulator_set_voltage(&reg_vproc2, vproc2, vproc2_max_uV);
	if (ret) {
		pal_log_err("mtk_regulator_set_voltage failed.(0x%x)\n", ret);
		ret = 0;
	}

/*
 * Bring up only, for voltage adjustment experiment
 * Modify 2nd parameter to the voltage(uV) you want
 */
#if 0
	mtk_regulator_set_voltage(&reg_vsram_others, 900000, vsram_others_max_uV);
	mtk_regulator_set_voltage(&reg_vsram_proc1, 900000, vsram_proc1_max_uV);
	mtk_regulator_set_voltage(&reg_vsram_proc2, 900000, vsram_proc2_max_uV);
	mtk_regulator_set_voltage(&reg_vcore, 800000, vcore_max_uV);
	mtk_regulator_set_voltage(&reg_vproc1, 800000, vproc1_max_uV);
	mtk_regulator_set_voltage(&reg_vproc2, 800000, vproc2_max_uV);
	mtk_regulator_set_voltage(&reg_vgpu11, 800000, vgpu11_max_uV);
	mtk_regulator_set_voltage(&reg_vpu, 800000, vpu_max_uV);
#endif

	/*--Get voltage--*/
	pal_log_info("vsram_others = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_others));
	pal_log_info("vsram_proc1 = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc1));
	pal_log_info("vsram_proc2 = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc2));
	pal_log_info("vsram_md = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_md));
	pal_log_info("vcore = %d uV\n", mtk_regulator_get_voltage(&reg_vcore));
	pal_log_info("vproc1 = %d uV\n", mtk_regulator_get_voltage(&reg_vproc1));
	pal_log_info("vproc2 = %d uV\n", mtk_regulator_get_voltage(&reg_vproc2));
	pal_log_info("vgpu11 = %d uV\n", mtk_regulator_get_voltage(&reg_vgpu11));
	pal_log_info("vpu = %d uV\n", mtk_regulator_get_voltage(&reg_vpu));
	pal_log_info("vmodem = %d uV\n", mtk_regulator_get_voltage(&reg_vmodem));

	/*--enable--, most regulators are default on, no need to enable it */
#if 0
	if (mtk_regulator_enable(&reg_vsram_md, 1))
		ret |= (1 << MTK_REGULATOR_VSRAM_MD);

	if (mtk_regulator_enable(&reg_vgpu11, 1))
		ret |= (1 << MTK_REGULATOR_VGPU11);

	if (ret) {
		pal_log_err("mtk_regulator_enable failed.(0x%x)\n", ret);
		ret = 0;
	}
#endif
}

void pmic_init_setting(void)
{
	U16 i;

#if INIT_SETTING_VERIFIED
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0x9CA6,
			      PMIC_TMA_KEY_MASK,
			      PMIC_TMA_KEY_SHIFT);
	pmic_config_interface(PMIC_HK_AUXADC_KEY_ADDR, 0x6359,
			      PMIC_HK_AUXADC_KEY_MASK,
			      PMIC_HK_AUXADC_KEY_SHIFT);
	pmic_config_interface(PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0x5543,
			      PMIC_BUCK_TOP_WRITE_KEY_MASK,
			      PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_FGADC_KEY_ADDR, 0x1605,
			      PMIC_BM_FGADC_KEY_MASK,
			      PMIC_BM_FGADC_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_BATON_KEY_ADDR, 0x1706,
			      PMIC_BM_BATON_KEY_MASK,
			      PMIC_BM_BATON_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_BIF_KEY_ADDR, 0x1807,
			      PMIC_BM_BIF_KEY_MASK,
			      PMIC_BM_BIF_KEY_SHIFT);
	pmic_config_interface(PMIC_RG_CPS_W_KEY_ADDR, 0x4729,
			      PMIC_RG_CPS_W_KEY_MASK,
			      PMIC_RG_CPS_W_KEY_SHIFT);

	for (i = 0; i < ARRAY_SIZE(init_setting); i++)
		pmic_config_interface(
			init_setting[i].addr, init_setting[i].val,
			init_setting[i].mask, init_setting[i].shift);

#if EFUSE_SW_LOAD
	/* PMIC SW load EFUSE to target register */
	pmic_efuse_sw_load();
#else
	pal_log_info("No EFUSE SW Load\n");
#endif

	pmic_config_interface(PMIC_RG_CPS_W_KEY_ADDR, 0,
			      PMIC_RG_CPS_W_KEY_MASK,
			      PMIC_RG_CPS_W_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_BIF_KEY_ADDR, 0,
			      PMIC_BM_BIF_KEY_MASK,
			      PMIC_BM_BIF_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_BATON_KEY_ADDR, 0,
			      PMIC_BM_BATON_KEY_MASK,
			      PMIC_BM_BATON_KEY_SHIFT);
	pmic_config_interface(PMIC_BM_FGADC_KEY_ADDR, 0,
			      PMIC_BM_FGADC_KEY_MASK,
			      PMIC_BM_FGADC_KEY_SHIFT);
	pmic_config_interface(PMIC_BUCK_TOP_WRITE_KEY_ADDR, 0,
			      PMIC_BUCK_TOP_WRITE_KEY_MASK,
			      PMIC_BUCK_TOP_WRITE_KEY_SHIFT);
	pmic_config_interface(PMIC_HK_AUXADC_KEY_ADDR, 0,
			      PMIC_HK_AUXADC_KEY_MASK,
			      PMIC_HK_AUXADC_KEY_SHIFT);
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0,
			      PMIC_TMA_KEY_MASK,
			      PMIC_TMA_KEY_SHIFT);

	pal_log_info("[PMIC] pmic_init_setting end. v181109\n");
#endif /* INIT_SETTING_VERIFIED */

	doe_load();
	/* MT6359 HW tracking init and default voltage */
	hw_tracking_set("vproc1", vproc1_track, vproc1_offset, vproc1_lb, vproc1_hb);
	hw_tracking_set("vproc2", vproc2_track, vproc2_offset, vproc2_lb, vproc2_hb);
	pmic_default_voltage();

	wk_vs2_voter_setting();
}


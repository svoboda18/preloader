#include <typedefs.h>
#include <platform.h>
#include <pmic.h>
#include <mt6357.h>

/* Enable this option when pmic initial setting is verified */
#define INIT_SETTING_VERIFIED 1

struct pmic_setting init_setting[] = {
	{0x1A, 0xA, 0xA, 0},
	{0x1E, 0x1F00, 0x1F00, 0},
	{0x2A, 0x1, 0x1, 0},
	{0x2C, 0x1, 0x1, 0},
	{0x32, 0x8888, 0xFFFF, 0},
	{0x36, 0x8888, 0xFFFF, 0},
	{0x38, 0x8888, 0xFFFF, 0},
	{0x8E, 0x0, 0xFFFF, 0},
	{0x10C, 0x18, 0x18, 0},
	{0x112, 0x4, 0x4, 0},
	{0x118, 0x8, 0x8, 0},
	{0x134, 0x80, 0x2880, 0},
	{0x14C, 0x20, 0x20, 0},
	{0x198, 0x0, 0x1FF, 0},
	{0x408, 0x1, 0x1, 0},
	{0x790, 0x280, 0x780, 0},
	{0x794, 0x88, 0xFF, 0},
	{0x79E, 0x80, 0x1E0, 0},
	{0x7AC, 0x0, 0x2000, 0},
	{0x98A, 0x40, 0x40, 0},
	{0x98C, 0x1, 0x1, 0},
	{0xA08, 0x1, 0x1, 0},
	{0xA20, 0x8, 0x8, 0},
	{0xA2C, 0x81E0, 0x81E0, 0},
	{0xA30, 0xFFF0, 0xFFF0, 0},
	{0xA3C, 0x200, 0x200, 0},
	{0xD08, 0x20, 0x70, 0},
	{0xF8C, 0xAA, 0x3FF, 0},
	{0xF8E, 0x0, 0x1, 0},
	{0x1008, 0x1, 0xF, 0},
	{0x1120, 0x0, 0x8000, 0},
	{0x1126, 0x340, 0xFFC0, 0},
	{0x1128, 0x8083, 0x8FFF, 0},
	{0x112A, 0x30, 0x3FF, 0},
	{0x112E, 0x4000, 0x4000, 0},
	{0x1130, 0xB, 0x3FF, 0},
	{0x1132, 0x30, 0x3FF, 0},
	{0x1134, 0xB, 0x3FF, 0},
	{0x1136, 0x6000, 0x7000, 0},
	{0x1138, 0x60, 0x70, 0},
	{0x113A, 0x95D0, 0xFFF0, 0},
	{0x113C, 0x55, 0xFF, 0},
	{0x1148, 0x4000, 0x4000, 0},
	{0x114C, 0x100, 0x103, 0},
	{0x1208, 0x8040, 0x83FF, 0},
	{0x120C, 0x4, 0x4, 0},
	{0x1210, 0x1, 0x1, 0},
	{0x122E, 0x0, 0x1, 0},
	{0x1412, 0x8, 0x8, 0},
	{0x1418, 0xF, 0xF, 0},
	{0x1444, 0x10, 0x10, 0},
	{0x148A, 0x29, 0x7F, 0},
	{0x148C, 0xF1F, 0x7F7F, 0},
	{0x148E, 0x1030, 0x3030, 0},
	{0x150A, 0x29, 0x7F, 0},
	{0x150C, 0x1A29, 0x7F7F, 0},
	{0x150E, 0x1030, 0x3030, 0},
	{0x158A, 0x38, 0x7F, 0},
	{0x158C, 0x70F, 0x7F7F, 0},
	{0x158E, 0x1010, 0x7070, 0},
	{0x160A, 0x40, 0x7F, 0},
	{0x160C, 0x1968, 0x7F7F, 0},
	{0x160E, 0x2020, 0x3030, 0},
	{0x162E, 0x40, 0x7F, 0},
	{0x1632, 0x48, 0x7F, 0},
	{0x168C, 0x202, 0x7F7F, 0},
	{0x168E, 0x70, 0x73, 0},
	{0x1690, 0xE, 0xCE, 0},
	{0x1698, 0x2810, 0x3F3F, 0},
	{0x169A, 0x800, 0x3F00, 0},
	{0x169E, 0x1, 0x1, 0},
	{0x1708, 0x2000, 0x3000, 0},
	{0x170A, 0x5E0, 0x7E0, 0},
	{0x170C, 0x17F, 0x1FF, 0},
	{0x170E, 0x60, 0x60, 0},
	{0x1710, 0x209, 0x60F, 0},
	{0x1712, 0x208, 0x60F, 0},
	{0x1716, 0xFF0, 0x7FF0, 0},
	{0x1718, 0x0, 0x7, 0},
	{0x171C, 0x8000, 0xE000, 0},
	{0x171E, 0x2AD, 0x7FF, 0},
	{0x1720, 0x327A, 0xFFFF, 0},
	{0x1722, 0x323A, 0xFFFF, 0},
	{0x1726, 0x304A, 0x7E7E, 0},
	{0x1728, 0x800, 0x3C00, 0},
	{0x172A, 0x3F, 0x3F, 0},
	{0x172C, 0x5258, 0xFFFF, 0},
	{0x1734, 0x3, 0x7, 0},
	{0x173A, 0x8, 0xF, 0},
	{0x1740, 0x50, 0x4F0, 0},
	{0x1742, 0x3C, 0xFC, 0},
	{0x1744, 0x0, 0x300, 0},
	{0x1746, 0x8886, 0xFFFF, 0},
	{0x174C, 0x6000, 0x6000, 0},
	{0x1758, 0x17, 0x3F, 0},
	{0x175C, 0x1700, 0x3F00, 0},
	{0x176C, 0x700, 0xF00, 0},
	{0x188C, 0x1, 0x1, 0},
	{0x188E, 0x0, 0x1, 0},
	{0x1890, 0x0, 0x1, 0},
	{0x1892, 0x0, 0x1, 0},
	{0x1894, 0x0, 0x1, 0},
	{0x1896, 0x0, 0x1, 0},
	{0x1898, 0x0, 0x1, 0},
	{0x189A, 0x0, 0x1, 0},
	{0x189C, 0x0, 0x1, 0},
	{0x189E, 0x0, 0x1, 0},
	{0x18A0, 0x0, 0x1, 0},
	{0x18A2, 0x0, 0x1, 0},
	{0x18A4, 0x0, 0x1, 0},
	{0x18A6, 0x0, 0x1, 0},
	{0x18A8, 0x0, 0x1, 0},
	{0x18AA, 0x0, 0x1, 0},
	{0x18AC, 0x0, 0x1, 0},
	{0x18AE, 0x0, 0x1, 0},
	{0x18B0, 0x0, 0x1, 0},
	{0x18B2, 0x0, 0x1, 0},
	{0x18B4, 0x0, 0x1, 0},
	{0x18B6, 0x0, 0x1, 0},
	{0x18B8, 0x0, 0x1, 0},
	{0x18BA, 0x0, 0x1, 0},
	{0x18BC, 0x0, 0x1, 0},
	{0x18BE, 0x0, 0x1, 0},
	{0x18C0, 0x0, 0x1, 0},
	{0x18C2, 0x0, 0x1, 0},
	{0x18E6, 0x1, 0x1, 0},
	{0x19B4, 0x35, 0x7F, 0},
	{0x19B6, 0xF1F, 0x7F7F, 0},
	{0x19D2, 0x65, 0x7F, 0},
	{0x19D4, 0xF1F, 0x7F7F, 0},
	{0x1B9E, 0x1, 0x1, 0},
};

static const struct mtk_regulator empty_regulator;
void pmic_default_voltage(void)
{
	int ret = 0;
	struct mtk_regulator reg_vsram_others = empty_regulator;
	struct mtk_regulator reg_vsram_proc = empty_regulator;
	struct mtk_regulator reg_vproc = empty_regulator;
	struct mtk_regulator reg_vcore = empty_regulator;
	struct mtk_regulator reg_vmodem = empty_regulator;
	unsigned int segment = seclib_get_devinfo_with_index(28);
	unsigned char vcore_segment = (unsigned char)((segment & 0xC0000000) >> 30);
	int vsram_others_vol = 1294000;
	int vsram_proc_vol = 0;
	int vproc_vol = 0;
	int vcore_vol = 1194000;
	int vmodem_vol = 0;

	/*vsram_proc_vol = 1250000;
	 *vproc_vol = 1150000;
	 *vcore_vol = 1150000;
	 *vmodem_vol = 1193750;
	 */
#if defined(SLT)
	pmic_read_interface(PMIC_RG_VDRAM_VOTRIM_ADDR, &ret,
				  PMIC_RG_VDRAM_VOTRIM_MASK,
				  PMIC_RG_VDRAM_VOTRIM_SHIFT);
	print("Before vdram votrim: 0x%x\n", ret);
	pmic_config_interface(PMIC_RG_VDRAM_VOTRIM_ADDR, 0,
				  PMIC_RG_VDRAM_VOTRIM_MASK,
				  PMIC_RG_VDRAM_VOTRIM_SHIFT);
	pmic_read_interface(PMIC_RG_VDRAM_VOTRIM_ADDR, &ret,
				  PMIC_RG_VDRAM_VOTRIM_MASK,
				  PMIC_RG_VDRAM_VOTRIM_SHIFT);
	print("After vdram votrim: 0x%x\n", ret);
	ret = 0;
#endif
#if defined(MTK_CTIGHTENING)
	print("MTK_CTIGHTENING");
	if ((vcore_segment & 0x3) == 0x1 ||
	    (vcore_segment & 0x3) == 0x2) {
#if defined(MTK_TURBO_CTIGHTENING)
		/* VCORE 1.11250V: 0x5F (-3%)*/
		vcore_vol = 1112500;
		vsram_others_vol = 1212500;
#else
		/* VCORE 1.0875V: 0x5B (-5%)*/
		vcore_vol = 1087500;
		vsram_others_vol = 1187500;
#endif
	} else {
#if defined(MTK_TURBO_CTIGHTENING)
		/* VCORE 1.15625V: 0x66 */
		vcore_vol = 1156250;
		vsram_others_vol = 1256250;
#else
		/* VCORE 1.13125V: 0x62 */
		vcore_vol = 1131250;
		vsram_others_vol = 1231250;
#endif
	}
#else
	if ((vcore_segment & 0x3) == 0x1 ||
	    (vcore_segment & 0x3) == 0x2) {
		vcore_vol = 1150000;
		vsram_others_vol = 1250000;
	} else {
		vcore_vol = 1194000;
		vsram_others_vol = 1294000;
	}
#endif

	vmodem_vol = 1193750;

	/*--Get regulator handle--*/
	if (mtk_regulator_get("vsram_others", &reg_vsram_others))
		ret |= (1 << MTK_REGULATOR_VSRAM_OTHERS);

	if (mtk_regulator_get("vsram_proc", &reg_vsram_proc))
		ret |= (1 << MTK_REGULATOR_VSRAM_PROC);

	if (mtk_regulator_get("vproc", &reg_vproc))
		ret |= (1 << MTK_REGULATOR_VPROC);

	if (mtk_regulator_get("vcore", &reg_vcore))
		ret |= (1 << MTK_REGULATOR_VCORE);

	if (mtk_regulator_get("vmodem", &reg_vmodem))
		ret |= (1 << MTK_REGULATOR_VMODEM);

	if (ret) {
		print("mtk_regulator_get failed.(0x%x)\n", ret);
		ret = 0;
	}

	/*--Set voltage--*/
	if (mtk_regulator_set_voltage(&reg_vcore, vcore_vol, vcore_max_uV))
		ret |= (1 << MTK_REGULATOR_VCORE);

	if ( mtk_regulator_set_voltage(&reg_vmodem, vmodem_vol, vmodem_max_uV))
		ret |= (1 << MTK_REGULATOR_VMODEM);

	if (mtk_regulator_set_voltage(&reg_vsram_others, vsram_others_vol, vsram_others_max_uV))
		ret |= (1 << MTK_REGULATOR_VSRAM_OTHERS);

	if (ret) {
		print("mtk_regulator_set_voltage failed.(0x%x)\n", ret);
		ret = 0;
	}

	/*--Get voltage--*/
	print("vsram_others = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_others));
	print("vsram_proc = %d uV\n", mtk_regulator_get_voltage(&reg_vsram_proc));
	print("vproc = %d uV\n", mtk_regulator_get_voltage(&reg_vproc));
	print("vcore = %d uV\n", mtk_regulator_get_voltage(&reg_vcore));
	print("vmodem = %d uV\n", mtk_regulator_get_voltage(&reg_vmodem));

	/*--enable--*/
	if (mtk_regulator_enable(&reg_vsram_others, 1))
		ret |= (1 << MTK_REGULATOR_VSRAM_OTHERS);

	if (mtk_regulator_enable(&reg_vsram_proc, 1))
		ret |= (1 << MTK_REGULATOR_VSRAM_PROC);

	if (mtk_regulator_enable(&reg_vproc, 1))
		ret |= (1 << MTK_REGULATOR_VPROC);

	if (mtk_regulator_enable(&reg_vcore, 1))
		ret |= (1 << MTK_REGULATOR_VCORE);

	if (mtk_regulator_enable(&reg_vmodem, 1))
		ret |= (1 << MTK_REGULATOR_VMODEM);

	if (ret) {
		print("mtk_regulator_enable failed.(0x%x)\n", ret);
		ret = 0;
	}
}

void pmic_init_setting(void)
{
	U16 i;
	U32 chip_ver = get_PMIC_chip_version();

#if INIT_SETTING_VERIFIED
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0x9CA8,
		PMIC_TMA_KEY_MASK, PMIC_TMA_KEY_SHIFT);
	for (i = 0; i < ARRAY_SIZE(init_setting); i++)
		pmic_config_interface(
			init_setting[i].addr, init_setting[i].val,
			init_setting[i].mask, init_setting[i].shift);
	pmic_config_interface(PMIC_TMA_KEY_ADDR, 0,
		PMIC_TMA_KEY_MASK, PMIC_TMA_KEY_SHIFT);

	/* DCXO:, E1 write 0 */
	if (((chip_ver & 0x00F0) >> 4) == 0x1) {
		pmic_config_interface(0x792, 0x0, 0x2, 0);
	}else {
	/* >= E2, set VCORE sleep voltage: 0.95V */
		pmic_config_interface(0x150A, 0x45, 0x7F, 0);
		pmic_config_interface(0x170A, 0x560, 0x7E0, 0);
	}
#endif /* INIT_SETTING_VERIFIED */
	print("[PMIC] pmic_init_setting end. v170824\n");
	/*mt6357_dump_register();*/

	pmic_default_voltage();
}

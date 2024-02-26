#include "platform.h"
#include "sec_devinfo.h"
#include "dramc_pi_api.h"
#include "emi.h"
#include "sec_devinfo.h"
#include "mtk_vcore_opp.h"
#include <dconfig_env.h>


static int vcore_uv_table[VCORE_OPP_NUM];

void set_vcore_uv_table(int vcore_opp, int vcore_uv)
{
	vcore_uv_table[vcore_opp] = vcore_uv;
}

int get_vcore_uv_table(int vcore_opp)
{
	return vcore_uv_table[vcore_opp];
}

unsigned int get_vcore_opp_volt(unsigned int ddr_type, unsigned int opp)
{
	return get_vcore_uv_table(opp);
}

static int get_vb_volt(int vcore_opp)
{
	int ret = 0;
	int ptpod41 = ((seclib_get_devinfo_with_index(41) >> 9) & 0x3);

	print("%s: PTPOD41: 0x%x\n", __func__, ptpod41);

	switch (vcore_opp) {
	case VCORE_OPP_0:
	case VCORE_OPP_1:
	case VCORE_OPP_3:
		break;
	case VCORE_OPP_2:
		if(ptpod41 != 0)
			ret = ptpod41 - 1;
		break;
	default:
		break;
	}
	return ret * 25000;
}

void dvfsrc_opp_level_mapping(void)
{
	int vcore_opp_0_uv, vcore_opp_1_uv;
	int vcore_opp_2_uv, vcore_opp_3_uv;
	int ddr_type = mt_get_dram_type();
	int info2 = 0;
#if MTK_DOE_CONFIG_ENV_SUPPORT
	char *doe_vcore;
	int dvfs_v_mode = 0;

	/* todo: remove when LP4 default enable */
	doe_vcore = dconfig_getenv("dvfs_v_mode");
	if (doe_vcore) {
		dvfs_v_mode = atoi(doe_vcore);
		print("Vcore dvfs_v_mode : %d\n", dvfs_v_mode);
	}
#endif
	print("%s: PTPOD0: 0x%x (ddr=%d, info2=0x%x)\n",
		__func__, ddr_type, info2);

	/* bypass efuse, allow dvfs */
	vcore_opp_0_uv = 800000;
	vcore_opp_1_uv = 700000;
	vcore_opp_2_uv = 700000 - get_vb_volt(VCORE_OPP_2);
	vcore_opp_3_uv = 650000;
	print("%s: skip use default vcore_opp_uv: %d, %d, %d, %d\n", __func__,
			vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);

#if MTK_DOE_CONFIG_ENV_SUPPORT
	if (dvfs_v_mode == 3) { /*LV */
		vcore_opp_0_uv = 756250;
		vcore_opp_1_uv = 662500;
		vcore_opp_2_uv = 662500;
		vcore_opp_3_uv = 612500;
	}else if (dvfs_v_mode == 1) { /*HV */
		vcore_opp_0_uv = 843750;
		vcore_opp_1_uv = 737500;
		vcore_opp_2_uv = 737500;
		vcore_opp_3_uv = 687500;
	}
#endif
	print("%s: final vcore_opp_uv: %d, %d, %d, %d\n", __func__,
			vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);

	set_vcore_uv_table(VCORE_OPP_0, vcore_opp_0_uv);
	set_vcore_uv_table(VCORE_OPP_1, vcore_opp_1_uv);
	if (info2 & 0x1) {
		set_vcore_uv_table(VCORE_OPP_2, vcore_opp_1_uv);
		set_vcore_uv_table(VCORE_OPP_3, vcore_opp_1_uv);
		print("%s: OPP_2 and OPP_3 adj to %d and %d\n",
			__func__,
			get_vcore_uv_table(VCORE_OPP_2),
			get_vcore_uv_table(VCORE_OPP_3));
	} else {
		set_vcore_uv_table(VCORE_OPP_2, vcore_opp_2_uv);
		set_vcore_uv_table(VCORE_OPP_3, vcore_opp_3_uv);
	}
}


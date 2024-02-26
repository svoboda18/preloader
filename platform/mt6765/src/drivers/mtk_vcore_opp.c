#include "platform.h"
#include "sec_devinfo.h"
#include "dramc_pi_api.h"
#include "emi.h"

#include "mtk_vcore_opp.h"

static int vcore_uv_table[VCORE_OPP_NUM];

void set_vcore_uv_table(int vcore_opp, int vcore_uv)
{
	vcore_uv_table[vcore_opp] = vcore_uv;
}

int get_vcore_uv_table(int vcore_opp)
{
	return vcore_uv_table[vcore_opp];
}

static int get_vb_volt(int vcore_opp)
{
	int ret = 0;
	int ptpod10 = seclib_get_devinfo_with_index(41);
	int ptpod0 = seclib_get_devinfo_with_index(42);

	print("%s: PTPOD10: 0x%x\n", __func__, ptpod10);

	switch (vcore_opp) {
	case VCORE_OPP_0:
		ret = min(((ptpod10 >> 2) & 0x3), ((ptpod10 >> 6) & 0x3));
		ret = min(ret, ((ptpod10 >> 12) & 0x3));

		/* GPU 730 Mhz 0.8V VB */
		ptpod10 = (ptpod10 >> 8) & 0x3;
		/* if GPU 730 Mhz needs 0.8V */
		if (ptpod10 == 1)
			ret = 0;
		if (ret > 1) {
			print("failed PTPOD10: 0x%x\n", ptpod10);
			ret = 0;
		}
		break;
	case VCORE_OPP_1:
		ret = min(((ptpod10 >> 0) & 0x3), ((ptpod10 >> 4) & 0x3));
		if (ret > 1) {
			print("failed PTPOD10: 0x%x\n", ptpod10);
			ret = 0;
		}
		break;
	case VCORE_OPP_2:
		ret = (ptpod10 >> 10) & 0x3;
		break;
	case VCORE_OPP_3:
		ret = (ptpod10 >> 4) & 0x3f;
	        if (ptpod0 == 0x0000FF00 || ptpod0 == 0x0) ret = 0;
		else if (ret == 0) ret = 1;
		else ret = 0;
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
	int ptpod0 = seclib_get_devinfo_with_index(42);
	int skip = 0;
	int ct_enable = 0;
	int ddr_type = mt_get_dram_type();
	int info2 = seclib_get_devinfo_with_index(18);

	/* todo: remove when LP4 default enable */
	if (ddr_type != TYPE_LPDDR3)
		skip = 1;

#if defined(MTK_VCORE_DVFS_SKIP_EFUSE)
	skip = 1;
#endif
	if ((ptpod0 != 0x0000FF00 && ptpod0 != 0x0))
		ct_enable = 1;

	print("%s: PTPOD0: 0x%x (ct_enable:%d, skip:%d, ddr=%d, info2=0x%x)\n",
		__func__, ptpod0, ct_enable, skip, ddr_type, info2);

	if (ct_enable) {
#if defined(MTK_VCORE_CT_SUPPORT)
		vcore_opp_0_uv = 800000 - get_vb_volt(VCORE_OPP_0);
		vcore_opp_1_uv = 700000 - get_vb_volt(VCORE_OPP_1);
		vcore_opp_2_uv = 700000 - get_vb_volt(VCORE_OPP_2);
		vcore_opp_3_uv = 650000 + get_vb_volt(VCORE_OPP_3);
		print("%s: CT EFUSE vcore_opp_uv: %d, %d, %d, %d\n", __func__,
				vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);
#if defined(MTK_MINI_SQC_SUPPORT)
		vcore_opp_0_uv -= 31250;
		vcore_opp_1_uv -= 31250;
		vcore_opp_2_uv -= 31250;
		vcore_opp_3_uv = min(vcore_opp_2_uv, vcore_opp_3_uv);
		print("%s: MINI_SQC vcore_opp_uv: %d, %d, %d, %d\n", __func__,
				vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);
#endif

		vcore_opp_2_uv = max(vcore_opp_2_uv, vcore_opp_3_uv);
		vcore_opp_1_uv = max(vcore_opp_0_uv - 100000, vcore_opp_1_uv);
		vcore_opp_1_uv = max(vcore_opp_1_uv, vcore_opp_2_uv);
		print("%s: DRAM vcore_opp_uv: %d, %d, %d %d\n", __func__,
				vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);
#else
		vcore_opp_0_uv = 800000;
		vcore_opp_1_uv = 700000;
		vcore_opp_2_uv = 700000;
		vcore_opp_3_uv = 650000 + get_vb_volt(VCORE_OPP_3);
		print("%s: default vcore_opp_uv: %d, %d, %d, %d\n", __func__,
				vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);
#endif
	} else if (skip == 1) {
		/* bypass efuse, allow dvfs */
		vcore_opp_0_uv = 800000;
		vcore_opp_1_uv = 700000;
		vcore_opp_2_uv = 700000;
		vcore_opp_3_uv = 650000 + get_vb_volt(VCORE_OPP_3);
		print("%s: skip use default vcore_opp_uv: %d, %d, %d, %d\n", __func__,
				vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);
	} else {
		/* can not dvfs */
		vcore_opp_0_uv = 800000;
		vcore_opp_1_uv = 800000;
		vcore_opp_2_uv = 800000;
		vcore_opp_3_uv = 800000;
		print("%s: disabled vcore_opp_uv: %d, %d, %d, %d\n", __func__,
				vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv, vcore_opp_3_uv);
	}

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


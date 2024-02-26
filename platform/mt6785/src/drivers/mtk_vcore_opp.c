#include "platform.h"
#include "sec_devinfo.h"
#include "mtk_vcore_opp.h"
#include <dconfig_env.h>

#define DVFSRC_BASE (0x10012000)
static int vcore_cpe[VCORE_OPP_NUM];
static int vcore_cpe_equation[VCORE_OPP_NUM][6] = {
/*	Nand0  Nand1  Nor0  Nor1  Coeff     criteria*/
	{ 590, 262,  330,  2683,  -2528742, 300000}, /*Vbin @0.825*/
	{ 623, -108, 436,  2592,  -2190425, 310000}, /*Vbin @0.725*/
	{ 913, -2610, 2630, 5374, -3820820, 300000}, /*Vbin @0.65*/
};

static int vcore_uv_table[VCORE_OPP_NUM];

void set_vcore_uv_table(int vcore_opp, int vcore_uv)
{
	vcore_uv_table[vcore_opp] = vcore_uv;
}

int get_vcore_uv_table(int vcore_opp)
{
	return vcore_uv_table[vcore_opp];
}

static int get_opp_min_ct(void)
{
	int opp_min_ct;
#if defined(MTK_VCORE_CT_SUPPORT)
	opp_min_ct = 2;
#else
	opp_min_ct = 3;
#endif
	return opp_min_ct;
}

static int get_vb_volt(int vcore_opp)
{
	int ret = 0;
	int idx, idx2;
	int opp_min_ct = get_opp_min_ct();
	int ptpod = seclib_get_devinfo_with_index(41);

	switch (vcore_opp) {
	case VCORE_OPP_0:
		idx = (ptpod >> 6) & 0x7;
		if (idx >= opp_min_ct)
			ret = 1;
		break;
	case VCORE_OPP_1:
		idx = (ptpod >> 6) & 0x7;
		idx2 = (ptpod >> 3) & 0x7;
		if ((idx >= opp_min_ct) && (idx2 >= opp_min_ct))
			ret = 1;
		break;
	case VCORE_OPP_2:
		idx = ptpod & 0x7;
		if (idx >= opp_min_ct)
			ret = 1;
		break;
	default:
		break;
	}

	return ret * 25000;
}

static void soc_ptp_check(void)
{
#ifndef MTK_VCORE_CPE_ENABLE
	int i;

	for(i = 0; i < VCORE_OPP_NUM; i++)
		vcore_cpe[i] = 0;
#else
	int fptp0, fptp1, fptp2, fptp3;
	int cpe_bin, soc_bin;
	int i;
	int ptpod = seclib_get_devinfo_with_index(40);

	fptp0 = eem_get_soc_det_cnt(0);
	fptp1 = eem_get_soc_det_cnt(1);
	fptp2 = eem_get_soc_det_cnt(2);
	fptp3 = eem_get_soc_det_cnt(3);

	pal_log_info("[CPE]: Det_Count : %d, %d, %d, %d\n", fptp0,
		fptp1, fptp2, fptp3);
	pal_log_info("[CPE]: vcore efuse: 0x%x\n", ptpod);

	if ((fptp0 == 0) || (fptp1 == 0)
		|| (fptp2 == 0) || (fptp3 == 0)) {
		pal_log_info("[CPE]: PTP Counter invalid\n");
	} else {
		for(i = 0; i < VCORE_OPP_NUM; i++) {
			if (i == VCORE_OPP_0)
				soc_bin = ((ptpod>> 6) & 0x7);
			else if (i == VCORE_OPP_1)
				soc_bin = ((ptpod >> 3) & 0x7);
			else if (i == VCORE_OPP_2)
				soc_bin = (ptpod & 0x7);

			cpe_bin = (vcore_cpe_equation[i][0] * fptp1)
				+ (vcore_cpe_equation[i][1] * fptp3)
				+ (vcore_cpe_equation[i][2] * fptp0)
				+ (vcore_cpe_equation[i][3] * fptp2)
				+ vcore_cpe_equation[i][4];

			if (cpe_bin  < vcore_cpe_equation[i][5])
				vcore_cpe[i] = 0;
			else
				vcore_cpe[i] = 1;

			pal_log_info("[CPE]: SOC_OPP=%d, SOC_BIN=%d, CPE_VBIN=%d, PASS_Criteria=%d, Result=%d\n",
				i, soc_bin, cpe_bin, vcore_cpe_equation[i][5], vcore_cpe[i]);
		}
	}
#endif
}

static int is_ct_support(void)
{
	soc_ptp_check();
#if defined(MTK_VCORE_CT_SUPPORT)
	return 1;
#else
	return 0;
#endif
}

static int is_qea_test(void)
{
	return 0;
}

void dvfsrc_opp_level_mapping(void)
{
	char *doe_vcore;
	int is_vcore_ct = is_ct_support();
	int is_vcore_aging = 0;
	int is_vcore_qea = is_qea_test();
	int vcore_opp_0_uv, vcore_opp_1_uv, vcore_opp_2_uv;
	int dvfs_v_mode = 0;

	doe_vcore = dconfig_getenv("vcore_ct");
	if (doe_vcore) {
		is_vcore_ct = atoi(doe_vcore);
		pal_log_info("Vcore_ct Doe : %d\n", is_vcore_ct);
	}

	doe_vcore = dconfig_getenv("dvfs_v_mode");
	if (doe_vcore) {
		dvfs_v_mode = atoi(doe_vcore);
		pal_log_info("Vcore dvfs_v_mode : %d\n", dvfs_v_mode);
	}

	vcore_opp_0_uv = 825000;
	vcore_opp_1_uv = 725000;
	vcore_opp_2_uv = 650000;

	if (is_vcore_ct) {
		#ifdef MTK_VCORE_CPE_ENABLE
			mt_reg_sync_writel((vcore_cpe[2] | (vcore_cpe[1] << 1)
				| (vcore_cpe[0] << 2) | 1 << 3),
		        DVFSRC_BASE + 0x94);
			if (vcore_cpe[VCORE_OPP_0])
				vcore_opp_0_uv = vcore_opp_0_uv - get_vb_volt(VCORE_OPP_0);
			if (vcore_cpe[VCORE_OPP_0] && vcore_cpe[VCORE_OPP_1])
				vcore_opp_1_uv = vcore_opp_1_uv - get_vb_volt(VCORE_OPP_1);
		#else
			vcore_opp_0_uv = vcore_opp_0_uv - get_vb_volt(VCORE_OPP_0);
			vcore_opp_1_uv = vcore_opp_1_uv - get_vb_volt(VCORE_OPP_1);
		#endif
	}

#if defined(MTK_AGING_FLAVOR_LOAD)
	vcore_opp_0_uv = vcore_opp_0_uv - 12500;
	vcore_opp_1_uv = vcore_opp_1_uv - 12500;
	vcore_opp_2_uv = vcore_opp_2_uv - 12500;
	is_vcore_aging = 1;
#else
	if (is_vcore_qea || (dvfs_v_mode == 3)) { /* LV */
		if (vcore_opp_0_uv == 800000)
			vcore_opp_0_uv = 756250;
		else
			vcore_opp_0_uv = 781250;

		if (vcore_opp_1_uv == 700000)
			vcore_opp_1_uv = 662500;
		else
			vcore_opp_1_uv = 687500;

		vcore_opp_2_uv = 612500;
	} else if (dvfs_v_mode == 1) {   /* HV */
		if (vcore_opp_0_uv == 800000)
			vcore_opp_0_uv = 843750;
		else
			vcore_opp_0_uv = 868750;

		if (vcore_opp_1_uv == 700000)
			vcore_opp_1_uv = 737500;
		else
			vcore_opp_1_uv = 762500;

		vcore_opp_2_uv = 687500;
	}
#endif


	pal_log_info("%s: CT=%d, AGING=%d, QEA=%d, VMODE=%d\n",
			__func__,
			is_vcore_ct,
			is_vcore_aging,
			is_vcore_qea,
			dvfs_v_mode);

	pal_log_info("%s: FINAL vcore_opp_uv: %d, %d, %d\n",
			__func__,
			vcore_opp_0_uv,
			vcore_opp_1_uv,
			vcore_opp_2_uv);

	set_vcore_uv_table(VCORE_OPP_0, vcore_opp_0_uv);
	set_vcore_uv_table(VCORE_OPP_1, vcore_opp_1_uv);
	set_vcore_uv_table(VCORE_OPP_2, vcore_opp_2_uv);
}


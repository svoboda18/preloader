#ifndef __MTK_VCORE_OPP_H___
#define __MTK_VCORE_OPP_H___

enum vcore_opp {
	VCORE_OPP_0 = 0,
	VCORE_OPP_1,
	VCORE_OPP_2,
	VCORE_OPP_NUM,
};

extern void dvfsrc_opp_level_mapping(void);
extern int get_vcore_uv_table(int vcore_opp);
extern unsigned int  eem_get_soc_det_cnt(int typeidx);

#endif

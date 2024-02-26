#ifndef __SSPM_MTK_REGULATOR_CORE_H
#define __SSPM_MTK_REGULATOR_CORE_H

#include <regulator/mtk_regulator.h>

struct regulator_ctrl {
	int (*set_voltage)(unsigned char id, unsigned int min_vol, unsigned int max_vol);
	int (*get_voltage)(unsigned char id);
	/* enable/disable regulator */
	int (*enable)(unsigned char id, unsigned char en);
	int (*is_enabled)(unsigned char id);
	int (*set_mode)(unsigned char id, unsigned char mode);
	int (*get_mode)(unsigned char id);
#ifdef LDO_VOTRIM_SUPPORT
	int (*set_votrim)(unsigned char id, int trim_volt);
	int (*get_votrim)(unsigned char id);
#endif /*--LDO_TRIM_SUPPORT--*/
};

extern int mtk_simple_regulator_register(struct mtk_regulator *mreg);

#endif /* __SSPM_MTK_REGULATOR_CORE_H */

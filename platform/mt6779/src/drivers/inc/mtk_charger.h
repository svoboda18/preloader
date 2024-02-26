#ifndef _MTK_CHARGER_H
#define _MTK_CHARGER_H

#include <typedefs.h>
#include <pmic.h>

#ifndef EBUSY
#define EBUSY  16
#endif

#define MTK_UNLOCK_BAT_CV_LEVEL 4350000/* uV */

#ifdef MTK_UNLOCK_BAT_SUPPORT
extern int mtk_try_unlock_bat(unsigned int uV);
#else
static inline int mtk_try_unlock_bat(unsigned int uV)
{
	return 0;
}
#endif /* MTK_UNLOCK_BAT_SUPPORT */

#ifdef MTK_EXT_CHGDET_SUPPORT
extern int mtk_ext_chgdet(CHARGER_TYPE *chg_type);
#else
static inline int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	return 0;
}
#endif /* MTK_EXT_CHGDET_SUPPORT */

#endif /* _MTK_CHARGER_H */

#include <typedefs.h>
#include <pmic.h>
#include <mtk_subpmic.h>

#define __WEAK __attribute__((weak))

__WEAK int mtk_try_unlock_bat(unsigned int uV)
{
	return 0;
}

__WEAK int mtk_ext_chgdet(CHARGER_TYPE *chg_type)
{
	return 0;
}

__WEAK int mtk_subpmic_init(void)
{
	return 0;
}

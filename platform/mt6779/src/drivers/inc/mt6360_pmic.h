#ifndef _MT6360_PMIC_H
#define _MT6360_PMIC_H

#include "typedefs.h"
#include <regulator/mtk_regulator_errno.h>
#include <regulator/mtk_regulator_core.h>

extern int mt6360_pmic_enable_poweroff_sequence(bool en);
extern int mt6360_pmic_read_interface(u8 addr, u8 *data, u8 mask, u8 shift);
extern int mt6360_pmic_config_interface(u8 addr, u8 data, u8 mask, u8 shift);
extern int mt6360_get_hw_trapping_type(void);
extern int mt6360_pmic_i2c_probe(void);

#endif /* _MT6360_PMIC_H */

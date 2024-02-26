#ifndef _MT6360_LDO_H
#define _MT6360_LDO_H

#include "typedefs.h"
#include <regulator/mtk_regulator_errno.h>
#include <regulator/mtk_regulator_core.h>


extern int mt6360_ldo_read_interface(u8 addr, u8 *data, u8 mask, u8 shift);
extern int mt6360_ldo_config_interface(u8 addr, u8 data, u8 mask, u8 shift);
extern int mt6360_ldo_i2c_probe(void);

#endif /* _MT6360_LDO_H */

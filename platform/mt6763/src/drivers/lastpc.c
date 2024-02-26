#include <typedefs.h>

#define MCU_ALL_PWR_ON_CTRL    0x0C530B58
#define MCU_ALL_PWR_ON_ENABLE  0x1

void lastpc_init(void)
{
	unsigned int ctrl = DRV_Reg32(MCU_ALL_PWR_ON_CTRL);

	DRV_WriteReg32(MCU_ALL_PWR_ON_CTRL, (ctrl|MCU_ALL_PWR_ON_ENABLE));
}

#include <typedefs.h>

#define CPC_FLOW_CTRL_CFG	0x0C53A814
#define RESET_ON_KEEP_EN	(1 << 17)

void lastpc_init(void)
{
	unsigned int ctrl = DRV_Reg32(CPC_FLOW_CTRL_CFG);

	DRV_WriteReg32(CPC_FLOW_CTRL_CFG, (ctrl|RESET_ON_KEEP_EN));
	dsb();
}

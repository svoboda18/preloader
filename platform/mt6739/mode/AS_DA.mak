# Reduce preloader size
MTK_SECURITY_SW_SUPPORT=no
CFG_EMERGENCY_DL_SUPPORT :=0
CFG_UART_TOOL_HANDSHAKE :=0
CFG_PRELOADER_EXTENSION :=0
CFG_DRAM_CALIB_OPTIMIZATION :=0
MTK_EFUSE_WRITER_RESERVE_CODESIZE :=no
# CFG_TEE_SUPPORT :=0

# Enable the feature
CFG_ATF_SUPPORT :=1
CFG_PRELOADER_AS_DA :=1

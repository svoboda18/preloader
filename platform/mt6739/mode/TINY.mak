HW_INIT_ONLY :=1
CFG_LOAD_MD_ROM :=0
CFG_LOAD_MD_RAMDISK :=0
CFG_LOAD_AP_ROM :=0
CFG_LOAD_UBOOT :=0
CFG_ATF_SUPPORT :=0

# If this feature is enabled, there is no UART log between log_buf_ctrl(1) and
# bldr_wait_forever(), since log buffer is switched to DRAM.
CFG_UART_TOOL_HANDSHAKE :=0

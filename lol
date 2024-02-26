/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
=================================================================
Building Configuration:
Project = k80_bsp
Platform = MT6580
Buildspec = buildspec.mak
Create SecLib = FALSE
Image Auth key exist = FALSE
Preloader EXT exist = FALSE
SECRO AC support = yes
=================================================================
=================================================================
Internal Feature:
HW_INIT_ONLY = 0
CFG_MDJTAG_SWITCH=
CFG_MDMETA_DETECT= 0
CFG_HW_WATCHDOG= 1
CFG_APWDT=
CFG_APWDT_DISABLE= 0
CFG_MDWDT_DISABLE= 0
CFG_SYS_STACK_SZ= 3*1024
CFG_MMC_ADDR_TRANS= 1
CFG_BOOT_ARGUMENT= 1
CFG_RAM_CONSOLE= 1
=================================================================
MTK_PATH_PLATFORM= /home/svoboda18/Desktop/android/mtk/preloader/platform/mt6580
MTK_PATH_CUSTOM= /home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp
MTK_ROOT_CUSTOM= /home/svoboda18/Desktop/android/mtk/preloader/custom
CUSTOM_PATH= /home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/security
=================================================================
===========================================
Platform Feature:
CFG_FPGA_PLATFORM= 0
CFG_EVB_PLATFORM= 0
CFG_BATTERY_DETECT= 1
CFG_PMT_SUPPORT= 0
===========================================
Communication Feature:
CFG_UART_TOOL_HANDSHAKE= 1
CFG_USB_TOOL_HANDSHAKE= 1
CFG_USB_DOWNLOAD= 1
CFG_LOG_BAUDRATE= 921600
CFG_META_BAUDRATE= 115200
CFG_UART_LOG= UART1
CFG_UART_META= UART1
CFG_EMERGENCY_DL_SUPPORT= 1
CFG_EMERGENCY_DL_TIMEOUT_MS= 1000*5
MTK_UART_USB_SWITCH= yes
===========================================
Image Loading:
CFG_LOAD_UBOOT= 1
CFG_LOAD_AP_ROM= 0
CFG_LOAD_MD_ROM= 0
CFG_LOAD_MD_RAMDISK= 0
CFG_LOAD_CONN_SYS= 0
CFG_UBOOT_MEMADDR= 0x81E00000
CFG_AP_ROM_MEMADDR= 0x86000000
CFG_MD1_ROM_MEMADDR= 0x80000000
CFG_MD1_RAMDISK_MEMADDR= 0x81400000
CFG_MD2_ROM_MEMADDR= 0x82000000
CFG_MD2_RAMDISK_MEMADDR= 0x83400000
CFG_CONN_SYS_MEMADDR= 0x85A00000
ONEKEY_REBOOT_NORMAL_MODE_PL= 1
KPD_USE_EXTEND_TYPE=
KPD_PMIC_LPRST_TD= 1
MTK_PMIC_RST_KEY=
CFG_ATF_SUPPORT= 0
CFG_ATF_LOG_SUPPORT= 0
CFG_TEE_SUPPORT= 0
CFG_TEE_VIRTUAL_RPMB_SUPPORT=
CFG_RPMB_SET_KEY=
CFG_TRUSTONIC_TEE_SUPPORT= 0
CFG_MICROTRUST_TEE_SUPPORT= 0
CFG_MICROTRUST_TEE_LITE_SUPPORT= 0
CFG_TRUSTKERNEL_TEE_SUPPORT=
CFG_TEE_SECURE_MEM_PROTECTED= 1
CFG_TEE_SECURE_MEM_TAG_FORMAT=
CFG_TEE_SECURE_MEM_SHARED=
CFG_TEE_TRUSTED_APP_HEAP_SIZE= 0xA00000
CFG_RPMB_KEY_PROGRAMED_IN_KERNEL=
MTK_EFUSE_WRITER_SUPPORT=
CFG_GOOGLE_TRUSTY_SUPPORT= 0
CFG_GZ_SUPPORT=
CFG_GZ_NEED_DESCRAMBLE=
CFG_GZ_REMAP=
CFG_NEBULA_VM_SUPPORT=
CFG_NEBULA_LOAD_IN_PART2=
CFG_GZ_PWRAP_ENABLE=
make -C /home/svoboda18/Desktop/android/mtk/preloader/platform/mt6580/src/ --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make -e -r -C security --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make -e -r -C auth --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make[3]: Nothing to be done for 'all'.
echo "Build-k80/obj/sec.o Build-k80/obj/sec_boot.o Build-k80/obj/sec_ctrl.o Build-k80/obj/sec_rom_info.o Build-k80/obj/sec_secroimg.o Build-k80/obj/sec_util.o Build-k80/obj/sec_region.o Build-k80/obj/sec_flashtool_cfg.o Build-k80/obj/verified_boot_dummy.o Build-k80/obj/platform_mt6580_src_security_auth.flag" > Build-k80/obj/platform_mt6580_src_security.flag
make -e -r -C core --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make[2]: Nothing to be done for 'all'.
make -e -r -C drivers --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make[2]: *** No rule to make target 'Build-k80/inc/custom_emi.h', needed by 'Build-k80/obj/emi.o'.  Stop.
make[1]: *** [/home/svoboda18/Desktop/android/mtk/preloader/build/common-dir.mak:58: Build-k80/obj/platform_mt6580_src_drivers.flag] Error 2
make: *** [Makefile:219: Build-k80/obj/platform_mt6580_src.flag] Error 2
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
=================================================================
Building Configuration:
Project = k80_bsp
Platform = MT6580
Buildspec = buildspec.mak
Create SecLib = FALSE
Image Auth key exist = FALSE
Preloader EXT exist = FALSE
SECRO AC support = yes
=================================================================
=================================================================
Internal Feature:
HW_INIT_ONLY = 0
CFG_MDJTAG_SWITCH=
CFG_MDMETA_DETECT= 0
CFG_HW_WATCHDOG= 1
CFG_APWDT=
CFG_APWDT_DISABLE= 0
CFG_MDWDT_DISABLE= 0
CFG_SYS_STACK_SZ= 3*1024
CFG_MMC_ADDR_TRANS= 1
CFG_BOOT_ARGUMENT= 1
CFG_RAM_CONSOLE= 1
=================================================================
MTK_PATH_PLATFORM= /home/svoboda18/Desktop/android/mtk/preloader/platform/mt6580
MTK_PATH_CUSTOM= /home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp
MTK_ROOT_CUSTOM= /home/svoboda18/Desktop/android/mtk/preloader/custom
CUSTOM_PATH= /home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/security
=================================================================
===========================================
Platform Feature:
CFG_FPGA_PLATFORM= 0
CFG_EVB_PLATFORM= 0
CFG_BATTERY_DETECT= 1
CFG_PMT_SUPPORT= 0
===========================================
Communication Feature:
CFG_UART_TOOL_HANDSHAKE= 1
CFG_USB_TOOL_HANDSHAKE= 1
CFG_USB_DOWNLOAD= 1
CFG_LOG_BAUDRATE= 921600
CFG_META_BAUDRATE= 115200
CFG_UART_LOG= UART1
CFG_UART_META= UART1
CFG_EMERGENCY_DL_SUPPORT= 1
CFG_EMERGENCY_DL_TIMEOUT_MS= 1000*5
MTK_UART_USB_SWITCH= yes
===========================================
Image Loading:
CFG_LOAD_UBOOT= 1
CFG_LOAD_AP_ROM= 0
CFG_LOAD_MD_ROM= 0
CFG_LOAD_MD_RAMDISK= 0
CFG_LOAD_CONN_SYS= 0
CFG_UBOOT_MEMADDR= 0x81E00000
CFG_AP_ROM_MEMADDR= 0x86000000
CFG_MD1_ROM_MEMADDR= 0x80000000
CFG_MD1_RAMDISK_MEMADDR= 0x81400000
CFG_MD2_ROM_MEMADDR= 0x82000000
CFG_MD2_RAMDISK_MEMADDR= 0x83400000
CFG_CONN_SYS_MEMADDR= 0x85A00000
ONEKEY_REBOOT_NORMAL_MODE_PL= 1
KPD_USE_EXTEND_TYPE=
KPD_PMIC_LPRST_TD= 1
MTK_PMIC_RST_KEY=
CFG_ATF_SUPPORT= 0
CFG_ATF_LOG_SUPPORT= 0
CFG_TEE_SUPPORT= 0
CFG_TEE_VIRTUAL_RPMB_SUPPORT=
CFG_RPMB_SET_KEY=
CFG_TRUSTONIC_TEE_SUPPORT= 0
CFG_MICROTRUST_TEE_SUPPORT= 0
CFG_MICROTRUST_TEE_LITE_SUPPORT= 0
CFG_TRUSTKERNEL_TEE_SUPPORT=
CFG_TEE_SECURE_MEM_PROTECTED= 1
CFG_TEE_SECURE_MEM_TAG_FORMAT=
CFG_TEE_SECURE_MEM_SHARED=
CFG_TEE_TRUSTED_APP_HEAP_SIZE= 0xA00000
CFG_RPMB_KEY_PROGRAMED_IN_KERNEL=
MTK_EFUSE_WRITER_SUPPORT=
CFG_GOOGLE_TRUSTY_SUPPORT= 0
CFG_GZ_SUPPORT=
CFG_GZ_NEED_DESCRAMBLE=
CFG_GZ_REMAP=
CFG_NEBULA_VM_SUPPORT=
CFG_NEBULA_LOAD_IN_PART2=
CFG_GZ_PWRAP_ENABLE=
make -C /home/svoboda18/Desktop/android/mtk/preloader/platform/mt6580/src/ --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make -e -r -C security --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make -e -r -C auth --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make[3]: Nothing to be done for 'all'.
echo "Build-k80/obj/sec.o Build-k80/obj/sec_boot.o Build-k80/obj/sec_ctrl.o Build-k80/obj/sec_rom_info.o Build-k80/obj/sec_secroimg.o Build-k80/obj/sec_util.o Build-k80/obj/sec_region.o Build-k80/obj/sec_flashtool_cfg.o Build-k80/obj/verified_boot_dummy.o Build-k80/obj/platform_mt6580_src_security_auth.flag" > Build-k80/obj/platform_mt6580_src_security.flag
make -e -r -C core --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make[2]: Nothing to be done for 'all'.
make -e -r -C drivers --no-print-directory
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:53: CFG_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:54: CFG_TRUSTONIC_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:55: CFG_WATCHDATA_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:56: CFG_MICROTRUST_TEE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:57: CFG_MICROTRUST_TEE_LITE_SUPPORT=0
/home/svoboda18/Desktop/android/mtk/preloader/custom/k80_bsp/cust_bldr.mak:58: CFG_GOOGLE_TRUSTY_SUPPORT=0
make[2]: *** No rule to make target 'Build-k80/inc/custom_emi.h', needed by 'Build-k80/obj/emi.o'.  Stop.
make[1]: *** [/home/svoboda18/Desktop/android/mtk/preloader/build/common-dir.mak:58: Build-k80/obj/platform_mt6580_src_drivers.flag] Error 2
make: *** [Makefile:219: Build-k80/obj/platform_mt6580_src.flag] Error 2

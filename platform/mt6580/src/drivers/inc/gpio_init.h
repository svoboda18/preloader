#ifndef _GPIO_INIT_H_
#define _GPIO_INIT_H_

/******************************************************************************
 * mt_gpio_init_value.c - MT6516 Linux GPIO Device Driver
 * 
 * Copyright 2008-2009 MediaTek Co.,Ltd.
 * 
 * DESCRIPTION:
 *     default GPIO init value
 *
 ******************************************************************************/

const UINT32 gpio_init_value[][3] = {
{
IO_CFG_T_BASE+0x054
,(GPIO92_SMT << 25) | (GPIO91_SMT << 24) | (GPIO90_SMT << 23) | (GPIO89_SMT << 22) | (GPIO88_SMT << 21) | (GPIO87_SMT << 20) | (GPIO86_SMT << 19) | (GPIO85_SMT << 18) | (GPIO84_SMT << 17) | (GPIO83_SMT << 16) | (GPIO82_SMT << 15) | (GPIO81_SMT << 14) | (GPIO80_SMT << 13) | (GPIO79_SMT << 12) | (GPIO69_SMT << 10) | (GPIO67_SMT << 7) | (GPIO65_SMT << 5) | (GPIO64_SMT << 4) | (GPIO63_SMT << 11) | (GPIO62_SMT << 9) | (GPIO60_SMT << 8) | (GPIO59_SMT << 6) | (GPIO3_SMT << 3) | (GPIO2_SMT << 2) | (GPIO1_SMT << 1) | (GPIO0_SMT << 0)
,(0x1 << 25) | (0x1 << 24) | (0x1 << 23) | (0x1 << 22) | (0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 10) | (0x1 << 7) | (0x1 << 5) | (0x1 << 4) | (0x1 << 11) | (0x1 << 9) | (0x1 << 8) | (0x1 << 6) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_R_BASE+0x054
,(GPIO76_SMT << 21) | (GPIO75_SMT << 20) | (GPIO74_SMT << 19) | (GPIO73_SMT << 18) | (GPIO72_SMT << 17) | (GPIO71_SMT << 16) | (GPIO70_SMT << 13) | (GPIO68_SMT << 12) | (GPIO66_SMT << 11) | (GPIO61_SMT << 15) | (GPIO58_SMT << 14) | (GPIO57_SMT << 5) | (GPIO56_SMT << 4) | (GPIO55_SMT << 3) | (GPIO54_SMT << 2) | (GPIO53_SMT << 1) | (GPIO52_SMT << 0) | (GPIO8_SMT << 10) | (GPIO7_SMT << 9) | (GPIO6_SMT << 8) | (GPIO5_SMT << 7) | (GPIO4_SMT << 6)
,(0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 17) | (0x1 << 16) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 15) | (0x1 << 14) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6)
},
{
IO_CFG_B_BASE+0x054
,(GPIO78_SMT << 1) | (GPIO77_SMT << 0) | (GPIO51_SMT << 19) | (GPIO50_SMT << 10) | (GPIO49_SMT << 11) | (GPIO48_SMT << 12) | (GPIO47_SMT << 13) | (GPIO46_SMT << 14) | (GPIO45_SMT << 15) | (GPIO44_SMT << 16) | (GPIO43_SMT << 17) | (GPIO42_SMT << 20) | (GPIO41_SMT << 18) | (GPIO21_SMT << 25) | (GPIO20_SMT << 24) | (GPIO19_SMT << 23) | (GPIO18_SMT << 22) | (GPIO17_SMT << 21) | (GPIO16_SMT << 9) | (GPIO15_SMT << 8) | (GPIO14_SMT << 7) | (GPIO13_SMT << 6) | (GPIO12_SMT << 5) | (GPIO11_SMT << 4) | (GPIO10_SMT << 3) | (GPIO9_SMT << 2)
,(0x1 << 1) | (0x1 << 0) | (0x1 << 19) | (0x1 << 10) | (0x1 << 11) | (0x1 << 12) | (0x1 << 13) | (0x1 << 14) | (0x1 << 15) | (0x1 << 16) | (0x1 << 17) | (0x1 << 20) | (0x1 << 18) | (0x1 << 25) | (0x1 << 24) | (0x1 << 23) | (0x1 << 22) | (0x1 << 21) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2)
},
{
IO_CFG_L_BASE+0x030
,(GPIO34_SMT << 13) | (GPIO33_SMT << 4) | (GPIO32_SMT << 8) | (GPIO31_SMT << 10) | (GPIO30_SMT << 11) | (GPIO29_SMT << 9) | (GPIO28_SMT << 7) | (GPIO27_SMT << 6) | (GPIO26_SMT << 5) | (GPIO25_SMT << 3) | (GPIO24_SMT << 2) | (GPIO23_SMT << 1) | (GPIO22_SMT << 0)
,(0x1 << 13) | (0x1 << 4) | (0x1 << 8) | (0x1 << 10) | (0x1 << 11) | (0x1 << 9) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_T_BASE+0x000
,(GPIO92_PULLEN << 17) | (GPIO91_PULLEN << 16) | (GPIO90_PULLEN << 15) | (GPIO89_PULLEN << 14) | (GPIO88_PULLEN << 13) | (GPIO87_PULLEN << 12) | (GPIO86_PULLEN << 11) | (GPIO85_PULLEN << 10) | (GPIO84_PULLEN << 9) | (GPIO83_PULLEN << 8) | (GPIO82_PULLEN << 7) | (GPIO81_PULLEN << 6) | (GPIO80_PULLEN << 5) | (GPIO79_PULLEN << 4) | (GPIO63_PULLEN << 21) | (GPIO62_PULLEN << 20) | (GPIO60_PULLEN << 19) | (GPIO59_PULLEN << 18) | (GPIO3_PULLEN << 3) | (GPIO2_PULLEN << 2) | (GPIO1_PULLEN << 1) | (GPIO0_PULLEN << 0)
,(0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_R_BASE+0x000
,(GPIO76_PULLEN << 15) | (GPIO75_PULLEN << 14) | (GPIO74_PULLEN << 13) | (GPIO73_PULLEN << 12) | (GPIO72_PULLEN << 11) | (GPIO71_PULLEN << 10) | (GPIO70_PULLEN << 7) | (GPIO68_PULLEN << 6) | (GPIO66_PULLEN << 5) | (GPIO61_PULLEN << 9) | (GPIO58_PULLEN << 8) | (GPIO8_PULLEN << 4) | (GPIO7_PULLEN << 3) | (GPIO6_PULLEN << 2) | (GPIO5_PULLEN << 1) | (GPIO4_PULLEN << 0)
,(0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 9) | (0x1 << 8) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_B_BASE+0x000
,(GPIO78_PULLEN << 1) | (GPIO77_PULLEN << 0) | (GPIO21_PULLEN << 14) | (GPIO20_PULLEN << 13) | (GPIO19_PULLEN << 12) | (GPIO18_PULLEN << 11) | (GPIO17_PULLEN << 10) | (GPIO16_PULLEN << 9) | (GPIO15_PULLEN << 8) | (GPIO14_PULLEN << 7) | (GPIO13_PULLEN << 6) | (GPIO12_PULLEN << 5) | (GPIO11_PULLEN << 4) | (GPIO10_PULLEN << 3) | (GPIO9_PULLEN << 2)
,(0x1 << 1) | (0x1 << 0) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2)
},
{
IO_CFG_L_BASE+0x000
,(GPIO34_PULLEN << 14) | (GPIO33_PULLEN << 5) | (GPIO32_PULLEN << 9) | (GPIO31_PULLEN << 11) | (GPIO30_PULLEN << 12) | (GPIO29_PULLEN << 10) | (GPIO28_PULLEN << 8) | (GPIO27_PULLEN << 7) | (GPIO26_PULLEN << 6) | (GPIO25_PULLEN << 3) | (GPIO24_PULLEN << 2) | (GPIO23_PULLEN << 1) | (GPIO22_PULLEN << 0)
,(0x1 << 14) | (0x1 << 5) | (0x1 << 9) | (0x1 << 11) | (0x1 << 12) | (0x1 << 10) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_R_BASE+0x0A8
,((GPIO40_MODE==GPIO_MODE_01)?0:(GPIO40_PULLEN << 5)) | ((GPIO39_MODE==GPIO_MODE_01)?0:(GPIO39_PULLEN << 4)) | ((GPIO38_MODE==GPIO_MODE_01)?0:(GPIO38_PULLEN << 3)) | ((GPIO37_MODE==GPIO_MODE_01)?0:(GPIO37_PULLEN << 2)) | ((GPIO36_MODE==GPIO_MODE_01)?0:(GPIO36_PULLEN << 1)) | ((GPIO35_MODE==GPIO_MODE_01)?0:(GPIO35_PULLEN << 0))
,((GPIO40_MODE==GPIO_MODE_01)?0:(0x1 << 5)) | ((GPIO39_MODE==GPIO_MODE_01)?0:(0x1 << 4)) | ((GPIO38_MODE==GPIO_MODE_01)?0:(0x1 << 3)) | ((GPIO37_MODE==GPIO_MODE_01)?0:(0x1 << 2)) | ((GPIO36_MODE==GPIO_MODE_01)?0:(0x1 << 1)) | ((GPIO35_MODE==GPIO_MODE_01)?0:(0x1 << 0))
},
{
IO_CFG_B_BASE+0x030
,(GPIO51_PULLEN << 9) | (GPIO50_PULLEN << 0) | (GPIO49_PULLEN << 1) | (GPIO48_PULLEN << 2) | (GPIO47_PULLEN << 3) | (GPIO46_PULLEN << 4) | (GPIO45_PULLEN << 5) | (GPIO44_PULLEN << 6) | (GPIO43_PULLEN << 7) | (GPIO42_PULLEN << 10) | (GPIO41_PULLEN << 8)
,(0x1 << 9) | (0x1 << 0) | (0x1 << 1) | (0x1 << 2) | (0x1 << 3) | (0x1 << 4) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7) | (0x1 << 10) | (0x1 << 8)
},
{
IO_CFG_B_BASE+0x024
,(GPIO51_PULLEN << 9) | (GPIO50_PULLEN << 0) | (GPIO49_PULLEN << 1) | (GPIO48_PULLEN << 2) | (GPIO47_PULLEN << 3) | (GPIO46_PULLEN << 4) | (GPIO45_PULLEN << 5) | (GPIO44_PULLEN << 6) | (GPIO43_PULLEN << 7) | (GPIO42_PULLEN << 10) | (GPIO41_PULLEN << 8)
,(0x1 << 9) | (0x1 << 0) | (0x1 << 1) | (0x1 << 2) | (0x1 << 3) | (0x1 << 4) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7) | (0x1 << 10) | (0x1 << 8)
},
{
IO_CFG_R_BASE+0x030
,(GPIO57_PULLEN << 5) | (GPIO56_PULLEN << 4) | (GPIO55_PULLEN << 3) | (GPIO54_PULLEN << 2) | (GPIO53_PULLEN << 1) | (GPIO52_PULLEN << 0)
,(0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_R_BASE+0x024
,(GPIO57_PULLEN << 5) | (GPIO56_PULLEN << 4) | (GPIO55_PULLEN << 3) | (GPIO54_PULLEN << 2) | (GPIO53_PULLEN << 1) | (GPIO52_PULLEN << 0)
,(0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_T_BASE+0x030
,(GPIO69_PULLEN << 3) | (GPIO67_PULLEN << 2) | (GPIO65_PULLEN << 1) | (GPIO64_PULLEN << 0)
,(0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_T_BASE+0x024
,(GPIO69_PULLEN << 3) | (GPIO67_PULLEN << 2) | (GPIO65_PULLEN << 1) | (GPIO64_PULLEN << 0)
,(0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_T_BASE+0x00C
,(GPIO92_PULL << 17) | (GPIO91_PULL << 16) | (GPIO90_PULL << 15) | (GPIO89_PULL << 14) | (GPIO88_PULL << 13) | (GPIO87_PULL << 12) | (GPIO86_PULL << 11) | (GPIO85_PULL << 10) | (GPIO84_PULL << 9) | (GPIO83_PULL << 8) | (GPIO82_PULL << 7) | (GPIO81_PULL << 6) | (GPIO80_PULL << 5) | (GPIO79_PULL << 4) | (GPIO3_PULL << 3) | (GPIO2_PULL << 2) | (GPIO1_PULL << 1) | (GPIO0_PULL << 0)
,(0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_R_BASE+0x00C
,(GPIO76_PULL << 15) | (GPIO75_PULL << 14) | (GPIO74_PULL << 13) | (GPIO73_PULL << 12) | (GPIO72_PULL << 11) | (GPIO71_PULL << 10) | (GPIO70_PULL << 7) | (GPIO68_PULL << 6) | (GPIO66_PULL << 5) | (GPIO61_PULL << 9) | (GPIO58_PULL << 8) | (GPIO8_PULL << 4) | (GPIO7_PULL << 3) | (GPIO6_PULL << 2) | (GPIO5_PULL << 1) | (GPIO4_PULL << 0)
,(0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 9) | (0x1 << 8) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_B_BASE+0x00C
,(GPIO78_PULL << 1) | (GPIO77_PULL << 0) | (GPIO21_PULL << 14) | (GPIO20_PULL << 13) | (GPIO19_PULL << 12) | (GPIO18_PULL << 11) | (GPIO17_PULL << 10) | (GPIO16_PULL << 9) | (GPIO15_PULL << 8) | (GPIO14_PULL << 7) | (GPIO13_PULL << 6) | (GPIO12_PULL << 5) | (GPIO11_PULL << 4) | (GPIO10_PULL << 3) | (GPIO9_PULL << 2)
,(0x1 << 1) | (0x1 << 0) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2)
},
{
IO_CFG_L_BASE+0x00C
,(GPIO34_PULL << 14) | (GPIO33_PULL << 5) | (GPIO32_PULL << 9) | (GPIO31_PULL << 11) | (GPIO30_PULL << 12) | (GPIO29_PULL << 10) | (GPIO28_PULL << 8) | (GPIO27_PULL << 7) | (GPIO26_PULL << 6) | (GPIO25_PULL << 3) | (GPIO24_PULL << 2) | (GPIO23_PULL << 1) | (GPIO22_PULL << 0)
,(0x1 << 14) | (0x1 << 5) | (0x1 << 9) | (0x1 << 11) | (0x1 << 12) | (0x1 << 10) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_R_BASE+0x0B4
,((GPIO40_MODE==GPIO_MODE_01)?0:(GPIO40_PULL << 5)) | ((GPIO39_MODE==GPIO_MODE_01)?0:(GPIO39_PULL << 4)) | ((GPIO38_MODE==GPIO_MODE_01)?0:(GPIO38_PULL << 3)) | ((GPIO37_MODE==GPIO_MODE_01)?0:(GPIO37_PULL << 2)) | ((GPIO36_MODE==GPIO_MODE_01)?0:(GPIO36_PULL << 1)) | ((GPIO35_MODE==GPIO_MODE_01)?0:(GPIO35_PULL << 0))
,((GPIO40_MODE==GPIO_MODE_01)?0:(0x1 << 5)) | ((GPIO39_MODE==GPIO_MODE_01)?0:(0x1 << 4)) | ((GPIO38_MODE==GPIO_MODE_01)?0:(0x1 << 3)) | ((GPIO37_MODE==GPIO_MODE_01)?0:(0x1 << 2)) | ((GPIO36_MODE==GPIO_MODE_01)?0:(0x1 << 1)) | ((GPIO35_MODE==GPIO_MODE_01)?0:(0x1 << 0))
},
{
IO_CFG_B_BASE+0x018
,((GPIO51_PULL << 9) | (GPIO50_PULL << 0) | (GPIO49_PULL << 1) | (GPIO48_PULL << 2) | (GPIO47_PULL << 3) | (GPIO46_PULL << 4) | (GPIO45_PULL << 5) | (GPIO44_PULL << 6) | (GPIO43_PULL << 7) | (GPIO42_PULL << 10) | (GPIO41_PULL << 8))^((0x1 << 9) | (0x1 << 0) | (0x1 << 1) | (0x1 << 2) | (0x1 << 3) | (0x1 << 4) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7) | (0x1 << 10) | (0x1 << 8))
,(0x1 << 9) | (0x1 << 0) | (0x1 << 1) | (0x1 << 2) | (0x1 << 3) | (0x1 << 4) | (0x1 << 5) | (0x1 << 6) | (0x1 << 7) | (0x1 << 10) | (0x1 << 8)
},
{
IO_CFG_R_BASE+0x018
,((GPIO57_PULL << 5) | (GPIO56_PULL << 4) | (GPIO55_PULL << 3) | (GPIO54_PULL << 2) | (GPIO53_PULL << 1) | (GPIO52_PULL << 0))^((0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0))
,(0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
IO_CFG_T_BASE+0x018
,((GPIO69_PULL << 3) | (GPIO67_PULL << 2) | (GPIO65_PULL << 1) | (GPIO64_PULL << 0))^((0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0))
,(0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
GPIO_BASE+0x054
,(GPIO7_MODE << 28) | (GPIO6_MODE << 24) | (GPIO5_MODE << 20) | (GPIO4_MODE << 16) | (GPIO3_MODE << 12) | (GPIO2_MODE << 8) | (GPIO1_MODE << 4) | (GPIO0_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x064
,(GPIO15_MODE << 28) | (GPIO14_MODE << 24) | (GPIO13_MODE << 20) | (GPIO12_MODE << 16) | (GPIO11_MODE << 12) | (GPIO10_MODE << 8) | (GPIO9_MODE << 4) | (GPIO8_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x074
,(GPIO23_MODE << 28) | (GPIO22_MODE << 24) | (GPIO21_MODE << 20) | (GPIO20_MODE << 16) | (GPIO19_MODE << 12) | (GPIO18_MODE << 8) | (GPIO17_MODE << 4) | (GPIO16_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x084
,(GPIO31_MODE << 28) | (GPIO30_MODE << 24) | (GPIO29_MODE << 20) | (GPIO28_MODE << 16) | (GPIO27_MODE << 12) | (GPIO26_MODE << 8) | (GPIO25_MODE << 4) | (GPIO24_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x094
,((GPIO39_MODE==GPIO_MODE_01)?0:(GPIO39_MODE << 28)) | ((GPIO38_MODE==GPIO_MODE_01)?0:(GPIO38_MODE << 24)) | ((GPIO37_MODE==GPIO_MODE_01)?0:(GPIO37_MODE << 20)) | ((GPIO36_MODE==GPIO_MODE_01)?0:(GPIO36_MODE << 16)) | ((GPIO35_MODE==GPIO_MODE_01)?0:(GPIO35_MODE << 12)) | (GPIO34_MODE << 8) | (GPIO33_MODE << 4) | (GPIO32_MODE << 0)
,((GPIO39_MODE==GPIO_MODE_01)?0:(0xF << 28)) | ((GPIO38_MODE==GPIO_MODE_01)?0:(0xF << 24)) | ((GPIO37_MODE==GPIO_MODE_01)?0:(0xF << 20)) | ((GPIO36_MODE==GPIO_MODE_01)?0:(0xF << 16)) | ((GPIO35_MODE==GPIO_MODE_01)?0:(0xF << 12)) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x0A4
,(GPIO47_MODE << 28) | (GPIO46_MODE << 24) | (GPIO45_MODE << 20) | (GPIO44_MODE << 16) | (GPIO43_MODE << 12) | (GPIO42_MODE << 8) | (GPIO41_MODE << 4) | ((GPIO40_MODE==GPIO_MODE_01)?0:(GPIO40_MODE << 0))
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | ((GPIO40_MODE==GPIO_MODE_01)?0:(0xF << 0))
},
{
GPIO_BASE+0x0B4
,(GPIO55_MODE << 28) | (GPIO54_MODE << 24) | (GPIO53_MODE << 20) | (GPIO52_MODE << 16) | (GPIO51_MODE << 12) | (GPIO50_MODE << 8) | (GPIO49_MODE << 4) | (GPIO48_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x0C4
,(GPIO63_MODE << 28) | (GPIO62_MODE << 24) | (GPIO61_MODE << 20) | (GPIO60_MODE << 16) | (GPIO59_MODE << 12) | (GPIO58_MODE << 8) | (GPIO57_MODE << 4) | (GPIO56_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x0D4
,(GPIO71_MODE << 28) | (GPIO70_MODE << 24) | (GPIO69_MODE << 20) | (GPIO68_MODE << 16) | (GPIO67_MODE << 12) | (GPIO66_MODE << 8) | (GPIO65_MODE << 4) | (GPIO64_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x0E4
,(GPIO79_MODE << 28) | (GPIO78_MODE << 24) | (GPIO77_MODE << 20) | (GPIO76_MODE << 16) | (GPIO75_MODE << 12) | (GPIO74_MODE << 8) | (GPIO73_MODE << 4) | (GPIO72_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x0F4
,(GPIO87_MODE << 28) | (GPIO86_MODE << 24) | (GPIO85_MODE << 20) | (GPIO84_MODE << 16) | (GPIO83_MODE << 12) | (GPIO82_MODE << 8) | (GPIO81_MODE << 4) | (GPIO80_MODE << 0)
,(0xF << 28) | (0xF << 24) | (0xF << 20) | (0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x104
,(GPIO92_MODE << 16) | (GPIO91_MODE << 12) | (GPIO90_MODE << 8) | (GPIO89_MODE << 4) | (GPIO88_MODE << 0)
,(0xF << 16) | (0xF << 12) | (0xF << 8) | (0xF << 4) | (0xF << 0)
},
{
GPIO_BASE+0x024
,(GPIO31_DATAOUT << 31) | (GPIO30_DATAOUT << 30) | (GPIO29_DATAOUT << 29) | (GPIO28_DATAOUT << 28) | (GPIO27_DATAOUT << 27) | (GPIO26_DATAOUT << 26) | (GPIO25_DATAOUT << 25) | (GPIO24_DATAOUT << 24) | (GPIO23_DATAOUT << 23) | (GPIO22_DATAOUT << 22) | (GPIO21_DATAOUT << 21) | (GPIO20_DATAOUT << 20) | (GPIO19_DATAOUT << 19) | (GPIO18_DATAOUT << 18) | (GPIO17_DATAOUT << 17) | (GPIO16_DATAOUT << 16) | (GPIO15_DATAOUT << 15) | (GPIO14_DATAOUT << 14) | (GPIO13_DATAOUT << 13) | (GPIO12_DATAOUT << 12) | (GPIO11_DATAOUT << 11) | (GPIO10_DATAOUT << 10) | (GPIO9_DATAOUT << 9) | (GPIO8_DATAOUT << 8) | (GPIO7_DATAOUT << 7) | (GPIO6_DATAOUT << 6) | (GPIO5_DATAOUT << 5) | (GPIO4_DATAOUT << 4) | (GPIO3_DATAOUT << 3) | (GPIO2_DATAOUT << 2) | (GPIO1_DATAOUT << 1) | (GPIO0_DATAOUT << 0)
,(0x1 << 31) | (0x1 << 30) | (0x1 << 29) | (0x1 << 28) | (0x1 << 27) | (0x1 << 26) | (0x1 << 25) | (0x1 << 24) | (0x1 << 23) | (0x1 << 22) | (0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
GPIO_BASE+0x030
,(GPIO61_DATAOUT << 29) | (GPIO58_DATAOUT << 26) | (GPIO57_DATAOUT << 25) | (GPIO56_DATAOUT << 24) | (GPIO55_DATAOUT << 23) | (GPIO54_DATAOUT << 22) | (GPIO53_DATAOUT << 21) | (GPIO52_DATAOUT << 20) | (GPIO51_DATAOUT << 19) | (GPIO50_DATAOUT << 18) | (GPIO49_DATAOUT << 17) | (GPIO48_DATAOUT << 16) | (GPIO47_DATAOUT << 15) | (GPIO46_DATAOUT << 14) | (GPIO45_DATAOUT << 13) | (GPIO44_DATAOUT << 12) | (GPIO43_DATAOUT << 11) | (GPIO42_DATAOUT << 10) | (GPIO41_DATAOUT << 9) | (GPIO40_DATAOUT << 8) | (GPIO39_DATAOUT << 7) | (GPIO38_DATAOUT << 6) | (GPIO37_DATAOUT << 5) | (GPIO36_DATAOUT << 4) | (GPIO35_DATAOUT << 3) | (GPIO34_DATAOUT << 2) | (GPIO33_DATAOUT << 1) | (GPIO32_DATAOUT << 0)
,(0x1 << 29) | (0x1 << 26) | (0x1 << 25) | (0x1 << 24) | (0x1 << 23) | (0x1 << 22) | (0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
GPIO_BASE+0x03C
,(GPIO92_DATAOUT << 28) | (GPIO91_DATAOUT << 27) | (GPIO90_DATAOUT << 26) | (GPIO89_DATAOUT << 25) | (GPIO88_DATAOUT << 24) | (GPIO87_DATAOUT << 23) | (GPIO86_DATAOUT << 22) | (GPIO85_DATAOUT << 21) | (GPIO84_DATAOUT << 20) | (GPIO83_DATAOUT << 19) | (GPIO82_DATAOUT << 18) | (GPIO81_DATAOUT << 17) | (GPIO80_DATAOUT << 16) | (GPIO79_DATAOUT << 15) | (GPIO78_DATAOUT << 14) | (GPIO77_DATAOUT << 13) | (GPIO76_DATAOUT << 12) | (GPIO75_DATAOUT << 11) | (GPIO74_DATAOUT << 10) | (GPIO73_DATAOUT << 9) | (GPIO72_DATAOUT << 8) | (GPIO71_DATAOUT << 7) | (GPIO70_DATAOUT << 6) | (GPIO69_DATAOUT << 5) | (GPIO68_DATAOUT << 4) | (GPIO67_DATAOUT << 3) | (GPIO66_DATAOUT << 2) | (GPIO65_DATAOUT << 1) | (GPIO64_DATAOUT << 0)
,(0x1 << 28) | (0x1 << 27) | (0x1 << 26) | (0x1 << 25) | (0x1 << 24) | (0x1 << 23) | (0x1 << 22) | (0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
GPIO_BASE+0x000
,(GPIO31_DIR << 31) | (GPIO30_DIR << 30) | (GPIO29_DIR << 29) | (GPIO28_DIR << 28) | (GPIO27_DIR << 27) | (GPIO26_DIR << 26) | (GPIO25_DIR << 25) | (GPIO24_DIR << 24) | (GPIO23_DIR << 23) | (GPIO22_DIR << 22) | (GPIO21_DIR << 21) | (GPIO20_DIR << 20) | (GPIO19_DIR << 19) | (GPIO18_DIR << 18) | (GPIO17_DIR << 17) | (GPIO16_DIR << 16) | (GPIO15_DIR << 15) | (GPIO14_DIR << 14) | (GPIO13_DIR << 13) | (GPIO12_DIR << 12) | (GPIO11_DIR << 11) | (GPIO10_DIR << 10) | (GPIO9_DIR << 9) | (GPIO8_DIR << 8) | (GPIO7_DIR << 7) | (GPIO6_DIR << 6) | (GPIO5_DIR << 5) | (GPIO4_DIR << 4) | (GPIO3_DIR << 3) | (GPIO2_DIR << 2) | (GPIO1_DIR << 1) | (GPIO0_DIR << 0)
,(0x1 << 31) | (0x1 << 30) | (0x1 << 29) | (0x1 << 28) | (0x1 << 27) | (0x1 << 26) | (0x1 << 25) | (0x1 << 24) | (0x1 << 23) | (0x1 << 22) | (0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
GPIO_BASE+0x00C
,(GPIO63_DIR << 31) | (GPIO62_DIR << 30) | (GPIO61_DIR << 29) | (GPIO60_DIR << 28) | (GPIO59_DIR << 27) | (GPIO58_DIR << 26) | (GPIO57_DIR << 25) | (GPIO56_DIR << 24) | (GPIO55_DIR << 23) | (GPIO54_DIR << 22) | (GPIO53_DIR << 21) | (GPIO52_DIR << 20) | (GPIO51_DIR << 19) | (GPIO50_DIR << 18) | (GPIO49_DIR << 17) | (GPIO48_DIR << 16) | (GPIO47_DIR << 15) | (GPIO46_DIR << 14) | (GPIO45_DIR << 13) | (GPIO44_DIR << 12) | (GPIO43_DIR << 11) | (GPIO42_DIR << 10) | (GPIO41_DIR << 9) | (GPIO34_DIR << 2) | (GPIO33_DIR << 1) | (GPIO32_DIR << 0)
,(0x1 << 31) | (0x1 << 30) | (0x1 << 29) | (0x1 << 28) | (0x1 << 27) | (0x1 << 26) | (0x1 << 25) | (0x1 << 24) | (0x1 << 23) | (0x1 << 22) | (0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
GPIO_BASE+0x018
,(GPIO92_DIR << 28) | (GPIO91_DIR << 27) | (GPIO90_DIR << 26) | (GPIO89_DIR << 25) | (GPIO88_DIR << 24) | (GPIO87_DIR << 23) | (GPIO86_DIR << 22) | (GPIO85_DIR << 21) | (GPIO84_DIR << 20) | (GPIO83_DIR << 19) | (GPIO82_DIR << 18) | (GPIO81_DIR << 17) | (GPIO80_DIR << 16) | (GPIO79_DIR << 15) | (GPIO78_DIR << 14) | (GPIO77_DIR << 13) | (GPIO76_DIR << 12) | (GPIO75_DIR << 11) | (GPIO74_DIR << 10) | (GPIO73_DIR << 9) | (GPIO72_DIR << 8) | (GPIO71_DIR << 7) | (GPIO70_DIR << 6) | (GPIO69_DIR << 5) | (GPIO68_DIR << 4) | (GPIO67_DIR << 3) | (GPIO66_DIR << 2) | (GPIO65_DIR << 1) | (GPIO64_DIR << 0)
,(0x1 << 28) | (0x1 << 27) | (0x1 << 26) | (0x1 << 25) | (0x1 << 24) | (0x1 << 23) | (0x1 << 22) | (0x1 << 21) | (0x1 << 20) | (0x1 << 19) | (0x1 << 18) | (0x1 << 17) | (0x1 << 16) | (0x1 << 15) | (0x1 << 14) | (0x1 << 13) | (0x1 << 12) | (0x1 << 11) | (0x1 << 10) | (0x1 << 9) | (0x1 << 8) | (0x1 << 7) | (0x1 << 6) | (0x1 << 5) | (0x1 << 4) | (0x1 << 3) | (0x1 << 2) | (0x1 << 1) | (0x1 << 0)
},
{
GPIO_BASE+0x114
,((((GPIO36_MODE==GPIO_MODE_01)?0x0:0x1)) << 13) | ((((GPIO38_MODE==GPIO_MODE_01)?0x0:0x1)) << 15) | ((((GPIO35_MODE==GPIO_MODE_01)?0x0:0x1)) << 12) | ((((GPIO37_MODE==GPIO_MODE_01)?0x0:0x1)) << 14) | ((((GPIO39_MODE==GPIO_MODE_01)?0x0:0x1)) << 16) | ((((GPIO40_MODE==GPIO_MODE_01)?0x0:0x1)) << 17)
,(0x1 << 13) | (0x1 << 15) | (0x1 << 12) | (0x1 << 14) | (0x1 << 16) | (0x1 << 17)
}
};

#endif //_GPIO_INIT_H_

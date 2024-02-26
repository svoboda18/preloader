#ifndef __LINUX_REGULATOR_MT6357_H_
#define __LINUX_REGULATOR_MT6357_H_

#include <regulator/mtk_regulator_core.h>

#define PMIC_PRELOADER 1
#define MT6357_DEBUG 1

#if MT6357_DEBUG
#define PMUTAG                "[MT6357] "
#define mreg_dbg_print(fmt, arg...) dbg_print(PMUTAG fmt, ##arg)
#else
#define mreg_dbg_print(...) do { } while(0)
#endif

typedef enum {
	DISABLE_REGULATOR,
	ENABLE_REGULATOR,
} enset;

typedef enum {
	AUTOMODE,
	PWMMODE,
} modeset;

#ifdef LDO_SUPPORT
typedef enum {
	VOLTOSEL,
	SELTOVOL,
} volseltran;
#endif /*--LDO_SUPPORT--*/

#ifdef LDO_VOTRIM_SUPPORT
typedef enum {
	TRIMTOSEL,
	SELTOTRIM,
} trimseltran;
#endif /*--LDO_VOTRIM_SUPPORT--*/

typedef enum {
	NON_REGULAR_VOLTAGE,
	FIXED_REGULAR_VOLTAGE,
	REGULAR_VOLTAGE,
} regulator_type;

typedef enum {
	ONE_STEP_VOLT,
	TWO_STEP_VOLT,
	SPE_STEP_VOLT,
	NON_STEP_VOLT,
} step_uV_type;
/*
 * BUCK FPWM RG NAME
 * PLEASE GREP CODA GEN "_VOSEL"
 * MASK POS [0x7F]:VMPOS_ONE, [0xF]:VMPOS_TWO, [0x3F]:VMPOS_THREE,
            [0x3]:VMPOS_FOUR, [0x7]:VMPOS_FIVE, NONE:VMPOS_NONE,
 * BIT POS  [0]:VSPOS_ONE, [8]:VSPOS_TWO, NONE:VSPOS_NONE,
 *
 * PLEASE GREP CODA GEN "_EN"
 * BIT POS [0]:EPOS_ONE, NONE:EPOS_NONE
 *
 * PLEASE GREP CODA GEN "FPWM" OR "MODESET"
 * BIT POS [1]:MPOS_ONE,[2]:MPOS_TWO,[3]:MPOS_THREE,[8]:MPOS_FOUR, NONE: MPOS_NONE
 *
 * Gen As Below
 */
typedef enum {
	VSPOS_ONE,
	VSPOS_TWO,
	VSPOS_NONE,
} vol_shift_pos;

typedef enum {
	VMPOS_ONE,
	VMPOS_TWO,
	VMPOS_THREE,
	VMPOS_FOUR,
	VMPOS_FIVE,
	VMPOS_NONE,
} vol_mask_pos;

typedef enum {
	EPOS_ONE,
	EPOS_NONE,
} enable_bit_pos;

typedef enum {
	MPOS_ONE,
	MPOS_TWO,
	MPOS_THREE,
	MPOS_FOUR,
	MPOS_NONE,
} mode_bit_pos;

#ifdef LDO_VOTRIM_SUPPORT
struct mt6357_ldo_trim_info {
	unsigned short trim_reg;
	unsigned short trim_mask;
	unsigned short trim_shift;
	const void *trim_voltages;
	unsigned int trim_size;
};
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
struct mt6357_ldo_info {
	const void *pvoltages;
	const void *idxs;
	unsigned int n_size;
};
#endif /*--LDO_SUPPORT--*/

/*--abstrac the same parameter--*/
struct mt6357_regulator_info {
	unsigned int min_uV;
	unsigned int max_uV;
	unsigned short vol_reg;
	unsigned short enable_reg;
	unsigned short mode_reg;
#ifdef LDO_SUPPORT
	struct mt6357_ldo_info *extinfo;
#endif /*--LDO_SUPPORT--*/
#ifdef LDO_VOTRIM_SUPPORT
	struct mt6357_ldo_trim_info *triminfo;
#endif /*--LDO_VOTRIM_SUPPORT--*/
	vol_mask_pos vmpos;
	vol_shift_pos vspos;
	enable_bit_pos ebpos;
	mode_bit_pos mbpos;
	step_uV_type vtype;
	regulator_type rtype;
};

#ifdef LDO_VOTRIM_SUPPORT
#define mt6357_ldo_trim_decl(_name, trim_array)\
{ \
	.trim_reg = _name##_trim_reg, \
	.trim_mask = _name##_trim_mask, \
	.trim_shift = _name##_trim_shift, \
	.trim_voltages = (void *)(trim_array), \
	.trim_size = ARRAY_SIZE(trim_array),	\
}
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
#define mt6357_ldo_decl(volt_array, idx_array)\
{ \
	.pvoltages = (void *)(volt_array), \
	.idxs = (void *)(idx_array), \
	.n_size = ARRAY_SIZE(volt_array),	\
}
#endif /*--LDO_SUPPORT--*/

#define mt6357_decl(_name)\
{ \
	.min_uV = _name##_min_uV, \
	.max_uV = _name##_max_uV, \
	.vol_reg = _name##_vol_reg, \
	.enable_reg = _name##_enable_reg, \
	.mode_reg = _name##_mode_reg, \
	.vmpos = _name##_vol_mask_pos, \
	.vspos = _name##_vol_shift_pos, \
	.ebpos = _name##_enable_bit_pos, \
	.mbpos = _name##_mode_bit_pos, \
	.vtype = _name##_step_uV_type, \
	.rtype = _name##_volt_type, \
}

/* -------Code Gen Start-------*/
#define vs1_vol_reg         PMIC_RG_BUCK_VS1_VOSEL_ADDR
#define vs1_enable_reg      PMIC_RG_BUCK_VS1_EN_ADDR
#define vs1_mode_reg        PMIC_RG_VS1_MODESET_ADDR
#define vs1_min_uV          1200000
#define vs1_max_uV          2200000
#define vs1_vol_mask_pos    VMPOS_ONE
#define vs1_vol_shift_pos   VSPOS_ONE
#define vs1_enable_bit_pos  EPOS_ONE
#define vs1_mode_bit_pos    MPOS_FOUR
#define vs1_step_uV_type    TWO_STEP_VOLT
#define vs1_volt_type       REGULAR_VOLTAGE
#define vs1_stb             220

#define vmodem_vol_reg         PMIC_RG_BUCK_VMODEM_VOSEL_ADDR
#define vmodem_enable_reg      PMIC_RG_BUCK_VMODEM_EN_ADDR
#define vmodem_mode_reg        PMIC_RG_VMODEM_FPWM_ADDR
#define vmodem_min_uV          500000
#define vmodem_max_uV          1193750
#define vmodem_vol_mask_pos    VMPOS_ONE
#define vmodem_vol_shift_pos   VSPOS_ONE
#define vmodem_enable_bit_pos  EPOS_ONE
#define vmodem_mode_bit_pos    MPOS_FOUR
#define vmodem_step_uV_type    ONE_STEP_VOLT
#define vmodem_volt_type       REGULAR_VOLTAGE
#define vmodem_stb             220

#define vcore_vol_reg         PMIC_RG_BUCK_VCORE_VOSEL_ADDR
#define vcore_enable_reg      PMIC_RG_BUCK_VCORE_EN_ADDR
#define vcore_mode_reg        PMIC_RG_VCORE_FPWM_ADDR
#define vcore_min_uV          518750
#define vcore_max_uV          1312500
//#define vcore_max_uV          1193750
#define vcore_vol_mask_pos    VMPOS_ONE
#define vcore_vol_shift_pos   VSPOS_ONE
#define vcore_enable_bit_pos  EPOS_ONE
#define vcore_mode_bit_pos    MPOS_ONE
#define vcore_step_uV_type    ONE_STEP_VOLT
#define vcore_volt_type       REGULAR_VOLTAGE
#define vcore_stb             220

#define vsram_others_vol_reg         PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_ADDR
#define vsram_others_enable_reg      PMIC_RG_LDO_VSRAM_OTHERS_EN_ADDR
#define vsram_others_mode_reg        0
#define vsram_others_min_uV          518750
#define vsram_others_max_uV          1312500
#define vsram_others_vol_mask_pos    VMPOS_ONE
#define vsram_others_vol_shift_pos   VSPOS_ONE
#define vsram_others_enable_bit_pos  EPOS_ONE
#define vsram_others_mode_bit_pos    MPOS_NONE
#define vsram_others_step_uV_type    ONE_STEP_VOLT
#define vsram_others_volt_type       REGULAR_VOLTAGE
#define vsram_others_stb             110

#define vproc_vol_reg         PMIC_RG_BUCK_VPROC_VOSEL_ADDR
#define vproc_enable_reg      PMIC_RG_BUCK_VPROC_EN_ADDR
#define vproc_mode_reg        PMIC_RG_VPROC_FPWM_ADDR
#define vproc_min_uV          518750
#define vproc_max_uV          1312500
#define vproc_vol_mask_pos    VMPOS_ONE
#define vproc_vol_shift_pos   VSPOS_ONE
#define vproc_enable_bit_pos  EPOS_ONE
#define vproc_mode_bit_pos    MPOS_TWO
#define vproc_step_uV_type    ONE_STEP_VOLT
#define vproc_volt_type       REGULAR_VOLTAGE
#define vproc_stb             220

#define vsram_proc_vol_reg         PMIC_RG_LDO_VSRAM_PROC_VOSEL_ADDR
#define vsram_proc_enable_reg      PMIC_RG_LDO_VSRAM_PROC_EN_ADDR
#define vsram_proc_mode_reg        0
#define vsram_proc_min_uV          518750
#define vsram_proc_max_uV          1312500
#define vsram_proc_vol_mask_pos    VMPOS_ONE
#define vsram_proc_vol_shift_pos   VSPOS_ONE
#define vsram_proc_enable_bit_pos  EPOS_ONE
#define vsram_proc_mode_bit_pos    MPOS_NONE
#define vsram_proc_step_uV_type    ONE_STEP_VOLT
#define vsram_proc_volt_type       REGULAR_VOLTAGE
#define vsram_proc_stb             110

#define vpa_vol_reg         PMIC_RG_BUCK_VPA_VOSEL_ADDR
#define vpa_enable_reg      PMIC_RG_BUCK_VPA_EN_ADDR
#define vpa_mode_reg        PMIC_RG_VPA_MODESET_ADDR
#define vpa_min_uV          500000
#define vpa_max_uV          3650000
#define vpa_vol_mask_pos    VMPOS_THREE
#define vpa_vol_shift_pos   VSPOS_ONE
#define vpa_enable_bit_pos  EPOS_ONE
#define vpa_mode_bit_pos    MPOS_THREE
#define vpa_step_uV_type    SPE_STEP_VOLT
#define vpa_volt_type       REGULAR_VOLTAGE
#define vpa_stb             220

#define vdram_vol_reg         PMIC_RG_VDRAM_VOSEL_ADDR
#define vdram_enable_reg      PMIC_RG_LDO_VDRAM_EN_ADDR
#define vdram_mode_reg        0
#define vdram_trim_reg        PMIC_RG_VDRAM_VOTRIM_ADDR
#define vdram_trim_mask       PMIC_RG_VDRAM_VOTRIM_MASK
#define vdram_trim_shift      PMIC_RG_VDRAM_VOTRIM_SHIFT
#define vdram_min_uV          1100000
#define vdram_max_uV          1200000
#define vdram_vol_mask_pos    VMPOS_FOUR
#define vdram_vol_shift_pos   VSPOS_TWO
#define vdram_enable_bit_pos  EPOS_ONE
#define vdram_mode_bit_pos    MPOS_NONE
#define vdram_step_uV_type    NON_STEP_VOLT
#define vdram_volt_type       NON_REGULAR_VOLTAGE
#define vdram_stb             3300

extern void mt6357_wdtdbg_vosel(void);
extern int mt6357_probe(void);

#endif /* __LINUX_REGULATOR_MT6357_H_ */

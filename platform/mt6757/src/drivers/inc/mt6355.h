#ifndef __LINUX_REGULATOR_MT6355_H_
#define __LINUX_REGULATOR_MT6355_H_

#include <regulator_core.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

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

typedef enum {
	 VSPOS_ONE,
         VSPOS_TWO,
         VSPOS_NONE,
} vol_shift_pos;

typedef enum {
	 VMPOS_ONE,
         VMPOS_TWO,
         VMPOS_THREE,
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
struct mt6355_ldo_trim_info {
    unsigned short trim_reg;
    unsigned short trim_mask;
    unsigned short trim_shift;
    const void *trim_voltages;
    unsigned int trim_size;
};
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
struct mt6355_ldo_info {
    const void *pvoltages;
    const void *idxs;
    unsigned int n_size;
};
#endif /*--LDO_SUPPORT--*/

/*--abstrac the same parameter--*/
struct mt6355_regulator_info {
    unsigned int min_uV;
    unsigned int max_uV;
    unsigned short vol_reg;
    unsigned short enable_reg;
    unsigned short mode_reg;
#ifdef LDO_SUPPORT
    struct mt6355_ldo_info *extinfo;
#endif /*--LDO_SUPPORT--*/
#ifdef LDO_VOTRIM_SUPPORT
    struct mt6355_ldo_trim_info *triminfo;
#endif /*--LDO_VOTRIM_SUPPORT--*/
    vol_mask_pos vmpos;
    vol_shift_pos vspos;
    enable_bit_pos ebpos;
    mode_bit_pos mbpos;
    step_uV_type vtype;
    regulator_type rtype;
};

#ifdef LDO_VOTRIM_SUPPORT
#define mt6355_ldo_trim_decl(_name, trim_array)\
{ \
    .trim_reg = _name##_trim_reg, \
    .trim_mask = _name##_trim_mask, \
    .trim_shift = _name##_trim_shift, \
    .trim_voltages = (void *)(trim_array), \
    .trim_size = ARRAY_SIZE(trim_array),	\
}
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
#define mt6355_ldo_decl(volt_array, idx_array)\
{ \
    .pvoltages = (void *)(volt_array), \
    .idxs = (void *)(idx_array), \
    .n_size = ARRAY_SIZE(volt_array),	\
}
#endif /*--LDO_SUPPORT--*/

#define mt6355_decl(_name)\
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

#define vdram2_vol_reg  (0x10C8)
#define vdram2_enable_reg (0x10C6)
#define vdram2_mode_reg (0x1274)
#define vdram2_min_uV 400000
#define vdram2_max_uV 1193750
#define vdram2_vol_mask_pos    VMPOS_ONE
#define vdram2_vol_shift_pos   VSPOS_ONE
#define vdram2_enable_bit_pos  EPOS_ONE
#define vdram2_mode_bit_pos    MPOS_FOUR
#define vdram2_step_uV_type    ONE_STEP_VOLT
#define vdram2_volt_type       REGULAR_VOLTAGE
#define vdram2_stb 1000

#define vs1_vol_reg  (0x1114)
#define vs1_enable_reg (0x1112)
#define vs1_mode_reg (0x129E)
#define vs1_min_uV 1200000
#define vs1_max_uV 2787500
#define vs1_vol_mask_pos    VMPOS_ONE
#define vs1_vol_shift_pos   VSPOS_ONE
#define vs1_enable_bit_pos  EPOS_ONE
#define vs1_mode_bit_pos    MPOS_FOUR
#define vs1_step_uV_type    TWO_STEP_VOLT
#define vs1_volt_type       REGULAR_VOLTAGE
#define vs1_stb 1000

#define vdram1_vol_reg (0x10A6)
#define vdram1_enable_reg (0x10A4)
#define vdram1_mode_reg (0x1264)
#define vdram1_min_uV 518750
#define vdram1_max_uV 1312500
#define vdram1_vol_mask_pos    VMPOS_ONE
#define vdram1_vol_shift_pos   VSPOS_ONE
#define vdram1_enable_bit_pos  EPOS_ONE
#define vdram1_mode_bit_pos    MPOS_FOUR
#define vdram1_step_uV_type    ONE_STEP_VOLT
#define vdram1_volt_type       REGULAR_VOLTAGE
#define vdram1_stb 1000

#define vmodem_vol_reg (0x10EA)
#define vmodem_enable_reg (0x10E8)
#define vmodem_mode_reg (0x1284)
#define vmodem_min_uV 400000
#define vmodem_max_uV 1193750
#define vmodem_vol_mask_pos    VMPOS_ONE
#define vmodem_vol_shift_pos   VSPOS_ONE
#define vmodem_enable_bit_pos  EPOS_ONE
#define vmodem_mode_bit_pos    MPOS_ONE
#define vmodem_step_uV_type    ONE_STEP_VOLT
#define vmodem_volt_type       REGULAR_VOLTAGE
#define vmodem_stb 1000

#define vcore_vol_reg (0x1060)
#define vcore_enable_reg (0x105E)
#define vcore_mode_reg (0x1242)
#define vcore_min_uV 406250
#define vcore_max_uV 1200000
#define vcore_vol_mask_pos    VMPOS_ONE
#define vcore_vol_shift_pos   VSPOS_ONE
#define vcore_enable_bit_pos  EPOS_ONE
#define vcore_mode_bit_pos    MPOS_ONE
#define vcore_step_uV_type    ONE_STEP_VOLT
#define vcore_volt_type       REGULAR_VOLTAGE
#define vcore_stb 1000

#define vs2_vol_reg (0x113E)
#define vs2_enable_reg (0x113C)
#define vs2_mode_reg (0x12AC)
#define vs2_min_uV 1200000
#define vs2_max_uV 2787500
#define vs2_vol_mask_pos    VMPOS_ONE
#define vs2_vol_shift_pos   VSPOS_ONE
#define vs2_enable_bit_pos  EPOS_ONE
#define vs2_mode_bit_pos    MPOS_ONE
#define vs2_step_uV_type    TWO_STEP_VOLT
#define vs2_volt_type       REGULAR_VOLTAGE
#define vs2_stb 1000

#define vpa_vol_reg (0x1168)
#define vpa_enable_reg (0x1166)
#define vpa_mode_reg (0x12B4)
#define vpa_min_uV 500000
#define vpa_max_uV 36500000
#define vpa_vol_mask_pos    VMPOS_THREE
#define vpa_vol_shift_pos   VSPOS_ONE
#define vpa_enable_bit_pos  EPOS_ONE
#define vpa_mode_bit_pos    MPOS_THREE
#define vpa_step_uV_type    SPE_STEP_VOLT
#define vpa_volt_type       REGULAR_VOLTAGE
#define vpa_stb 1000

#define vproc12_vol_reg (0x103E)
#define vproc12_enable_reg (0x103C)
#define vproc12_mode_reg (0x1220)
#define vproc12_min_uV 406250
#define vproc12_max_uV 1200000
#define vproc12_vol_mask_pos    VMPOS_ONE
#define vproc12_vol_shift_pos   VSPOS_ONE
#define vproc12_enable_bit_pos  EPOS_ONE
#define vproc12_mode_bit_pos    MPOS_TWO
#define vproc12_step_uV_type    ONE_STEP_VOLT
#define vproc12_volt_type       REGULAR_VOLTAGE
#define vproc12_stb 1000

#define vgpu_vol_reg (0x1084)
#define vgpu_enable_reg (0x1082)
#define vgpu_mode_reg (0x1242)
#define vgpu_min_uV 406250
#define vgpu_max_uV 1200000
#define vgpu_vol_mask_pos    VMPOS_ONE
#define vgpu_vol_shift_pos   VSPOS_ONE
#define vgpu_enable_bit_pos  EPOS_ONE
#define vgpu_mode_bit_pos    MPOS_TWO
#define vgpu_step_uV_type    ONE_STEP_VOLT
#define vgpu_volt_type       REGULAR_VOLTAGE
#define vgpu_stb 1000

#define vproc11_vol_reg (0x101C)
#define vproc11_enable_reg (0x101A)
#define vproc11_mode_reg (0x1220)
#define vproc11_min_uV 406250
#define vproc11_max_uV 1200000
#define vproc11_vol_mask_pos    VMPOS_ONE
#define vproc11_vol_shift_pos   VSPOS_ONE
#define vproc11_enable_bit_pos  EPOS_ONE
#define vproc11_mode_bit_pos    MPOS_ONE
#define vproc11_step_uV_type    ONE_STEP_VOLT
#define vproc11_volt_type       REGULAR_VOLTAGE
#define vproc11_stb 1000

#define vsram_gpu_vol_reg (0x18EA)
#define vsram_gpu_enable_reg (0x18E8)
#define vsram_gpu_mode_reg (0)
#define vsram_gpu_min_uV 518750
#define vsram_gpu_max_uV 1312500
#define vsram_gpu_vol_mask_pos    VMPOS_ONE
#define vsram_gpu_vol_shift_pos   VSPOS_ONE
#define vsram_gpu_enable_bit_pos  EPOS_ONE
#define vsram_gpu_mode_bit_pos    MPOS_NONE
#define vsram_gpu_step_uV_type    ONE_STEP_VOLT
#define vsram_gpu_volt_type       REGULAR_VOLTAGE
#define vsram_gpu_stb 110

#define vsram_core_vol_reg (0x18CA)
#define vsram_core_enable_reg (0x18C8)
#define vsram_core_mode_reg (0)
#define vsram_core_min_uV 518750
#define vsram_core_max_uV 1312500
#define vsram_core_vol_mask_pos    VMPOS_ONE
#define vsram_core_vol_shift_pos   VSPOS_ONE
#define vsram_core_enable_bit_pos  EPOS_ONE
#define vsram_core_mode_bit_pos    MPOS_NONE
#define vsram_core_step_uV_type    ONE_STEP_VOLT
#define vsram_core_volt_type       REGULAR_VOLTAGE
#define vsram_core_stb 110

#define vsram_proc_vol_reg (0x18AA)
#define vsram_proc_enable_reg (0x18A8)
#define vsram_proc_mode_reg (0)
#define vsram_proc_min_uV 518750
#define vsram_proc_max_uV 1312500
#define vsram_proc_vol_mask_pos    VMPOS_ONE
#define vsram_proc_vol_shift_pos   VSPOS_ONE
#define vsram_proc_enable_bit_pos  EPOS_ONE
#define vsram_proc_mode_bit_pos    MPOS_NONE
#define vsram_proc_step_uV_type    ONE_STEP_VOLT
#define vsram_proc_volt_type       REGULAR_VOLTAGE
#define vsram_proc_stb 110

#define vsram_md_vol_reg (0x190A)
#define vsram_md_enable_reg (0x1908)
#define vsram_md_mode_reg (0)
#define vsram_md_min_uV 518750
#define vsram_md_max_uV 1312500
#define vsram_md_vol_mask_pos    VMPOS_ONE
#define vsram_md_vol_shift_pos   VSPOS_ONE
#define vsram_md_enable_bit_pos  EPOS_ONE
#define vsram_md_mode_bit_pos    MPOS_NONE
#define vsram_md_step_uV_type    ONE_STEP_VOLT
#define vsram_md_volt_type       REGULAR_VOLTAGE
#define vsram_md_stb 110

#define vcamd1_vol_reg (0x1A70)
#define vcamd1_enable_reg (0x16F4)
#define vcamd1_mode_reg (0)
#define vcamd1_trim_reg (0x1A70)
#define vcamd1_trim_mask (0xF)
#define vcamd1_trim_shift (0)
#define vcamd1_min_uV  900000
#define vcamd1_max_uV 1200000
#define vcamd1_vol_mask_pos   VMPOS_TWO
#define vcamd1_vol_shift_pos  VSPOS_TWO
#define vcamd1_enable_bit_pos EPOS_ONE
#define vcamd1_mode_bit_pos   MPOS_NONE
#define vcamd1_step_uV_type   NON_STEP_VOLT
#define vcamd1_volt_type      NON_REGULAR_VOLTAGE
#define vcamd1_stb 264

#define va10_vol_reg (0x163E)
#define va10_enable_reg (0x163C)
#define va10_mode_reg (0)
#define va10_trim_reg (0x1A78)
#define va10_trim_mask (0xF)
#define va10_trim_shift (0)
#define va10_min_uV 600000
#define va10_max_uV 2100000
#define va10_vol_mask_pos   VMPOS_TWO
#define va10_vol_shift_pos  VSPOS_TWO
#define va10_enable_bit_pos EPOS_ONE
#define va10_mode_bit_pos   MPOS_NONE
#define va10_step_uV_type   NON_STEP_VOLT
#define va10_volt_type      NON_REGULAR_VOLTAGE
#define va10_stb 264

#define vio18_vol_reg (0)
#define vio18_enable_reg (0x1614)
#define vio18_mode_reg (0)
#define vio18_trim_reg (0x1A66)
#define vio18_trim_mask (0xF)
#define vio18_trim_shift (0)
#define vio18_min_uV 1800000
#define vio18_max_uV 1800000
#define vio18_vol_mask_pos   VMPOS_NONE
#define vio18_vol_shift_pos  VSPOS_NONE
#define vio18_enable_bit_pos EPOS_ONE
#define vio18_mode_bit_pos   MPOS_NONE
#define vio18_step_uV_type   NON_STEP_VOLT
#define vio18_volt_type      FIXED_REGULAR_VOLTAGE
#define vio18_stb 264

#define vio28_vol_reg (0)
#define vio28_enable_reg (0x1600)
#define vio28_mode_reg (0)
#define vio28_trim_reg (0x1A3A)
#define vio28_trim_mask (0xF)
#define vio28_trim_shift (0)
#define vio28_min_uV 2800000
#define vio28_max_uV 2800000
#define vio28_vol_mask_pos   VMPOS_NONE
#define vio28_vol_shift_pos  VSPOS_NONE
#define vio28_enable_bit_pos EPOS_ONE
#define vio28_mode_bit_pos   MPOS_NONE
#define vio28_step_uV_type   NON_STEP_VOLT
#define vio28_volt_type      FIXED_REGULAR_VOLTAGE
#define vio28_stb 264

#define vusb33_vol_reg (0)
#define vusb33_enable_reg (0x167A)
#define vusb33_mode_reg (0)
#define vusb33_trim_reg (0x1A1E)
#define vusb33_trim_mask (0xF)
#define vusb33_trim_shift (0)
#define vusb33_min_uV 3000000
#define vusb33_max_uV 3000000
#define vusb33_vol_mask_pos   VMPOS_NONE
#define vusb33_vol_shift_pos  VSPOS_NONE
#define vusb33_enable_bit_pos EPOS_ONE
#define vusb33_mode_bit_pos   MPOS_NONE
#define vusb33_step_uV_type   NON_STEP_VOLT
#define vusb33_volt_type      FIXED_REGULAR_VOLTAGE
#define vusb33_stb 264

extern int mt6355_probe(void);

#endif /* __LINUX_REGULATOR_MT6355_H_ */

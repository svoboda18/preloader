#ifndef __LINUX_REGULATOR_MT6358_H_
#define __LINUX_REGULATOR_MT6358_H_

#include "pal_log.h"

#define PMIC_PRELOADER 1
#define LDO_SUPPORT
#define LDO_VOTRIM_SUPPORT
#define MT6358_DEBUG 1

#if MT6358_DEBUG
#define PMUTAG                "[MT6358] "
#define mreg_dbg_print(fmt, arg...) pal_log_info(PMUTAG fmt, ##arg)
#else
#define mreg_dbg_print(...) do { } while(0)
#endif

enum {
	MTK_REGULATOR_VSRAM_OTHERS,
	MTK_REGULATOR_VSRAM_PROC11,
	MTK_REGULATOR_VSRAM_PROC12,
	MTK_REGULATOR_VSRAM_GPU,
	MTK_REGULATOR_VCORE,
	MTK_REGULATOR_VPROC11,
	MTK_REGULATOR_VPROC12,
	MTK_REGULATOR_VGPU,
	MTK_REGULATOR_VMODEM,
	MTK_REGULATOR_VDRAM1,
#ifdef LDO_SUPPORT
	MTK_REGULATOR_VDRAM2,
	MTK_REGULATOR_LDO_SUPPORT = MTK_REGULATOR_VDRAM2,
	MTK_REGULATOR_VIO18,
#endif /*--LDO_SUPPORT--*/
	MTK_REGULATOR_MAX_NR,
};

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

#ifdef LDO_VOTRIM_SUPPORT
struct mt6358_ldo_trim_info {
	unsigned short trim_reg;
	unsigned short trim_mask;
	unsigned short trim_shift;
	const void *trim_voltages;
	unsigned int trim_size;
};
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
static unsigned int mt6358_ldo_convert_data(unsigned char id,
	unsigned int cnvdata, volseltran transtype);

struct mt6358_ldo_info {
	const void *pvoltages;
	const void *idxs;
	unsigned int n_size;
};
#endif /*--LDO_SUPPORT--*/

/*--abstrac the same parameter--*/
struct mt6358_regulator_info {
	unsigned int min_uV;
	unsigned int max_uV;
	unsigned short vol_reg;
	unsigned short vol_mask;
	unsigned short vol_shift;
	unsigned short da_vol_reg;
	unsigned short da_vol_mask;
	unsigned short da_vol_shift;
	unsigned short enable_reg;
	unsigned short enable_shift;
	unsigned short mode_reg;
	unsigned short mode_shift;
#ifdef LDO_SUPPORT
	struct mt6358_ldo_info *extinfo;
#endif /*--LDO_SUPPORT--*/
#ifdef LDO_VOTRIM_SUPPORT
	struct mt6358_ldo_trim_info *triminfo;
#endif /*--LDO_VOTRIM_SUPPORT--*/
	unsigned short step_uV;
	regulator_type rtype;
};

extern int hw_tracking_set(const char *name, bool en,
			   int offset, unsigned int lb, unsigned int hb);
extern void mt6358_wdtdbg_vosel(void);
extern int mt6358_probe(void);

#ifdef LDO_VOTRIM_SUPPORT
#define mt6358_ldo_trim_decl(_name, trim_array)\
{ \
	.trim_reg = _name##_trim_reg, \
	.trim_mask = _name##_trim_mask, \
	.trim_shift = _name##_trim_shift, \
	.trim_voltages = (void *)(trim_array), \
	.trim_size = ARRAY_SIZE(trim_array),	\
}
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
#define mt6358_ldo_decl(volt_array, idx_array)\
{ \
	.pvoltages = (void *)(volt_array), \
	.idxs = (void *)(idx_array), \
	.n_size = ARRAY_SIZE(volt_array),	\
}
#endif /*--LDO_SUPPORT--*/

#define mt6358_decl(_name)\
{ \
	.min_uV = _name##_min_uV, \
	.max_uV = _name##_max_uV, \
	.vol_reg = _name##_vol_reg, \
	.vol_mask = _name##_vol_mask, \
	.vol_shift = _name##_vol_shift, \
	.da_vol_reg = _name##_da_vol_reg, \
	.da_vol_mask = _name##_da_vol_mask, \
	.da_vol_shift = _name##_da_vol_shift, \
	.enable_reg = _name##_enable_reg, \
	.enable_shift = _name##_enable_shift, \
	.mode_reg = _name##_mode_reg, \
	.mode_shift = _name##_mode_shift, \
	.step_uV = _name##_step_uV, \
	.rtype = _name##_volt_type, \
}

/* -------Code Gen Start-------*/
#define vsram_others_vol_reg            PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_ADDR
#define vsram_others_vol_mask           PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_MASK
#define vsram_others_vol_shift          PMIC_RG_LDO_VSRAM_OTHERS_VOSEL_SHIFT
#define vsram_others_da_vol_reg         PMIC_DA_VSRAM_OTHERS_VOSEL_ADDR
#define vsram_others_da_vol_mask        PMIC_DA_VSRAM_OTHERS_VOSEL_MASK
#define vsram_others_da_vol_shift       PMIC_DA_VSRAM_OTHERS_VOSEL_SHIFT
#define vsram_others_enable_reg         PMIC_RG_LDO_VSRAM_OTHERS_EN_ADDR
#define vsram_others_enable_shift       PMIC_RG_LDO_VSRAM_OTHERS_EN_SHIFT
#define vsram_others_mode_reg           0
#define vsram_others_mode_shift         0
#define vsram_others_trim_reg           0
#define vsram_others_trim_mask          0
#define vsram_others_trim_shift         0
#define vsram_others_min_uV             500000
#define vsram_others_max_uV             1293750
#define vsram_others_step_uV            6250
#define vsram_others_volt_type          REGULAR_VOLTAGE
#define vsram_others_stb                240


#define vsram_proc11_vol_reg            PMIC_RG_LDO_VSRAM_PROC11_VOSEL_ADDR
#define vsram_proc11_vol_mask           PMIC_RG_LDO_VSRAM_PROC11_VOSEL_MASK
#define vsram_proc11_vol_shift          PMIC_RG_LDO_VSRAM_PROC11_VOSEL_SHIFT
#define vsram_proc11_da_vol_reg         PMIC_DA_VSRAM_PROC11_VOSEL_ADDR
#define vsram_proc11_da_vol_mask        PMIC_DA_VSRAM_PROC11_VOSEL_MASK
#define vsram_proc11_da_vol_shift       PMIC_DA_VSRAM_PROC11_VOSEL_SHIFT
#define vsram_proc11_enable_reg         PMIC_RG_LDO_VSRAM_PROC11_EN_ADDR
#define vsram_proc11_enable_shift       PMIC_RG_LDO_VSRAM_PROC11_EN_SHIFT
#define vsram_proc11_mode_reg           0
#define vsram_proc11_mode_shift         0
#define vsram_proc11_trim_reg           0
#define vsram_proc11_trim_mask          0
#define vsram_proc11_trim_shift         0
#define vsram_proc11_min_uV             500000
#define vsram_proc11_max_uV             1293750
#define vsram_proc11_step_uV            6250
#define vsram_proc11_volt_type          REGULAR_VOLTAGE
#define vsram_proc11_stb                240


#define vsram_proc12_vol_reg            PMIC_RG_LDO_VSRAM_PROC12_VOSEL_ADDR
#define vsram_proc12_vol_mask           PMIC_RG_LDO_VSRAM_PROC12_VOSEL_MASK
#define vsram_proc12_vol_shift          PMIC_RG_LDO_VSRAM_PROC12_VOSEL_SHIFT
#define vsram_proc12_da_vol_reg         PMIC_DA_VSRAM_PROC12_VOSEL_ADDR
#define vsram_proc12_da_vol_mask        PMIC_DA_VSRAM_PROC12_VOSEL_MASK
#define vsram_proc12_da_vol_shift       PMIC_DA_VSRAM_PROC12_VOSEL_SHIFT
#define vsram_proc12_enable_reg         PMIC_RG_LDO_VSRAM_PROC12_EN_ADDR
#define vsram_proc12_enable_shift       PMIC_RG_LDO_VSRAM_PROC12_EN_SHIFT
#define vsram_proc12_mode_reg           0
#define vsram_proc12_mode_shift         0
#define vsram_proc12_trim_reg           0
#define vsram_proc12_trim_mask          0
#define vsram_proc12_trim_shift         0
#define vsram_proc12_min_uV             500000
#define vsram_proc12_max_uV             1293750
#define vsram_proc12_step_uV            6250
#define vsram_proc12_volt_type          REGULAR_VOLTAGE
#define vsram_proc12_stb                240


#define vsram_gpu_vol_reg               PMIC_RG_LDO_VSRAM_GPU_VOSEL_ADDR
#define vsram_gpu_vol_mask              PMIC_RG_LDO_VSRAM_GPU_VOSEL_MASK
#define vsram_gpu_vol_shift             PMIC_RG_LDO_VSRAM_GPU_VOSEL_SHIFT
#define vsram_gpu_da_vol_reg            PMIC_DA_VSRAM_GPU_VOSEL_ADDR
#define vsram_gpu_da_vol_mask           PMIC_DA_VSRAM_GPU_VOSEL_MASK
#define vsram_gpu_da_vol_shift          PMIC_DA_VSRAM_GPU_VOSEL_SHIFT
#define vsram_gpu_enable_reg            PMIC_RG_LDO_VSRAM_GPU_EN_ADDR
#define vsram_gpu_enable_shift          PMIC_RG_LDO_VSRAM_GPU_EN_SHIFT
#define vsram_gpu_mode_reg              0
#define vsram_gpu_mode_shift            0
#define vsram_gpu_trim_reg              0
#define vsram_gpu_trim_mask             0
#define vsram_gpu_trim_shift            0
#define vsram_gpu_min_uV                500000
#define vsram_gpu_max_uV                1293750
#define vsram_gpu_step_uV               6250
#define vsram_gpu_volt_type             REGULAR_VOLTAGE
#define vsram_gpu_stb                   240


#define vcore_vol_reg                   PMIC_RG_BUCK_VCORE_VOSEL_ADDR
#define vcore_vol_mask                  PMIC_RG_BUCK_VCORE_VOSEL_MASK
#define vcore_vol_shift                 PMIC_RG_BUCK_VCORE_VOSEL_SHIFT
#define vcore_da_vol_reg                PMIC_DA_VCORE_VOSEL_ADDR
#define vcore_da_vol_mask               PMIC_DA_VCORE_VOSEL_MASK
#define vcore_da_vol_shift              PMIC_DA_VCORE_VOSEL_SHIFT
#define vcore_enable_reg                PMIC_RG_BUCK_VCORE_EN_ADDR
#define vcore_enable_shift              PMIC_RG_BUCK_VCORE_EN_SHIFT
#define vcore_mode_reg                  PMIC_RG_VCORE_FPWM_ADDR
#define vcore_mode_shift                PMIC_RG_VCORE_FPWM_SHIFT
#define vcore_trim_reg                  0
#define vcore_trim_mask                 0
#define vcore_trim_shift                0
#define vcore_min_uV                    500000
#define vcore_max_uV                    1293750
#define vcore_step_uV                   6250
#define vcore_volt_type                 REGULAR_VOLTAGE
#define vcore_stb                       1000


#define vproc11_vol_reg                 PMIC_RG_BUCK_VPROC11_VOSEL_ADDR
#define vproc11_vol_mask                PMIC_RG_BUCK_VPROC11_VOSEL_MASK
#define vproc11_vol_shift               PMIC_RG_BUCK_VPROC11_VOSEL_SHIFT
#define vproc11_da_vol_reg              PMIC_DA_VPROC11_VOSEL_ADDR
#define vproc11_da_vol_mask             PMIC_DA_VPROC11_VOSEL_MASK
#define vproc11_da_vol_shift            PMIC_DA_VPROC11_VOSEL_SHIFT
#define vproc11_enable_reg              PMIC_RG_BUCK_VPROC11_EN_ADDR
#define vproc11_enable_shift            PMIC_RG_BUCK_VPROC11_EN_SHIFT
#define vproc11_mode_reg                PMIC_RG_VPROC11_FPWM_ADDR
#define vproc11_mode_shift              PMIC_RG_VPROC11_FPWM_SHIFT
#define vproc11_trim_reg                0
#define vproc11_trim_mask               0
#define vproc11_trim_shift              0
#define vproc11_min_uV                  500000
#define vproc11_max_uV                  1293750
#define vproc11_step_uV                 6250
#define vproc11_volt_type               REGULAR_VOLTAGE
#define vproc11_stb                     1000


#define vproc12_vol_reg                 PMIC_RG_BUCK_VPROC12_VOSEL_ADDR
#define vproc12_vol_mask                PMIC_RG_BUCK_VPROC12_VOSEL_MASK
#define vproc12_vol_shift               PMIC_RG_BUCK_VPROC12_VOSEL_SHIFT
#define vproc12_da_vol_reg              PMIC_DA_VPROC12_VOSEL_ADDR
#define vproc12_da_vol_mask             PMIC_DA_VPROC12_VOSEL_MASK
#define vproc12_da_vol_shift            PMIC_DA_VPROC12_VOSEL_SHIFT
#define vproc12_enable_reg              PMIC_RG_BUCK_VPROC12_EN_ADDR
#define vproc12_enable_shift            PMIC_RG_BUCK_VPROC12_EN_SHIFT
#define vproc12_mode_reg                PMIC_RG_VPROC12_FPWM_ADDR
#define vproc12_mode_shift              PMIC_RG_VPROC12_FPWM_SHIFT
#define vproc12_trim_reg                0
#define vproc12_trim_mask               0
#define vproc12_trim_shift              0
#define vproc12_min_uV                  500000
#define vproc12_max_uV                  1293750
#define vproc12_step_uV                 6250
#define vproc12_volt_type               REGULAR_VOLTAGE
#define vproc12_stb                     1000


#define vgpu_vol_reg                    PMIC_RG_BUCK_VGPU_VOSEL_ADDR
#define vgpu_vol_mask                   PMIC_RG_BUCK_VGPU_VOSEL_MASK
#define vgpu_vol_shift                  PMIC_RG_BUCK_VGPU_VOSEL_SHIFT
#define vgpu_da_vol_reg                 PMIC_DA_VGPU_VOSEL_ADDR
#define vgpu_da_vol_mask                PMIC_DA_VGPU_VOSEL_MASK
#define vgpu_da_vol_shift               PMIC_DA_VGPU_VOSEL_SHIFT
#define vgpu_enable_reg                 PMIC_RG_BUCK_VGPU_EN_ADDR
#define vgpu_enable_shift               PMIC_RG_BUCK_VGPU_EN_SHIFT
#define vgpu_mode_reg                   PMIC_RG_VGPU_FPWM_ADDR
#define vgpu_mode_shift                 PMIC_RG_VGPU_FPWM_SHIFT
#define vgpu_trim_reg                   0
#define vgpu_trim_mask                  0
#define vgpu_trim_shift                 0
#define vgpu_min_uV                     500000
#define vgpu_max_uV                     1293750
#define vgpu_step_uV                    6250
#define vgpu_volt_type                  REGULAR_VOLTAGE
#define vgpu_stb                        1000


#define vmodem_vol_reg                  PMIC_RG_BUCK_VMODEM_VOSEL_ADDR
#define vmodem_vol_mask                 PMIC_RG_BUCK_VMODEM_VOSEL_MASK
#define vmodem_vol_shift                PMIC_RG_BUCK_VMODEM_VOSEL_SHIFT
#define vmodem_da_vol_reg               PMIC_DA_VMODEM_VOSEL_ADDR
#define vmodem_da_vol_mask              PMIC_DA_VMODEM_VOSEL_MASK
#define vmodem_da_vol_shift             PMIC_DA_VMODEM_VOSEL_SHIFT
#define vmodem_enable_reg               PMIC_RG_BUCK_VMODEM_EN_ADDR
#define vmodem_enable_shift             PMIC_RG_BUCK_VMODEM_EN_SHIFT
#define vmodem_mode_reg                 PMIC_RG_VMODEM_FPWM_ADDR
#define vmodem_mode_shift               PMIC_RG_VMODEM_FPWM_SHIFT
#define vmodem_trim_reg                 0
#define vmodem_trim_mask                0
#define vmodem_trim_shift               0
#define vmodem_min_uV                   500000
#define vmodem_max_uV                   1293750
#define vmodem_step_uV                  6250
#define vmodem_volt_type                REGULAR_VOLTAGE
#define vmodem_stb                      1000


#define vdram1_vol_reg                  PMIC_RG_BUCK_VDRAM1_VOSEL_ADDR
#define vdram1_vol_mask                 PMIC_RG_BUCK_VDRAM1_VOSEL_MASK
#define vdram1_vol_shift                PMIC_RG_BUCK_VDRAM1_VOSEL_SHIFT
#define vdram1_da_vol_reg               PMIC_DA_VDRAM1_VOSEL_ADDR
#define vdram1_da_vol_mask              PMIC_DA_VDRAM1_VOSEL_MASK
#define vdram1_da_vol_shift             PMIC_DA_VDRAM1_VOSEL_SHIFT
#define vdram1_enable_reg               PMIC_RG_BUCK_VDRAM1_EN_ADDR
#define vdram1_enable_shift             PMIC_RG_BUCK_VDRAM1_EN_SHIFT
#define vdram1_mode_reg                 PMIC_RG_VDRAM1_FPWM_ADDR
#define vdram1_mode_shift               PMIC_RG_VDRAM1_FPWM_SHIFT
#define vdram1_trim_reg                 0
#define vdram1_trim_mask                0
#define vdram1_trim_shift               0
#define vdram1_min_uV                   500000
#define vdram1_max_uV                   2087500
#define vdram1_step_uV                  12500
#define vdram1_volt_type                REGULAR_VOLTAGE
#define vdram1_stb                      1000


#define vdram2_vol_reg                  PMIC_RG_LDO_VDRAM2_VOSEL_0_ADDR
#define vdram2_vol_mask                 PMIC_RG_LDO_VDRAM2_VOSEL_0_MASK
#define vdram2_vol_shift                PMIC_RG_LDO_VDRAM2_VOSEL_0_SHIFT
#define vdram2_da_vol_reg               0
#define vdram2_da_vol_mask              0
#define vdram2_da_vol_shift             0
#define vdram2_enable_reg               PMIC_RG_LDO_VDRAM2_EN_ADDR
#define vdram2_enable_shift             PMIC_RG_LDO_VDRAM2_EN_SHIFT
#define vdram2_mode_reg                 0
#define vdram2_mode_shift               0
#define vdram2_trim_reg                 PMIC_RG_VDRAM2_VOTRIM_ADDR
#define vdram2_trim_mask                PMIC_RG_VDRAM2_VOTRIM_MASK
#define vdram2_trim_shift               PMIC_RG_VDRAM2_VOTRIM_SHIFT
#define vdram2_min_uV                   600000
#define vdram2_max_uV                   1800000
#define vdram2_step_uV                  0
#define vdram2_volt_type                NON_REGULAR_VOLTAGE
#define vdram2_stb                      3300


#define vio28_vol_reg                   PMIC_RG_VIO28_VOCAL_ADDR
#define vio28_vol_mask                  0
#define vio28_vol_shift                 0
#define vio28_da_vol_reg                0
#define vio28_da_vol_mask               0
#define vio28_da_vol_shift              0
#define vio28_enable_reg                PMIC_RG_LDO_VIO28_EN_ADDR
#define vio28_enable_shift              PMIC_RG_LDO_VIO28_EN_SHIFT
#define vio28_mode_reg                  0
#define vio28_mode_shift                0
#define vio28_trim_reg                  PMIC_RG_VIO28_VOTRIM_ADDR
#define vio28_trim_mask                 PMIC_RG_VIO28_VOTRIM_MASK
#define vio28_trim_shift                PMIC_RG_VIO28_VOTRIM_SHIFT
#define vio28_min_uV                    2800000
#define vio28_max_uV                    2800000
#define vio28_step_uV                   0
#define vio28_volt_type                 FIXED_REGULAR_VOLTAGE
#define vio28_stb                       270


#define vio18_vol_reg                   PMIC_RG_VIO18_VOCAL_ADDR
#define vio18_vol_mask                  0
#define vio18_vol_shift                 0
#define vio18_da_vol_reg                0
#define vio18_da_vol_mask               0
#define vio18_da_vol_shift              0
#define vio18_enable_reg                PMIC_RG_LDO_VIO18_EN_ADDR
#define vio18_enable_shift              PMIC_RG_LDO_VIO18_EN_SHIFT
#define vio18_mode_reg                  0
#define vio18_mode_shift                0
#define vio18_trim_reg                  PMIC_RG_VIO18_VOTRIM_ADDR
#define vio18_trim_mask                 PMIC_RG_VIO18_VOTRIM_MASK
#define vio18_trim_shift                PMIC_RG_VIO18_VOTRIM_SHIFT
#define vio18_min_uV                    1800000
#define vio18_max_uV                    1800000
#define vio18_step_uV                   0
#define vio18_volt_type                 FIXED_REGULAR_VOLTAGE
#define vio18_stb                       2700


#define va12_vol_reg                    PMIC_RG_VA12_VOCAL_ADDR
#define va12_vol_mask                   0
#define va12_vol_shift                  0
#define va12_da_vol_reg                 0
#define va12_da_vol_mask                0
#define va12_da_vol_shift               0
#define va12_enable_reg                 PMIC_RG_LDO_VA12_EN_ADDR
#define va12_enable_shift               PMIC_RG_LDO_VA12_EN_SHIFT
#define va12_mode_reg                   0
#define va12_mode_shift                 0
#define va12_trim_reg                   PMIC_RG_VA12_VOTRIM_ADDR
#define va12_trim_mask                  PMIC_RG_VA12_VOTRIM_MASK
#define va12_trim_shift                 PMIC_RG_VA12_VOTRIM_SHIFT
#define va12_min_uV                     1200000
#define va12_max_uV                     1200000
#define va12_step_uV                    0
#define va12_volt_type                  FIXED_REGULAR_VOLTAGE
#define va12_stb                        270


#define vusb_vol_reg                    PMIC_RG_VUSB_VOSEL_ADDR
#define vusb_vol_mask                   PMIC_RG_VUSB_VOSEL_MASK
#define vusb_vol_shift                  PMIC_RG_VUSB_VOSEL_SHIFT
#define vusb_da_vol_reg                 0
#define vusb_da_vol_mask                0
#define vusb_da_vol_shift               0
#define vusb_enable_reg                 PMIC_RG_LDO_VUSB_EN_0_ADDR
#define vusb_enable_shift               PMIC_RG_LDO_VUSB_EN_0_SHIFT
#define vusb_mode_reg                   0
#define vusb_mode_shift                 0
#define vusb_trim_reg                   PMIC_RG_VUSB_VOTRIM_ADDR
#define vusb_trim_mask                  PMIC_RG_VUSB_VOTRIM_MASK
#define vusb_trim_shift                 PMIC_RG_VUSB_VOTRIM_SHIFT
#define vusb_min_uV                     3000000
#define vusb_max_uV                     3100000
#define vusb_step_uV                    0
#define vusb_volt_type                  NON_REGULAR_VOLTAGE
#define vusb_stb                        270

#define vcn33_bt_vol_reg                PMIC_RG_VCN33_VOSEL_ADDR
#define vcn33_bt_vol_mask               PMIC_RG_VCN33_VOSEL_MASK
#define vcn33_bt_vol_shift              PMIC_RG_VCN33_VOSEL_SHIFT
#define vcn33_bt_da_vol_reg             0
#define vcn33_bt_da_vol_mask            0
#define vcn33_bt_da_vol_shift           0
#define vcn33_bt_enable_reg             PMIC_RG_LDO_VCN33_EN_0_ADDR
#define vcn33_bt_enable_shift           PMIC_RG_LDO_VCN33_EN_0_SHIFT
#define vcn33_bt_mode_reg               0
#define vcn33_bt_mode_shift             0
#define vcn33_bt_trim_reg               PMIC_RG_VCN33_VOTRIM_ADDR
#define vcn33_bt_trim_mask              PMIC_RG_VCN33_VOTRIM_MASK
#define vcn33_bt_trim_shift             PMIC_RG_VCN33_VOTRIM_SHIFT
#define vcn33_bt_min_uV                 3300000
#define vcn33_bt_max_uV                 3500000
#define vcn33_bt_step_uV                0
#define vcn33_bt_volt_type              NON_REGULAR_VOLTAGE
#define vcn33_bt_stb                    270


#define vcn33_wifi_vol_reg              PMIC_RG_VCN33_VOSEL_ADDR
#define vcn33_wifi_vol_mask             PMIC_RG_VCN33_VOSEL_MASK
#define vcn33_wifi_vol_shift            PMIC_RG_VCN33_VOSEL_SHIFT
#define vcn33_wifi_da_vol_reg           0
#define vcn33_wifi_da_vol_mask          0
#define vcn33_wifi_da_vol_shift         0
#define vcn33_wifi_enable_reg           PMIC_RG_LDO_VCN33_EN_1_ADDR
#define vcn33_wifi_enable_shift         PMIC_RG_LDO_VCN33_EN_1_SHIFT
#define vcn33_wifi_mode_reg             0
#define vcn33_wifi_mode_shift           0
#define vcn33_wifi_trim_reg             PMIC_RG_VCN33_VOTRIM_ADDR
#define vcn33_wifi_trim_mask            PMIC_RG_VCN33_VOTRIM_MASK
#define vcn33_wifi_trim_shift           PMIC_RG_VCN33_VOTRIM_SHIFT
#define vcn33_wifi_min_uV               3300000
#define vcn33_wifi_max_uV               3500000
#define vcn33_wifi_step_uV              0
#define vcn33_wifi_volt_type            NON_REGULAR_VOLTAGE
#define vcn33_wifi_stb                  270

#endif /* __LINUX_REGULATOR_MT6358_H_ */

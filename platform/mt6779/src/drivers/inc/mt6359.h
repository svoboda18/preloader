#ifndef __LINUX_REGULATOR_MT6359_H_
#define __LINUX_REGULATOR_MT6359_H_

#include "pal_log.h"

#define PMIC_PRELOADER 1
#define LDO_SUPPORT
#define LDO_VOTRIM_SUPPORT
#define MT6359_DEBUG 1

#if MT6359_DEBUG
#define PMUTAG                "[MT6359] "
#define mreg_dbg_print(fmt, arg...) pal_log_info(PMUTAG fmt, ##arg)
#else
#define mreg_dbg_print(...) do { } while(0)
#endif

enum {
	MTK_REGULATOR_VSRAM_OTHERS,
	MTK_REGULATOR_VSRAM_PROC2,
	MTK_REGULATOR_VSRAM_PROC1,
	MTK_REGULATOR_VSRAM_MD,
	MTK_REGULATOR_VGPU11,
	MTK_REGULATOR_VMODEM,
	MTK_REGULATOR_VPU,
	MTK_REGULATOR_VCORE,
	MTK_REGULATOR_VPROC2,
	MTK_REGULATOR_VPROC1,
#ifdef LDO_SUPPORT
	MTK_REGULATOR_VM18,
	MTK_REGULATOR_LDO_SUPPORT = MTK_REGULATOR_VM18,
#endif /*--LDO_SUPPORT--*/
	MTK_REGULATOR_VMDLA,
	MTK_REGULATOR_VDRAM1,
	MTK_REGULATOR_VMDDR,
	MTK_REGULATOR_VDRAM2,
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
struct mt6359_ldo_trim_info {
	unsigned short trim_reg;
	unsigned short trim_mask;
	unsigned short trim_shift;
	const void *trim_voltages;
	unsigned int trim_size;
};
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
static unsigned int mt6359_ldo_convert_data(unsigned char id,
	unsigned int cnvdata, volseltran transtype);

struct mt6359_ldo_info {
	const void *pvoltages;
	const void *idxs;
	unsigned int n_size;
};
#endif /*--LDO_SUPPORT--*/

/*--abstrac the same parameter--*/
struct mt6359_regulator_info {
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
	struct mt6359_ldo_info *extinfo;
#endif /*--LDO_SUPPORT--*/
#ifdef LDO_VOTRIM_SUPPORT
	struct mt6359_ldo_trim_info *triminfo;
#endif /*--LDO_VOTRIM_SUPPORT--*/
	unsigned short step_uV;
	regulator_type rtype;
};

extern int mt6359_tracking_set(const char *name, bool en,
			       int offset, unsigned int lb, unsigned int hb);
extern void mt6359_wdtdbg_vosel(void);
extern int mt6359_probe(void);

#ifdef LDO_VOTRIM_SUPPORT
#define mt6359_ldo_trim_decl(_name, trim_array)\
{ \
	.trim_reg = _name##_trim_reg, \
	.trim_mask = _name##_trim_mask, \
	.trim_shift = _name##_trim_shift, \
	.trim_voltages = (void *)(trim_array), \
	.trim_size = ARRAY_SIZE(trim_array),	\
}
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
#define mt6359_ldo_decl(volt_array, idx_array)\
{ \
	.pvoltages = (void *)(volt_array), \
	.idxs = (void *)(idx_array), \
	.n_size = ARRAY_SIZE(volt_array),	\
}
#endif /*--LDO_SUPPORT--*/

#define mt6359_decl(_name)\
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
#define vsram_others_max_uV             1193750
#define vsram_others_step_uV            6250
#define vsram_others_volt_type          REGULAR_VOLTAGE
#define vsram_others_stb                240


#define vsram_proc2_vol_reg             PMIC_RG_LDO_VSRAM_PROC2_VOSEL_ADDR
#define vsram_proc2_vol_mask            PMIC_RG_LDO_VSRAM_PROC2_VOSEL_MASK
#define vsram_proc2_vol_shift           PMIC_RG_LDO_VSRAM_PROC2_VOSEL_SHIFT
#define vsram_proc2_da_vol_reg          PMIC_DA_VSRAM_PROC2_VOSEL_ADDR
#define vsram_proc2_da_vol_mask         PMIC_DA_VSRAM_PROC2_VOSEL_MASK
#define vsram_proc2_da_vol_shift        PMIC_DA_VSRAM_PROC2_VOSEL_SHIFT
#define vsram_proc2_enable_reg          PMIC_RG_LDO_VSRAM_PROC2_EN_ADDR
#define vsram_proc2_enable_shift        PMIC_RG_LDO_VSRAM_PROC2_EN_SHIFT
#define vsram_proc2_mode_reg            0
#define vsram_proc2_mode_shift          0
#define vsram_proc2_trim_reg            0
#define vsram_proc2_trim_mask           0
#define vsram_proc2_trim_shift          0
#define vsram_proc2_min_uV              500000
#define vsram_proc2_max_uV              1193750
#define vsram_proc2_step_uV             6250
#define vsram_proc2_volt_type           REGULAR_VOLTAGE
#define vsram_proc2_stb                 240


#define vsram_proc1_vol_reg             PMIC_RG_LDO_VSRAM_PROC1_VOSEL_ADDR
#define vsram_proc1_vol_mask            PMIC_RG_LDO_VSRAM_PROC1_VOSEL_MASK
#define vsram_proc1_vol_shift           PMIC_RG_LDO_VSRAM_PROC1_VOSEL_SHIFT
#define vsram_proc1_da_vol_reg          PMIC_DA_VSRAM_PROC1_VOSEL_ADDR
#define vsram_proc1_da_vol_mask         PMIC_DA_VSRAM_PROC1_VOSEL_MASK
#define vsram_proc1_da_vol_shift        PMIC_DA_VSRAM_PROC1_VOSEL_SHIFT
#define vsram_proc1_enable_reg          PMIC_RG_LDO_VSRAM_PROC1_EN_ADDR
#define vsram_proc1_enable_shift        PMIC_RG_LDO_VSRAM_PROC1_EN_SHIFT
#define vsram_proc1_mode_reg            0
#define vsram_proc1_mode_shift          0
#define vsram_proc1_trim_reg            0
#define vsram_proc1_trim_mask           0
#define vsram_proc1_trim_shift          0
#define vsram_proc1_min_uV              500000
#define vsram_proc1_max_uV              1193750
#define vsram_proc1_step_uV             6250
#define vsram_proc1_volt_type           REGULAR_VOLTAGE
#define vsram_proc1_stb                 240


#define vsram_md_vol_reg                PMIC_RG_LDO_VSRAM_MD_VOSEL_ADDR
#define vsram_md_vol_mask               PMIC_RG_LDO_VSRAM_MD_VOSEL_MASK
#define vsram_md_vol_shift              PMIC_RG_LDO_VSRAM_MD_VOSEL_SHIFT
#define vsram_md_da_vol_reg             PMIC_DA_VSRAM_MD_VOSEL_ADDR
#define vsram_md_da_vol_mask            PMIC_DA_VSRAM_MD_VOSEL_MASK
#define vsram_md_da_vol_shift           PMIC_DA_VSRAM_MD_VOSEL_SHIFT
#define vsram_md_enable_reg             PMIC_RG_LDO_VSRAM_MD_EN_ADDR
#define vsram_md_enable_shift           PMIC_RG_LDO_VSRAM_MD_EN_SHIFT
#define vsram_md_mode_reg               0
#define vsram_md_mode_shift             0
#define vsram_md_trim_reg               0
#define vsram_md_trim_mask              0
#define vsram_md_trim_shift             0
#define vsram_md_min_uV                 500000
#define vsram_md_max_uV                 1100000
#define vsram_md_step_uV                6250
#define vsram_md_volt_type              REGULAR_VOLTAGE
#define vsram_md_stb                    240


#define vgpu11_vol_reg                  PMIC_RG_BUCK_VGPU11_VOSEL_ADDR
#define vgpu11_vol_mask                 PMIC_RG_BUCK_VGPU11_VOSEL_MASK
#define vgpu11_vol_shift                PMIC_RG_BUCK_VGPU11_VOSEL_SHIFT
#define vgpu11_da_vol_reg               PMIC_DA_VGPU11_VOSEL_ADDR
#define vgpu11_da_vol_mask              PMIC_DA_VGPU11_VOSEL_MASK
#define vgpu11_da_vol_shift             PMIC_DA_VGPU11_VOSEL_SHIFT
#define vgpu11_enable_reg               PMIC_RG_BUCK_VGPU11_EN_ADDR
#define vgpu11_enable_shift             PMIC_RG_BUCK_VGPU11_EN_SHIFT
#define vgpu11_mode_reg                 PMIC_RG_VGPU11_FCCM_ADDR
#define vgpu11_mode_shift               PMIC_RG_VGPU11_FCCM_SHIFT
#define vgpu11_trim_reg                 0
#define vgpu11_trim_mask                0
#define vgpu11_trim_shift               0
#define vgpu11_min_uV                   400000
#define vgpu11_max_uV                   1193750
#define vgpu11_step_uV                  6250
#define vgpu11_volt_type                REGULAR_VOLTAGE
#define vgpu11_stb                      200


#define vmodem_vol_reg                  PMIC_RG_BUCK_VMODEM_VOSEL_ADDR
#define vmodem_vol_mask                 PMIC_RG_BUCK_VMODEM_VOSEL_MASK
#define vmodem_vol_shift                PMIC_RG_BUCK_VMODEM_VOSEL_SHIFT
#define vmodem_da_vol_reg               PMIC_DA_VMODEM_VOSEL_ADDR
#define vmodem_da_vol_mask              PMIC_DA_VMODEM_VOSEL_MASK
#define vmodem_da_vol_shift             PMIC_DA_VMODEM_VOSEL_SHIFT
#define vmodem_enable_reg               PMIC_RG_BUCK_VMODEM_EN_ADDR
#define vmodem_enable_shift             PMIC_RG_BUCK_VMODEM_EN_SHIFT
#define vmodem_mode_reg                 PMIC_RG_VMODEM_FCCM_ADDR
#define vmodem_mode_shift               PMIC_RG_VMODEM_FCCM_SHIFT
#define vmodem_trim_reg                 0
#define vmodem_trim_mask                0
#define vmodem_trim_shift               0
#define vmodem_min_uV                   400000
#define vmodem_max_uV                   1100000
#define vmodem_step_uV                  6250
#define vmodem_volt_type                REGULAR_VOLTAGE
#define vmodem_stb                      200


#define vpu_vol_reg                     PMIC_RG_BUCK_VPU_VOSEL_ADDR
#define vpu_vol_mask                    PMIC_RG_BUCK_VPU_VOSEL_MASK
#define vpu_vol_shift                   PMIC_RG_BUCK_VPU_VOSEL_SHIFT
#define vpu_da_vol_reg                  PMIC_DA_VPU_VOSEL_ADDR
#define vpu_da_vol_mask                 PMIC_DA_VPU_VOSEL_MASK
#define vpu_da_vol_shift                PMIC_DA_VPU_VOSEL_SHIFT
#define vpu_enable_reg                  PMIC_RG_BUCK_VPU_EN_ADDR
#define vpu_enable_shift                PMIC_RG_BUCK_VPU_EN_SHIFT
#define vpu_mode_reg                    PMIC_RG_VPU_FCCM_ADDR
#define vpu_mode_shift                  PMIC_RG_VPU_FCCM_SHIFT
#define vpu_trim_reg                    0
#define vpu_trim_mask                   0
#define vpu_trim_shift                  0
#define vpu_min_uV                      400000
#define vpu_max_uV                      1193750
#define vpu_step_uV                     6250
#define vpu_volt_type                   REGULAR_VOLTAGE
#define vpu_stb                         200


#define vcore_vol_reg                   PMIC_RG_BUCK_VCORE_VOSEL_ADDR
#define vcore_vol_mask                  PMIC_RG_BUCK_VCORE_VOSEL_MASK
#define vcore_vol_shift                 PMIC_RG_BUCK_VCORE_VOSEL_SHIFT
#define vcore_da_vol_reg                PMIC_DA_VCORE_VOSEL_ADDR
#define vcore_da_vol_mask               PMIC_DA_VCORE_VOSEL_MASK
#define vcore_da_vol_shift              PMIC_DA_VCORE_VOSEL_SHIFT
#define vcore_enable_reg                PMIC_RG_BUCK_VCORE_EN_ADDR
#define vcore_enable_shift              PMIC_RG_BUCK_VCORE_EN_SHIFT
#define vcore_mode_reg                  PMIC_RG_VCORE_FCCM_ADDR
#define vcore_mode_shift                PMIC_RG_VCORE_FCCM_SHIFT
#define vcore_trim_reg                  0
#define vcore_trim_mask                 0
#define vcore_trim_shift                0
#define vcore_min_uV                    400000
#define vcore_max_uV                    1193750
#define vcore_step_uV                   6250
#define vcore_volt_type                 REGULAR_VOLTAGE
#define vcore_stb                       200


#define vproc2_vol_reg                  PMIC_RG_BUCK_VPROC2_VOSEL_ADDR
#define vproc2_vol_mask                 PMIC_RG_BUCK_VPROC2_VOSEL_MASK
#define vproc2_vol_shift                PMIC_RG_BUCK_VPROC2_VOSEL_SHIFT
#define vproc2_da_vol_reg               PMIC_DA_VPROC2_VOSEL_ADDR
#define vproc2_da_vol_mask              PMIC_DA_VPROC2_VOSEL_MASK
#define vproc2_da_vol_shift             PMIC_DA_VPROC2_VOSEL_SHIFT
#define vproc2_enable_reg               PMIC_RG_BUCK_VPROC2_EN_ADDR
#define vproc2_enable_shift             PMIC_RG_BUCK_VPROC2_EN_SHIFT
#define vproc2_mode_reg                 PMIC_RG_VPROC2_FCCM_ADDR
#define vproc2_mode_shift               PMIC_RG_VPROC2_FCCM_SHIFT
#define vproc2_trim_reg                 0
#define vproc2_trim_mask                0
#define vproc2_trim_shift               0
#define vproc2_min_uV                   400000
#define vproc2_max_uV                   1193750
#define vproc2_step_uV                  6250
#define vproc2_volt_type                REGULAR_VOLTAGE
#define vproc2_stb                      200


#define vproc1_vol_reg                  PMIC_RG_BUCK_VPROC1_VOSEL_ADDR
#define vproc1_vol_mask                 PMIC_RG_BUCK_VPROC1_VOSEL_MASK
#define vproc1_vol_shift                PMIC_RG_BUCK_VPROC1_VOSEL_SHIFT
#define vproc1_da_vol_reg               PMIC_DA_VPROC1_VOSEL_ADDR
#define vproc1_da_vol_mask              PMIC_DA_VPROC1_VOSEL_MASK
#define vproc1_da_vol_shift             PMIC_DA_VPROC1_VOSEL_SHIFT
#define vproc1_enable_reg               PMIC_RG_BUCK_VPROC1_EN_ADDR
#define vproc1_enable_shift             PMIC_RG_BUCK_VPROC1_EN_SHIFT
#define vproc1_mode_reg                 PMIC_RG_VPROC1_FCCM_ADDR
#define vproc1_mode_shift               PMIC_RG_VPROC1_FCCM_SHIFT
#define vproc1_trim_reg                 0
#define vproc1_trim_mask                0
#define vproc1_trim_shift               0
#define vproc1_min_uV                   400000
#define vproc1_max_uV                   1193750
#define vproc1_step_uV                  6250
#define vproc1_volt_type                REGULAR_VOLTAGE
#define vproc1_stb                      200


#define vio28_vol_reg                   PMIC_RG_VIO28_VOSEL_ADDR
#define vio28_vol_mask                  PMIC_RG_VIO28_VOSEL_MASK
#define vio28_vol_shift                 PMIC_RG_VIO28_VOSEL_SHIFT
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
#define vio28_max_uV                    3300000
#define vio28_step_uV                   0
#define vio28_volt_type                 NON_REGULAR_VOLTAGE
#define vio28_stb                       480


#define vm18_vol_reg                    PMIC_RG_VM18_VOSEL_ADDR
#define vm18_vol_mask                   PMIC_RG_VM18_VOSEL_MASK
#define vm18_vol_shift                  PMIC_RG_VM18_VOSEL_SHIFT
#define vm18_da_vol_reg                 0
#define vm18_da_vol_mask                0
#define vm18_da_vol_shift               0
#define vm18_enable_reg                 PMIC_RG_LDO_VM18_EN_ADDR
#define vm18_enable_shift               PMIC_RG_LDO_VM18_EN_SHIFT
#define vm18_mode_reg                   0
#define vm18_mode_shift                 0
#define vm18_trim_reg                   PMIC_RG_VM18_VOTRIM_ADDR
#define vm18_trim_mask                  PMIC_RG_VM18_VOTRIM_MASK
#define vm18_trim_shift                 PMIC_RG_VM18_VOTRIM_SHIFT
#define vm18_min_uV                     1700000
#define vm18_max_uV                     1900000
#define vm18_step_uV                    0
#define vm18_volt_type                  NON_REGULAR_VOLTAGE
#define vm18_stb                        1920


#define va12_vol_reg                    PMIC_RG_VA12_VOSEL_ADDR
#define va12_vol_mask                   PMIC_RG_VA12_VOSEL_MASK
#define va12_vol_shift                  PMIC_RG_VA12_VOSEL_SHIFT
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
#define va12_max_uV                     1300000
#define va12_step_uV                    0
#define va12_volt_type                  NON_REGULAR_VOLTAGE
#define va12_stb                        240


#define vusb_vol_reg                    0
#define vusb_vol_mask                   0
#define vusb_vol_shift                  0
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
#define vusb_max_uV                     3000000
#define vusb_step_uV                    0
#define vusb_volt_type                  FIXED_REGULAR_VOLTAGE
#define vusb_stb                        240

#endif /* __LINUX_REGULATOR_MT6359_H_ */

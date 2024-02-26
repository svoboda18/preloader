#include <typedefs.h>
#include <platform.h>
#include <mt6355.h>
#include <pmic.h>

#define PMIC_DEBUG

#ifdef PMIC_DEBUG
#define PMICTAG                "[PMIC] "
#define PMICLOG(fmt, arg...)   dbg_print(PMICTAG fmt, ##arg)
#else
#define PMICLOG(fmt, arg...)
#endif

#ifdef LDO_SUPPORT
static const int va10_volts[] = {
	600000,
	700000,
	800000,
	900000,
	1000000,
	1100000,
	1200000,
	1300000,
	1400000,
	1500000,
	1600000,
	1700000,
	1800000,
	1900000,
	2000000,
	2100000,
};

static const int vio18_volts[] = {
	1800000,
};

static const int vio28_volts[] = {
	2800000,
};

static const int vusb33_volts[] = {
	3000000,
};

static const int va10_idxs[] = {
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
}; /*--ldo_volt_table1--*/

static const int fix_idxs[] = {
	0,
};

static const int votrim_1_type[] = {
	0, (-1)*20000, (-1)*40000, (-1)*60000, (-1)*80000, (-1)*100000, (-1)*120000, (-1)*140000,
	160000, 140000, 1200000, 100000, 80000, 60000, 40000, 20000,
};

static const int votrim_2_type[] = {
	0, (-1)*10000, (-1)*20000, (-1)*30000, (-1)*40000, (-1)*50000, (-1)*60000, (-1)*70000,
	80000, 70000, 600000, 50000, 40000, 30000, 20000, 10000,
};

static int mt6355_ldo_convert_data(unsigned char id, unsigned int cnvdata, volseltran transtype);

#endif /*--LDO_SUPPORT--*/

#ifndef SSPM_MODE
static unsigned int step_uv[4] = { 6250, 12500, 50000, 0xFFFF};
static unsigned char vo_shift[3] = { 0, 8 , 0xFF};
static unsigned char vo_mask[4] = { 0x7F, 0xF, 0x3F, 0xFF};
static unsigned char en_bit[2] = { 0, 0xFF};
static unsigned char mod_bit[5] = { 1, 2, 3, 8, 0xFF};
#else
#define step_uV      6250
#define vol_shift    0
#define vol_mask     0x7F
#define enable_bit   0
#define enable_mask  1
#define mode_shift   0
#define mode_mask    1
#endif

#ifdef LDO_VOTRIM_SUPPORT
static struct mt6355_ldo_trim_info ldo_trim_info[] = {
	mt6355_ldo_trim_decl(va10, votrim_2_type),
	mt6355_ldo_trim_decl(vio18, votrim_2_type),
	mt6355_ldo_trim_decl(vio28, votrim_1_type),
	mt6355_ldo_trim_decl(vusb33, votrim_1_type),
};
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
static struct mt6355_ldo_info ldo_ext_info[] = {
	mt6355_ldo_decl(va10_volts, va10_idxs),
	mt6355_ldo_decl(vio18_volts, fix_idxs),
	mt6355_ldo_decl(vio28_volts, fix_idxs),
	mt6355_ldo_decl(vusb33_volts, fix_idxs),
};
#endif /*--LDO_SUPPORT--*/

static struct mt6355_regulator_info mt6355_regu_info[] = {
	mt6355_decl(vdram2),
	/*mt6355_decl(vs1),*/
	mt6355_decl(vdram1),
	mt6355_decl(vmodem),
	mt6355_decl(vcore),
	/*mt6355_decl(vs2),
	mt6355_decl(vpa),
	mt6355_decl(vproc12),*/
	mt6355_decl(vgpu),
	mt6355_decl(vproc11),
	mt6355_decl(vsram_gpu),
	mt6355_decl(vsram_core),
	mt6355_decl(vsram_proc),
	mt6355_decl(vsram_md),
#ifdef LDO_SUPPORT
	mt6355_decl(va10),
	mt6355_decl(vio18),
	mt6355_decl(vio28),
	mt6355_decl(vusb33),
#endif /*--LDO_SUPPORT--*/
};

static int regu_size = ARRAY_SIZE(mt6355_regu_info);

static int mt6355_set_voltage(unsigned char id,
        unsigned int volt, unsigned int max_volt) {
    unsigned short selector = 0;
    int ret = 0;

#ifndef SSPM_MODE
    unsigned int step_uV = step_uv[(mt6355_regu_info[id].vtype)];
    unsigned short vol_shift = vo_shift[(mt6355_regu_info[id].vspos)];
    unsigned short vol_mask = vo_mask[(mt6355_regu_info[id].vmpos)];
#endif /*--NON SSPM MODE--*/

    if (volt > mt6355_regu_info[id].max_uV || volt < mt6355_regu_info[id].min_uV) {
      dbg_print("vp\n");
      return -1;
    }
    
    if (mt6355_regu_info[id].rtype == REGULAR_VOLTAGE)
        selector = (volt - mt6355_regu_info[id].min_uV) / (step_uV);
#ifdef LDO_SUPPORT
    else if (mt6355_regu_info[id].rtype == NON_REGULAR_VOLTAGE)
        selector = mt6355_ldo_convert_data(id, volt, VOLTOSEL);
    else if (mt6355_regu_info[id].rtype == FIXED_REGULAR_VOLTAGE) {
        dbg_print("vswf\n");
        return -1;
    } else
        ;
#else
    else {
    	dbg_print("ldo not support\n");
        return -1;
    }
#endif /*--LDO_SUPPORT--*/
       
    PMICLOG("1 %d,%d\n", id, selector);
    
    ret = pmic_config_interface(mt6355_regu_info[id].vol_reg, selector, vol_mask, vol_shift);
    return ret;
}

static int mt6355_get_voltage(unsigned char id)
{
    unsigned int selector = 0;
    unsigned int volt = 0;
    int ret = 0;

#ifndef SSPM_MODE
    unsigned int step_uV = step_uv[(mt6355_regu_info[id].vtype)];
    unsigned short vol_shift = vo_shift[(mt6355_regu_info[id].vspos)];
    unsigned short vol_mask = vo_mask[(mt6355_regu_info[id].vmpos)];
#endif /*--NON SSPM MODE--*/

    ret = pmic_read_interface(mt6355_regu_info[id].vol_reg, &selector, vol_mask, vol_shift);
    if (ret)
	return -1;
    
    if (mt6355_regu_info[id].rtype == REGULAR_VOLTAGE)
        volt = ((selector * step_uV) + mt6355_regu_info[id].min_uV);
#ifdef LDO_SUPPORT
    else if (mt6355_regu_info[id].rtype == NON_REGULAR_VOLTAGE)
        volt = mt6355_ldo_convert_data(id, selector, SELTOVOL);
    else if (mt6355_regu_info[id].rtype == FIXED_REGULAR_VOLTAGE) {
        volt = *((int *)(mt6355_regu_info[id].extinfo->pvoltages));
    } else
        ;
#else
    else
        return -1;
#endif /*--LDO_SUPPORT--*/
    
    PMICLOG("%d, %d, %d\n", id, selector, volt);
    if (volt > mt6355_regu_info[id].max_uV || volt < mt6355_regu_info[id].min_uV) {
        dbg_print("vgw\n");
        return -1;
    }

    return volt;
}

static int mt6355_enable(unsigned char id, enset en)
{
    int ret = 0;

#ifndef SSPM_MODE
    unsigned short enable_bit = en_bit[(mt6355_regu_info[id].ebpos)];
    unsigned short enable_mask = 1;
#endif /*--NON SSPM MODE--*/

    if (mt6355_regu_info[id].ebpos != EPOS_NONE) {
        PMICLOG("2 %d,%d\n", id, en);
        ret = pmic_config_interface(mt6355_regu_info[id].enable_reg, en, enable_mask, enable_bit);
    } else
	return -1;

    return ret;
}

static int mt6355_is_enabled(unsigned char id)
{
    unsigned int en = 0;
    int ret = 0;

#ifndef SSPM_MODE
    unsigned short enable_bit = en_bit[(mt6355_regu_info[id].ebpos)];
    unsigned short enable_mask = 1;
#endif /*--NON SSPM MODE--*/

    if (mt6355_regu_info[id].ebpos != EPOS_NONE) {
        ret = pmic_read_interface(mt6355_regu_info[id].enable_reg, &en, enable_mask, enable_bit);
        PMICLOG("3 %d,%d\n", id, en);
    } else
	return -1;

    return (ret ? ret : en) ;
}

static int mt6355_set_mode(unsigned char id, modeset mode)
{
    int ret = 0;

#ifndef SSPM_MODE
    unsigned short mode_bit = mod_bit[(mt6355_regu_info[id].mbpos)];
    unsigned short mode_mask = 1;
#endif /*--NON SSPM MODE--*/

    if (mt6355_regu_info[id].mbpos != MPOS_NONE) {
        PMICLOG("4 %d,%d\n", id, mode);
	if (id == MTK_REGULATOR_VDRAM1) {
		if (mode == PWMMODE) {
			/*RG_VDRAM1_FCOT=1’b1*/
			/*RG_VDRAM1_RCOMP=4’h2*/
			pmic_config_interface(0x1264, 0x5, 0x1F, 1);
			/*RG_VDRAM1_CCOMP=2’h2*/
			pmic_config_interface(0x126C, 0x2, 0x3, 0);
		} else {
			/*RG_VDRAM1_FCOT=1’b0*/
			/*RG_VDRAM1_RCOMP=4’h4*/
			pmic_config_interface(0x1264, 0x8, 0x1F, 1);
			/*RG_VDRAM1_CCOMP=2’h0*/
			pmic_config_interface(0x126C, 0, 0x3, 0);
		}
	}
        ret = pmic_config_interface(mt6355_regu_info[id].mode_reg, mode, mode_mask, mode_bit);
    } else
	return -1;

    return ret;
}

static int mt6355_get_mode(unsigned char id)
{
    unsigned int mode = 0;
    int ret = 0;

#ifndef SSPM_MODE
    unsigned short mode_bit = mod_bit[(mt6355_regu_info[id].mbpos)];
    unsigned short mode_mask = 1;
#endif /*--NON SSPM MODE--*/

    if (mt6355_regu_info[id].mbpos != MPOS_NONE) {
        ret = pmic_read_interface(mt6355_regu_info[id].mode_reg, &mode, mode_mask, mode_bit);
        PMICLOG("5 %d,%d\n", id, mode);
    } else
	return -1;

    return (ret ? ret: mode);
}

#ifdef LDO_VOTRIM_SUPPORT
static int mt6355_ldo_votrim_convert_data(unsigned char id, int cnvdata, trimseltran transtype)
{
    int i = 0, trim_size = 0, choice;
    const int *trim_Voltage;

    if ((mt6355_regu_info[id].triminfo->trim_voltages != NULL)) {
	trim_Voltage = mt6355_regu_info[id].triminfo->trim_voltages;
        trim_size = mt6355_regu_info[id].triminfo->trim_size; 
        PMICLOG("votrim_size %d, cnvdata %d\n", trim_size, cnvdata);
            switch (transtype) {
                 case TRIMTOSEL:
        		for (i = 0; i <= trim_size; i++)
        		{
                      		if (trim_Voltage[i] == cnvdata) {
                              		choice = i;
		                  	break;
				}
                      	}
                      break;
                 case SELTOTRIM:
                        choice = trim_Voltage[cnvdata];
                      break;
                 default:
                      break;
            }
     }
    
    return choice;
}

static int mt6355_set_votrim(unsigned char id, int trim_volt) {
    unsigned short selector = 0;
    int ret = 0;
       
    selector = mt6355_ldo_votrim_convert_data(id, trim_volt, TRIMTOSEL);
    
    if ((mt6355_regu_info[id].triminfo->trim_voltages != NULL)) {
    	PMICLOG("6 %d,%d\n", id, selector);
        ret = pmic_config_interface(mt6355_regu_info[id].triminfo->trim_reg, selector, mt6355_regu_info[id].triminfo->trim_mask, mt6355_regu_info[id].triminfo->trim_shift);
    }

    return ret;
}

static int mt6355_get_votrim(unsigned char id)
{
    unsigned int selector = 0;
    int ret = 0;
    
    ret = pmic_read_interface(mt6355_regu_info[id].triminfo->trim_reg, &selector, mt6355_regu_info[id].triminfo->trim_mask, mt6355_regu_info[id].triminfo->trim_shift);
    if (ret)
	return -1;
    
    ret = mt6355_ldo_votrim_convert_data(id, selector, SELTOTRIM);
    
    PMICLOG("7 %d,%d,%d\n", id, selector,ret);

    return ret;
}
#endif /*--LDO_VOTRIM_SUPPORT--*/

#ifdef LDO_SUPPORT
static int mt6355_ldo_convert_data(unsigned char id, unsigned int cnvdata, volseltran transtype)
{
    int i = 0, n_size = 0, choice;
    const int *pVoltage;
    const int *iDx;

    if ((mt6355_regu_info[id].extinfo->pvoltages != NULL) && (mt6355_regu_info[id].extinfo->idxs != NULL)) {
	pVoltage = mt6355_regu_info[id].extinfo->pvoltages;
	iDx = mt6355_regu_info[id].extinfo->idxs;
        n_size = mt6355_regu_info[id].extinfo->n_size; 
        PMICLOG("n_size %d\n", n_size);
        for (i = 0; i <= n_size; i++)
        {
            switch (transtype) {
                 case VOLTOSEL:
                      if ( pVoltage[i] == cnvdata) {
                              choice = iDx[i];
		                  break;
                      }
                      break;
                 case SELTOVOL:
            		if (iDx[i] == cnvdata) {
                              choice = pVoltage[i];
                                  break;
                      }
                      break;
                 default:
                      break;
            }
        }
    }
    
    return choice;
}
#endif /*--LDO_SUPPORT--*/

static struct regulator_ctrl mt6355_regulator_ctrl = {
    .set_voltage = mt6355_set_voltage,
    .get_voltage = mt6355_get_voltage,
    .enable = mt6355_enable,
    .is_enabled = mt6355_is_enabled,
    .set_mode = mt6355_set_mode,
    .get_mode = mt6355_get_mode,
#ifdef LDO_VOTRIM_SUPPORT
    .set_votrim = mt6355_set_votrim,
    .get_votrim = mt6355_get_votrim,
#endif /*--LDO_VOTRIM_SUPPORT--*/
};

static struct regulator mt6355_regulator[] = {
    {
        .name = "vdram2",
        .id = MTK_REGULATOR_VDRAM2,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    /*{
        .name = "vs1",
        .id = MTK_REGULATOR_VS1,
        .reg_ops = &mt6355_regulator_ctrl,
    },*/
    {
        .name = "vdram1",
        .id = MTK_REGULATOR_VDRAM1,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vmodem",
        .id = MTK_REGULATOR_VMODEM,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vcore",
        .id = MTK_REGULATOR_VCORE,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    /*{
        .name = "vs2",
        .id = MTK_REGULATOR_VS2,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vpa",
        .id = MTK_REGULATOR_VPA,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vproc12",
        .id = MTK_REGULATOR_VPROC12,
        .reg_ops = &mt6355_regulator_ctrl,
    },*/
    {
        .name = "vgpu",
        .id = MTK_REGULATOR_VGPU,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vproc11",
        .id = MTK_REGULATOR_VPROC11,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vsram_gpu",
        .id = MTK_REGULATOR_VSRAM_GPU,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vsram_core",
        .id = MTK_REGULATOR_VSRAM_CORE,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vsram_proc",
        .id = MTK_REGULATOR_VSRAM_PROC,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vsram_md",
        .id = MTK_REGULATOR_VSRAM_MD,
        .reg_ops = &mt6355_regulator_ctrl,
    },
#ifdef LDO_SUPPORT
    {
        .name = "va10",
        .id = MTK_REGULATOR_VA10,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vio18",
        .id = MTK_REGULATOR_VIO18,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vio28",
        .id = MTK_REGULATOR_VIO28,
        .reg_ops = &mt6355_regulator_ctrl,
    },
    {
        .name = "vusb33",
        .id = MTK_REGULATOR_VUSB33,
        .reg_ops = &mt6355_regulator_ctrl,
    },
#endif /*--LDO_SUPPORT--*/
};

/* ====================
 * Driver operations
 * ====================
 */
int mt6355_probe(void)
{
    int ret = 0;
    unsigned int i = 0;

    for (i = 0; i < regu_size; i++) {
#ifdef LDO_SUPPORT
        if (mt6355_regu_info[i].rtype != REGULAR_VOLTAGE)
    	    mt6355_regu_info[i].extinfo = &ldo_ext_info[(i-MTK_REGULATOR_LDO_SUPPORT)];
#endif /*--LDO_SUPPORT--*/
#ifdef LDO_VOTRIM_SUPPORT
        if (mt6355_regu_info[i].rtype != REGULAR_VOLTAGE)
    	    mt6355_regu_info[i].triminfo = &ldo_trim_info[(i-MTK_REGULATOR_LDO_SUPPORT)];
#endif /*--LDO_SUPPORT--*/
            ret = mtk_simple_regulator_register(&mt6355_regulator[i]);
            if (ret < 0) {
                /* register mtk regulator error */
                dbg_print("mt6355 regulator, %s\n", mt6355_regulator[i].name);
                return ret;
            }
    }

    return 0;
}

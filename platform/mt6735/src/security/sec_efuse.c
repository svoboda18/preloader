#include "typedefs.h"
#include "pmic.h"
#include "msdc.h"
#include "partition.h"
#include "pll.h"
#include "sec_efuse.h"

#define EFUSE_VBAT_LIMIT        3700
#define EFUSE_PART              "efuse"
#define EFUSE_READ_VOL_HIGH     88   /* vol=0.60+00625*88 = 1.15 */
#define EFUSE_READ_VOL_LOW      75   /* vol=0.60+00625*75 = 1.06875 */
#define EFUSE_STORAGE_NUM       0

/**************************************************************
 * Partition 
 **************************************************************/
int efuse_part_get_base(unsigned long *base)
{
    part_t *part_ptr;

    part_init();

    part_ptr = part_get(EFUSE_PART); 
    if (!part_ptr) {
        print("[%s] Error: Can't get partition base address\n", "EFUSE");
        return -1;
    } else {
        *base = part_ptr->start_sect;
        return 0; 
    }    
} 

/**************************************************************
 * Storage
 **************************************************************/
int efuse_storage_init(void)
{
    int ret;
    ret = mmc_init(EFUSE_STORAGE_NUM, MSDC_MODE_DEFAULT);     
    return ret;
}

int efuse_storage_read(unsigned long blknr, U32 blkcnt, unsigned long *dst)
{
    int ret;
    ret = mmc_block_read(EFUSE_STORAGE_NUM, blknr, blkcnt, dst);
    return ret;
}

int efuse_storage_write(unsigned long blknr, U32 blkcnt, unsigned long *src)
{
    int ret;
    ret = mmc_block_write(EFUSE_STORAGE_NUM, blknr, blkcnt, src);
    return ret;
}

/**************************************************************
 * WDT 
 **************************************************************/
void efuse_wdt_init(void)
{
    mtk_wdt_init();
}

void efuse_wdt_disable(void)
{
    mtk_wdt_disable();
}

void efuse_wdt_sw_reset(void)
{
    mtk_wdt_sw_reset();
}

/**************************************************************
 * DDR reserved mode
 **************************************************************/
int efuse_dram_reserved(int enable)
{
	/* return 0: success, -1: fail */
	return rgu_dram_reserved(enable);
}

/**************************************************************
 * PLL 
 **************************************************************/
void efuse_pll_set(void)
{
    clkmux_26M();
}


/**************************************************************
 * Vbat 
 **************************************************************/
int efuse_check_lowbat(void)
{
    int volt;
    volt = get_bat_sense_volt(5);
    if (volt < EFUSE_VBAT_LIMIT)
        return 1;
    else
        return 0;
}

/****************************************************
 * Fsource 
 * return 0 : success
 ****************************************************/
U32 efuse_fsource_set(void)
{
    U32 ret_val = 0;

    /* Fsource = 2.0V */
    ret_val |= pmic_config_interface(MT6328_PMIC_RG_VEFUSE_VOSEL_ADDR, 0x5, 
        MT6328_PMIC_RG_VEFUSE_VOSEL_MASK, MT6328_PMIC_RG_VEFUSE_VOSEL_SHIFT);

    ret_val |= pmic_config_interface(MT6328_PMIC_RG_VEFUSE_CAL_ADDR, 0x0, 
        MT6328_PMIC_RG_VEFUSE_CAL_MASK, MT6328_PMIC_RG_VEFUSE_CAL_SHIFT);
    
    /* Fsource enable */
    ret_val |= pmic_config_interface(MT6328_PMIC_RG_VEFUSE_EN_ADDR, 0x1, 
        MT6328_PMIC_RG_VEFUSE_EN_MASK, MT6328_PMIC_RG_VEFUSE_EN_SHIFT);
    
    mdelay(10);

    return ret_val;
}

U32 efuse_fsource_close(void)
{    
    U32 ret_val = 0;

    /* Fsource disable */
    ret_val |= pmic_config_interface(MT6328_PMIC_RG_VEFUSE_EN_ADDR, 0x0, 
        MT6328_PMIC_RG_VEFUSE_EN_MASK, MT6328_PMIC_RG_VEFUSE_EN_SHIFT);

    mdelay(10);

    return ret_val;
}

/**************************************************************
 * Vcore 
 **************************************************************/
U32 efuse_vcore_high(void)
{
    U32 ret=0;

    /* Voltage = 0.6+0.00625*step V */
    ret |= pmic_config_interface(MT6328_PMIC_VCORE1_VOSEL_ADDR, EFUSE_READ_VOL_HIGH, 
        MT6328_PMIC_VCORE1_VOSEL_MASK, MT6328_PMIC_VCORE1_VOSEL_SHIFT);

    ret |= pmic_config_interface(MT6328_PMIC_VCORE1_VOSEL_ON_ADDR, EFUSE_READ_VOL_HIGH,
        MT6328_PMIC_VCORE1_VOSEL_ON_MASK, MT6328_PMIC_VCORE1_VOSEL_ON_SHIFT);

    mdelay(10);

    return ret;
}

U32 efuse_vcore_low(void)
{
    U32 ret = 0;

    /* Voltage = 0.6+0.00625*step V */
    ret |= pmic_config_interface(MT6328_PMIC_VCORE1_VOSEL_ADDR, EFUSE_READ_VOL_LOW, 
        MT6328_PMIC_VCORE1_VOSEL_MASK, MT6328_PMIC_VCORE1_VOSEL_SHIFT);

    ret |= pmic_config_interface(MT6328_PMIC_VCORE1_VOSEL_ON_ADDR, EFUSE_READ_VOL_LOW,
        MT6328_PMIC_VCORE1_VOSEL_ON_MASK, MT6328_PMIC_VCORE1_VOSEL_ON_SHIFT);

    mdelay(10);

    return ret;
}

/**************************************************************
 * Others  
 **************************************************************/
/* re-initial modules after declinie clock */
int efuse_module_reinit(void)
{
    int ret;
    ret = pwrap_init();
    return ret;
}



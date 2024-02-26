#include "tb_emi.h"
#include "tb_dramc_common.h"
#include "tb_dramc_pi_api.h"
#include "tb_x_hal_io.h"
//#ifdef DDR_RESERVE_MODE
//#include "platform.h"
//#include "wdt.h"
//#endif

#ifdef DDR_RESERVE_MODE
#define LP3_CKE_FIX_ON_TO_LEAVE_SR_WITH_LVDRAM 0

extern DRAMC_CTX_T *psCurrDramCtx;
extern DRAMC_CTX_T DramCtx_LPDDR4;
extern DRAMC_CTX_T DramCtx_LPDDR3;
static U32 u4ReserveRegBackupAddress[] =
{
    (DRAMC_REG_SPCMDCTRL),
    (DRAMC_REG_SPCMDCTRL + ((U32)CHANNEL_B<<POS_BANK_NUM)),
    (DRAMC_REG_SHU_SCINTV),
    (DRAMC_REG_SHU_SCINTV + ((U32)CHANNEL_B<<POS_BANK_NUM)),
    (DRAMC_REG_DQSOSCR),
    (DRAMC_REG_DQSOSCR + ((U32)CHANNEL_B<<POS_BANK_NUM)),
    (DRAMC_REG_DUMMY_RD),
    (DRAMC_REG_DUMMY_RD + ((U32)CHANNEL_B<<POS_BANK_NUM)),
    (DRAMC_REG_DRAMC_PD_CTRL),
    (DRAMC_REG_DRAMC_PD_CTRL + ((U32)CHANNEL_B<<POS_BANK_NUM))
};
static U32 u4DramType = 0;

#if LP3_CKE_FIX_ON_TO_LEAVE_SR_WITH_LVDRAM
#if ENABLE_LP3_SW
static U32 u4Cha_cke_backup = 0;
static U32 u4Chb_cke_backup = 0;
#endif
#endif

static int Reserve_Sync_Writel(U32 addr, unsigned int val)
{
	(*(volatile unsigned int *)(addr)) = val;
	dsb();
	return 0;
}

static unsigned int Reserve_Reg_Readl(U32 addr)
{
	return (*(volatile unsigned int *)(addr));
}

#define RE_POWERON_CONFIG_EN               (0x10006000)
#define RE_PCM_PWR_IO_EN                   (0x1000602C)
#define RE_DRAMC_DPY_CLK_SW_CON_SEL        (0x10006460)
#define RE_DRAMC_DPY_CLK_SW_CON_SEL2       (0x10006470)
#define RE_SPM_S1_MODE_CH                  (0x10006468)
void Set_Spm_Poweron_Config_En(DRAMC_CTX_T * p)
{
    U32 u4value = 0;

    Reserve_Sync_Writel(RE_POWERON_CONFIG_EN, 0x0B160001);
    Reserve_Sync_Writel(RE_PCM_PWR_IO_EN, 0);
    u4value = (u1IsLP4Family(p->dram_type)) ? (0x00ffffff) : (0xaaffffff);
    Reserve_Sync_Writel(RE_DRAMC_DPY_CLK_SW_CON_SEL, u4value);
    Reserve_Sync_Writel(RE_DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff);
    u4value = Reserve_Reg_Readl(SPM_POWER_ON_VAL0);
    u4value |= ((0x1 << 8) | (0x1 << 12) | (0x1 << 13) | (0x1 << 14) | (0x1 << 15));
    Reserve_Sync_Writel(SPM_POWER_ON_VAL0, u4value);
    //u4value = Reserve_Reg_Readl(RE_SPM_S1_MODE_CH);
    //u4value |= 0x3;
    //Reserve_Sync_Writel(SPM_S1_MODE_CH, u4value);

    return;
}

void dump_SR(DRAMC_CTX_T * p, U32 u4DramType)
{
    U32 u4value = 0;

    u4value = u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA, MISC_STATUSA_SREF_STATE);
    mcSHOW_ERR_MSG(("!CHA SREF_STATE[0x%X]\n", u4value));
    if((u4DramType == 2) || (u4DramType == 3))
    {
        u4value = u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA + ((U32) CHANNEL_B << POS_BANK_NUM), MISC_STATUSA_SREF_STATE);
        mcSHOW_ERR_MSG(("!CHB SREF_STATE[0x%X]\n", u4value));
    }
    return;
}

void dump_gating_error_rg(DRAMC_CTX_T * p, U32 u4DramType)
{
    U32 u4value = 0;

    u4value = u4IO32Read4B(DDRPHY_MISC_STBERR_RK0_R);
    mcSHOW_ERR_MSG(("###DDR reserve CHA Gating error[0x%X]\n", u4value));
    if((u4DramType == 2) || (u4DramType == 3))
    {
        u4value = u4IO32Read4B(DDRPHY_MISC_STBERR_RK0_R + ((U32) CHANNEL_B << POS_BANK_NUM));
        mcSHOW_ERR_MSG(("###DDR reserve CHB Gating error[0x%X]\n", u4value));
    }
    return;
}

#if 0//ENABLE_DVFS_CDC_SYNCHRONIZER_OPTION
/* Only use while CDC option is 1 originally. But we use the patch for gating/tx/rx delay reload from AO to NAO in M17 since CDC_OPTION is 0*/
static void DDR_Reserved_Mode_Cdc_Option_Patch(DRAMC_CTX_T *p)
{
    U32 backup_broadcast = GetDramcBroadcast();
    U32 u4RegBackupAddress[] =
    {
        (DRAMC_REG_DRAMC_PD_CTRL),
        (DRAMC_REG_DRAMC_PD_CTRL + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_MISC_CG_CTRL5),
        (DDRPHY_MISC_CG_CTRL5 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DRAMC_REG_PRE_TDQSCK1),
        (DRAMC_REG_PRE_TDQSCK1 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DRAMC_REG_STBCAL),
        (DRAMC_REG_STBCAL + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_MISC_CTRL1),
        (DDRPHY_MISC_CTRL1 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_B0_RXDVS0),
        (DDRPHY_B0_RXDVS0 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_B1_RXDVS0),
        (DDRPHY_B1_RXDVS0 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_R0_B0_RXDVS2),
        (DDRPHY_R0_B0_RXDVS2 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_R1_B0_RXDVS2),
        (DDRPHY_R1_B0_RXDVS2 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_R0_B1_RXDVS2),
        (DDRPHY_R0_B1_RXDVS2 + ((U32)CHANNEL_B<<POS_BANK_NUM)),
        (DDRPHY_R1_B1_RXDVS2),
        (DDRPHY_R1_B1_RXDVS2 + ((U32)CHANNEL_B<<POS_BANK_NUM))
	};
	mcSHOW_DBG_MSG(("Apply DDR_Reserved_Mode_Cdc_Option_Patch\n"));

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    //Backup regs
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
    //1. Toggle for shuffle level reset
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 0, SHUCTRL2_R_DVFS_CDC_OPTION);//Lynx: Need to toggle this bit to toggle shuffle level from spm side band to conf AO
    mcDELAY_US(1);//delay 20NS
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 1, SHUCTRL2_R_DVFS_CDC_OPTION);
    //2. DQ DCM OFF
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0, DRAMC_PD_CTRL_COMBCLKCTRL);
    //3. TX DLY/PI DCM OFF
    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
                | P_Fld(0x0, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));
    //4. Dramc_idle OFF
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0, DRAMC_PD_CTRL_DCMEN2);
    //5. TX tracking DCM disable
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 1, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
    //6. Gating tracking disable
    vIO32WriteFldMulti_All(DRAMC_REG_STBCAL, P_Fld(0x0, STBCAL_STBCALEN)
                | P_Fld(0x0, STBCAL_STB_SELPHYCALEN));
    //Gating tracking Enable
    vIO32WriteFldMulti_All(DRAMC_REG_STBCAL, P_Fld(0x1, STBCAL_STBCALEN)
                | P_Fld(0x1, STBCAL_STB_SELPHYCALEN));
    //7. Load TX PI from conf to tx tracking
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 1, MISC_CTRL1_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMARPIDQ_SW);
    //8. TX tracking DCM Enable
    vIO32WriteFldAlign_All(DRAMC_REG_PRE_TDQSCK1, 0, PRE_TDQSCK1_TXUIPI_CAL_CGAR);
    //9. RX delay reload from conf to RX DLY tracking
    vIO32WriteFldAlign_All(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);
    vIO32WriteFldAlign_All(DDRPHY_B0_RXDVS0, 0, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 0, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);
    vIO32WriteFldAlign_All(DDRPHY_R0_B0_RXDVS2, 0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R1_B0_RXDVS2, 0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R0_B1_RXDVS2, 0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_R1_B1_RXDVS2, 0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_R0_B0_RXDVS2, 2, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R1_B0_RXDVS2, 2, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R0_B1_RXDVS2, 2, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_R1_B1_RXDVS2, 2, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1);
    //10. DQ DCM ON
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_COMBCLKCTRL);
    //11. Dramc_idle ON
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_DCMEN2);
    //12. TX DLY/PI DCM ON
    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL5, P_Fld(0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_PI_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_PI_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_CA_DLY_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_DQ0_DLY_DCM_EN)
                | P_Fld(0x1, MISC_CG_CTRL5_R_DQ1_DLY_DCM_EN));
    //Restore regs
    DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
    DramcBroadcastOnOff(backup_broadcast);
    return;
}
#endif

#define RESERVE_PDEF_SPM_PLL_CON (0x1000644C)
void Dramc_DDR_Reserved_Mode_setting(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    U32 backup_broadcast = GetDramcBroadcast();
    DRAMC_CTX_T * p;

    mcSHOW_DBG_MSG(("Enter Dramc_DDR_Reserved_Mode_setting \n"));
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    u4DramType = (*((volatile unsigned int *)(0x1022A010)) >> 10 & 0x7);
    mcSHOW_DBG_MSG(("Dram type =%x \n", u4DramType));
    if(u4DramType == 1)
    {
        psCurrDramCtx = &DramCtx_LPDDR3;
    }
    else if((u4DramType == 2) || (u4DramType == 3))
    {   // LPDDR4
        psCurrDramCtx = &DramCtx_LPDDR4;
    }
    p = psCurrDramCtx;
    Set_Spm_Poweron_Config_En(p);
    u4value = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);
    mcSHOW_ERR_MSG(("### shuffle level[%d]\n", u4value));
#if 0//ENABLE_DVFS_CDC_SYNCHRONIZER_OPTION
    if(u4IO32ReadFldAlign(DRAMC_REG_SHUCTRL2, SHUCTRL2_R_DVFS_CDC_OPTION))
    {
        DDR_Reserved_Mode_Cdc_Option_Patch(p);
    }
#endif
#ifdef HW_GATING
    DramcHWGatingOnOff(p, 0);//Disable HW Gating tracking for gating tracking fifo mode
#endif
    u4DramType = u4IO32ReadFldAlign(DRAMC_REG_ARBCTL, ARBCTL_RSV_DRAM_TYPE);
    mcSHOW_DBG_MSG(("Dramc_DDR_Reserved_Mode_setting dram type =%x \n", u4DramType));

    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);//change DVFS to RG mode

    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);//toggle shu_en to sync shu_level, need MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL = 1
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);//toggle shu_en to sync shu_level
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);//toggle shu_en to sync shu_level
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);//toggle shu_en to sync shu_level
    //vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(1, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM)
    //            | P_Fld(1, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM));//Set spm shuffle enable to 1
    //mcDELAY_US(1);
    //vIO32WriteFldMulti(SPM_POWER_ON_VAL0, P_Fld(0, SPM_POWER_ON_VAL0_SC_PHYPLL_SHU_EN_PCM)
    //            | P_Fld(0, SPM_POWER_ON_VAL0_SC_PHYPLL2_SHU_EN_PCM));//Set spm shuffle enable to 0

    mcDELAY_US(1);
    //! toggle hsu restore
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 1, SHUCTRL2_R_SHU_RESTORE);
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DRAMC_REG_SHUCTRL2, 0, SHUCTRL2_R_SHU_RESTORE);
    mcDELAY_US(1);

    if(u4DramType == 1)//LPDDR3
    {
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0 + ((U32)CHANNEL_B<<POS_BANK_NUM), 0, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF);//Open CHB controller clk
    }
    //Backup regs
    DramcBackupRegisters(p, u4ReserveRegBackupAddress, sizeof(u4ReserveRegBackupAddress)/sizeof(U32));
    //Disable MR4
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_REFRDIS);
    //Disable DQSOSC en
    vIO32WriteFldAlign_All(DRAMC_REG_SHU_SCINTV, 1, SHU_SCINTV_DQSOSCENDIS);
    //Disable DQSOSC rd
    vIO32WriteFldAlign_All(DRAMC_REG_DQSOSCR, 1, DQSOSCR_DQSOSCRDIS);
    //Disable Dummy Read
    vIO32WriteFldMulti_All(DRAMC_REG_DUMMY_RD, P_Fld(0x0, DUMMY_RD_DQSG_DMYWR_EN)
            | P_Fld(0x0, DUMMY_RD_DQSG_DMYRD_EN)
            | P_Fld(0x0, DUMMY_RD_SREF_DMYRD_EN)
            | P_Fld(0x0, DUMMY_RD_DUMMY_RD_EN)
            | P_Fld(0x0, DUMMY_RD_DMY_RD_DBG)
            | P_Fld(0x0, DUMMY_RD_DMY_WR_DBG));
    //Disable DDRPHY dynamic clock gating
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0, DRAMC_PD_CTRL_PHYCLKDYNGEN);//disable DDRPHY dynamic clock gating

    //To 26MHz
    if((u4DramType == 2) || (u4DramType == 3))
    {
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
                    | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    }
    else//LP3
    {
#if LP3_CKE_FIX_ON_TO_LEAVE_SR_WITH_LVDRAM
#if ENABLE_LP3_SW
        //Dram clock free run
        vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 1, DRAMC_PD_CTRL_MIOCKCTRLOFF);//disable DDRPHY dynamic clock gating

        //CKE control
        u4Cha_cke_backup = u4IO32Read4B(DRAMC_REG_CKECTRL);
        u4Chb_cke_backup = u4IO32Read4B(DRAMC_REG_CKECTRL + ((U32)CHANNEL_B<<POS_BANK_NUM));
        //CKE fix on
        vIO32WriteFldMulti_All(DRAMC_REG_CKECTRL, P_Fld(0x0, CKECTRL_CKE1FIXOFF) |
            P_Fld(0x0, CKECTRL_CKEFIXOFF) |
            P_Fld(0x1, CKECTRL_CKE1FIXON) |
            P_Fld(0x1, CKECTRL_CKEFIXON));
#endif
#endif
        vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
                    | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    }

    //RG_*PHDET_EN=0 (DLL)
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);

    //*PI_CG=1, RG_*MPDIV_CG=1
    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    //RG_*BIAS_EN=0
    //vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
    //vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
    //vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN);

    //RG_*VREF_EN=0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD5, 0x0, CA_CMD5_RG_RX_ARCMD_VREF_EN);

    ///TODO: only CHA?
    //RG_*MCK8X_EN=0
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0, PLL4_RG_RPHYPLL_ADA_MCK8X_EN)
                    | P_Fld(0, PLL4_RG_RPHYPLL_AD_MCK8X_EN));//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA
    //RG_*MIDPI_EN=0 RG_*MIDPI_CKDIV4_EN=0
    vIO32WriteFldMulti(DDRPHY_SHU1_B0_DQ6, P_Fld(0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0) | P_Fld(0, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0));
    vIO32WriteFldMulti(DDRPHY_SHU1_B1_DQ6, P_Fld(0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1) | P_Fld(0, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1));
    vIO32WriteFldMulti(DDRPHY_SHU1_CA_CMD6, P_Fld(0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA) | P_Fld(0, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA));
    //vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0x0, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0);
    //vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0x0, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1);
    //vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0x0, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA);

    vIO32WriteFldAlign(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN); //RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN); //RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA

    //!set sc_mpll to SPM register //Chen-Hsiang modify @20170316
    //u4value = Reserve_Reg_Readl(RESERVE_PDEF_SPM_PLL_CON);
    //Reserve_Sync_Writel(RESERVE_PDEF_SPM_PLL_CON, u4value | (0x1 << 8) | (0x1 << 4));//set sc_mpll_off=1 , sc_mpll_s_off=1
    //TBD
    //*((UINT32P)(0x1000631c )) |= (0x1 << 1); //ddrphy_pwr_iso=1 //Lewis@20160621: Fix LP3 Hang from S0 suspend into reserve mode
    //*((UINT32P)(0x100063b8 )) |= (0x1 << 1);

    //RG_*RESETB=0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x0, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_PLL4, 0x0, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA
    //Lewis@20160628: Fix LP3 enter S0 then into reserve mode fail due to CHB PHY not reset(LP3 PHY use 2 channels)

    MPLLInit();

    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) &= ~(0x1 << 2); //ddrphy_pwr_on=0
    //*((UINT32P)(0x100063b8 )) &= ~(0x1 << 2);

    // wait 1us
    mcDELAY_US(1);

    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) |= (0x1 << 2); //ddrphy_pwr_on=1
    //*((UINT32P)(0x100063b8 )) |= (0x1 << 2);
    // wait 1us
    //mcDELAY_US(1);

    //RG_*RESETB=1
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_PLL4, 0x1, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA

    //TBD
    //*((UINT32P)(0x1000631c )) &= ~(0x1 << 1); //ddrphy_pwr_iso=0 //Lewis@20160621: Fix LP3 Hang from S0 suspend into reserve mode
    //*((UINT32P)(0x100063b8 )) &= ~(0x1 << 1);

    u4value = Reserve_Reg_Readl(RESERVE_PDEF_SPM_PLL_CON);
    Reserve_Sync_Writel(RESERVE_PDEF_SPM_PLL_CON, u4value & ~((0x1 << 8) | (0x1 << 4)));//set sc_mpll_off=0 , sc_mpll_s_off=0
    mcDELAY_US(20);

    //RG_*PLL_EN=1
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN); //RG_*PLL_EN=1; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN); //RG_*PLL_EN=1; Since there is only 1 PLL, only control CHA

     //Wait 20us for MEMPLL
     mcDELAY_US(20);

    //RG_*VREF_EN=1 (Vref is only used in read, DQ(B0, B1)is use for RX)
    if((u4DramType == 2) || (u4DramType == 3))
    {
        vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    }
    else
    {
        vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
        vIO32WriteFldAlign(DDRPHY_B0_DQ5  + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
        vIO32WriteFldAlign(DDRPHY_CA_CMD5  + ((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_CMD5_RG_RX_ARCMD_VREF_EN);
    }
    //RG_*MCK8X_EN=1
    vIO32WriteFldAlign(DDRPHY_PLL4, 1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN);//RG_*MCK8X_EN=1; Since there is only 1 PLL, only to control CHA

    //wait 1us
    mcDELAY_US(1);

    if((u4DramType == 2) || (u4DramType == 3))
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_EN_B1);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_EN_CA);

        //wait 1us
        mcDELAY_US(1);

        //*PI_CG=0, RG_*MPDIV_CG=0
        vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
        vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)//not include 11th bit(CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    }
    else
    {
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B0_DQ6, 0x1, SHU1_B0_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B0);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_B1_DQ6, 0x1, SHU1_B1_DQ6_RG_ARPI_MIDPI_CKDIV4_EN_B1);
        vIO32WriteFldAlign_All(DDRPHY_SHU1_CA_CMD6, 0x1, SHU1_CA_CMD6_RG_ARPI_MIDPI_CKDIV4_EN_CA);

        //wait 1us
        mcDELAY_US(1);

        //*PI_CG=0, RG_*MPDIV_CG=0
        vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
        vIO32WriteFldMulti(DDRPHY_B0_DLL_ARPI2 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                    | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
        vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                    | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));

        vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)//not include 11th bit(CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                   // | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)//11th
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
        vIO32WriteFldMulti(DDRPHY_CA_DLL_ARPI2 + ((U32)CHANNEL_B<<POS_BANK_NUM), P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)//not include 11th bit(CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN)//11th
                    | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
    }

    //RG_*BIAS_EN=1
    //vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);//Chen-Hsiang modify @20170316
    //vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);//Chen-Hsiang modify @20170316

    // wait 1us
    mcDELAY_US(1);

    if((u4DramType == 2) || (u4DramType == 3))
    {
        vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
                    | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b01: memory clock, [0]: change memory clock
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    }
    else
    {
        vIO32WriteFldMulti(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM)
                    | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b01: memory clock, [0]: change memory clock
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
        mcDELAY_US(1);
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    }

    //force top feedback MCK not divide
    //rg_ddrphy_fb_ck_force_en = 1 -- TBD
    //*CLK_MEM_DFS_CFG |= (0x1 << 8); //rg_ddrphy_fb_ck_force_en = 1
    //u4value = Reserve_Reg_Readl(0x10060464);
    //Reserve_Sync_Writel(0x10060464, u4value | (0x3 << 20)); //set sc_ddrphy_fb_ck_ch*_en = 1
    //u4value = Reserve_Reg_Readl(0x10060004);
    //Reserve_Sync_Writel(0x10060004, u4value | (0x1 << 16)); //set sc_ddrphy_fb_ck_ch*_en = 1
    //*DRAMC_WBR = 0x3;
    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN)
        | P_Fld(1, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN));//clock freerun

    //1st DLL enable
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//Since CHA CA is DLL master
    // wait 1us
    mcDELAY_US(1);
    //2nd DLL enable
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2+ ((U32)CHANNEL_B<<POS_BANK_NUM), 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);//Chen-Hsiang modify @20170316
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);//Chen-Hsiang modify @20170316
    // wait 1us
    mcDELAY_US(1);

    vIO32WriteFldMulti(DDRPHY_MISC_SPM_CTRL1, P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH1_EN)
        | P_Fld(0, MISC_SPM_CTRL1_RG_DDRPHY_DB_CK_CH0_EN));//disable clock freerun
    //top feedback MCK to divided frequency -- TBD
    //*CLK_MEM_DFS_CFG &= ~(0x1 << 8); //rg_ddrphy_fb_ck_force_en = 0
    //u4value = Reserve_Reg_Readl(0x10060464);
    //Reserve_Sync_Writel(0x10060464, u4value & ~(0x3 << 20)); //set sc_ddrphy_fb_ck_ch*_en = 0
    //u4value = Reserve_Reg_Readl(0x10060004);
    //Reserve_Sync_Writel(0x10060004, u4value & ~(0x1 << 16)); //set sc_ddrphy_fb_ck_ch*_en = 0

    vIO32WriteFldAlign_All(DRAMC_REG_DDRCONF0, 1, DDRCONF0_RDATRST);//R_DMRDATRST = 1
    vIO32WriteFldAlign_All(DRAMC_REG_DDRCONF0, 0, DDRCONF0_RDATRST);//R_DMRDATRST = 0

    //! set SPM to control PLL enable and disable PLL enable from ddrphy conf
    /*TINFO ="Switching PHYPLL enable path from DDRPHY to SPM control by setting SPM SW" */
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);//change DVFS to SPM mode^M
    mcDELAY_US(1);

    vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW_PCM);
    u4value = Reserve_Reg_Readl(SPM_DRAMC_DPY_CLK_SW_CON2);
    Reserve_Sync_Writel(SPM_DRAMC_DPY_CLK_SW_CON2, u4value | (0x1 << 2));
    mcDELAY_US(1);
    //RG_*PLL_EN=1
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN); //disable DDRPHY PHYPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN); //disable DDRPHY CLRPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    /*TINFO ="Setting RX input delay tracking enable from SPM side(un-paused)" */
    //! speed >= 3200 need enable RX input delay tracking
    if((u4DramType == 2) || (u4DramType == 3))
    {
        //u4value = Reserve_Reg_Readl(SPM_DRAMC_DPY_CLK_SW_CON2);
        //Reserve_Sync_Writel(SPM_DRAMC_DPY_CLK_SW_CON2, u4value & ~(0x3 << 16));
        vIO32WriteFldAlign(SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);
    }
    DramcBroadcastOnOff(backup_broadcast);
    //DDRPhyFreqMeter();
    //dump_gating_error_rg(p, u4DramType);
    //dump_SR(p, u4DramType);
}

//#define USE_TA2_IN_DDR_RESERVE_MODE //Just for debug
#ifdef USE_TA2_IN_DDR_RESERVE_MODE
static void TA2_test_run_time_HW(DRAMC_CTX_T * p)//notice: Rank number and channel number should handle
{
    DRAM_CHANNEL_T channel_bak = p->channel;
    DRAM_RANK_T rank_bak = p->rank;
    U8 u1ChannelIdx = 0;
    U8 u1RankIdx = 0;
    U32 u4ErrorValue = 0;
    static U32 err_count = 0;
    static U32 pass_count = 0;

    vIO32WriteFldAlign_All(DRAMC_REG_TEST2_4, 4, TEST2_4_TESTAGENTRKSEL);//Rank select is controlled By HW
    //vIO32WriteFldAlign_All(DRAMC_REG_TEST2_2, 0x400, TEST2_2_TEST2_OFF); //Not change, suppose this area is preserved for memeory test
    for(u1ChannelIdx = CHANNEL_A; u1ChannelIdx < p->support_channel_num; u1ChannelIdx++)
    {
        p->channel = u1ChannelIdx;
        DramcEngine2SetPat(p, TEST_XTALK_PATTERN, p->support_rank_num - 1, 0);//TEST2_3_TESTCNT 1: HW switch R0 and R1 automatically 0: only R0
        vIO32WriteFldAlign(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), 1, TEST2_3_TEST2W);//Trigger W
        mcDELAY_MS(1);
        u4ErrorValue = DramcEngine2Compare(p, TE_OP_WRITE_READ_CHECK);
        for(u1RankIdx =0 ; u1RankIdx < p->support_rank_num; u1RankIdx++)
        {
            if(u4ErrorValue & (1<<u1RankIdx))
            {
                err_count++;
                mcSHOW_DBG_MSG(("HW channel(%d) Rank(%d), TA2 failed, pass_cnt:%d, err_cnt:%d, u4ErrorValue 0x%x\n", u1ChannelIdx, u1RankIdx, pass_count, err_count, u4ErrorValue));
            }
            else
            {
                pass_count++;
                mcSHOW_DBG_MSG(("HW channel(%d) Rank(%d), TA2 pass, pass_cnt:%d, err_cnt:%d\n", u1ChannelIdx, u1RankIdx, pass_count, err_count));
            }
        }
        vIO32WriteFldMulti(DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), P_Fld(0, TEST2_3_TEST2W) | P_Fld(0, TEST2_3_TEST2R) | P_Fld(0, TEST2_3_TEST1));
    }
    p->channel = channel_bak;
    p->rank = rank_bak;
    return;
}
#endif

void Dramc_DDR_Reserved_Mode_AfterSR(void)
{
    U32 u4WbrBackup = 0;
    DRAMC_CTX_T * p = psCurrDramCtx; //done in ddr reserve mode setting
    U32 u4LP3_MR2RLWL = 0;
    U32 u4LP4_MR13FSP = 0;
    U32 u4LP4_MR2RLWL = 0;
    U32 backup_broadcast = GetDramcBroadcast();
    U8 u1channel = CHANNEL_A;
    BOOL bSupport2Rank = (u4IO32ReadFldAlign(DRAMC_REG_RSTMASK, RSTMASK_RSV_DRAM_SUPPORT_RANK_NUM) == 0) ? TRUE : FALSE;

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);
    mcSHOW_DBG_MSG(("Dramc_DDR_Reserved_Mode_AfterSR dram type =%x \n", u4DramType));
    if(u4DramType == 1)
    {   //LPDDR3
        vIO32WriteFldAlign(DDRPHY_MISC_CG_CTRL0 + ((U32)CHANNEL_B<<POS_BANK_NUM), 1, MISC_CG_CTRL0_RG_CG_DRAMC_CHB_CK_OFF);//Close CHB controller clk

        //Since we do MR2 with (RG of Mr13) first while DVFS, we use (RG of MR13) to apply OP of MR2
        u4LP3_MR2RLWL = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR13, SHU_HWSET_MR13_HWSET_MR13_OP);
        p->channel = CHANNEL_A;
        DramcModeRegWriteByRank(p, 0, 2, u4LP3_MR2RLWL);
        DramcModeRegWriteByRank(p, 0, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
        if(bSupport2Rank == TRUE)//DRAM is dual rank
        {
            DramcModeRegWriteByRank(p, 1, 2, u4LP3_MR2RLWL);
            DramcModeRegWriteByRank(p, 1, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
        }
    }
    else if((u4DramType == 2) || (u4DramType == 3))
    {   //LPDDR4
        u4LP4_MR13FSP = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR13, SHU_HWSET_MR13_HWSET_MR13_OP);
        u4LP4_MR2RLWL = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR2, SHU_HWSET_MR2_HWSET_MR2_OP);
        for(u1channel = CHANNEL_A; u1channel < p->support_channel_num; u1channel++)
        {
            p->channel = u1channel;
            DramcModeRegWriteByRank(p, 0, 13, u4LP4_MR13FSP);
            DramcModeRegWriteByRank(p, 0, 2, u4LP4_MR2RLWL);
            DramcModeRegWriteByRank(p, 0, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
            if(bSupport2Rank == TRUE)//DRAM is dual rank
            {
                DramcModeRegWriteByRank(p, 1, 13, u4LP4_MR13FSP);
                DramcModeRegWriteByRank(p, 1, 2, u4LP4_MR2RLWL);
                DramcModeRegWriteByRank(p, 1, 17, 0);//R1 lost data WA, set R0 R1 both for safe{On charging mode, system may issue MR17=0xFF in R1}
            }
        }
    }
#ifdef USE_TA2_IN_DDR_RESERVE_MODE
    {
        int i = 0;
        for(i=0; i<10; i++)
        {
            TA2_test_run_time_HW(p);
        }
    }
#endif
    //Restore regs
    DramcRestoreRegisters(p, u4ReserveRegBackupAddress, sizeof(u4ReserveRegBackupAddress)/sizeof(U32));
#if LP3_CKE_FIX_ON_TO_LEAVE_SR_WITH_LVDRAM
#if ENABLE_LP3_SW
    if(u4DramType == 1)
    {
        vIO32Write4B(DRAMC_REG_CKECTRL, u4Cha_cke_backup);
        vIO32Write4B(DRAMC_REG_CKECTRL + ((U32)CHANNEL_B<<POS_BANK_NUM), u4Chb_cke_backup);
    }
#endif
#endif

#ifdef HW_GATING
    DramcHWGatingOnOff(p,1);//Enable HW Gating tracking for gating tracking fifo mode
#endif
    DramcBroadcastOnOff(backup_broadcast);
    dump_gating_error_rg(p, u4DramType);
    //DramcRegDump(p);
    return;
}

void Before_Init_DRAM_While_Reserve_Mode_fail(DRAM_DRAM_TYPE_T dram_type)
{
    DRAMC_CTX_T * p;
    mcSHOW_DBG_MSG(("\n\tReserve mode fail\tBefore_Init_DRAM_While_Reserve_Mode_fail\n"));
    if(u1IsLP4Family(dram_type))
    {
        psCurrDramCtx = &DramCtx_LPDDR4;
    }
    else
    {
#if (!__ETT__ && ENABLE_LP3_SW==0)
#if (FOR_DV_SIMULATION_USED==0 && SW_CHANGE_FOR_SIMULATION==0)
// Preloader which does not support LP3
// scy: reduce code size by removing unused LPDDR3 structure
	mcSHOW_DBG_MSG(("LPDDR3 not supported\n"));
	ASSERT(0);
#endif
#else
        psCurrDramCtx = &DramCtx_LPDDR3;
#endif
    }
    p = psCurrDramCtx;

#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif
    Switch26MHzDisableDummyReadRefreshAllBank(p);
	*((volatile unsigned int *)SPM_DRAMC_DPY_CLK_SW_CON2) &= ~(0x1 << 2);//20170210: confirmed by chen-Hsiang
 	*((volatile unsigned int *)SPM_POWER_ON_VAL0) &= ~(0x1 << 28);//20170210: confirmed by chen-Hsiang
}
#endif


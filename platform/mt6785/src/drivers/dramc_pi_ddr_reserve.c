#include "emi.h"
#include "dramc_common.h"
#include "dramc_pi_api.h"
#include "x_hal_io.h"
#ifdef DDR_RESERVE_MODE  
#include "platform.h"
#include "wdt.h"
#endif

#ifdef DDR_RESERVE_MODE
extern DRAMC_CTX_T *psCurrDramCtx;
extern DRAMC_CTX_T DramCtx_LPDDR4;
static U32 u4DramType = 0;
static U32 u4ReserveRegBackupAddress[] = 
{       
    (DRAMC_REG_SPCMDCTRL),
    (DRAMC_REG_SPCMDCTRL + SHIFT_TO_CHB_ADDR),
    (DRAMC_REG_DUMMY_RD),
    (DRAMC_REG_DUMMY_RD + SHIFT_TO_CHB_ADDR),
    (DRAMC_REG_DRAMC_PD_CTRL),
    (DRAMC_REG_DRAMC_PD_CTRL + SHIFT_TO_CHB_ADDR),
    (DDRPHY_B0_DQ5),
    (DDRPHY_B0_DQ5 + SHIFT_TO_CHB_ADDR),
    (DDRPHY_B1_DQ5),
    (DDRPHY_B1_DQ5 + SHIFT_TO_CHB_ADDR),
    (DDRPHY_B0_RXDVS0),
    (DDRPHY_B0_RXDVS0 + SHIFT_TO_CHB_ADDR),
    (DDRPHY_B1_RXDVS0),
    (DDRPHY_B1_RXDVS0 + SHIFT_TO_CHB_ADDR),
    (DDRPHY_R0_B0_RXDVS2),
    (DDRPHY_R0_B0_RXDVS2 + SHIFT_TO_CHB_ADDR),
    (DDRPHY_R0_B1_RXDVS2),
    (DDRPHY_R0_B1_RXDVS2 + SHIFT_TO_CHB_ADDR),
    (DDRPHY_R1_B0_RXDVS2),
    (DDRPHY_R1_B0_RXDVS2 + SHIFT_TO_CHB_ADDR),  
    (DDRPHY_R1_B1_RXDVS2),
    (DDRPHY_R1_B1_RXDVS2 + SHIFT_TO_CHB_ADDR),
    (DDRPHY_MISC_CG_CTRL4),//pipe line DCM
    (DDRPHY_MISC_CG_CTRL4 + SHIFT_TO_CHB_ADDR),//pipe line DCM
    (DDRPHY_MISC_CTRL1),//TX tracking reload from AO
    (DDRPHY_MISC_CTRL1 + SHIFT_TO_CHB_ADDR)//TX tracking reload from AO
};

void Set_Spm_Poweron_Config_En(DRAMC_CTX_T * p)
{
    vIO32WriteFldMulti(SPM_POWERON_CONFIG_EN, P_Fld(0x1, POWERON_CONFIG_EN_BCLK_CG_EN)//SPM power on
                | P_Fld(0xB16, POWERON_CONFIG_EN_PROJECT_CODE));
    vIO32WriteFldAlign(SPM_PCM_PWR_IO_EN, 0, PCM_PWR_IO_EN_FULL);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL, 0xffffffff, DRAMC_DPY_CLK_SW_CON_SEL_FULL);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON_SEL2, 0xffffffff, DRAMC_DPY_CLK_SW_CON_SEL2_FULL);
    vIO32WriteFldMulti(SPM_SPM_POWER_ON_VAL0, P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_2ND_DLL_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_DLL_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_DLL_CK_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_DPY_VREF_EN)
                | P_Fld(0x1, SPM_POWER_ON_VAL0_SC_PHYPLL_EN));
    vIO32WriteFldAlign(SPM_SPM_S1_MODE_CH, 0x3, SPM_S1_MODE_CH_SPM_S1_MODE_CH);//SR request by side band
    
    return;
}


void dump_SR(DRAMC_CTX_T * p, U32 u4DramType)
{
    U32 u4value = 0;

    u4value = u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA, MISC_STATUSA_SREF_STATE);
    mcSHOW_ERR_MSG(("!CHA SREF_STATE[0x%X]\n", u4value));
    if((u4DramType == 2) || (u4DramType == 3))
    { 
        u4value = u4IO32ReadFldAlign(DRAMC_REG_MISC_STATUSA + SHIFT_TO_CHB_ADDR, MISC_STATUSA_SREF_STATE);
        mcSHOW_ERR_MSG(("!CHB SREF_STATE[0x%X]\n", u4value));
    }
    return;
}


void dump_gating_error_rg(DRAMC_CTX_T * p, U32 u4DramType)
{
    U32 u4value = 0;

    u4value = u4IO32Read4B(DDRPHY_MISC_STBERR_RK0_R);
    mcSHOW_ERR_MSG(("###DDR reserve CHA DDRPHY_MISC_STBERR_RK0_R [0x%X]\n", u4value));
    if((u4DramType == 2) || (u4DramType == 3))
    {
        u4value = u4IO32Read4B(DDRPHY_MISC_STBERR_RK0_R + SHIFT_TO_CHB_ADDR);
        mcSHOW_ERR_MSG(("###DDR reserve CHB DDRPHY_MISC_STBERR_RK0_R[0x%X]\n", u4value));
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
        (DRAMC_REG_DRAMC_PD_CTRL + SHIFT_TO_CHB_ADDR),
        (DDRPHY_MISC_CG_CTRL5),
        (DDRPHY_MISC_CG_CTRL5 + SHIFT_TO_CHB_ADDR),
        (DRAMC_REG_PRE_TDQSCK1),
        (DRAMC_REG_PRE_TDQSCK1 + SHIFT_TO_CHB_ADDR),
        (DRAMC_REG_STBCAL),
        (DRAMC_REG_STBCAL + SHIFT_TO_CHB_ADDR),
        (DDRPHY_MISC_CTRL1),
        (DDRPHY_MISC_CTRL1 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B0_RXDVS0),
        (DDRPHY_B0_RXDVS0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_B1_RXDVS0),
        (DDRPHY_B1_RXDVS0 + SHIFT_TO_CHB_ADDR),        
        (DDRPHY_R0_B0_RXDVS2),
        (DDRPHY_R0_B0_RXDVS2 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_R1_B0_RXDVS2),
        (DDRPHY_R1_B0_RXDVS2 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_R0_B1_RXDVS2),
        (DDRPHY_R0_B1_RXDVS2 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_R1_B1_RXDVS2),
        (DDRPHY_R1_B1_RXDVS2 + SHIFT_TO_CHB_ADDR)
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


void DramcDcmDisBySRAM(DRAMC_CTX_T *p)
{
    DRAM_DFS_FREQUENCY_TABLE_T *pFreqTable = p->pDFSTable; // from dramc conf shu0
    U8 u1ShuffleIdx;
    U32 u4DramcShuOffset = 0;
    U32 u4DDRPhyShuOffset = 0;
    U32 u4RegBackupAddress[] = 
    {       
        (DDRPHY_SRAM_DMA0),
        (DDRPHY_SRAM_DMA0 + SHIFT_TO_CHB_ADDR),
        (DDRPHY_SRAM_DMA2),
        (DDRPHY_SRAM_DMA2 + SHIFT_TO_CHB_ADDR),
    };

    //Backup regs
    DramcBackupRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
	
    //Aceess DMA SRAM by APB bus use debug mode by conf SHU3
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA0, 0x1, SRAM_DMA0_R_APB_SLV_SEL);
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA2, 0x1, SRAM_DMA2_R_APB_DMA_DBG_ACCESS);

    for(u1ShuffleIdx = 0; u1ShuffleIdx<=DRAM_DFS_SRAM_MAX; u1ShuffleIdx++) //fill SHU1 of conf while (u1ShuffleIdx==DRAM_DFS_SRAM_MAX)
    {
        if(u1ShuffleIdx == DRAM_DFS_SRAM_MAX)
        {
            //for SHU0 restore to SRAM
            vSetDFSTable(p, pFreqTable);//Restore DFS table
            u4DramcShuOffset = 0;
            u4DDRPhyShuOffset = 0;
            //Restore regs, or SHU0 RG cannot be set
            DramcRestoreRegisters(p, u4RegBackupAddress, sizeof(u4RegBackupAddress)/sizeof(U32));
        }
        else
        {
            //Aceess DMA SRAM by APB bus use debug mode by conf SHU3
            vSetDFSTable(p, get_FreqTbl_by_shuffleIndex(p, u1ShuffleIdx));//Update DFS table
            u4DramcShuOffset = SHU_GRP_DRAMC_OFFSET*3; //SHU3 
            u4DDRPhyShuOffset = SHU_GRP_DDRPHY_OFFSET*3; //SHU3
            vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA2, u1ShuffleIdx, SRAM_DMA2_R_APB_DMA_DBG_LEVEL);
        }

        // add your function
        // For example: EnableDramcPhyDCMShuffle(p, enable_dcm, u4DramcShuOffset, u4DDRPhyShuOffset, u1ShuffleIdx);
#if (SW_CHANGE_FOR_SIMULATION == 0)
        EnableDramcPhyDCMShuffle(p, 0, u4DramcShuOffset, u4DDRPhyShuOffset);
#endif        
    }
}



void Dramc_DDR_Reserved_Mode_setting(void)
{
    U32 u4WbrBackup = 0;
    U32 u4value = 0;
    U32 backup_broadcast = GetDramcBroadcast();
    DRAMC_CTX_T * p;
    psCurrDramCtx = &DramCtx_LPDDR4;
    p = psCurrDramCtx;

    mcSHOW_DBG_MSG(("Enter Dramc_DDR_Reserved_Mode_setting \n"));
    Set_Spm_Poweron_Config_En(p);
    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);	

    u4value = u4IO32ReadFldAlign(DRAMC_REG_SHUSTATUS, SHUSTATUS_SHUFFLE_LEVEL);//from conf: 0 or 1
    mcSHOW_ERR_MSG(("### shuffle level[%d]\n", u4value));
#if 0//ENABLE_DVFS_CDC_SYNCHRONIZER_OPTION
    if(u4IO32ReadFldAlign(DRAMC_REG_SHUCTRL2, SHUCTRL2_R_DVFS_CDC_OPTION))    
    {   
        DDR_Reserved_Mode_Cdc_Option_Patch(p);
    }
#endif

    vSetDFSFreqSelByTable(p, &gFreqTbl[3]);
    DramcDcmDisBySRAM(p);//free run clk for DRM
    EnableDramcPhyDCMNonShuffle(p, 0);//free run clk for DRM

#ifdef HW_GATING
    DramcHWGatingOnOff(p, 0);//Disable HW Gating tracking for gating tracking fifo mode
#endif
    u4DramType = u4IO32ReadFldAlign(DRAMC_REG_ARBCTL, ARBCTL_RSV_DRAM_TYPE);
    mcSHOW_DBG_MSG(("### dram type =%x \n", u4DramType));

#ifdef IMPEDANCE_TRACKING_ENABLE
    //Disable IMP tracking
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 0, IMPCAL_IMPCAL_HW);
#endif
    // Disable HW ZQ
    vIO32WriteFldAlign_All(DRAMC_REG_ADDR(DRAMC_REG_SPCMDCTRL), 0, SPCMDCTRL_ZQCALDISB);// LP3 and LP4 are different, be careful.

#if ENABLE_TX_TRACKING
    //Disable TX tracking
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS);
#endif
#if ENABLE_RX_TRACKING_LP4
    //Disable RX tracking
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);
#endif
    // For DDR800semi WA
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE); // for CA_PI clock CG free-run
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL5, 0x0, MISC_CG_CTRL5_R_CA_PI_DCM_EN);
    
    //Backup regs
    //NOTE: Restore regs by Dramc_DDR_Reserved_Mode_AfterSR
    DramcBackupRegisters(p, u4ReserveRegBackupAddress, sizeof(u4ReserveRegBackupAddress)/sizeof(U32));

    //Pipe line dcm off
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL4, u4IO32Read4B(DDRPHY_MISC_CG_CTRL4) | 0x33000000, MISC_CG_CTRL4_R_PHY_MCK_CG_CTRL);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x1,MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE); // @Darren, free-run for exit self-refresh reset signal

    //Close RX tracking by conf RG, fix 1:4 mode enter DDR reserve mode fail. PIC: Lynx
    vIO32WriteFldAlign_All(DDRPHY_B0_RXDVS0, 0x0, B0_RXDVS0_R_RX_DLY_TRACK_ENA_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 0x0, B1_RXDVS0_R_RX_DLY_TRACK_ENA_B1);
    // clear rx tracking FSM - start
    vIO32WriteFldAlign_All(DDRPHY_B0_RXDVS0, 1, B0_RXDVS0_R_RX_DLY_TRACK_CLR_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_RXDVS0, 1, B1_RXDVS0_R_RX_DLY_TRACK_CLR_B1);
    // clear rx tracking FSM - end
    vIO32WriteFldAlign_All(DDRPHY_R0_B0_RXDVS2, 0x0, R0_B0_RXDVS2_R_RK0_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R0_B1_RXDVS2, 0x0, R0_B1_RXDVS2_R_RK0_DVS_MODE_B1);
    vIO32WriteFldAlign_All(DDRPHY_R1_B0_RXDVS2, 0x0, R1_B0_RXDVS2_R_RK1_DVS_MODE_B0);
    vIO32WriteFldAlign_All(DDRPHY_R1_B1_RXDVS2, 0x0, R1_B1_RXDVS2_R_RK1_DVS_MODE_B1);
    // @Darren, Fix rx tracking cause DDR3733 TA2 fail via ddr reserved mode
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQS0_DVS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQS0_DVS_EN_B1);

    //Disable MR4
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 0x0, SPCMDCTRL_SPDR_MR4_OPT); // @Darren, disable MR4 after leave sref
    vIO32WriteFldAlign_All(DRAMC_REG_SPCMDCTRL, 1, SPCMDCTRL_REFRDIS);

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
    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM) 
                | P_Fld(0, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b00: 26MHz, [0]: change memory clock
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change

    //Load shuffle 0 setting from SRAM by DMA 
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA0, 0, SRAM_DMA0_R_SW_DMA_FIRE);
    vIO32WriteFldMulti_All(DDRPHY_SRAM_DMA0, P_Fld(1, SRAM_DMA0_R_SW_MODE)
                | P_Fld(1, SRAM_DMA0_R_SW_STEP_EN_MODE));
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA0, 0, SRAM_DMA0_R_SRAM_WR_MODE);
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA0, 1, SRAM_DMA0_R_APB_WR_MODE);
    vIO32WriteFldMulti_All(DDRPHY_SRAM_DMA0, P_Fld(0, SRAM_DMA0_R_SW_SHU_LEVEL_APB)
                | P_Fld(0, SRAM_DMA0_R_SW_SHU_LEVEL_SRAM));
    vIO32WriteFldAlign_All(DDRPHY_SRAM_DMA0, 1, SRAM_DMA0_R_SW_DMA_FIRE);
    //Wait sram_done APB_done
    while((u4IO32ReadFldAlign(DDRPHY_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_APB_DONE) != 1) || 
        (u4IO32ReadFldAlign(DDRPHY_MISC_DMA_DEBUG0, MISC_DMA_DEBUG0_SRAM_DONE) != 1));
    while((u4IO32ReadFldAlign(DDRPHY_MISC_DMA_DEBUG0 + SHIFT_TO_CHB_ADDR, MISC_DMA_DEBUG0_APB_DONE) != 1) ||
        (u4IO32ReadFldAlign(DDRPHY_MISC_DMA_DEBUG0 + SHIFT_TO_CHB_ADDR, MISC_DMA_DEBUG0_SRAM_DONE) != 1));
    vIO32WriteFldMulti_All(DDRPHY_SRAM_DMA0, P_Fld(0, SRAM_DMA0_R_SW_DMA_FIRE)
                | P_Fld(0, SRAM_DMA0_R_SW_MODE)
                | P_Fld(0, SRAM_DMA0_R_SW_STEP_EN_MODE));

#if ENABLE_TX_TRACKING
    vIO32WriteFldAlign_All(DRAMC_REG_DRAMC_PD_CTRL, 0x0, DRAMC_PD_CTRL_DCMEN2);

    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 1, MISC_CTRL1_R_DMARPIDQ_SW);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CTRL1, 0, MISC_CTRL1_R_DMARPIDQ_SW);
#endif

    //===========================================================================================================
    //PLL close sequence: (Need to sync with Maoauo)
    //DLL_PHDET_EN_* = 0 
    //==> CG = 1 
    //==> MIDPI_EN = 0(async) 
    //==> RG_ARPI_RESETB_* = 0 
    //==> MCK8X_EN(source of clk gating) = 0 
    //==> PLL_EN = 0 PIC: Ying-Yu

    //RG_*PHDET_EN = 0 (DLL)
#if ENABLE_MCK8X_MODE
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_DLL0, 0x0, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x0, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x0, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);
#else
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x0, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x0, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x0, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
#endif
    //RG_*VREF_EN=0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x0, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x0, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD5, 0x0, CA_CMD5_RG_RX_ARCMD_VREF_EN);

    //*PI_CG = 1, RG_*MPDIV_CG = 1
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

    //RG_*MIDPI_EN = 0
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ6, 0x0, SHU_B0_DQ6_RG_ARPI_MIDPI_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ6, 0x0, SHU_B1_DQ6_RG_ARPI_MIDPI_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD6, 0x0, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA);



    //RG_*BIAS_EN = 0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x0, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x0, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x0, CA_CMD6_RG_RX_ARCMD_BIAS_EN);


    //RG_*RESETB = 0
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x0, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x0, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x0, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x0, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x0, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x0, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    vIO32WriteFldAlign(DDRPHY_PLL4, 0x0, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA

    //RG_*MCK8X_EN = 0	
#if ENABLE_MCK8X_MODE    
    vIO32WriteFldAlign(DDRPHY_SHU_PLL22, 0, SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
    vIO32WriteFldAlign(DDRPHY_PLL4, 0, PLL4_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA
#else
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(0, PLL4_RG_RPHYPLL_ADA_MCK8X_EN) 
                    | P_Fld(0, PLL4_RG_RPHYPLL_AD_MCK8X_EN));//RG_*MCK8X_EN=0; Since there is only 1 PLL, only to control CHA
#endif
    //PLL_EN = 0
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);//RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);//RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    //===========================================================================================================
    MPLLInit();
    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) &= ~(0x1 << 2); //ddrphy_pwr_on=0
    //*((UINT32P)(0x100063b8 )) &= ~(0x1 << 2);
    mcDELAY_US(1);
    //marked by kaihsin on May 7th, not necessary to PDN DDRPHY
    //*((UINT32P)(0x1000631c )) |= (0x1 << 2); //ddrphy_pwr_on=1
    //*((UINT32P)(0x100063b8 )) |= (0x1 << 2);
    mcDELAY_US(1);

    //PLL open sequence
    //PLL_EN = 1 
    //==> some of MIDPI*_EN = 1(async) 
    //==> RG_ARPI_RESETB_* = 1(async, open first) 
    //==> MCK8X_EN(source of clk gating) = 1 
    //==> CG = 0 
    //==> DLL_PHDET_EN_* = 1 PIC: Ying-Yu

    //RG_*RESETB = 1
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ3, 0x1, B0_DQ3_RG_ARDQ_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ3, 0x1, B1_DQ3_RG_ARDQ_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD3, 0x1, CA_CMD3_RG_ARCMD_RESETB);
    vIO32WriteFldAlign(DDRPHY_PLL4, 0x1, PLL4_RG_RPHYPLL_RESETB);//Since there is only 1 PLL, only control CHA

    vIO32WriteFldMulti(SPM_SPM_POWER_ON_VAL0, P_Fld(0x0, SPM_POWER_ON_VAL0_SC_MPLL_S_OFF)
                | P_Fld(0x0, SPM_POWER_ON_VAL0_SC_MPLL_OFF));
    mcDELAY_US(20);

    //Change DVFS to SPM mode
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);//change DVFS to RG mode
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);//toggle shu_en to sync shu_level, need MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL = 1
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 1, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);//toggle shu_en to sync shu_level
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL_SHU_EN);//toggle shu_en to sync shu_level
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_RG_PHYPLL2_SHU_EN);//toggle shu_en to sync shu_level
    vIO32WriteFldAlign(DDRPHY_MISC_SPM_CTRL1, 0, MISC_SPM_CTRL1_SPM_DVFS_CONTROL_SEL);//change DVFS to SPM mode

    //MPDIV_CG = 0 FB_CG = 0 
    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));

    //RG_*PLL_EN = 1
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x1, PLL1_RG_RPHYPLL_EN);//RG_*PLL_EN=1; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x1, PLL2_RG_RCLRPLL_EN);//RG_*PLL_EN=1; Since there is only 1 PLL, only control CHA
    mcDELAY_US(20);//Wait 20us for MEMPLL

    //MPDIV_CG = 1 FB_CG = 1 
    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)
                | P_Fld(0x1, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)
                | P_Fld(0x1, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)
                | P_Fld(0x1, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));
     
    //RG_*VREF_EN = 1 (Vref is only used in read, DQ(B0, B1)is use for RX) 
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ5, 0x1, B0_DQ5_RG_RX_ARDQ_VREF_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ5, 0x1, B1_DQ5_RG_RX_ARDQ_VREF_EN_B1);
    mcDELAY_US(1);

    //MIDPI*_EN = 1
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DQ6, 0x1, SHU_B0_DQ6_RG_ARPI_MIDPI_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DQ6, 0x1, SHU_B1_DQ6_RG_ARPI_MIDPI_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_CMD6, 0x1, SHU_CA_CMD6_RG_ARPI_MIDPI_EN_CA);
    mcDELAY_US(10);

    //RG_ARPI_RESETB_* = 1
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI0, 0x1, B0_DLL_ARPI0_RG_ARPI_RESETB_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI0, 0x1, B1_DLL_ARPI0_RG_ARPI_RESETB_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI0, 0x1, CA_DLL_ARPI0_RG_ARPI_RESETB_CA);
    mcDELAY_US(1);

    //RG_*MCK8X_EN = 1	
#if ENABLE_MCK8X_MODE     
    vIO32WriteFldAlign(DDRPHY_SHU_PLL22, 1, SHU_PLL22_RG_RPHYPLL_ADA_MCK8X_EN_SHU);
    vIO32WriteFldAlign(DDRPHY_PLL4, 1, PLL4_RG_RPHYPLL_AD_MCK8X_EN);//RG_*MCK8X_EN=1; Since there is only 1 PLL, only to control CHA
#else
    vIO32WriteFldMulti(DDRPHY_PLL4, P_Fld(1, PLL4_RG_RPHYPLL_ADA_MCK8X_EN) 
                    | P_Fld(1, PLL4_RG_RPHYPLL_AD_MCK8X_EN));//RG_*MCK8X_EN=1; Since there is only 1 PLL, only to control CHA
#endif                    
    mcDELAY_US(1);

    //*PI_CG = 0, RG_*MPDIV_CG = 0
    vIO32WriteFldMulti_All(DDRPHY_B0_DLL_ARPI2, P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_MCTL_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_FB_B0)//
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQS_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQM_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQ_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B0)
                | P_Fld(0x0, B0_DLL_ARPI2_RG_ARPI_MPDIV_CG_B0));//
    vIO32WriteFldMulti_All(DDRPHY_B1_DLL_ARPI2, P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_MCTL_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_FB_B1)//
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQS_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQM_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQ_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_CG_DQSIEN_B1)
                | P_Fld(0x0, B1_DLL_ARPI2_RG_ARPI_MPDIV_CG_B1));//
    vIO32WriteFldMulti_All(DDRPHY_CA_DLL_ARPI2, P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_CA)//not include 11th bit(CA_DLL_ARPI2_RG_ARPI_CG_CLKIEN) LewisToDo
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCK_FB2DLL_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_MCTL_CA)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_FB_CA)//
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CS)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CLK)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_CG_CMD)
                | P_Fld(0x0, CA_DLL_ARPI2_RG_ARPI_MPDIV_CG_CA));//
    
    //RG_*BIAS_EN = 1
    vIO32WriteFldAlign_All(DDRPHY_B0_DQ6, 0x1, B0_DQ6_RG_RX_ARDQ_BIAS_EN_B0);
    vIO32WriteFldAlign_All(DDRPHY_B1_DQ6, 0x1, B1_DQ6_RG_RX_ARDQ_BIAS_EN_B1);
    vIO32WriteFldAlign_All(DDRPHY_CA_CMD6, 0x1, CA_CMD6_RG_RX_ARCMD_BIAS_EN);
    mcDELAY_US(1);

    vIO32WriteFldMulti_All(DDRPHY_MISC_CG_CTRL0, P_Fld(0, MISC_CG_CTRL0_W_CHG_MEM) 
                | P_Fld(1, MISC_CG_CTRL0_CLK_MEM_SEL));//[5:4] mem_ck mux: 2'b01: memory clock, [0]: change memory clock
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 1, MISC_CG_CTRL0_W_CHG_MEM);//change clock freq
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0, MISC_CG_CTRL0_W_CHG_MEM);//disable memory clock change
    
    //force top feedback MCK not divide
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, 3, DRAMC_DPY_CLK_SW_CON_SW_DDRPHY_FB_CK_EN);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);

    //DLL
    //vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    //mcDELAY_US(1);
    //vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);
    //mcDELAY_US(1);
    //vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);
    //mcDELAY_US(1);
    //vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);
    //mcDELAY_US(1);
#if DLL_ASYNC_MODE 
#if ENABLE_MCK8X_MODE 
    vIO32WriteFldAlign_All(DDRPHY_SHU_CA_DLL0, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);//Since CHA CA and CHB CA are DLL master
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x1, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x1, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);//2nd DLL enable
    mcDELAY_US(1);
#else 
    vIO32WriteFldAlign_All(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//Since CHA CA and CHB CA are DLL master
    mcDELAY_US(1);
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);//2nd DLL enable
    mcDELAY_US(1);
#endif    
#else //DLL_ASYNC_MODE
#if ENABLE_MCK8X_MODE 
    vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);//Since CHA CA is DLL master
    mcDELAY_US(1);
    vIO32WriteFldAlign(DDRPHY_SHU_CA_DLL0 + SHIFT_TO_CHB_ADDR, 0x1, SHU_CA_DLL0_RG_ARDLL_PHDET_EN_CA_SHU);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_SHU_B0_DLL0, 0x1, SHU_B0_DLL0_RG_ARDLL_PHDET_EN_B0_SHU);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_SHU_B1_DLL0, 0x1, SHU_B1_DLL0_RG_ARDLL_PHDET_EN_B1_SHU);//2nd DLL enable
    mcDELAY_US(1);
#else
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//Since CHA CA is DLL master
    mcDELAY_US(1);
    vIO32WriteFldAlign(DDRPHY_CA_DLL_ARPI2 + SHIFT_TO_CHB_ADDR, 0x1, CA_DLL_ARPI2_RG_ARDLL_PHDET_EN_CA);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B0_DLL_ARPI2, 0x1, B0_DLL_ARPI2_RG_ARDLL_PHDET_EN_B0);//2nd DLL enable
    vIO32WriteFldAlign_All(DDRPHY_B1_DLL_ARPI2, 0x1, B1_DLL_ARPI2_RG_ARDLL_PHDET_EN_B1);//2nd DLL enable
    mcDELAY_US(1);
#endif    
#endif

    // For DDR800semi WA restore
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL5, 0x1, MISC_CG_CTRL5_R_CA_PI_DCM_EN);
    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x0, MISC_CG_CTRL0_RG_CG_CMD_OFF_DISABLE); // for CA_PI clock CG free-run
    
    //top feedback MCK to divided frequency
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON, 0, DRAMC_DPY_CLK_SW_CON_SW_DDRPHY_FB_CK_EN);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DDRPHY_FB_CK_EN);

    vIO32WriteFldAlign_All(DRAMC_REG_DDRCONF0, 1, DDRCONF0_RDATRST);//R_DMRDATRST = 1
    vIO32WriteFldAlign_All(DRAMC_REG_DDRCONF0, 0, DDRCONF0_RDATRST);//R_DMRDATRST = 0

    //Switching PHYPLL enable path from DDRPHY to SPM control by setting SPM SW
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 1, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);
    mcDELAY_US(1);

    //RG_*PLL_EN=1
    vIO32WriteFldAlign(DDRPHY_PLL1, 0x0, PLL1_RG_RPHYPLL_EN);//disable DDRPHY PHYPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA
    vIO32WriteFldAlign(DDRPHY_PLL2, 0x0, PLL2_RG_RCLRPLL_EN);//disable DDRPHY CLRPLL enable, RG_*PLL_EN=0; Since there is only 1 PLL, only control CHA

    DramcBroadcastOnOff(backup_broadcast);
    //DDRPhyFreqMeter();
    //dump_gating_error_rg(p, u4DramType);
    //dump_SR(p, u4DramType);
}


//#define USE_TA2_IN_DDR_RESERVE_MODE //Just for debug
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

#if MRW_CHECK_ONLY
    mcSHOW_MRW_MSG(("\n==[MR Dump] %s==\n", __func__));
#endif

    DramcBroadcastOnOff(DRAMC_BROADCAST_OFF);	
    mcSHOW_DBG_MSG(("Dramc_DDR_Reserved_Mode_AfterSR dram type =%x \n", u4DramType));
     
    u4LP4_MR13FSP = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_VRCG, SHU_HWSET_VRCG_HWSET_VRCG_OP);//Use the second MRW{MR13 VRCG=0} as reference
    u4LP4_MR2RLWL = u4IO32ReadFldAlign(DRAMC_REG_SHU_HWSET_MR2, SHU_HWSET_MR2_HWSET_MR2_OP);
    //mcSHOW_DBG_MSG(("\n***MR13: 0x%x MR2: 0x%x ***\n", u4LP4_MR13FSP, u4LP4_MR2RLWL));
    
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

#ifdef HW_GATING
    DramcHWGatingOnOff(p,1);//Enable HW Gating tracking for gating tracking fifo mode
#endif
    //Restore regs. @HJ, Must enable DQSG tracking before dummy read/MR4 on
    DramcRestoreRegisters(p, u4ReserveRegBackupAddress, sizeof(u4ReserveRegBackupAddress)/sizeof(U32)); 

    vIO32WriteFldAlign_All(DDRPHY_MISC_CG_CTRL0, 0x0,MISC_CG_CTRL0_RG_CG_PHY_OFF_DIABLE); // @Darren, Rollback free-run for exit self-refresh reset signal 

    dump_gating_error_rg(p, u4DramType);
#if ENABLE_RX_TRACKING_LP4
    //Enable RX tracking
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DPHY_RXDLY_TRACK_EN);
#endif
#if ENABLE_TX_TRACKING
    //Enable TX trackin
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_TX_TRACKING_DIS);
#endif
#ifdef IMPEDANCE_TRACKING_ENABLE
    //Enable IMP tracking, 
    //SPM only handle CHA IMPCAL_IMPCAL_HW, DDR2400 into DDR reserve mode(IMPCAL_IMPCAL_HW CHA = 0 CHB = 1)
    //CHB NAO have been reset to 0, need do hand shake with CHA, but CHA IMP tracking is off --> dead lock
    vIO32WriteFldAlign_All(DRAMC_REG_IMPCAL, 1, IMPCAL_IMPCAL_HW);
#endif    
    DramcBroadcastOnOff(backup_broadcast);
    //DramcRegDump(p);
#ifdef USE_TA2_IN_DDR_RESERVE_MODE
    {   
        mcSHOW_DBG_MSG(("\n***TA2 test after exit DRM***\n"));
        int i = 0;
        for(i = 0; i < 10; i++)
        {   
            TA2_Test_Run_Time_HW(p);
        }   
    }   
#endif
    return;
}


void Before_Init_DRAM_While_Reserve_Mode_fail(DRAM_DRAM_TYPE_T dram_type)
{
    DRAMC_CTX_T * p;
    mcSHOW_DBG_MSG(("\n\tReserve mode fail\tBefore_Init_DRAM_While_Reserve_Mode_fail\n"));
    psCurrDramCtx = &DramCtx_LPDDR4;
    p = psCurrDramCtx;  

#if (SW_CHANGE_FOR_SIMULATION==0)
    EnableDramcPhyDCM(p, 0);
#endif
    Switch26MHzDisableDummyReadRefreshAllBank(p);
    vIO32WriteFldAlign(SPM_DRAMC_DPY_CLK_SW_CON2, 0, DRAMC_DPY_CLK_SW_CON2_SW_PHYPLL_MODE_SW);//PLL control by RG
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_PHYPLL_MODE_SW);//PLL control by RG
}

#if __ETT__
void Func_Emi_On(DRAMC_CTX_T *p);
void Func_Emi_Off(DRAMC_CTX_T *p);
void ETT_DRM(DRAMC_CTX_T *p) 
{
    Func_Emi_Off(p);
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 1, SPM_POWER_ON_VAL0_SC_DMSUS_OFF);//DMSUS set 1
    Dramc_DDR_Reserved_Mode_setting();
    vIO32WriteFldAlign(SPM_SPM_POWER_ON_VAL0, 0, SPM_POWER_ON_VAL0_SC_DMSUS_OFF);//DMSUS set 0
    Func_Emi_On(p);
    Dramc_DDR_Reserved_Mode_AfterSR();
}
#endif
#endif


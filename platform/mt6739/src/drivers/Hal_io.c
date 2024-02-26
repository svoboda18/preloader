/** @file hal_io.cpp
 *  hal_io.cpp provides functions of register access
 */

#include "x_hal_io.h"
//#include "dramc_common.h"
//#include "dramc_pi_api.h"

#if 1//REG_ACCESS_PORTING_DGB
extern U8 RegLogEnable;
#endif

#if (fcFOR_CHIP_ID == fcWhitney) 
const U32 arRegBaseAddrList[12] = 
{
    Channel_A_DRAMC_NAO_BASE_ADDRESS,
    Channel_B_DRAMC_NAO_BASE_ADDRESS,
    Channel_C_DRAMC_NAO_BASE_ADDRESS,
    Channel_D_DRAMC_NAO_BASE_ADDRESS,
    Channel_A_DRAMC_AO_BASE_ADDRESS,
    Channel_B_DRAMC_AO_BASE_ADDRESS,
    Channel_C_DRAMC_AO_BASE_ADDRESS,
    Channel_D_DRAMC_AO_BASE_ADDRESS,
    Channel_A_PHY_BASE_ADDRESS, 
    Channel_B_PHY_BASE_ADDRESS,
    Channel_C_PHY_BASE_ADDRESS,
    Channel_D_PHY_BASE_ADDRESS
};
#endif

DRAMC_CTX_T *pgCurrDramCtx;
void SaveCurrDramCtx(DRAMC_CTX_T *p)
{
    pgCurrDramCtx = p;
}

// Translate SW virtual base address to real HW base.
U32 u4RegBaseAddrTraslate(U32 u4reg_addr)
{
	U32 u4Offset = u4reg_addr & 0x1fff;
	U32 u4RegType = ((u4reg_addr-Channel_A_DRAMC_NAO_BASE_VIRTUAL) >> POS_BANK_NUM) & 0xf;

    if (u4reg_addr < Channel_A_DRAMC_NAO_BASE_VIRTUAL || u4reg_addr >= MAX_BASE_VIRTUAL) return u4reg_addr;

    #if (fcFOR_CHIP_ID == fcWhitney)
    if (u4RegType <=3)
    #endif
    {
        // NAO Register
        if (u4Offset >= (DRAMC_REG_RK0_DQSOSC_STATUS-DRAMC_NAO_BASE_ADDRESS) && u4Offset < (DRAMC_REG_RK1_DQSOSC_STATUS-DRAMC_NAO_BASE_ADDRESS))
        {
            u4Offset += pgCurrDramCtx->rank * 0x100;
        }
    }
    #if (fcFOR_CHIP_ID == fcWhitney)
    else if (u4RegType >=4 && u4RegType <=7)
    #endif
    {
        // AO Register
        if (u4Offset >= (DRAMC_REG_RK0_DQSOSC-DRAMC_AO_BASE_ADDRESS) && u4Offset < (DRAMC_REG_RK1_DQSOSC-DRAMC_AO_BASE_ADDRESS))
        {
            u4Offset += pgCurrDramCtx->rank * 0x100;
        }
        else if (u4Offset >= (DRAMC_REG_SHURK0_DQSCTL-DRAMC_AO_BASE_ADDRESS) && u4Offset < (DRAMC_REG_SHURK1_DQSCTL-DRAMC_AO_BASE_ADDRESS))
        {
            u4Offset += pgCurrDramCtx->rank * 0x100;
        }
    }
    else
    {
        // PHY Register
        if (u4Offset >= (DDRPHY_R0_B0_RXDVS0-DDRPHY_BASE_ADDR) && u4Offset < (DDRPHY_R1_B0_RXDVS0-DDRPHY_BASE_ADDR))
        {
            if ((u4Offset >= (DDRPHY_RFU_0X620-DDRPHY_BASE_ADDR) && u4Offset < (DDRPHY_R0_B1_RXDVS0-DDRPHY_BASE_ADDR)) ||
                (u4Offset >= (DDRPHY_RFU_0X6A0-DDRPHY_BASE_ADDR) && u4Offset < (DDRPHY_R0_CA_RXDVS0-DDRPHY_BASE_ADDR)))
            {
                //mcSHOW_DBG_MSG(("\n[u4RegBaseAddrTraslate] Not Rank address accessed in Rank address!\n"));
            }
            else
            {
                u4Offset += pgCurrDramCtx->rank * 0x200;            
            } 
        }
        else if (u4Offset >= (DDRPHY_SHU1_R0_B0_DQ0-DDRPHY_BASE_ADDR) && u4Offset < (DDRPHY_SHU1_R1_B0_DQ0-DDRPHY_BASE_ADDR))
        {
            u4Offset += pgCurrDramCtx->rank * 0x100;
        }      
        else if (u4Offset >= (DDRPHY_MISC_STBERR_RK0_R-DDRPHY_BASE_ADDR) && u4Offset < (DDRPHY_MISC_STBERR_RK1_R-DDRPHY_BASE_ADDR))
        {
            u4Offset += pgCurrDramCtx->rank * 0x8;
        }
    }
	
       //mcSHOW_DBG_MSG(("\n[u4RegBaseAddrTraslate]  0x%x => 0x%x(0x%x, 0x%x)", u4reg_addr , (arRegBaseAddrList[u4RegType] +u4Offset), arRegBaseAddrList[u4RegType], u4Offset));
	return (arRegBaseAddrList[u4RegType] +u4Offset); 
}

//-------------------------------------------------------------------------
/** ucDram_Register_Read
 *  DRAM register read (32-bit).
 *  @param  u4reg_addr    register address in 32-bit.
 *  @param  pu4reg_value  Pointer of register read value.
 *  @retval 0: OK, 1: FAIL 
 */
//-------------------------------------------------------------------------
// This function need to be porting by BU requirement
U32 u4Dram_Register_Read(U32 u4reg_addr)
{
    U32 u4reg_value;
    
    #if (fcFOR_CHIP_ID == fcWhitney)
    u4reg_addr = u4RegBaseAddrTraslate(u4reg_addr);
    #endif

#if (fcFOR_CHIP_ID == fcWhitney)
   #if (FOR_DV_SIMULATION_USED==1)   //DV
   u4reg_value = register_read_c(u4reg_addr);
    #elif (SW_CHANGE_FOR_SIMULATION==1) // whole chip sim
    u4reg_value = *((UINT32P)(u4reg_addr));
    #else // real chip  
   u4reg_value = (*(volatile unsigned int *)(u4reg_addr));
   #endif
#endif
   return u4reg_value;
}


//-------------------------------------------------------------------------
/** ucDram_Register_Write
 *  DRAM register write (32-bit).
 *  @param  u4reg_addr    register address in 32-bit.
 *  @param  u4reg_value   register write value.
 *  @retval 0: OK, 1: FAIL 
 */
//-------------------------------------------------------------------------
#if REG_SHUFFLE_REG_CHECK
extern U8 ShuffleRegCheck;
extern void ShuffleRegCheckProgram(U32 u4Addr);
#endif

// This function need to be porting by BU requirement
U8 ucDram_Register_Write(U32 u4reg_addr, U32 u4reg_value)
{
    U8 ucstatus;
    ucstatus = 0;  // for SW_CHANGE_FOR_SIMULATION

    #if (fcFOR_CHIP_ID == fcWhitney)
    u4reg_addr = u4RegBaseAddrTraslate(u4reg_addr);
    #endif
    

#if (fcFOR_CHIP_ID == fcWhitney)
        #if (FOR_DV_SIMULATION_USED==1) //DV 
        register_write_c(u4reg_addr,u4reg_value);   
        #elif (SW_CHANGE_FOR_SIMULATION==1) // //whole chip sim
        (*(volatile unsigned int *)u4reg_addr) = u4reg_value;
        #else
        (*(volatile unsigned int *)u4reg_addr) = u4reg_value;// real chip
        dsb();
        #endif
#endif
        
#if REG_ACCESS_PORTING_DGB
		if(RegLogEnable)
		{
			mcSHOW_DBG_MSG(("\n[REG_ACCESS_PORTING_DBG]   ucDramC_Register_Write Reg(0x%X) = 0x%X\n",u4reg_addr,  u4reg_value));
			mcFPRINTF((fp_A60501, "\n[REG_ACCESS_PORTING_DBG]	ucDramC_Register_Write Reg(0x%X) = 0x%X\n",u4reg_addr,	u4reg_value));
			//mcFPRINTF((fp_A60501, "\nRISCWrite ('h%05X , 32'h%X);\n",u4reg_addr&0xFFFFF,	u4reg_value));
		}
#endif

#if REG_SHUFFLE_REG_CHECK
	if(ShuffleRegCheck)
	{
		ShuffleRegCheckProgram(u4reg_addr);
	}
#endif

    return ucstatus;
}


#if SW_CHANGE_FOR_SIMULATION
U32 u4IO32ReadFldAlign(U32 reg32, U32 fld)
{
    if(Fld_ac(fld)==AC_FULLDW)
    {
        return u4IO32Read4B(reg32);
    }
    else
    {
        return ((u4IO32Read4B(reg32)&Fld2Msk32(fld))>>Fld_shft(fld));
    }
}

void vIO32WriteFldAlign(U32 reg32, U32 val, U32 fld)
{
        if(Fld_ac(fld)==AC_FULLDW)
        {
            vIO32Write4B((reg32),(val));
        }
        else
        {
            vIO32Write4BMsk((reg32),((U32)(val)<<Fld_shft(fld)),Fld2Msk32(fld));
        }
}

void vIO32WriteFldAlign_All(U32 reg32, U32 val, U32 fld)
{
        if(Fld_ac(fld)==AC_FULLDW)
        {
            vIO32Write4B_All((reg32),(val));
        }
        else
        {
            vIO32Write4BMsk_All((reg32),((U32)(val)<<Fld_shft(fld)),Fld2Msk32(fld));
        }
}
#endif

void vIO32Write4BMsk(U32 reg32, U32 val32, U32 msk32)
{
    U32 u4Val;
    
    val32 &=msk32;
    
    u4Val = u4Dram_Register_Read(reg32);
    u4Val = ((u4Val &~msk32)|val32);   
    ucDram_Register_Write(reg32, u4Val);
}


void vIO32Write4B_All(U32 reg32, U32 val32)
{
U8 ii, u1AllCount;
    U32 u4RegType = (reg32 & (0xf <<POS_BANK_NUM));
    
    reg32 &= 0xffff;     // remove channel information
    
    if(u4RegType >=Channel_A_PHY_BASE_VIRTUAL)//PHY 
    {
#if CHANNEL_NUMBER==4    
        u1AllCount =4;
#else
        u1AllCount =2;
#endif
        reg32 += Channel_A_PHY_BASE_VIRTUAL;
    }
    else if(u4RegType >=Channel_A_DRAMC_AO_BASE_VIRTUAL)// DramC AO
    {
#if CHANNEL_NUMBER==4    
                u1AllCount =4;
#else
        u1AllCount =2;
#endif
        reg32 += Channel_A_DRAMC_AO_BASE_VIRTUAL;
    }
    else // DramC NAO
    {
#if CHANNEL_NUMBER==4    
                u1AllCount =4;
#else
        u1AllCount =2;
#endif
        reg32 += Channel_A_DRAMC_NAO_BASE_VIRTUAL;
    }

for(ii=0; ii< u1AllCount; ii++)
{
    vIO32Write4B(reg32+((U32)ii<<POS_BANK_NUM), val32);
}   


#if 0
    // debug
    //mcSHOW_DBG_MSG(("RISCWrite : address %05x data %08x\n", reg32&0x000fffff, val32));
    mcFPRINTF((fp_A60501, "RISCWrite : address %05x data %08x wait  0\n", reg32&0x000fffff, val32));
    //mcSHOW_DBG_MSG(("RISCWrite : address %05x data %08x\n", (reg32+((U32)1<<POS_BANK_NUM))&0x000fffff, val32));
    mcFPRINTF((fp_A60501, "RISCWrite : address %05x data %08x wait  0\n", (reg32+((U32)1<<POS_BANK_NUM))&0x000fffff, val32));
    //mcSHOW_DBG_MSG(("RISCWrite : address %05x data %08x\n", (reg32+((U32)2<<POS_BANK_NUM))&0x000fffff, val32));
    mcFPRINTF((fp_A60501, "RISCWrite : address %05x data %08x wait  0\n", (reg32+((U32)2<<POS_BANK_NUM))&0x000fffff, val32));
#endif
}

void vIO32Write4BMsk_All(U32 reg32, U32 val32, U32 msk32)
{
    U32 u4Val;
    U8 ii, u1AllCount;
    U32 u4RegType = (reg32 & (0xf <<POS_BANK_NUM));

    reg32 &= 0xffff;     // remove channel information

    if(u4RegType >=Channel_A_PHY_BASE_VIRTUAL)//PHY 
    {
#if CHANNEL_NUMBER==4    
        u1AllCount =4;
#else
        u1AllCount =2;
#endif
        reg32 += Channel_A_PHY_BASE_VIRTUAL;
    }
    else if(u4RegType >=Channel_A_DRAMC_AO_BASE_VIRTUAL)// DramC AO
    {
#if CHANNEL_NUMBER==4    
        u1AllCount =4;
#else
        u1AllCount =2;
#endif
        reg32 += Channel_A_DRAMC_AO_BASE_VIRTUAL;
    }
    else // DramC NAO
    {
#if CHANNEL_NUMBER==4    
        u1AllCount =4;
#else
        u1AllCount =2;
#endif
        reg32 += Channel_A_DRAMC_NAO_BASE_VIRTUAL;
    }

    for(ii=0; ii< u1AllCount; ii++)
    {
        u4Val = u4Dram_Register_Read(reg32+((U32)ii<<POS_BANK_NUM));    
        u4Val = ((u4Val &~msk32)|val32);    
        ucDram_Register_Write(reg32+((U32)ii<<POS_BANK_NUM), u4Val);
    }


}

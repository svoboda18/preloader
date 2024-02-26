/* * @file hal_io.cpp
 * hal_io.cpp provides functions of register access
 */

#include "x_hal_io.h"
#include "dramc_common.h"
#include "dramc_pi_api.h"

#if __ETT__
#include <barriers.h>
#endif

#if 1/* REG_ACCESS_PORTING_DGB */
extern U8 RegLogEnable;
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
U8  gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag = 0;
#endif

#if defined(SLT) || (FT_DSIM_USED)
#define SHUFFLE_ADDR_NUM_DRAMC          4
#define SHUFFLE_ADDR_NUM_DDRPHY         14

typedef struct _SHUFFLE_REG_ADDR
{
	U32 u4StartAddr;
	U32 u4EndAddr;
} SHUFFLE_REG_ADDR;

U32 u4RegShuffleOffset(U32 u4RegType, U32 u4Offset, U32 u4Shuffle_Index)
{
	SHUFFLE_REG_ADDR ShuffleRegTableDramc[SHUFFLE_ADDR_NUM_DRAMC] =
	{
		{DRAMC_AO_BASE_ADDRESS + (0x0800), DRAMC_AO_BASE_ADDRESS + (0x08DC) },
		{DRAMC_AO_BASE_ADDRESS + (0x0A00), DRAMC_AO_BASE_ADDRESS + (0x0A50) },
		{DRAMC_AO_BASE_ADDRESS + (0x0B00), DRAMC_AO_BASE_ADDRESS + (0x0B50) },
		{DRAMC_AO_BASE_ADDRESS + (0x0C00), DRAMC_AO_BASE_ADDRESS + (0x0C54) },
	};
	SHUFFLE_REG_ADDR ShuffleRegTableDDRPhy[SHUFFLE_ADDR_NUM_DDRPHY] =
	{
		{DDRPHY_AO_BASE_ADDRESS + (0x0C00), DDRPHY_AO_BASE_ADDRESS + (0x0C38) },
		{DDRPHY_AO_BASE_ADDRESS + (0x0C80), DDRPHY_AO_BASE_ADDRESS + (0x0CB8) },
		{DDRPHY_AO_BASE_ADDRESS + (0x0D00), DDRPHY_AO_BASE_ADDRESS + (0x0D38) },
		{DDRPHY_AO_BASE_ADDRESS + (0x0D80), DDRPHY_AO_BASE_ADDRESS + (0x0DBC) }, /* PLL */
		{DDRPHY_AO_BASE_ADDRESS + (0x0DD0), DDRPHY_AO_BASE_ADDRESS + (0x0DF0) }, /* PLL */
		{DDRPHY_AO_BASE_ADDRESS + (0x0E00), DDRPHY_AO_BASE_ADDRESS + (0x0E2C) }, /* R0 */
		{DDRPHY_AO_BASE_ADDRESS + (0x0E50), DDRPHY_AO_BASE_ADDRESS + (0x0E7C) },
		{DDRPHY_AO_BASE_ADDRESS + (0x0EA0), DDRPHY_AO_BASE_ADDRESS + (0x0ECC) },
		{DDRPHY_AO_BASE_ADDRESS + (0x0F00), DDRPHY_AO_BASE_ADDRESS + (0x0F2C) }, /* R1 */
		{DDRPHY_AO_BASE_ADDRESS + (0x0F50), DDRPHY_AO_BASE_ADDRESS + (0x0F7C) },
		{DDRPHY_AO_BASE_ADDRESS + (0x0FA0), DDRPHY_AO_BASE_ADDRESS + (0x0FCC) },
		{DDRPHY_AO_BASE_ADDRESS + (0x1000), DDRPHY_AO_BASE_ADDRESS + (0x102C) }, /* R2 */
		{DDRPHY_AO_BASE_ADDRESS + (0x1050), DDRPHY_AO_BASE_ADDRESS + (0x107C) },
		{DDRPHY_AO_BASE_ADDRESS + (0x10A0), DDRPHY_AO_BASE_ADDRESS + (0x10CC) },
	};

	U32 tbIdx;

	/* mcSHOW_DUMP_INIT_RG_MSG(("fra u4RegType=%d, u4Offset=0x%x, u4Shuffle_Index=%d ==\n",u4RegType, u4Offset, u4Shuffle_Index)); */


	if (u4RegType >= 2 && u4RegType <= 3)
	for (tbIdx = 0; tbIdx < SHUFFLE_ADDR_NUM_DRAMC; tbIdx++)
	{
		if (u4Offset >= (ShuffleRegTableDramc[tbIdx].u4StartAddr & 0x1fff) && u4Offset <= (ShuffleRegTableDramc[tbIdx].u4EndAddr & 0x1fff))
		{
			/* mcSHOW_DUMP_INIT_RG_MSG(("fra dramc offset=0x%x (0x%x, 0x%x) ==\n",u4Offset,ShuffleRegTableDramc[tbIdx].u4StartAddr, ShuffleRegTableDramc[tbIdx].u4EndAddr)); */
			return SHU_GRP_DRAMC_OFFSET*u4Shuffle_Index;
		}
	}

	if (u4RegType >= 6 && u4RegType <= 7)
	for (tbIdx = 0; tbIdx < SHUFFLE_ADDR_NUM_DDRPHY; tbIdx++)
	{
		if (u4Offset >= (ShuffleRegTableDDRPhy[tbIdx].u4StartAddr & 0x1fff) && u4Offset <= (ShuffleRegTableDDRPhy[tbIdx].u4EndAddr & 0x1fff))
		{
			/* mcSHOW_DUMP_INIT_RG_MSG(("fra phy offset=0x%x (0x%x, 0x%x) ==\n",u4Offset,ShuffleRegTableDDRPhy[tbIdx].u4StartAddr, ShuffleRegTableDDRPhy[tbIdx].u4EndAddr)); */
			return SHU_GRP_DDRPHY_OFFSET*u4Shuffle_Index;
		}
	}

	return 0;
}

U32 u4RegBaseAddrTraslate(DRAM_RANK_T eRank, U32 u4reg_addr, U32 u4Shuffle_Index)
#else
U32 u4RegBaseAddrTraslate(DRAM_RANK_T eRank, U32 u4reg_addr)
#endif
{
	U32 u4Offset = u4reg_addr & 0x1fff;
	U32 u4RegType = ((u4reg_addr - Channel_A_DRAMC_NAO_BASE_VIRTUAL) >> POS_BANK_NUM) & 0xf;
	U32 u4BaseAddr = 0;

	if (u4reg_addr < Channel_A_DRAMC_NAO_BASE_VIRTUAL || u4reg_addr >= MAX_BASE_VIRTUAL) return u4reg_addr;

	if (eRank == RANK_1)
	{
		if (u4RegType <= 1)
		{
			/* DramC NAO Register */
			if (u4Offset >= (DRAMC_REG_RK0_DQSOSC_STATUS - DRAMC_NAO_BASE_ADDRESS) && u4Offset < (DRAMC_REG_RK1_DQSOSC_STATUS - DRAMC_NAO_BASE_ADDRESS))
			{
				u4Offset += 0x100;
			}
		}
		else if (u4RegType >= 2 && u4RegType <= 3)
		{
			/* DramC AO Register */
			if (u4Offset >= (DRAMC_REG_RK0_DQSOSC - DRAMC_AO_BASE_ADDRESS) && u4Offset < (DRAMC_REG_RK1_DQSOSC - DRAMC_AO_BASE_ADDRESS))
			{
				u4Offset += 0x100;
			}
			else if (u4Offset >= (DRAMC_REG_SHURK0_DQSCTL - DRAMC_AO_BASE_ADDRESS) && u4Offset < (DRAMC_REG_SHURK1_DQSCTL - DRAMC_AO_BASE_ADDRESS))
			{
			#if defined(SLT) || (FT_DSIM_USED)
				u4Offset += 0x100 + u4Shuffle_Index*SHU_GRP_DRAMC_OFFSET;
			#else
				u4Offset += 0x100;
			#endif
			}
			else if (u4Offset >= (DRAMC_REG_SHU2RK0_DQSCTL - DRAMC_AO_BASE_ADDRESS) && u4Offset < (DRAMC_REG_SHU2RK1_DQSCTL - DRAMC_AO_BASE_ADDRESS))
			{
				u4Offset += 0x100;
			}
			else if (u4Offset >= (DRAMC_REG_SHU3RK0_DQSCTL - DRAMC_AO_BASE_ADDRESS) && u4Offset < (DRAMC_REG_SHU3RK1_DQSCTL - DRAMC_AO_BASE_ADDRESS))
			{
				u4Offset += 0x100;
			}
		}
		else if (u4RegType >= 4 && u4RegType <= 5)
		{
			/* PHY NAO Register */
		}
		else
		{
			/* PHY AO Register */
			if (u4Offset >= (DDRPHY_R0_B0_RXDVS0 - DDRPHY_AO_BASE_ADDRESS) && u4Offset < (DDRPHY_R1_B0_RXDVS0 - DDRPHY_AO_BASE_ADDRESS))
			{
				if ((u4Offset >= (DDRPHY_RFU_0X620 - DDRPHY_AO_BASE_ADDRESS) && u4Offset < (DDRPHY_R0_B1_RXDVS0 - DDRPHY_AO_BASE_ADDRESS))
					|| (u4Offset >= (DDRPHY_RFU_0X6A0 - DDRPHY_AO_BASE_ADDRESS) && u4Offset < (DDRPHY_R1_B0_RXDVS0 - DDRPHY_AO_BASE_ADDRESS))
					)
				{
					/* mcSHOW_DBG_MSG(("\n[u4RegBaseAddrTraslate] Not Rank address accessed in Rank address!\n")); */
				}
				else
				{
					u4Offset += 0x200;
				}
			}
			else if (u4Offset >= (DDRPHY_SHU1_R0_B0_DQ0 - DDRPHY_AO_BASE_ADDRESS) && u4Offset < (DDRPHY_SHU1_R1_B0_DQ0 - DDRPHY_AO_BASE_ADDRESS))
			{
			#if defined(SLT) || (FT_DSIM_USED)
				u4Offset += 0x100 + u4Shuffle_Index*SHU_GRP_DDRPHY_OFFSET;
			#else
				u4Offset += 0x100;
			#endif
			}
			else if (u4Offset >= (DDRPHY_MISC_STBERR_RK0_R - DDRPHY_AO_BASE_ADDRESS) && u4Offset < (DDRPHY_MISC_STBERR_RK1_R - DDRPHY_AO_BASE_ADDRESS))
			{
				u4Offset += 0x8;
			}
		}
	}

#if defined(SLT) || (FT_DSIM_USED)
    if (u4Shuffle_Index!=0)
    {
        u4Offset += u4RegShuffleOffset(u4RegType, u4Offset, u4Shuffle_Index);
    }
#endif

#if (fcFOR_CHIP_ID == fcTalbot)
    /*const U32 arRegBaseAddrList[8] =
    {
        Channel_A_DRAMC_NAO_BASE_ADDRESS,
        Channel_B_DRAMC_NAO_BASE_ADDRESS,
        Channel_A_DRAMC_AO_BASE_ADDRESS,
        Channel_B_DRAMC_AO_BASE_ADDRESS,
        Channel_A_PHY_NAO_BASE_ADDRESS,
        Channel_B_PHY_NAO_BASE_ADDRESS,
        Channel_A_PHY_AO_BASE_ADDRESS,
        Channel_B_PHY_AO_BASE_ADDRESS,
    };*/
    switch(u4RegType)
    {
        case 0:
             u4BaseAddr = Channel_A_DRAMC_NAO_BASE_ADDRESS;
             break;
         case 1:
             u4BaseAddr = Channel_B_DRAMC_NAO_BASE_ADDRESS;
             break;
         case 2:
             u4BaseAddr = Channel_A_DRAMC_AO_BASE_ADDRESS;
             break;
         case 3:
             u4BaseAddr = Channel_B_DRAMC_AO_BASE_ADDRESS;
             break;
         case 4:
               u4BaseAddr = Channel_A_PHY_NAO_BASE_ADDRESS;
               break;
         case 5:
               u4BaseAddr = Channel_B_PHY_NAO_BASE_ADDRESS;
               break;
         case 6:
               u4BaseAddr = Channel_A_PHY_AO_BASE_ADDRESS;
               break;
         case 7:
               u4BaseAddr = Channel_B_PHY_AO_BASE_ADDRESS;
               break;
    }

    return (u4BaseAddr +u4Offset);
#else
#error No defined arRegBaseAddrList for your chip !!!
#endif
	/*
	 * mcSHOW_DBG_MSG(("\n[u4RegBaseAddrTraslate]  0x%x => 0x%x(0x%x, 0x%x)", u4reg_addr , (arRegBaseAddrList[u4RegType] +u4Offset), arRegBaseAddrList[u4RegType], u4Offset));
	 * return (arRegBaseAddrList[u4RegType] +u4Offset);
	 */
}

/* ------------------------------------------------------------------------- */
/* * ucDram_Register_Read
 * DRAM register read (32-bit).
 * @param  u4reg_addr    register address in 32-bit.
 * @param  pu4reg_value  Pointer of register read value.
 * @retval 0: OK, 1: FAIL
 */
/*
 * -------------------------------------------------------------------------
 * This function need to be porting by BU requirement
 */
U32 u4Dram_Register_Read(DRAMC_CTX_T *p, U32 u4reg_addr)
{
	U32 u4reg_value;

#if defined(SLT) || (FT_DSIM_USED)
	u4reg_addr = u4RegBaseAddrTraslate(p->rank, u4reg_addr, p->shuffle_index);
#else
	u4reg_addr = u4RegBaseAddrTraslate(p->rank, u4reg_addr);
#endif

#if (FOR_DV_SIMULATION_USED == 1)   /* DV */
	u4reg_value = register_read_c(u4reg_addr);
#elif (SW_CHANGE_FOR_SIMULATION == 1) /* whole chip sim */
	u4reg_value = (*UINT32P(u4reg_addr));
#else /* real chip */
	u4reg_value = (*(volatile unsigned int *)(u4reg_addr));
#endif
	return u4reg_value;
}


/* ------------------------------------------------------------------------- */
/* * ucDram_Register_Write
 * DRAM register write (32-bit).
 * @param  u4reg_addr    register address in 32-bit.
 * @param  u4reg_value   register write value.
 * @retval 0: OK, 1: FAIL
 */
/* ------------------------------------------------------------------------- */
#if REG_SHUFFLE_REG_CHECK
extern U8 ShuffleRegCheck;
extern void ShuffleRegCheckProgram(U32 u4Addr);
#endif

#if REG_ACCESS_NAO_DGB
#if (fcFOR_CHIP_ID == fcTalbot)
U8 Check_RG_Not_AO(U32 u4reg_addr)
{
	U8 RegNotAO = 0;
	if ((u4reg_addr >= DRAMC_AO_BASE_ADDRESS) && (u4reg_addr <= DRAMC_REG_SHU4_DQSG_RETRY))
	{
	}
	else if ((u4reg_addr >= DRAMC_AO_BASE_ADDRESS + SHIFT_TO_CHB_ADDR) && (u4reg_addr <= DRAMC_REG_SHU4_DQSG_RETRY + SHIFT_TO_CHB_ADDR))
	{
	}
	else if ((u4reg_addr >= DDRPHY_AO_BASE_ADDRESS) && (u4reg_addr <= DDRPHY_RFU_0X1FCC))
	{
	}
	else if ((u4reg_addr >= DDRPHY_AO_BASE_ADDRESS + SHIFT_TO_CHB_ADDR) && (u4reg_addr <= DDRPHY_RFU_0X1FCC + SHIFT_TO_CHB_ADDR))
	{
	}
	else
	{
		RegNotAO = 1;
	}
	return RegNotAO;
}
#endif
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
void RG_RW_dump(U8 rw, U32 rg, U32 val)
{
	U8 u1ChannelIdx, u1SectionIdx, flag = 0;
	U32 base = Channel_A_PHY_AO_BASE_ADDRESS, rg_idx;

#if 0
	/*
	 * mcSHOW_DBG_MSG(("=== %s: addr: 0x%x = %x\n",rw==0?"Read ":"Write",rg,val));
	 * mcSHOW_DBG_MSG(("=== %s: addr: 0x%x = %x\n",rw==0?"R ":"W ",rg,val));
	 */
	mcSHOW_DBG_MSG6(("*((%s)(0x%X)) = 0x%X; \n", "UINT32P", rg, val));
#else
	/* mcSHOW_DBG_MSG6(("=== %s\n",__FUNCTION__)); */
	for (u1ChannelIdx = 0; u1ChannelIdx < CHANNEL_NUM; u1ChannelIdx++)
	{
		for (u1SectionIdx = 0; u1SectionIdx < 4; u1SectionIdx++)
		{
			base = Channel_A_PHY_AO_BASE_ADDRESS + (0x8000*u1ChannelIdx) + (0x2000*u1SectionIdx);
			rg_idx = rg - base;
			if (rg_idx < 0x2000)
			{
				flag = 1;
				/*
				 * #define Channel_A_DRAMC_NAO_BASE_ADDRESS    0x1022C000
				 * #define Channel_B_DRAMC_NAO_BASE_ADDRESS    0x10234000
				 * #define Channel_A_DRAMC_AO_BASE_ADDRESS     0x1022A000
				 * #define Channel_B_DRAMC_AO_BASE_ADDRESS     0x10232000
				 * #define Channel_A_PHY_NAO_BASE_ADDRESS      0x1022E000
				 * #define Channel_B_PHY_NAO_BASE_ADDRESS      0x10236000
				 * #define Channel_A_PHY_AO_BASE_ADDRESS       0x10228000
				 * #define Channel_B_PHY_AO_BASE_ADDRESS       0x10230000
				 *
				 * #define DDRPHY0AO__BASE      0x10228000
				 * #define DRAMC0_AO__BASE      0x1022A000
				 * #define DRAMC0_NAO_BASE      0x1022C000
				 * #define DDRPHY0NAO_BASE      0x1022E000
				 *
				 * #define DDRPHY1AO__BASE      0x10230000
				 * #define DRAMC1_AO__BASE      0x10232000
				 * #define DRAMC1_NAO_BASE      0x10234000
				 * #define DDRPHY1NAO_BASE      0x10236000
				 */

				/* mcSHOW_DBG_MSG6(("=== %s: 0x%x:\t*((%s*)(%s + 0x%X)) = 0x%X;\n",rw==0?"Read ":"Write", rg, "unsigned int", */

				mcSHOW_DUMP_INIT_RG_MSG(("*((%s)(%s + 0x%X)) = 0x%X; \n", "UINT32P",
					(base == Channel_A_PHY_AO_BASE_ADDRESS) ? "DDRPHY0AO_BASE":
					(base == Channel_A_DRAMC_AO_BASE_ADDRESS) ? "DRAMC0_AO_BASE":
					(base == Channel_A_DRAMC_NAO_BASE_ADDRESS) ? "DRAMC0_NAO_BASE":
					(base == Channel_A_PHY_NAO_BASE_ADDRESS) ? "DDRPHY0NAO_BASE":
					(base == Channel_B_PHY_AO_BASE_ADDRESS) ? "DDRPHY1AO_BASE":
					(base == Channel_B_DRAMC_AO_BASE_ADDRESS) ? "DRAMC1_AO_BASE":
					(base == Channel_B_DRAMC_NAO_BASE_ADDRESS) ? "DRAMC1_NAO_BASE":
					(base == Channel_B_PHY_NAO_BASE_ADDRESS) ? "DDRPHY1NAO_BASE":"unknown",
					rg_idx, val));

				/*
				 * mcSHOW_DBG_MSG(("=== 0x%x: *(U32*) ( %c, %s + 0x%x) = %s   0x%x\n", rg, 'A'+u1ChannelIdx,
				 * (u1SectionIdx==0)?"DDRPHY AO":(u1SectionIdx==1)?"DRAMC  AO":(u1SectionIdx==2)?"DRAMC  NAO":"DDRPHY NAO",rg_idx,rw==0?"R":"W",val));
				 */
				/*
				 * mcSHOW_DBG_MSG(("=== 0x%x: base: 0x%x, ch: %c, %s\t*(U32*)(xxx + 0x%x) =\t0x%x\n", rg, base, 'A'+u1ChannelIdx,
				 * (u1SectionIdx==0)?"DDRPHY AO":(u1SectionIdx==1)?"DRAMC  AO":(u1SectionIdx==2)?"DRAMC  NAO":"DDRPHY NAO",rg_idx,val));
				 * mcSHOW_DBG_MSG((" *(U32*)(xxx + 0x%x) = 0x%x\n",rg_idx,val));
				 */
				break;
			}
		}
		if (flag)
			break;
	}
	if (!flag)
	{
		/* mcSHOW_DBG_MSG6(("=== %s: 0x%x:\t*((%s*)(0x%X)) = 0x%X;\n",rw==0?"Read ":"Write", rg, "unsigned int", rg, val)); */
		mcSHOW_DUMP_INIT_RG_MSG(("*((%s)(0x%X)) = 0x%X;\n", "UINT32P", rg, val));
	}
#endif
}
#endif

/* This function need to be porting by BU requirement */
U8 ucDram_Register_Write(DRAMC_CTX_T *p, U32 u4reg_addr, U32 u4reg_value)
{
	U8 ucstatus;
	ucstatus = 0;  /* for SW_CHANGE_FOR_SIMULATION */
#if defined(SLT) || FT_DSIM_USED
#ifdef DUMP_INIT_RG_LOG_TO_DE
	U32 u4reg_addr_shu0 = u4reg_addr;
#endif
#endif
#if REG_ACCESS_NAO_DGB
	U8 RegNotAO = 0;
#endif

#if __ETT__
	if (u1IsLP4Family(p->dram_type))
	{
		CheckDramcWBR(u4reg_addr);
	}
#if REG_ACCESS_NAO_DGB
	RegNotAO = Check_RG_Not_AO(u4reg_addr);
#endif
#endif

#if defined(SLT) || FT_DSIM_USED
#ifdef DUMP_INIT_RG_LOG_TO_DE
	if (p->shuffle_index != 0) u4reg_addr_shu0 = u4RegBaseAddrTraslate(p->rank, u4reg_addr_shu0, 0);
#endif

	u4reg_addr = u4RegBaseAddrTraslate(p->rank, u4reg_addr, p->shuffle_index);
#else
	u4reg_addr = u4RegBaseAddrTraslate(p->rank, u4reg_addr);
#endif

#ifdef DUMP_INIT_RG_LOG_TO_DE
	if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag == 1)
	{
		/* mcSHOW_DUMP_INIT_RG_MSG(("*((UINT32P)(0x%x)) = 0x%x;\n",u4reg_addr,u4reg_value)); */
		RG_RW_dump(1, u4reg_addr, u4reg_value);
		mcDELAY_MS(1);
	}
#endif


#if (FOR_DV_SIMULATION_USED == 1) /* DV */
	register_write_c(u4reg_addr, u4reg_value);
#elif (SW_CHANGE_FOR_SIMULATION == 1) /* whole chip sim */
		(*(volatile unsigned int *)u4reg_addr) = u4reg_value;
#else
		(*(volatile unsigned int *)u4reg_addr) = u4reg_value; /* real chip */
	dsb();
#endif

#if defined(SLT) || FT_DSIM_USED
#ifdef DUMP_INIT_RG_LOG_TO_DE
		(*(volatile unsigned int *)u4reg_addr_shu0) = u4reg_value; /* real chip */
	dsb();
#endif
#endif

#if REG_ACCESS_PORTING_DGB
	if (RegLogEnable)
	{
		mcSHOW_DBG_MSG(("\n[REG_ACCESS_PORTING_DBG]   ucDramC_Register_Write Reg(0x%X) = 0x%X\n", u4reg_addr, u4reg_value));
		mcFPRINTF((fp_A60501, "\n[REG_ACCESS_PORTING_DBG]	ucDramC_Register_Write Reg(0x%X) = 0x%X\n", u4reg_addr, u4reg_value));
		/* mcFPRINTF((fp_A60501, "\nRISCWrite ('h%05X , 32'h%X);\n",u4reg_addr&0xFFFFF,	u4reg_value)); */
	}
#endif
#if REG_ACCESS_NAO_DGB
	if (RegNotAO)
	{
		mcSHOW_DBG_MSG(("\n[REG_ACCESS_NAO_DGB] ucDramC_Register_Write Reg(0x%X) = 0x%X\n", u4reg_addr, u4reg_value));
	}
#endif
#if REG_SHUFFLE_REG_CHECK
	if (ShuffleRegCheck)
	{
		ShuffleRegCheckProgram(u4reg_addr);
	}
#endif
	return ucstatus;
}

#if SW_CHANGE_FOR_SIMULATION
U32 u4IO32ReadFldAlign2(DRAMC_CTX_T *p, U32 reg32, U32 fld)
{
	if (Fld_ac(fld) == AC_FULLDW)
	{
		return u4IO32Read4B(reg32);
	}
	else
	{
		return ((u4IO32Read4B(reg32) & Fld2Msk32(fld)) >> Fld_shft(fld));
	}
}

void vIO32WriteFldAlign2(DRAMC_CTX_T *p, U32 reg32, U32 val, U32 fld)
{
	if (Fld_ac(fld) == AC_FULLDW)
	{
		vIO32Write4B((reg32), (val));
	}
	else
	{
		vIO32Write4BMsk((reg32), ((U32)(val) << Fld_shft(fld)), Fld2Msk32(fld));
	}
}

void vIO32WriteFldAlign_All2(DRAMC_CTX_T *p, U32 reg32, U32 val, U32 fld)
{
	if (Fld_ac(fld) == AC_FULLDW)
	{
		vIO32Write4B_All((reg32), (val));
	}
	else
	{
		vIO32Write4BMsk_All((reg32), ((U32)(val) << Fld_shft(fld)), Fld2Msk32(fld));
	}
}
#endif

void vIO32Write4BMsk2(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32)
{
	U32 u4Val;

	val32 &= msk32;

	u4Val = u4Dram_Register_Read(p, reg32);
	u4Val = ((u4Val & ~msk32) | val32);
	ucDram_Register_Write(p, reg32, u4Val);
}


void vIO32Write4B_All2(DRAMC_CTX_T *p, U32 reg32, U32 val32)
{
	U8 ii, u1AllCount;
	U32 u4RegType = (reg32 & (0xf << POS_BANK_NUM));

#if __ETT__
	if (GetDramcBroadcast() == DRAMC_BROADCAST_ON)
	{
		mcSHOW_ERR_MSG(("Error! virtual address 0x%x don't have to use write_all when Dramc WBR is on\n", reg32));
		while (1);
	}
#endif

	reg32 &= 0xffff;     /* remove channel information */

#if CHANNEL_NUM == 4
	u1AllCount = 4;
#else
	u1AllCount = 2;
#endif

	if (u4RegType >= Channel_A_PHY_AO_BASE_VIRTUAL)/* PHY */
	{
		reg32 += Channel_A_PHY_AO_BASE_VIRTUAL;
	}
	else if (u4RegType >= Channel_A_DRAMC_AO_BASE_VIRTUAL)/* DramC AO */
	{
		reg32 += Channel_A_DRAMC_AO_BASE_VIRTUAL;
	}
	else /* DramC NAO */
	{
		reg32 += Channel_A_DRAMC_NAO_BASE_VIRTUAL;
	}

	for (ii = 0; ii < u1AllCount; ii++)
	{
		vIO32Write4B(reg32 + ((U32)ii << POS_BANK_NUM), val32);
	}

#if 0
	/*
	 * debug
	 * mcSHOW_DBG_MSG(("RISCWrite : address %05x data %08x\n", reg32&0x000fffff, val32));
	 */
	mcFPRINTF((fp_A60501, "RISCWrite : address %05x data %08x wait  0\n", reg32 & 0x000fffff, val32));
	/* mcSHOW_DBG_MSG(("RISCWrite : address %05x data %08x\n", (reg32+((U32)1<<POS_BANK_NUM))&0x000fffff, val32)); */
	mcFPRINTF((fp_A60501, "RISCWrite : address %05x data %08x wait  0\n", (reg32 + ((U32)1 << POS_BANK_NUM)) & 0x000fffff, val32));
	/* mcSHOW_DBG_MSG(("RISCWrite : address %05x data %08x\n", (reg32+((U32)2<<POS_BANK_NUM))&0x000fffff, val32)); */
	mcFPRINTF((fp_A60501, "RISCWrite : address %05x data %08x wait  0\n", (reg32 + ((U32)2 << POS_BANK_NUM)) & 0x000fffff, val32));
#endif
}

void vIO32Write4BMsk_All2(DRAMC_CTX_T *p, U32 reg32, U32 val32, U32 msk32)
{
	U32 u4Val;
	U8 ii, u1AllCount;
	U32 u4RegType = (reg32 & (0xf << POS_BANK_NUM));

#if __ETT__
	if (GetDramcBroadcast() == DRAMC_BROADCAST_ON)
	{
		mcSHOW_ERR_MSG(("Error! virtual address 0x%x don't have to use write_all when Dramc WBR is on\n", reg32));
		while (1);
	}
#endif

	reg32 &= 0xffff;     /* remove channel information */

#if CHANNEL_NUM == 4
	u1AllCount = 4;
#else
	u1AllCount = 2;
#endif

	if (u4RegType >= Channel_A_PHY_AO_BASE_VIRTUAL)/* PHY */
	{
		reg32 += Channel_A_PHY_AO_BASE_VIRTUAL;
	}
	else if (u4RegType >= Channel_A_DRAMC_AO_BASE_VIRTUAL)/* DramC AO */
	{
		reg32 += Channel_A_DRAMC_AO_BASE_VIRTUAL;
	}
	else /* DramC NAO */
	{
		reg32 += Channel_A_DRAMC_NAO_BASE_VIRTUAL;
	}

	for (ii = 0; ii < u1AllCount; ii++)
	{
		u4Val = u4Dram_Register_Read(p, reg32 + ((U32)ii << POS_BANK_NUM));
		u4Val = ((u4Val & ~msk32) | val32);
		ucDram_Register_Write(p, reg32 + ((U32)ii << POS_BANK_NUM), u4Val);
	}
}

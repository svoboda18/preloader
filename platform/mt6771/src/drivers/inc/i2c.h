#ifndef __MT_I2C_H__
#define __MT_I2C_H__

#include "platform.h"
//#include <mt_reg_base.h>
#include "pal_log.h" /* For print */

#define I2C_NR		12 /* Number of I2C controllers */

#define DIS_CH0_PULL    0
#define DIS_CH1_PULL	0
#define DIS_CH2_PULL	0
#define DIS_CH3_PULL	0
#define DIS_CH4_PULL	0
#define DIS_CH5_PULL	0
#define DIS_CH6_PULL	0

/*override default*/
#ifdef MTK_I2C_CH0_PULL_DIS
#undef DIS_CH0_PULL
#define DIS_CH0_PULL 1
#endif

/*override default*/
#ifdef MTK_I2C_CH5_PULL_DIS
#undef DIS_CH5_PULL
#define DIS_CH5_PULL 1
#endif

#define I2CTAG                "[I2C] "
#if 0
#define I2CLOG(fmt, arg...)   dprintf(INFO,I2CTAG fmt, ##arg)
#define I2CMSG(fmt, arg...)   dprintf(SPEW,fmt, ##arg)
#define I2CERR(fmt, arg...)   dprintf(ALWAYS,I2CTAG "%d: "fmt, __LINE__, ##arg)
#define I2CFUC(fmt, arg...)   dprintf(I2CTAG "%s\n", __FUNCTION__)
#endif
#define I2CLOG(fmt, arg...)   pal_log_info(I2CTAG fmt, ##arg)
#define I2CMSG(fmt, arg...)   pal_log_info(fmt, ##arg)
#define I2CERR(fmt, arg...)   pal_log_err(I2CTAG "%d: "fmt, __LINE__, ##arg)
#define I2CFUC(fmt, arg...)   pal_log_info(I2CTAG "%s\n", __FUNCTION__)
#if (CFG_FPGA_PLATFORM)
  #define CONFIG_MT_I2C_FPGA_ENABLE
#endif

#ifdef CONFIG_MT_I2C_FPGA_ENABLE
#define FPGA_CLOCK		10000 /* FPGA crystal frequency (KHz) */
#define I2C_CLK_DIV		(2*5) /* frequency divisor */
#define I2C_CLK_RATE		(FPGA_CLOCK / I2C_CLK_DIV) /* I2C base clock (KHz) */
#define SCP_I2C_CLK		(FPGA_CLOCK / 16)
#else
#define I2C_CLK_RATE		124800 / I2C_CLK_DIV /* TODO: Calculate from bus clock */
#define I2C_CLK_DIV		(5) /* frequency divisor */
#define SCP_I2C_CLK		(26000 / 2) /* TODO: Check the correct frequency */
#endif

#define I2C_MB()
#define I2C_BUG_ON(a)
#define I2C_M_RD       0x0001

#ifndef I2C0_BASE
#define I2C0_BASE (0x11007000)
#endif
#ifndef I2C1_BASE
#define I2C1_BASE (0x11011000)
#endif
#ifndef I2C2_BASE
#define I2C2_BASE (0x11009000 + 0x100)
#endif
#ifndef I2C2_BASE_SE
#define I2C2_BASE_SE (0x11009000)
#endif
#ifndef I2C3_BASE
#define I2C3_BASE (0x1100F000)
#endif
#ifndef I2C4_BASE
#define I2C4_BASE (0x11008000 + 0x100)
#endif
#ifndef I2C4_BASE_SE
#define I2C4_BASE_SE (0x11008000)
#endif
#ifndef I2C5_BASE
#define I2C5_BASE (0x11017000)
#endif
#ifndef I2C6_BASE
#define I2C6_BASE (0x11005000)
#endif
#ifndef I2C7_BASE
#define I2C7_BASE (0x1101A000)
#endif
#ifndef I2C8_BASE
#define I2C8_BASE (0x1101B000)
#endif
#ifndef I2C9_BASE
#define I2C9_BASE (0x11014000)
#endif
#ifndef I2C10_BASE
#define I2C10_BASE (0x11015000)
#endif
#ifndef I2C11_BASE
#define I2C11_BASE (0x11016000)
#endif

#define I2C0_APDMA_BASE (0x11000080)
#define I2C1_APDMA_BASE (0x11000480)
#define I2C2_APDMA_BASE (0x11000280)
#define I2C3_APDMA_BASE (0x11000400)
#define I2C4_APDMA_BASE (0x11000100)
#define I2C5_APDMA_BASE (0x11000580)
#define I2C6_APDMA_BASE (0x11000600)
#define I2C7_APDMA_BASE (0x11000680)
#define I2C8_APDMA_BASE (0x11000700)
#define I2C9_APDMA_BASE (0x11000180)
#define I2C10_APDMA_BASE (0x11000300)
#define I2C11_APDMA_BASE (0x11000500)

#define MODULE_CLK_SEL_BASE (0x10001098)
#define USE_PLL_MUX         0x800

#define PERICFG_I2C_ARBITRATION	(PERICFG_BASE+0x428)
#define INFRA_MODULE_CLK_SEL	(INFRACFG_AO_BASE+0x98)
#define I2C_CLK_SEL_OFST	(11)

#define I2C_OK                              0x0000
#define EAGAIN_I2C                          11  /* Try again */
#define EINVAL_I2C                          22  /* Invalid argument */
#define EOPNOTSUPP_I2C                      95  /* Operation not supported on transport endpoint */
#define ETIMEDOUT_I2C                       110 /* Connection timed out */
#define EREMOTEIO_I2C                       121 /* Remote I/O error */
#define ENOTSUPP_I2C                        524 /* Remote I/O error */
#define I2C_WRITE_FAIL_HS_NACKERR           0xA013
#define I2C_WRITE_FAIL_ACKERR               0xA014
#define I2C_WRITE_FAIL_TIMEOUT              0xA015
#define DUTY_CYCLE			    45

/***For internal pull-up resister setting***/
#define IOCFG_RM_BASE			    0x11D20000
#define IOCFG_RB_BASE			    0x11D30000
#define IOCFG_LB_BASE			    0x11E70000
#define IOCFG_BL_BASE			    0x11E90000
#define IOCFG_LT_BASE			    0x11F20000

#define I2C_SET_REG32(reg, field, val) \
    do { \
        uint32 tv = DRV_Reg32(reg); \
        tv &= ~(field); \
        tv |= (val); \
        DRV_WriteReg32(reg, tv); \
    } while(0)

#define SHADOW_REG_MODE           (1 << 1)
/******************************************register operation***********************************/
/* offset is different in mt6771 */
enum I2C_REGS_OFFSET {
  OFFSET_DATA_PORT      = 0x0,
  OFFSET_SLAVE_ADDR     = 0x04,
  OFFSET_INTR_MASK      = 0x08,
  OFFSET_INTR_STAT      = 0x0c,
  OFFSET_CONTROL        = 0x10,
  OFFSET_TRANSFER_LEN   = 0x14,
  OFFSET_TRANSAC_LEN    = 0x18,
  OFFSET_DELAY_LEN      = 0x1c,
  OFFSET_HTIMING         = 0x20,
  OFFSET_START          = 0x24,
  OFFSET_EXT_CONF       = 0x28,
  OFFSET_LTIMING        = 0x2c,
  OFFSET_HS             = 0x30,
  OFFSET_IO_CONFIG      = 0x34,
  OFFSET_FIFO_ADDR_CLR  = 0x38,
  OFFSET_TRANSFER_LEN_AUX = 0x44,
  OFFSET_CLOCK_DIV      = 0x48,
  OFFSET_SOFTRESET      = 0x50,
  OFFSET_DEBUGSTAT      = 0xe4,
  OFFSET_DEBUGCTRL      = 0xe8,
  OFFSET_FIFO_STAT      = 0xf4,
  OFFSET_FIFO_THRESH    = 0xf8,
  OFFSET_DCM_EN         = 0xf88,
  OFFSET_MULTI_DMA      = 0xf8c,
  OFFSET_ROLLBACK       = 0xf98,

};

#define I2C_HS_NACKERR            (1 << 2)
#define I2C_ACKERR                (1 << 1)
#define I2C_TRANSAC_COMP          (1 << 0)

#define I2C_FIFO_SIZE             8

#define MAX_ST_MODE_SPEED         100  /* khz */
#define MAX_FS_MODE_SPEED         400  /* khz */
#define MAX_HS_MODE_SPEED         3400 /* khz */

#define MAX_DMA_TRANS_SIZE        65532 /* Max(65535) aligned to 4 bytes = 65532 */
#define MAX_DMA_TRANS_NUM         256

#define MAX_SAMPLE_CNT_DIV        8
#define MAX_STEP_CNT_DIV          64
#define MAX_HS_STEP_CNT_DIV       8

#define DMA_ADDRESS_HIGH          (0xC0000000)

enum DMA_REGS_OFFSET {
  OFFSET_INT_FLAG       = 0x0,
  OFFSET_INT_EN         = 0x04,
  OFFSET_EN             = 0x08,
  OFFSET_RST            = 0x0C,
  OFFSET_CON            = 0x18,
  OFFSET_TX_MEM_ADDR    = 0x1C,
  OFFSET_RX_MEM_ADDR    = 0x20,
  OFFSET_TX_LEN         = 0x24,
  OFFSET_RX_LEN         = 0x28,
};

enum i2c_trans_st_rs {
    I2C_TRANS_STOP = 0,
    I2C_TRANS_REPEATED_START,
};

typedef enum {
     ST_MODE,
     FS_MODE,
     HS_MODE,
 }I2C_SPEED_MODE;

enum mt_trans_op {
  I2C_MASTER_NONE = 0,
  I2C_MASTER_WR = 1,
  I2C_MASTER_RD,
  I2C_MASTER_WRRD,
};

//CONTROL
#define I2C_CONTROL_RS                    (0x1 << 1)
#define I2C_CONTROL_DMA_EN                (0x1 << 2)
#define I2C_CONTROL_CLK_EXT_EN            (0x1 << 3)
#define I2C_CONTROL_DIR_CHANGE            (0x1 << 4)
#define I2C_CONTROL_ACKERR_DET_EN         (0x1 << 5)
#define I2C_CONTROL_TRANSFER_LEN_CHANGE   (0x1 << 6)
#define I2C_CONTROL_WRAPPER               (0x1 << 0)
/***********************************end of register operation****************************************/
/***********************************I2C Param********************************************************/
struct mt_trans_data {
  U16 trans_num;
  U16 data_size;
  U16 trans_len;
  U16 trans_auxlen;
};

typedef struct mt_i2c_t {
  //==========set in i2c probe============//
  U32      base;    /* i2c base addr */
  U16      id;
  U16      irqnr;    /* i2c interrupt number */
  U16      irq_stat; /* i2c interrupt status */
  U32      clk;     /* host clock speed in khz */
  U32      pdn;     /*clock number*/
  //==========common data define============//
  enum     i2c_trans_st_rs st_rs;
  enum     mt_trans_op op;
  U32      pdmabase;
  U32      speed;   //The speed (khz)
  U16      delay_len;    //number of half pulse between transfers in a trasaction
  U32      msg_len;    //number of half pulse between transfers in a trasaction
  U8       *msg_buf;    /* pointer to msg data      */
  U8       addr;      //The address of the slave device, 7bit,the value include read/write bit.
  U8       master_code;/* master code in HS mode */
  U8       mode;    /* ST/FS/HS mode */
  //==========reserved funtion============//
  U8       is_push_pull_enable; //IO push-pull or open-drain
  U8       is_clk_ext_disable;   //clk entend default enable
  U8       is_dma_enabled;   //Transaction via DMA instead of 8-byte FIFO
  U8       read_flag;//read,write,read_write
  BOOL     dma_en;
  BOOL     poll_en;
  BOOL     pushpull;//open drain
  BOOL     filter_msg;//filter msg error log
  BOOL     i2c_3dcamera_flag;//flag for 3dcamera

  //==========define reg============//
  U16      htiming_reg;
  U16      ltiming_reg;
  U16      high_speed_reg;
  U16      control_reg;
  struct   mt_trans_data trans_data;
}mt_i2c;

//===========================i2c old driver===================================================//
#define I2C_APPM I2C5
enum {
	I2C0 = 0,
	I2C1 = 1,
	I2C2 = 2,
	I2C3 = 3,
	I2C4 = 4,
	I2C5 = 5,
	I2C6 = 6,
	I2C7 = 7,
	I2C8 = 8,
	I2C9 = 9,
	I2C10 = 10,
	I2C11 = 11,
};

//==============================================================================
// I2C Exported Function
//==============================================================================
extern S32 i2c_read(mt_i2c *i2c,U8 *buffer, U32 len);
extern S32 i2c_write(mt_i2c *i2c,U8  *buffer, U32 len);
extern S32 i2c_write_read(mt_i2c *i2c,U8 *buffer, U32 write_len, U32 read_len);
extern S32 i2c_set_speed(mt_i2c *i2c);

//#define I2C_EARLY_PORTING
#ifdef I2C_EARLY_PORTING
int mt_i2c_test(void);
#endif
#endif /* __MT_I2C_H__ */

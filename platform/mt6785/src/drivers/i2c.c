#include <i2c.h>
#include <gpio.h>
#include <sync_write.h>
#include <pmic.h>

/******************************internal API********************************************************/
#define I2C_PMIC_WR(addr, data)   pwrap_write((U32)addr, data)
#define I2C_PMIC_RD(addr)         ({ \
    U32 ext_data; \
    (pwrap_read((U32)addr,&ext_data) != 0)?-1:ext_data;})

static inline void i2c_writel(mt_i2c * i2c, U16 offset, U16 value)
{
  //__raw_writew(value, (i2c->base) + (offset));
  mt_reg_sync_writel(value, (i2c->base) + (offset));
}

static inline U32 i2c_readl(mt_i2c *i2c, U16 offset)
{
  return DRV_Reg32((i2c->base) + (offset));
}
/***********************************declare  API**************************/
static void mt_i2c_clock_enable(mt_i2c *i2c);
static void mt_i2c_clock_disable(mt_i2c *i2c);

/***********************************i2c debug********************************************************/
#define I2C_DEBUG_FS
#ifdef I2C_DEBUG_FS
  #define PORT_COUNT 8
  #define MESSAGE_COUNT 16
  #define I2C_T_DMA 1
  #define I2C_T_TRANSFERFLOW 2
  #define I2C_T_SPEED 3
  /*7 ports,16 types of message*/
  U8 i2c_port[ PORT_COUNT ][ MESSAGE_COUNT ];

  #define I2CINFO( type, format, arg...) do { \
    if ( type < MESSAGE_COUNT && type >= 0 ) { \
      if ( i2c_port[i2c->id][0] != 0 && ( i2c_port[i2c->id][type] != 0 || i2c_port[i2c->id][MESSAGE_COUNT - 1] != 0) ) { \
        I2CLOG( format, ## arg); \
      } \
    } \
  } while (0)
#else
  #define I2CINFO(type, format, arg...)
#endif
/***********************************common API********************************************************/
/* calculate i2c port speed */
static S32 mtk_i2c_calculate_speed(mt_i2c *i2c,
	U32 clk_src_in_khz,
	U32 speed_hkz,
	U32 *timing_step_cnt,
	U32 *timing_sample_cnt)
{
	U32 khz;
	U32 step_cnt;
	U32 sample_cnt;
	U32 sclk;
	U32 hclk;
	U32 max_step_cnt;
	U32 sample_div = MAX_SAMPLE_CNT_DIV;
	U32 step_div;
	U32 min_div;
	U32 best_mul;
	U32 cnt_mul;

	if (speed_hkz > MAX_HS_MODE_SPEED) {
			return -EINVAL_I2C;
	} else if (speed_hkz > MAX_FS_MODE_SPEED) {
		max_step_cnt = MAX_HS_STEP_CNT_DIV;
	} else {
		max_step_cnt = MAX_STEP_CNT_DIV;
	}
	step_div = max_step_cnt;

	/* Find the best combination */
	khz = speed_hkz;
	hclk = clk_src_in_khz;
	min_div = ((hclk >> 1) + khz - 1) / khz;
	best_mul = MAX_SAMPLE_CNT_DIV * max_step_cnt;
	for (sample_cnt = 1; sample_cnt <= MAX_SAMPLE_CNT_DIV; sample_cnt++) {
		step_cnt = (min_div + sample_cnt - 1) / sample_cnt;
		cnt_mul = step_cnt * sample_cnt;
		if (step_cnt > max_step_cnt)
			continue;
		if (cnt_mul < best_mul) {
			best_mul = cnt_mul;
			sample_div = sample_cnt;
			step_div = step_cnt;
			if (best_mul == min_div)
				break;
		}
	}
	sample_cnt = sample_div;
	step_cnt = step_div;
	sclk = hclk / (2 * sample_cnt * step_cnt);
	if (sclk > khz) {
		I2CERR("i2c%d %s mode: unsupported speed (%dkhz)\n",
			i2c->id, (speed_hkz > MAX_FS_MODE_SPEED) ?  "HS" : "ST/FT", khz);
		return -EINVAL_I2C;
	}

	/* Just for MT6761/MT6762/MT6765/MT6785 */
	/* which the step_cnt needn't minus 1 when sample_cnt==1 */
	if (--sample_cnt)
		step_cnt--;

	*timing_step_cnt = step_cnt;
	*timing_sample_cnt = sample_cnt;

	return 0;
}

/*Set i2c port speed*/
S32 i2c_set_speed(mt_i2c *i2c)
{
	int ret = 0;
	U32 step_cnt = 0;
	U32 sample_cnt = 0;
	U32 l_step_cnt = 0;
	U32 l_sample_cnt = 0;
	static U32 speedInHz;
	U32 clk_src_in_khz = i2c->clk;
	U32 duty = 50;
	U32 high_speed_reg;
	static S32 mode;
	U32 timing_reg;
	U32 ltiming_reg;

	if (clk_src_in_khz == 0) {
		I2CERR(" zero i2c source clock.\n");
		ret = -EINVAL_I2C;
		goto end;
	}

	mode = i2c->mode;
	speedInHz = i2c->speed;

	if ((mode == FS_MODE && speedInHz > MAX_FS_MODE_SPEED) ||
	    (mode == HS_MODE && speedInHz > MAX_HS_MODE_SPEED)) {
		I2CERR(" the speed is too fast for this mode.\n");
		I2C_BUG_ON((mode == FS_MODE && khz > MAX_FS_MODE_SPEED) ||
			   (mode == HS_MODE && khz > MAX_HS_MODE_SPEED));
		ret = -EINVAL_I2C;
		goto end;
	}

	if (speedInHz > MAX_FS_MODE_SPEED && !i2c->pushpull) {
		/* Set the high speed mode register */
		ret = mtk_i2c_calculate_speed(i2c, clk_src_in_khz,
			MAX_FS_MODE_SPEED, &l_step_cnt, &l_sample_cnt);
		if (ret < 0)
			return ret;

		ret = mtk_i2c_calculate_speed(i2c, clk_src_in_khz,
			speedInHz, &step_cnt, &sample_cnt);
		if (ret < 0)
			return ret;

		high_speed_reg = 0x3 |
			(sample_cnt & 0x7) << 12 |
			(step_cnt & 0x7) << 8;

		timing_reg =
			(l_sample_cnt & 0x7) << 8 |
			(l_step_cnt & 0x3f) << 0;


		ltiming_reg = (l_sample_cnt << 6) | (l_step_cnt << 0) |
			      (sample_cnt & 0x7) << 12 |
			      (step_cnt & 0x7) << 9;
	} else {
		if (speedInHz > MAX_ST_MODE_SPEED && speedInHz <= MAX_FS_MODE_SPEED)
			duty = DUTY_CYCLE;
		ret = mtk_i2c_calculate_speed(i2c, clk_src_in_khz,
			(speedInHz * 50 / duty), &step_cnt, &sample_cnt);
		if (ret < 0)
			return ret;

		ret = mtk_i2c_calculate_speed(i2c, clk_src_in_khz,
			(speedInHz * 50 / (100 - duty)), &l_step_cnt, &l_sample_cnt);
		if (ret < 0)
			return ret;

		timing_reg =
			(sample_cnt & 0x7) << 8 |
			(step_cnt & 0x3f) << 0;

		ltiming_reg = (l_sample_cnt & 0x7) << 6 |
			      (l_step_cnt & 0x3f) << 0;
		/* Disable the high speed transaction */
		high_speed_reg = 0x0;
	}

	i2c->htiming_reg = timing_reg;
	i2c->ltiming_reg = ltiming_reg;
	i2c->high_speed_reg = high_speed_reg;
end:
	return ret;
}

void _i2c_dump_info(mt_i2c *i2c)
{
  //I2CFUC();
  I2CERR("I2C structure:\n"
    I2CTAG"Clk=%d,Id=%d,Mode=%x,St_rs=%x,Dma_en=%x,Op=%x,Poll_en=%x,Irq_stat=%x\n"
    I2CTAG"Trans_len=%x,Trans_num=%x,Trans_auxlen=%x,Data_size=%x,speed=%d\n",
    //,Trans_stop=%u,Trans_comp=%u,Trans_error=%u\n"
    i2c->clk,i2c->id,i2c->mode,i2c->st_rs,i2c->dma_en,i2c->op,i2c->poll_en,i2c->irq_stat,
    i2c->trans_data.trans_len,i2c->trans_data.trans_num,i2c->trans_data.trans_auxlen,i2c->trans_data.data_size,i2c->speed);
    // atomic_read(&i2c->trans_stop),atomic_read(&i2c->trans_comp),atomic_read(&i2c->trans_err),

  I2CERR("base address 0x%x\n",i2c->base);
  I2CERR("I2C register:\n"
    I2CTAG"SLAVE_ADDR=%x,INTR_MASK=%x,INTR_STAT=%x,CONTROL=%x,TRANSFER_LEN=%x\n"
    I2CTAG"TRANSAC_LEN=%x,DELAY_LEN=%x,HTIMING=%x,LTIMING=%x,START=%x,FIFO_STAT=%x\n"
    I2CTAG"IO_CONFIG=%x,HS=%x,DCM_EN=%x,DEBUGSTAT=%x,EXT_CONF=%x,TRANSFER_LEN_AUX=%x\n",
    (i2c_readl(i2c, OFFSET_SLAVE_ADDR)),
    (i2c_readl(i2c, OFFSET_INTR_MASK)),
    (i2c_readl(i2c, OFFSET_INTR_STAT)),
    (i2c_readl(i2c, OFFSET_CONTROL)),
    (i2c_readl(i2c, OFFSET_TRANSFER_LEN)),
    (i2c_readl(i2c, OFFSET_TRANSAC_LEN)),
    (i2c_readl(i2c, OFFSET_DELAY_LEN)),
    (i2c_readl(i2c, OFFSET_HTIMING)),
    (i2c_readl(i2c, OFFSET_LTIMING)),
    (i2c_readl(i2c, OFFSET_START)),
    (i2c_readl(i2c, OFFSET_FIFO_STAT)),
    (i2c_readl(i2c, OFFSET_IO_CONFIG)),
    (i2c_readl(i2c, OFFSET_HS)),
    (i2c_readl(i2c, OFFSET_DCM_EN)),
    (i2c_readl(i2c, OFFSET_DEBUGSTAT)),
    (i2c_readl(i2c, OFFSET_EXT_CONF)),
    (i2c_readl(i2c, OFFSET_TRANSFER_LEN_AUX)));
  /*
  I2CERR("DMA register:\nINT_FLAG %x\nCON %x\nTX_MEM_ADDR %x\nRX_MEM_ADDR %x\nTX_LEN %x\nRX_LEN %x\nINT_EN %x\nEN %x\n",
      (__raw_readl(i2c->pdmabase+OFFSET_INT_FLAG)),
      (__raw_readl(i2c->pdmabase+OFFSET_CON)),
      (__raw_readl(i2c->pdmabase+OFFSET_TX_MEM_ADDR)),
      (__raw_readl(i2c->pdmabase+OFFSET_RX_MEM_ADDR)),
      (__raw_readl(i2c->pdmabase+OFFSET_TX_LEN)),
      (__raw_readl(i2c->pdmabase+OFFSET_RX_LEN)),
      (__raw_readl(i2c->pdmabase+OFFSET_S32_EN)),
      (__raw_readl(i2c->pdmabase+OFFSET_EN)));
  */
  /*6589 side and PMIC side clock*/
  //I2CERR("Clock %s\n",(((DRV_Reg32(0xF0003018)>>26) | (DRV_Reg32(0xF000301c)&0x1 << 6)) & (1 << i2c->id))?"disable":"enable");
  //if(i2c->id >=4)
  //  I2CERR("Clock PMIC %s\n",((I2C_PMIC_RD(0x011A) & 0x7) & (1 << (i2c->id - 4)))?"disable":"enable");
      //1<<(i2c->id-4): 0x011A bit[0~2]:i2c0~2,i2c->id:i2c 4~6
  return;
}
static S32 _i2c_deal_result(mt_i2c *i2c)
{
  long tmo = 1;
  U16 data_size = 0;
  U8 *ptr = i2c->msg_buf;
  BOOL TRANSFER_ERROR = FALSE;
  S32 ret = i2c->msg_len;
  long tmo_poll = 0xffff;
  //I2CFUC();
  //addr_reg = i2c->read_flag ? ((i2c->addr << 1) | 0x1) : ((i2c->addr << 1) & ~0x1);

  if(i2c->poll_en)
  { /*master read && poll mode*/
    for (;;)
    { /*check the interrupt status register*/
      i2c->irq_stat = i2c_readl(i2c, OFFSET_INTR_STAT);
      //I2CLOG("irq_stat = 0x%x\n", i2c->irq_stat);
      if(i2c->irq_stat & (I2C_HS_NACKERR | I2C_ACKERR ))
      {
        //transfer error
        //atomic_set(&i2c->trans_stop, 1);
        //spin_lock(&i2c->lock);
        /*Clear interrupt status,write 1 clear*/
        //i2c_writel(i2c, OFFSET_INTR_STAT, (I2C_HS_NACKERR | I2C_ACKERR ));
        TRANSFER_ERROR=TRUE;
        tmo = 1;
        //spin_unlock(&i2c->lock);
        break;
      }else if(i2c->irq_stat &  I2C_TRANSAC_COMP)
      {
        //transfer complete
        tmo = 1;
        break;
      }
      tmo_poll --;
      if(tmo_poll == 0) {
        tmo = 0;
        break;
      }
    }
  } else { /*Interrupt mode,wait for interrupt wake up*/
    //tmo = wait_event_timeout(i2c->wait,atomic_read(&i2c->trans_stop), tmo);
  }

  /*Save status register status to i2c struct*/
  //I2CLOG("tmo = 0x%x\n", tmo);
  /*Check the transfer status*/
  if (!(tmo == 0 )&& TRANSFER_ERROR==FALSE )
  {
    /*Transfer success ,we need to get data from fifo*/
    if((!i2c->dma_en) && (i2c->op == I2C_MASTER_RD || i2c->op == I2C_MASTER_WRRD) )
    { /*only read mode or write_read mode and fifo mode need to get data*/
      if(I2C_MASTER_WRRD != i2c->op)
        data_size = (i2c->msg_len) & 0xFFFF;
      else
        data_size = (i2c->msg_len >> 16) & 0xFFFF;

      //I2CLOG("data_size=%d\n",data_size);
      while (data_size--)
      {
        *ptr = i2c_readl(i2c, OFFSET_DATA_PORT);
        #ifdef I2C_EARLY_PORTING
          I2CLOG("addr %x read byte = 0x%x\n", i2c->addr, *ptr);
        #endif
        ptr++;
      }
    }
  }else
  {
    /*Timeout or ACKERR*/
    if ( tmo == 0 ){
      I2CERR("id=%d,addr: %x, transfer timeout\n",i2c->id, i2c->addr);
      ret = -ETIMEDOUT_I2C;
    } else
    {
      I2CERR("id=%d,addr: %x, transfer error\n",i2c->id,i2c->addr);
      ret = -EREMOTEIO_I2C;
    }
    if (i2c->irq_stat & I2C_HS_NACKERR)
      I2CERR("I2C_HS_NACKERR\n");
    if (i2c->irq_stat & I2C_ACKERR)
      I2CERR("I2C_ACKERR\n");
    if (i2c->filter_msg==FALSE) //TEST
    {
      _i2c_dump_info(i2c);
    }

    //spin_lock(&i2c->lock);
    /*Reset i2c port*/
    i2c_writel(i2c, OFFSET_SOFTRESET, 0x0001);
    /*Set slave address*/
    i2c_writel( i2c, OFFSET_SLAVE_ADDR, 0x0000 );
    /*Clear interrupt status*/
    i2c_writel(i2c, OFFSET_INTR_STAT, (I2C_HS_NACKERR|I2C_ACKERR|I2C_TRANSAC_COMP));
    /*Clear fifo address*/
    i2c_writel(i2c, OFFSET_FIFO_ADDR_CLR, 0x0005);

    //spin_unlock(&i2c->lock);
  }
  return ret;
}


void _i2c_write_reg(mt_i2c *i2c)
{
  U8 *ptr = i2c->msg_buf;
  U32 data_size=i2c->trans_data.data_size;
  U32 addr_reg=0;
  //I2CFUC();

  //i2c_writel(i2c, OFFSET_CONTROL, i2c->control_reg);

  i2c_writel(i2c, OFFSET_CLOCK_DIV, ((I2C_CLK_DIV - 1) << 8) + (I2C_CLK_DIV - 1));

  /*set start condition */
  if(i2c->speed <= 100){
    i2c_writel(i2c,OFFSET_EXT_CONF, 0x8001);
  } else {
    i2c_writel(i2c,OFFSET_EXT_CONF, 0x1801);
  }
  //set timing reg
  i2c_writel(i2c, OFFSET_HTIMING, i2c->htiming_reg);
  i2c_writel(i2c, OFFSET_LTIMING, i2c->ltiming_reg);
  i2c_writel(i2c, OFFSET_HS, i2c->high_speed_reg);

  if(0 == i2c->delay_len)
    i2c->delay_len = 2;
  if(~i2c->control_reg & I2C_CONTROL_RS){  // bit is set to 1, i.e.,use repeated stop
    i2c_writel(i2c, OFFSET_DELAY_LEN, i2c->delay_len);
  }

  /*Set ioconfig*/
  if (i2c->pushpull) {/* pushpull mode disabled clk extension */
	  i2c->control_reg &= (~I2C_CONTROL_CLK_EXT_EN);
	  i2c_writel(i2c, OFFSET_CONTROL, i2c->control_reg);
	  i2c_writel(i2c, OFFSET_IO_CONFIG, 0x0000);
  } else {
    i2c_writel(i2c, OFFSET_IO_CONFIG, 0x0003);
    i2c_writel(i2c, OFFSET_CONTROL, i2c->control_reg);
  }

  /*Set slave address*/

  addr_reg = i2c->read_flag ? ((i2c->addr << 1) | 0x1) : ((i2c->addr << 1) & ~0x1);
  i2c_writel(i2c, OFFSET_SLAVE_ADDR, addr_reg);
  /*Clear interrupt status*/
  i2c_writel(i2c, OFFSET_INTR_STAT, (I2C_HS_NACKERR | I2C_ACKERR | I2C_TRANSAC_COMP));
  /*Clear fifo address*/
  i2c_writel(i2c, OFFSET_FIFO_ADDR_CLR, 0x0005);
  /*Setup the interrupt mask flag*/
  if(i2c->poll_en)
    i2c_writel(i2c, OFFSET_INTR_MASK, i2c_readl(i2c, OFFSET_INTR_MASK) & ~(I2C_HS_NACKERR | I2C_ACKERR | I2C_TRANSAC_COMP)); /*Disable interrupt*/
  else
    i2c_writel(i2c, OFFSET_INTR_MASK, i2c_readl(i2c, OFFSET_INTR_MASK) | (I2C_HS_NACKERR | I2C_ACKERR | I2C_TRANSAC_COMP)); /*Enable interrupt*/
  /*Set transfer len */
  i2c_writel(i2c, OFFSET_TRANSFER_LEN, i2c->trans_data.trans_len & 0xFFFF);
  i2c_writel(i2c, OFFSET_TRANSFER_LEN_AUX, i2c->trans_data.trans_auxlen & 0xFFFF);
  /*Set transaction len*/
  i2c_writel(i2c, OFFSET_TRANSAC_LEN, i2c->trans_data.trans_num & 0xFF);

  /*Prepare buffer data to start transfer*/

  if(i2c->dma_en)
  {
    /* Reset I2C DMA status */
    mt_reg_sync_writel(0x0001, i2c->pdmabase + OFFSET_RST);
    if (I2C_MASTER_RD == i2c->op) {
      mt_reg_sync_writel(0x0000, i2c->pdmabase + OFFSET_INT_FLAG);
      mt_reg_sync_writel(0x0001, i2c->pdmabase + OFFSET_CON);
      mt_reg_sync_writel((U32)i2c->msg_buf, i2c->pdmabase + OFFSET_RX_MEM_ADDR);
      mt_reg_sync_writel(i2c->trans_data.data_size, i2c->pdmabase + OFFSET_RX_LEN);
    } else if (I2C_MASTER_WR == i2c->op) {
      mt_reg_sync_writel(0x0000, i2c->pdmabase + OFFSET_INT_FLAG);
      mt_reg_sync_writel(0x0000, i2c->pdmabase + OFFSET_CON);
      mt_reg_sync_writel((U32)i2c->msg_buf, i2c->pdmabase + OFFSET_TX_MEM_ADDR);
      mt_reg_sync_writel(i2c->trans_data.data_size, i2c->pdmabase + OFFSET_TX_LEN);
    } else {
      mt_reg_sync_writel(0x0000, i2c->pdmabase + OFFSET_INT_FLAG);
      mt_reg_sync_writel(0x0000, i2c->pdmabase + OFFSET_CON);
      mt_reg_sync_writel((U32)i2c->msg_buf, i2c->pdmabase + OFFSET_TX_MEM_ADDR);
      mt_reg_sync_writel((U32)i2c->msg_buf, i2c->pdmabase + OFFSET_RX_MEM_ADDR);
      mt_reg_sync_writel(i2c->trans_data.trans_len, i2c->pdmabase + OFFSET_TX_LEN);
      mt_reg_sync_writel(i2c->trans_data.trans_auxlen, i2c->pdmabase + OFFSET_RX_LEN);
    }
    I2C_MB();
    mt_reg_sync_writel(0x0001, i2c->pdmabase + OFFSET_EN);

    I2CINFO( I2C_T_DMA, "addr %.2x dma %.2X byte\n", i2c->addr, i2c->trans_data.data_size);
    I2CINFO( I2C_T_DMA, "DMA Register:INT_FLAG:0x%x,CON:0x%x,TX_MEM_ADDR:0x%x, \
                 RX_MEM_ADDR:0x%x,TX_LEN:0x%x,RX_LEN:0x%x,EN:0x%x\n",\
                  DRV_Reg32(i2c->pdmabase + OFFSET_INT_FLAG),\
                  DRV_Reg32(i2c->pdmabase + OFFSET_CON),\
                  DRV_Reg32(i2c->pdmabase + OFFSET_TX_MEM_ADDR),\
                  DRV_Reg32(i2c->pdmabase + OFFSET_RX_MEM_ADDR),\
                  DRV_Reg32(i2c->pdmabase + OFFSET_TX_LEN),\
                  DRV_Reg32(i2c->pdmabase + OFFSET_RX_LEN),\
                  DRV_Reg32(i2c->pdmabase + OFFSET_EN));

  }
  else
  {
    /*Set fifo mode data*/
    if (I2C_MASTER_RD == i2c->op)
    {
      /*do not need set fifo data*/
    }else
    { /*both write && write_read mode*/
      while (data_size--)
      {
        i2c_writel(i2c, OFFSET_DATA_PORT, *ptr);
        //dev_info(i2c->dev, "addr %.2x write byte = 0x%.2X\n", addr, *ptr);
        ptr++;
      }
    }
  }
  /*Set trans_data*/
  i2c->trans_data.data_size = data_size;

}
S32 _i2c_get_transfer_len(mt_i2c *i2c)
{
  S32 ret = I2C_OK;
  u16 trans_num = 0;
  u16 data_size = 0;
  u16 trans_len = 0;
  u16 trans_auxlen = 0;
  //I2CFUC();
  /*Get Transfer len and transaux len*/
  if(FALSE == i2c->dma_en)
  { /*non-DMA mode*/
    if(I2C_MASTER_WRRD != i2c->op)
    {
      trans_len = (i2c->msg_len) & 0xFFFF;
      trans_num = (i2c->msg_len >> 16) & 0xFF;
      if(0 == trans_num)
        trans_num = 1;
      trans_auxlen = 0;
      data_size = trans_len*trans_num;

      if(!trans_len || !trans_num || trans_len*trans_num > I2C_FIFO_SIZE)
      {
        I2CERR(" non-WRRD transfer length is not right. trans_len=%x, tans_num=%x, trans_auxlen=%x\n", trans_len, trans_num, trans_auxlen);
        I2C_BUG_ON(!trans_len || !trans_num || trans_len*trans_num > I2C_FIFO_SIZE);
        ret = -EINVAL_I2C;
      }
    } else
    {
      trans_len = (i2c->msg_len) & 0xFFFF;
      trans_auxlen = (i2c->msg_len >> 16) & 0xFFFF;
      trans_num = 2;
      data_size = trans_len;
      if(!trans_len || !trans_auxlen || trans_len > I2C_FIFO_SIZE || trans_auxlen > I2C_FIFO_SIZE)
      {
        I2CERR(" WRRD transfer length is not right. trans_len=%x, tans_num=%x, trans_auxlen=%x\n", trans_len, trans_num, trans_auxlen);
        I2C_BUG_ON(!trans_len || !trans_auxlen || trans_len > I2C_FIFO_SIZE || trans_auxlen > I2C_FIFO_SIZE);
        ret = -EINVAL_I2C;
      }
    }
  }
  else
  { /*DMA mode*/
    if(I2C_MASTER_WRRD != i2c->op)
    {
      trans_len = (i2c->msg_len) & 0xFFFF;
      trans_num = (i2c->msg_len >> 16) & 0xFF;
      if(0 == trans_num)
        trans_num = 1;
      trans_auxlen = 0;
      data_size = trans_len*trans_num;

      if(!trans_len || !trans_num || trans_len > MAX_DMA_TRANS_SIZE || trans_num > MAX_DMA_TRANS_NUM)
      {
        I2CERR(" DMA non-WRRD transfer length is not right. trans_len=%x, tans_num=%x, trans_auxlen=%x\n", trans_len, trans_num, trans_auxlen);
        I2C_BUG_ON(!trans_len || !trans_num || trans_len > MAX_DMA_TRANS_SIZE || trans_num > MAX_DMA_TRANS_NUM);
        ret = -EINVAL_I2C;
      }
      I2CINFO(I2C_T_DMA, "DMA non-WRRD mode!trans_len=%x, tans_num=%x, trans_auxlen=%x\n",trans_len, trans_num, trans_auxlen);
    } else
    {
      trans_len = (i2c->msg_len) & 0xFFFF;
      trans_auxlen = (i2c->msg_len >> 16) & 0xFFFF;
      trans_num = 2;
      data_size = trans_len;
      if(!trans_len || !trans_auxlen || trans_len > MAX_DMA_TRANS_SIZE || trans_auxlen > MAX_DMA_TRANS_NUM)
      {
        I2CERR(" DMA WRRD transfer length is not right. trans_len=%x, tans_num=%x, trans_auxlen=%x\n", trans_len, trans_num, trans_auxlen);
        I2C_BUG_ON(!trans_len || !trans_auxlen || trans_len > MAX_DMA_TRANS_SIZE || trans_auxlen > MAX_DMA_TRANS_NUM);
        ret = -EINVAL_I2C;
      }
      I2CINFO(I2C_T_DMA, "DMA WRRD mode!trans_len=%x, tans_num=%x, trans_auxlen=%x\n",trans_len, trans_num, trans_auxlen);
    }
  }

  i2c->trans_data.trans_num = trans_num;
  i2c->trans_data.trans_len = trans_len;
  i2c->trans_data.data_size = data_size;
  i2c->trans_data.trans_auxlen = trans_auxlen;

  return ret;
}
S32 _i2c_transfer_interface(mt_i2c *i2c)
{
  S32 return_value=0;
  S32 ret=0;
  U8 *ptr = i2c->msg_buf;
  //I2CFUC();

  if(i2c->dma_en)
  {
    I2CINFO( I2C_T_DMA, "DMA Transfer mode!\n");
    if (i2c->pdmabase == 0) {
      I2CERR(" I2C%d doesnot support DMA mode!\n",i2c->id);
      I2C_BUG_ON(i2c->pdmabase == NULL);
      ret = -EINVAL_I2C;
      goto err;
    }
    if((U32)ptr > DMA_ADDRESS_HIGH){
      I2CERR(" DMA mode should use physical buffer address!\n");
      I2C_BUG_ON((U32)ptr > DMA_ADDRESS_HIGH);
      ret = -EINVAL_I2C;
      goto err;
    }
  }

  i2c->irq_stat = 0;

  return_value=_i2c_get_transfer_len(i2c);
  if ( return_value < 0 ){
    I2CERR("_i2c_get_transfer_len fail,return_value=%d\n",return_value);
    ret =-EINVAL_I2C;
    goto err;
  }
  //get clock
#if CFG_FPGA_PLATFORM
    i2c->clk = I2C_CLK_RATE;
#else
    i2c->clk = I2C_CLK_RATE;
    /* i2c->clk = mt_get_bus_freq() / I2C_CLK_DIV; */
#endif

  return_value=i2c_set_speed(i2c);
  if ( return_value < 0 ){
    I2CERR("i2c_set_speed fail,return_value=%d\n",return_value);
    ret =-EINVAL_I2C;
    goto err;
  }
  /*Set Control Register*/
#if CFG_FPGA_PLATFORM
  i2c->control_reg = I2C_CONTROL_ACKERR_DET_EN;
#else 
  i2c->control_reg = I2C_CONTROL_ACKERR_DET_EN | I2C_CONTROL_CLK_EXT_EN;
#endif
  if(i2c->dma_en) {
    i2c->control_reg |= I2C_CONTROL_DMA_EN;
  }
  if(I2C_MASTER_WRRD == i2c->op)
    i2c->control_reg |= I2C_CONTROL_DIR_CHANGE;

  if(HS_MODE == i2c->mode || (i2c->trans_data.trans_num > 1 && I2C_TRANS_REPEATED_START == i2c->st_rs)) {
    i2c->control_reg |= I2C_CONTROL_RS;
  }

  _i2c_write_reg(i2c);

  /*All register must be prepared before setting the start bit [SMP]*/
  I2C_MB();

  I2CINFO( I2C_T_TRANSFERFLOW, "Before start .....\n");
  /*Start the transfer*/
  i2c_writel(i2c, OFFSET_START, 0x0001);
  ret = _i2c_deal_result(i2c);
  I2CINFO(I2C_T_TRANSFERFLOW, "After i2c transfer .....\n");
err:
end:
    return ret;
}
S32 _i2c_check_para(mt_i2c *i2c)
{
  S32 ret=0;
  //I2CFUC();
  if(i2c->addr == 0){
    I2CERR(" addr is invalid.\n");
    I2C_BUG_ON(i2c->addr == NULL);
    ret = -EINVAL_I2C;
    goto err;
  }

  if(i2c->msg_buf == NULL){
    I2CERR(" data buffer is NULL.\n");
    I2C_BUG_ON(i2c->msg_buf == NULL);
    ret = -EINVAL_I2C;
    goto err;
  }
err:
  return ret;

}

void _config_mt_i2c(mt_i2c *i2c)
{
  //I2CFUC();
  switch(i2c->id)
  {
    case I2C0:
      i2c->base = I2C0_BASE;
      i2c->pdmabase = I2C0_APDMA_BASE;
      break;
    case I2C1:
      i2c->base = I2C1_BASE;
      i2c->pdmabase = I2C1_APDMA_BASE;
      break;
    case I2C2:
      i2c->base = I2C2_BASE;
      i2c->pdmabase = I2C2_APDMA_BASE;
      break;
    case I2C3:
      i2c->base = I2C3_BASE;
      i2c->pdmabase = I2C3_APDMA_BASE;
      break;
    case I2C4:
      i2c->base = I2C4_BASE;
      i2c->pdmabase = I2C4_APDMA_BASE;
      break;
    case I2C5:
      i2c->base = I2C5_BASE;
      i2c->pdmabase = I2C5_APDMA_BASE;
      break;
    case I2C6:
      i2c->base = I2C6_BASE;
      i2c->pdmabase = I2C6_APDMA_BASE;
      break;
    case I2C7:
      i2c->base = I2C7_BASE;
      i2c->pdmabase = I2C7_APDMA_BASE;
      break;
    case I2C8:
      i2c->base = I2C8_BASE;
      i2c->pdmabase = I2C8_APDMA_BASE;
      break;
    case I2C9:
      i2c->base = I2C9_BASE;
      i2c->pdmabase = I2C9_APDMA_BASE;
      break;
    default:
      I2CERR("invalid para: i2c->id=%d\n",i2c->id);
      break;
  }
  if(i2c->st_rs == I2C_TRANS_REPEATED_START)
    i2c->st_rs = I2C_TRANS_REPEATED_START;
  else
    i2c->st_rs = I2C_TRANS_STOP;
#if 0
  if(i2c->dma_en == TRUE)
    i2c->dma_en = TRUE;
  else
    i2c->dma_en = FALSE;
#else
  i2c->dma_en = FALSE;
#endif
  i2c->poll_en = TRUE;

  if(i2c->filter_msg == TRUE)
    i2c->filter_msg = TRUE;
  else
    i2c->filter_msg = FALSE;

  ///*Set device speed,set it before set_control register
  if(0 == i2c->speed)
  {
    i2c->mode  = ST_MODE;
    i2c->speed = MAX_ST_MODE_SPEED;
  }
  else
  {
    if (i2c->mode  == HS_MODE)
    i2c->mode  = HS_MODE;
    else
    i2c->mode  = FS_MODE;
  }

  /*Set ioconfig*/
  if (i2c->pushpull==TRUE)
    i2c->pushpull=TRUE;
  else
    i2c->pushpull=FALSE;

}

/*-----------------------------------------------------------------------
 * new read interface: Read bytes
 *   mt_i2c:    I2C chip config, see mt_i2c_t.
 *   buffer:  Where to read/write the data.
 *   len:     How many bytes to read/write
 *   Returns: ERROR_CODE
 */
S32 i2c_read(mt_i2c *i2c,U8 *buffer, U32 len)
{
  S32 ret = I2C_OK;
  #ifdef I2C_EARLY_PORTING
  I2CFUC();
  #endif
  //read
  i2c->read_flag|= I2C_M_RD;
  i2c->op = I2C_MASTER_RD;
  i2c->msg_buf = buffer;
  i2c->msg_len = len;
  ret=_i2c_check_para(i2c);
  if(ret< 0){
    I2CERR(" _i2c_check_para fail\n");
    goto err;
  }

  _config_mt_i2c(i2c);
  //get the addr
  ret=_i2c_transfer_interface(i2c);

  if(i2c->msg_len != ret){
    I2CERR("read %d bytes fails,ret=%d.\n",i2c->msg_len,ret);
    ret = -1;
    return ret;
  }else{
    ret = I2C_OK;
    //I2CLOG("read %d bytes pass,ret=%d.\n",i2c->msg_len,ret);
  }
err:
  return ret;
}

/*-----------------------------------------------------------------------
 * New write interface: Write bytes
 *   i2c:    I2C chip config, see mt_i2c_t.
 *   buffer:  Where to read/write the data.
 *   len:     How many bytes to read/write
 *   Returns: ERROR_CODE
 */
S32 i2c_write(mt_i2c *i2c,U8  *buffer, U32 len)
{
  S32 ret = I2C_OK;
  #ifdef I2C_EARLY_PORTING
    I2CFUC();
  #endif
  //write
  i2c->read_flag = !I2C_M_RD;
  i2c->op = I2C_MASTER_WR;
  i2c->msg_buf = buffer;
  i2c->msg_len = len;
  ret=_i2c_check_para(i2c);
  if(ret< 0){
    I2CERR(" _i2c_check_para fail\n");
    goto err;
  }

  _config_mt_i2c(i2c);
  //get the addr
  ret=_i2c_transfer_interface(i2c);

  if(i2c->msg_len != ret){
    I2CERR("Write %d bytes fails,ret=%d.\n",i2c->msg_len,ret);
    ret = -1;
    return ret;
  }else{
    ret = I2C_OK;
    //I2CLOG("Write %d bytes pass,ret=%d.\n",i2c->msg_len,ret);
  }
err:
  return ret;
}

/*-----------------------------------------------------------------------
 * New write then read back interface: Write bytes then read bytes
 *   i2c:    I2C chip config, see mt_i2c_t.
 *   buffer:  Where to read/write the data.
 *   write_len:     How many bytes to write
 *   read_len:     How many bytes to read
 *   Returns: ERROR_CODE
 */
S32 i2c_write_read(mt_i2c *i2c,U8 *buffer, U32 write_len, U32 read_len)
{
  S32 ret = I2C_OK;
  //I2CFUC();
  //write and read
  i2c->op = I2C_MASTER_WRRD;
  i2c->read_flag=!I2C_M_RD;
  i2c->msg_buf = buffer;
  i2c->msg_len = ((read_len & 0xFFFF) << 16) | (write_len & 0xFFFF);
  ret=_i2c_check_para(i2c);
  if(ret< 0){
    I2CERR(" _i2c_check_para fail\n");
    goto err;
  }

  _config_mt_i2c(i2c);
  //get the addr
  ret=_i2c_transfer_interface(i2c);

  if(i2c->msg_len != ret){
    I2CERR("write_read 0x%x bytes fails,ret=%d.\n",i2c->msg_len,ret);
    ret = -1;
    return ret;
  }else{
    ret = I2C_OK;
    //I2CLOG("write_read 0x%x bytes pass,ret=%d.\n",i2c->msg_len,ret);
  }
err:
  return ret;
}

#ifndef CONFIG_MT_I2C_FPGA_ENABLE
static void pull_up_setting(void)
{
#if DIS_CH0_PULL
	/* Switch to GPIO mode (EH)*/
	/* 0x0:GPIO 0x1:I2C */
	/* SDA0[14:12] SCL0[2:0] */
        I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x7 << 12) | (0x7 << 0),
		      (0 << 12) | (0 << 0));
	/* Disable pull-up (PU)*/
	/* 0x0:Disable 0x1:Enable */
	/* SDA0[9] SCL0[1] */
        I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x1 << 21) | (0x1 << 15),
		      (0x0 << 21) | (0x0 << 15));
#endif

#if DIS_CH1_PULL
	/* SDA1[5:3] SCL1[2:0] */
        I2C_SET_REG32(IOCFG_BL_BASE + 0x10, (0x7 << 0) | (0x7 << 3),
		      (0 << 0) | (0 << 3));
	/* SDA1[14] SCL1[13] */
        I2C_SET_REG32(IOCFG_BL_BASE + 0x40, (0x1 << 14) | (0x1 << 13),
		      (0x0 << 14) | (0x0 << 13));
#endif

#if DIS_CH2_PULL
	/* SDA2[8:6] SCL2[2:0] */
        I2C_SET_REG32(IOCFG_BR_BASE + 0x30, (0x7 << 6) | (0x7 << 0),
		      (0 << 6) | (0 << 0));
	/* SDA2[17] SCL2[15] */
        I2C_SET_REG32(IOCFG_BR_BASE + 0xA0, (0x1 << 17) | (0x1 << 15),
		      (0x0 << 17) | (0x0 << 15));
#endif

#if DIS_CH3_PULL
	/* SDA3[20:18] SCL3[8:6] */
        I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x7 << 15) | (0x7 << 3),
		      (0 << 15) | (0 << 3));
	/* SDA3[11] SCL3[3] */
        I2C_SET_REG32(IOCFG_RT_BASE + 0xA0, (0x1 << 18) | (0x1 << 16),
		      (0x0 << 18) | (0x0 << 16));
#endif

#if DIS_CH4_PULL
	/* SDA4[11:9] SCL4[5:3] */
        I2C_SET_REG32(IOCFG_BR_BASE + 0x30, (0x7 << 9) | (0x7 << 3),
		      (0 << 9) | (0 << 3));
	/* SDA4[18] SCL4[16] */
        I2C_SET_REG32(IOCFG_BR_BASE + 0xA0, (0x1 << 18) | (0x1 << 16),
		      (0x0 << 18) | (0x0 << 16));
#endif

#if DIS_CH5_PULL
	/* SDA5[5:3] SCL5[2:0] */
        I2C_SET_REG32(IOCFG_LT_BASE + 0x20, (0x7 << 3) | (0x7 << 0),
		      (0 << 3) | (0 << 0));
	/* SDA5[25] SCL5[24] */
        I2C_SET_REG32(IOCFG_LT_BASE + 0x80, (0x1 << 25) | (0x1 << 24),
		      (0x0 << 25) | (0x0 << 24));
#endif

#if DIS_CH6_PULL
	/* PAD_EINT11 & EINT12 FUNC. 3 internal pullup disable */
	/* SDA6[8] SCL6[7] */
        //I2C_SET_REG32(IOCFG_BL_BASE + 0x90, (0x3 << 7), (0 << 7));
	/* SDA3[23:21] SCL3[11:9] */
        I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x7 << 18) | (0x7 << 6),
		      (0 << 18) | (0 << 6));
	/* SDA3[14] SCL3[4] */
        I2C_SET_REG32(IOCFG_RT_BASE + 0xA0, (0x1 << 23) | (0x1 << 17),
		      (0x0 << 23) | (0x0 << 17));
#endif
#if DIS_CH7_PULL
	/* SDA7[5:3] SCL7[2:0] */
	I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x7 << 21) | 0x7 << 9),
		(0 << 21) | (0 << 9));
	/* SDA7[15] SCL7[14] */
	I2C_SET_REG32(IOCFG_RT_BASE + 0xA0, (0x1 << 24) | (0x1 << 18),
		(0x0 << 24) | (0x0 << 18));
#endif

}

static void internal_resister_setting(void)
{
	/* Switch internal resistor */
	/* 0x0:75k 0x1:5k */
	/* 0x2:15k 0x3:1k */
	/* SDA2[5:4] SCL2[1:0] */
	/* SDA4[7:6] SCL4[3:2] */
	I2C_SET_REG32(IOCFG_BR_BASE + 0xD0, 0xFF, 0xFF);
	/* SDA5[3:2] SCL5[1:0] */
	I2C_SET_REG32(IOCFG_LT_BASE + 0xc0, 0xF, 0xF);
	/* SDA0[9:8] SCL0[1:0] */
	/* SDA3[11:10] SCL3[3:2] */
	/* SDA6[13:12] SCL6[5:4] */
	/* SDA7[15:14] SCL7[7:6] */
	I2C_SET_REG32(IOCFG_RT_BASE + 0x100, 0xFFFF, 0xFFFF);
	/* SDA1[3:2] SCL1[1:0] */
	I2C_SET_REG32(IOCFG_BL_BASE + 0x60, 0xF, 0xF);

}
#endif

int i2c_hw_init(void)
{
	U32 tmp;

	/*enable i2c arbitration*/
	//DRV_WriteReg8(PERICFG_I2C_ARBITRATION, DRV_Reg8(PERICFG_I2C_ARBITRATION) | 0x7);

#ifndef CONFIG_MT_I2C_FPGA_ENABLE
	/*swtich clock to PLL*/
	tmp = DRV_Reg32(MODULE_CLK_SEL_BASE);
	DRV_WriteReg32(MODULE_CLK_SEL_BASE, tmp | USE_PLL_MUX);
#endif

	/* Enable I2C2 & I2C3 & I2C4 &I2C7 buf/imm mode */
	tmp = DRV_Reg32(I2C2_BASE_SE + OFFSET_MULTI_DMA);
	mt_reg_sync_writel(SHADOW_REG_MODE | tmp,
                           I2C2_BASE_SE + OFFSET_MULTI_DMA);
	tmp = DRV_Reg32(I2C4_BASE_SE + OFFSET_MULTI_DMA);
	mt_reg_sync_writel(SHADOW_REG_MODE | tmp,
                           I2C4_BASE_SE + OFFSET_MULTI_DMA);
	tmp = DRV_Reg32(I2C7_BASE_SE + OFFSET_MULTI_DMA);
	mt_reg_sync_writel(SHADOW_REG_MODE | tmp,
                           I2C7_BASE_SE + OFFSET_MULTI_DMA);

#ifndef CONFIG_MT_I2C_FPGA_ENABLE
	pull_up_setting();
	internal_resister_setting();
	I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x7 << 12) | (0x7 << 0),
		      (7 << 12) | (7 << 0));
	/* SDA1[5:3] SCL1[2:0] */
        I2C_SET_REG32(IOCFG_BL_BASE + 0x10, (0x7 << 0) | (0x7 << 3),
		      (7 << 0) | (7 << 3));
	/* SDA2[8:6] SCL2[2:0] */
        I2C_SET_REG32(IOCFG_BR_BASE + 0x30, (0x7 << 6) | (0x7 << 0),
		      (7 << 6) | (7 << 0));
	/* SDA3[20:18] SCL3[8:6] */
        I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x7 << 15) | (0x7 << 3),
		      (7 << 15) | (7 << 3));
	/* SDA4[11:9] SCL4[5:3] */
        I2C_SET_REG32(IOCFG_BR_BASE + 0x30, (0x7 << 9) | (0x7 << 3),
		      (7 << 9) | (7 << 3));
	/* SDA5[5:3] SCL5[2:0] */
        I2C_SET_REG32(IOCFG_LT_BASE + 0x20, (0x7 << 3) | (0x7 << 0),
		      (7 << 3) | (7 << 0));
	/* SDA6[23:21] SCL6[11:9] */
        I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x7 << 18) | (0x7 << 6),
		      (7 << 18) | (7 << 6));
	/* SDA7[5:3] SCL7[2:0] */
	I2C_SET_REG32(IOCFG_RT_BASE + 0x40, (0x7 << 21) | (0x7 << 9),
		(7 << 21) | (7 << 9));
#endif

	return 0;
}

/* Test I2C */
#ifdef I2C_EARLY_PORTING
U32 mt_i2c_test_device(int id, int addr, int offset, int value)
{
	U32 ret = 0;
	U32 len = 0;
	U8 write_byte[2], read_byte[2];
	U32 delay_count = 0xff;
	struct mt_i2c_t i2c;
	//int i = 0;

	I2CLOG("i2c %d test start++++++++++++++++++++\n", id);


	i2c.id = id;
	i2c.addr = addr;
	i2c.mode = FS_MODE;
	i2c.speed = 200;
	/* i2c.mode = FS_MODE;
	i2c.speed = 200;*/
	/* ================================================== */

	I2CLOG("\ntest i2c write\n\n");
	write_byte[0] = offset;
	write_byte[1] = value;
	len = 2;
	ret = i2c_write(&i2c, write_byte, len);
	if (I2C_OK != ret) {
		I2CERR("Write 2 bytes fails(%lx).\n", ret);
		ret = -1;
		return ret;
	} else {
		I2CLOG("Write 2 bytes pass,these bytes are %x, %x.\n", write_byte[0],
			   write_byte[1]);
	}

	/* mdelay(1000); */

	for (delay_count = 0xff; delay_count > 0; delay_count--) ;
	/* ================================================== */
	I2CLOG("\ntest i2c read\n\n");
	//1st:write addree 00,1byte(0x0A)
	//write_byte[0] = 0x0e;
	write_byte[0] = offset;

	len = 1;
	ret = i2c_write(&i2c, write_byte, len);
	if (I2C_OK != ret) {
		I2CERR("Write 1 bytes fails(%lx).\n", ret);
		ret = -1;
		return ret;
		//continue;
	} else {
		I2CLOG("Write 1 bytes pass,these bytes are %x.\n", write_byte[0]);
	}
	/* mdelay(1000); */
	for (delay_count = 0xff; delay_count > 0; delay_count--) ;
	/* 2rd:read back 1byte(0x0A) */
	read_byte[0] = 0x55;
	len = 1;
	ret = i2c_read(&i2c, read_byte, len);
	if ((I2C_OK != ret) || read_byte[0] != value) {
		I2CERR("read 1 bytes fails(%lx).\n", ret);
		I2CLOG("read 1 bytes ,read_byte=%x\n", read_byte[0]);
		ret = -1;
		return ret;
		//continue;
	} else {
		I2CLOG("read 1 bytes pass,read_byte=%x\n", read_byte[0]);
	}

	/* mdelay(1000); */

	for (delay_count = 0xff; delay_count > 0; delay_count--) ;
	/* ================================================== */
	I2CLOG("\ntest i2c write_read\n\n");
	read_byte[0] = offset;
	/* write_byte[1] = 0x34; */
	len = (1 & 0xFF) << 8 | (1 & 0xFF);
	ret = i2c_write_read(&i2c, read_byte, 1, 1);
	if (I2C_OK != ret || read_byte[0] != value) {
		I2CERR("write_read 1 byte fails(ret=%lx).\n", ret);
		I2CLOG("write_read 1 byte fails, read_byte=%x\n", read_byte[0]);
		ret = -1;
		return ret;
	} else {
		I2CLOG("Write_Read 1 byte pass ,this byte is %x.\n", read_byte[0]);
		ret = 0;
	}

	I2CLOG("i2c %d test done-------------------\n", id);
	return ret;
}

int mt_i2c_test(void)
{
	int ret;
	int i = 0;

	for (i = 0; i < 1; i++) {
		ret = mt_i2c_test_device(i, 0x0c, 0x31, 0x03);
		if (0 == ret) {
			I2CLOG("I2C%d,EEPROM test PASS!!\n", i);
		} else {
			I2CLOG("I2C%d,EEPROM test FAIL!!(%d)\n", i, ret);
		}
	}
	return 0;
}
#endif

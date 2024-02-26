#include <stdlib.h>
#include <typedefs.h>
#include <platform.h>
#include <dramc.h>
#include <emi_hw.h>

#define DRAM_START (0x80000000)
#define RANK_SIZE (0x20000000)

extern int RANK_CURR;
extern int DQS_PRINT;
char *opt_gw_coarse_value, *opt_gw_fine_value;
char *opt_gw_coarse_value0, *opt_gw_fine_value0;
char *opt_gw_coarse_value1, *opt_gw_fine_value1;
#ifdef fcGATING_SAME_MCK
char *pre_gw_coarse_value0, *pre_gw_fine_value0;
char *pre_gw_coarse_value1, *pre_gw_fine_value1;
#endif

static U8 dqs_gw_cnt_break;

void dqs_print(LPCSTR sz, ...)
{
   if (DQS_PRINT == 1)
       print(sz);
}

unsigned int score, high_score;
void ett_recursive_factor_tuning(unsigned int n, tuning_factor *tuning_factors, print_callbacks *cbs)
{
    unsigned int i;
    int result;


    //Print banner.
    cbs->ett_print_banner(n);

    if (n == 0) {
        //Before starting the most inner loop, print something.
        cbs->ett_print_before_start_loop_zero();
    }

    for ( i = 0 ; tuning_factors[n].factor_tbl[i] != NULL ; i++) {
        //adjust factor steps
        tuning_factors[n].factor_handler(tuning_factors[n].factor_tbl[i]);
        //set the current factor steps
        tuning_factors[n].curr_val = tuning_factors[n].factor_tbl[i];
        if (n == 0) {//The most inner loop
            //Before each round of the most inner loop, print something.
            cbs->ett_print_before_each_round_of_loop_zero();
            //run test code
            score += cbs->ett_print_result();
            //After each round of the most inner loop, print something.
            cbs->ett_print_after_each_round_of_loop_zero();
        } else {//Other loops. Call this function recursively.
            ett_recursive_factor_tuning(n-1, tuning_factors, cbs);
        }
    }

    cbs->ett_calc_opt_value(n, &score, &high_score);

    //After finishing the each loop, print something.
    cbs->ett_print_after_finish_loop_n(n);
}


/* DQS gating window (coarse) */
char *dqsi_gw_dly_coarse_tbl[] =
{
	"0","1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", 
	"12", "13", "14", "15",
	"16","17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", //"29", "30", "31",
	NULL,
};

void dqsi_gw_dly_coarse_factor_handler_rank1(char *factor_value) 
{
    int curr_val = atoi(factor_value);

    DRAMC_WRITE_REG((DRAMC_READ_REG(0x0118) & 0xFFFFFFF8)
    | (((curr_val >> 2) & 7) << 0), /* DQSINCTL: DRAMC_DQSCTL1[26:24], 3bits */
    0x0118);
    
    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_DQSGCTL/* 0x124 */) & 0xFFFFFF33) /* Reserve original values for DRAMC_DQSGCTL/ */
      | ((curr_val & 0x3) << 2) /* R1DQSG_COARSE_DLY_COM0: DRAMC_DQSGCTL[3:2], 2 bits */
      | ((curr_val & 0x3) << 6), /* R1DQSG_COARSE_DLY_COM1: DRAMC_DQSGCTL[7:6], 2 bits */
      DRAMC_DQSGCTL/* 0x124 */);
}

void dqsi_gw_dly_coarse_factor_handler(char *factor_value) 
{
    int curr_val = atoi(factor_value);

    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_DQSCTL1/* 0xE0 */) & 0xF8FFFFFF)	/* Reserve original values for DRAMC_DQSCTL1 */
		| (((curr_val >> 2) & 7) << 24),			/* DQSINCTL: DRAMC_DQSCTL1[26:24],       3bits */
		DRAMC_DQSCTL1/* 0xE0 */);

    DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_DQSGCTL/* 0x124 */) & 0xFFFFFFCC)	/* Reserve original values for DRAMC_DQSGCTL/ */
      | ((curr_val & 0x3) << 0)			/* R0DQSG_COARSE_DLY_COM0: DRAMC_DQSGCTL[1:0],        2 bits */
      | ((curr_val & 0x3) << 4),			/* R0DQSG_COARSE_DLY_COM1: DRAMC_DQSGCTL[5:4],       2 bits */
      DRAMC_DQSGCTL/* 0x124 */);
}

/* DQS gating window (fine) */
char *dqsi_gw_dly_fine_tbl[] =
{
	"0",/* "1", "2", "3", "4", "5", "6", "7",*/ "8",/* "9", "10", "11", "12", "13", "14", "15",*/
	"16",/* "17", "18", "19", "20", "21", "22", "23",*/ "24",/* "25", "26", "27", "28", "29", "30", "31",*/
	"32",/* "33", "34", "35", "36", "37", "38", "39",*/ "40",/* "41", "42", "43", "44", "45", "46", "47",*/
	"48",/* "49", "50", "51", "52", "53", "54", "55",*/ "56",/* "57", "58", "59", "60", "61", "62", "63",*/
	"64",/* "65", "66", "67", "68", "69", "70", "71",*/ "72", /* "73", "74", "75", "76", "77", "78", "79",*/
	"80",/* "81", "82", "83", "84", "85", "86", "87",*/ "88",/* "89", "90", "91", "92", "93", "94", "95",*/
	"96",/* "97", "98", "99", "100", "101", "102", "103",*/ "104",/* "105", "106", "107", "108", "109", "110", "111",*/
	"112",/* "113", "114", "115", "116", "117", "118", "119",*/ "120",/* "121", "122", "123", "124", "125", "126", "127",*/
	NULL,
};

void dqsi_gw_dly_fine_factor_handler_rank1(char *factor_value) 
{
    int curr_val = atoi(factor_value);
        DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_R1DQSIEN/* 0x98 */) & 0x80000000)  /* Reserve original values for DRAMC_DQSIEN[31] */
          | ((curr_val & 0x7F) << 0)		/* DQS0IEN: DRAMC_DQSIEN[0:6],   7 bits */
          | ((curr_val & 0x7F) << 8)	/* DQS1IEN: DRAMC_DQSIEN[8:14],  7 bits */
          | ((curr_val & 0x7F) << 16)	/* DQS2IEN: DRAMC_DQSIEN[16:22], 7 bits */
          | ((curr_val & 0x7F) << 24),	/* DQS3IEN: DRAMC_DQSIEN[24:30], 7 bits */
          DRAMC_R1DQSIEN/* 0x98 */);
 
}
void dqsi_gw_dly_fine_factor_handler(char *factor_value) 
{
    int curr_val = atoi(factor_value);

        DRAMC_WRITE_REG((DRAMC_READ_REG(DRAMC_R0DQSIEN/* 0x94 */) & 0x80000000)  /* Reserve original values for DRAMC_DQSIEN[31] */
          | ((curr_val & 0x7F) << 0)		/* DQS0IEN: DRAMC_DQSIEN[0:6],   7 bits */
          | ((curr_val & 0x7F) << 8)	/* DQS1IEN: DRAMC_DQSIEN[8:14],  7 bits */
          | ((curr_val & 0x7F) << 16)	/* DQS2IEN: DRAMC_DQSIEN[16:22], 7 bits */
          | ((curr_val & 0x7F) << 24),	/* DQS3IEN: DRAMC_DQSIEN[24:30], 7 bits */
          DRAMC_R0DQSIEN/* 0x94 */);

}

struct dqs_gw_pass_win
{
    int coarse_end;
    int fine_end;
    int size;   /* gating window's size in this range */
};

void Sequence_Read(unsigned int start, unsigned int len, void *ext_arg)
{
     int i;
    volatile int rval;	
        /* DQS gating window counter reset */
    DRAMC_WRITE_SET((1 << 9),DRAMC_SPCMD);
    DRAMC_WRITE_CLEAR((1 << 9),DRAMC_SPCMD);

     DDR_PHY_RESET_NEW();
     for (i = 0 ; i < len ; i++) {
        rval = *(unsigned int *)(start);
    }

}

#if 0
//#ifdef MTK_DDR3_SUPPORT
int Do_Read_Test_DDR3(unsigned int start, unsigned int len, void *ext_arg)
{//for DDR3 x16 and DDR3 x32 gating counter
#if 1
	   //fy for 92 bring up
       int err = 0;
	   unsigned int DQSCounter0 = DRAMC_DQSGNWCNT0;
	   unsigned int DQSCounter1 = DRAMC_DQSGNWCNT1;

	   if(((*(volatile unsigned int *)(0x10004004)) & 0x01) == 0x00)
	   {//16bit
			DQSCounter0 = DRAMC_DQSGNWCNT1;
	   }
       Sequence_Read(start,len,ext_arg);
        //print("[1]DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1:%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
        if (DRAMC_READ_REG(DQSCounter0/* 0x3C0 */) == 0x02020202) {
            Sequence_Read(start,len,ext_arg);
           if (DRAMC_READ_REG(DQSCounter1/* 0x3C0 */) == 0x02020202) {
                err = 0;
            } else {
               err = -1;
               //print("DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1:%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
            }
        } else {
               err = -1;
               //print("DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1:%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
        }

    return err;
#else
    int err = 0;
    int check_result = (int)ext_arg;
    unsigned int data;
    unsigned int cmp_err;
    /* set test patern length*/
    DRAMC_WRITE_REG(0x55000000,0x3C);
    data = DRAMC_READ_REG(0x40);
    DRAMC_WRITE_REG((data & 0xAA000000) | 0x3FF, 0x40);


    DRAMC_WRITE_SET((1 << 31),DRAMC_CONF2); //Test Agent 2 write enabling
    DRAMC_WRITE_SET((1 << 30) | (1 << 31),DRAMC_CONF2); //Test Agent 2 write enabling, Test Agent 2 read enabling

    
    while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));

    /* 
     * NoteXXX: Need to wait for at least 400 ns 
     *          After checking the simulation result, 
     *          there will be a delay on DLE_CNT_OK/DM_CMP_ERR updates after getting DM_CMP_CPT.
     *          i.e; After getting the complete status, need to wait for a while before reading DLE_CNT_OK/DM_CMP_ERR in the TESTRPT register.
     */
    delay_a_while(400);
	check_result = 0;

   // if (check_result)
	if(1)
	{
        //if (check_result == 0xEEEEEEEE)
			if(1)
			{
            //test agent2 called by tx_factor_tuning
            //do per byte check
             cmp_err = DRAMC_READ_REG(DRAMC_CMP_ERR);           
             //print("ta2 cmp_err:%x\n",cmp_err);
             data = DRAMC_READ_REG(DRAMC_PADCTL2);
             //if (cmp_err)	err = -1;
        }
        else
        {
            if (DRAMC_READ_REG(DRAMC_TESTRPT) & (1 << 14)) {
                err = -1;
            } else if (!(DRAMC_READ_REG(DRAMC_TESTRPT) & (1 << 18))) {
                err = -1;
            }
        }
    }

    DRAMC_WRITE_CLEAR(((1 << 30) | (1 << 31)),DRAMC_CONF2); //disable test agent2 r/w

    if ((int)ext_arg == 0xEEEEEEEE )
    {
        if (!err)
            return 0;
        else
            return -1;
    }
    
    //dbg_print("ext_arg:%x,err:%x\n",ext_arg,err);
#ifdef RX_TEST_BY_BIT
    if (((int)ext_arg == 0xFFFFFFFF) || ((int)ext_arg == 0xeeeeeee0))
#else
    if ((int)ext_arg == 0xFFFFFFFF)
#endif
    {
        return err;
    }

   //print("[2]DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1,%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
    DDR_PHY_RESET();
    if (!err) {
        //if ((DRAMC_READ_REG(DRAMC_DQSGNWCNT0) == HW_DQS_GW_COUNTER)&& (DRAMC_READ_REG(DRAMC_DQSGNWCNT1) == HW_DQS_GW_COUNTER)) 
		//dbg_print("[3]DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1,%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));

		if((DRAMC_READ_REG(DRAMC_DQSGNWCNT1) == 0xF8F8F8F8)) 
		{
            err = 0;
        } else {
            err = -1;
        }
    }

	//DDR_PHY_RESET();
    /* DQS gating window counter reset */
      DRAMC_WRITE_SET((1 << 9),DRAMC_SPCMD);
      DRAMC_WRITE_CLEAR((1 << 9),DRAMC_SPCMD);
/* 
    //redundant check?? ask CC Hwang
           dbg_print("[3]DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1,%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
    DDR_PHY_RESET();
    if (!err) {
        if ((DRAMC_READ_REG(DRAMC_DQSGNWCNT0) == HW_DQS_GW_COUNTER)
            && (DRAMC_READ_REG(DRAMC_DQSGNWCNT1) == HW_DQS_GW_COUNTER)) {
            err = 0;
        } else {
            err = -1;
        }
    }
    */
    
dramc_ta2_exit:
    return err;


#endif
}
int Do_Read_Test_DDR2(unsigned int start, unsigned int len, void *ext_arg)
{
       int err = 0;
       Sequence_Read(start,len,ext_arg);
        //print("[1]DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1:%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
        if (DRAMC_READ_REG(DRAMC_DQSGNWCNT0/* 0x3C0 */) == 0x04040404) {
            Sequence_Read(start,len,ext_arg);
           if (DRAMC_READ_REG(DRAMC_DQSGNWCNT0/* 0x3C0 */) == 0x04040404) {
                err = 0;
            } else {
               err = -1;
               //print("DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1:%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
            }
        } else {
               err = -1;
               //print("DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1:%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
        }

    return err;
}
#endif

//Define how many steps we have in coarse tune, fine tune
//check the number of  dqsi_gw_dly_fine_tbl and dqsi_gw_dly_coarse_tbl
//To-be-porting
#define DQS_GW_COARSE_MAX 32
#define DQS_GW_FINE_MAX 16
#define DQS_GW_FINE_CHK_RANGE 4
//static const int HW_DQS_GW_COUNTER = 0x80808080; 
static const int HW_DQS_GW_COUNTER = 0x40404040;
static unsigned int dqs_gw[DQS_GW_COARSE_MAX];
static int dqs_gw_coarse, dqs_gw_fine;
static struct dqs_gw_pass_win cur_pwin, max_pwin;

#if 0
/*in MT6589 cpu read test only used in DQS gating window calibration*/
int cpu_read_test(unsigned int start, unsigned int len, void *ext_arg){
   
    int err = 0;
    int check_result = (int)ext_arg;
    unsigned int data;
    /* cpu read test */

    if (mt_get_dram_type()  == TYPE_LPDDR2)    //LPDDR2
        err = Do_Read_Test_DDR2(start, 1, ext_arg);
    else if (mt_get_dram_type() == TYPE_LPDDR3)//LPDDR3
        err = Do_Read_Test_DDR2(start, 1, ext_arg);
    else if (mt_get_dram_type() == TYPE_PCDDR3)//PCDDR3
        err = Do_Read_Test_DDR3(start, 1, ext_arg);
    else
        err = Do_Read_Test_DDR2(start, 1, ext_arg);


    //err = Do_Read_Test(start, len, ext_arg);
    DRAMC_WRITE_SET((1 << 9),DRAMC_SPCMD);
    DRAMC_WRITE_CLEAR((1 << 9),DRAMC_SPCMD);

    DDR_PHY_RESET_NEW();
    if(err !=0)
    {
        return err;
    }

    return 0;
 
}
#endif
#if 0
/*in MT6589 test agent 1 only used in DLE calibration*/
int dramc_ta1(unsigned int start, unsigned int len, void *ext_arg){
    int err =  TEST_PASS;
    int check_result = (int)ext_arg;
    unsigned int data;
    /* set test patern length*/
    data = DRAMC_READ_REG(0x40);
    DRAMC_WRITE_REG((data & 0xFF000000) | len, 0x40);


    DRAMC_WRITE_SET((1 << 29) ,DRAMC_CONF2); //Test Agent1 

    
    //print("0x3C:%x\n",DRAMC_READ_REG(0x3c));
    //print("0x40:%x\n",DRAMC_READ_REG(0x40));
    //print("DRAMC_CONF2:%x\n",DRAMC_READ_REG(DRAMC_CONF2));
    while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));

    /* 
     * NoteXXX: Need to wait for at least 400 ns 
     *          After checking the simulation result, 
     *          there will be a delay on DLE_CNT_OK/DM_CMP_ERR updates after getting DM_CMP_CPT.
     *          i.e; After getting the complete status, need to wait for a while before reading DLE_CNT_OK/DM_CMP_ERR in the TESTRPT register.
     */
    delay_a_while(400);
    //print("DRAMC_TESTRPT:%x\n",DRAMC_READ_REG(DRAMC_TESTRPT));
    if (check_result) {
        if (DRAMC_READ_REG(DRAMC_TESTRPT) & (1 << 14)) {
            err = -1;
        }
        /*} else if (!(DRAMC_READ_REG(DRAMC_TESTRPT) & (1 << 18))) {
            err = -1;
        }*/
    }

    DRAMC_WRITE_CLEAR((1 << 29) ,DRAMC_CONF2); //disable test agent1

    
    //print("ext_arg:%x,err:%x\n",ext_arg,err);
    if ((int)ext_arg == 0xFFFFFFFF)
    {
        return err;
    }

    return;

}
#endif
/*
 * dramc_ta2: Run DRAMC test agent 2.
 * @start: test start address
 * @len: test length
 * @ext_arg: extend argument (0: don't check read/write results; 1: check)
 * Return error code.
 */
int dramc_ta2(unsigned int start, unsigned int len, void *ext_arg)
{
    int err = 0;
    int check_result = (int)ext_arg;
    unsigned int data;
    unsigned int cmp_err;

    DDR_PHY_RESET_NEW();
    
    //if (RANK_CURR == 1) {
       /*setup dramc test agent base address for rank1 test*/
       //DRAMC_WRITE_SET((0xF<<20),DRAMC_TEST2_1);
    //}

    if (check_result == 0xEEEEEEEE || check_result == 0xFFFFFFFD || check_result == 0xEEEEEEE0 || check_result == 0xFFFFFFFF || check_result == 0xFFFFFFFE) {
    #if 0
        /* set test patern length*/
        DRAMC_WRITE_REG(0x55000000,0x3C);
        data = DRAMC_READ_REG(0x40);
        DRAMC_WRITE_REG((data & 0xAA000000) | 0x3FF, 0x40);
        //DRAMC_WRITE_REG((data & 0xAA000000) | 0x10, 0x40);

        DRAMC_WRITE_SET((1 << 31),DRAMC_CONF2); //Test Agent 2 write enabling
        DRAMC_WRITE_SET((1 << 30) | (1 << 31),DRAMC_CONF2); //Test Agent 2 write enabling, Test Agent 2 read enabling    
        //DRAMC_WRITE_SET((1 << 30),DRAMC_CONF2); //Test Agent 2 read enabling
    #else
        /* set test patern length*/
        DRAMC_WRITE_REG(0x55000000,0x3C);
        data = DRAMC_READ_REG(0x40);
        DRAMC_WRITE_REG((data & 0xAA000000) | 0x3FF, 0x40);
        //DRAMC_WRITE_REG((data & 0xAA000000) | 0x10, 0x40);

        DRAMC_WRITE_SET((1 << 31),DRAMC_CONF2); //Test Agent 2 write enabling
        while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));
        DRAMC_WRITE_CLEAR((1 << 31),DRAMC_CONF2); //disable test agent2 w
        
        DRAMC_WRITE_SET((1 << 30),DRAMC_CONF2); //Test Agent 2 read enabling            
    #endif
    } else {
        /* set test patern length*/
        DRAMC_WRITE_REG(0x55000000,0x3C);
        data = DRAMC_READ_REG(0x40);
        //DRAMC_WRITE_REG((data & 0xAA000000) | 0x3FF, 0x40);
        DRAMC_WRITE_REG((data & 0xAA000000) | 0x10, 0x40);

        //DRAMC_WRITE_SET((1 << 31),DRAMC_CONF2); //Test Agent 2 write enabling
        //DRAMC_WRITE_SET((1 << 30) | (1 << 31),DRAMC_CONF2); //Test Agent 2 write enabling, Test Agent 2 read enabling
        DRAMC_WRITE_SET((1 << 30),DRAMC_CONF2); //Test Agent 2 read enabling
    }
    
    while(!(DRAMC_READ_REG(DRAMC_TESTRPT)&(1 << 10)));

    /* 
     * NoteXXX: Need to wait for at least 400 ns 
     *          After checking the simulation result, 
     *          there will be a delay on DLE_CNT_OK/DM_CMP_ERR updates after getting DM_CMP_CPT.
     *          i.e; After getting the complete status, need to wait for a while before reading DLE_CNT_OK/DM_CMP_ERR in the TESTRPT register.
     */
    delay_a_while(400);
    if (check_result) {
        if (DRAMC_READ_REG(DRAMC_TESTRPT) & (1 << 14)) {
            err = -1;
        } else if (!(DRAMC_READ_REG(DRAMC_TESTRPT) & (1 << 18))) {
            err = -1;
        }
    }

    DRAMC_WRITE_CLEAR(((1 << 30) | (1 << 31)),DRAMC_CONF2); //disable test agent2 r/w

    
    //print("ext_arg:%x,err:%x\n",ext_arg,err);
    if ((int)ext_arg == 0xFFFFFFFF)
    {
        return err;
    }

    //dbg_print("[2]DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1,%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
    DDR_PHY_RESET_NEW();
    if (!err) {
        if ((DRAMC_READ_REG(DRAMC_DQSGNWCNT0) == HW_DQS_GW_COUNTER)
            && (DRAMC_READ_REG(DRAMC_DQSGNWCNT1) == HW_DQS_GW_COUNTER)) {
            err = 0;
        } else {
            err = -1;
        }
    }

    /* DQS gating window counter reset */
    DRAMC_WRITE_SET((1 << 9),DRAMC_SPCMD);
    DRAMC_WRITE_CLEAR((1 << 9),DRAMC_SPCMD);

/* 
    //redundant check?? ask CC Hwang
           dbg_print("[3]DRAMC_DQSGNWCNT0:%x,DRAMC_DQSGNWCNT1,%x\n",DRAMC_READ_REG(DRAMC_DQSGNWCNT0),DRAMC_READ_REG(DRAMC_DQSGNWCNT1));
    DDR_PHY_RESET();
    if (!err) {
        if ((DRAMC_READ_REG(DRAMC_DQSGNWCNT0) == HW_DQS_GW_COUNTER)
            && (DRAMC_READ_REG(DRAMC_DQSGNWCNT1) == HW_DQS_GW_COUNTER)) {
            err = 0;
        } else {
            err = -1;
        }
    }
*/
    
dramc_ta2_exit:
    return err;
}

tuning_factor dqs_gw_tuning_factors[] =
{
    {
        .name = "DQS Gating Window Delay (Fine Scale)",	
        .factor_tbl = dqsi_gw_dly_fine_tbl,
        .curr_val = NULL, 
        .opt_val = NULL, 
        .factor_handler = dqsi_gw_dly_fine_factor_handler,
    },
    {
        .name = "DQS Gating Window Delay (Coarse Scale)",
        .factor_tbl = dqsi_gw_dly_coarse_tbl,
        .curr_val = NULL, 
        .opt_val = NULL, 
        .factor_handler = dqsi_gw_dly_coarse_factor_handler,
    },
};
#if 0
test_case dqs_gw_test_cases_1[] = 
{
    {
        .test_case = cpu_read_test, //use dramc test agent 
        .start = DRAM_START, 
        .range = 0xA, 
        .ext_arg = (void *)0, //check_result, 1st run, no need to check the r/w value
    },
};
#endif

test_case dqs_gw_test_cases_2[] = 
{
    {
        .test_case = dramc_ta2, //use dramc test agent 
        .start = DRAM_START, 
        .range = 0xA, 
        .ext_arg = (void *)0, //check_result, 2nd run, it's need to check the r/w value
    },
};
#if 0
test_case dqs_gw_test_cases_1[] =
{
    {
        .test_case = dramc_ta2,
        .start = RANK_SIZE,
        .range = 0xA,
        .ext_arg = (void *)0,
    },
};

test_case dqs_gw_test_cases_2[] =
{
    {
        .test_case = dramc_ta2,
        .start = RANK_SIZE,
        .range = 0xA,
        .ext_arg = (void *)1,
    },
};
#endif

void ett_print_dqs_gw_banner(unsigned int n)
{
    if (n == 1) {
#if 1
        unsigned int i;
        dqs_print("=============================================\n");
        for ( i = 2 ; i < ETT_TUNING_FACTOR_NUMS(dqs_gw_tuning_factors) ; i++) {
            dqs_print("%s = %d\n", dqs_gw_tuning_factors[i].name, atoi(dqs_gw_tuning_factors[i].curr_val));
        }
        dqs_print("X-axis: %s\n", dqs_gw_tuning_factors[0].name);
        if (ETT_TUNING_FACTOR_NUMS(dqs_gw_tuning_factors) > 1) {
            dqs_print("Y-axis: %s\n", dqs_gw_tuning_factors[1].name);
        }
        dqs_print("=============================================\n");
#endif
#if !defined(RELEASE)
        dqs_print("          0    8   16   24   32   40   48   56   64   72   80   88   96  104  112  120\n");
#endif
        dqs_print("      --------------------------------------------------------------------------------\n");
    }
}

void ett_print_dqs_gw_before_start_loop_zero(void) 
{
#if !defined(RELEASE)
    dqs_print("%H:|", atoi(dqs_gw_tuning_factors[1].curr_val));
#endif
}

void ett_print_dqs_gw_before_each_round_of_loop_zero(void) 
{
    /* DQS gating window counter reset */
    DRAMC_WRITE_SET((1 << 9),DRAMC_SPCMD);
    DRAMC_WRITE_CLEAR((1 << 9),DRAMC_SPCMD);
#if 0
    *(volatile unsigned int *)DRAMC_SPCMD |= (1 << 9);
    *(volatile unsigned int *)DRAMC_SPCMD &= ~(1 << 9);
#endif
    DDR_PHY_RESET();
}

static unsigned int __ett_print_dqs_gw_result(test_case *test_cases, int nr_ts)
{
    unsigned int i, score = 1;

    for (i = 0; i < nr_ts; i++) {
        /* fixup the test start_address due to dual rank*/
            test_cases[i].start = DRAM_START;

        if (test_cases[i].test_case(test_cases[i].start, test_cases[i].range, test_cases[i].ext_arg) < 0) {
            dqs_print("    0");
            score = 0;
            break;
        }
    }
    /*
     * DQS GW calibration rule 1: Identify a pass-window with the max gw.
     */
#if 0
    if (score != 0) {
        cur_pwin.size++;
        //print("(%d)",cur_pwin.size);
    } else if (cur_pwin.size != 0) {
        /* end of the pass-window */
        if (dqs_gw_fine) {
            print("(%d)",dqs_gw_coarse);
            cur_pwin.coarse_end = dqs_gw_coarse;
            cur_pwin.fine_end = dqs_gw_fine - 1;
        } else {
            if (dqs_gw_coarse == 0) {
                /* never happen */ 
                print("Critical error! dqs_gw_coarse = 0 but dqs_gw_fine = 0!\n");
            }
            cur_pwin.coarse_end = dqs_gw_coarse - 1;
            cur_pwin.fine_end = DQS_GW_FINE_MAX - 1;
        }

        /* update the max pass-window */
        if (cur_pwin.size > max_pwin.size) {
            memcpy((void *)&max_pwin, (void *)&cur_pwin, sizeof(struct dqs_gw_pass_win));
        }

        memset((void *)&cur_pwin, 0, sizeof(struct dqs_gw_pass_win));
    }

#endif
    if (score != 0) {
        dqs_print("    1");
        dqs_gw[dqs_gw_coarse] |= (1 << dqs_gw_fine); //attension:dqs_gw_fine steps must less then 32
    }

    dqs_gw_fine++;
    if (dqs_gw_fine >= DQS_GW_FINE_MAX) { //dqs_gw_fine >= 16
        dqs_gw_coarse++; 
        dqs_gw_fine &= (DQS_GW_FINE_MAX - 1);
    }
    if (dqs_gw_coarse > DQS_GW_COARSE_MAX) {
        dqs_print("Critical error!! dqs_gw_coarse > DQS_GW_COARSE_MAX\n");
    }

    return score;
}

#if 0
static unsigned int ett_print_dqs_gw_result_1(void)
{
   return __ett_print_dqs_gw_result(dqs_gw_test_cases_1, ETT_TEST_CASE_NUMS(dqs_gw_test_cases_1)); 
}
#endif

static unsigned int ett_print_dqs_gw_result_2(void)
{
   return __ett_print_dqs_gw_result(dqs_gw_test_cases_2, ETT_TEST_CASE_NUMS(dqs_gw_test_cases_2)); 
}

void ett_print_dqs_gw_after_each_round_of_loop_zero(void) 
{
}

void ett_calc_dqs_gw_opt_value(unsigned int n, unsigned int *score, unsigned int *high_score)
{
}

void ett_print_dqs_gw_after_finish_loop_n(unsigned int n) 
{
    if (n == 0) {
        dqs_print("\n");
    }
}

void ett_print_dqs_gw_fine_after_finish_loop_zero(int n)
{
    if (n == 0){
        dqs_print("\n");
    }
}

/*
 * nr_bit_set: Get the number of bits set in the given value.
 * @val: the gieven value
 * Return the number of bits set.
 */
static int nr_bit_set(unsigned int val)
{
    int i, cnt;

    for (cnt = 0, i = 0; i < (8 * sizeof(unsigned int)); i++) {
        if (val & (1 << i)) 
        {
            cnt++;
        }
        else
        {
            if (cnt != 0)
            {
                // cnt !=0, val ==0, break;
                dqs_gw_cnt_break = 1;
                break;
            }
        }
    }

    return cnt;
#if 0
    /*
    * Hamming Weight algorithm
    * http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
    */
    val = val - ((val >> 1) & 0x55555555);
    val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
    return (((val + (val >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
#endif

}

/*
 * first_bit_set: Get the first bit set in the given value.
 * @val: the gieven value
 * Return the first bit set.
 */
static int first_bit_set(unsigned int val)
{
    int i;

    for (i = 0; i < (8 * sizeof(unsigned int)); i++) {
        if (val & (1 << i)) {
            return i;
        }
    }

    return -1;
#if 0
    /*
    * using arm support instruction CLZ to find leading set
    * CLZ user guide:
    * The result value is 32 if no bits are set in the source register, and zero if bit 31 is set.
    * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0204h/Cihjgjed.html
    */
    int ret;
    volatile asm("clz\t%0, %1" : "=r" (ret) : "r" (val));
    ret = 31 -ret;
    return ret;
#endif

}

#ifdef fcFIX_GATING_PRE_FAKEWIN
// clear bit when cnt < 3
static int first_bit_set_clear(unsigned int *val)
{
    int i;
    int clear_bit_flag = 0;

    for (i = 0; i < 32; i++) 
    {
        if (*val & (1 << i)) 
        {		    
            *val = *val & (~(1 << i));
            clear_bit_flag = 1;
            //return i;
        }
        else
        {
            if (clear_bit_flag == 1)
            {
                clear_bit_flag = 0;
                return i;
            }
        }
    }

    return -1;
}
#endif

/*
 * __do_dqs_gw_calib: do DQS gating window calibration.
 * @cbs: pointer to the print_callbacks structure.
 * Return error code.
 */
static int __do_dqs_gw_calib(print_callbacks *cbs)
{
    int err;
    int i, c=0, f, f0, cnt, max=0;
#ifdef fcGATING_SAME_MCK
    int c1, f1, cnt1;
#endif
#ifdef fcNEW_GATING_FINETUNE_LIMIT
    int opt_fine_idx_0, first_bit_idx_0;
#endif
#ifdef FINETUNE_CENTER
    int MinCenterDiff = 127, CenterDiff, fine_idx, FineTuneKPt;    
    //FineTuneKPt = 64; // in the middle of fine tune taps 
    FineTuneKPt = 68; // more margin to favor less coarse tune tap 
#endif
    unsigned int dqs_max = sizeof(dqsi_gw_dly_fine_tbl)/sizeof(char*);


    err = -1;

    dqs_gw_coarse = 0; //from begin of coarse tune, reset to 0
    dqs_gw_fine = 0;   //from begin of fine tune, reset to 0
    for (i = 0; i < DQS_GW_COARSE_MAX; i++) {
        dqs_gw[i] = 0;
    }
    memset((void *)&cur_pwin, 0, sizeof(struct dqs_gw_pass_win));
    memset((void *)&max_pwin, 0, sizeof(struct dqs_gw_pass_win));

    /* 1.enable burst mode for gating window */
    /*   enable DQS gating window counter */
    DRAMC_WRITE_SET((1 << 28),DRAMC_DQSCTL1);
    DRAMC_WRITE_SET((1 << 8),DRAMC_SPCMD);

    if (ETT_TUNING_FACTOR_NUMS(dqs_gw_tuning_factors) > 0) {
        ett_recursive_factor_tuning(ETT_TUNING_FACTOR_NUMS(dqs_gw_tuning_factors)-1, dqs_gw_tuning_factors, cbs);
    }

    for (i = 0; i < DQS_GW_COARSE_MAX; i++) 
    {
            // find the max passed window
            dqs_gw_cnt_break = 0;
            cnt = nr_bit_set(dqs_gw[i]);

        #ifdef fcFIX_GATING_PRE_FAKEWIN
            if ((dqs_gw_cnt_break == 1)&&(cnt < 3))
            {            
                f0 = first_bit_set(dqs_gw[i]);
                if (f0 != -1)
                {                  
                    if (f0 >= (DQS_GW_FINE_MAX-cnt))
                    {
                        dqs_gw_cnt_break = 0;
                    }
                }
		    }
		
            if ((dqs_gw_cnt_break == 1)&&(cnt < 3))
            {
                // cnt = 1 or 2, pre-amble has fake window
                // skip this and continue to count              
                f0 = first_bit_set_clear(&(dqs_gw[i]));
                if (f0 != -1)
                {
                    dqs_print("[fcFIX_GATING_PRE_FAKEWIN] fake/break pre-amble bit %d\n", f0);
                }	                	
						
                cnt = 0;
                dqs_gw_cnt_break = 0;                       
                cnt = nr_bit_set(dqs_gw[i]);                
                dqs_print("[fcFIX_GATING_PRE_FAKEWIN]cnt < 3 and break window, re-count window\n");			
            }		
        #endif

        #ifdef FINETUNE_CENTER
            if (cnt)
            {
                f0 = first_bit_set(dqs_gw[i]);
                f = f0 + cnt / 2;

                if ((f >= dqs_max) || (f < 0)) {
                    //print("[DRAMC] dqsi_gw_dly_fine_tbl array szie = %d\n", dqs_max);
                    f = 0;
                }

                fine_idx = atoi(dqsi_gw_dly_fine_tbl[f]);

                if (fine_idx > FineTuneKPt)
                {
                    CenterDiff = fine_idx - FineTuneKPt;
                }
                else
                {
                    CenterDiff = FineTuneKPt - fine_idx ;
                }
                dqs_print("first_bit_idx=%d,", f);
                dqs_print(" fine_idx=%d,", fine_idx);
                dqs_print(" CenterDiff=%d,", CenterDiff);
                dqs_print(" MinCenterDiff=%d,", MinCenterDiff);
                dqs_print(" FineTuneKPt=%d\n", FineTuneKPt);

                if (CenterDiff < MinCenterDiff)
                {
                    MinCenterDiff = CenterDiff;
                    max = cnt;
                    c = i;
                }
                if (f0==0)
                {
                    break;
                }
            }
        #else
#if 0
            print("nr_bit_set(dqs_gw[%d]) = %d,max:%d\n", i, cnt, max);
#endif
            if (cnt > max) {
                max = cnt;
                c = i;
            }
        #endif
    }
    /*if ((RANK_CURR == 1) && ((atoi(opt_gw_coarse_value0) & 0x1c)!= (atoi(dqsi_gw_dly_coarse_tbl[c]) & 0x1c))){    
        //fix rank 0 coarse value not match rank 1 coarse value
        c = atoi(opt_gw_coarse_value0) - atoi(dqsi_gw_dly_coarse_tbl[0]);
        dqs_print("[EMI] Fix Rank 1 coarse value\n");
    }*/
    dqs_print("Rank %d coarse tune value selection :", RANK_CURR);
    dqs_print(" %d,", c);
    dqs_print(" %s\n", dqsi_gw_dly_coarse_tbl[c]);
      
    cnt = nr_bit_set(dqs_gw[c]);
    
#ifdef fcGATING_SAME_MCK
    if (c >= 1)
    {
        c1 = c - 1;
        cnt1 = nr_bit_set(dqs_gw[c1]);
    }
    else
    {
        c1 = c;
        cnt1 = cnt;
    }

    //record the prviois coarse tune and fine tune
    if (cnt1)
    {
        f1 = first_bit_set(dqs_gw[c1]) + cnt1 / 2;
        if (RANK_CURR == 0)
        {
            pre_gw_coarse_value0 = dqsi_gw_dly_coarse_tbl[c1];
            pre_gw_fine_value0 = dqsi_gw_dly_fine_tbl[f1];
            dbg_print("[fcGATING_SAME_MCK]rank0 gating record N-1, ");
            dbg_print("coarse = %s, ", pre_gw_coarse_value0);
            dbg_print("fine = %s\n", pre_gw_fine_value0);
        }
        else
        {
            pre_gw_coarse_value1 = dqsi_gw_dly_coarse_tbl[c1];
            pre_gw_fine_value1 = dqsi_gw_dly_fine_tbl[f1];
            dbg_print("[fcGATING_SAME_MCK]rank1 gating record N-1, ");
            dbg_print("coarse = %s, ", pre_gw_coarse_value1);
            dbg_print("fine = %s\n", pre_gw_fine_value1);
        }
    }    
#endif
    
    if (cnt) {
        //print("first_bit_set(dqs_gw[c]):%d,f:%d\n",first_bit_set(dqs_gw[c]),cnt/2);
        f0 = first_bit_set(dqs_gw[c]);
        f = f0 + cnt / 2;

        if ((f >= dqs_max) || (f < 0)) {
            //print("[DRAMC] dqsi_gw_dly_fine_tbl array szie = %d\n", dqs_max);
            f = 0;
        }

    #ifdef fcNEW_GATING_FINETUNE_LIMIT   // to filter fake passed window        
        if (c<=1)
            c = 1;
        first_bit_idx_0 = first_bit_set(dqs_gw[c-1]);
        if (first_bit_idx_0 != -1)
        {
            opt_fine_idx_0 = first_bit_idx_0;
        }
        else
        {
            opt_fine_idx_0 = 127;
        }        

        //dqs_print("[New Gating with FineTune Limit] opt_coarse_idx=%d/%d, first_bit_idx=%d/%d,", c-1, c, first_bit_idx_0, f0);
        //dqs_print(" opt_fine_idx=%d/%d\n", opt_fine_idx_0, f);

        dqs_print("[New Gating with FineTune Limit]");
        dqs_print(" opt_coarse_idx=%d/", c-1);
        dqs_print("%d,", c);
        dqs_print(" first_bit_idx=%d/", first_bit_idx_0);
        dqs_print("%d,", f0);
        dqs_print(" opt_fine_idx=%d/", opt_fine_idx_0);
        dqs_print("%d\n", f);

        if ((first_bit_idx_0>f0) && (opt_fine_idx_0 < f))
        {
            f = opt_fine_idx_0;
            dqs_print("[New Gating with FineTune Limit] Limit opt_fine_idx to %d!!\n", f);
        } 
    #endif

#ifdef RELEASE
        dbg_print("%s\n", opt_gw_coarse_value = dqsi_gw_dly_coarse_tbl[c]);
        dbg_print("%s\n", opt_gw_fine_value = dqsi_gw_dly_fine_tbl[f]);
#else
        opt_gw_coarse_value = dqsi_gw_dly_coarse_tbl[c];
        opt_gw_fine_value = dqsi_gw_dly_fine_tbl[f];
#endif

        /* setup the opt coarse value and fine value according to calibration result*/
        dqsi_gw_dly_coarse_factor_handler(dqsi_gw_dly_coarse_tbl[c]);
        dqsi_gw_dly_fine_factor_handler(dqsi_gw_dly_fine_tbl[f]);
        err = 0;
    }
    else {
        dqs_print("Cannot find any pass-window\n");
    }
    /* disable DQS gating window counter */
    DRAMC_WRITE_CLEAR((1 << 8),DRAMC_SPCMD);

    DDR_PHY_RESET_NEW();

__do_dqs_gw_calib_exit:
    return err;
}

/*
 * do_dqs_gw_calib_1: do DQS gating window calibration (phase 1).
 * Return error code.
 */
#if 0
int do_dqs_gw_calib_1(void)
{
    print_callbacks cbs = {
        .ett_print_banner = ett_print_dqs_gw_banner,
        .ett_print_before_start_loop_zero = ett_print_dqs_gw_before_start_loop_zero,
        .ett_print_before_each_round_of_loop_zero = ett_print_dqs_gw_before_each_round_of_loop_zero,
        .ett_print_result = ett_print_dqs_gw_result_1, //difference between do_dqs_gw_calib_2
        .ett_print_after_each_round_of_loop_zero = ett_print_dqs_gw_after_each_round_of_loop_zero,
        .ett_calc_opt_value = ett_calc_dqs_gw_opt_value,
        .ett_print_after_finish_loop_n = ett_print_dqs_gw_fine_after_finish_loop_zero,
    };

    return __do_dqs_gw_calib(&cbs);
}
#endif

/*
 * do_dqs_gw_calib_2: do DQS gating window calibration (phase 2).
 * Return error code.
 */
int do_dqs_gw_calib_2(void)
{
    print_callbacks cbs = {
        .ett_print_banner = ett_print_dqs_gw_banner,
        .ett_print_before_start_loop_zero = ett_print_dqs_gw_before_start_loop_zero,
        .ett_print_before_each_round_of_loop_zero = ett_print_dqs_gw_before_each_round_of_loop_zero,
        .ett_print_result = ett_print_dqs_gw_result_2, //difference between do_dqs_gw_calib_1
        .ett_print_after_each_round_of_loop_zero = ett_print_dqs_gw_after_each_round_of_loop_zero,
        .ett_calc_opt_value = ett_calc_dqs_gw_opt_value,
        .ett_print_after_finish_loop_n = ett_print_dqs_gw_fine_after_finish_loop_zero,
    };

    return __do_dqs_gw_calib(&cbs);
}


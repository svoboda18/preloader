//
//  DRAMC_COMMON.H    
//

#ifndef _DRAMC_COMMON_H_
#define _DRAMC_COMMON_H_

#include "dramc_register.h"
#include "dramc_pi_api.h"
#include "dramc_int_global.h"
#include "emi.h"
#if __ETT__
    #include <common.h>
#endif

/***********************************************************************/
/*                  Public Types                                       */
/***********************************************************************/
#if SW_CHANGE_FOR_SIMULATION
typedef signed char     INT8;
typedef signed short    INT16;
typedef signed int      INT32;
typedef signed char     S8;
typedef signed short    S16;
typedef signed int      S32;
typedef unsigned char       U8;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef unsigned char       BOOL;
#define FALSE   0
#define TRUE    1
#endif
/*------------------------------------------------------------*/
/*                  macros, defines, typedefs, enums          */
/*------------------------------------------------------------*/
/************************** Common Macro *********************/
#if __ETT__
#if ((!defined(_WIN32)) && (FOR_DV_SIMULATION_USED==0) && (SW_CHANGE_FOR_SIMULATION==0))
#else
#undef dsb
#define dsb()
#endif
#endif

// K2?? : The following needs to be porting.
// choose a proper mcDELAY
#if __ETT__
#if defined(DUMP_INIT_RG_LOG_TO_DE)
#define mcDELAY_US(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_US(%d);\n",x)); GPT_Delay_us((U32) (x))
#define mcDELAY_MS(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_MS(%d);\n",x)); GPT_Delay_ms((U32) (x))
#elif (FOR_DV_SIMULATION_USED==0)
#define mcDELAY_US(x)       GPT_Delay_us((U32) (x))
#define mcDELAY_MS(x)       GPT_Delay_ms((U32) (x))
#else
#define mcDELAY_US(x)       GPT_Delay_us((U32) (1))
#define mcDELAY_MS(x)       GPT_Delay_ms((U32) (1))
#define mcDELAY_XUS(x)      GPT_Delay_ms((U32) (x))
#endif
#else
//====
  #if __FLASH_TOOL_DA__
    #define mcDELAY_US(x)       gpt4_busy_wait_us(x)
    #define mcDELAY_MS(x)       gpt4_busy_wait_us(x*1000)
  #else
        #if (FOR_DV_SIMULATION_USED==1)  
#if defined(DUMP_INIT_RG_LOG_TO_DE)//saint dump RG
            #define mcDELAY_US(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_US(%d);\n",x)); delay_us(1)
            #define mcDELAY_XUS(x)      if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_US(%d);\n",x)); delay_us(x)
            #define mcDELAY_MS(x)       if (gDUMP_INIT_RG_LOG_TO_DE_RG_log_flag) mcSHOW_DUMP_INIT_RG_MSG(("DELAY_MS(%d);\n",x)); delay_us(1)
#else
            #define mcDELAY_US(x)       delay_us(1)
            #define mcDELAY_XUS(x)      delay_us(x)
            #define mcDELAY_MS(x)       delay_us(1)
#endif
        #elif (SW_CHANGE_FOR_SIMULATION==1)
            #define mcDELAY_US(x)       *MDM_TM_WAIT_US = x; while (*MDM_TM_WAIT_US>0);
            #define mcDELAY_MS(x)       *MDM_TM_WAIT_US = x; while (*MDM_TM_WAIT_US>0);
        #else
            #define mcDELAY_US(x)       gpt_busy_wait_us(x)
            #define mcDELAY_MS(x)       gpt_busy_wait_us(x*1000)
        #endif
  #endif
#endif

/**********************************************/
/* Priority of debug log                      */
/*--------------------------------------------*/
/* mcSHOW_DBG_MSG: High                       */
/* mcSHOW_DBG_MSG2: Medium High               */
/* mcSHOW_DBG_MSG3: Medium Low                */ 
/* mcSHOW_DBG_MSG4: Low                       */
/**********************************************/
#if __FLASH_TOOL_DA__
  #define printf DBG_MSG
  #define print DBG_MSG
#elif defined(RELEASE)
#if !__ETT__
  #undef printf
  #define printf
  #undef print
  #define print
#endif
#endif

#if __ETT__
    #if (defined(DDR_INIT_TIME_PROFILING))
    #define mcSHOW_DBG_MSG(_x_) 
    #define mcSHOW_DBG_MSG2(_x_)   
    #define mcSHOW_DBG_MSG3(_x_) 
    #define mcSHOW_DBG_MSG4(_x_) 
    #define mcSHOW_JV_LOG_MSG(_x_) 
    #define mcSHOW_EYESCAN_MSG(_x_)    
    #define mcSHOW_TIME_MSG(_x_)   {opt_print _x_;}
    #define mcSHOW_ERR_MSG(_x_)  
    #define mcDUMP_REG_MSG(_x_)
    #elif DUMP_ALLSUH_RG
    #define mcSHOW_DBG_MSG(_x_) 
    #define mcSHOW_DBG_MSG2(_x_)   
    #define mcSHOW_DBG_MSG3(_x_) 
    #define mcSHOW_DBG_MSG4(_x_) 
    #define mcSHOW_JV_LOG_MSG(_x_) 
    #define mcSHOW_EYESCAN_MSG(_x_)    
    #define mcSHOW_TIME_MSG(_x_)  
    #define mcSHOW_ERR_MSG(_x_) {mcDELAY_US(50);opt_print _x_;}
    #define mcDUMP_REG_MSG(_x_) {mcDELAY_US(50);opt_print _x_;}
    #elif defined(RELEASE)
    #define mcSHOW_DBG_MSG(_x_)  //{opt_print _x_;}
    #define mcSHOW_DBG_MSG2(_x_)   
    #define mcSHOW_DBG_MSG3(_x_) 
    #define mcSHOW_DBG_MSG4(_x_) 
    #define mcSHOW_JV_LOG_MSG(_x_) {opt_print _x_;}    
    #define mcSHOW_EYESCAN_MSG(_x_) {opt_print _x_;}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV   
    #define mcSHOW_TIME_MSG(_x_) 
    #define mcSHOW_ERR_MSG(_x_)  
    #define mcDUMP_REG_MSG(_x_)
    #elif VENDER_JV_LOG
    #define mcSHOW_DBG_MSG(_x_)  
    #define mcSHOW_DBG_MSG2(_x_)   
    #define mcSHOW_DBG_MSG3(_x_) 
    #define mcSHOW_DBG_MSG4(_x_) 
    #define mcSHOW_JV_LOG_MSG(_x_)    {opt_print _x_;}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV
    #define mcSHOW_EYESCAN_MSG(_x_) {opt_print _x_;}   //mcSHOW_JV_LOG_MSG(_x_) is for vendor JV   
    #define mcSHOW_TIME_MSG(_x_)
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #elif FOR_DV_SIMULATION_USED
    #define mcSHOW_DBG_MSG(_x_)   {printf _x_;}    
    #define mcSHOW_DBG_MSG2(_x_)  {printf _x_;}    
    #define mcSHOW_DBG_MSG3(_x_)  {printf _x_;}    
    //#define mcSHOW_DBG_MSG4(_x_)  if (RXPERBIT_LOG_PRINT) {printf _x_;}
    #define mcSHOW_DBG_MSG4(_x_)  {printf _x_;}
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_TIME_MSG(_x_)    
    #define mcSHOW_EYESCAN_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)   {printf _x_;}
    #define mcDUMP_REG_MSG(_x_)
    #elif SW_CHANGE_FOR_SIMULATION
    #define mcSHOW_DBG_MSG(_x_)     
    #define mcSHOW_DBG_MSG2(_x_)    
    #define mcSHOW_DBG_MSG3(_x_)    
    #define mcSHOW_DBG_MSG4(_x_)   
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_)  
    #define mcSHOW_TIME_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)  
    #define mcDUMP_REG_MSG(_x_)
    #elif defined(DUMP_INIT_RG_LOG_TO_DE)
    #define mcSHOW_DBG_MSG(_x_)     
    #define mcSHOW_DBG_MSG2(_x_)    
    #define mcSHOW_DBG_MSG3(_x_)    
    #define mcSHOW_DBG_MSG4(_x_)  
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_DUMP_INIT_RG_MSG(_x_) opt_print _x_; 
    #define mcSHOW_EYESCAN_MSG(_x_)    
    #define mcSHOW_TIME_MSG(_x_)      
    #define mcSHOW_ERR_MSG(_x_) 
    #define mcDUMP_REG_MSG(_x_)
    #elif MRW_CHECK_ONLY
    #define mcSHOW_DBG_MSG(_x_)     
    #define mcSHOW_DBG_MSG2(_x_)    
    #define mcSHOW_DBG_MSG3(_x_)    
    #define mcSHOW_DBG_MSG4(_x_)   
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_MRW_MSG(_x_)    {printf _x_;}
    #define mcSHOW_EYESCAN_MSG(_x_)  
    #define mcSHOW_TIME_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)
    #define mcDUMP_REG_MSG(_x_)
    #else   // ETT real chip
    #define mcSHOW_DBG_MSG(_x_)   {mcDELAY_US(10);opt_print _x_;}
    #define mcSHOW_DBG_MSG2(_x_)  {mcDELAY_US(10);opt_print _x_;}
    #define mcSHOW_DBG_MSG3(_x_)
    #define mcSHOW_DBG_MSG4(_x_)  
    #define mcSHOW_JV_LOG_MSG(_x_)  
    #define mcSHOW_EYESCAN_MSG(_x_) {if (gEye_Scan_color_flag) {mcDELAY_US(200);};opt_print _x_;}
    #define mcSHOW_TIME_MSG(_x_)  
    #define mcSHOW_ERR_MSG(_x_)   {opt_print _x_;}
    #define mcDUMP_REG_MSG(_x_)
    #endif    
    
#else  // preloader
    #if defined(DDR_INIT_TIME_PROFILING)   
    #define mcSHOW_DBG_MSG(_x_) 
    #define mcSHOW_DBG_MSG2(_x_)   
    #define mcSHOW_DBG_MSG3(_x_) 
    #define mcSHOW_DBG_MSG4(_x_) 
    #define mcSHOW_JV_LOG_MSG(_x_) 
    #define mcSHOW_EYESCAN_MSG(_x_)    
    #define mcSHOW_TIME_MSG(_x_)   {print _x_;}
    #define mcSHOW_ERR_MSG(_x_)  
    #define mcDUMP_REG_MSG(_x_)
    #elif defined(TARGET_BUILD_VARIANT_ENG) //&& !defined(MTK_EFUSE_WRITER_SUPPORT) && !CFG_TEE_SUPPORT && !MTK_EMMC_SUPPORT
    #define mcSHOW_DBG_MSG(_x_)   {print _x_;}    
    #define mcSHOW_DBG_MSG2(_x_)  {print _x_;}    
    #define mcSHOW_DBG_MSG3(_x_)      
    #define mcSHOW_DBG_MSG4(_x_)  
    #define mcSHOW_EYESCAN_MSG(_x_)  {if(dram_log_to_storage_flag) print _x_;}
    #define mcSHOW_JV_LOG_MSG(_x_)   {if(dram_log_to_storage_flag )print _x_;}
    #define mcSHOW_TIME_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)   {print _x_;}
    #define mcDUMP_REG_MSG(_x_)
    #elif FOR_DV_SIMULATION_USED
        #if defined(DUMP_INIT_RG_LOG_TO_DE)
        #define mcSHOW_DBG_MSG(_x_)     
        #define mcSHOW_DBG_MSG2(_x_)    
        #define mcSHOW_DBG_MSG3(_x_)    
        #define mcSHOW_DBG_MSG4(_x_)  
        #define mcSHOW_JV_LOG_MSG(_x_)
        #define mcSHOW_DUMP_INIT_RG_MSG(_x_) {printf _x_;}
        #define mcSHOW_EYESCAN_MSG(_x_)    
        #define mcSHOW_TIME_MSG(_x_)      
        #define mcSHOW_ERR_MSG(_x_) 
        #define mcDUMP_REG_MSG(_x_)
        #else
        #define mcSHOW_DBG_MSG(_x_)   {printf _x_;}    
        #define mcSHOW_DBG_MSG2(_x_)  {printf _x_;}    
        #define mcSHOW_DBG_MSG3(_x_)  {printf _x_;}    
        #define mcSHOW_DBG_MSG4(_x_)  {printf _x_;}
        #define mcSHOW_JV_LOG_MSG(_x_)
        #define mcSHOW_DUMP_INIT_RG_MSG(_x_)
        #define mcSHOW_EYESCAN_MSG(_x_)    
        #define mcSHOW_TIME_MSG(_x_)    
        #define mcSHOW_ERR_MSG(_x_)   {printf _x_;}
        #define mcDUMP_REG_MSG(_x_)
        #endif
    #else 
    #define mcSHOW_DBG_MSG(_x_)     
    #define mcSHOW_DBG_MSG2(_x_)    
    #define mcSHOW_DBG_MSG3(_x_)    
    #define mcSHOW_DBG_MSG4(_x_)    
    #define mcSHOW_JV_LOG_MSG(_x_)
    #define mcSHOW_EYESCAN_MSG(_x_)    
    #define mcSHOW_TIME_MSG(_x_)    
    #define mcSHOW_ERR_MSG(_x_)     
    #define mcDUMP_REG_MSG(_x_)
    #endif
#endif

#define mcFPRINTF(_x_)           
//#define mcFPRINTF(_x_)          fprintf _x_;

extern int dump_log;
#endif   // _DRAMC_COMMON_H_

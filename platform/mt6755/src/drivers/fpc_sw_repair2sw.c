#include <project.h>
#include "typedefs.h"
#include "wdt.h"
#define printf print

/* will be placed in mediatek/platform/mt6582/lk/platform.c */
int repair_sram(void)
{
     int ret = 0;
     // --------------------------------------------------
     // common
     // --------------------------------------------------

     //lte_mbist_rstb_assert
     *((UINT32P) (0x26716054)) = 0x00000000;
     *((UINT32P) (0x26716064)) = 0x00000000;
     *((UINT32P) (0x2670605c)) = 0x00000000;
     *((UINT32P) (0x26706070)) = 0x00000000;
     *((UINT32P) (0x2673604c)) = 0x00000000;
     *((UINT32P) (0x26736060)) = 0x00000000;
     *((UINT32P) (0x26726100)) = 0x00000000;
     *((UINT32P) (0x26726114)) = 0x00000000;
     *((UINT32P) (0x267261c8)) = 0x00000000;
     *((UINT32P) (0x26746048)) = 0x00000000;
     *((UINT32P) (0x26746088)) = 0x00000000;
     *((UINT32P) (0x26746058)) = 0x00000000;
     *((UINT32P) (0x26756048)) = 0x00000000;
     *((UINT32P) (0x26756088)) = 0x00000000;
     *((UINT32P) (0x26756058)) = 0x00000000;
     
     //lte_mbist_mode_deassert
     *((UINT32P) (0x26716000)) = 0x00000000;
     *((UINT32P) (0x26706000)) = 0x00000000;
     *((UINT32P) (0x26706004)) = 0x00000000;
     *((UINT32P) (0x26736000)) = 0x00000000;
     *((UINT32P) (0x26736004)) = 0x00000000;
     *((UINT32P) (0x26726000)) = 0x00000000;
     *((UINT32P) (0x26726004)) = 0x00000000;
     *((UINT32P) (0x26726008)) = 0x00000000;
     *((UINT32P) (0x26746000)) = 0x00000000;
     *((UINT32P) (0x26756000)) = 0x00000000;
     
     //lte_mbist_rstb_release
     *((UINT32P) (0x26716054)) = 0x00000001;
     *((UINT32P) (0x26716064)) = 0x00000001;
     *((UINT32P) (0x2670605c)) = 0x00000001;
     *((UINT32P) (0x26706070)) = 0x00000001;
     *((UINT32P) (0x2673604c)) = 0x00000001;
     *((UINT32P) (0x26736060)) = 0x00000001;
     *((UINT32P) (0x26726100)) = 0x00000001;
     *((UINT32P) (0x26726114)) = 0x00000001;
     *((UINT32P) (0x267261c8)) = 0x00000001;
     *((UINT32P) (0x26746048)) = 0x00000001;
     *((UINT32P) (0x26746088)) = 0x00000001;
     *((UINT32P) (0x26746058)) = 0x00000001;
     *((UINT32P) (0x26756048)) = 0x00000001;
     *((UINT32P) (0x26756088)) = 0x00000001;
     *((UINT32P) (0x26756058)) = 0x00000001;
     
     //lte_mbist_mode_assert
     *((UINT32P) (0x26716000)) = 0xffffffff;
     *((UINT32P) (0x26706000)) = 0xffffffff;
     *((UINT32P) (0x26706004)) = 0x00007fff;
     *((UINT32P) (0x26736000)) = 0x0000ffff;
     *((UINT32P) (0x26736004)) = 0x00000001;
     *((UINT32P) (0x26726000)) = 0xffffffff;
     *((UINT32P) (0x26726004)) = 0xffffffff;
     *((UINT32P) (0x26726008)) = 0x00000007;
     *((UINT32P) (0x26746000)) = 0xffffffff;
     *((UINT32P) (0x26756000)) = 0xffffffff;
     
     //wait lte_done
     udelay(2000);
     
     //read done
     if (*((UINT32P) (0x26716034)) != 0xffffffff){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26716038)) != 0x00000001){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x2670603c)) != 0xffffffff){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26706040)) != 0x001fffff){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26736034)) != 0x0001ffff){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26736038)) != 0x00000001){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26726060)) != 0xffffffff){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26726064)) != 0xffffffff){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26726068)) != 0x0000000f){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26746030)) != 0x0fffffff){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26756030)) != 0x3fffffff){
     		printf("lte mbist is not ready %d\n", __LINE__);
     		ret = -1;
     }
     
     //read fail
     if (*((UINT32P) (0x26716068)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26706074)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26706078)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26736064)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26726120)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x2674605c)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x2675605c)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x2671606c)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26706084)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26706088)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x2673606c)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26726134)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26746064)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     if (*((UINT32P) (0x26756064)) != 0x00000000){
     		printf("lte mbist repair fail %d\n", __LINE__);
     		ret = -1;
     }
     
     //load fuse
     *((UINT32P) (0x26612060)) = *((UINT32P) (0x26612060)) | 0x5a5a0001;
     *((UINT32P) (0x26612060)) = *((UINT32P) (0x26612060)) & 0xfffffffe;
     
     *((UINT32P) (0x266020cc)) = *((UINT32P) (0x266020cc)) | 0x5a5a0001;
     *((UINT32P) (0x266020cc)) = *((UINT32P) (0x266020cc)) & 0xfffffffe;
     
     *((UINT32P) (0x2663206c)) = *((UINT32P) (0x2663206c)) | 0x5a5a0001;
     *((UINT32P) (0x2663206c)) = *((UINT32P) (0x2663206c)) & 0xfffffffe;
     
     *((UINT32P) (0x26622110)) = *((UINT32P) (0x26622110)) | 0x5a5a0001;
     *((UINT32P) (0x26622110)) = *((UINT32P) (0x26622110)) & 0xfffffffe;
     
     *((UINT32P) (0x26642080)) = *((UINT32P) (0x26642080)) | 0x5a5a0001;
     *((UINT32P) (0x26642080)) = *((UINT32P) (0x26642080)) & 0xfffffffe;
     
     *((UINT32P) (0x26652078)) = *((UINT32P) (0x26652078)) | 0x5a5a0001;
     *((UINT32P) (0x26652078)) = *((UINT32P) (0x26652078)) & 0xfffffffe;

     return ret;
}


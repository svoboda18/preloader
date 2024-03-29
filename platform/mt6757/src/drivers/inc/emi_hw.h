#ifndef __EMI_HW_H__
#define __EMI_HW_H__

//typedef volatile unsigned int *      P_U32;
#define         EMI_CONA                (( P_U32)(EMI_BASE+0x000))
#define         EMI_CONB                (( P_U32)(EMI_BASE+0x008))
#define         EMI_CONC                (( P_U32)(EMI_BASE+0x010))
#define         EMI_COND                (( P_U32)(EMI_BASE+0x018))
#define         EMI_CONE                (( P_U32)(EMI_BASE+0x020))
#define         EMI_CONF                (( P_U32)(EMI_BASE+0x028))
#define         EMI_CONG                (( P_U32)(EMI_BASE+0x030))
#define         EMI_CONH                (( P_U32)(EMI_BASE+0x038))
#define         EMI_CONH_2ND            (( P_U32)(EMI_BASE+0x03C))
#define         EMI_CONI                (( P_U32)(EMI_BASE+0x040))
#define         EMI_CONJ                (( P_U32)(EMI_BASE+0x048))
#define         EMI_CONM                (( P_U32)(EMI_BASE+0x060))
#define         EMI_CONN                (( P_U32)(EMI_BASE+0x068))
#define         EMI_CONO                (( P_U32)(EMI_BASE+0x070))
#define         EMI_MDCT                (( P_U32)(EMI_BASE+0x078))
#define         EMI_MDCT_2ND            (( P_U32)(EMI_BASE+0x07C))
#define         EMI_LPCT0               (( P_U32)(EMI_BASE+0x0A0))
#define         EMI_LPCT1               (( P_U32)(EMI_BASE+0x0A4))
#define         EMI_LPCT2               (( P_U32)(EMI_BASE+0x0A8))
#define         EMI_LPCT5               (( P_U32)(EMI_BASE+0x0B4))
#define         EMI_LPCT6               (( P_U32)(EMI_BASE+0x0B8))
#define         EMI_IOCL                (( P_U32)(EMI_BASE+0x0D0))
#define         EMI_IOCL_2ND            (( P_U32)(EMI_BASE+0x0D4))
#define         EMI_IOCM                (( P_U32)(EMI_BASE+0x0D8))
#define         EMI_IOCM_2ND            (( P_U32)(EMI_BASE+0x0DC))
#define         EMI_TESTB               (( P_U32)(EMI_BASE+0x0E8))
#define         EMI_TESTC               (( P_U32)(EMI_BASE+0x0F0))
#define         EMI_TESTD               (( P_U32)(EMI_BASE+0x0F8))
#define         EMI_ARBA                (( P_U32)(EMI_BASE+0x100))
#define         EMI_ARBB                (( P_U32)(EMI_BASE+0x108))
#define         EMI_ARBC                (( P_U32)(EMI_BASE+0x110))
#define         EMI_ARBD                (( P_U32)(EMI_BASE+0x118))
#define         EMI_ARBE                (( P_U32)(EMI_BASE+0x120))
#define         EMI_ARBF                (( P_U32)(EMI_BASE+0x128))
#define         EMI_ARBG                (( P_U32)(EMI_BASE+0x130))
#define         EMI_ARBH                (( P_U32)(EMI_BASE+0x138))
#define         EMI_ARBI                (( P_U32)(EMI_BASE+0x140))
#define         EMI_ARBI_2ND            (( P_U32)(EMI_BASE+0x144))
#define         EMI_ARBK                (( P_U32)(EMI_BASE+0x150))
#define         EMI_ARBK_2ND            (( P_U32)(EMI_BASE+0x154))
#define         EMI_SLCT                (( P_U32)(EMI_BASE+0x158))
#define         EMI_MPUD0_ST            (( P_U32)(EMI_BASE+0x160))
#define         EMI_MPUD1_ST            (( P_U32)(EMI_BASE+0x164))
#define         EMI_MPUD2_ST            (( P_U32)(EMI_BASE+0x168))
#define         EMI_MPUD3_ST            (( P_U32)(EMI_BASE+0x16C))
#define         EMI_MPUD4_ST            (( P_U32)(EMI_BASE+0x170))
#define         EMI_MPUD5_ST            (( P_U32)(EMI_BASE+0x174))
#define         EMI_MPUD6_ST            (( P_U32)(EMI_BASE+0x178))
#define         EMI_MPUD7_ST            (( P_U32)(EMI_BASE+0x17C))
#define         EMI_MPUS                (( P_U32)(EMI_BASE+0x1F0))
#define         EMI_MPUT                (( P_U32)(EMI_BASE+0x1F8))
#define         EMI_MPUT_2ND            (( P_U32)(EMI_BASE+0x1FC))
#define         EMI_D0_ST2              (( P_U32)(EMI_BASE+0x200))
#define         EMI_D1_ST2              (( P_U32)(EMI_BASE+0x204))
#define         EMI_D2_ST2              (( P_U32)(EMI_BASE+0x208))
#define         EMI_D3_ST2              (( P_U32)(EMI_BASE+0x20C))
#define         EMI_D4_ST2              (( P_U32)(EMI_BASE+0x210))
#define         EMI_D5_ST2              (( P_U32)(EMI_BASE+0x214))
#define         EMI_D6_ST2              (( P_U32)(EMI_BASE+0x218))
#define         EMI_D7_ST2              (( P_U32)(EMI_BASE+0x21C))
#define         EMI_BMEN                (( P_U32)(EMI_BASE+0x400))
#define         EMI_BSTP                (( P_U32)(EMI_BASE+0x404))
#define         EMI_BCNT                (( P_U32)(EMI_BASE+0x408))
#define         EMI_TACT                (( P_U32)(EMI_BASE+0x410))
#define         EMI_TSCT                (( P_U32)(EMI_BASE+0x418))
#define         EMI_WACT                (( P_U32)(EMI_BASE+0x420))
#define         EMI_WSCT                (( P_U32)(EMI_BASE+0x428))
#define         EMI_BACT                (( P_U32)(EMI_BASE+0x430))
#define         EMI_BSCT                (( P_U32)(EMI_BASE+0x438))
#define         EMI_MSEL                (( P_U32)(EMI_BASE+0x440))
#define         EMI_TSCT2               (( P_U32)(EMI_BASE+0x448))
#define         EMI_TSCT3               (( P_U32)(EMI_BASE+0x450))
#define         EMI_WSCT2               (( P_U32)(EMI_BASE+0x458))
#define         EMI_WSCT3               (( P_U32)(EMI_BASE+0x460))
#define         EMI_WSCT4               (( P_U32)(EMI_BASE+0x464))
#define         EMI_MSEL2               (( P_U32)(EMI_BASE+0x468))
#define         EMI_MSEL3               (( P_U32)(EMI_BASE+0x470))
#define         EMI_MSEL4               (( P_U32)(EMI_BASE+0x478))
#define         EMI_MSEL5               (( P_U32)(EMI_BASE+0x480))
#define         EMI_MSEL6               (( P_U32)(EMI_BASE+0x488))
#define         EMI_MSEL7               (( P_U32)(EMI_BASE+0x490))
#define         EMI_MSEL8               (( P_U32)(EMI_BASE+0x498))
#define         EMI_MSEL9               (( P_U32)(EMI_BASE+0x4A0))
#define         EMI_MSEL10              (( P_U32)(EMI_BASE+0x4A8))
#define         EMI_BMID0               (( P_U32)(EMI_BASE+0x4B0))
#define         EMI_BMID1               (( P_U32)(EMI_BASE+0x4B4))
#define         EMI_BMID2               (( P_U32)(EMI_BASE+0x4B8))
#define         EMI_BMID3               (( P_U32)(EMI_BASE+0x4BC))
#define         EMI_BMID4               (( P_U32)(EMI_BASE+0x4C0))
#define         EMI_BMID5               (( P_U32)(EMI_BASE+0x4C4))
#define         EMI_BMID6               (( P_U32)(EMI_BASE+0x4C8))
#define         EMI_BMID7               (( P_U32)(EMI_BASE+0x4CC))
#define         EMI_BMID8               (( P_U32)(EMI_BASE+0x4D0))
#define         EMI_BMID9               (( P_U32)(EMI_BASE+0x4D4))
#define         EMI_BMID10              (( P_U32)(EMI_BASE+0x4D8))
#define         EMI_BMEN1               (( P_U32)(EMI_BASE+0x4E0))
#define         EMI_BMEN2               (( P_U32)(EMI_BASE+0x4E8))
#define         EMI_BMRW0               (( P_U32)(EMI_BASE+0x4F8))
#define         EMI_BMRW1               (( P_U32)(EMI_BASE+0x4FC))
#define         EMI_TTYPE1              (( P_U32)(EMI_BASE+0x500))
#define         EMI_TTYPE2              (( P_U32)(EMI_BASE+0x508))
#define         EMI_TTYPE3              (( P_U32)(EMI_BASE+0x510))
#define         EMI_TTYPE4              (( P_U32)(EMI_BASE+0x518))
#define         EMI_TTYPE5              (( P_U32)(EMI_BASE+0x520))
#define         EMI_TTYPE6              (( P_U32)(EMI_BASE+0x528))
#define         EMI_TTYPE7              (( P_U32)(EMI_BASE+0x530))
#define         EMI_TTYPE8              (( P_U32)(EMI_BASE+0x538))
#define         EMI_TTYPE9              (( P_U32)(EMI_BASE+0x540))
#define         EMI_TTYPE10             (( P_U32)(EMI_BASE+0x548))
#define         EMI_TTYPE11             (( P_U32)(EMI_BASE+0x550))
#define         EMI_TTYPE12             (( P_U32)(EMI_BASE+0x558))
#define         EMI_TTYPE13             (( P_U32)(EMI_BASE+0x560))
#define         EMI_TTYPE14             (( P_U32)(EMI_BASE+0x568))
#define         EMI_TTYPE15             (( P_U32)(EMI_BASE+0x570))
#define         EMI_TTYPE16             (( P_U32)(EMI_BASE+0x578))
#define         EMI_TTYPE17             (( P_U32)(EMI_BASE+0x580))
#define         EMI_TTYPE18             (( P_U32)(EMI_BASE+0x588))
#define         EMI_TTYPE19             (( P_U32)(EMI_BASE+0x590))
#define         EMI_TTYPE20             (( P_U32)(EMI_BASE+0x598))
#define         EMI_TTYPE21             (( P_U32)(EMI_BASE+0x5A0))
#define         EMI_BWCT0               (( P_U32)(EMI_BASE+0x5B0))
#define         EMI_BWST0               (( P_U32)(EMI_BASE+0x5C4))
#define         EMI_EX_CON              (( P_U32)(EMI_BASE+0x5D0))
#define         EMI_EX_ST0              (( P_U32)(EMI_BASE+0x5D4))
#define         EMI_EX_ST1              (( P_U32)(EMI_BASE+0x5D8))
#define         EMI_EX_ST2              (( P_U32)(EMI_BASE+0x5DC))
#define         EMI_WP_ADR              (( P_U32)(EMI_BASE+0x5E0))
#define         EMI_WP_ADR_2ND          (( P_U32)(EMI_BASE+0x5E4))
#define         EMI_WP_CTRL             (( P_U32)(EMI_BASE+0x5E8))
#define         EMI_CHKER               (( P_U32)(EMI_BASE+0x5F0))
#define         EMI_CHKER_TYPE          (( P_U32)(EMI_BASE+0x5F4))
#define         EMI_CHKER_ADR           (( P_U32)(EMI_BASE+0x5F8))
#define         EMI_CHKER_ADR_2ND       (( P_U32)(EMI_BASE+0x5FC))
#define         EMI_SLVA                (( P_U32)(EMI_BASE+0x800))

#define         EMI_MPU_CTRL                 (EMI_MPU_BASE+0x000)
#define         EMI_MPU_DBG                  (EMI_MPU_BASE+0x004)
#define         EMI_MPU_SA0                  (EMI_MPU_BASE+0x100)
#define         EMI_MPU_EA0                  (EMI_MPU_BASE+0x200)
#define         EMI_MPU_SA(region)           (EMI_MPU_SA0 + (region*4))
#define         EMI_MPU_EA(region)           (EMI_MPU_EA0 + (region*4))
#define         EMI_MPU_APC0                 (EMI_MPU_BASE+0x300)
#define         EMI_MPU_APC(region, domain)  (EMI_MPU_APC0 + (region*4) + ((domain/8)*0x100))
#define         EMI_MPU_CTRL_D0              (EMI_MPU_BASE+0x800)
#define         EMI_MPU_CTRL_D(domain)       (EMI_MPU_CTRL_D0 + (domain*4))
#define         EMI_RG_MASK_D0               (EMI_MPU_BASE+0x900)
#define         EMI_RG_MASK_D(domain)        (EMI_RG_MASK_D0 + (domain*4))

#define EMI_MPU_REGION_NUMBER	(24)
#define EMI_MPU_DOMAIN_NUMBER	(8)

#endif	// __EMI_HW_H__

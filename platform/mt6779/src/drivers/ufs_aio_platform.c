/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2015. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#include "ufs_aio_cfg.h"
#include "ufs_aio_platform.h"
#include "ufs_aio_hcd.h"
#include "ufs_aio_utils.h"
#if defined(MTK_UFS_DRV_DA)
#include <dev/irq/irq.h>        /* for MT_UFS_IRQ_ID */
#elif defined(MTK_UFS_DRV_LK)
#include <platform/mt_irq.h>
#endif
#ifdef MTK_UFS_DRV_PRELOADER
#include "pal_log.h"

#ifdef UFS_CFG_HQA_MODE
void ufs_aio_hqa_mode(void)
{
	pal_log_info("[UFS] ------- HQA or MPHY Debugging Mode ------");
}
#endif
#endif
//-------------------------------------------------------------------------
/** Round_Operation
 *  Round operation of A/B
 *  @param  A
 *  @param  B
 *  @retval round(A/B)
 */
//-------------------------------------------------------------------------
static unsigned int ufs_round(unsigned int A, unsigned int B)
{
    unsigned int temp;

    if (B == 0)
    {
        return 0xffffffff;
    }

    temp = A/B;

    if ((A-temp*B) >= ((temp+1)*B-A))
    {
        return (temp+1);
    }
    else
    {
        return temp;
    }
}

int ufs_aio_init_mphy(struct ufs_hba *hba)
{
	unsigned int N;
	unsigned int reg = 0;
	unsigned int temp = 0;

	/*
	Support t12 UFS TX Deemphasis by Software.
	RG_MP_TX_RDN_TRIM_CODE		0x11FA9000  ,  Bit [12:8]
	RG_MP_TX0_DRV_MASTER_3P5DB	0x11FA9008  ,  Bit [4:0]
	RG_MP_TX0_DRV_SLAVE_3P5DB	0x11FA9008  ,  Bit [7:5]
	RG_MP_TX0_DRV_MASTER_6DB	0x11FA9008  ,  Bit [12:8]
	RG_MP_TX0_DRV_SLAVE_6DB	0x11FA9008  ,  Bit [15:13]
	*/

	/* N = RG_MP_TX0_RDN_TRIM_CODE<12:8> + 9 */
	reg = readl(0x11fa9000);
	N = ((reg & (0x00001F00)) >> 8) + 9;

	/* RG_MP_TX0_DRV_MASTER_3P5DB = round(N*(5/6)) - 9 */
	temp = ufs_round(N*5, 6) - 9;
	reg = temp & 0x0000001F;

	/* RG_MP_TX0_DRV_SLAVE_3P5DB = N - round(N*(5/6)) - 3 */
	temp = N - ufs_round(N*5, 6) - 3;
	reg |= ((temp << 5) & 0x000000E0);

	/* RG_MP_TX0_DRV_MASTER_6DB = round(N*(3/4)) - 9 */
	temp = ufs_round(N*3, 4) - 9;
	reg |= ((temp << 8) & 0x00001F00);

	/* RG_MP_TX0_DRV_SLAVE_6DB = N - round(N*(3/4)) - 3 */
	temp = N - ufs_round(N*3, 4) - 3;
	reg |= ((temp << 13) & 0x0000E000);

	writel(((readl(0x11fa9008) & 0xFFFF0000) | reg), 0x11fa9008);

	return 0;
}

#if defined(MTK_UFS_DRV_DA) || defined(MTK_UFS_DRV_LK)
int ufs_get_irq_id(void)
{
	return MT_UFS_IRQ_ID;
}
#endif


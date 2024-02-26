/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

/*
 * HEADER FILES
 */
#include "typedefs.h"
#include "platform.h"
#include "usbd.h"
#include "usbtty.h"
#include "usbphy.h"

#include "ssusb_sifslv_ippc_c_header.h"
#include "ssusb_usb3_mac_csr_c_header.h"

/*
 * USB DEBUG
 */
#define  USBD_DBG_LOG   1
#define  MOD_TAG	"[U]"

u32 isUSBDebug=0;

#define USB_ERR_LOG(_format, ...)  do{ \
    if (isUSBDebug != 0)\
	printf(MOD_TAG "[ERR][%d] " #_format "\n", __LINE__, ##__VA_ARGS__);\
	} while(0)
#if USBD_DBG_LOG
	#define USB_LOG(_format, ...) do{  \
	    if (isUSBDebug != 0)\
		printf(MOD_TAG " " #_format "\n", ##__VA_ARGS__);\
		} while(0)
	#define USB_BUG() do { \
	    if (isUSBDebug != 0)\
		printf("U-Boot BUG at %s:%d!\n", __FILE__, __LINE__); \
		} while (0)
	#define USB_BUG_ON(condition) do { if ((condition)==FALSE) BUG(); } while(0)
#else
	#define USB_LOG
	#define USB_BUG() do {} while (0)
	#define USB_BUG_ON(condition) do {} while(0)
#endif

/*
 * GLOBAL VARIABLES
 */
static struct urb *gp_ep0_urb = NULL;
static struct mt_dev *gp_udc_dev = NULL;
EP0_STATE ge_ep0_state = EP0_IDLE;
int g_set_address = 0;
int g_usbphy_ok = 0;
int g_usb_port_state = 0;
u32 g_tx_fifo_addr = USB_TX_FIFO_START_ADDRESS;
u32 g_rx_fifo_addr = USB_RX_FIFO_START_ADDRESS;
int g_enable_u3 = 0;

extern struct string_descriptor **usb_string_table;

static struct usb_acm_line_coding g_line_coding = {
	921600, 0, 0, 8,
};

/*
 * STATIC FUNCTION DECLARATIONS
 */
static void copy_desc(struct urb *urb, void *data, int max_length,
					   int max_buf);
static int usb_get_descriptor(struct mt_dev *device,
struct urb *urb, int max, int descriptor_type,
	int index);
static int ep0_standard_setup(struct urb *urb);
static int ep0_class_setup(struct urb *urb);
static int mt_read_fifo(struct mt_ep *endpoint);
static int mt_write_fifo(struct mt_ep *endpoint);
static struct mt_ep *mt_find_ep(int ep_num, u8 dir);
static void mt_udc_flush_fifo(u8 ep_num, u8 dir);
static void mt_udc_reset(USB_SPEED speed);
static void mt_udc_ep0_write(void);
static void mt_udc_ep0_read(void);
static void mt_udc_ep0_setup(void);
static void mt_udc_ep0_handler(void);
static void mt_udc_epx_handler(u8 ep_num, u8 dir);
static void udc_stall_ep(unsigned int ep_num, u8 dir);
static void USB_UART_Share(u8 usb_mode);
static void USB_Charger_Detect_Init(void);
static void USB_Charger_Detect_Release(void);
static void USB_Check_Standard_Charger(void);

extern void mt_usb_phy_poweron(void);
extern void mt_usb_phy_savecurrent(void);
extern void mt_usb_phy_recover(void);

extern u32 get_max_packet_size(int enableU3);

static void dump_setup_packet(char *str, struct device_request *p_req)
{
	USB_LOG(str);
	USB_LOG("    bmRequestType = %x", p_req->bmRequestType);
	USB_LOG("    bRequest = %x", p_req->bRequest);
	USB_LOG("    wValue = %x", p_req->wValue);
	USB_LOG("    wIndex = %x", p_req->wIndex);
	USB_LOG("    wLength = %x\n", p_req->wLength);
}

void explain_csr0(u32 csr0)
{
#if USBD_DBG_LOG
	if (csr0 & EP0_EP_RESET) {
		USB_LOG("EP0_EP_RESET is set");
	}
	if (csr0 & EP0_AUTOCLEAR) {
		USB_LOG("EP0_AUTOCLEAR is set");
	}
	if (csr0 & EP0_AUTOSET) {
		USB_LOG("EP0_AUTOSET is set");
	}
	if (csr0 & EP0_DMAREQEN) {
		USB_LOG("EP0_DMAREQEN is set");
	}
	if (csr0 & EP0_SENDSTALL) {
		USB_LOG("EP0_SENDSTALL is set");
	}
	if (csr0 & EP0_FIFOFULL) {
		USB_LOG("EP0_FIFOFULL is set");
	}
	if (csr0 & EP0_SENTSTALL) {
		USB_LOG("EP0_SENTSTALL is set");
	}
	if (csr0 & EP0_DPHTX) {
		USB_LOG("EP0_DPHTX is set");
	}
	if (csr0 & EP0_DATAEND) {
		USB_LOG("EP0_DATAEND is set");
	}
	if (csr0 & EP0_TXPKTRDY) {
		USB_LOG("EP0_TXPKTRDY is set");
	}
	if (csr0 & EP0_SETUPPKTRDY) {
		USB_LOG("EP0_SETUPPKTRDY is set");
	}
	if (csr0 & EP0_RXPKTRDY) {
		USB_LOG("EP0_RXPKTRDY is set");
	}
#endif
}

static void udc_chg_ep0_state(EP0_STATE eState)
{
	u32 csr0 = __raw_readl(U3D_EP0CSR);
	u32 temp = 0;

	USB_LOG(" csr0 is %x b4 %s", csr0, __func__);

	if ((ge_ep0_state == EP0_IDLE) && (eState == EP0_TX)) {
		/* IDLE -> TX */
		USB_LOG("	set (EP0_SETUPPKTRDY | EP0_DPHTX)");
		temp |= (EP0_SETUPPKTRDY | EP0_DPHTX);
	} else if ((ge_ep0_state == EP0_IDLE) && (eState == EP0_RX)) {
		/* IDLE -> RX */
		USB_LOG("	set EP0_SETUPPKTRDY");
		temp |= EP0_SETUPPKTRDY;
	} else if ((ge_ep0_state == EP0_TX) && (eState == EP0_TX)) {
		/* TX -> TX */
		USB_LOG("	set EP0_TXPKTRDY");
		temp |= EP0_TXPKTRDY;
	} else if ((ge_ep0_state == EP0_RX) && (eState == EP0_RX)) {
		/* RX -> RX */
		//USB_LOG("	set EP0_RXPKTRDY");
		//temp |= EP0_RXPKTRDY;
		goto NO_SET_REG;
	} else if ((ge_ep0_state == EP0_RX) && (eState == EP0_IDLE)) {
		/* RX -> IDLE */
		USB_LOG("	set (EP0_RXPKTRDY| EP0_DATAEND)");
		temp |= (EP0_RXPKTRDY| EP0_DATAEND);
	} else if ((ge_ep0_state == EP0_TX) && (eState == EP0_IDLE)) {
		/* TX -> IDLE */
		//USB_LOG("	set (EP0_TXPKTRDY | EP0_DATAEND)");
		//temp |= (EP0_TXPKTRDY | EP0_DATAEND);
		goto NO_SET_REG;
	} else if ((ge_ep0_state == EP0_IDLE) && (eState == EP0_IDLE)) {
		/* IDLE -> IDLE */
		//USB_LOG("	set EP0_SETUPPKTRDY");
		//temp |= EP0_SETUPPKTRDY;
		goto NO_SET_REG;
	}

	USB_EP0CSR_SETMASK(temp);

NO_SET_REG:
	USB_LOG (" Change EP0 state from %d to %d", ge_ep0_state, eState);
	ge_ep0_state = eState;
}

static void copy_desc(struct urb *p_urb, void *p_data, int max_length, int max_buf)
{
	int available;
	int length;

	length = *(unsigned char *) p_data;

	if (length > max_length)
		length = max_length;

	if ((available = max_buf - p_urb->actual_length) <= 0) {
		return;
	}

	if (length > available) {
		length = available;
	}

	memcpy (p_urb->p_buf + p_urb->actual_length, p_data, length);
	p_urb->actual_length += length;
}

static int copy_all_descs(
	int index,
	struct mt_dev *device,
	struct urb *p_urb,
	int max)
{
	int interfaceNum = 0;
	int configNum = 0;
	struct mt_config *p_config = NULL;
	struct configuration_descriptor *p_config_descriptor = NULL;

	configNum = (index == 0 ? 0 : index - 1);
	if (configNum >= device->configurations) {
		USB_ERR_LOG("configNum (%d) >= device->configurations %d",
			configNum, device->configurations);
		return -1;
	}

	if (p_config = device->p_conf_array + configNum) {
		p_config_descriptor = p_config->configuration_descriptor;
	} else {
		USB_ERR_LOG("p_config is NULL");
		return -1;
	}

	copy_desc(p_urb, p_config_descriptor, sizeof (*p_config_descriptor), max);

	/* loop over all interfaces */
	for (interfaceNum = 0;
		interfaceNum < p_config_descriptor->bNumInterfaces;
		++interfaceNum)
	{
		int alternateNum = 0;
		struct mt_intf *p_interface = NULL;
		struct interface_descriptor *p_intf_descriptor = NULL;

		p_interface = p_config->interface_array[interfaceNum];

		if (!p_interface) {
			USB_ERR_LOG("p_interface is NULL");
			return -1;
		}

		/* loop over all interface alternates */
		for (alternateNum = 0;
			alternateNum < p_interface->alternates; ++alternateNum)
		{
			int endpoint = 0;
			struct mt_altsetting *p_altsetting = NULL;

			if (!(p_altsetting = p_interface->altsetting_array + alternateNum)) {
				USB_ERR_LOG("p_altsetting is NULL");
				return -1;
			}

			if (!(p_intf_descriptor = p_altsetting->p_interface_desc)) {
				USB_ERR_LOG("p_intf_descriptor is NULL");
				return -1;
			}

			/* copy descriptor for this interface */
			copy_desc (p_urb, p_intf_descriptor,
				sizeof (*p_intf_descriptor), max);

			if (p_altsetting->p_header_function_desc) {
				copy_desc (p_urb, p_altsetting->p_header_function_desc,
					sizeof (*p_altsetting->p_header_function_desc), max);
			}

			if (p_altsetting->p_abstract_control_desc) {
				copy_desc (p_urb, p_altsetting->p_abstract_control_desc,
					sizeof (*p_altsetting->p_abstract_control_desc), max);
			}

			if (p_altsetting->p_union_function_desc) {
				copy_desc (p_urb, p_altsetting->p_union_function_desc,
					sizeof (*p_altsetting->p_union_function_desc), max);
			}

			if (p_altsetting->p_call_management_desc) {
				copy_desc (p_urb, p_altsetting->p_call_management_desc,
					sizeof (*p_altsetting->p_call_management_desc), max);
			}

			/* iterate across endpoints for this alternate interface */
			for (endpoint = 0; endpoint < p_altsetting->endpoints; ++endpoint) {
				struct endpoint_descriptor *p_ep_desc = NULL;
				struct ss_ep_comp_descriptor * p_ep_comp =
					p_altsetting->p_ss_ep_comp_desc;

				p_ep_desc = *(p_altsetting->pp_eps_desc_array + endpoint);
				if (NULL == p_ep_desc) {
					USB_ERR_LOG("p_ep_desc is NULL");
					return -1;
				}

				/* copy descriptor for this endpoint */
				copy_desc (p_urb, p_ep_desc, sizeof (*p_ep_desc), max);

				if (SSUSB_SPEED_SUPER == device->speed)
					copy_desc(p_urb, p_ep_comp, sizeof(*p_ep_comp), max);
			}
		}
	}
	return 0;
}

static int usb_get_descriptor(
	struct mt_dev *p_dev,
	struct urb *p_urb,
	int max,
	int descriptor_type,
	int index)
{
	if (!p_urb || !p_urb->p_buf || !p_urb->buffer_length
		|| (p_urb->buffer_length < 255 || !p_dev)) {
		return -1L;
	}

	/* setup tx urb */
	p_urb->actual_length = 0;

	switch (descriptor_type)
	{
	case USB_DESCRIPTOR_TYPE_DEVICE:
		USB_LOG("Get USB_DESCRIPTOR_TYPE_DEVICE");
		{
			struct device_descriptor *p_dev_desc;

			if (!(p_dev_desc = p_dev->p_dev_desc)) {
				return -1;
			}
			/* copy device descriptor for this device */
			copy_desc (p_urb, p_dev_desc, sizeof (*p_dev_desc), max);
		}
		break;
	case USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER:
		USB_LOG("Get USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER");
		{
			struct device_qualifier_descriptor *p_dev_q_desc;

			if (!(p_dev_q_desc = p_dev->p_dev_qualifier_desc)) {
				return -1;
			}

			/* copy device qualifier descriptor for this device */
			copy_desc (p_urb, p_dev_q_desc, sizeof (*p_dev_q_desc), max);
		}
		break;
	case USB_DESCRIPTOR_TYPE_CONFIGURATION:
		USB_LOG("Get USB_DESCRIPTOR_TYPE_CONFIGURATION");
		{
			if (0 != copy_all_descs(index, p_dev, p_urb, max)) {
				return -1;
			}
		}
		break;

	case USB_DESCRIPTOR_TYPE_STRING:
		USB_LOG("Get USB_DESCRIPTOR_TYPE_STRING");
		{
			struct string_descriptor *p_str_desc;

			p_str_desc = usb_string_table[index];
			copy_desc (p_urb, p_str_desc, p_str_desc->bLength, max);
		}
		break;

	case USB_DESCRIPTOR_TYPE_BOS:
		USB_LOG("Get USB_DESCRIPTOR_TYPE_BOS");
		{
			copy_desc(p_urb, p_dev->p_bos_desc,
				p_dev->p_bos_desc->bLength, max);
			copy_desc(p_urb, p_dev->p_ext_cap_desc,
				p_dev->p_ext_cap_desc->bLength, max);
			copy_desc(p_urb, p_dev->p_ss_cap_desc,
				p_dev->p_ss_cap_desc->bLength, max);
		}
		break;

	default:
		return -1;
	}

	return 0;
}

static void u3d_set_address(int addr)
{
	__raw_writel((addr << DEV_ADDR_OFST), U3D_DEVICE_CONF);
}

/* Service standard usb requests,
 * not all requests required for USBCV are
 * supported here.
 */
static int ep0_standard_setup(struct urb *p_urb)
{
	struct device_request *p_req = NULL;
	struct mt_dev *p_dev = NULL;
	char *p_buf = NULL;
	u8 ep_num = 0;


	if (!p_urb || !p_urb->p_dev) {
		USB_ERR_LOG ("!p_urb || !p_urb->p_dev");
		return -1;
	}

	p_buf = p_urb->p_buf;
	p_req = &p_urb->dev_request;
	p_dev = p_urb->p_dev;

	dump_setup_packet("[U]: Device Request\n", p_req);

	/* handle all requests that return data (direction bit set on bm RequestType) */
	if ((p_req->bmRequestType & USB_DIR_MASK) == USB_DIR_IN) {
		udc_chg_ep0_state(EP0_TX);

		switch (p_req->bRequest) {
		/* data stage: from device to host */
		case STDREQ_GET_STATUS:
			USB_LOG("STDREQ_GET_STATUS");
			p_urb->actual_length = 2;
			p_buf[0] = p_buf[1] = 0;
			switch (p_req->bmRequestType & USB_RECIP_MASK) {
			case USB_RECIP_DEVICE:
				p_buf[0] = USB_STAT_SELFPOWERED;
				break;
			case USB_RECIP_OTHER:
				p_urb->actual_length = 0;
				break;
			default:
				break;
			}
			break;

		case STDREQ_GET_DESCRIPTOR:
			USB_LOG("STDREQ_GET_DESCRIPTOR");
			return usb_get_descriptor (p_dev, p_urb,
				(p_req->wLength),
				(p_req->wValue) >> 8,
				(p_req->wValue) & 0xff);

		case STDREQ_GET_CONFIGURATION:
			USB_LOG("STDREQ_GET_CONFIGURATION");
			p_urb->actual_length = 1;
			((char *) p_urb->p_buf)[0] = p_dev->configuration;
			break;

		case STDREQ_GET_INTERFACE:
			USB_LOG("STDREQ_GET_INTERFACE");
			p_urb->actual_length = 1;
			((char *) p_urb->p_buf)[0] = p_dev->alternate;
			break;

		default:
			USB_ERR_LOG("Unsupported command %x at TX data stage\n"
				, p_req->bRequest);
			goto ERR;
			break;
		}
	}
	else {//if ((p_req->bmRequestType & USB_DIR_MASK) == USB_DIR_OUT)
		switch (p_req->bRequest) {
		case STDREQ_SET_ADDRESS:
			USB_LOG("STDREQ_SET_ADDRESS");
			p_dev->address = (p_req->wValue);
			g_set_address = 1;
			u3d_set_address(p_dev->address);
			break;

		case STDREQ_SET_CONFIGURATION:
			USB_LOG("STDREQ_SET_CONFIGURATION");
			p_dev->configuration = (p_req->wValue) & 0x7f;
			p_dev->interface = p_dev->alternate = 0;
			config_usbtty (p_dev);
			break;

		case STDREQ_CLEAR_FEATURE:
			USB_LOG("STDREQ_CLEAR_FEATURE");
			switch((p_req->bmRequestType & USB_RECIP_MASK)) {
			case USB_RECIP_ENDPOINT:
				{
					u8 dir = p_req->wIndex & USB_DIR_MASK;;

					ep_num = p_req->wIndex & 0x0f;
					USB_LOG("STDREQ_CLEAR_FEATURE,ep_num=%d, dir=%d",
						ep_num, dir);
					#ifdef DIAG_COMPOSITE_PRELOADER
					/* workaround for hangshake with PC tool*/
					if ((dir == USB_DIR_OUT)
					 &&(ep_num == USBD_DIAG_OUT_ENDPOINT)) {
						set_tool_exist();
					}
					#endif
				}
				break;
			default:
				USB_ERR_LOG("Unsupported bmRequestType");
				goto ERR;
				break;
			}
			break;

		case STDREQ_SET_FEATURE:
			USB_LOG("STDREQ_SET_FEATURE");
			break;

#if defined(SUPPORT_U3)
		case STDREQ_SET_SEL:
			USB_LOG("STDREQ_SET_SEL");
			udc_chg_ep0_state(EP0_RX);
			break;
#endif

		default:
			USB_ERR_LOG ("Unsupported command %x at RX data stage",
				p_req->bRequest);
			goto ERR;
			break;
		}
	}

	if (ge_ep0_state == EP0_IDLE) {
		USB_LOG("[%d] csr0 was %x b4 setting EP0_SETUPPKTRDY|EP0_DATAEND",__LINE__
			, __raw_readl(U3D_EP0CSR));
		USB_EP0CSR_SETMASK(EP0_SETUPPKTRDY|EP0_DATAEND);
	}

	return 0;

ERR:
	return -1;
}

static int ep0_class_setup (struct urb *p_urb)
{
	struct device_request *p_req;
	struct mt_dev *device;

	p_req = &p_urb->dev_request;
	device = p_urb->p_dev;

	switch (p_req->bRequest) {
		case CDCACM_REQ_SET_LINE_CODING:
			USB_LOG ("CDCACM_REQ_SET_LINE_CODING");
			udc_chg_ep0_state(EP0_RX);
			set_tool_exist();
			break;

		case CDCACM_REQ_GET_LINE_CODING:
			USB_LOG ("CDCACM_REQ_SET_LINE_CODING");
			memcpy (p_urb->p_buf, &g_line_coding, sizeof (g_line_coding));
			p_urb->actual_length = sizeof (g_line_coding);
			udc_chg_ep0_state(EP0_TX);
			break;

		case CDCACM_REQ_SET_CONTROL_LINE_STATE:
			USB_LOG ("CDCACM_REQ_GET_LINE_CODING, p_req->wValue=%x",
					 p_req->wValue);
			g_usb_port_state = p_req->wValue;
			break;

		case CDCACM_REQ_SEND_BREAK:
			USB_LOG ("CDCACM_REQ_SEND_BREAK");
			break;

		default:
			return -1;
	}

	if (ge_ep0_state == EP0_IDLE) {
		USB_LOG("csr0 was %x b4 setting EP0_SETUPPKTRDY|EP0_DATAEND",
			__raw_readl(U3D_EP0CSR));
		USB_EP0CSR_SETMASK(EP0_SETUPPKTRDY|EP0_DATAEND);
	}

	return 0;
}

/* mu3d hal related functions */
/* functinos used by mu3d hal functions */
int wait_for_value(int addr, unsigned int msk, int value, int ms_intvl, int count)
{
	int i;

	for (i = 0; i < count; i++) {
		if ((__raw_readl(addr) & msk) == value)
			return RET_SUCCESS;

		mdelay(ms_intvl);
	}

	return RET_FAIL;
}

/*
 * mu3d_hal_pio_read_fifo - pio read one packet
 * @args - arg1: ep number,  arg2: data buffer
 */
int mu3d_hal_pio_read_fifo(int ep_num,u8 *p_buf)
{
	u32 count = 0, residue = 0;
	u32 temp = 0;
 	u8 *p_tmpbuf = p_buf;

	if (ep_num == 0) {
		residue = count = __raw_readl(U3D_RXCOUNT0);
	} else {
		residue = count = USB_READCSR32(U3D_RX1CSR3, ep_num) >> 16;
	}

	while (residue > 0) {
		temp = __raw_readl(USB_FIFO(ep_num));

		*p_tmpbuf = temp&0xFF;
		if (residue > 1)
			*(p_tmpbuf + 1) = (temp >> 8)&0xFF;

		if (residue > 2)
			*(p_tmpbuf + 2) = (temp >> 16)&0xFF;

		if (residue > 3)
			*(p_tmpbuf + 3) = (temp >> 24)&0xFF;

		if (residue > 4) {
			p_tmpbuf = p_tmpbuf + 4;
			residue -= 4;
	   	} else {
			residue = 0;
		}
	}

	return count;
}

/*
 * mu3d_hal_pio_write_fifo - pio write one packet
 * @args - arg1: ep number,  arg2: data buffer
 */
int mu3d_hal_pio_write_fifo(int ep_num, int length, u8 *p_buf, int maxp)
{
	u32 residue = length;
	u32 temp = 0;

	USB_LOG("%s process ep_num: %d, length: %d, buf: %p, maxp: %d",
		__func__, ep_num, length, p_buf, maxp);

	while (residue > 0) {
		switch(residue) {
		case 1:
			temp = ((*p_buf) & 0xFF);
			__raw_writeb(temp, USB_FIFO(ep_num));
			p_buf += 1;
			residue -= 1;
			break;
		case 2:
			temp = ((*p_buf) & 0xFF)
				+ (((*(p_buf + 1)) << 8)&0xFF00);
			__raw_writew(temp, USB_FIFO(ep_num));
			p_buf += 2;
			residue -= 2;
			break;
		case 3:
			temp = ((*p_buf) & 0xFF)
				+ (((*(p_buf+1)) << 8)&0xFF00);
			__raw_writew(temp, USB_FIFO(ep_num));
			p_buf += 2;

			temp = ((*p_buf)&0xFF);
			__raw_writeb(temp, USB_FIFO(ep_num));
			p_buf += 1;
			residue -= 3;
			break;
		default:
			temp = ((*p_buf) & 0xFF)
				+ (((*(p_buf + 1)) <<  8)&0xFF00)
				+ (((*(p_buf + 2)) << 16)&0xFF0000)
				+ (((*(p_buf + 3)) << 24)&0xFF000000);
			__raw_writel(temp, USB_FIFO(ep_num));
			p_buf += 4;
			residue -= 4;
			break;
		};
	}

	return length;
}

/*
 * mu3d_hal_check_clk_sts - check sys125,u3 mac,u2 mac clock status
 */
int mu3d_hal_check_clk_sts(void)
{
	int ret;

	ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_SYS125_RST_B_STS, SSUSB_SYS125_RST_B_STS, 1, 10);
	if (ret == RET_FAIL) {
		USB_LOG("SSUSB_SYS125_RST_B_STS NG");
		goto CHECK_ERROR;
	} else {
		USB_LOG("clk sys125:OK");
	}

	ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_U3_MAC_RST_B_STS, SSUSB_U3_MAC_RST_B_STS, 1, 10);
	if (ret == RET_FAIL) {
		USB_LOG("SSUSB_U3_MAC_RST_B_STS NG");
		goto CHECK_ERROR;
	} else {
		USB_LOG("clk mac3:OK");
	}

	ret = wait_for_value(U3D_SSUSB_IP_PW_STS2, SSUSB_U2_MAC_SYS_RST_B_STS, SSUSB_U2_MAC_SYS_RST_B_STS, 1, 10);
	if (ret == RET_FAIL) {
		USB_LOG("SSUSB_U2_MAC_SYS_RST_B_STS NG");
		goto CHECK_ERROR;
	} else {
		USB_LOG("clk mac2:OK");
	}

	return RET_SUCCESS;

CHECK_ERROR:
	USB_LOG("Reference clock stability check failed!");
	return RET_FAIL;
}

/*
 * mu3d_hal_ssusb_en - disable ssusb power down & enable u2/u3 ports
 */
void mu3d_hal_ssusb_en(void)
{
	USB_CLRMASK(U3D_SSUSB_IP_PW_CTRL0, SSUSB_IP_SW_RST);
	USB_CLRMASK(U3D_SSUSB_IP_PW_CTRL2, SSUSB_IP_DEV_PDN);
//#if defined(SUPPORT_U3) || defined(CFG_FPGA_PLATFORM)
#if defined(SUPPORT_U3)
	USB_CLRMASK(U3D_SSUSB_U3_CTRL_0P, (SSUSB_U3_PORT_DIS | SSUSB_U3_PORT_PDN | SSUSB_U3_PORT_HOST_SEL));
#endif
	USB_CLRMASK(U3D_SSUSB_U2_CTRL_0P, (SSUSB_U2_PORT_DIS | SSUSB_U2_PORT_PDN | SSUSB_U2_PORT_HOST_SEL));

	USB_SETMASK(U3D_SSUSB_REF_CK_CTRL, (SSUSB_REF_MAC_CK_GATE_EN | SSUSB_REF_PHY_CK_GATE_EN | SSUSB_REF_CK_GATE_EN | SSUSB_REF_MAC3_CK_GATE_EN));

	/* check U3D sys125,u3 mac,u2 mac clock status. */
	mu3d_hal_check_clk_sts();
}

/*
 * mu3d_hal_u2dev_connect - u2 device softconnect
 */
void mu3d_hal_u2dev_connect(void)
{
	USB_SETMASK(U3D_POWER_MANAGEMENT, SOFT_CONN);
}

void mu3d_hal_u2dev_disconnect(void)
{
	USB_CLRMASK(U3D_POWER_MANAGEMENT, SOFT_CONN);
}

void mu3d_hal_u3dev_connect(void)
{
#ifdef SUPPORT_U3
	USB_SETMASK(U3D_USB3_CONFIG, USB3_EN);
	mdelay(40);
#endif
}

void mu3d_hal_u3dev_disconnect(void)
{
#ifdef SUPPORT_U3
	USB_CLRMASK(U3D_USB3_CONFIG, USB3_EN);
#endif
}

/*
 * mu3d_hal_set_speed - enable ss or connect to hs/fs
 *@args - arg1: speed
 */
void mu3d_hal_set_speed(USB_SPEED speed)
{
	/* clear ltssm state */
	switch(speed) {
	case SSUSB_SPEED_FULL:
		USB_CLRMASK(U3D_POWER_MANAGEMENT, HS_ENABLE);
		break;
	case SSUSB_SPEED_HIGH:
		enable_highspeed();
		USB_SETMASK(U3D_POWER_MANAGEMENT, HS_ENABLE);
		break;
#if defined(SUPPORT_U3)
	case SSUSB_SPEED_SUPER:
		enable_superspeed();
		USB_SETMASK(U3D_POWER_MANAGEMENT, HS_ENABLE);
		g_enable_u3 = true;
		break;
#endif
	default:
		USB_ERR_LOG("Unsupported speed %d!!", speed);
		break;
	};
}

void mu3d_hal_rst_dev(void)
{
	int ret;

	__raw_writel(SSUSB_DEV_SW_RST, U3D_SSUSB_DEV_RST_CTRL);
	__raw_writel(0, U3D_SSUSB_DEV_RST_CTRL);

	/* do not check when SSUSB_U2_PORT_DIS = 1, because U2 port stays in reset state */
	if (!(__raw_readl(U3D_SSUSB_U2_CTRL_0P) & SSUSB_U2_PORT_DIS))
	{
		ret = wait_for_value(U3D_SSUSB_IP_PW_STS2, SSUSB_U2_MAC_SYS_RST_B_STS, SSUSB_U2_MAC_SYS_RST_B_STS, 1, 10);
		if (ret == RET_FAIL)
			USB_LOG("[ERR]SSUSB_U2_MAC_SYS_RST_B_STS NG");
	}

#ifdef SUPPORT_U3
	/* do not check when SSUSB_U3_PORT_PDN = 1, because U3 port stays in reset state */
	if (!(__raw_readl(U3D_SSUSB_U3_CTRL_0P) & SSUSB_U3_PORT_PDN))
	{
		ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_U3_MAC_RST_B_STS, SSUSB_U3_MAC_RST_B_STS, 1, 10);
		if (ret == RET_FAIL)
			USB_LOG("[ERR]SSUSB_U3_MAC_RST_B_STS NG");
	}
#endif

	ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_DEV_QMU_RST_B_STS, SSUSB_DEV_QMU_RST_B_STS, 1, 10);
	if (ret == RET_FAIL)
		USB_LOG("[ERR][%d]SSUSB_DEV_QMU_RST_B_STS NG", __LINE__);

	ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_DEV_BMU_RST_B_STS, SSUSB_DEV_BMU_RST_B_STS, 1, 10);
	if (ret == RET_FAIL)
		USB_LOG("[ERR][%d]SSUSB_DEV_BMU_RST_B_STS NG", __LINE__);

	ret = wait_for_value(U3D_SSUSB_IP_PW_STS1, SSUSB_DEV_RST_B_STS, SSUSB_DEV_RST_B_STS, 1, 10);
	if (ret == RET_FAIL)
		USB_LOG("[ERR][%d]SSUSB_DEV_RST_B_STS NG", __LINE__);

	mdelay(50);
}

/*
void checkReferenceClk4PHYA()
{
    // for xtal clk
	__raw_writel(0x1, U3D_SSUSB_PRB_CTRL0);
	__raw_writel(0x8, U3D_SSUSB_PRB_CTRL1);
	__raw_writel(0x91, U3D_SSUSB_PRB_CTRL3);
	#define PHYD_ADDR 0x11280950
	__raw_writel((__raw_readl(PHYD_ADDR)&0x0000FFFF)|0x0E<<16, PHYD_ADDR);
    print("[EASON]PHYD_ADDR(0x%x) = 0x%x\n", PHYD_ADDR, DRV_Reg32(PHYD_ADDR));

	print("[EASON]U3D_SSUSB_PRB_CTRL0(0x%x) = 0x%x\n", U3D_SSUSB_PRB_CTRL0, DRV_Reg32(U3D_SSUSB_PRB_CTRL0));
	print("[EASON]U3D_SSUSB_PRB_CTRL1(0x%x) = 0x%x\n", U3D_SSUSB_PRB_CTRL1, DRV_Reg32(U3D_SSUSB_PRB_CTRL1));
	print("[EASON]U3D_SSUSB_PRB_CTRL3(0x%x) = 0x%x\n", U3D_SSUSB_PRB_CTRL3, DRV_Reg32(U3D_SSUSB_PRB_CTRL3));

	{
	    //Check bit[0] to see whether reference clk is toggling
	    int i = 0;
	    for (; i < 10; ++i){
	        print("[EASON][%d]U3D_SSUSB_PRB_CTRL5(0x%x) = 0x%x\n", i, U3D_SSUSB_PRB_CTRL5, DRV_Reg32(U3D_SSUSB_PRB_CTRL5));
	    }
	}
}
*/

/*
 * mu3d_hal_system_intr_en - enable system global interrupt
 */
void mu3d_hal_system_intr_en(void)
{
	u32 int_en = 0;
	u32 ltssm_int_en = 0;

	__raw_writel(__raw_readl(U3D_EPIER), U3D_EPIECR);
	__raw_writel(__raw_readl(U3D_DMAIER), U3D_DMAIECR);

	/* clear and enable common USB interrupts */
	__raw_writel(0, U3D_COMMON_USB_INTR_ENABLE);
	__raw_writel(__raw_readl(U3D_COMMON_USB_INTR), U3D_COMMON_USB_INTR);
	int_en = SUSPEND_INTR_EN | RESUME_INTR_EN | RESET_INTR_EN | CONN_INTR_EN |
		DISCONN_INTR_EN  | VBUSERR_INTR_EN | LPM_INTR_EN | LPM_RESUME_INTR_EN;
	__raw_writel(int_en, U3D_COMMON_USB_INTR_ENABLE);

#ifdef SUPPORT_U3
	/* clear and enable LTSSM interrupts */
	__raw_writel(0, U3D_LTSSM_INTR_ENABLE);
	__raw_writel(__raw_readl(U3D_LTSSM_INTR), U3D_LTSSM_INTR);
	ltssm_int_en = SS_INACTIVE_INTR_EN | SS_DISABLE_INTR_EN | COMPLIANCE_INTR_EN |
		LOOPBACK_INTR_EN  | HOT_RST_INTR_EN | WARM_RST_INTR_EN | RECOVERY_INTR_EN |
		ENTER_U0_INTR_EN | ENTER_U1_INTR_EN | ENTER_U2_INTR_EN | ENTER_U3_INTR_EN |
		EXIT_U1_INTR_EN | EXIT_U2_INTR_EN | EXIT_U3_INTR_EN | RXDET_SUCCESS_INTR_EN |
		VBUS_RISE_INTR_EN | VBUS_FALL_INTR_EN | U3_LFPS_TMOUT_INTR_EN |
		U3_RESUME_INTR_EN;
	__raw_writel(ltssm_int_en, U3D_LTSSM_INTR_ENABLE);
#endif

	__raw_writel(SSUSB_DEV_SPEED_CHG_INTR_EN, U3D_DEV_LINK_INTR_ENABLE);
}

/* usb generic functions */
static int mt_read_fifo(struct mt_ep *p_ep)
{
	const int ep_num = GET_EP_NUM(p_ep->endpoint_address);
	struct urb *p_urb = (ep_num == 0) ? (gp_ep0_urb) : (p_ep->p_rcv_urb);
	int count = 0;
	unsigned char *p_buff = NULL;

	if (p_urb == NULL)
		return 0;

	p_buff = (u8 *)(p_urb->p_buf + p_urb->actual_length);

	count = mu3d_hal_pio_read_fifo(ep_num, p_buff);

	p_urb->actual_length += count;

	if (ep_num == 0) {
		if (count <= p_ep->rx_pktsz) {
			/* last packet */
			udc_chg_ep0_state(EP0_IDLE);
		}
	}else {
		USB_WRITECSR32(U3D_RX1CSR0, ep_num,
			USB_READCSR32(U3D_RX1CSR0, ep_num) | RX_RXPKTRDY);
	}

	return count;
}

static void update_csr(int ep_num, bool data_end)
{
	if (ep_num == 0) {
		if (data_end == true) {
			USB_LOG("csr0 was %x, set EP0_DATAEND now",
				__raw_readl(U3D_EP0CSR));
			USB_EP0CSR_SETMASK(EP0_DATAEND);
			udc_chg_ep0_state(EP0_IDLE);
		} else {
			USB_LOG("csr0 was %x, set EP0_TXPKTRDY now",
				__raw_readl(U3D_EP0CSR));
			USB_EP0CSR_SETMASK(EP0_TXPKTRDY);
		}

		return;
	}

	/* epx */
	if (data_end == false) {
		USB_LOG("txcsr0 of ep%d was %x, set TX_TXPKTRDY now",
			ep_num, USB_READCSR32(U3D_TX1CSR0, ep_num));
		USB_WRITECSR32(U3D_TX1CSR0, ep_num,
			USB_READCSR32(U3D_TX1CSR0, ep_num) | TX_TXPKTRDY);
	}
}

static int mt_write_fifo(struct mt_ep *p_ep)
{
	const int ep_num = GET_EP_NUM(p_ep->endpoint_address);
	struct urb *p_urb = (ep_num == 0) ? (gp_ep0_urb) : (p_ep->p_tx_urb);
	int last = 0, count = 0;
	unsigned char *p_buf = NULL;
	u32 wrote = 0;

	if (p_urb) {
		count = last = MIN(p_urb->actual_length - p_ep->sent, p_ep->tx_pktsz);
		USB_LOG ("[%s] p_urb->actual_length = %d, endpoint->sent = %d",
			__func__, p_urb->actual_length, p_ep->sent);

		do {
			p_buf = p_urb->p_buf + p_ep->sent;
			wrote = mu3d_hal_pio_write_fifo(ep_num, count, p_buf, p_urb->p_ep->tx_pktsz);

			update_csr(ep_num, (bool)(wrote==0));

			count -= wrote;
		} while (count > 0);

		p_ep->last = last;
		p_ep->sent += last;
	}

	return last;
}

static struct mt_ep *mt_find_ep(int ep_num, u8 dir)
{
	int i;

	for (i = 0; i < gp_udc_dev->max_endpoints; i++) {
		int ep_addr = gp_udc_dev->p_ep_array[i].endpoint_address;
		if ((GET_EP_NUM(ep_addr) == ep_num) && (GET_EP_DIR(ep_addr) == dir))
			return &gp_udc_dev->p_ep_array[i];
	}
	return NULL;
}

static void mt_udc_flush_fifo(u8 ep_num, u8 dir)
{
	struct mt_ep *p_ep = mt_find_ep(ep_num, dir);

	if (ep_num == 0) {
		USB_SETMASK(U3D_EP_RST, EP0_RST);
		USB_CLRMASK(U3D_EP_RST, EP0_RST);
	} else {
		p_ep = mt_find_ep(ep_num, dir);
		if (GET_EP_DIR(p_ep->endpoint_address) == USB_DIR_OUT) {
			USB_SETMASK(U3D_EP_RST, (1 << ep_num));//reset RX EP
			USB_CLRMASK(U3D_EP_RST, (1 << ep_num));//reset reset RX EP
		} else {
			USB_SETMASK(U3D_EP_RST, (BIT16 << ep_num));//reset TX EP
			USB_CLRMASK(U3D_EP_RST, (BIT16 << ep_num));//reset reset TX EP
		}
	}
}

static void mt_udc_flush_ep0_fifo(void)
{
	mt_udc_flush_fifo(0, 0);
}

static void mt_udc_rxtxmap_recover(void)
{
	//TODO: Need to check what needs to be recover here
#if 0
	int i;

	for (i = 1; i < gp_udc_dev->max_endpoints; i++) {
		__raw_writeb(i, INDEX);

		if (GET_EP_DIR(gp_udc_dev->p_ep_array[i].endpoint_address) == USB_DIR_IN)
			__raw_writel(gp_udc_dev->p_ep_array[i].rx_pktsz, (IECSR + RXMAP));
		else
			__raw_writel(gp_udc_dev->p_ep_array[i].tx_pktsz, (IECSR + TXMAP));
	}
#endif
}

USB_SPEED mt_udc_get_speed(void)
{
	u32 speed = __raw_readl(U3D_DEVICE_CONF) & SSUSB_DEV_SPEED;
	switch (speed) {
		case 1:
			print("FS is detected\n");
			return SSUSB_SPEED_FULL;
			break;
		case 3:
			print("HS is detected\n");
			return SSUSB_SPEED_HIGH;
			break;
		case 4:
			print("SS is detected\n");
			return SSUSB_SPEED_SUPER;
			break;
		default:
			USB_LOG("Unrecognized Speed %d", speed);
			break;
	};

	return SSUSB_SPEED_FULL;
}

/*
 * u3d_ep0en - enable ep0 function
 */
void u3d_ep0en(void)
{
	u32 temp = 0;
	struct mt_ep *p_ep0 = (struct mt_ep *) (gp_udc_dev->p_ep_array);

	if (mt_udc_get_speed() == SSUSB_SPEED_SUPER) {
		p_ep0->rx_pktsz = p_ep0->tx_pktsz = EP0_MAX_PACKET_SIZE_U3;
	} else {
		p_ep0->rx_pktsz = p_ep0->tx_pktsz = EP0_MAX_PACKET_SIZE;
	}

	/* EP0CSR */
	temp = (p_ep0->rx_pktsz & 0x3ff);
	__raw_writel(temp, U3D_EP0CSR);
	USB_LOG("csr0 is now %x", __raw_readl(U3D_EP0CSR));

	/* enable EP0 interrupts */
	USB_SETMASK(U3D_EPIESR, (EP0ISR | SETUPENDISR));
}

void u3d_irq_en()
{
	__raw_writel(0xFFFFFFFF, U3D_LV1IESR);
}

void mu3d_initialize_drv(void)
{
	u3d_irq_en();

	__raw_writel(__raw_readl(U3D_POWER_MANAGEMENT) & ~LPM_MODE, U3D_POWER_MANAGEMENT);
	__raw_writel(__raw_readl(U3D_POWER_MANAGEMENT) | (LPM_MODE&0x1), U3D_POWER_MANAGEMENT);

#ifdef EXT_VBUS_DET
	/* force VBUS on */
	__raw_writel(0x3, U3D_MISC_CTRL);
#else
	__raw_writel(0x0, U3D_MISC_CTRL);
#endif

	mu3d_hal_system_intr_en();

	u3d_ep0en();
}

/* reset USB hardware */
void mt_udc_reset(USB_SPEED speed)
{
	u32 dwtmp1 = 0, dwtmp2 = 0, dwtmp3 = 0;

	dwtmp1 = __raw_readl(U3D_SSUSB_PRB_CTRL1);
	dwtmp2 = __raw_readl(U3D_SSUSB_PRB_CTRL2);
	dwtmp3 = __raw_readl(U3D_SSUSB_PRB_CTRL3);

	mu3d_hal_rst_dev();

	__raw_writel(dwtmp1, U3D_SSUSB_PRB_CTRL1);
	__raw_writel(dwtmp2, U3D_SSUSB_PRB_CTRL2);
	__raw_writel(dwtmp3, U3D_SSUSB_PRB_CTRL3);

	mdelay(50);

	mu3d_hal_ssusb_en();
	mu3d_hal_set_speed(speed);

	gp_udc_dev->address = 0;
	g_tx_fifo_addr = USB_TX_FIFO_START_ADDRESS;
	g_rx_fifo_addr = USB_RX_FIFO_START_ADDRESS;

	mu3d_initialize_drv();

	//reset all eps including ep0 ?
	//__raw_writel(0, U3D_EP_RST);
}

static void mt_udc_ep0_write(void)
{
	struct mt_ep *p_ep = (struct mt_ep *)(gp_udc_dev->p_ep_array);
	unsigned int count = 0;

	count = mt_write_fifo(p_ep);

	if (count < p_ep->tx_pktsz) {
		/* last packet */
		gp_ep0_urb->actual_length = 0;
		p_ep->sent = 0;
	} else {
		USB_LOG(" %s wrote %d bytes n there's more, maxp is %d",
		__func__, count, p_ep->tx_pktsz);
	}
}

static void mt_udc_ep0_read(void)
{
	struct mt_ep *p_ep = (struct mt_ep *)(gp_udc_dev->p_ep_array + 0);
	int count = 0;
	u32 csr0 = __raw_readl(U3D_EP0CSR);

	/* erroneous ep0 interrupt */
	if (!(csr0 & EP0_RXPKTRDY)) {
		return;
	}

	count = mt_read_fifo(p_ep);
}

static void mt_udc_ep0_setup(void)
{
	struct mt_ep *p_ep = (struct mt_ep *)(gp_udc_dev->p_ep_array + 0);
	u32 csr0 = 0;
	u16 count = 0;
	u8 stall = 0;
	struct device_request *p_request;
	u8 bmRequestType = 0;
	u32 wait_setup = 0;

	csr0 = __raw_readl(U3D_EP0CSR);
	if (!(csr0 & EP0_SETUPPKTRDY)) {
		USB_LOG(" No EP0_SETUPPKTRDY, U3D_EP0CSR is %x, exit", csr0);
		return;
	}

	/* unload fifo */
	gp_ep0_urb->actual_length = 0;
	count = mt_read_fifo (p_ep);

	/* decode command */
	p_request = &gp_ep0_urb->dev_request;
	memcpy(p_request, gp_ep0_urb->p_buf, sizeof(*p_request));

	//TODO: May not be necessasry
	#if 0
	if (__raw_readl(U3D_EPISR) & SETUPENDISR) {
		USB_LOG("Abort this command because of SETUP");
		return;
	}
	#endif

	// Fulfill the request
	bmRequestType = p_request->bmRequestType;
	if (((bmRequestType) & USB_TYPE_MASK) == USB_TYPE_STANDARD) {
		stall = ep0_standard_setup(gp_ep0_urb);
		if (stall) {
			dump_setup_packet("[U]: STANDARD REQUEST NOT SUPPORTED", p_request);
		}
	} else if (((bmRequestType) & USB_TYPE_MASK) == USB_TYPE_CLASS) {
		stall = ep0_class_setup(gp_ep0_urb);
		if (stall) {
			dump_setup_packet("[U]: CLASS REQUEST NOT SUPPORTED", p_request);
		}
	} else if (((bmRequestType) & USB_TYPE_MASK) == USB_TYPE_VENDOR) {
		USB_LOG(" ALL VENDOR-SPECIFIC REQUESTS ARE NOT SUPPORTED!!");
	}

	if (stall) {
		/* the received command is not supported */
		udc_stall_ep(0, USB_DIR_OUT);
		return;
	}

	csr0 = __raw_readl(U3D_EP0CSR);
	switch (ge_ep0_state) {
	case EP0_TX:
		/* data stage: from device to host */
		mt_udc_ep0_write();
		break;
	case EP0_RX:
		/* data stage: from host to device */
	case EP0_IDLE:
		/* no data stage */
		break;
	default:
		USB_LOG("Unrecognized ep0 state%d", ge_ep0_state);
		break;
	}
}

static void mt_udc_ep0_handler(void)
{
	u32 csr0 = __raw_readl(U3D_EP0CSR);

	if (csr0 & EP0_SENTSTALL) {
		USB_LOG ("Found EP0_SENTSTALL");
		/* needs implementation for exception handling here */
		udc_chg_ep0_state(EP0_IDLE);
		USB_EP0CSR_SETMASK(EP0_SENTSTALL);
	}

	USB_LOG(" %s handles ge_ep0_state %d", __func__, ge_ep0_state);
	switch (ge_ep0_state) {
	case EP0_IDLE:
		mt_udc_ep0_setup();
		break;
	case EP0_TX:
		if (csr0 & EP0_SETUPPKTRDY) {
			USB_ERR_LOG("Recieve EP0_SETUPPKTRDY at EP0_TX state");
			USB_EP0CSR_SETMASK(EP0_DATAEND);
			udc_chg_ep0_state(EP0_IDLE);
			return;
		}

		mt_udc_ep0_write();
		break;
	case EP0_RX:
		mt_udc_ep0_read();
		break;
	default:
		USB_LOG("Unrecognized ep0 state%d", ge_ep0_state);
		break;
	}
}

static void mt_udc_epx_handler(u8 ep_num, u8 dir)
{
	u32 csr;
	u32 count;
	struct mt_ep *endpoint = NULL;
	struct urb *p_urb = NULL;

	endpoint = mt_find_ep(ep_num, dir);

	USB_LOG("%s for EP %d", __func__, ep_num);

	switch (dir) {
	case USB_DIR_OUT:
		/* transfer direction is from host to device */
		/* from the view of usb device, it's RX */
		csr = USB_READCSR32(U3D_RX1CSR0, ep_num);

		if (csr & RX_SENTSTALL) {
			USB_LOG ("EP %d(RX): RX_SENTSTALL", ep_num);
			/* exception handling: implement this!! */
			return;
		}

		if (!(csr & RX_RXPKTRDY)) {
			USB_LOG ("EP %d: No RX_RXPKTRDY, abort %s", ep_num, __func__);
			return;
		}

		count = mt_read_fifo(endpoint);

		USB_LOG ("EP%d(RX) read %d bytes", ep_num, count);

		break;
	case USB_DIR_IN:
		/* transfer direction is from device to host */
		/* from the view of usb device, it's tx */
		csr = USB_READCSR32(U3D_TX1CSR0, ep_num);

		if (csr & TX_SENTSTALL) {
			USB_LOG("EP %d(TX): TX_SENTSTALL", ep_num);
			/* exception handling: implement this!! */
			return;
		}

		if (csr & TX_TXPKTRDY) {
			USB_LOG("EP%d TX_TXPKTRDY is set, not ready to write", ep_num);
			return;
		}

		count = mt_write_fifo(endpoint);
		USB_LOG ("EP%d(TX) wrote %d bytes, %d bytes in total",
			ep_num, count, endpoint->sent);

		p_urb = endpoint->p_tx_urb;
		if (endpoint->p_tx_urb->actual_length - endpoint->sent <= 0) {
			p_urb->actual_length = 0;
			endpoint->sent = 0;
			endpoint->last = 0;
		}
		break;
	default:
		break;
	}
}

void report_ltssm_type(u32 ltssm)
{
#if USBD_DBG_LOG
	if (ltssm & RXDET_SUCCESS_INTR) {
		USB_LOG("RXDET_SUCCESS_INTR");
	}

	if (ltssm & HOT_RST_INTR) {
		USB_LOG("HOT_RST_INTR");
	}

	if (ltssm & WARM_RST_INTR) {
		USB_LOG("WARM_RST_INTR");
	}

	if (ltssm & ENTER_U0_INTR) {
		USB_LOG("ENTER_U0_INTR");
	}

	if (ltssm & VBUS_RISE_INTR) {
		USB_LOG("VBUS_RISE_INTR");
	}

	if (ltssm & VBUS_FALL_INTR) {
		USB_LOG("VBUS_FALL_INTR");
	}

	if (ltssm & ENTER_U1_INTR) {
		USB_LOG("ENTER_U1_INTR");
	}

	if (ltssm & ENTER_U2_INTR) {
		USB_LOG("ENTER_U2_INTR");
	}

	if (ltssm & ENTER_U3_INTR) {
		USB_LOG("ENTER_U3_INTR");
	}

	if (ltssm & EXIT_U1_INTR) {
		USB_LOG("EXIT_U1_INTR");
	}

	if (ltssm & EXIT_U2_INTR) {
		USB_LOG("EXIT_U2_INTR");
	}

	if (ltssm & EXIT_U3_INTR) {
		USB_LOG("EXIT_U3_INTR");
	}
#endif
}

void mt_udc_irq(u32 ltssm, u32 intrusb, u32 dmaintr, u16 intrtx, u16 intrrx,
	u32 intrqmu, u32 intrqmudone, u32 linkint)
#if 0
void mt_udc_irq(
	u32 ltssm,
	u32 intrusb,
	u32 dmaintr,
	u16 intrtx,
	u16 intrrx,
	u32 intrqmu,
	u32 intrqmudone,
	u32 linkint)
#endif
{
	u32 temp = 0;
	u32 ep_num = 0;

	USB_LOG("%s starts ****************", __func__);
	USB_LOG("   LTSSM: %x", ltssm);
	USB_LOG("   INTRUSB: %x", intrusb);
	USB_LOG("   DMAINTR: %x", dmaintr);
	USB_LOG("   INTRTX: %x", intrtx);
	USB_LOG("   INTRRX: %x", intrrx);
	USB_LOG("   INTRQMU: %x", intrqmu);
	USB_LOG("   INTRQMUDONE: %x", intrqmudone);
	USB_LOG("   LINKINT: %x", linkint);
	USB_LOG("   U3D_EP0CSR: %x", __raw_readl(U3D_EP0CSR));
	USB_LOG("   U3D_EPIER: %x", __raw_readl(U3D_EPIER));
	USB_LOG("   U3D_EPISR: %x", __raw_readl(U3D_EPISR));
	USB_LOG("   U3D_DEVICE_CONF: %x", __raw_readl(U3D_DEVICE_CONF));
	USB_LOG("   U3D_DEVICE_MONITOR: %x", __raw_readl(U3D_DEVICE_MONITOR));

#ifdef SUPPORT_QMU
	if (intrqmudone) {
		//qmu_proc(); // test driver
		qmu_done_interrupt(intrqmudone);
	}
 	if (intrqmu) {
		qmu_handler(intrqmu);
 	}
#endif

 	if (linkint & SSUSB_DEV_SPEED_CHG_INTR) {
		USB_LOG("[INTR] SSUSB_DEV_SPEED_CHG_INTR");

		mu3d_hal_set_speed(mt_udc_get_speed());
	}

	/* Check for reset interrupt */
	if (intrusb & RESET_INTR) {
		USB_LOG("[INTR] RESET_INTR");

		//TODO: Come back to check where this went wrong!!!
		//mt_udc_reset(gp_udc_dev->speed);
		#if 0
		mu3d_initialize_drv();
		g_tx_fifo_addr = USB_TX_FIFO_START_ADDRESS;
		g_rx_fifo_addr = USB_RX_FIFO_START_ADDRESS;

		#endif

		/* set device address to 0 after reset */
		g_set_address = 0;
		intrtx = 0;
		intrrx = 0;
	}

#ifdef SUPPORT_U3
	if (ltssm) {
		if (ltssm & SS_DISABLE_INTR) {
			USB_LOG("[INTR] SS_DISABLE_INTR, DISABLE_CNT(U3D_LTSSM_INFO): %x",
				(__raw_readl(U3D_LTSSM_INFO) & DISABLE_CNT) >> DISABLE_CNT_OFST);

			/* Set soft_conn to enable U2 termination */
			mu3d_hal_u2dev_connect();
			u3d_ep0en();

			ltssm = 0;
		}

		report_ltssm_type(ltssm);

		if (ltssm & ENTER_U0_INTR) {
			USB_LOG(" ENTER_U0_INTR");
			mu3d_initialize_drv();
			g_tx_fifo_addr = USB_TX_FIFO_START_ADDRESS;
			g_rx_fifo_addr = USB_RX_FIFO_START_ADDRESS;
			g_set_address = 0;
			intrtx = 0;
			intrrx = 0;
		}

#ifndef POWER_SAVING_MODE
		if (ltssm & U3_RESUME_INTR) {
			USB_LOG("[INTR] U3_RESUME_INTR");
			USB_CLRMASK(U3D_SSUSB_U3_CTRL_0P, SSUSB_U3_PORT_PDN);
			USB_CLRMASK(U3D_SSUSB_IP_PW_CTRL2, SSUSB_IP_DEV_PDN);
			while(!(__raw_readl(U3D_SSUSB_IP_PW_STS1) & SSUSB_U3_MAC_RST_B_STS));
			USB_SETMASK(U3D_LINK_POWER_CONTROL, UX_EXIT);
		}
#endif
	}
#endif

	if (intrusb & DISCONN_INTR) {
		USB_LOG("[INTR] DISCONN_INTR");
	}

	if (intrusb & CONN_INTR) {
		USB_LOG("[INTR] CONN_INTR");
	}

	if (intrusb & SUSPEND_INTR) {
		USB_LOG("[INTR]SUSPEND_INTR");
	}

	/* TODO: Possibly don't need to handle this interrupt */
	if (intrusb & LPM_INTR) {
		USB_LOG("[INTR] LPM Interrupt");

		temp = __raw_readl(U3D_USB20_LPM_PARAM);
		USB_LOG("%x, BESL: %x, x <= %x <= %x",
			temp&0xf, (temp >> 8)&0xf, (temp >> 12)&0xf, (temp >> 4)&0xf);

		temp = __raw_readl(U3D_POWER_MANAGEMENT);
		USB_LOG("RWP: %x", (temp & LPM_RWP) >> 11);

		{
			// s/w LPM only
			USB_SETMASK(U3D_USB20_MISC_CONTROL, LPM_U3_ACK_EN);

			//wait a while before remote wakeup, so xHCI PLS status is not affected
			mdelay(20);
			USB_SETMASK(U3D_POWER_MANAGEMENT, RESUME);

			USB_LOG("RESUME: %d", __raw_readl(U3D_POWER_MANAGEMENT) & RESUME);
		}
	}

	if (intrusb & LPM_RESUME_INTR) {
		USB_LOG("[INTR]LPM_RESUME_INTR");

		if (!(__raw_readl(U3D_POWER_MANAGEMENT) & LPM_HRWE)) {
			USB_SETMASK(U3D_USB20_MISC_CONTROL, LPM_U3_ACK_EN);
		}
	}

	/* Check for resume from suspend mode */
	if (intrusb & RESUME_INTR) {
		USB_LOG("[INTR] RESUME_INTR");
	}

#ifdef SUPPORT_DMA
	if (dmaintr) {
	  	u3d_dma_handler(dmaintr);
	}
#endif

	/* For EP0 */
	if ((intrtx & 0x1) || (intrrx & 0x1)) {
		if (intrrx & 0x1) {
			USB_LOG("Service SETUPEND");
		}

		mt_udc_ep0_handler();
		intrtx = intrtx & ~0x1; //EPISR
		intrrx = intrrx & ~0x1; //SETUPENDISR of EP0
	}

	/* For EPx (TX) */
	if (intrtx) {
		for(ep_num = 1; ep_num <= TX_FIFO_NUM; ++ep_num) {
		  	if (intrtx & (1 << ep_num)) {
				if (!(USB_READCSR32(U3D_TX1CSR0, ep_num) & TX_SENTSTALL)) {
					mt_udc_epx_handler(ep_num, USB_DIR_IN);
				} else {
					USB_WRITECSR32(U3D_TX1CSR0, ep_num,
						USB_READCSR32(U3D_TX1CSR0, ep_num) | TX_SENTSTALL);

				}
			}
	   	}
  	}

	/* For EPx (RX) */
	if (intrrx) {
	 	for(ep_num = 1; ep_num <= RX_FIFO_NUM; ++ep_num) {
		 	if (intrrx & (1 << ep_num)) {
				if (!(USB_READCSR32(U3D_RX1CSR0, ep_num) & RX_SENTSTALL)) {
		  			mt_udc_epx_handler(ep_num, USB_DIR_OUT);
				} else {
					USB_WRITECSR32(U3D_RX1CSR0, ep_num,
						USB_READCSR32(U3D_RX1CSR0, ep_num) | RX_SENTSTALL);

				}
			}
		}
 	}
}

/*
 * Start of public functions.
 */

/* Called to start packet transmission. */
void mt_ep_write(struct mt_ep *endpoint)
{
	int ep_num = GET_EP_NUM(endpoint->endpoint_address);
	u32 u32Csr = 0;

	if (ep_num != 0) {
		if (GET_EP_DIR(endpoint->endpoint_address) == USB_DIR_IN) {
			mt_write_fifo (endpoint);
			service_interrupts();
		} else {
			USB_LOG("%s: ep%d is RX endpoint", __func__, ep_num);
		}
	} else {
		USB_LOG("%s: ep0 cannot be written", __func__);
	}
}

/* is_tx_ep_busy: used by usbtty.c */
int is_tx_ep_busy(struct mt_ep *endpoint)
{
	int ep_num = GET_EP_NUM(endpoint->endpoint_address);
	u32 csr = 0;

	if (GET_EP_DIR(endpoint->endpoint_address) == USB_DIR_IN) {
		csr = USB_READCSR32(U3D_TX1CSR0, ep_num);
	} else {
		USB_LOG ("%s: ep%d is RX endpoint", __func__, ep_num);
	}

	return (csr & TX_TXPKTRDY);
}

int is_tx_ep_not_empty(struct mt_ep *endpoint)
{
	int ep_num = GET_EP_NUM(endpoint->endpoint_address);
	u32 csr = 0;

	if (GET_EP_DIR(endpoint->endpoint_address) == USB_DIR_IN) {
		csr = USB_READCSR32(U3D_TX1CSR0, ep_num);
	} else {
		USB_LOG ("%s: ep%d is RX endpoint", __func__, ep_num);
	}

	return !(csr & TX_FIFOEMPTY);
}



static void udc_clean_sentstall(unsigned int ep_num, u8 dir)
{
	if (ep_num == 0) {
		USB_EP0CSR_SETMASK(EP0_SENDSTALL);
	} else {
		if (dir == USB_DIR_OUT) {
			USB_WRITECSR32(U3D_RX1CSR0, ep_num, USB_READCSR32(U3D_RX1CSR0, ep_num) | RX_SENTSTALL);
			USB_WRITECSR32(U3D_RX1CSR0, ep_num, USB_READCSR32(U3D_RX1CSR0, ep_num) &~ RX_SENDSTALL);
		} else {
			USB_WRITECSR32(U3D_TX1CSR0, ep_num, USB_READCSR32(U3D_TX1CSR0, ep_num) | TX_SENTSTALL);
			USB_WRITECSR32(U3D_TX1CSR0, ep_num, USB_READCSR32(U3D_TX1CSR0, ep_num) &~ TX_SENDSTALL);
		}
	}
}

/* the endpoint does not support the received command, stall it!! */
static void udc_stall_ep(unsigned int ep_num, u8 dir)
{
	struct mt_ep *endpoint = mt_find_ep (ep_num, dir);

	USB_LOG("%s for EP %d", __func__, ep_num);

	if (ep_num == 0) {
		mt_udc_flush_ep0_fifo();
		USB_EP0CSR_SETMASK(EP0_SENDSTALL);

		//Waiting HW to send stall
		while(!(__raw_readl(U3D_EP0CSR) & EP0_SENTSTALL));

		udc_clean_sentstall(0, USB_DIR_OUT);
	} else {
		if (GET_EP_DIR(endpoint->endpoint_address) == USB_DIR_OUT) {
			u32 u32Csr = USB_READCSR32(U3D_RX1CSR0, ep_num);

			u32Csr &= RX_W1C_BITS;
			u32Csr |= RX_SENDSTALL;
			USB_WRITECSR32(U3D_RX1CSR0, ep_num, u32Csr);

			//Waiting HW to send stall
			while(!(USB_READCSR32(U3D_RX1CSR0, ep_num) & RX_SENTSTALL));

			udc_clean_sentstall(ep_num, USB_DIR_OUT);
			mt_udc_flush_fifo(ep_num, USB_DIR_OUT);
		} else {
			u32 u32Csr = USB_READCSR32(U3D_TX1CSR0, ep_num);

			u32Csr &= TX_W1C_BITS;
			u32Csr |= TX_SENDSTALL;
			USB_WRITECSR32(U3D_TX1CSR0, ep_num, u32Csr);

			//Waiting HW to send stall
			while(!(USB_READCSR32(U3D_TX1CSR0, ep_num) & TX_SENTSTALL));

			udc_clean_sentstall(ep_num, USB_DIR_IN);
			mt_udc_flush_fifo(ep_num, USB_DIR_IN);
		}
	}

	udc_chg_ep0_state(EP0_IDLE);

	return;
}

/*
 * get_seg_size()
 *
 * Return value indicates the TxFIFO size of 2^n bytes, (ex: value 10 means 2^10 =
 * 1024 bytes.) TXFIFOSEGSIZE should be equal or bigger than 4. The TxFIFO size of
 * 2^n bytes also should be equal or bigger than TXMAXPKTSZ. This EndPoint occupy
 * total memory size  (TX_SLOT + 1 )*2^TXFIFOSEGSIZE bytes.
 */
u8 get_seg_size(u32 max_packet_size)
{
	/* Set fifo size(double buffering is currently not enabled) */
	switch (max_packet_size) {
	case 8:
	case 16:
		return USB_FIFOSZ_SIZE_16;
	case 32:
		return USB_FIFOSZ_SIZE_32;
	case 64:
		return USB_FIFOSZ_SIZE_64;
	case 128:
		return USB_FIFOSZ_SIZE_128;
	case 256:
		return USB_FIFOSZ_SIZE_256;
	case 512:
		return USB_FIFOSZ_SIZE_512;
	case 1023:
	case 1024:
	case 2048:
	case 3072:
	case 4096:
		return USB_FIFOSZ_SIZE_1024;
	default:
		USB_LOG("The max_packet_size %d is not supported", max_packet_size);
		return USB_FIFOSZ_SIZE_512;
	}
}

/*
 * udc_setup_ep - setup endpoint
 *
 * Associate a physical endpoint with endpoint_instance and initialize FIFO
 */
void mt_setup_ep(struct mt_dev *device, unsigned int ep_num, struct mt_ep *p_ep)
{
	u32 csr0, csr1, csr2;
	u32 max_packet_size;
	u8 seg_size;
	u8 max_pkt;
	u8 burst = p_ep->burst;
	u8 mult = p_ep->mult;
	u8 type = p_ep->type;
	u8 slot = p_ep->slot;

	/* Nothing needs to be done for ep0 */
	if (ep_num == 0) { // or (endpoint->type == USB_EP_XFER_CTRL)
		return;
	}

	/* Configure endpoint fifo */
	/* Set fifo address, fifo size, and fifo max packet size */
	if (GET_EP_DIR(p_ep->endpoint_address) == USB_DIR_OUT) {
		USB_LOG("USB_RxEPInit for ep %d starts, maxpacketsize is %d",
			ep_num, p_ep->rx_pktsz);

		mt_udc_flush_fifo(ep_num, USB_DIR_OUT);

		max_packet_size = p_ep->rx_pktsz;
		seg_size = get_seg_size(max_packet_size);

		csr0 = USB_READCSR32(U3D_RX1CSR0, ep_num) &~ RX_RXMAXPKTSZ;
		csr0 |= (max_packet_size & RX_RXMAXPKTSZ);
   		csr0 &= ~RX_DMAREQEN;

		max_pkt = (burst + 1) * (mult + 1) - 1;
		csr1 = (burst & SS_RX_BURST);
		csr1 |= (slot << RX_SLOT_OFST) & RX_SLOT;
		csr1 |= (max_pkt << RX_MAX_PKT_OFST) & RX_MAX_PKT;
		csr1 |= (mult << RX_MULT_OFST) & RX_MULT;

		csr2 = (g_rx_fifo_addr >> 4) & RXFIFOADDR;
		csr2 |= (seg_size << RXFIFOSEGSIZE_OFST) & RXFIFOSEGSIZE;

		if (type == USB_EP_XFER_BULK) {
			USB_LOG("ep %d is bulk", ep_num);
			csr1 |= TYPE_BULK;
		} else if (type == USB_EP_XFER_INT) {
			USB_LOG("ep %d is int", ep_num);
			csr1 |= TYPE_INT;
			csr2 |= (p_ep->binterval << RXBINTERVAL_OFST)&RXBINTERVAL;
		}

		/* Write 1 to clear EPIER */
		USB_SETMASK(U3D_EPIECR, (BIT16 << ep_num));
		USB_WRITECSR32(U3D_RX1CSR0, ep_num, csr0);
		USB_WRITECSR32(U3D_RX1CSR1, ep_num, csr1);
		USB_WRITECSR32(U3D_RX1CSR2, ep_num, csr2);

		USB_LOG("U3D_RX1CSR0 :%x", USB_READCSR32(U3D_RX1CSR0, ep_num));
		USB_LOG("U3D_RX1CSR1 :%x", USB_READCSR32(U3D_RX1CSR1, ep_num));
		USB_LOG("U3D_RX1CSR2 :%x", USB_READCSR32(U3D_RX1CSR2, ep_num));

		/* Write 1 to set EPIER */
		__raw_writel(__raw_readl(U3D_EPIER) | (BIT16 << ep_num), U3D_EPIESR);

		if (max_packet_size == 1023) {
			g_rx_fifo_addr += (1024 * (slot + 1));
		} else {
			g_rx_fifo_addr += (max_packet_size * (slot + 1));
		}

		if (g_rx_fifo_addr > __raw_readl(U3D_CAP_EPNRXFFSZ)) {
			USB_ERR_LOG("g_rx_fifo_addr is %x and U3D_CAP_EPNTXFFSZ is %x for ep%d",
				g_rx_fifo_addr, __raw_readl(U3D_CAP_EPNRXFFSZ), ep_num);
			USB_LOG("max_packet_size = %d", max_packet_size);
			USB_LOG("slot = %d]n", slot);
		}
	}
	else //(GET_EP_DIR(endpoint->endpoint_address) == USB_DIR_IN)
	{
		USB_LOG("USB_TxEPInit for ep %d starts, maxpacketsize is %d",
			ep_num, p_ep->tx_pktsz);

		mt_udc_flush_fifo(ep_num, USB_DIR_IN);

		max_packet_size = p_ep->tx_pktsz;
		seg_size = get_seg_size(max_packet_size);

		csr0 = USB_READCSR32(U3D_TX1CSR0, ep_num) &~ TX_TXMAXPKTSZ;
		csr0 |= (max_packet_size & TX_TXMAXPKTSZ);
		csr0 &= ~TX_DMAREQEN;

		max_pkt = (burst + 1) * (mult + 1) - 1;
		csr1 = (burst & SS_TX_BURST);
		csr1 |= (slot << TX_SLOT_OFST) & TX_SLOT;
		csr1 |= (max_pkt << TX_MAX_PKT_OFST) & TX_MAX_PKT;
		csr1 |= (mult << TX_MULT_OFST) & TX_MULT;

		csr2 = (g_tx_fifo_addr >> 4) & TXFIFOADDR;
		csr2 |= (seg_size << TXFIFOSEGSIZE_OFST) & TXFIFOSEGSIZE;

		if (type == USB_EP_XFER_BULK) {
			USB_LOG("ep %d is bulk", ep_num);
			csr1 |= TYPE_BULK;
		} else if (type == USB_EP_XFER_INT) {
			USB_LOG("ep %d is int", ep_num);
			csr1 |= TYPE_INT;
			csr2 |= (p_ep->binterval << TXBINTERVAL_OFST) & TXBINTERVAL;
		}
		/* Write 1 to clear EPIER */
		USB_SETMASK(U3D_EPIECR, (BIT0 << ep_num));
		USB_WRITECSR32(U3D_TX1CSR0, ep_num, csr0);
		USB_WRITECSR32(U3D_TX1CSR1, ep_num, csr1);
		USB_WRITECSR32(U3D_TX1CSR2, ep_num, csr2);

		USB_LOG("[CSR]U3D_TX1CSR0: %x", USB_READCSR32(U3D_TX1CSR0, ep_num));
		USB_LOG("[CSR]U3D_TX1CSR1: %x", USB_READCSR32(U3D_TX1CSR1, ep_num));
		USB_LOG("[CSR]U3D_TX1CSR2: %x", USB_READCSR32(U3D_TX1CSR2, ep_num));

		/* Write 1 to set EPIER */
		__raw_writel(__raw_readl(U3D_EPIER) | (BIT0 << ep_num), U3D_EPIESR);

		if (max_packet_size == 1023) {
			g_tx_fifo_addr += (1024 * (slot + 1));
		} else {
			g_tx_fifo_addr += (max_packet_size * (slot + 1));
		}

		if (g_tx_fifo_addr > __raw_readl(U3D_CAP_EPNTXFFSZ)) {
			USB_ERR_LOG("g_tx_fifo_addr is %x and U3D_CAP_EPNTXFFSZ is %x for ep%d",
				g_tx_fifo_addr, __raw_readl(U3D_CAP_EPNTXFFSZ), ep_num);
			USB_LOG("max_packet_size = %d, slot = %d",
				max_packet_size, slot);
		}
	}
}

/* Turn on the USB connection by enabling the pullup resistor */
void mt_usb_connect_internal(void)
{
	if (gp_udc_dev->speed != SSUSB_SPEED_SUPER) {
		mu3d_hal_u2dev_connect();
	} else {
		mu3d_hal_u3dev_connect();
	}
}

/* Turn off the USB connection by disabling the pullup resistor */
void mt_usb_disconnect_internal(void)
{
	mu3d_hal_u2dev_disconnect();
	mu3d_hal_u3dev_disconnect();
}

/* Switch on the UDC */
void udc_enable(struct mt_dev *pdev)
{
	/* Initialize ep0 state */
	ge_ep0_state = EP0_IDLE;
	gp_udc_dev = pdev;
	gp_ep0_urb = usb_alloc_urb(gp_udc_dev, &gp_udc_dev->p_ep_array[0]);

	/* reset USB hardware */
	mt_usb_phy_recover();
	mt_udc_reset(U3D_DFT_SPEED);
}

/* Switch off the UDC */
void udc_disable(void)
{
}

void udc_stop(void)
{
	mt_usb_disconnect_internal();
	mt_usb_phy_savecurrent();
}



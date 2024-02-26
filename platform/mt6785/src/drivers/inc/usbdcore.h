/* Copyright Statement:
 *
 * This software/firmware and related documentation	("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * MediaTek Inc.	(C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS	("MEDIATEK SOFTWARE")
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
 * The following software/firmware and/or related documentation	("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#ifndef __USBDCORE_H__
#define __USBDCORE_H__

#include "typedefs.h"

#ifndef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#endif

/* Request types */
#define USB_TYPE_MASK		(0x03 << 5)
#define USB_TYPE_STANDARD	(0x00 << 5)
#define USB_TYPE_CLASS		(0x01 << 5)
#define USB_TYPE_VENDOR		(0x02 << 5)
#define USB_TYPE_RESERVED	(0x03 << 5)

/* USB recipients */
#define USB_RECIP_MASK		0x03
#define USB_RECIP_DEVICE	0x00
#define USB_RECIP_INTERFACE	0x01
#define USB_RECIP_ENDPOINT	0x02
#define USB_RECIP_OTHER		0x03

/* USB transfer directions */
#define USB_DIR_MASK		0x80
#define USB_DIR_IN		0x80
#define USB_DIR_OUT		0

/* Endpoints */
#define USB_EP_NUM_MASK		0x0f	/* in bEndpointAddress */

#define GET_EP_NUM(bEndpointAddress)	(bEndpointAddress & USB_EP_NUM_MASK)
#define GET_EP_DIR(bEndpointAddress)	(bEndpointAddress & USB_DIR_MASK)

#define USB_EP_XFER_CTRL		0
#define USB_EP_XFER_ISO			1
#define USB_EP_XFER_BULK		2
#define USB_EP_XFER_INT			3

/* Standard requests */
#define STDREQ_GET_STATUS		0x00
#define STDREQ_CLEAR_FEATURE		0x01
#define STDREQ_SET_FEATURE		0x03
#define STDREQ_SET_ADDRESS		0x05
#define STDREQ_SET_SEL			0x30

#define STDREQ_GET_DESCRIPTOR		0x06
#define STDREQ_GET_CONFIGURATION	0x08
#define STDREQ_SET_CONFIGURATION	0x09
#define STDREQ_GET_INTERFACE		0x0A


/* CDC ACM Class-specific requests */

#define CDCACM_REQ_SET_LINE_CODING		0x20
#define CDCACM_REQ_GET_LINE_CODING		0x21
#define CDCACM_REQ_SET_CONTROL_LINE_STATE	0x22
#define CDCACM_REQ_SEND_BREAK			0x23

/* USB release number	(2.0 does not mean high speed!) */
#define USB_BCD_VERSION			0x0210
#define USB3_BCD_VERSION		0x0300

/* values used in GET_STATUS requests */
#define USB_STAT_SELFPOWERED		0x01

/* Descriptor types */
#define USB_DESCRIPTOR_TYPE_DEVICE			0x01
#define USB_DESCRIPTOR_TYPE_CONFIGURATION		0x02
#define USB_DESCRIPTOR_TYPE_STRING			0x03
#define USB_DESCRIPTOR_TYPE_INTERFACE			0x04
#define USB_DESCRIPTOR_TYPE_ENDPOINT			0x05
#define USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER		0x06
#define USB_DESCRIPTOR_TYPE_BOS			 	0x0f
#define USB_DESCRIPTOR_TYPE_DEVICE_CAPABILITY		0x10
#define USB_DESCRIPTOR_TYPE_SS_ENDPOINT_COMPANION	0x30

/* USB Requests */
struct device_request
{
	u8 bmRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
} __attribute__	((packed));

#define URB_BUF_SIZE 512

struct urb
{
	struct mt_ep *p_ep;
	struct mt_dev *p_dev;
	struct device_request dev_request;
	u32 qmu_complete;

	u8 *p_buf;
	unsigned int buffer_length;
	unsigned int actual_length;

	u16 buffer_data[URB_BUF_SIZE];	/* buffer for data */
};

/* endpoint data */
struct mt_ep
{
	int endpoint_address;	/* endpoint address */

	/* rx side */
	struct urb *p_rcv_urb;	/* active urb */
	int rx_pktsz;		/* maximum packet size from endpoint descriptor */

	/* tx side */
	struct urb *p_tx_urb;	/* active urb */
	int tx_pktsz;		/* maximum packet size from endpoint descriptor */

	int sent;		/* data already sent */
	int last;		/* data sent in last packet, tty checks this value to trigger tx */

	u8 slot;
	u8 burst;
	u8 mult;
	u8 type;
	u8 binterval;
};

struct mt_altsetting
{
	struct interface_descriptor *p_interface_desc;
	/* communication class specific interface descriptors */
	/* only communication interfaces have these fields */
	struct cdcacm_class_header_function_descriptor *p_header_function_desc;
	struct cdcacm_class_abstract_control_descriptor *p_abstract_control_desc;
	struct cdcacm_class_union_function_descriptor *p_union_function_desc;
	struct cdcacm_class_call_management_descriptor *p_call_management_desc;
	int endpoints;
	struct endpoint_descriptor **pp_eps_desc_array;
	struct ss_ep_comp_descriptor *p_ss_ep_comp_desc;
};

struct mt_intf
{
	int alternates;
	struct mt_altsetting *altsetting_array;
};

struct mt_config
{
	int interfaces;
	struct configuration_descriptor *configuration_descriptor;
	struct mt_intf **interface_array;
};

typedef enum {
	RET_SUCCESS = 0,
	RET_FAIL,
} USB_RESULT;

typedef enum {
	SSUSB_SPEED_FULL = 1,
	SSUSB_SPEED_HIGH = 2,
	SSUSB_SPEED_SUPER = 3,
} USB_SPEED;

typedef enum{
	DEV_CAP_WIRELESS = 1,	/* Wireless_USB, */
	DEV_CAP_USB20_EXT = 2,	/* USB2.0 extension */
	DEV_CAP_SS_USB = 3,	/* Superspeed USB */
	DEV_CAP_CON_ID = 4	/* Container ID */
} DEV_CAPABILITY_TYPE;

struct mt_dev
{
	char *p_name;
	struct device_descriptor *p_dev_desc;	/* per device descriptor */
	struct device_qualifier_descriptor *p_dev_qualifier_desc;
	struct bos_descriptor *p_bos_desc;
	struct ext_cap_descriptor *p_ext_cap_desc;
	struct ss_cap_descriptor *p_ss_cap_desc;

	/* configuration descriptors */
	int configurations;
	struct mt_config *p_conf_array;

	u8 address;		 /* function address, 0 by default */
	u8 configuration;	/* configuration, 0 by default, means unconfigured */
	u8 interface;		/* interface, 0 by default */
	u8 alternate;		/* alternate setting */
	u8 speed;
	struct mt_ep *p_ep_array;
	int max_endpoints;
	unsigned char maxpacketsize;
};

struct device_descriptor
{
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;
	u8 iManufacturer;
	u8 iProduct;
	u8 iSerialNumber;
	u8 bNumConfigurations;
} __attribute__ ((packed));

/* U2 spec 9.6.2 */
struct device_qualifier_descriptor
{
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u8 bNumConfigurations;
} __attribute__	((packed));

struct bos_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumDeviceCaps;
} __attribute__((packed));

#define SUPPORT_LPM_PROTOCOL	(1 << 1)	/* supports LPM */

/* Link Power Management */

struct ext_cap_descriptor {
	u8	bLength;
	u8	bDescriptorType;
	u8	bDevCapabilityType;
	u32 bmAttributes;
} __attribute__((packed));

#define NOT_LTM_CAPABLE		(0 << 1)	/* Not able to generate Latenct Tolerance Messages */
#define SUPPORT_LS_OP		(1)		/* Low speed operation */
#define SUPPORT_FS_OP		(1 << 1)	/* Full speed operation */
#define SUPPORT_HS_OP		(1 << 2)	/* High speed operation */
#define SUPPORT_5GBPS_OP	(1 << 3)	/* Operation at 5Gbps */
#define DEFAULT_U1_DEV_EXIT_LAT	0x01		/* Less then 1 microsec */
#define DEFAULT_U2_DEV_EXIT_LAT	0x1F4		/* Less then 500 microsec */

struct ss_cap_descriptor {			/* Link Power Management */
	u8 bLength;
	u8 bDescriptorType;
	u8 bDevCapabilityType;
	u8 bmAttributes;
	u16 wSpeedSupported;
	u8 bFunctionalitySupport;
	u8 bU1devExitLat;
	u16 bU2DevExitLat;
} __attribute__((packed));

struct configuration_descriptor
{
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumInterfaces;
	u8 bConfigurationValue;
	u8 iConfiguration;
	u8 bmAttributes;
	u8 bMaxPower;
} __attribute__	((packed));

struct interface_descriptor
{
	u8 bLength;
	u8 bDescriptorType;
	u8 bInterfaceNumber;
	u8 bAlternateSetting;
	u8 bNumEndpoints;
	u8 bInterfaceClass;
	u8 bInterfaceSubClass;
	u8 bInterfaceProtocol;
	u8 iInterface;
} __attribute__	((packed));

struct endpoint_descriptor
{
	u8 bLength;
	u8 bDescriptorType;
	u8 bEndpointAddress;
	u8 bmAttributes;
	u16 wMaxPacketSize;
	u8 bInterval;
} __attribute__	((packed));

struct ss_ep_comp_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bMaxBurst;
	u8 bmAttributes;
	u16 wBytesPerInterval;
} __attribute__	((packed));

struct string_descriptor
{
	u8 bLength;
	u8 bDescriptorType;	 /* 0x03 */
	u16 wData[256];
};	//__attribute__	((packed));

/* Descriptors used by CDC ACM */
struct cdcacm_class_header_function_descriptor
{
	u8 bFunctionLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u16 bcdCDC;
} __attribute__	((packed));

struct cdcacm_class_call_management_descriptor
{
	u8 bFunctionLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bmCapabilities;
	u8 bDataInterface;
} __attribute__	((packed));

struct cdcacm_class_abstract_control_descriptor
{
	u8 bFunctionLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bmCapabilities;
} __attribute__	((packed));

struct cdcacm_class_union_function_descriptor
{
	u8 bFunctionLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bMasterInterface;
	u8 bSlaveInterface0;
};	//__attribute__	((packed));

#endif

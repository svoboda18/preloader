/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 */
/* MediaTek Inc. (C) 2010. All rights reserved.
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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#include "platform.h"
#include "circbuf.h"
#include "usbtty.h"

#if 0
#define TTYDBG(fmt,args...) print("[%s] %s %d: "fmt, __FILE__,__FUNCTION__,__LINE__,##args)
#else
#define TTYDBG(fmt,args...) do{}while(0)
#endif

#if 0
#define TTYERR(fmt,args...) print("ERROR![%s] %s %d: "fmt, __FILE__,__FUNCTION__,__LINE__,##args)
#else
#define TTYERR(fmt,args...) do{}while(0)
#endif

/**************************************************************************
 *  USB TTY DEBUG
 **************************************************************************/
#define  mt6573_USB_TTY_DBG_LOG   0

#if mt6573_USB_TTY_DBG_LOG
#define USB_TTY_LOG    print
#else
#define USB_TTY_LOG
#endif

/* USB input/output data buffers */
static circbuf_t usb_input_buffer;
static circbuf_t usb_output_buffer;

static struct mt_dev mt_usb_device[1];
static struct mt_config mt_usb_config[NUM_CONFIGS];
static struct mt_intf *mt_usb_interface[NUM_INTERFACES];
static struct mt_intf mt_usb_data_interface[NUM_DATA_INTERFACES];
static struct mt_altsetting
    mt_usb_data_alternate_interface[NUM_DATA_INTERFACES];
static struct mt_intf mt_usb_comm_interface[NUM_COMM_INTERFACES];
static struct mt_altsetting
    mt_usb_comm_alternate_interface[NUM_COMM_INTERFACES];

#if DIAG_COMPOSITE_PRELOADER
static struct mt_intf mt_usb_diag_interface[1];
static struct mt_altsetting
    mt_usb_diag_alternate_interface[1];
#endif

static struct mt_ep mt_usb_ep[NUM_ENDPOINTS + 1];       /* one extra for control endpoint */
u16 serialstate;

struct urb mt6573_tx_urb;
struct urb mt6573_rx_urb;
struct urb mt6573_ep0_urb;

#if DIAG_COMPOSITE_PRELOADER
#define RX_ENDPOINT DIAG_BULK_OUT_ENDPOINT
#define TX_ENDPOINT DIAG_BULK_IN_ENDPOINT
#else
//MTK_TC1_FEATURE
#define RX_ENDPOINT ACM_BULK_OUT_ENDPOINT
#define TX_ENDPOINT ACM_BULK_IN_ENDPOINT
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE

#if CONFIG_MTK_USB_UNIQUE_SERIAL
#define SERIALNO_LEN 32
#endif
//MTK_TC1_FEATURE

int usb_configured = 0;
int tool_exists = 0;

struct string_descriptor **usb_string_table;

/* USB descriptors */

/* string descriptors */
static u8 language[4] = { 4, USB_DESCRIPTOR_TYPE_STRING, 0x9, 0x4 };
static u8 manufacturer[2 + 2 * (sizeof (USBD_MANUFACTURER) - 1)];
static u8 product[2 + 2 * (sizeof (USBD_PRODUCT_NAME) - 1)];
static u8 configuration[2 + 2 * (sizeof (USBD_CONFIGURATION_STR) - 1)];
static u8 dataInterface[2 + 2 * (sizeof (USBD_DATA_INTERFACE_STR) - 1)];
static u8 commInterface[2 + 2 * (sizeof (USBD_COMM_INTERFACE_STR) - 1)];
//MTK_TC1_FEATURE
static u8 iserial[2 + 2 * (sizeof (USBD_ISERIAL_STR) - 1)];
#if DIAG_COMPOSITE_PRELOADER
static u8 serialInterface[2 + 2 * (sizeof (USBD_DIAG_INTERFACE_STR) - 1)];
#endif
//MTK_TC1_FEATURE

static struct string_descriptor *usbtty_string_table[] = {
    (struct string_descriptor *) language,
    (struct string_descriptor *) manufacturer,
    (struct string_descriptor *) product,
    (struct string_descriptor *) configuration,
    (struct string_descriptor *) dataInterface,
    (struct string_descriptor *) commInterface,
//MTK_TC1_FEATURE
    (struct string_descriptor *) iserial,
#if DIAG_COMPOSITE_PRELOADER
    (struct string_descriptor *) serialInterface,
#endif
//MTK_TC1_FEATURE
};

/* device descriptor */
static struct device_descriptor device_descriptor = {
    sizeof (struct device_descriptor),
    USB_DESCRIPTOR_TYPE_DEVICE,
    USB_BCD_VERSION,
    USBDL_DEVICE_CLASS,
    USBDL_DEVICE_SUBCLASS,
    USBDL_DEVICE_PROTOCOL,
    EP0_MAX_PACKET_SIZE,
    USBD_VENDORID,
    USBD_PRODUCTID,
    USBD_BCD_DEVICE,
    STR_MANUFACTURER,
    STR_PRODUCT,
//MTK_TC1_FEATURE
#if CONFIG_MTK_USB_UNIQUE_SERIAL
    STR_ISERIAL,
#else
//MTK_TC1_FEATURE
    0,
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
    NUM_CONFIGS
};

/* device qualifier descriptor */
static struct device_qualifier_descriptor device_qualifier_descriptor = {
    sizeof (struct device_qualifier_descriptor),
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    USB_BCD_VERSION,
    USBDL_DEVICE_CLASS,
    USBDL_DEVICE_SUBCLASS,
    USBDL_DEVICE_PROTOCOL,
    EP0_MAX_PACKET_SIZE_FULL,
    NUM_CONFIGS,
};

/* configuration descriptor */
static struct configuration_descriptor config_descriptors[NUM_CONFIGS] = {
    {
     sizeof (struct configuration_descriptor),
     USB_DESCRIPTOR_TYPE_CONFIGURATION,
     (sizeof (struct configuration_descriptor) * NUM_CONFIGS) +
     (sizeof (struct interface_descriptor) * NUM_INTERFACES) +
     (sizeof (struct cdcacm_class_header_function_descriptor)) +
     (sizeof (struct cdcacm_class_abstract_control_descriptor)) +
     (sizeof (struct cdcacm_class_union_function_descriptor)) +
     (sizeof (struct cdcacm_class_call_management_descriptor)) +
     (sizeof (struct endpoint_descriptor) * NUM_ENDPOINTS),
     NUM_INTERFACES,
     1,
     STR_CONFIG,
     0xc0,
     USBD_MAXPOWER},
};

static struct interface_descriptor interface_descriptors[NUM_INTERFACES] = {
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
/* interface_descriptors[0]: data interface
 * interface_descriptors[1]: communication interface
 * interface_descriptors[2]: diag interface */
    {
     sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     0,
     0,
     NUM_COMM_ENDPOINTS,
     USBDL_COMM_INTERFACE_CLASS,
     USBDL_COMM_INTERFACE_SUBCLASS,
     USBDL_COMM_INTERFACE_PROTOCOL,
     STR_COMM_INTERFACE},
        {
     sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     1,
     0,
     NUM_DATA_ENDPOINTS,
     USBDL_DATA_INTERFACE_CLASS,
     USBDL_DATA_INTERFACE_SUBCLASS,
     USBDL_DATA_INTERFACE_PROTOCOL,
     STR_DATA_INTERFACE},
    {
     sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     2,
     0,
     NUM_DIAG_ENDPOINTS,
     USBDL_DIAG_INTERFACE_CLASS,
     USBDL_DIAG_INTERFACE_SUBCLASS,
     USBDL_DIAG_INTERFACE_PROTOCOL,
     STR_DIAG_INTERFACE},
#else
//MTK_TC1_FEATURE
/* interface_descriptors[0]: data interface          *
 * interface_descriptors[1]: communication interface */
    {
     sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     0,
     0,
     NUM_DATA_ENDPOINTS,
     USBDL_DATA_INTERFACE_CLASS,
     USBDL_DATA_INTERFACE_SUBCLASS,
     USBDL_DATA_INTERFACE_PROTOCOL,
     STR_DATA_INTERFACE},
    {
     sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     1,
     0,
     NUM_COMM_ENDPOINTS,
     USBDL_COMM_INTERFACE_CLASS,
     USBDL_COMM_INTERFACE_SUBCLASS,
     USBDL_COMM_INTERFACE_PROTOCOL,
     STR_COMM_INTERFACE},
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

static struct cdcacm_class_header_function_descriptor
    header_function_descriptor = {
    0x05,
    0x24,
    0x00,                       /* 0x00 for header functional descriptor */
    0x0110,
};

static struct cdcacm_class_abstract_control_descriptor
    abstract_control_descriptor = {
    0x04,
    0x24,
    0x02,                       /* 0x02 for abstract control descriptor */
    0x0f,
};

struct cdcacm_class_union_function_descriptor union_function_descriptor = {
    0x05,
    0x24,
    0x06,                       /* 0x06 for union functional descriptor */
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    0x00,
    0x01,
#else
//MTK_TC1_FEATURE
    0x01,
    0x00,
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

struct cdcacm_class_call_management_descriptor call_management_descriptor = {
    0x05,
    0x24,
    0x01,                       /* 0x01 for call management descriptor */
    0x03,
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    0x01,
#else
//MTK_TC1_FEATURE
    0x00,
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

static struct endpoint_descriptor hs_ep_descriptors[NUM_ENDPOINTS] = {
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_HS_PKTSIZE,
     0x10                       /* polling interval is every 16 frames */
    },
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_HS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_HS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_DIAG_IN_HS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_DIAG_OUT_HS_PKTSIZE,
     0},
#else
//MTK_TC1_FEATURE
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_HS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_HS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_HS_PKTSIZE,
     0x10                       /* polling interval is every 16 frames */
     },
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

static struct endpoint_descriptor fs_ep_descriptors[NUM_ENDPOINTS] = {
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_FS_PKTSIZE,
     0x10                       /* polling interval is every 16 frames */
     },
        {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_FS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_FS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_DIAG_IN_FS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_DIAG_OUT_FS_PKTSIZE,
     0},
#else
//MTK_TC1_FEATURE
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_FS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_FS_PKTSIZE,
     0},
    {
     sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_FS_PKTSIZE,
     0x10                       /* polling interval is every 16 frames */
     },
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

static struct endpoint_descriptor
    *hs_data_ep_descriptor_ptrs[NUM_DATA_ENDPOINTS] = {
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    &(hs_ep_descriptors[1]),
    &(hs_ep_descriptors[2]),
#else
//MTK_TC1_FEATURE
    &(hs_ep_descriptors[0]),
    &(hs_ep_descriptors[1]),
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

static struct endpoint_descriptor
    *hs_comm_ep_descriptor_ptrs[NUM_COMM_ENDPOINTS] = {
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    &(hs_ep_descriptors[0]),
#else
//MTK_TC1_FEATURE
    &(hs_ep_descriptors[2]),
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};


static struct endpoint_descriptor
    *hs_diag_ep_descriptor_ptrs[2] = {
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    &(hs_ep_descriptors[3]),
    &(hs_ep_descriptors[4]),
#else
//MTK_TC1_FEATURE
    &(hs_ep_descriptors[2]),
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

static struct endpoint_descriptor
    *fs_data_ep_descriptor_ptrs[NUM_DATA_ENDPOINTS] = {
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    &(fs_ep_descriptors[1]),
    &(fs_ep_descriptors[2]),
#else
//MTK_TC1_FEATURE
    &(fs_ep_descriptors[0]),
    &(fs_ep_descriptors[1]),
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

static struct endpoint_descriptor
    *fs_comm_ep_descriptor_ptrs[NUM_COMM_ENDPOINTS] = {
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    &(fs_ep_descriptors[0]),
#else
//MTK_TC1_FEATURE
    &(fs_ep_descriptors[2]),
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
};

//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
static struct endpoint_descriptor
    *fs_diag_ep_descriptor_ptrs[2] = {
    &(fs_ep_descriptors[3]),
    &(fs_ep_descriptors[4]),
};
#endif
//MTK_TC1_FEATURE

static void
str2wide (char *str, u16 * wide)
{
    int i;

    for (i = 0; i < strlen (str) && str[i]; i++)
        wide[i] = (u16) str[i];
}

int usbdl_configured (void);

static void buf_to_ep (circbuf_t * buf);
static int ep_to_buf (circbuf_t * buf);

void usbdl_poll (void);
void service_interrupts (void);

struct urb *
usb_alloc_urb (struct mt_dev *device, struct mt_ep *endpoint)
{
    struct urb *urb;
    int ep_num = 0;
    int dir = 0;

    ep_num = endpoint->endpoint_address & USB_EP_NUM_MASK;
    dir = endpoint->endpoint_address & USB_EP_DIR_MASK;

    if (ep_num == 0)
      {
          urb = &mt6573_ep0_urb;
      }
    else if (dir)
      {                         // tx
          urb = &mt6573_tx_urb;
      }
    else
      {                         // rx
          urb = &mt6573_rx_urb;
      }

    memset (urb, 0, sizeof (struct urb));
    urb->endpoint = endpoint;
    urb->device = device;
    urb->buffer = (u8 *) urb->buffer_data;
    urb->buffer_length = sizeof (urb->buffer_data);

    return urb;
}

//MTK_TC1_FEATURE
#if CONFIG_MTK_USB_UNIQUE_SERIAL
static char udc_chr[32] = {"ABCDEFGHIJKLMNOPQRSTUVWSYZ456789"};
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

int get_serial(u64 hwkey, u32 chipid, char ser[SERIALNO_LEN])
{
    u16 hashkey[4];
    int idx, ser_idx;
    u32 digit, id;
    u64 tmp = hwkey;

    memset(ser, 0x00, SERIALNO_LEN);

    /* split to 4 key with 16-bit width each */
    tmp = hwkey;
    for (idx = 0; idx < ARRAY_SIZE(hashkey); idx++) {
        hashkey[idx] = (u16)(tmp & 0xffff);
        tmp >>= 16;
    }

    /* hash the key with chip id */
    id = chipid;
    for (idx = 0; idx < ARRAY_SIZE(hashkey); idx++) {
        digit = (id % 10);
        hashkey[idx] = (hashkey[idx] >> digit) | (hashkey[idx] << (16-digit));
        id = (id / 10);
    }

    /* generate serail using hashkey */
    ser_idx = 0;
    for (idx = 0; idx < ARRAY_SIZE(hashkey); idx++) {
        ser[ser_idx++] = (hashkey[idx] & 0x001f);
        ser[ser_idx++] = (hashkey[idx] & 0x00f8) >> 3;
        ser[ser_idx++] = (hashkey[idx] & 0x1f00) >> 8;
        ser[ser_idx++] = (hashkey[idx] & 0xf800) >> 11;
    }
    for (idx = 0; idx < ser_idx; idx++)
        ser[idx] = udc_chr[(int)ser[idx]];
    ser[ser_idx] = 0x00;
    return 0;
}
#endif
//MTK_TC1_FEATURE

/*
 * Initialize the usb client port.
 *
 */
int
usbdl_init (void)
{
    /* initialize usb variables */
    extern int usb_configured;
    usb_configured = 0;
    extern EP0_STATE ep0_state;
    ep0_state = EP0_IDLE;
    extern int set_address;
    set_address = 0;
    extern u32 fifo_addr;
    fifo_addr = FIFO_ADDR_START;

    int i;
    struct string_descriptor *string;

//MTK_TC1_FEATURE
    #if CONFIG_MTK_USB_UNIQUE_SERIAL
    u64 key = 0;
    u8 serial_num[SERIALNO_LEN];
    #endif
//MTK_TC1_FEATURE

    tool_exists = 0;

    /* prepare buffers... */
    buf_input_init (&usb_input_buffer, USBTTY_BUFFER_SIZE);
    buf_output_init (&usb_output_buffer, USBTTY_BUFFER_SIZE);

    /* initialize string descriptor array */
    string = (struct string_descriptor *) manufacturer;
    string->bDescriptorType = USB_DESCRIPTOR_TYPE_STRING;
    string->bLength = sizeof (manufacturer);
    str2wide (USBD_MANUFACTURER, string->wData);

    string = (struct string_descriptor *) product;
    string->bLength = sizeof (product);
    string->bDescriptorType = USB_DESCRIPTOR_TYPE_STRING;
    str2wide (USBD_PRODUCT_NAME, string->wData);

    string = (struct string_descriptor *) configuration;
    string->bLength = sizeof (configuration);
    string->bDescriptorType = USB_DESCRIPTOR_TYPE_STRING;
    str2wide (USBD_CONFIGURATION_STR, string->wData);

    string = (struct string_descriptor *) dataInterface;
    string->bLength = sizeof (dataInterface);
    string->bDescriptorType = USB_DESCRIPTOR_TYPE_STRING;
    str2wide (USBD_DATA_INTERFACE_STR, string->wData);

    string = (struct string_descriptor *) commInterface;
    string->bLength = sizeof (commInterface);
    string->bDescriptorType = USB_DESCRIPTOR_TYPE_STRING;
    str2wide (USBD_COMM_INTERFACE_STR, string->wData);

//MTK_TC1_FEATURE
#if CONFIG_MTK_USB_UNIQUE_SERIAL
    memset(iserial, 0, sizeof(iserial));
    memset(serial_num, 0, sizeof(serial_num));

    /*Iserial string adding.*/
    string = (struct string_descriptor *) iserial;
    string->bDescriptorType = USB_DESCRIPTOR_TYPE_STRING;

      key = seclib_get_devinfo_with_index(13);
      key = (key << 32) | (u32)seclib_get_devinfo_with_index(12);

     if (key != 0)
     {
	get_serial(key, 0x6582, serial_num);
     }
     else
     {
	memcpy(serial_num, "0123456789ABCDEF", 16);
     }

    string->bLength = sizeof (iserial);
    str2wide (serial_num, string->wData);
#endif

#if DIAG_COMPOSITE_PRELOADER
    string = (struct string_descriptor *) serialInterface;
    string->bLength = sizeof (serialInterface);
    string->bDescriptorType = USB_DESCRIPTOR_TYPE_STRING;
    str2wide (STR_DIAG_INTERFACE, string->wData);
#endif
//MTK_TC1_FEATURE

    /* Now, initialize the string table for ep0 handling */
    usb_string_table = usbtty_string_table;

    /* device instance initialization */
    memset (mt_usb_device, 0, sizeof (struct mt_dev));
    mt_usb_device->device_descriptor = &device_descriptor;
    mt_usb_device->device_qualifier_descriptor = &device_qualifier_descriptor;
    mt_usb_device->configurations = NUM_CONFIGS;
    mt_usb_device->configuration_array = mt_usb_config;
    mt_usb_device->speed = 0;   //1: high-speed, 0: full-speed
    mt_usb_device->endpoint_array = mt_usb_ep;
    mt_usb_device->max_endpoints = 1;
    mt_usb_device->maxpacketsize = 64;

    /* configuration instance initialization */
    memset (mt_usb_config, 0, sizeof (struct mt_config));
    mt_usb_config->interfaces = NUM_INTERFACES;
    mt_usb_config->configuration_descriptor = config_descriptors;
    mt_usb_config->interface_array = mt_usb_interface;

//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    mt_usb_interface[0] = mt_usb_comm_interface;
    mt_usb_interface[1] = mt_usb_data_interface;
    mt_usb_interface[2] = mt_usb_diag_interface;
#else
//MTK_TC1_FEATURE
    mt_usb_interface[0] = mt_usb_data_interface;
    mt_usb_interface[1] = mt_usb_comm_interface;
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
    /* data interface instance */
    memset (mt_usb_data_interface, 0,
            NUM_DATA_INTERFACES * sizeof (struct mt_intf));
    mt_usb_data_interface->alternates = 1;
    mt_usb_data_interface->altsetting_array = mt_usb_data_alternate_interface;

    /* data alternates instance */
    memset (mt_usb_data_alternate_interface, 0,
            NUM_DATA_INTERFACES * sizeof (struct mt_altsetting));

//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    mt_usb_data_alternate_interface->interface_descriptor =
        &interface_descriptors[1];
#else
//MTK_TC1_FEATURE
    mt_usb_data_alternate_interface->interface_descriptor =
        &interface_descriptors[0];
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
    mt_usb_data_alternate_interface->endpoints = NUM_DATA_ENDPOINTS;
    mt_usb_data_alternate_interface->endpoints_descriptor_array =
        fs_data_ep_descriptor_ptrs;

    /* communication interface instance */
    memset (mt_usb_comm_interface, 0,
            NUM_COMM_INTERFACES * sizeof (struct mt_intf));
    mt_usb_comm_interface->alternates = 1;
    mt_usb_comm_interface->altsetting_array = mt_usb_comm_alternate_interface;

    /* communication alternates instance */
    /* contains communication class specific interface descriptors */
    memset (mt_usb_comm_alternate_interface, 0,
            NUM_COMM_INTERFACES * sizeof (struct mt_altsetting));
//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    mt_usb_comm_alternate_interface->interface_descriptor =
        &interface_descriptors[0];
#else
//MTK_TC1_FEATURE
    mt_usb_comm_alternate_interface->interface_descriptor =
        &interface_descriptors[1];
//MTK_TC1_FEATURE
#endif
//MTK_TC1_FEATURE
    mt_usb_comm_alternate_interface->header_function_descriptor =
        &header_function_descriptor;
    mt_usb_comm_alternate_interface->abstract_control_descriptor =
        &abstract_control_descriptor;
    mt_usb_comm_alternate_interface->union_function_descriptor =
        &union_function_descriptor;
    mt_usb_comm_alternate_interface->call_management_descriptor =
        &call_management_descriptor;
    mt_usb_comm_alternate_interface->endpoints = NUM_COMM_ENDPOINTS;
    mt_usb_comm_alternate_interface->endpoints_descriptor_array =
        fs_comm_ep_descriptor_ptrs;

//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    /* DIAG serial interface instance */
    memset (mt_usb_diag_interface, 0,
            NUM_DIAG_INTERFACES * sizeof (struct mt_intf));
    mt_usb_diag_interface->alternates = 1;
    mt_usb_diag_interface->altsetting_array = mt_usb_diag_alternate_interface;

    /* DIAG serial alternates instance */
    memset (mt_usb_diag_alternate_interface, 0,
            NUM_DIAG_INTERFACES * sizeof (struct mt_altsetting));
    mt_usb_diag_alternate_interface->interface_descriptor =
        &interface_descriptors[2];
    mt_usb_diag_alternate_interface->endpoints = NUM_DIAG_ENDPOINTS;
    mt_usb_diag_alternate_interface->endpoints_descriptor_array =
        fs_diag_ep_descriptor_ptrs;
#endif
//MTK_TC1_FEATURE

    /* endpoint instances */
    memset (&mt_usb_ep[0], 0, sizeof (struct mt_ep));
    mt_usb_ep[0].endpoint_address = 0;
    mt_usb_ep[0].rcv_packetSize = EP0_MAX_PACKET_SIZE;
    mt_usb_ep[0].tx_packetSize = EP0_MAX_PACKET_SIZE;
    mt_setup_ep (mt_usb_device, 0, &mt_usb_ep[0]);

    for (i = 1; i <= NUM_ENDPOINTS; i++)
      {
          memset (&mt_usb_ep[i], 0, sizeof (struct mt_ep));

          mt_usb_ep[i].endpoint_address =
              fs_ep_descriptors[i - 1].bEndpointAddress;

          mt_usb_ep[i].rcv_packetSize =
              fs_ep_descriptors[i - 1].wMaxPacketSize;

          mt_usb_ep[i].tx_packetSize =
              fs_ep_descriptors[i - 1].wMaxPacketSize;

          if (mt_usb_ep[i].endpoint_address & USB_DIR_IN)
              mt_usb_ep[i].tx_urb =
                  usb_alloc_urb (mt_usb_device, &mt_usb_ep[i]);
          else
              mt_usb_ep[i].rcv_urb =
                  usb_alloc_urb (mt_usb_device, &mt_usb_ep[i]);
      }

    udc_enable (mt_usb_device);
    return 0;
}

/*********************************************************************************/

static void
buf_to_ep (circbuf_t * buf)
{
    int i;

    if (!usbdl_configured ())
      {
          return;
      }

    if (buf->size)
      {

          struct mt_ep *endpoint = &mt_usb_ep[TX_ENDPOINT];
          struct urb *current_urb = endpoint->tx_urb;

          int space_avail;
          int popnum;

          /* Break buffer into urb sized pieces, and link each to the endpoint */
          while (buf->size > 0)
            {

                if (!current_urb
                    || (space_avail =
                        current_urb->buffer_length -
                        current_urb->actual_length) <= 0)
                  {
                      print ("write_buffer, no available urbs\n");
                      return;
                  }

                //buf_pop (buf, dest, popnum);
                popnum = buf_pop (buf, current_urb->buffer +
                                  current_urb->actual_length,
                                  MIN (space_avail, buf->size));

                /* update the used space of current_urb */
                current_urb->actual_length += popnum;

                /* nothing is in the buffer or the urb can hold no more data */
                if (popnum == 0)
                    break;

                /* if the endpoint is idle, trigger the tx transfer */
                if (endpoint->last == 0)
                  {
                      mt_ep_write (endpoint);
                  }

            }                   /* end while */
      }                         /* end if buf->size */

    return;
}

static int
ep_to_buf (circbuf_t * buf)
{
    struct mt_ep *endpoint;
    int nb;

    if (!usbdl_configured())
        return 0;

    endpoint = &mt_usb_ep[RX_ENDPOINT];

    if (!endpoint->rcv_urb)
        return 0;

    nb = endpoint->rcv_urb->actual_length;

    if (endpoint->rcv_urb && nb)
      {
          buf_push (buf, (char *) endpoint->rcv_urb->buffer,
                    endpoint->rcv_urb->actual_length);
          endpoint->rcv_urb->actual_length = 0;

          return nb;
      }

    return 0;
}

int
tool_is_present (void)
{
    return tool_exists;
}

void
tool_state_update (int state)
{
    tool_exists = state;

    return;
}

int
usbdl_configured (void)
{
    return usb_configured;
}

void
enable_highspeed (void)
{

    int i;

    mt_usb_device->speed = 1;   //1: high-speed, 0: full-speed
    mt_usb_data_alternate_interface->endpoints_descriptor_array =
        hs_data_ep_descriptor_ptrs;
    mt_usb_comm_alternate_interface->endpoints_descriptor_array =
        hs_comm_ep_descriptor_ptrs;

//MTK_TC1_FEATURE
#if DIAG_COMPOSITE_PRELOADER
    mt_usb_diag_alternate_interface->endpoints_descriptor_array =
        hs_diag_ep_descriptor_ptrs;
#endif
//MTK_TC1_FEATURE

    for (i = 1; i <= NUM_ENDPOINTS; i++)
      {

          mt_usb_ep[i].endpoint_address =
              hs_ep_descriptors[i - 1].bEndpointAddress;

          mt_usb_ep[i].rcv_packetSize =
              hs_ep_descriptors[i - 1].wMaxPacketSize;

          mt_usb_ep[i].tx_packetSize =
              hs_ep_descriptors[i - 1].wMaxPacketSize;
      }

    return;
}

//#define usbtty_event_log print
#define usbtty_event_log

/*********************************************************************************/
void
config_usbtty (struct mt_dev *device)
{

    int i;

    usb_configured = 1;
    mt_usb_device->max_endpoints = NUM_ENDPOINTS + 1;
    for (i = 0; i <= NUM_ENDPOINTS; i++)
      {
          mt_setup_ep (mt_usb_device, mt_usb_ep[i].endpoint_address & (~USB_DIR_IN), &mt_usb_ep[i]);
      }

    return;
}

/*********************************************************************************/



/* Used to emulate interrupt handling */
void
usbdl_poll (void)
{
    /* New interrupts? */
    service_interrupts ();

    /* Write any output data to host buffer (do this before checking interrupts to avoid missing one) */
    buf_to_ep (&usb_output_buffer);

    /* Check for new data from host.. (do this after checking interrupts to get latest data) */
    ep_to_buf (&usb_input_buffer);
}

extern ulong get_timer(ulong base);

void usbdl_flush(void)
{
    u32 start_time = get_timer(0);

    while (((usb_output_buffer.size) > 0) || mt_ep_busy(&mt_usb_ep[TX_ENDPOINT]))
    {
        usbdl_poll ();

        if(get_timer(start_time) > 300)
        {
            print ("usbdl_flush timeout\n");
            break;
        }
    }

    return;
}

void
service_interrupts (void)
{

    volatile u8 intrtx, intrrx, intrusb;
    /* polling interrupt status for incoming interrupts and service it */
    u16 rxcsr;

    intrtx = __raw_readb (INTRTX);
    __raw_writew(intrtx, INTRTX);
    intrrx = __raw_readb (INTRRX);
    __raw_writew(intrrx, INTRRX);
    intrusb = __raw_readb (INTRUSB);
    __raw_writeb(intrusb, INTRUSB);

    intrusb &= ~INTRUSB_SOF;

    if (intrtx | intrrx | intrusb)
    {
        mt_udc_irq (intrtx, intrrx, intrusb);
    }

}

/* API for preloader download engine */

void mt_usbtty_flush(void) 
{
    usbdl_flush();
}

/*
 * Test whether a character is in the RX buffer
 */
int
mt_usbtty_tstc (void)
{

    usbdl_poll ();
    return (usb_input_buffer.size > 0);
}


/* get a single character and copy it to usb_input_buffer */
int
mt_usbtty_getc (void)
{

    char c;

    while (usb_input_buffer.size <= 0)
      {
          usbdl_poll ();
      }

    buf_pop (&usb_input_buffer, &c, 1);
    return c;
}

/* get n characters and copy it to usb_input_buffer */
int
mt_usbtty_getcn (int count, char *buf)
{

    int data_count = 0;
    int tmp = 0;

    /* wait until received 'count' bytes of data */
    while (data_count < count)
      {
          if (usb_input_buffer.size < 512)
              usbdl_poll ();
          if (usb_input_buffer.size > 0)
            {
                tmp = usb_input_buffer.size;
                if (data_count + tmp > count)
                  {
                      tmp = count - data_count;
                  }

                //print("usb_input_buffer.data = %s\n",usb_input_buffer.data);
                buf_pop (&usb_input_buffer, buf + data_count, tmp);
                data_count += tmp;
            }
      }

    return 0;
}

void
mt_usbtty_putc (const char c, int flush)
{

    buf_push (&usb_output_buffer, &c, 1);

    /* Poll at end to handle new data... */
    if (((usb_output_buffer.size) >= usb_output_buffer.totalsize) || flush)
      {
          usbdl_poll ();
      }

    return;
}

void
mt_usbtty_putcn (int count, char *buf, int flush)
{

    char *cp = buf;

    while (count > 0)
      {
          if (count > 512)
            {
                buf_push (&usb_output_buffer, cp, 512);
                cp += 512;
                count -= 512;
            }
          else
            {
                buf_push (&usb_output_buffer, cp, count);
                cp += count;
                count = 0;
            }

          if (((usb_output_buffer.size) >= usb_output_buffer.totalsize)
              || flush)
            {
                usbdl_poll ();
            }
      }

    return;
}

void
mt_usbtty_puts (const char *str)
{
    int len = strlen (str);
    int maxlen = usb_output_buffer.totalsize;
    int space, n;

    /* break str into chunks < buffer size, if needed */
    while (len > 0)
      {
          space = maxlen - usb_output_buffer.size;

          /* Empty buffer here, if needed, to ensure space... */
#if 0
          if (space <= 0)
            {
                ASSERT (0);
            }
#endif
          n = MIN (space, MIN (len, maxlen));

          buf_push (&usb_output_buffer, str, n);

          str += n;
          len -= n;

          service_interrupts ();
      }

    /* Poll at end to handle new data... */
    usbdl_poll ();
    usbdl_flush();
}

int
mt_usbtty_query_data_size (void)
{

    if (usb_input_buffer.size < 512)
      {
          if (usbdl_configured ())
            {
                service_interrupts ();
                ep_to_buf (&usb_input_buffer);
            }
      }

    return usb_input_buffer.size;
}


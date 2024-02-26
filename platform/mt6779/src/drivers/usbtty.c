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
#define  USB_TTY_DBG_LOG   0
#define  MOD_TAG    "[UTTY]"

#if USB_TTY_DBG_LOG
#define TTY_LOG(_format, ...) do{  \
        print(MOD_TAG " " #_format "\n", ##__VA_ARGS__);\
        } while(0)
#else
#define TTY_LOG(_format, ...)
#endif

/* USB input/output data buffers */
static circbuf_t usb_input_buffer;
static circbuf_t usb_output_buffer;

static struct mt_dev mt_usb_dev[1];
static struct mt_config mt_usb_config[NUM_CONFIGS];
static struct mt_intf *mt_usb_interface[NUM_INTERFACES];
static struct mt_intf mt_usb_data_interface[NUM_DATA_INTERFACES];
static struct mt_altsetting mt_usb_data_alt_if[NUM_DATA_INTERFACES];
static struct mt_intf mt_usb_comm_interface[NUM_COMM_INTERFACES];
static struct mt_altsetting mt_usb_comm_alt_if[NUM_COMM_INTERFACES];
#ifdef DIAG_COMPOSITE_PRELOADER
    static struct mt_intf mt_usb_diag_interface[NUM_DIAG_INTERFACES];
    static struct mt_altsetting mt_usb_diag_alt_if[NUM_DIAG_INTERFACES];
#endif

/* one extra for control endpoint */
static struct mt_ep mt_usb_ep[NUM_ENDPOINTS + 1];
u16 serialstate;

struct urb tx_urb;
struct urb rx_urb;
struct urb ep0_urb;

#ifdef DIAG_COMPOSITE_PRELOADER
    #define RX_ENDPOINT DIAG_BULK_OUT_ENDPOINT
    #define TX_ENDPOINT DIAG_BULK_IN_ENDPOINT
#else
    #define RX_ENDPOINT ACM_BULK_OUT_ENDPOINT
    #define TX_ENDPOINT ACM_BULK_IN_ENDPOINT
#endif

//#define CONFIG_MTK_USB_UNIQUE_SERIAL

static int g_usb_configured = 0;
#define SET_USB_CONFIGUTED do{g_usb_configured = 1;}while(0);
#define SET_USB_UNCONFIGUTED do{g_usb_configured = 0;}while(0);

static int g_tool_exists = 0;
#define SET_TOOL_NOT_EXIST do{g_tool_exists = 0;}while(0);
#define SET_TOOL_EXIST do{g_tool_exists = 1;}while(0);

struct string_descriptor **usb_string_table;

/* USB descriptors */

/* string descriptors */
static u8 language[4]__attribute__((aligned(4))) = { 4, USB_DESCRIPTOR_TYPE_STRING, 0x9, 0x4 };
static u8 manufacturer[2 + 2 * (sizeof (USBD_MANUFACTURER) - 1)]__attribute__((aligned(4)));
static u8 product[2 + 2 * (sizeof (USBD_PRODUCT_NAME) - 1)]__attribute__((aligned(4)));
static u8 configuration[2 + 2 * (sizeof (USBD_CONFIGURATION_STR) - 1)]__attribute__((aligned(4)));
static u8 dataInterface[2 + 2 * (sizeof (USBD_DATA_INTERFACE_STR) - 1)]__attribute__((aligned(4)));
static u8 commInterface[2 + 2 * (sizeof (USBD_COMM_INTERFACE_STR) - 1)]__attribute__((aligned(4)));
#ifdef DIAG_COMPOSITE_PRELOADER
    static u8 serialInterface[2 + 2 * (sizeof (USBD_DIAG_INTERFACE_STR) - 1)]__attribute__((aligned(4)));
#endif

static struct string_descriptor *usbtty_string_table[] = {
    (struct string_descriptor *) language,
    (struct string_descriptor *) manufacturer,
    (struct string_descriptor *) product,
    (struct string_descriptor *) configuration,
    (struct string_descriptor *) dataInterface,
    (struct string_descriptor *) commInterface,
#ifdef DIAG_COMPOSITE_PRELOADER
    (struct string_descriptor *) serialInterface,
#endif
};

/* device descriptor */
static struct device_descriptor device_desc = {
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
#ifdef CONFIG_MTK_USB_UNIQUE_SERIAL
    STR_ISERIAL,
#else
    0,
#endif
    NUM_CONFIGS
};

static struct bos_descriptor bos_descriptor = {
    sizeof(struct bos_descriptor),
    USB_DESCRIPTOR_TYPE_BOS,
    sizeof(struct bos_descriptor) + sizeof(struct ext_cap_descriptor) + sizeof(struct ss_cap_descriptor),
    2
};

static struct ext_cap_descriptor ext_cap_descriptor = {
    sizeof(struct ext_cap_descriptor),
    USB_DESCRIPTOR_TYPE_DEVICE_CAPABILITY,
    DEV_CAP_USB20_EXT,
    SUPPORT_LPM_PROTOCOL
};

static struct ss_cap_descriptor ss_cap_descriptor = {
    sizeof(struct ss_cap_descriptor),
    USB_DESCRIPTOR_TYPE_DEVICE_CAPABILITY,
    DEV_CAP_SS_USB,
    NOT_LTM_CAPABLE,
    (SUPPORT_FS_OP | SUPPORT_HS_OP | SUPPORT_5GBPS_OP),
    SUPPORT_FS_OP,
    DEFAULT_U1_DEV_EXIT_LAT,
    DEFAULT_U2_DEV_EXIT_LAT
};

/* device qualifier descriptor */
static struct device_qualifier_descriptor device_qualifier_desc = {
    sizeof (struct device_qualifier_descriptor),
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    USB_BCD_VERSION,
    USBDL_DEVICE_CLASS,
    USBDL_DEVICE_SUBCLASS,
    USBDL_DEVICE_PROTOCOL,
    EP0_MAX_PACKET_SIZE,
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
#ifdef DIAG_COMPOSITE_PRELOADER
/* interface_descriptors[0]: communication interface
 * interface_descriptors[1]: data interface
 * interface_descriptors[2]: diag interface */
    {sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     0,
     0,
     NUM_COMM_ENDPOINTS,
     USBDL_COMM_INTERFACE_CLASS,
     USBDL_COMM_INTERFACE_SUBCLASS,
     USBDL_COMM_INTERFACE_PROTOCOL,
     STR_COMM_INTERFACE},
    {sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     1,
     0,
     NUM_DATA_ENDPOINTS,
     USBDL_DATA_INTERFACE_CLASS,
     USBDL_DATA_INTERFACE_SUBCLASS,
     USBDL_DATA_INTERFACE_PROTOCOL,
     STR_DATA_INTERFACE},
    {sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     2,
     0,
     NUM_DIAG_ENDPOINTS,
     USBDL_DIAG_INTERFACE_CLASS,
     USBDL_DIAG_INTERFACE_SUBCLASS,
     USBDL_DIAG_INTERFACE_PROTOCOL,
     STR_DIAG_INTERFACE}
#else
/* interface_descriptors[0]: data interface          *
 * interface_descriptors[1]: communication interface */
    {sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     1,
     0,
     NUM_DATA_ENDPOINTS,
     USBDL_DATA_INTERFACE_CLASS,
     USBDL_DATA_INTERFACE_SUBCLASS,
     USBDL_DATA_INTERFACE_PROTOCOL,
     STR_DATA_INTERFACE},
    {sizeof (struct interface_descriptor),
     USB_DESCRIPTOR_TYPE_INTERFACE,
     0,
     0,
     NUM_COMM_ENDPOINTS,
     USBDL_COMM_INTERFACE_CLASS,
     USBDL_COMM_INTERFACE_SUBCLASS,
     USBDL_COMM_INTERFACE_PROTOCOL,
     STR_COMM_INTERFACE}
#endif
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
#ifdef DIAG_COMPOSITE_PRELOADER
    0x00,
    0x01,
#else
    0x01,
    0x00,
#endif
};

struct cdcacm_class_call_management_descriptor call_management_descriptor = {
    0x05,
    0x24,
    0x01,                       /* 0x01 for call management descriptor */
    0x03,
#ifdef DIAG_COMPOSITE_PRELOADER
    0x01,
#else
    0x00,
#endif
};

static struct endpoint_descriptor ss_ep_descriptors[NUM_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_SS_PKTSIZE,
     0x10                       /* polling interval is every 16 frames */
    },
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_SS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_SS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_DIAG_IN_SS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_DIAG_OUT_SS_PKTSIZE,
     0}
#else
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_SS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_SS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_SS_PKTSIZE,
     0x10}                       /* polling interval is every 16 frames */
#endif
};

//Currently all ep can share the same companion descriptors
static struct ss_ep_comp_descriptor ss_ep_comp_descriptor = {
    sizeof(struct ss_ep_comp_descriptor),
    USB_DESCRIPTOR_TYPE_SS_ENDPOINT_COMPANION,
};

static struct endpoint_descriptor hs_ep_descriptors[NUM_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_HS_PKTSIZE,
     0x10                       /* polling interval is every 16 frames */
    },
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_HS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_HS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_DIAG_IN_HS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_DIAG_OUT_HS_PKTSIZE,
     0}
#else
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_HS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_HS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_HS_PKTSIZE,
     0x10}                       /* polling interval is every 16 frames */
#endif
};

static struct endpoint_descriptor fs_ep_descriptors[NUM_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_FS_PKTSIZE,
     0x10                       /* polling interval is every 16 frames */
     },
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_FS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_FS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_DIAG_IN_FS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_DIAG_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_DIAG_OUT_FS_PKTSIZE,
     0}
#else
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_OUT_ENDPOINT | USB_DIR_OUT,
     USB_EP_XFER_BULK,
     USBD_SERIAL_OUT_FS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_SERIAL_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_BULK,
     USBD_SERIAL_IN_FS_PKTSIZE,
     0},
    {sizeof (struct endpoint_descriptor),
     USB_DESCRIPTOR_TYPE_ENDPOINT,
     USBD_INT_IN_ENDPOINT | USB_DIR_IN,
     USB_EP_XFER_INT,
     USBD_INT_IN_FS_PKTSIZE,
     0x10                       /* polling interval is every 16 frames */
     }
#endif
};

static struct endpoint_descriptor
    *ss_data_ep_descriptor_ptrs[NUM_DATA_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    &(ss_ep_descriptors[1]),
    &(ss_ep_descriptors[2]),
#else
    &(ss_ep_descriptors[0]),
    &(ss_ep_descriptors[1]),
#endif
};

static struct endpoint_descriptor
    *ss_comm_ep_descriptor_ptrs[NUM_COMM_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    &(ss_ep_descriptors[0]),
#else
    &(ss_ep_descriptors[2]),
#endif
};

#ifdef DIAG_COMPOSITE_PRELOADER
static struct endpoint_descriptor
    *ss_diag_ep_descriptor_ptrs[NUM_DIAG_ENDPOINTS] = {
    &(ss_ep_descriptors[3]),
    &(ss_ep_descriptors[4])
};
#endif

static struct endpoint_descriptor
    *hs_data_ep_descriptor_ptrs[NUM_DATA_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    &(hs_ep_descriptors[1]),
    &(hs_ep_descriptors[2]),
#else
    &(hs_ep_descriptors[0]),
    &(hs_ep_descriptors[1]),
#endif
};

static struct endpoint_descriptor
    *hs_comm_ep_descriptor_ptrs[NUM_COMM_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    &(hs_ep_descriptors[0]),
#else
    &(hs_ep_descriptors[2]),
#endif
};

#ifdef DIAG_COMPOSITE_PRELOADER
static struct endpoint_descriptor
    *hs_diag_ep_descriptor_ptrs[NUM_DIAG_ENDPOINTS] = {
    &(hs_ep_descriptors[3]),
    &(hs_ep_descriptors[4])
};
#endif

static struct endpoint_descriptor
    *fs_data_ep_descriptor_ptrs[NUM_DATA_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    &(fs_ep_descriptors[1]),
    &(fs_ep_descriptors[2]),
#else
    &(fs_ep_descriptors[0]),
    &(fs_ep_descriptors[1]),
#endif
};

static struct endpoint_descriptor
    *fs_comm_ep_descriptor_ptrs[NUM_COMM_ENDPOINTS] = {
#ifdef DIAG_COMPOSITE_PRELOADER
    &(fs_ep_descriptors[0]),
#else
    &(fs_ep_descriptors[2]),
#endif
};

#ifdef DIAG_COMPOSITE_PRELOADER
static struct endpoint_descriptor
    *fs_diag_ep_descriptor_ptrs[2] = {
    &(fs_ep_descriptors[3]),
    &(fs_ep_descriptors[4])
};
#endif

static void str2wide (char *str, u16 * wide)
{
    int i;

    for (i = 0; i < strlen (str) && str[i]; ++i)
        wide[i] = (u16) str[i];
}

int usbdl_configured (void);
static void buf_to_ep (circbuf_t * buf);
static int ep_to_buf (circbuf_t * buf);
void usbdl_poll (void);
void service_interrupts (void);

struct urb * usb_alloc_urb (struct mt_dev *p_device, struct mt_ep *p_ep)
{
    struct urb *p_urb = NULL;
    int ep_num = GET_EP_NUM(p_ep->endpoint_address);
    int dir = GET_EP_DIR(p_ep->endpoint_address);

    if (ep_num == 0){
        p_urb = &ep0_urb;
    }
    else{
        if (dir == USB_DIR_IN){
            p_urb = &tx_urb;
        }
        else{ // dir == USB_DIR_OUT
            p_urb = &rx_urb;
        }
    }

    memset(p_urb, 0, sizeof(*p_urb));
    p_urb->p_ep = p_ep;
    p_urb->p_dev = p_device;
    p_urb->p_buf = (u8 *) p_urb->buffer_data;
    p_urb->buffer_length = sizeof (p_urb->buffer_data);

    return p_urb;
}

struct device_descriptor* get_device_descriptor(int enableU3)
{
    if (enableU3 == 0){
        device_desc.bcdUSB = USB_BCD_VERSION;
        device_desc.bMaxPacketSize0 = EP0_MAX_PACKET_SIZE;
    }
    else{
        device_desc.bcdUSB = USB3_BCD_VERSION;
        device_desc.bMaxPacketSize0 = EP0_MAX_PACKET_SIZE_U3_EXP;
    }

    return &device_desc;
}

struct device_qualifier_descriptor* get_qualified_descriptor(int enableU3)
{
    if (enableU3 == 0){
        device_qualifier_desc.bcdUSB = USB_BCD_VERSION;
        device_qualifier_desc.bMaxPacketSize0 = EP0_MAX_PACKET_SIZE;
    }
    else{
        device_qualifier_desc.bcdUSB = USB3_BCD_VERSION;
        device_qualifier_desc.bMaxPacketSize0 = EP0_MAX_PACKET_SIZE_U3_EXP;
    }

    return &device_qualifier_desc;
}

struct configuration_descriptor* get_configuration_descriptor(int enableU3)
{
    int i = 0;

    if (enableU3 == 0){
        //2.0
        for(;i < NUM_CONFIGS;++i){
            config_descriptors[i].bMaxPower = USBD_MAXPOWER;
            config_descriptors[i].wTotalLength =
                (sizeof (struct configuration_descriptor) * NUM_CONFIGS) +
                (sizeof (struct interface_descriptor) * NUM_INTERFACES) +
                (sizeof (struct cdcacm_class_header_function_descriptor)) +
                (sizeof (struct cdcacm_class_abstract_control_descriptor)) +
                (sizeof (struct cdcacm_class_union_function_descriptor)) +
                (sizeof (struct cdcacm_class_call_management_descriptor)) +
                (sizeof (struct endpoint_descriptor) * NUM_ENDPOINTS);
        }
    }else{
        //3.0
        for(;i < NUM_CONFIGS;++i){
            config_descriptors[i].bMaxPower = USBD_SS_MAXPOWER;
            config_descriptors[i].wTotalLength =
                (sizeof (struct configuration_descriptor) * NUM_CONFIGS) +
                (sizeof (struct interface_descriptor) * NUM_INTERFACES) +
                (sizeof (struct cdcacm_class_header_function_descriptor)) +
                (sizeof (struct cdcacm_class_abstract_control_descriptor)) +
                (sizeof (struct cdcacm_class_union_function_descriptor)) +
                (sizeof (struct cdcacm_class_call_management_descriptor)) +
                (sizeof (struct endpoint_descriptor) * NUM_ENDPOINTS) +
                (sizeof (struct ss_ep_comp_descriptor) * NUM_ENDPOINTS);;
        }
    }

    return &config_descriptors;
}


u32 get_max_packet_size(int enableU3)
{
    if (enableU3 == 0){
        return EP0_MAX_PACKET_SIZE;
    }
    else{
        return EP0_MAX_PACKET_SIZE_U3;
    }
}

extern int g_enable_u3;

#define INIT_STR_DESC(array, desctype, str)\
    do{\
        struct string_descriptor *string = NULL;\
        string = (struct string_descriptor *) array;\
        string->bDescriptorType = desctype;\
        string->bLength = sizeof (array);\
        str2wide (str, string->wData);\
    }while(0)

void initialize_epx()
{
    int i;

    mt_usb_dev->max_endpoints = NUM_ENDPOINTS + 1;
    for (i = 0; i <= NUM_ENDPOINTS; ++i){
        struct mt_ep* p_ep = &mt_usb_ep[i];
        mt_setup_ep (mt_usb_dev, p_ep->endpoint_address & (~USB_DIR_IN), p_ep);
    }
}

void init_device()
{
    /* device instance initialization */
    memset (mt_usb_dev, 0, sizeof (mt_usb_dev));
    mt_usb_dev->p_dev_desc = get_device_descriptor(g_enable_u3);
    mt_usb_dev->p_dev_qualifier_desc = get_qualified_descriptor(g_enable_u3);
    mt_usb_dev->p_bos_desc = &bos_descriptor;
    mt_usb_dev->p_ext_cap_desc = &ext_cap_descriptor;
    mt_usb_dev->p_ss_cap_desc = &ss_cap_descriptor;
    mt_usb_dev->configurations = NUM_CONFIGS;
    mt_usb_dev->p_conf_array = mt_usb_config;
    mt_usb_dev->speed = SSUSB_SPEED_FULL;
    mt_usb_dev->p_ep_array = mt_usb_ep;
    mt_usb_dev->max_endpoints = 1;
    mt_usb_dev->maxpacketsize = 64;
}

void init_config()
{
    /* configuration instance initialization */
    memset (mt_usb_config, 0, sizeof (mt_usb_config));
    mt_usb_config->interfaces = NUM_INTERFACES;
    mt_usb_config->configuration_descriptor =
        get_configuration_descriptor(g_enable_u3);
    mt_usb_config->interface_array = mt_usb_interface;
}

void init_interfaces()
{
#ifdef DIAG_COMPOSITE_PRELOADER
    mt_usb_interface[0] = mt_usb_comm_interface;
    mt_usb_interface[1] = mt_usb_data_interface;
    mt_usb_interface[2] = mt_usb_diag_interface;
#else
    mt_usb_interface[0] = mt_usb_comm_interface;
    mt_usb_interface[1] = mt_usb_data_interface;
#endif

    /* data interface instance */
    memset (mt_usb_data_interface, 0, sizeof (mt_usb_data_interface));
    mt_usb_data_interface->alternates = 1;
    mt_usb_data_interface->altsetting_array = mt_usb_data_alt_if;

    /* data alternates instance */
    memset (mt_usb_data_alt_if, 0, sizeof (mt_usb_data_alt_if));
#ifdef DIAG_COMPOSITE_PRELOADER
    mt_usb_data_alt_if->p_interface_desc = &interface_descriptors[1];
#else
    mt_usb_data_alt_if->p_interface_desc = &interface_descriptors[0];
#endif
    mt_usb_data_alt_if->endpoints = NUM_DATA_ENDPOINTS;
    mt_usb_data_alt_if->pp_eps_desc_array = fs_data_ep_descriptor_ptrs;

    /* communication interface instance */
    memset (mt_usb_comm_interface, 0, sizeof (mt_usb_comm_interface));
    mt_usb_comm_interface->alternates = 1;
    mt_usb_comm_interface->altsetting_array = mt_usb_comm_alt_if;

    /* communication alternates instance */
    /* contains communication class specific interface descriptors */
    memset (mt_usb_comm_alt_if, 0, sizeof (mt_usb_comm_alt_if));
#ifdef DIAG_COMPOSITE_PRELOADER
    mt_usb_comm_alt_if->p_interface_desc = &interface_descriptors[0];
#else
    mt_usb_comm_alt_if->p_interface_desc = &interface_descriptors[1];
#endif
    mt_usb_comm_alt_if->p_header_function_desc = &header_function_descriptor;
    mt_usb_comm_alt_if->p_abstract_control_desc = &abstract_control_descriptor;
    mt_usb_comm_alt_if->p_union_function_desc = &union_function_descriptor;
    mt_usb_comm_alt_if->p_call_management_desc = &call_management_descriptor;
    mt_usb_comm_alt_if->endpoints = NUM_COMM_ENDPOINTS;
    mt_usb_comm_alt_if->pp_eps_desc_array = fs_comm_ep_descriptor_ptrs;

#ifdef DIAG_COMPOSITE_PRELOADER
    /* DIAG serial interface instance */
    memset (mt_usb_diag_interface, 0, sizeof (mt_usb_diag_interface));
    mt_usb_diag_interface->alternates = 1;
    mt_usb_diag_interface->altsetting_array = mt_usb_diag_alt_if;

    /* DIAG serial alternates instance */
    memset (mt_usb_diag_alt_if, 0, sizeof (mt_usb_diag_alt_if));
    mt_usb_diag_alt_if->p_interface_desc = &interface_descriptors[2];
    mt_usb_diag_alt_if->endpoints = NUM_DIAG_ENDPOINTS;
    mt_usb_diag_alt_if->pp_eps_desc_array = fs_diag_ep_descriptor_ptrs;
#endif
}

void init_endpoints()
{
    /* endpoint instances */
    memset (&mt_usb_ep[0], 0, sizeof (mt_usb_ep[0]));
    mt_usb_ep[0].endpoint_address = 0;
    mt_usb_ep[0].rx_pktsz = get_max_packet_size(g_enable_u3);
    mt_usb_ep[0].tx_pktsz = get_max_packet_size(g_enable_u3);
    mt_setup_ep (mt_usb_dev, 0, &mt_usb_ep[0]);

    {
        int i = 1;
        for (; i <= NUM_ENDPOINTS; ++i){
            struct mt_ep* p_ep = &mt_usb_ep[i];
            struct endpoint_descriptor* p_epdesc = &fs_ep_descriptors[i - 1];

            memset (p_ep, 0, sizeof (*p_ep));
            //p_ep->burst = 0;
            p_ep->slot = 1;
            //p_ep->mult = 0;
            p_ep->endpoint_address = p_epdesc->bEndpointAddress;
            p_ep->rx_pktsz = p_epdesc->wMaxPacketSize;
            p_ep->tx_pktsz = p_epdesc->wMaxPacketSize;
            p_ep->binterval = p_epdesc->bInterval;
            p_ep->type = p_epdesc->bmAttributes;
            if (p_ep->endpoint_address & USB_DIR_IN)
                p_ep->p_tx_urb = usb_alloc_urb (mt_usb_dev, p_ep);
            else
                p_ep->p_rcv_urb = usb_alloc_urb (mt_usb_dev, p_ep);
        }
    }
}

void init_strings()
{
    /* initialize string descriptor array */
    INIT_STR_DESC(manufacturer, USB_DESCRIPTOR_TYPE_STRING, USBD_MANUFACTURER);
    INIT_STR_DESC(product, USB_DESCRIPTOR_TYPE_STRING, USBD_PRODUCT_NAME);
    INIT_STR_DESC(configuration, USB_DESCRIPTOR_TYPE_STRING, USBD_CONFIGURATION_STR);
    INIT_STR_DESC(dataInterface, USB_DESCRIPTOR_TYPE_STRING, USBD_DATA_INTERFACE_STR);
    INIT_STR_DESC(commInterface, USB_DESCRIPTOR_TYPE_STRING, USBD_COMM_INTERFACE_STR);
#ifdef DIAG_COMPOSITE_PRELOADER
    INIT_STR_DESC(serialInterface, USB_DESCRIPTOR_TYPE_STRING, USBD_DIAG_INTERFACE_STR);
#endif

    /* Now, initialize the string table for ep0 handling */
    usb_string_table = usbtty_string_table;
}

/*
 * Initialize the usb client port.
 *
 */
int usbdl_init (void)
{
    /* initialize usb variables */
    SET_USB_UNCONFIGUTED;
    SET_TOOL_NOT_EXIST;

    /* prepare buffers... */
    buf_input_init (&usb_input_buffer, USBTTY_BUFFER_SIZE);
    buf_output_init (&usb_output_buffer, USBTTY_BUFFER_SIZE);

    init_strings();
    init_device();
    init_config();
    init_interfaces();
    init_endpoints();

    udc_enable(mt_usb_dev);
    initialize_epx();
    return 0;
}

/*********************************************************************************/

static void buf_to_ep (circbuf_t * buf)
{
    int i;

    if (!usbdl_configured ()){
        TTY_LOG("%s usbdl not configured\n", __func__);
        return;
    }

    if (buf->size){
        struct mt_ep *endpoint = &mt_usb_ep[TX_ENDPOINT];
        struct urb *p_urb = endpoint->p_tx_urb;
        int space_avail = 0;
        int popnum = 0;

        /* Break buffer into urb sized pieces, and link each to the endpoint */
        while (buf->size > 0){
            if (!p_urb){
                TTY_LOG ("%s, no available URB", __func__);
                return;
            }
            space_avail = p_urb->buffer_length - p_urb->actual_length;
            if (space_avail <= 0){
                TTY_LOG ("%s, no available spaces", __func__);
                return;
            }

            popnum = buf_pop (buf, p_urb->p_buf +
                p_urb->actual_length, MIN (space_avail, buf->size));

            /* update the used space of current_urb */
            p_urb->actual_length += popnum;

            /* nothing is in the buffer or the urb can hold no more data */
            if (popnum == 0)
                break;

            /* if the endpoint is idle, trigger the tx transfer */
            if (endpoint->last == 0){
                mt_ep_write (endpoint);
            }
        }/* end while */
    }/* end if buf->size */
}

static int ep_to_buf (circbuf_t * buf)
{
    struct mt_ep *p_ep = NULL;
    int length = 0;
    struct urb *p_urb = NULL;

    if (!usbdl_configured()){
        TTY_LOG("%s ends, usb is not configured", __func__);
        return 0;
    }

    p_ep = &mt_usb_ep[RX_ENDPOINT];
    p_urb = p_ep->p_rcv_urb;

    if (!p_urb){
        TTY_LOG ("%s, no available URB", __func__);
        return 0;
    }
    length = p_urb->actual_length;

    if (p_urb && length){
        buf_push (buf, (char *) p_urb->p_buf, p_urb->actual_length);
        p_urb->actual_length = 0;
    }

    return length;
}

int does_tool_exist(void)
{
    return g_tool_exists;
}

void set_tool_exist(void)
{
    SET_TOOL_EXIST;
}

int usbdl_configured (void)
{
    return g_usb_configured;
}

void enable_highspeed (void)
{
    int i;

    g_enable_u3 = 0;

    mt_usb_dev->speed = SSUSB_SPEED_HIGH;
    mt_usb_dev->p_dev_desc = get_device_descriptor(g_enable_u3);
    mt_usb_dev->p_dev_qualifier_desc = get_qualified_descriptor(g_enable_u3);
    mt_usb_data_alt_if->pp_eps_desc_array = hs_data_ep_descriptor_ptrs;
    mt_usb_comm_alt_if->pp_eps_desc_array = hs_comm_ep_descriptor_ptrs;
    mt_usb_config->configuration_descriptor
        = get_configuration_descriptor(g_enable_u3);
#ifdef DIAG_COMPOSITE_PRELOADER
    mt_usb_diag_alt_if->pp_eps_desc_array = hs_diag_ep_descriptor_ptrs;
#endif

    for (i = 1; i <= NUM_ENDPOINTS; i++){
        struct mt_ep* p_ep = &mt_usb_ep[i];
        struct endpoint_descriptor* p_epdesc = &hs_ep_descriptors[i - 1];

        p_ep->endpoint_address = p_epdesc->bEndpointAddress;
        p_ep->rx_pktsz = p_epdesc->wMaxPacketSize;
        p_ep->tx_pktsz = p_epdesc->wMaxPacketSize;
    }
}

void enable_superspeed (void)
{
    int i;

    g_enable_u3 = 1;

    mt_usb_dev->speed = SSUSB_SPEED_SUPER;
    mt_usb_dev->p_dev_desc = get_device_descriptor(g_enable_u3);
    mt_usb_dev->p_dev_qualifier_desc = get_qualified_descriptor(g_enable_u3);
    mt_usb_data_alt_if->pp_eps_desc_array = ss_data_ep_descriptor_ptrs;
    mt_usb_data_alt_if->p_ss_ep_comp_desc = &ss_ep_comp_descriptor;
    mt_usb_comm_alt_if->pp_eps_desc_array = ss_comm_ep_descriptor_ptrs;
    mt_usb_comm_alt_if->p_ss_ep_comp_desc = &ss_ep_comp_descriptor;
    mt_usb_config->configuration_descriptor
        = get_configuration_descriptor(g_enable_u3);

#ifdef DIAG_COMPOSITE_PRELOADER
    mt_usb_diag_alt_if->pp_eps_desc_array = ss_diag_ep_descriptor_ptrs;
    mt_usb_diag_alt_if->p_ss_ep_comp_desc = &ss_ep_comp_descriptor;
#endif

    for (i = 1; i <= NUM_ENDPOINTS; i++){
        struct mt_ep* p_ep = &mt_usb_ep[i];
        struct endpoint_descriptor* p_epdesc = &ss_ep_descriptors[i - 1];

        p_ep->endpoint_address = p_epdesc->bEndpointAddress;
        p_ep->rx_pktsz = p_epdesc->wMaxPacketSize;
        p_ep->tx_pktsz = p_epdesc->wMaxPacketSize;
    }
}

//#define usbtty_event_log print
#define usbtty_event_log

/*********************************************************************************/

void config_usbtty (struct mt_dev *device)
{
    SET_USB_CONFIGUTED;
    initialize_epx();
}

/*********************************************************************************/

extern u32 isUSBDebug;

/* Used to emulate interrupt handling */
void usbdl_poll (void)
{
    if (isUSBDebug != 0)
        printf("[U]%s is called\n",__func__);

    /* New interrupts? */
    service_interrupts ();

    /* Write any output data to host buffer (do this before checking interrupts to avoid missing one) */
    buf_to_ep (&usb_output_buffer);

    /* Check for new data from host.. (do this after checking interrupts to get latest data) */
    ep_to_buf (&usb_input_buffer);

    if (isUSBDebug != 0)
        printf("[U]%s ends\n",__func__);

}

extern ulong get_timer(ulong base);

void usbdl_flush(void)
{
    u32 start_time = get_timer(0);
    u32 timeout = 0;

    if (isUSBDebug != 0)
        printf("%s check usb_output_buffer.size %d, ep busy %d\n",
            __func__,
            usb_output_buffer.size,
            is_tx_ep_busy(&mt_usb_ep[TX_ENDPOINT]));

    while (((usb_output_buffer.size) > 0)
        || is_tx_ep_not_empty(&mt_usb_ep[TX_ENDPOINT])
		|| mt_usb_ep[TX_ENDPOINT].last != 0 )
    {
        usbdl_poll ();

        if(get_timer(start_time) > 300){
            u32 intrep = __raw_readl(U3D_EPISR) & __raw_readl(U3D_EPIER);
            u32 intrtx = intrep & 0xFFFF;
	        u32 intrrx = intrep >> 16;

            printf("usbdl_flush timeout");
            printf("intrep :%x, IntrTx[%x] IntrRx [%x]",
                intrep, intrtx, intrrx);

            timeout = 1;
            break;
        }
    }

    if (timeout == 1){
    	start_time = get_timer(0);
    	printf("Start check delay time");

        while (1)
        {
            u32 size = usb_output_buffer.size;
            int isBusy =  is_tx_ep_busy(&mt_usb_ep[TX_ENDPOINT]);
            u32 intrep = __raw_readl(U3D_EPISR) & __raw_readl(U3D_EPIER);
            u32 intrtx = intrep & 0xFFFF;
	        u32 intrrx = intrep >> 16;

            if (!isBusy){
                printf("ep is free after %d ms, buffer size %d",
                    get_timer(start_time), size);
                printf("intrep :%x, IntrTx[%x] IntrRx [%x]", intrep, intrtx, intrrx);
                break;
            }

            if(get_timer(start_time) > 3000){
    	        printf("Abort check after %d ms", get_timer(start_time));
                printf("intrep :%x, IntrTx[%x] IntrRx [%x]", intrep, intrtx, intrrx);
                printf("buffer size %d, is_tx_ep_busy %d", size, isBusy);
                break;
            }
        }
    }

    if (isUSBDebug != 0)
        printf("%s ends, usb_output_buffer.size %d, ep busy %d\n",
            __func__,
            usb_output_buffer.size,
            is_tx_ep_busy(&mt_usb_ep[TX_ENDPOINT]));
}

void service_interrupts(void)
{
	u32 ltssm = 0;
	u32 intrusb = 0;
	u32 dmaintr = 0;
	u16 intrtx = 0;
	u16 intrrx = 0;
	u32 intrqmu = 0;
	u32 intrqmudone = 0;
	u32 linkint = 0;
	u32 intrep = 0;
	u32 lv1_isr = 0;

    if (isUSBDebug != 0)
        printf("[U]%s is called\n",__func__);

	/* give ltssm and intrusb initial value */
	ltssm = 0;
	intrusb = 0;

	lv1_isr = __raw_readl(U3D_LV1ISR);

	if(lv1_isr & MAC2_INTR){
		intrusb = __raw_readl(U3D_COMMON_USB_INTR) & __raw_readl(U3D_COMMON_USB_INTR_ENABLE);
	}

#ifdef SUPPORT_U3
	if (lv1_isr & MAC3_INTR) {
		ltssm = __raw_readl(U3D_LTSSM_INTR) & __raw_readl(U3D_LTSSM_INTR_ENABLE);
	}
#endif

	#if 0
	dmaintr = __raw_readl(U3D_DMAISR)
		& __raw_readl(U3D_DMAIER);
	#endif

	intrep = __raw_readl(U3D_EPISR) & __raw_readl(U3D_EPIER);

#ifdef SUPPORT_QMU
	intrqmu = __raw_readl(U3D_QISAR1);

	intrqmudone = __raw_readl(U3D_QISAR0) & __raw_readl(U3D_QIER0);
#endif

	intrtx = intrep & 0xFFFF;
	intrrx = intrep >> 16;

	linkint = __raw_readl(U3D_DEV_LINK_INTR) & __raw_readl(U3D_DEV_LINK_INTR_ENABLE);

	TTY_LOG("wIntrQMUDoneValue :%x",intrqmudone);
	TTY_LOG("intrep :%x",intrep);
	TTY_LOG("Interrupt: IntrUsb [%x] IntrTx[%x] IntrRx [%x] IntrDMA[%x] IntrQMU [%x] IntrLTSSM [%x]"
		, intrusb, intrtx, intrrx, dmaintr, intrqmu, ltssm);

#ifdef SUPPORT_QMU
	usb_writel(intrqmudone, U3D_QISAR0);
#endif

	if (lv1_isr & MAC2_INTR) {
   		__raw_writel(intrusb, U3D_COMMON_USB_INTR);
	}
#ifdef SUPPORT_U3
	if (lv1_isr & MAC3_INTR) {
		__raw_writel(ltssm, U3D_LTSSM_INTR);
	}
#endif
   	__raw_writel(intrep, U3D_EPISR);
 	//__raw_writel(dmaintr, U3D_DMAISR);
 	__raw_writel(linkint, U3D_DEV_LINK_INTR);


	if (ltssm | intrusb | dmaintr | intrtx
		| intrrx | intrqmu | intrqmudone | linkint) {
		mt_udc_irq(ltssm, intrusb, dmaintr, intrtx,
			intrrx, intrqmu, intrqmudone, linkint);
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
int mt_usbtty_tstc (void)
{
    usbdl_poll();
    return (usb_input_buffer.size > 0);
}


/* get a single character and copy it to usb_input_buffer */
int mt_usbtty_getc (void)
{
    char c;

    while (usb_input_buffer.size <= 0){
        usbdl_poll ();
    }

    buf_pop (&usb_input_buffer, &c, sizeof(c));
    return c;
}

/* get n characters and copy it to usb_input_buffer */
int mt_usbtty_getcn (int count, char *buf)
{
    int data_count = 0;
    int tmp = 0;

    /* wait until received 'count' bytes of data */
    while (data_count < count){
        if (usb_input_buffer.size < 512)
            usbdl_poll ();

        if (usb_input_buffer.size > 0){
            tmp = usb_input_buffer.size;
            if (data_count + tmp > count){
                  tmp = count - data_count;
            }
            buf_pop(&usb_input_buffer, buf + data_count, tmp);
            data_count += tmp;
        }
    }

    return 0;
}

void mt_usbtty_putc (const char c, int flush)
{
    buf_push (&usb_output_buffer, &c, sizeof(c));

    /* Poll at end to handle new data... */
    if (((usb_output_buffer.size) >= usb_output_buffer.totalsize) || flush){
        usbdl_poll ();
    }
}


void mt_usbtty_putcn (int count, char *buf, int flush)
{
    #define LENGTH      512
    char *cp = buf;

    if (isUSBDebug != 0)
        printf("[U]%s is sending %d bytes\n", __func__, count);

    while (count > 0){
        if (count > LENGTH){
            buf_push (&usb_output_buffer, cp, LENGTH);
            cp += LENGTH;
            count -= LENGTH;
        }
        else{
            buf_push (&usb_output_buffer, cp, count);
            cp += count;
            count = 0;
        }

        if (isUSBDebug != 0)
            printf("[U]%s check usb_output_buffer.size %d, usb_output_buffer.totalsize %d, flush %d\n",
                __func__,
                usb_output_buffer.size,
                usb_output_buffer.totalsize,
                flush);

        if (((usb_output_buffer.size) >= usb_output_buffer.totalsize)
          || flush)
        {
            usbdl_poll ();
        }
    }

    if (isUSBDebug != 0)
        printf("[U]%s ends\n", __func__);

}

void mt_usbtty_puts (const char *str)
{
    int len = strlen (str);
    int maxlen = usb_output_buffer.totalsize;
    int space = 0, n = 0;

    /* break str into chunks < buffer size, if needed */
    while (len > 0){
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

int mt_usbtty_query_data_size (void)
{
    if (usb_input_buffer.size < 512){
        if (usbdl_configured ()){
            service_interrupts ();
            ep_to_buf (&usb_input_buffer);
        }
    }

    return usb_input_buffer.size;
}

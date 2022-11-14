#ifndef _USB_CONFIG_H
#define _USB_CONFIG_H

#ifdef R8_USB_RX_LEN
#undef R8_USB_RX_LEN
#endif
#ifdef R16_PIN_ANALOG_IE
#undef R16_PIN_ANALOG_IE
#endif

/* USB common Configuration */
#ifndef CONFIG_USB_DBG_LEVEL
#define CONFIG_USB_DBG_LEVEL USB_DBG_ERROR
#endif

#ifndef CONFIG_USB_PRINTF
#ifdef PLF_DEBUG
#define CONFIG_USB_PRINTF printf
#else
#define CONFIG_USB_PRINTF(...)
#endif
#endif

// #define CONFIG_USB_PRINTF_COLOR_ENABLE

#define USB_NOCACHE_RAM_SECTION __attribute__((section(".noncacheable")))
#define USB_NUM_BIDIR_ENDPOINTS 8

/* USB DEVICE Configuration */
/* core */
#ifndef CONFIG_USBDEV_REQUEST_BUFFER_LEN
#define CONFIG_USBDEV_REQUEST_BUFFER_LEN 256
#endif

#ifndef CONFIG_USBDEV_DESC_CHECK
#define CONFIG_USBDEV_DESC_CHECK 0
#endif

#ifndef CONFIG_USB_ALIGN_SIZE
#define CONFIG_USB_ALIGN_SIZE 4
#endif

/* msc class */
#ifndef CONFIG_USBDEV_MSC_BLOCK_SIZE
#define CONFIG_USBDEV_MSC_BLOCK_SIZE 512
#endif

#ifndef CONFIG_USBDEV_MSC_MANUFACTURER_STRING
#define CONFIG_USBDEV_MSC_MANUFACTURER_STRING ""
#endif

#ifndef CONFIG_USBDEV_MSC_PRODUCT_STRING
#define CONFIG_USBDEV_MSC_PRODUCT_STRING ""
#endif

#ifndef CONFIG_USBDEV_MSC_VERSION_STRING
#define CONFIG_USBDEV_MSC_VERSION_STRING "0.01"
#endif

/* audio class */
#ifndef CONFIG_USBDEV_AUDIO_VERSION
#define CONFIG_USBDEV_AUDIO_VERSION 0x0100
#endif

#ifndef CONFIG_USBDEV_AUDIO_MAX_CHANNEL
#define CONFIG_USBDEV_AUDIO_MAX_CHANNEL 2
#endif

/* USB HOST Configuration */
#ifndef CONFIG_USBHOST_RHPORTS
#define CONFIG_USBHOST_RHPORTS 1
#endif

#ifndef CONFIG_USBHOST_EHPORTS
#define CONFIG_USBHOST_EHPORTS 4
#endif

#ifndef CONFIG_USBHOST_PIPE_NUM
#define CONFIG_USBHOST_PIPE_NUM 10
#endif

#ifndef CONFIG_USBHOST_INTF_NUM
#define CONFIG_USBHOST_INTF_NUM 6
#endif

#ifndef CONFIG_USBHOST_EP_NUM
#define CONFIG_USBHOST_EP_NUM 4
#endif

#ifndef CONFIG_USBHOST_CONTROL_TRANSFER_TIMEOUT
#define CONFIG_USBHOST_CONTROL_TRANSFER_TIMEOUT 5000
#endif

#ifndef CONFIG_USBHOST_MSC_TIMEOUT
#define CONFIG_USBHOST_MSC_TIMEOUT 5000
#endif

#ifndef CONFIG_USBHOST_HPWORKQ_PRIO
#define CONFIG_USBHOST_HPWORKQ_PRIO 5
#endif
#ifndef CONFIG_USBHOST_HPWORKQ_STACKSIZE
#define CONFIG_USBHOST_HPWORKQ_STACKSIZE 2048
#endif

#ifndef CONFIG_USBHOST_LPWORKQ_PRIO
#define CONFIG_USBHOST_LPWORKQ_PRIO 1
#endif
#ifndef CONFIG_USBHOST_LPWORKQ_STACKSIZE
#define CONFIG_USBHOST_LPWORKQ_STACKSIZE 2048
#endif

#ifndef CONFIG_USBHOST_PSC_PRIO
#define CONFIG_USBHOST_PSC_PRIO 4
#endif
#ifndef CONFIG_USBHOST_PSC_STACKSIZE
#define CONFIG_USBHOST_PSC_STACKSIZE 4096
#endif

#ifndef CONFIG_USBHOST_DEV_NAMELEN
#define CONFIG_USBHOST_DEV_NAMELEN 16
#endif

#define CONFIG_USBHOST_ASYNCH
//#define CONFIG_USBHOST_GET_STRING_DESC

/* EHCI Configuration */
#define CONFIG_USB_EHCI_HCCR_BASE (0x20072000)
#define CONFIG_USB_EHCI_HCOR_BASE (0x20072000 + 0x10)
#define CONFIG_USB_EHCI_QH_NUM    (10)
#define CONFIG_USB_EHCI_QTD_NUM   (10)
// #define CONFIG_USB_EHCI_INFO_ENABLE
#define CONFIG_USB_ECHI_HCOR_RESERVED_DISABLE
// #define CONFIG_USB_EHCI_CONFIGFLAG

#if defined pEP0_DataBuf
#undef pEP0_DataBuf
#endif
#if defined pEP1_OUT_DataBuf
#undef pEP1_OUT_DataBuf
#endif
#if defined pEP1_IN_DataBuf
#undef pEP1_IN_DataBuf
#endif
#if defined pEP2_OUT_DataBuf
#undef pEP2_OUT_DataBuf
#endif
#if defined pEP2_IN_DataBuf
#undef pEP2_IN_DataBuf
#endif
#if defined pEP3_OUT_DataBuf
#undef pEP3_OUT_DataBuf
#endif
#if defined pEP3_IN_DataBuf
#undef pEP3_IN_DataBuf
#endif
#if defined pEP4_OUT_DataBuf
#undef pEP4_OUT_DataBuf
#endif
#if defined pEP4_IN_DataBuf
#undef pEP4_IN_DataBuf
#endif
#if defined pEP5_OUT_DataBuf
#undef pEP5_OUT_DataBuf
#endif
#if defined pEP5_IN_DataBuf
#undef pEP5_IN_DataBuf
#endif
#if defined pEP6_OUT_DataBuf
#undef pEP6_OUT_DataBuf
#endif
#if defined pEP6_IN_DataBuf
#undef pEP6_IN_DataBuf
#endif
#if defined pEP7_OUT_DataBuf
#undef pEP7_OUT_DataBuf
#endif
#if defined pEP7_IN_DataBuf
#undef pEP7_IN_DataBuf
#endif

#if defined USB_PID_OUT
#undef USB_PID_OUT
#endif
#if defined USB_PID_IN
#undef USB_PID_IN
#endif
#if defined USB_PID_SOF
#undef USB_PID_SOF
#endif
#if defined USB_PID_SETUP
#undef USB_PID_SETUP
#endif
#if defined USB_PID_DATA0
#undef USB_PID_DATA0
#endif
#if defined USB_PID_DATA1
#undef USB_PID_DATA1
#endif
#if defined USB_PID_ACK
#undef USB_PID_ACK
#endif
#if defined USB_PID_NAK
#undef USB_PID_NAK
#endif
#if defined USB_PID_STALL
#undef USB_PID_STALL
#endif
#if defined USB_PID_PRE
#undef USB_PID_PRE
#endif

#endif

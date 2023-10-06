/*
 * Copyright (c) 2022, sakumisu
 *               2023, Huckies
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef CHERRYUSB_CONFIG_H
#define CHERRYUSB_CONFIG_H

#define CHERRYUSB_VERSION 0x000900

#ifdef R8_USB_RX_LEN
#undef R8_USB_RX_LEN
#endif
#ifdef R16_PIN_ANALOG_IE
#undef R16_PIN_ANALOG_IE
#endif

/* ================ USB common Configuration ================ */

#define usb_malloc(size) malloc(size)
#define usb_free(ptr)    free(ptr)

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

/* Enable print with color */
// #define CONFIG_USB_PRINTF_COLOR_ENABLE

/* data align size when use dma */
#ifndef CONFIG_USB_ALIGN_SIZE
#define CONFIG_USB_ALIGN_SIZE 4
#endif

/* attribute data into no cache ram */
#define USB_NOCACHE_RAM_SECTION __attribute__((section(".noncacheable")))
#define USB_NUM_BIDIR_ENDPOINTS ENDPOINT_TOTAL_ENDPOINTS
#define EP_NUMS                 USB_NUM_BIDIR_ENDPOINTS

/* Ep0 max transfer buffer, specially for receiving data from ep0 out */
#ifdef RGB_RAW_ENABLE
#define CONFIG_USBDEV_REQUEST_BUFFER_LEN 512
#else
#define CONFIG_USBDEV_REQUEST_BUFFER_LEN 256
#endif

/* Setup packet log for debug */
// #define CONFIG_USBDEV_SETUP_LOG_PRINT

/* Check if the input descriptor is correct */
// #define CONFIG_USBDEV_DESC_CHECK

/* Enable test mode */
// #define CONFIG_USBDEV_TEST_MODE

// #define CONFIG_USBDEV_TX_THREAD
// #define CONFIG_USBDEV_RX_THREAD

#ifdef CONFIG_USBDEV_TX_THREAD
#ifndef CONFIG_USBDEV_TX_PRIO
#define CONFIG_USBDEV_TX_PRIO 4
#endif
#ifndef CONFIG_USBDEV_TX_STACKSIZE
#define CONFIG_USBDEV_TX_STACKSIZE 2048
#endif
#endif

#ifdef CONFIG_USBDEV_RX_THREAD
#ifndef CONFIG_USBDEV_RX_PRIO
#define CONFIG_USBDEV_RX_PRIO 4
#endif
#ifndef CONFIG_USBDEV_RX_STACKSIZE
#define CONFIG_USBDEV_RX_STACKSIZE 2048
#endif
#endif

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

#ifndef CONFIG_USBDEV_AUDIO_VERSION
#define CONFIG_USBDEV_AUDIO_VERSION 0x0100
#endif

#ifndef CONFIG_USBDEV_AUDIO_MAX_CHANNEL
#define CONFIG_USBDEV_AUDIO_MAX_CHANNEL 2
#endif

#ifndef CONFIG_USBDEV_RNDIS_RESP_BUFFER_SIZE
#define CONFIG_USBDEV_RNDIS_RESP_BUFFER_SIZE 128
#endif

#ifndef CONFIG_USBDEV_RNDIS_ETH_MAX_FRAME_SIZE
#define CONFIG_USBDEV_RNDIS_ETH_MAX_FRAME_SIZE 1536
#endif

#ifndef CONFIG_USBDEV_RNDIS_VENDOR_ID
#define CONFIG_USBDEV_RNDIS_VENDOR_ID 0x0000ffff
#endif

#ifndef CONFIG_USBDEV_RNDIS_VENDOR_DESC
#define CONFIG_USBDEV_RNDIS_VENDOR_DESC "CherryUSB"
#endif

// #define CONFIG_USBDEV_RNDIS_USING_LWIP

/* ================ USB HOST Stack Configuration ================== */

#define CONFIG_USBHOST_MAX_RHPORTS          1
#define CONFIG_USBHOST_MAX_EXTHUBS          1
#define CONFIG_USBHOST_MAX_EHPORTS          4
#define CONFIG_USBHOST_MAX_INTERFACES       6
#define CONFIG_USBHOST_MAX_INTF_ALTSETTINGS 1
#define CONFIG_USBHOST_MAX_ENDPOINTS        4

#define CONFIG_USBHOST_MAX_CDC_ACM_CLASS 4
#define CONFIG_USBHOST_MAX_HID_CLASS     4
#define CONFIG_USBHOST_MAX_MSC_CLASS     2
#define CONFIG_USBHOST_MAX_AUDIO_CLASS   1
#define CONFIG_USBHOST_MAX_VIDEO_CLASS   1
#define CONFIG_USBHOST_MAX_RNDIS_CLASS   1

#define CONFIG_USBHOST_DEV_NAMELEN 16

#ifndef CONFIG_USBHOST_PSC_PRIO
#define CONFIG_USBHOST_PSC_PRIO 4
#endif
#ifndef CONFIG_USBHOST_PSC_STACKSIZE
#define CONFIG_USBHOST_PSC_STACKSIZE 2048
#endif

// #define CONFIG_USBHOST_GET_STRING_DESC

/* Ep0 max transfer buffer */
#define CONFIG_USBHOST_REQUEST_BUFFER_LEN 512

#ifndef CONFIG_USBHOST_CONTROL_TRANSFER_TIMEOUT
#define CONFIG_USBHOST_CONTROL_TRANSFER_TIMEOUT 500
#endif

#ifndef CONFIG_USBHOST_MSC_TIMEOUT
#define CONFIG_USBHOST_MSC_TIMEOUT 5000
#endif

/* ================ USB Device Port Configuration ================*/

// #define USBD_IRQHandler USBD_IRQHandler
// #define USB_BASE (0x40080000UL)
// #define USB_NUM_BIDIR_ENDPOINTS 4

/* ================ USB Host Port Configuration ==================*/

#define CONFIG_USBHOST_PIPE_NUM 10

/* ================ EHCI Configuration ================ */

#define CONFIG_USB_EHCI_HCCR_BASE       (0x20072000)
#define CONFIG_USB_EHCI_HCOR_BASE       (0x20072000 + 0x10)
#define CONFIG_USB_EHCI_FRAME_LIST_SIZE 1024
// #define CONFIG_USB_EHCI_INFO_ENABLE
// #define CONFIG_USB_ECHI_HCOR_RESERVED_DISABLE
// #define CONFIG_USB_EHCI_CONFIGFLAG
// #define CONFIG_USB_EHCI_PORT_POWER

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

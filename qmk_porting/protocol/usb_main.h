#pragma once

#include <stdint.h>

#ifdef NKRO_ENABLE
#include "protocol/usb_descriptor.h"
#else
#define KEYBOARD_EPSIZE 8
#define RAW_EPSIZE      32
#endif

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< keyboard in endpoint */
#define KBD_IN_EP          0x81
#define KBD_IN_EP_SIZE     KEYBOARD_EPSIZE
#define KBD_IN_EP_INTERVAL 1

/*!< keyboard out endpoint */
#define KBD_OUT_EP          0x02
#define KBD_OUT_EP_SIZE     1
#define KBD_OUT_EP_INTERVAL 1

/*!< Extrkey in endpoint */
#define EXKEY_IN_EP 0x83
#ifdef NKRO_ENABLE
#define EXKEY_IN_EP_SIZE SHARED_EPSIZE
#else
#define EXKEY_IN_EP_SIZE 8
#endif
#define EXKEY_IN_EP_INTERVAL 1

/*!< hidraw in endpoint */
#define HIDRAW_IN_EP       0x84
#define HIDRAW_IN_SIZE     RAW_EPSIZE
#define HIDRAW_IN_INTERVAL 1

/*!< hidraw out endpoint */
#define HIDRAW_OUT_EP          0x04
#define HIDRAW_OUT_EP_SIZE     HIDRAW_IN_SIZE
#define HIDRAW_OUT_EP_INTERVAL 1

#define USBD_VID           VENDOR_ID
#define USBD_PID           PRODUCT_ID
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 0x0409

void init_usb_driver();
void hid_bios_keyboard_send_report(uint8_t *data, uint8_t len);
void hid_nkro_keyboard_send_report(uint8_t *data, uint8_t len);
void raw_hid_send(uint8_t *data, uint8_t length);
void hid_exkey_send_report(uint8_t *data, uint8_t len);
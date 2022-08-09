#pragma once

#include <stdint.h>

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< keyboard in endpoint */
#define KBD_IN_EP          0x81 /*!< address */
#define KBD_IN_EP_SIZE     8    /*!< max packet length */
#define KBD_IN_EP_INTERVAL 1    /*!< polling time */

/*!< keyboard out endpoint */
#define KBD_OUT_EP          0x01 /*!< address */
#define KBD_OUT_EP_SIZE     1    /*!< max packet length */
#define KBD_OUT_EP_INTERVAL 1    /*!< polling time */

/*!< hidraw in endpoint */
#define HIDRAW_IN_EP       0x82
#define HIDRAW_IN_SIZE     32
#define HIDRAW_IN_INTERVAL 1

/*!< hidraw out endpoint */
#define HIDRAW_OUT_EP          0x03
#define HIDRAW_OUT_EP_SIZE     HIDRAW_IN_SIZE
#define HIDRAW_OUT_EP_INTERVAL 1

/*!< Extrkey out endpoint */
#define EXKEY_IN_EP          0x84 /*!< address */
#define EXKEY_IN_EP_SIZE     5    /*!< max packet length */
#define EXKEY_IN_EP_INTERVAL 1    /*!< polling time */

#define USBD_VID           VENDOR_ID
#define USBD_PID           PRODUCT_ID
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 0x0409

void init_usb_driver();
void hid_keyboard_send_report(uint8_t ep, uint8_t *data, uint8_t len);
void raw_hid_send(uint8_t *data, uint8_t length);
void hid_exkey_send_report(uint8_t ep, uint8_t *data, uint8_t len);
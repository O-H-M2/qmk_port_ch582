/*
Copyright 2022 Huckies <https://github.com/Huckies>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <stdint.h>

#ifdef NKRO_ENABLE
#include "protocol/usb_descriptor.h"
#ifdef USB_CONFIG_POWER_MA
#undef USB_CONFIG_POWER_MA
#endif
#else
#define KEYBOARD_EPSIZE 8
#define RAW_EPSIZE      32
#endif
#define RGBRAW_EP_SIZE 64

enum {
    KEYBOARD_MODE_BIOS = 0,
    KEYBOARD_MODE_NKRO,
};

enum {
    IN_ENDPOINT_START = 0x81,
    KBD_IN_EP = IN_ENDPOINT_START,
#ifdef RGB_RAW_ENABLE
    RGBRAW_IN_EP,
#endif
    EXKEY_IN_EP,
#ifdef RAW_ENABLE
    QMKRAW_IN_EP,
#endif

    KBD_OUT_EP = KBD_IN_EP - 0x80,
#ifdef RGB_RAW_ENABLE
    RGBRAW_OUT_EP = RGBRAW_IN_EP - 0x80,
#endif
#ifdef RAW_ENABLE
    QMKRAW_OUT_EP = QMKRAW_IN_EP - 0x80,
#endif
};

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

#define KBD_IN_EP_SIZE     KEYBOARD_EPSIZE
#define KBD_IN_EP_INTERVAL 1

#define KBD_OUT_EP_SIZE     1
#define KBD_OUT_EP_INTERVAL 1

#define RGBRAW_IN_SIZE     RGBRAW_EP_SIZE
#define RGBRAW_IN_INTERVAL 1

#define RGBRAW_OUT_EP_SIZE     RGBRAW_EP_SIZE
#define RGBRAW_OUT_EP_INTERVAL 1

#ifdef NKRO_ENABLE
#define EXKEY_IN_EP_SIZE SHARED_EPSIZE
#else
#define EXKEY_IN_EP_SIZE 8
#endif
#define EXKEY_IN_EP_INTERVAL 1

#define QMKRAW_IN_SIZE     RAW_EPSIZE
#define QMKRAW_IN_INTERVAL 1

#define QMKRAW_OUT_EP_SIZE     RAW_EPSIZE
#define QMKRAW_OUT_EP_INTERVAL 1

#define USBD_VID           VENDOR_ID
#define USBD_PID           PRODUCT_ID
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 0x0409

void init_usb_driver();
void hid_keyboard_send_report(uint8_t mode, uint8_t *data, uint8_t len);
void raw_hid_send(uint8_t *data, uint8_t length);
void hid_exkey_send_report(uint8_t *data, uint8_t len);
void hid_qmk_raw_send_report(uint8_t *data, uint8_t len);
void hid_rgb_raw_send_report(uint8_t *data, uint8_t len);

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

#include "usbd_core.h"
#include "usbd_hid.h"
#include "usb_def.h"
#include "report.h"
#include "usb_descriptor_common.h"
#ifdef RGB_RAW_ENABLE
#include "auxiliary_rgb.h"
#endif

enum {
    InterfaceNumber_start = 0x00,
    InterfaceNumber_keyboard = InterfaceNumber_start,
#ifdef RGB_RAW_ENABLE
    InterfaceNumber_rgb_raw,
    InterfaceNumber_dynamic_lighting,
#endif
    InterfaceNumber_extra_key,
#ifdef RAW_ENABLE
    InterfaceNumber_qmk_raw,
#endif
};

const uint8_t KeyboardReport[] = {
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x06,       // Usage (Keyboard)
    0xA1, 0x01,       // Collection (Application)
    0x05, 0x07,       //   Usage Page (Keyboard/Keypad)
    0x19, 0xE0,       //   Usage Minimum (Keyboard Left Control)
    0x29, 0xE7,       //   Usage Maximum (Keyboard Right GUI)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x95, 0x08,       //   Report Count (8)
    0x75, 0x01,       //   Report Size (1)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)
    0x95, 0x01,       //   Report Count (1)
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x03,       //   Input (Constant)
    0x05, 0x08,       //   Usage Page (LED)
    0x19, 0x01,       //   Usage Minimum (Num Lock)
    0x29, 0x05,       //   Usage Maximum (Kana)
    0x95, 0x05,       //   Report Count (5)
    0x75, 0x01,       //   Report Size (1)
    0x91, 0x02,       //   Output (Data, Variable, Absolute)
    0x95, 0x01,       //   Report Count (1)
    0x75, 0x03,       //   Report Size (3)
    0x91, 0x03,       //   Output (Constant)
    0x05, 0x07,       //   Usage Page (Keyboard/Keypad)
    0x19, 0x00,       //   Usage Minimum (0)
    0x29, 0xFF,       //   Usage Maximum (255)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, 0x06,       //   Report Count (6)
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x00,       //   Input (Data, Array, Absolute)
    0xC0,             // End Collection
};

#ifdef NKRO_ENABLE
const uint8_t NkroReport[] = {
    0x05, 0x01,                                  // Usage Page (Generic Desktop)
    0x09, 0x06,                                  // Usage (Keyboard)
    0xA1, 0x01,                                  // Collection (Application),
    0x85, REPORT_ID_NKRO,                        //   REPORT_ID
    0x75, 0x01,                                  //   Report Size (1),
    0x95, 0x08,                                  //   Report Count (8),
    0x05, 0x07,                                  //   Usage Page (Key Codes),
    0x19, 0xE0,                                  //   Usage Minimum (224),
    0x29, 0xE7,                                  //   Usage Maximum (231),
    0x15, 0x00,                                  //   Logical Minimum (0),
    0x25, 0x01,                                  //   Logical Maximum (1),
    0x81, 0x02,                                  //   Input (Data, Variable, Absolute), ;Modifier byte
    0x95, KEYBOARD_REPORT_BITS * 8,              //   Report Count (),
    0x75, 0x01,                                  //   Report Size (1),
    0x15, 0x00,                                  //   Logical Minimum (0),
    0x25, 0x01,                                  //   Logical Maximum(1),
    0x05, 0x07,                                  //   Usage Page (Key Codes),
    0x19, 0x00,                                  //   Usage Minimum (0),
    0x29, (KEYBOARD_REPORT_BITS * 8 - 1) & 0xFF, //   Usage Maximum (),
    0x81, 0x02,                                  //   Input (Data, Variable, Absolute),
    0x05, 0x08,                                  //   Usage Page (LED)
    0x19, 0x01,                                  //   Usage Minimum (Num Lock)
    0x29, 0x05,                                  //   Usage Maximum (Kana)
    0x95, 0x05,                                  //   Report Count (5)
    0x75, 0x01,                                  //   Report Size (1)
    0x91, 0x02,                                  //   Output (Data, Variable, Absolute)
    0x95, 0x01,                                  //   Report Count (1)
    0x75, 0x03,                                  //   Report Size (3)
    0x91, 0x03,                                  //   Output (Constant)
    0xC0,                                        // End Collection
};
#endif

#ifdef RGB_RAW_ENABLE
const uint8_t RGBRawReport[] = {
    0x06, WBVAL(RGB_RAW_USAGE_PAGE), // Usage Page (Vendor Defined)
    0x09, RGB_RAW_USAGE_ID,          // Usage (Vendor Defined)
    0xA1, 0x01,                      // Collection (Application)
    // Data to host
    0x09, 0x62,       //   Usage (Vendor Defined)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, 0x40,       //   Report Count
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)
    // Data from host
    0x09, 0x63,       //   Usage (Vendor Defined)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, 0x40,       //   Report Count
    0x75, 0x08,       //   Report Size (8)
    0x91, 0x02,       //   Output (Data, Variable, Absolute)
    0xC0,             // End Collection
};

const uint8_t DynamicLightingReport[] = {
    0x05, 0x59,                   // Usage Page (LightingAndIllumination)
    0x09, 0x01,                   // Usage (LampArray)
    0xa1, 0x01,                   // Collection (Application)
    0x85, 0x01,                   //   REPORT_ID
    0x09, 0x02,                   //   Usage (LampArrayAttributesReport)
    0xa1, 0x02,                   //   Collection (Logical)
    0x09, 0x03,                   //     Usage (LampCount)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xff, 0xff, 0x00, 0x00, //     Logical Maximum (65535)
    0x75, 0x10,                   //     Report Size (16)
    0x95, 0x01,                   //     Report Count (1)
    0xb1, 0x03,                   //     Feature (Cnst,Var,Abs)
    0x09, 0x04,                   //     Usage (BoundingBoxWidthInMicrometers)
    0x09, 0x05,                   //     Usage (BoundingBoxHeightInMicrometers)
    0x09, 0x06,                   //     Usage (BoundingBoxDepthInMicrometers)
    0x09, 0x07,                   //     Usage (LampArrayKind)
    0x09, 0x08,                   //     Usage (MinUpdateIntervalInMicroseconds)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xff, 0xff, 0xff, 0x7f, //     Logical Maximum (2147483647)
    0x75, 0x20,                   //     Report Size (32)
    0x95, 0x05,                   //     Report Count (5)
    0xb1, 0x03,                   //     Feature (Cnst,Var,Abs)
    0xc0,                         //   End Collection
    0x85, 0x02,                   //   REPORT_ID
    0x09, 0x20,                   //   Usage (LampAttributesRequestReport)
    0xa1, 0x02,                   //   Collection (Logical)
    0x09, 0x21,                   //     Usage (LampId)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xff, 0xff, 0x00, 0x00, //     Logical Maximum (65535)
    0x75, 0x10,                   //     Report Size (16)
    0x95, 0x01,                   //     Report Count (1)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0xc0,                         //   End Collection
    0x85, 0x03,                   //   REPORT_ID
    0x09, 0x22,                   //   Usage (LampAttributesReponseReport)
    0xa1, 0x02,                   //   Collection (Logical)
    0x09, 0x21,                   //     Usage (LampId)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xff, 0xff, 0x00, 0x00, //     Logical Maximum (65535)
    0x75, 0x10,                   //     Report Size (16)
    0x95, 0x01,                   //     Report Count (1)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0x09, 0x23,                   //     Usage (PositionXInMicrometers)
    0x09, 0x24,                   //     Usage (PositionYInMicrometers)
    0x09, 0x25,                   //     Usage (PositionZInMicrometers)
    0x09, 0x27,                   //     Usage (UpdateLatencyInMicroseconds)
    0x09, 0x26,                   //     Usage (LampPurposes)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xff, 0xff, 0xff, 0x7f, //     Logical Maximum (2147483647)
    0x75, 0x20,                   //     Report Size (32)
    0x95, 0x05,                   //     Report Count (5)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0x09, 0x28,                   //     Usage (RedLevelCount)
    0x09, 0x29,                   //     Usage (GreenLevelCount)
    0x09, 0x2a,                   //     Usage (BlueLevelCount)
    0x09, 0x2b,                   //     Usage (IntensityLevelCount)
    0x09, 0x2c,                   //     Usage (IsProgrammable)
    0x09, 0x2d,                   //     Usage (InputBinding)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x26, 0xff, 0x00,             //     Logical Maximum (255)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x06,                   //     Report Count (6)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0xc0,                         //   End Collection
    0x85, 0x04,                   //   REPORT_ID
    0x09, 0x50,                   //   Usage (LampMultiUpdateReport)
    0xa1, 0x02,                   //   Collection (Logical)
    0x09, 0x03,                   //     Usage (LampCount)
    0x09, 0x55,                   //     Usage (LampUpdateFlags)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x25, 0x08,                   //     Logical Maximum (8)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x02,                   //     Report Count (2)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0x09, 0x21,                   //     Usage (LampId)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xff, 0xff, 0x00, 0x00, //     Logical Maximum (65535)
    0x75, 0x10,                   //     Report Size (16)
    0x95, 0x08,                   //     Report Count (8)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x26, 0xff, 0x00,             //     Logical Maximum (255)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x20,                   //     Report Count (32)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0xc0,                         //   End Collection
    0x85, 0x05,                   //   REPORT_ID
    0x09, 0x60,                   //   Usage (LampRangeUpdateReport)
    0xa1, 0x02,                   //   Collection (Logical)
    0x09, 0x55,                   //     Usage (LampUpdateFlags)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x25, 0x08,                   //     Logical Maximum (8)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x01,                   //     Report Count (1)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0x09, 0x61,                   //     Usage (LampIdStart)
    0x09, 0x62,                   //     Usage (LampIdEnd)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x27, 0xff, 0xff, 0x00, 0x00, //     Logical Maximum (65535)
    0x75, 0x10,                   //     Report Size (16)
    0x95, 0x02,                   //     Report Count (2)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0x09, 0x51,                   //     Usage (RedUpdateChannel)
    0x09, 0x52,                   //     Usage (GreenUpdateChannel)
    0x09, 0x53,                   //     Usage (BlueUpdateChannel)
    0x09, 0x54,                   //     Usage (IntensityUpdateChannel)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x26, 0xff, 0x00,             //     Logical Maximum (255)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x04,                   //     Report Count (4)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0xc0,                         //   End Collection
    0x85, 0x06,                   //   REPORT_ID
    0x09, 0x70,                   //   Usage (LampArrayControlReport)
    0xa1, 0x02,                   //   Collection (Logical)
    0x09, 0x71,                   //     Usage (AutonomousMode)
    0x15, 0x00,                   //     Logical Minimum (0)
    0x25, 0x01,                   //     Logical Maximum (1)
    0x75, 0x08,                   //     Report Size (8)
    0x95, 0x01,                   //     Report Count (1)
    0xb1, 0x02,                   //     Feature (Data,Var,Abs)
    0xc0,                         //   End Collection
    0xc0                          // End Collection
};
#endif

const uint8_t ExtrakeyReport[] = {
#ifdef MOUSE_ENABLE
    0x05, 0x01,            // Usage Page (Generic Desktop)
    0x09, 0x02,            // Usage (Mouse)
    0xA1, 0x01,            // Collection (Application)
    0x85, REPORT_ID_MOUSE, //   Report ID
    0x09, 0x01,            //   Usage (Pointer)
    0xA1, 0x00,            //   Collection (Physical)
    0x05, 0x09,            //     Usage Page (Button)
    0x19, 0x01,            //     Usage Minimum (Button 1)
    0x29, 0x08,            //     Usage Maximum (Button 8)
    0x15, 0x00,            //     Logical Minimum (0)
    0x25, 0x01,            //     Logical Maximum (1)
    0x95, 0x08,            //     Report Count (8)
    0x75, 0x01,            //     Report Size (1)
    0x81, 0x02,            //     Input (Data, Variable, Absolute)
    0x05, 0x01,            //     Usage Page (Generic Desktop)
    0x09, 0x30,            //     Usage (X)
    0x09, 0x31,            //     Usage (Y)
    0x15, 0x81,            //     Logical Minimum (-127)
    0x25, 0x7F,            //     Logical Maximum (127)
    0x95, 0x02,            //     Report Count (2)
    0x75, 0x08,            //     Report Size (8)
    0x81, 0x06,            //     Input (Data, Variable, Relative)
    0x09, 0x38,            //     Usage (Wheel)
    0x15, 0x81,            //     Logical Minimum (-127)
    0x25, 0x7F,            //     Logical Maximum (127)
    0x95, 0x01,            //     Report Count (1)
    0x75, 0x08,            //     Report Size (8)
    0x81, 0x06,            //     Input (Data, Variable, Relative)
    0x05, 0x0C,            //     Usage Page (Consumer)
    0x0A, 0x38, 0x02,      //     Usage (AC Pan)
    0x15, 0x81,            //     Logical Minimum (-127)
    0x25, 0x7F,            //     Logical Maximum (127)
    0x95, 0x01,            //     Report Count (1)
    0x75, 0x08,            //     Report Size (8)
    0x81, 0x06,            //     Input (Data, Variable, Relative)
    0xC0,                  //   End Collection
    0xC0,                  // End Collection
#endif
    // Extrakeys report descriptor
    0x05, 0x01,               // Usage Page (Generic Desktop)
    0x09, 0x80,               // Usage (System Control)
    0xA1, 0x01,               // Collection (Application)
    0x85, REPORT_ID_SYSTEM,   //   Report ID
    0x19, 0x01,               //   Usage Minimum (Pointer)
    0x2A, 0xB7, 0x00,         //   Usage Maximum (System Display LCD Autoscale)
    0x15, 0x01,               //   Logical Minimum
    0x26, 0xB7, 0x00,         //   Logical Maximum
    0x95, 0x01,               //   Report Count (1)
    0x75, 0x10,               //   Report Size (16)
    0x81, 0x00,               //   Input (Data, Array, Absolute)
    0xC0,                     // End Collection
    0x05, 0x0C,               // Usage Page (Consumer)
    0x09, 0x01,               // Usage (Consumer Control)
    0xA1, 0x01,               // Collection (Application)
    0x85, REPORT_ID_CONSUMER, //   Report ID
    0x19, 0x01,               //   Usage Minimum (Consumer Control)
    0x2A, 0xA0, 0x02,         //   Usage Maximum (AC Desktop Show All Applications)
    0x15, 0x01,               //   Logical Minimum
    0x26, 0xA0, 0x02,         //   Logical Maximum
    0x95, 0x01,               //   Report Count (1)
    0x75, 0x10,               //   Report Size (16)
    0x81, 0x00,               //   Input (Data, Array, Absolute)
    0xC0,                     // End Collection
};

#ifdef RAW_ENABLE
const uint8_t QMKRawReport[] = {
    0x06, WBVAL(RAW_USAGE_PAGE), // Usage Page (Vendor Defined)
    0x09, RAW_USAGE_ID,          // Usage (Vendor Defined)
    0xA1, 0x01,                  // Collection (Application)
    // Data to host
    0x09, 0x62,       //   Usage (Vendor Defined)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, 0x20,       //   Report Count
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x02,       //   Input (Data, Variable, Absolute)
    // Data from host
    0x09, 0x63,       //   Usage (Vendor Defined)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x95, 0x20,       //   Report Count
    0x75, 0x08,       //   Report Size (8)
    0x91, 0x02,       //   Output (Data, Variable, Absolute)
    0xC0,             // End Collection
};
#endif

/*!< config descriptor size */
#define USB_HID_CONFIG_DESC_SIZ_SCRATCH (USB_SIZEOF_CONFIG_DESC +                                          \
                                         USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC * 2 + \
                                         USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC)

/*!< report descriptor size */
#ifdef RGB_RAW_ENABLE
#define HID_RGBRAW_REPORT_DESC_SIZE          sizeof(RGBRawReport)
#define HID_DYNAMICLIGHTING_REPORT_DESC_SIZE sizeof(DynamicLightingReport)
#endif
#define HID_EXTRAKEY_REPORT_DESC_SIZE sizeof(ExtrakeyReport)
#ifdef RAW_ENABLE
#define HID_QMKRAW_REPORT_DESC_SIZE sizeof(QMKRawReport)
#endif

/*!< global descriptor */
const uint8_t hid_descriptor_scratch_1[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, DEVICE_VER, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT((USB_HID_CONFIG_DESC_SIZ_SCRATCH
#ifdef RGB_RAW_ENABLE
                                + USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC * 2 +
                                USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC * 2
#endif
#ifdef RAW_ENABLE
                                + USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC * 2
#endif
                                ),
                               (0x02
#ifdef RGB_RAW_ENABLE
                                + 0x01 +
                                0x01
#endif
#ifdef RAW_ENABLE
                                + 0x01
#endif
                                ),
                               0x01, USB_CONFIG_REMOTE_WAKEUP | USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
};

#ifdef NKRO_ENABLE
const uint8_t hid_descriptor_scratch_2_nkro[] = {
    /************** Descriptor of Keyboard interface ****************/
    0x09,                     /* bLength */
    0x04,                     /* bDescriptorType */
    InterfaceNumber_keyboard, /* bInterfaceNumber */
    0x00,                     /* bAlternateSetting */
    0x02,                     /* bNumEndpoints */
    0x03,                     /* bInterfaceClass */
    0x01,                     /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01,                     /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00,                     /* iInterface: Index of string descriptor */
    /******************** Descriptor of Keyboard HID ********************/
    0x09,                      /* bLength */
    0x21,                      /* bDescriptorType */
    0x11, 0x01,                /* bcdHID */
    0x00,                      /* bCountryCode */
    0x01,                      /* bNumDescriptors */
    0x22,                      /* bDescriptorType */
    WBVAL(sizeof(NkroReport)), /* wItemLength */
    /******************** Descriptor of Keyboard in endpoint ********************/
    0x07,                    /* bLength */
    0x05,                    /* bDescriptorType */
    KBD_IN_EP,               /* bEndpointAddress */
    0x03,                    /* bmAttributes */
    WBVAL(EXKEY_IN_EP_SIZE), /* wMaxPacketSize */
    KBD_IN_EP_INTERVAL,      /* bInterval */
    /******************** Descriptor of Keyboard out endpoint ********************/
    0x07,                   /* bLength */
    0x05,                   /* bDescriptorType */
    KBD_OUT_EP,             /* bEndpointAddress */
    0x03,                   /* bmAttributes */
    WBVAL(KBD_OUT_EP_SIZE), /* wMaxPacketSize */
    KBD_OUT_EP_INTERVAL,    /* bInterval */
};
#endif

const uint8_t hid_descriptor_scratch_2_bios[] = {
    /************** Descriptor of Keyboard interface ****************/
    0x09,                     /* bLength */
    0x04,                     /* bDescriptorType */
    InterfaceNumber_keyboard, /* bInterfaceNumber */
    0x00,                     /* bAlternateSetting */
    0x02,                     /* bNumEndpoints */
    0x03,                     /* bInterfaceClass */
    0x01,                     /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01,                     /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00,                     /* iInterface: Index of string descriptor */
    /******************** Descriptor of Keyboard HID ********************/
    0x09,                          /* bLength */
    0x21,                          /* bDescriptorType */
    0x11, 0x01,                    /* bcdHID */
    0x00,                          /* bCountryCode */
    0x01,                          /* bNumDescriptors */
    0x22,                          /* bDescriptorType */
    WBVAL(sizeof(KeyboardReport)), /* wItemLength */
    /******************** Descriptor of Keyboard in endpoint ********************/
    0x07,                  /* bLength */
    0x05,                  /* bDescriptorType */
    KBD_IN_EP,             /* bEndpointAddress */
    0x03,                  /* bmAttributes */
    WBVAL(KBD_IN_EP_SIZE), /* wMaxPacketSize */
    KBD_IN_EP_INTERVAL,    /* bInterval */
    /******************** Descriptor of Keyboard out endpoint ********************/
    0x07,                   /* bLength */
    0x05,                   /* bDescriptorType */
    KBD_OUT_EP,             /* bEndpointAddress */
    0x03,                   /* bmAttributes */
    WBVAL(KBD_OUT_EP_SIZE), /* wMaxPacketSize */
    KBD_OUT_EP_INTERVAL,    /* bInterval */
};

const uint8_t hid_descriptor_scratch_3[] = {
#ifdef RGB_RAW_ENABLE
    /************** Descriptor of RGB_RAW interface *****************/
    0x09,                    /* bLength */
    0x04,                    /* bDescriptorType */
    InterfaceNumber_rgb_raw, /* bInterfaceNumber */
    0x00,                    /* bAlternateSetting */
    0x02,                    /* bNumEndpoints */
    0x03,                    /* bInterfaceClass */
    0x00,                    /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                    /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00,                    /* iInterface: Index of string descriptor */
    /******************** Descriptor of RGB_RAW HID ********************/
    0x09,                               /* bLength */
    0x21,                               /* bDescriptorType */
    0x11, 0x01,                         /* bcdHID */
    0x00,                               /* bCountryCode */
    0x01,                               /* bNumDescriptors */
    0x22,                               /* bDescriptorType */
    WBVAL(HID_RGBRAW_REPORT_DESC_SIZE), /* wItemLength */
    /******************** Descriptor of RGB_RAW in endpoint ********************/
    0x07,                  /* bLength */
    0x05,                  /* bDescriptorType */
    RGBRAW_IN_EP,          /* bEndpointAddress */
    0x03,                  /* bmAttributes */
    WBVAL(RGBRAW_IN_SIZE), /* wMaxPacketSize */
    RGBRAW_IN_INTERVAL,    /* bInterval */
    /******************** Descriptor of RGB_RAW out endpoint ********************/
    0x07,                      /* bLength */
    0x05,                      /* bDescriptorType */
    RGBRAW_OUT_EP,             /* bEndpointAddress */
    0x03,                      /* bmAttributes */
    WBVAL(RGBRAW_OUT_EP_SIZE), /* wMaxPacketSize */
    RGBRAW_OUT_EP_INTERVAL,    /* bInterval */

    /************** Descriptor of DYNAMIC_LIGHTING interface *****************/
    0x09,                             /* bLength */
    0x04,                             /* bDescriptorType */
    InterfaceNumber_dynamic_lighting, /* bInterfaceNumber */
    0x00,                             /* bAlternateSetting */
    0x02,                             /* bNumEndpoints */
    0x03,                             /* bInterfaceClass */
    0x00,                             /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                             /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of DYNAMIC_LIGHTING HID ********************/
    0x09,                                        /* bLength */
    0x21,                                        /* bDescriptorType */
    0x11, 0x01,                                  /* bcdHID */
    0x00,                                        /* bCountryCode */
    0x01,                                        /* bNumDescriptors */
    0x22,                                        /* bDescriptorType */
    WBVAL(HID_DYNAMICLIGHTING_REPORT_DESC_SIZE), /* wItemLength */
    /******************** Descriptor of DYNAMIC_LIGHTING in endpoint ********************/
    0x07,                           /* bLength */
    0x05,                           /* bDescriptorType */
    DYNAMICLIGHTING_IN_EP,          /* bEndpointAddress */
    0x03,                           /* bmAttributes */
    WBVAL(DYNAMICLIGHTING_IN_SIZE), /* wMaxPacketSize */
    DYNAMICLIGHTING_IN_INTERVAL,    /* bInterval */
    /******************** Descriptor of DYNAMIC_LIGHTING out endpoint ********************/
    0x07,                               /* bLength */
    0x05,                               /* bDescriptorType */
    DYNAMICLIGHTING_OUT_EP,             /* bEndpointAddress */
    0x03,                               /* bmAttributes */
    WBVAL(DYNAMICLIGHTING_OUT_EP_SIZE), /* wMaxPacketSize */
    DYNAMICLIGHTING_OUT_EP_INTERVAL,    /* bInterval */
#endif

    /************** Descriptor of EXTRAKEY interface *****************/
    0x09,                      /* bLength */
    0x04,                      /* bDescriptorType */
    InterfaceNumber_extra_key, /* bInterfaceNumber */
    0x00,                      /* bAlternateSetting */
    0x01,                      /* bNumEndpoints */
    0x03,                      /* bInterfaceClass */
    0x00,                      /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                      /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00,                      /* iInterface: Index of string descriptor */
    /******************** Descriptor of EXTRAKEY HID ********************/
    0x09,                                 /* bLength */
    0x21,                                 /* bDescriptorType */
    0x11, 0x01,                           /* bcdHID */
    0x00,                                 /* bCountryCode */
    0x01,                                 /* bNumDescriptors */
    0x22,                                 /* bDescriptorType */
    WBVAL(HID_EXTRAKEY_REPORT_DESC_SIZE), /* wItemLength */
    /******************** Descriptor of EXTRAKEY in endpoint ********************/
    0x07,                    /* bLength */
    0x05,                    /* bDescriptorType */
    EXKEY_IN_EP,             /* bEndpointAddress */
    0x03,                    /* bmAttributes */
    WBVAL(EXKEY_IN_EP_SIZE), /* wMaxPacketSize */
    EXKEY_IN_EP_INTERVAL,    /* bInterval */

#ifdef RAW_ENABLE
    /************** Descriptor of QMKRAW interface *****************/
    0x09,                    /* bLength */
    0x04,                    /* bDescriptorType */
    InterfaceNumber_qmk_raw, /* bInterfaceNumber */
    0x00,                    /* bAlternateSetting */
    0x02,                    /* bNumEndpoints */
    0x03,                    /* bInterfaceClass */
    0x00,                    /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                    /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00,                    /* iInterface: Index of string descriptor */
    /******************** Descriptor of QMKRAW HID ********************/
    0x09,                               /* bLength */
    0x21,                               /* bDescriptorType */
    0x11, 0x01,                         /* bcdHID */
    0x00,                               /* bCountryCode */
    0x01,                               /* bNumDescriptors */
    0x22,                               /* bDescriptorType */
    WBVAL(HID_QMKRAW_REPORT_DESC_SIZE), /* wItemLength */
    /******************** Descriptor of QMKRAW in endpoint ********************/
    0x07,                  /* bLength */
    0x05,                  /* bDescriptorType */
    QMKRAW_IN_EP,          /* bEndpointAddress */
    0x03,                  /* bmAttributes */
    WBVAL(QMKRAW_IN_SIZE), /* wMaxPacketSize */
    QMKRAW_IN_INTERVAL,    /* bInterval */
    /******************** Descriptor of QMKRAW out endpoint ********************/
    0x07,                      /* bLength */
    0x05,                      /* bDescriptorType */
    QMKRAW_OUT_EP,             /* bEndpointAddress */
    0x03,                      /* bmAttributes */
    WBVAL(QMKRAW_OUT_EP_SIZE), /* wMaxPacketSize */
    QMKRAW_OUT_EP_INTERVAL,    /* bInterval */
#endif

    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x10,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'H', 0x00,                  /* wcChar0 */
    'u', 0x00,                  /* wcChar1 */
    'c', 0x00,                  /* wcChar2 */
    'k', 0x00,                  /* wcChar3 */
    'i', 0x00,                  /* wcChar4 */
    'e', 0x00,                  /* wcChar5 */
    's', 0x00,                  /* wcChar6 */
};

const uint8_t hid_descriptor_scratch_5[] = {
    ///////////////////////////////////////
    /// string3 descriptor//serial for vial
    ///////////////////////////////////////
    0x1C, 0x03, 0x76, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x3A, 0x00, 0x66, 0x00, 0x36, 0x00, 0x34, 0x00, 0x63, 0x00, 0x32, 0x00, 0x62, 0x00, 0x33, 0x00, 0x63, 0x00,
    //"vial:f64c2b3c"
    0x00
};

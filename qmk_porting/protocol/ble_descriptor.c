#include <stdint.h>
#include "report.h"
#include "protocol_ble.h"

const uint8_t hidReportMap[] = {
    0x05, 0x01,                                  // Usage Page (Generic Desktop)
    0x09, 0x06,                                  // Usage (Keyboard)
    0xA1, 0x01,                                  // Collection (Application),
    0x85, BLE_REPORT_ID_KEYBOARD,                //   REPORT_ID
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
#ifdef MOUSE_ENABLE
    0x05, 0x01,                // Usage Page (Generic Desktop)
    0x09, 0x02,                // Usage (Mouse)
    0xA1, 0x01,                // Collection (Application)
    0x85, BLE_REPORT_ID_MOUSE, //   Report ID
    0x09, 0x01,                //   Usage (Pointer)
    0xA1, 0x00,                //   Collection (Physical)
    0x05, 0x09,                //     Usage Page (Button)
    0x19, 0x01,                //     Usage Minimum (Button 1)
    0x29, 0x08,                //     Usage Maximum (Button 8)
    0x15, 0x00,                //     Logical Minimum (0)
    0x25, 0x01,                //     Logical Maximum (1)
    0x95, 0x08,                //     Report Count (8)
    0x75, 0x01,                //     Report Size (1)
    0x81, 0x02,                //     Input (Data, Variable, Absolute)
    0x05, 0x01,                //     Usage Page (Generic Desktop)
    0x09, 0x30,                //     Usage (X)
    0x09, 0x31,                //     Usage (Y)
    0x15, 0x81,                //     Logical Minimum (-127)
    0x25, 0x7F,                //     Logical Maximum (127)
    0x95, 0x02,                //     Report Count (2)
    0x75, 0x08,                //     Report Size (8)
    0x81, 0x06,                //     Input (Data, Variable, Relative)
    0x09, 0x38,                //     Usage (Wheel)
    0x15, 0x81,                //     Logical Minimum (-127)
    0x25, 0x7F,                //     Logical Maximum (127)
    0x95, 0x01,                //     Report Count (1)
    0x75, 0x08,                //     Report Size (8)
    0x81, 0x06,                //     Input (Data, Variable, Relative)
    0x05, 0x0C,                //     Usage Page (Consumer)
    0x0A, 0x38, 0x02,          //     Usage (AC Pan)
    0x15, 0x81,                //     Logical Minimum (-127)
    0x25, 0x7F,                //     Logical Maximum (127)
    0x95, 0x01,                //     Report Count (1)
    0x75, 0x08,                //     Report Size (8)
    0x81, 0x06,                //     Input (Data, Variable, Relative)
    0xC0,                      //   End Collection
    0xC0,                      // End Collection
#endif
    0x05, 0x01,                   // Usage Page (Generic Desktop)
    0x09, 0x80,                   // Usage (System Control)
    0xA1, 0x01,                   // Collection (Application)
    0x85, BLE_REPORT_ID_SYSTEM,   //   Report ID
    0x19, 0x01,                   //   Usage Minimum (Pointer)
    0x2A, 0xB7, 0x00,             //   Usage Maximum (System Display LCD Autoscale)
    0x15, 0x01,                   //   Logical Minimum
    0x26, 0xB7, 0x00,             //   Logical Maximum
    0x95, 0x01,                   //   Report Count (1)
    0x75, 0x10,                   //   Report Size (16)
    0x81, 0x00,                   //   Input (Data, Array, Absolute)
    0xC0,                         // End Collection
    0x05, 0x0C,                   // Usage Page (Consumer)
    0x09, 0x01,                   // Usage (Consumer Control)
    0xA1, 0x01,                   // Collection (Application)
    0x85, BLE_REPORT_ID_CONSUMER, //   Report ID
    0x19, 0x01,                   //   Usage Minimum (Consumer Control)
    0x2A, 0xA0, 0x02,             //   Usage Maximum (AC Desktop Show All Applications)
    0x15, 0x01,                   //   Logical Minimum
    0x26, 0xA0, 0x02,             //   Logical Maximum
    0x95, 0x01,                   //   Report Count (1)
    0x75, 0x10,                   //   Report Size (16)
    0x81, 0x00,                   //   Input (Data, Array, Absolute)
    0xC0,                         // End Collection
    0x06, 0x60, 0xFF,             // Usage Page (Vendor Defined)
    0x09, 0x61,                   // Usage (Vendor Defined)
    0xA1, 0x01,                   // Collection (Application)
    0x85, BLE_REPORT_ID_CUSTOM,   //   REPORT_ID
    0x09, 0x62,                   //   Usage (Vendor Defined)
    0x15, 0x00,                   //   Logical Minimum (0)
    0x26, 0xFF, 0x00,             //   Logical Maximum (255)
    0x95, 0x20,                   //   Report Count
    0x75, 0x08,                   //   Report Size (8)
    0x81, 0x02,                   //   Input (Data, Variable, Absolute)
    0x09, 0x63,                   //   Usage (Vendor Defined)
    0x15, 0x00,                   //   Logical Minimum (0)
    0x26, 0xFF, 0x00,             //   Logical Maximum (255)
    0x95, 0x20,                   //   Report Count
    0x75, 0x08,                   //   Report Size (8)
    0x91, 0x02,                   //   Output (Data, Variable, Absolute)
    0xC0,                         // End Collection
};

const uint16_t hidReportMapLen = sizeof(hidReportMap);

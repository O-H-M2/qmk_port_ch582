#pragma once

#include "usbd_core.h"
#include "usbd_hid.h"
#include "usb_def.h"
#include "report.h"

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

const uint8_t ExtrkeyReport[] = {
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
#ifdef NKRO_ENABLE
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
    0xC0,                                        // End Collection
#endif
};

const uint8_t RawReport[] = {
    0x06, 0x60, 0xFF, // Usage Page (Vendor Defined)
    0x09, 0x61,       // Usage (Vendor Defined)
    0xA1, 0x01,       // Collection (Application)
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

/*!< config descriptor size */
// #define USB_HID_CONFIG_DESC_SIZ 0x5B
#define USB_HID_CONFIG_DESC_SIZ (USB_SIZEOF_CONFIG_DESC +                                          \
                                 USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC * 2 + \
                                 USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC +     \
                                 USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC * 2)

/*!< report descriptor size */
#define HID_KEYBOARD_REPORT_DESC_SIZE sizeof(KeyboardReport)

#define HID_RAWHID_REPORT_DESC_SIZE sizeof(RawReport)

#define HID_EXTRAKEY_REPORT_DESC_SIZE sizeof(ExtrkeyReport)

/*!< global descriptor */
const uint8_t hid_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, DEVICE_VER, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_HID_CONFIG_DESC_SIZ, 0x03, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),

    /************** Descriptor of Keyboard interface ****************/
    0x09, /* bLength */
    0x04, /* bDescriptorType */
    0x00, /* bInterfaceNumber */
    0x00, /* bAlternateSetting */
    0x02, /* bNumEndpoints */
    0x03, /* bInterfaceClass */
    0x01, /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01, /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00, /* iInterface: Index of string descriptor */
    /******************** Descriptor of Keyboard HID ********************/
    0x09,                                 /* bLength */
    0x21,                                 /* bDescriptorType */
    0x11, 0x01,                           /* bcdHID */
    0x00,                                 /* bCountryCode */
    0x01,                                 /* bNumDescriptors */
    0x22,                                 /* bDescriptorType */
    WBVAL(HID_KEYBOARD_REPORT_DESC_SIZE), /* wItemLength */
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

    /************** Descriptor of EXTRAKEY interface *****************/
    0x09, /* bLength */
    0x04, /* bDescriptorType */
    0x01, /* bInterfaceNumber */
    0x00, /* bAlternateSetting */
    0x01, /* bNumEndpoints */
    0x03, /* bInterfaceClass */
    0x00, /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00, /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00, /* iInterface: Index of string descriptor */
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

    /************** Descriptor of RAWHID interface *****************/
    0x09, /* bLength */
    0x04, /* bDescriptorType */
    0x02, /* bInterfaceNumber */
    0x00, /* bAlternateSetting */
    0x02, /* bNumEndpoints */
    0x03, /* bInterfaceClass */
    0x00, /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00, /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0x00, /* iInterface: Index of string descriptor */
    /******************** Descriptor of RAWHID HID ********************/
    0x09,                               /* bLength */
    0x21,                               /* bDescriptorType */
    0x11, 0x01,                         /* bcdHID */
    0x00,                               /* bCountryCode */
    0x01,                               /* bNumDescriptors */
    0x22,                               /* bDescriptorType */
    WBVAL(HID_RAWHID_REPORT_DESC_SIZE), /* wItemLength */
    /******************** Descriptor of RAWHID in endpoint ********************/
    0x07,                  /* bLength */
    0x05,                  /* bDescriptorType */
    HIDRAW_IN_EP,          /* bEndpointAddress */
    0x03,                  /* bmAttributes */
    WBVAL(HIDRAW_IN_SIZE), /* wMaxPacketSize */
    HIDRAW_IN_INTERVAL,    /* bInterval */
    /******************** Descriptor of RAWHID out endpoint ********************/
    0x07,                      /* bLength */
    0x05,                      /* bDescriptorType */
    HIDRAW_OUT_EP,             /* bEndpointAddress */
    0x03,                      /* bmAttributes */
    WBVAL(HIDRAW_OUT_EP_SIZE), /* wMaxPacketSize */
    HIDRAW_OUT_EP_INTERVAL,    /* bInterval */

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
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x28,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'M', 0x00,                  /* wcChar0 */
    'e', 0x00,                  /* wcChar1 */
    'c', 0x00,                  /* wcChar2 */
    'h', 0x00,                  /* wcChar3 */
    'a', 0x00,                  /* wcChar4 */
    'n', 0x00,                  /* wcChar5 */
    'i', 0x00,                  /* wcChar6 */
    'c', 0x00,                  /* wcChar7 */
    'a', 0x00,                  /* wcChar8 */
    'l', 0x00,                  /* wcChar9 */
    ' ', 0x00,                  /* wcChar10 */
    'k', 0x00,                  /* wcChar11 */
    'e', 0x00,                  /* wcChar12 */
    'y', 0x00,                  /* wcChar13 */
    'b', 0x00,                  /* wcChar14 */
    'o', 0x00,                  /* wcChar15 */
    'a', 0x00,                  /* wcChar16 */
    'r', 0x00,                  /* wcChar17 */
    'd', 0x00,                  /* wcChar18 */
    ///////////////////////////////////////
    /// string3 descriptor//serial for vial
    ///////////////////////////////////////
    0x1C, 0x03, 0x76, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x3A, 0x00, 0x66, 0x00, 0x36, 0x00, 0x34, 0x00, 0x63, 0x00, 0x32, 0x00, 0x62, 0x00, 0x33, 0x00, 0x63, 0x00,
    //"vial:f64c2b3c"
    0x00
};
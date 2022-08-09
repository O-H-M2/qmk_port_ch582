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
#define USB_HID_CONFIG_DESC_SIZ 0x5B

/*!< report descriptor size */
#define HID_KEYBOARD_REPORT_DESC_SIZE sizeof(KeyboardReport)

#define HID_RAWHID_REPORT_DESC_SIZE sizeof(RawReport)

#define HID_EXTRAKEY_REPORT_DESC_SIZE sizeof(ExtrkeyReport)

/*!< global descriptor */
const uint8_t hid_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0002, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_HID_CONFIG_DESC_SIZ, 0x03, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),

    /************** Descriptor of Keyboard interface ****************/
    //0x09,0x04,0x00,0x00,0x01,0x03,0x01,0x01,0x00
    /*err 09 */
    0x09, /* bLength: Interface Descriptor size */
    0x04, /* bDescriptorType: Interface descriptor type */
    0x00, /* bInterfaceNumber: Number of Interface */
    0x00, /* bAlternateSetting: Alternate setting */
    0x01, /* bNumEndpoints */
    0x03, /* bInterfaceClass: HID */
    0x01, /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01, /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,    /* iInterface: Index of string descriptor */
    /******************** Descriptor of Keyboard HID ********************/
    /*err 18 */
    //0x09,0x21,0x11,0x01,0x00,0x01,0x22,0x40,0x00
    0x09, /* bLength: HID Descriptor size */
    0x21, /* bDescriptorType: HID */
    0x11, /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                          /* bCountryCode: Hardware target country */
    0x01,                          /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                          /* bDescriptorType */
    HID_KEYBOARD_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Keyboard in endpoint ********************/
    /*err 27 */
    //0x07,0x05,0x81,0x03,0x08,0x00,0x0A
    0x07,           /* bLength: Endpoint Descriptor size */
    0x05,           /* bDescriptorType: */
    KBD_IN_EP,      /* bEndpointAddress: Endpoint Address (IN) */
    0x03,           /* bmAttributes: Interrupt endpoint */
    KBD_IN_EP_SIZE, /* wMaxPacketSize: 4 Byte max */
    0x00,
    KBD_IN_EP_INTERVAL, /* bInterval: Polling Interval */

    /************** Descriptor of RAWHID interface *****************/
    /*err 41 */
    //0x09,0x04,0x01,0x00,0x02,0x03,0x00,0x00,0x00
    0x09, /* bLength: Interface Descriptor size */
    0x04, /* bDescriptorType: Interface descriptor type */
    0x01, /* bInterfaceNumber: Number of Interface */
    0x00, /* bAlternateSetting: Alternate setting */
    0x02, /* bNumEndpoints */
    0x03, /* bInterfaceClass: HID */
    0x00, /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00, /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,    /* iInterface: Index of string descriptor */
    /******************** Descriptor of RAWHID HID ********************/
    /*err 50 */
    //0x09,0x21,0x11,0x01,0x00,0x01,0x22,0x22,0x00
    0x09, /* bLength: HID Descriptor size */
    0x21, /* bDescriptorType: HID */
    0x11, /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                        /* bCountryCode: Hardware target country */
    0x01,                        /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                        /* bDescriptorType */
    HID_RAWHID_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of RAWHID in endpoint ********************/
    /*err 59 */
    //0x07,0x05,0x82,0x03,0x20,0x00,0x0A
    0x07,           /* bLength: Endpoint Descriptor size */
    0x05,           /* bDescriptorType: */
    HIDRAW_IN_EP,   /* bEndpointAddress: Endpoint Address (IN) */
    0x03,           /* bmAttributes: Interrupt endpoint */
    HIDRAW_IN_SIZE, /* wMaxPacketSize: 4 Byte max */
    0x00,
    HIDRAW_IN_INTERVAL, /* bInterval: Polling Interval */
    /******************** Descriptor of RAWHID out endpoint ********************/
    /*err 66 */
    //0x07,0x05,0x03,0x03,0x20,0x00,0x01
    0x07,               /* bLength: Endpoint Descriptor size */
    0x05,               /* bDescriptorType: */
    HIDRAW_OUT_EP,      /* bEndpointAddress: Endpoint Address (IN) */
    0x03,               /* bmAttributes: Interrupt endpoint */
    HIDRAW_OUT_EP_SIZE, /* wMaxPacketSize: 4 Byte max */
    0x00,
    HIDRAW_OUT_EP_INTERVAL, /* bInterval: Polling Interval */

    /************** Descriptor of EXTRAKEY interface *****************/
    /*err 41 */
    //0x09,0x04,0x02,0x00,0x01,0x03,0x00,0x00,0x00
    0x09, /* bLength: Interface Descriptor size */
    0x04, /* bDescriptorType: Interface descriptor type */
    0x02, /* bInterfaceNumber: Number of Interface */
    0x00, /* bAlternateSetting: Alternate setting */
    0x01, /* bNumEndpoints */
    0x03, /* bInterfaceClass: HID */
    0x00, /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00, /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,    /* iInterface: Index of string descriptor */
    /******************** Descriptor of EXTRAKEY HID ********************/
    /* 50 */
    //0x09,0x21,0x11,0x01,0x00,0x01,0x22,0xBC,0x00
    0x09, /* bLength: HID Descriptor size */
    0x21, /* bDescriptorType: HID */
    0x11, /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                          /* bCountryCode: Hardware target country */
    0x01,                          /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                          /* bDescriptorType */
    HID_EXTRAKEY_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of EXTRAKEY out endpoint ********************/
    /*err 66 */
    //0x07,0x05,0x84,0x03,0x20,0x00,0x01
    0x07,             /* bLength: Endpoint Descriptor size */
    0x05,             /* bDescriptorType: */
    0x84,             /* bEndpointAddress: Endpoint Address (IN) */
    0x03,             /* bmAttributes: Interrupt endpoint */
    EXKEY_IN_EP_SIZE, /* wMaxPacketSize: 4 Byte max */
    0x00,
    HIDRAW_OUT_EP_INTERVAL, /* bInterval: Polling Interval */
    /*err 73 */
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'O', 0x00,                  /* wcChar0 */
    'c', 0x00,                  /* wcChar1 */
    't', 0x00,                  /* wcChar2 */
    'o', 0x00,                  /* wcChar3 */
    'c', 0x00,                  /* wcChar4 */
    'K', 0x00,                  /* wcChar5 */
    'i', 0x00,                  /* wcChar6 */
    'e', 0x00,                  /* wcChar7 */
    's', 0x00,                  /* wcChar8 */
    'Z', 0x00,                  /* wcChar9 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x0A,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'm', 0x00,                  /* wcChar0 */
    'k', 0x00,                  /* wcChar1 */
    '0', 0x00,                  /* wcChar2 */
    '1', 0x00,                  /* wcChar3 */
    ///////////////////////////////////////
    /// string3 descriptor//serial for vial
    ///////////////////////////////////////
    0x1C, 0x03, 0x76, 0x00, 0x69, 0x00, 0x61, 0x00, 0x6C, 0x00, 0x3A, 0x00, 0x66, 0x00, 0x36, 0x00, 0x34, 0x00, 0x63, 0x00, 0x32, 0x00, 0x62, 0x00, 0x33, 0x00, 0x63, 0x00,
    //"vial:f64c2b3c"
    0x00
};
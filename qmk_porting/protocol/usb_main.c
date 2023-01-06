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

#include "usb_main.h"
#include "usb_descriptors.h"
#if ESB_ENABLE == 2
#include "esb.h"
#endif

uint8_t keyboard_protocol = 1;
uint8_t keyboard_idle = 0;
static uint8_t *hid_descriptor = NULL;
/*!< hid state ! Data can be sent only when state is idle  */
static struct usbd_interface keyboard_interface;
static struct usbd_interface extrakey_interface;
#ifdef RAW_ENABLE
static struct usbd_interface qmkraw_interface;
#endif

#ifdef EZRAW_ENABLE
static struct usbd_interface ezraw_interface;
#endif

static uint8_t keyboard_state = HID_STATE_IDLE;
static uint8_t extrakey_state = HID_STATE_IDLE;
#ifdef RAW_ENABLE
static uint8_t qmkraw_state = HID_STATE_IDLE;
#endif
#ifdef EZRAW_ENABLE
static uint8_t ezraw_state = HID_STATE_IDLE;
#endif

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t kbd_out_buffer[CONFIG_USB_ALIGN_SIZE];
#ifdef RAW_ENABLE
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t qmkraw_out_buffer[QMKRAW_OUT_EP_SIZE];
#endif
#ifdef EZRAW_ENABLE
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t ezraw_out_buffer[EZRAW_OUT_EP_SIZE];
#endif

void usbd_hid_kbd_in_callback(uint8_t ep, uint32_t nbytes)
{
    keyboard_state = HID_STATE_IDLE;
}

void usbd_hid_kbd_out_callback(uint8_t ep, uint32_t nbytes)
{
    usbd_ep_start_read(ep, kbd_out_buffer, KBD_OUT_EP_SIZE);
#if ESB_ENABLE == 2
    extern void esb_send_response(uint8_t reportid, uint8_t * data, uint8_t len);

    esb_send_response(REPORT_ID_KEYBOARD, kbd_out_buffer, KBD_OUT_EP_SIZE);
#else
    keyboard_led_state = kbd_out_buffer[0];
#endif
}

#ifdef RAW_ENABLE
void usbd_hid_qmk_raw_in_callback(uint8_t ep, uint32_t nbytes)
{
    qmkraw_state = HID_STATE_IDLE;
}

void usbd_hid_qmk_raw_out_callback(uint8_t ep, uint32_t nbytes)
{
    usbd_ep_start_read(ep, qmkraw_out_buffer, sizeof(qmkraw_out_buffer));
#if ESB_ENABLE == 2
    extern void esb_send_response(uint8_t reportid, uint8_t * data, uint8_t len);

    esb_send_response(REPORT_ID_CUSTOM, qmkraw_out_buffer, sizeof(qmkraw_out_buffer));
#else
    extern void raw_hid_receive(uint8_t * data, uint8_t length);

    raw_hid_receive(qmkraw_out_buffer, sizeof(qmkraw_out_buffer));
#endif
}
#endif

#ifdef EZRAW_ENABLE
void usbd_hid_ez_raw_in_callback(uint8_t ep, uint32_t nbytes)
{
    ezraw_state = HID_STATE_IDLE;
}

void usbd_hid_ez_raw_out_callback(uint8_t ep, uint32_t nbytes)
{
    usbd_ep_start_read(ep, ezraw_out_buffer, sizeof(ezraw_out_buffer));
    extern void ez_raw_hid_receive(uint8_t * data, uint8_t length);

    ezraw_hid_receive(ezraw_out_buffer, sizeof(ezraw_out_buffer));
}
#endif

void usbd_hid_exkey_in_callback(uint8_t ep, uint32_t nbytes)
{
    extrakey_state = HID_STATE_IDLE;
}

void usbd_configure_done_callback()
{
    usbd_ep_start_read(KBD_OUT_EP, kbd_out_buffer, KBD_OUT_EP_SIZE);
#ifdef RAW_ENABLE
    usbd_ep_start_read(QMKRAW_OUT_EP, qmkraw_out_buffer, sizeof(qmkraw_out_buffer));
#endif
#ifdef EZRAW_ENABLE
    usbd_ep_start_read(EZRAW_OUT_EP, ezraw_out_buffer, sizeof(ezraw_out_buffer));
#endif
}

void usb_dc_low_level_init()
{
    DelayUs(100);
    PFIC_EnableIRQ(USB_IRQn);
}

void init_usb_driver()
{
    struct usbd_endpoint keyboard_in_ep = {
        .ep_cb = usbd_hid_kbd_in_callback,
        .ep_addr = KBD_IN_EP
    };

    struct usbd_endpoint keyboard_out_ep = {
        .ep_cb = usbd_hid_kbd_out_callback,
        .ep_addr = KBD_OUT_EP
    };

    struct usbd_endpoint exkey_in_ep = {
        .ep_cb = usbd_hid_exkey_in_callback,
        .ep_addr = EXKEY_IN_EP
    };

#ifdef RAW_ENABLE
    struct usbd_endpoint qmkraw_in_ep = {
        .ep_cb = usbd_hid_qmk_raw_in_callback,
        .ep_addr = QMKRAW_IN_EP
    };

    struct usbd_endpoint qmkraw_out_ep = {
        .ep_cb = usbd_hid_qmk_raw_out_callback,
        .ep_addr = QMKRAW_OUT_EP
    };
#endif

#ifdef EZRAW_ENABLE
    struct usbd_endpoint ezraw_in_ep = {
        .ep_cb = usbd_hid_ez_raw_in_callback,
        .ep_addr = EZRAW_IN_EP
    };

    struct usbd_endpoint ezraw_out_ep = {
        .ep_cb = usbd_hid_ez_raw_out_callback,
        .ep_addr = EZRAW_OUT_EP
    };
#endif

    // build descriptor according to the keyboard name
#if ESB_ENABLE == 2
    uint8_t keyboard_name[] = MACRO2STR(PRODUCT) "2.4G";
#else
    uint8_t keyboard_name[] = MACRO2STR(PRODUCT);
#endif
    uint8_t hid_descriptor_scratch_2[((sizeof(keyboard_name) - 1) + 1) * 2] = {
        ///////////////////////////////////////
        /// string2 descriptor
        ///////////////////////////////////////
        ((sizeof(keyboard_name) - 1) + 1) * 2, /* bLength */
        USB_DESCRIPTOR_TYPE_STRING,            /* bDescriptorType */
    };

    for (uint8_t i = 0; i < (sizeof(keyboard_name) - 1); i++) {
        hid_descriptor_scratch_2[2 + i * 2] = keyboard_name[i];
        hid_descriptor_scratch_2[2 + i * 2 + 1] = 0x00;
    }

    if (hid_descriptor != NULL) {
        free(hid_descriptor);
    }
    hid_descriptor = (uint8_t *)malloc(sizeof(hid_descriptor_scratch_1) + sizeof(hid_descriptor_scratch_2) + sizeof(hid_descriptor_scratch_3));
#if ESB_ENABLE == 2
    tmos_memcpy(hid_descriptor, hid_descriptor_scratch_1, sizeof(hid_descriptor_scratch_1));
    tmos_memcpy(hid_descriptor + sizeof(hid_descriptor_scratch_1), hid_descriptor_scratch_2, sizeof(hid_descriptor_scratch_2));
    tmos_memcpy(hid_descriptor + sizeof(hid_descriptor_scratch_1) + sizeof(hid_descriptor_scratch_2), hid_descriptor_scratch_3, sizeof(hid_descriptor_scratch_3));
#else
    memcpy(hid_descriptor, hid_descriptor_scratch_1, sizeof(hid_descriptor_scratch_1));
    memcpy(hid_descriptor + sizeof(hid_descriptor_scratch_1), hid_descriptor_scratch_2, sizeof(hid_descriptor_scratch_2));
    memcpy(hid_descriptor + sizeof(hid_descriptor_scratch_1) + sizeof(hid_descriptor_scratch_2), hid_descriptor_scratch_3, sizeof(hid_descriptor_scratch_3));
#endif

    usbd_desc_register(hid_descriptor);

    usbd_add_interface(usbd_hid_init_intf(&keyboard_interface, KeyboardReport, HID_KEYBOARD_REPORT_DESC_SIZE));
    usbd_add_endpoint(&keyboard_in_ep);
    usbd_add_endpoint(&keyboard_out_ep);

    usbd_add_interface(usbd_hid_init_intf(&extrakey_interface, ExtrakeyReport, HID_EXTRAKEY_REPORT_DESC_SIZE));
    usbd_add_endpoint(&exkey_in_ep);

#ifdef RAW_ENABLE
    usbd_add_interface(usbd_hid_init_intf(&qmkraw_interface, QMKRawReport, HID_QMKRAW_REPORT_DESC_SIZE));
    usbd_add_endpoint(&qmkraw_in_ep);
    usbd_add_endpoint(&qmkraw_out_ep);
#endif

#ifdef EZRAW_ENABLE
    usbd_add_interface(usbd_hid_init_intf(&ezraw_interface, EZRawReport, HID_EZRAW_REPORT_DESC_SIZE));
    usbd_add_endpoint(&ezraw_in_ep);
    usbd_add_endpoint(&ezraw_out_ep);
#endif

    usbd_initialize();
}

void hid_bios_keyboard_send_report(uint8_t *data, uint8_t len)
{
    int ret = usbd_ep_start_write(KBD_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    keyboard_state = HID_STATE_BUSY;
}

void hid_nkro_keyboard_send_report(uint8_t *data, uint8_t len)
{
    hid_exkey_send_report(data, len);
}

inline void hid_exkey_send_report(uint8_t *data, uint8_t len)
{
    int ret = usbd_ep_start_write(EXKEY_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    extrakey_state = HID_STATE_BUSY;
}

#ifdef RAW_ENABLE
void hid_qmk_raw_send_report(uint8_t *data, uint8_t len)
{
    int ret = usbd_ep_start_write(QMKRAW_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    qmkraw_state = HID_STATE_BUSY;
}
#endif

#ifdef EZRAW_ENABLE
void hid_ez_raw_send_report(uint8_t *data, uint8_t len)
{
    int ret = usbd_ep_start_write(EZRAW_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    ezraw_state = HID_STATE_BUSY;
}
#endif
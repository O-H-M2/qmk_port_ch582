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
/*!< hid state ! Data can be sent only when state is idle  */
static struct usbd_interface keyboard_interface;
static struct usbd_interface exkey_interface;
static struct usbd_interface rawhid_interface;
static uint8_t keyboard_state = HID_STATE_IDLE;
static uint8_t custom_state = HID_STATE_IDLE;
static uint8_t exkey_state = HID_STATE_IDLE;

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t kbd_out_buffer[CONFIG_USB_ALIGN_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t custom_out_buffer[HIDRAW_OUT_EP_SIZE];

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

void usbd_hid_custom_in_callback(uint8_t ep, uint32_t nbytes)
{
    custom_state = HID_STATE_IDLE;
}

void usbd_hid_custom_out_callback(uint8_t ep, uint32_t nbytes)
{
    usbd_ep_start_read(ep, custom_out_buffer, HIDRAW_OUT_EP_SIZE);
#if ESB_ENABLE == 2
    extern void esb_send_response(uint8_t reportid, uint8_t * data, uint8_t len);

    esb_send_response(REPORT_ID_CUSTOM, custom_out_buffer, HIDRAW_OUT_EP_SIZE);
#else
    extern void raw_hid_receive(uint8_t * data, uint8_t length);

    raw_hid_receive(custom_out_buffer, HIDRAW_OUT_EP_SIZE);
#endif
}

void usbd_hid_exkey_in_callback(uint8_t ep, uint32_t nbytes)
{
    exkey_state = HID_STATE_IDLE;
}

void usbd_configure_done_callback()
{
    usbd_ep_start_read(KBD_OUT_EP, kbd_out_buffer, KBD_OUT_EP_SIZE);
    usbd_ep_start_read(HIDRAW_OUT_EP, custom_out_buffer, HIDRAW_OUT_EP_SIZE);
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

    struct usbd_endpoint rawhid_in_ep = {
        .ep_cb = usbd_hid_custom_in_callback,
        .ep_addr = HIDRAW_IN_EP
    };

    struct usbd_endpoint rawhid_out_ep = {
        .ep_cb = usbd_hid_custom_out_callback,
        .ep_addr = HIDRAW_OUT_EP
    };

    usbd_desc_register(hid_descriptor);

    usbd_add_interface(usbd_hid_init_intf(&keyboard_interface, KeyboardReport, HID_KEYBOARD_REPORT_DESC_SIZE));
    usbd_add_endpoint(&keyboard_in_ep);
    usbd_add_endpoint(&keyboard_out_ep);

    usbd_add_interface(usbd_hid_init_intf(&exkey_interface, ExtrkeyReport, HID_EXTRAKEY_REPORT_DESC_SIZE));
    usbd_add_endpoint(&exkey_in_ep);

    usbd_add_interface(usbd_hid_init_intf(&rawhid_interface, RawReport, HID_RAWHID_REPORT_DESC_SIZE));
    usbd_add_endpoint(&rawhid_in_ep);
    usbd_add_endpoint(&rawhid_out_ep);

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

void hid_exkey_send_report(uint8_t *data, uint8_t len)
{
    int ret = usbd_ep_start_write(EXKEY_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    exkey_state = HID_STATE_BUSY;
}

void hid_custom_send_report(uint8_t *data, uint8_t len)
{
    int ret = usbd_ep_start_write(HIDRAW_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    custom_state = HID_STATE_BUSY;
}

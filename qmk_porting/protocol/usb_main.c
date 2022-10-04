#include "usb_main.h"
#include "usb_descriptors.h"

uint8_t keyboard_protocol = 1;
uint8_t keyboard_idle = 0;
extern uint8_t keyboard_led_state;

static usbd_class_t hid_class;
static usbd_interface_t hid_intf_1;
static usbd_interface_t hid_intf_2;
static usbd_interface_t hid_intf_3;
/*!< hid state ! Data can be sent only when state is idle  */
static uint8_t keyboard_state = HID_STATE_IDLE;
static uint8_t custom_state = HID_STATE_IDLE;
static uint8_t exkey_state = HID_STATE_IDLE;

void usbd_hid_kbd_in_callback(uint8_t ep)
{
    if (keyboard_state == HID_STATE_BUSY) {
        keyboard_state = HID_STATE_IDLE;
    }
}

void usbd_hid_kbd_out_callback(uint8_t ep)
{
    uint8_t led_state;

    usbd_ep_read(ep, &led_state, KBD_OUT_EP_SIZE, NULL);
    keyboard_led_state = led_state;
}

__attribute__((weak)) void raw_hid_receive(uint8_t *data, uint8_t length)
{
    // placeholder in case VIA is disabled
}

void usbd_hid_custom_in_callback(uint8_t ep)
{
    if (custom_state == HID_STATE_BUSY) {
        custom_state = HID_STATE_IDLE;
    }
}

void usbd_hid_custom_out_callback(uint8_t ep)
{
    uint8_t custom_data[HIDRAW_OUT_EP_SIZE];
    usbd_ep_read(HIDRAW_OUT_EP, custom_data, HIDRAW_OUT_EP_SIZE, NULL);

    raw_hid_receive(custom_data, HIDRAW_OUT_EP_SIZE);
}

void usbd_hid_exkey_in_callback(uint8_t ep)
{
    if (exkey_state == HID_STATE_BUSY) {
        exkey_state = HID_STATE_IDLE;
    }
}

void init_usb_driver()
{
    usbd_endpoint_t keyboard_in_ep = {
        .ep_cb = usbd_hid_kbd_in_callback,
        .ep_addr = KBD_IN_EP
    };

    usbd_endpoint_t keyboard_out_ep = {
        .ep_cb = usbd_hid_kbd_out_callback,
        .ep_addr = KBD_OUT_EP
    };

    usbd_endpoint_t exkey_in_ep = {
        .ep_cb = usbd_hid_exkey_in_callback,
        .ep_addr = EXKEY_IN_EP
    };

    usbd_endpoint_t rawhid_in_ep = {
        .ep_cb = usbd_hid_custom_in_callback,
        .ep_addr = HIDRAW_IN_EP
    };

    usbd_endpoint_t rawhid_out_ep = {
        .ep_cb = usbd_hid_custom_out_callback,
        .ep_addr = HIDRAW_OUT_EP
    };

    usbd_desc_register(hid_descriptor);
    usbd_hid_add_interface(&hid_class, &hid_intf_1);
    usbd_interface_add_endpoint(&hid_intf_1, &keyboard_in_ep);
    usbd_interface_add_endpoint(&hid_intf_1, &keyboard_out_ep);

    usbd_hid_add_interface(&hid_class, &hid_intf_2);
    usbd_interface_add_endpoint(&hid_intf_2, &exkey_in_ep);

    usbd_hid_add_interface(&hid_class, &hid_intf_3);
    usbd_interface_add_endpoint(&hid_intf_3, &rawhid_in_ep);
    usbd_interface_add_endpoint(&hid_intf_3, &rawhid_out_ep);

    usbd_hid_report_descriptor_register(0, KeyboardReport, HID_KEYBOARD_REPORT_DESC_SIZE);
    usbd_hid_report_descriptor_register(1, ExtrkeyReport, HID_EXTRAKEY_REPORT_DESC_SIZE);
    usbd_hid_report_descriptor_register(2, RawReport, HID_RAWHID_REPORT_DESC_SIZE);
    usbd_initialize();
}

void hid_bios_keyboard_send_report(uint8_t *data, uint8_t len)
{
    if (usb_device_is_configured()) {
        if (keyboard_state == HID_STATE_IDLE) {
            keyboard_state = HID_STATE_BUSY;
            usbd_ep_write(KBD_IN_EP, data, len, NULL);
        }
    }
}

void hid_nkro_keyboard_send_report(uint8_t *data, uint8_t len)
{
    hid_exkey_send_report(data, len);
}

void hid_exkey_send_report(uint8_t *data, uint8_t len)
{
    if (usb_device_is_configured()) {
        if (exkey_state == HID_STATE_IDLE) {
            exkey_state = HID_STATE_BUSY;
            usbd_ep_write(EXKEY_IN_EP, data, len, NULL);
        }
    }
}

void hid_custom_send_report(uint8_t *data, uint8_t len)
{
    if (len != HIDRAW_IN_SIZE) {
        return;
    }
    if (usb_device_is_configured()) {
        if (custom_state == HID_STATE_IDLE) {
            custom_state = HID_STATE_BUSY;
            usbd_ep_write(HIDRAW_IN_EP, data, len, NULL);
        }
    }
}

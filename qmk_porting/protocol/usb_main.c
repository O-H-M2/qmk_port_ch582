#include "usb_main.h"
#include "usb_descriptors.h"

uint8_t keyboard_protocol = 1;
uint8_t keyboard_idle = 0;

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
    /*!< endpoint call back */
    /*!< transfer successfully */
    if (keyboard_state == HID_STATE_BUSY) {
        /*!< update the state  */
        keyboard_state = HID_STATE_IDLE;
    }
}

void usbd_hid_kbd_out_callback(uint8_t ep)
{
    /*!< here you can write the LED processing from the host */
    enum hid_kbd_led led_state;
    /*!< read the led data from host send */
    usbd_ep_read(ep, (uint8_t *)(&led_state), KBD_OUT_EP_SIZE, NULL);
    /*!< diy */
    //TODO: finish the keyboard led part
    if (led_state & HID_KBD_LED_NUM_LOCK) {
        /*!< num lock */
        /*!< do what you like */
    } else {
    }
    if (led_state & HID_KBD_LED_CAPS_LOCK) {
        /*!< caps lock */
    } else {
    }

    if (led_state & HID_KBD_LED_SCROLL_LOCK) {
        /*!< scroll lock */
        /*!< do what you like */
    } else {
    }
    if (led_state & HID_KBD_LED_COMPOSE) {
        /*!< compose led */
        /*!< do what you like */
    } else {
    }
    if (led_state & HID_KBD_LED_KANA) {
        /*!< kana led */
        /*!< do what you like */
    } else {
    }
}

__attribute__((weak)) void raw_hid_receive(uint8_t *data, uint8_t length)
{
}

void usbd_hid_custom_in_callback(uint8_t ep)
{
    /*!< endpoint call back */
    /*!< transfer successfully */
    if (custom_state == HID_STATE_BUSY) {
        /*!< update the state  */
        custom_state = HID_STATE_IDLE;
    }
}

void usbd_hid_custom_out_callback(uint8_t ep)
{
    /*!< read the data from host send */
    uint8_t custom_data[HIDRAW_OUT_EP_SIZE];
    usbd_ep_read(HIDRAW_OUT_EP, custom_data, HIDRAW_OUT_EP_SIZE, NULL);

    /*!< you can use the data do some thing you like */
    raw_hid_receive(custom_data, HIDRAW_OUT_EP_SIZE);
}

void usbd_hid_exkey_in_callback(uint8_t ep)
{
    /*!< endpoint call back */
    /*!< transfer successfully */
    if (exkey_state == HID_STATE_BUSY) {
        /*!< update the state  */
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
            /*!< updata the state */
            keyboard_state = HID_STATE_BUSY;
            /*!< write buffer */
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
            /*!< updata the state */
            exkey_state = HID_STATE_BUSY;
            /*!< write buffer */
            usbd_ep_write(EXKEY_IN_EP, data, len, NULL);
        }
    }
}

static void hid_custom_send_report(uint8_t *data, uint8_t len)
{
    if (usb_device_is_configured()) {
        if (custom_state == HID_STATE_IDLE) {
            /*!< updata the state */
            custom_state = HID_STATE_BUSY;
            /*!< write buffer */
            usbd_ep_write(HIDRAW_IN_EP, data, len, NULL);
        }
    }
}

void raw_hid_send(uint8_t *data, uint8_t length)
{
    if (length != HIDRAW_IN_SIZE) {
        return;
    }
    hid_custom_send_report(data, length);
}

#include "usb_main.h"

static usbd_class_t hid_class;
static usbd_interface_t hid_intf_1;
static usbd_interface_t hid_intf_2;
static usbd_interface_t hid_intf_3;
/*!< hid state ! Data can be sent only when state is idle  */
static uint8_t keyboard_state = HID_STATE_IDLE;
static uint8_t custom_state = HID_STATE_IDLE;
static uint8_t extra_state = HID_STATE_IDLE;

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
    //TODO: finish this
}

void usbd_hid_exkey_in_callback(uint8_t ep)
{
    /*!< endpoint call back */
    /*!< transfer successfully */
    if (extra_state == HID_STATE_BUSY) {
        /*!< update the state  */
        extra_state = HID_STATE_IDLE;
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

    usbd_endpoint_t rawhid_in_ep = {
        .ep_cb = usbd_hid_custom_in_callback,
        .ep_addr = HIDRAW_IN_EP
    };

    usbd_endpoint_t rawhid_out_ep = {
        .ep_cb = usbd_hid_custom_out_callback,
        .ep_addr = HIDRAW_OUT_EP
    };

    usbd_endpoint_t exkey_in_ep = {
        .ep_cb = usbd_hid_exkey_in_callback,
        .ep_addr = EXKEY_IN_EP
    };

    usbd_desc_register(hid_descriptor);
    /*!< add interface ! the first interface */
    usbd_hid_add_interface(&hid_class, &hid_intf_1);
    /*!< interface0 add endpoint ! the first endpoint */
    usbd_interface_add_endpoint(&hid_intf_1, &keyboard_in_ep);
    /*!< interface0 add endpoint ! the second endpoint */
    usbd_interface_add_endpoint(&hid_intf_1, &keyboard_out_ep);

    /*!< add interface the ! second interface */
    usbd_hid_add_interface(&hid_class, &hid_intf_2);
    /*!< interface1 add endpoint ! the first endpoint */
    usbd_interface_add_endpoint(&hid_intf_2, &rawhid_in_ep);
    /*!< interface1 add endpoint ! the second endpoint */
    usbd_interface_add_endpoint(&hid_intf_2, &rawhid_out_ep);

    /*!< register report descriptor interface 3 */
    usbd_hid_add_interface(&hid_class, &hid_intf_3);
    /*!< interface2 add endpoint ! the 3rd endpoint */
    usbd_interface_add_endpoint(&hid_intf_3, &exkey_in_ep);

    /*!< register report descriptor interface 0 */
    usbd_hid_report_descriptor_register(0, KeyboardReport, HID_KEYBOARD_REPORT_DESC_SIZE);
    /*!< register report descriptor interface 1 */
    usbd_hid_report_descriptor_register(1, RawReport, HID_RAWHID_REPORT_DESC_SIZE);
    /*!< register report descriptor interface 1 */
    usbd_hid_report_descriptor_register(2, ExtrkeyReport, HID_EXTRAKEY_REPORT_DESC_SIZE);
    usbd_initialize();
}

/**
  * @brief            device send report to host
  * @pre              none
  * @param[in]        ep endpoint address
  * @param[in]        data points to the data buffer waiting to be sent
  * @param[in]        len length of data to be sent
  * @retval           none
  */
void hid_keyboard_send_report(uint8_t ep, uint8_t *data, uint8_t len)
{
    if (usb_device_is_configured()) {
        if (keyboard_state == HID_STATE_IDLE) {
            /*!< updata the state */
            keyboard_state = HID_STATE_BUSY;
            /*!< write buffer */
            usbd_ep_write(ep, data, len, NULL);
        }
    }
}

/**
  * @brief            device send report to host
  * @pre              none
  * @param[in]        ep endpoint address
  * @param[in]        data points to the data buffer waiting to be sent
  * @param[in]        len length of data to be sent
  * @retval           none
  */
void hid_custom_send_report(uint8_t ep, uint8_t *data, uint8_t len)
{
    if (usb_device_is_configured()) {
        if (custom_state == HID_STATE_IDLE) {
            /*!< updata the state */
            custom_state = HID_STATE_BUSY;
            /*!< write buffer */
            usbd_ep_write(ep, data, len, NULL);
        }
    }
}

/**
  * @brief            hid custom test
  * @pre              none
  * @param[in]        none
  * @retval           none
  */
void hid_custom_test(void)
{
    /*!< keyboard test */
    uint8_t sendbuffer1[8] = { 0x00, 0x00, HID_KEY_A, 0x00, 0x00, 0x00, 0x00, 0x00 }; //A
    hid_keyboard_send_report(KBD_IN_EP, sendbuffer1, KBD_IN_EP_SIZE);
    /*!< delay 10ms */
    //HAL_Delay(10);
    /*!< send button up */
    sendbuffer1[2] = 0;
    hid_keyboard_send_report(KBD_IN_EP, sendbuffer1, KBD_IN_EP_SIZE);
    /*!< delay 1000ms the custom test */
    //HAL_Delay(1000);
    /*!< custom test */
    uint8_t sendbuffer2[64] = { 6 };
    hid_custom_send_report(HIDRAW_IN_EP, sendbuffer2, HIDRAW_IN_SIZE);
    //HAL_Delay(1000);
}

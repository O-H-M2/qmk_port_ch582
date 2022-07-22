#include "usb_main.h"

/* function ------------------------------------------------------------------*/
/**
  * @brief            hid custom init
  * @pre              none
  * @param[in]        none
  * @retval           none
  */
void hid_custom_keyboard_init(void)
{
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
    usbd_interface_add_endpoint(&hid_intf_2, &custom_in_ep);
    /*!< interface1 add endpoint ! the second endpoint */
    usbd_interface_add_endpoint(&hid_intf_2, &custom_out_ep);
    /*!< register report descriptor interface 0 */
    usbd_hid_report_descriptor_register(0, hid_keyboard_report_desc, HID_KEYBOARD_REPORT_DESC_SIZE);
    /*!< register report descriptor interface 1 */
    usbd_hid_report_descriptor_register(1, hid_custom_report_desc, HID_CUSTOM_REPORT_DESC_SIZE);

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

void init_usb_driver()
{
}
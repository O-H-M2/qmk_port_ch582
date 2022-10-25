#include "usb_main.h"
#include "usb_descriptors.h"
#if ESB_ENABLE == 2
#include "esb.h"
#endif

uint8_t keyboard_protocol = 1;
uint8_t keyboard_idle = 0;
/*!< hid state ! Data can be sent only when state is idle  */
static uint8_t keyboard_state = HID_STATE_IDLE;
static uint8_t custom_state = HID_STATE_IDLE;
static uint8_t exkey_state = HID_STATE_IDLE;

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t kbd_out_buffer[CONFIG_USB_ALIGN_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t custom_out_buffer[HIDRAW_OUT_EP_SIZE];

void usbd_hid_kbd_in_callback(uint8_t ep, uint32_t nbytes)
{
    if (keyboard_state == HID_STATE_BUSY) {
        keyboard_state = HID_STATE_IDLE;
    }
}

void usbd_hid_kbd_out_callback(uint8_t ep, uint32_t nbytes)
{
    usbd_ep_start_read(ep, kbd_out_buffer, KBD_OUT_EP_SIZE);
#if ESB_ENABLE == 2
    wireless_ringbuffer_write(KBD_OUT_EP_SIZE, REPORT_ID_KEYBOARD, kbd_out_buffer);
#else
    keyboard_led_state = kbd_out_buffer[0];
#endif
}

void usbd_hid_custom_in_callback(uint8_t ep, uint32_t nbytes)
{
    if (custom_state == HID_STATE_BUSY) {
        custom_state = HID_STATE_IDLE;
    }
}

void usbd_hid_custom_out_callback(uint8_t ep, uint32_t nbytes)
{
    usbd_ep_start_read(ep, custom_out_buffer, HIDRAW_OUT_EP_SIZE);
#if ESB_ENABLE == 2
    wireless_ringbuffer_write(HIDRAW_OUT_EP_SIZE, REPORT_ID_CUSTOM, custom_out_buffer);
#else
    extern void raw_hid_receive(uint8_t * data, uint8_t length);
    raw_hid_receive(custom_out_buffer, HIDRAW_OUT_EP_SIZE);
#endif
}

void usbd_hid_exkey_in_callback(uint8_t ep, uint32_t nbytes)
{
    if (exkey_state == HID_STATE_BUSY) {
        exkey_state = HID_STATE_IDLE;
    }
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

    usbd_add_interface(usbd_hid_alloc_intf(KeyboardReport, HID_KEYBOARD_REPORT_DESC_SIZE));
    usbd_add_endpoint(&keyboard_in_ep);
    usbd_add_endpoint(&keyboard_out_ep);

    usbd_add_interface(usbd_hid_alloc_intf(ExtrkeyReport, HID_EXTRAKEY_REPORT_DESC_SIZE));
    usbd_add_endpoint(&exkey_in_ep);

    usbd_add_interface(usbd_hid_alloc_intf(RawReport, HID_RAWHID_REPORT_DESC_SIZE));
    usbd_add_endpoint(&rawhid_in_ep);
    usbd_add_endpoint(&rawhid_out_ep);

    usbd_initialize();
}

void hid_bios_keyboard_send_report(uint8_t *data, uint8_t len)
{
    if (usb_device_is_configured()) {
        if (keyboard_state == HID_STATE_IDLE) {
            keyboard_state = HID_STATE_BUSY;
            if (len == KBD_IN_EP_SIZE) {
                usbd_ep_start_write(KBD_IN_EP, data, len);
            } else {
                uint8_t reconstruct[KBD_IN_EP_SIZE];

                memcpy(reconstruct, data, len);
                memset(reconstruct + len, 0x00, KBD_IN_EP_SIZE - len);
                usbd_ep_start_write(KBD_IN_EP, reconstruct, KBD_IN_EP_SIZE);
            }
        } else {
            hid_bios_keyboard_send_report(data, len);
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
            if (len == EXKEY_IN_EP_SIZE) {
                usbd_ep_start_write(EXKEY_IN_EP, data, len);
            } else {
                uint8_t reconstruct[EXKEY_IN_EP_SIZE];

                memcpy(reconstruct, data, len);
                memset(reconstruct + len, 0x00, EXKEY_IN_EP_SIZE - len);
                usbd_ep_start_write(EXKEY_IN_EP, reconstruct, EXKEY_IN_EP_SIZE);
            }
        } else {
            hid_exkey_send_report(data, len);
        }
    }
}

void hid_custom_send_report(uint8_t *data, uint8_t len)
{
    if (usb_device_is_configured()) {
        if (custom_state == HID_STATE_IDLE) {
            custom_state = HID_STATE_BUSY;
            if (len == HIDRAW_IN_SIZE) {
                usbd_ep_start_write(HIDRAW_IN_EP, data, len);
            } else {
                uint8_t reconstruct[HIDRAW_IN_SIZE];

                memcpy(reconstruct, data, len);
                memset(reconstruct + len, 0x00, HIDRAW_IN_SIZE - len);
                usbd_ep_start_write(HIDRAW_IN_EP, reconstruct, HIDRAW_IN_SIZE);
            }
        } else {
            hid_custom_send_report(data, len);
        }
    }
}

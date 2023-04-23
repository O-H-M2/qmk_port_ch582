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

#include "gpio.h"
#include "timer.h"
#include "usb_interface.h"
#include "usb_descriptors.h"
#include "usb_ch58x_usbfs_reg.h"
#include "protocol.h"
#if ESB_ENABLE == 2
#include "config.h"
#define memcmp(...) tmos_memcmp(__VA_ARGS__) ? 0 : 1
#define strlen      tmos_strlen
#define memset      tmos_memset
#define memcpy      tmos_memcpy
#endif

uint8_t keyboard_protocol = 1;
uint8_t keyboard_idle = 0;
static uint8_t *hid_descriptor = NULL;

static uint8_t keyboard_current_mode = KEYBOARD_MODE_NKRO;
static struct usbd_interface keyboard_interface;
#ifdef RGB_RAW_ENABLE
static struct usbd_interface rgbraw_interface;
#endif
static struct usbd_interface extrakey_interface;
#ifdef RAW_ENABLE
static struct usbd_interface qmkraw_interface;
#endif
/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t keyboard_state = HID_STATE_IDLE;
#ifdef RGB_RAW_ENABLE
static volatile uint8_t rgbraw_state = HID_STATE_IDLE;
#endif
static volatile uint8_t extrakey_state = HID_STATE_IDLE;
#ifdef RAW_ENABLE
static volatile uint8_t qmkraw_state = HID_STATE_IDLE;
#endif

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t kbd_out_buffer[CONFIG_USB_ALIGN_SIZE];
#ifdef RGB_RAW_ENABLE
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t rgbraw_out_buffer[RGBRAW_OUT_EP_SIZE];
#endif
#ifdef RAW_ENABLE
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t qmkraw_out_buffer[QMKRAW_OUT_EP_SIZE];
#endif

void usbd_hid_kbd_in_callback(uint8_t ep, uint32_t nbytes)
{
    keyboard_state = HID_STATE_IDLE;
}

void usbd_hid_kbd_out_callback(uint8_t ep, uint32_t nbytes)
{
    keyboard_leds_set(kbd_out_buffer[0]);
    usbd_ep_start_read(ep, kbd_out_buffer, KBD_OUT_EP_SIZE);
}

#ifdef RGB_RAW_ENABLE
void usbd_hid_rgb_raw_in_callback(uint8_t ep, uint32_t nbytes)
{
    rgbraw_state = HID_STATE_IDLE;
}

void usbd_hid_rgb_raw_out_callback(uint8_t ep, uint32_t nbytes)
{
    receive_rgb_raw(rgbraw_out_buffer, sizeof(rgbraw_out_buffer));
    usbd_ep_start_read(ep, rgbraw_out_buffer, sizeof(rgbraw_out_buffer));
}
#endif

void usbd_hid_exkey_in_callback(uint8_t ep, uint32_t nbytes)
{
    extrakey_state = HID_STATE_IDLE;
}

#ifdef RAW_ENABLE
void usbd_hid_qmk_raw_in_callback(uint8_t ep, uint32_t nbytes)
{
    qmkraw_state = HID_STATE_IDLE;
}

void usbd_hid_qmk_raw_out_callback(uint8_t ep, uint32_t nbytes)
{
    receive_qmk_raw(qmkraw_out_buffer, sizeof(qmkraw_out_buffer));
    usbd_ep_start_read(ep, qmkraw_out_buffer, sizeof(qmkraw_out_buffer));
}
#endif

static inline bool usb_remote_wakeup()
{
    if (!(R8_USB_MIS_ST & RB_UMS_SUSPEND)) {
        return true;
    }

    R16_PIN_ANALOG_IE &= ~RB_PIN_USB_DP_PU;
    R8_UDEV_CTRL |= RB_UD_LOW_SPEED;
    DelayMs(2);
    R8_UDEV_CTRL &= ~RB_UD_LOW_SPEED;
    R16_PIN_ANALOG_IE |= RB_PIN_USB_DP_PU;

    uint16_t timeout_timer = timer_read();

    while (R8_USB_MIS_ST & RB_UMS_SUSPEND) {
        if ((timer_elapsed(timeout_timer) > 2)) {
            return false;
        }
    }
    return true;
}

void usbd_configure_done_callback()
{
    usbd_ep_start_read(KBD_OUT_EP, kbd_out_buffer, KBD_OUT_EP_SIZE);
#ifdef RGB_RAW_ENABLE
    usbd_ep_start_read(RGBRAW_OUT_EP, rgbraw_out_buffer, sizeof(rgbraw_out_buffer));
#endif
#ifdef RAW_ENABLE
    usbd_ep_start_read(QMKRAW_OUT_EP, qmkraw_out_buffer, sizeof(qmkraw_out_buffer));
#endif
}

void usb_dc_low_level_init()
{
    DelayUs(100);
    PFIC_EnableIRQ(USB_IRQn);
}

void usb_dc_low_level_deinit()
{
    PFIC_DisableIRQ(USB_IRQn);
    R16_PIN_ANALOG_IE &= ~(RB_PIN_USB_IE | RB_PIN_USB_DP_PU);
    R32_USB_CONTROL = 0;
    R8_USB_CTRL |= RB_UC_RESET_SIE | RB_UC_CLR_ALL;
    DelayMs(10);
    R8_USB_CTRL &= ~(RB_UC_RESET_SIE | RB_UC_CLR_ALL);
    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF1 |= RB_SLP_CLK_USB;
        sys_safe_access_disable();
    } while (!(R8_SLP_CLK_OFF1 & RB_SLP_CLK_USB));
    setPinInputLow(B10);
    setPinInputLow(B11);
}

int usb_dc_deinit()
{
    usb_dc_low_level_deinit();
    keyboard_protocol = 1;
    keyboard_idle = 0;
    if (hid_descriptor != NULL) {
        free(hid_descriptor);
        hid_descriptor = NULL;
    }

    memset(&keyboard_interface, 0x00, sizeof(keyboard_interface));
#ifdef RGB_RAW_ENABLE
    memset(&rgbraw_interface, 0x00, sizeof(rgbraw_interface));
#endif
    memset(&extrakey_interface, 0x00, sizeof(extrakey_interface));
#ifdef RAW_ENABLE
    memset(&qmkraw_interface, 0x00, sizeof(qmkraw_interface));
#endif
    keyboard_state = HID_STATE_IDLE;
#ifdef RGB_RAW_ENABLE
    rgbraw_state = HID_STATE_IDLE;
#endif
    extrakey_state = HID_STATE_IDLE;
#ifdef RAW_ENABLE
    qmkraw_state = HID_STATE_IDLE;
#endif

    memset(kbd_out_buffer, 0x00, sizeof(kbd_out_buffer));
#ifdef RGB_RAW_ENABLE
    memset(rgbraw_out_buffer, 0x00, sizeof(rgbraw_out_buffer));
#endif
#ifdef RAW_ENABLE
    memset(qmkraw_out_buffer, 0x00, sizeof(qmkraw_out_buffer));
#endif

    return 0;
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

#ifdef RGB_RAW_ENABLE
    struct usbd_endpoint rgbraw_in_ep = {
        .ep_cb = usbd_hid_rgb_raw_in_callback,
        .ep_addr = RGBRAW_IN_EP
    };

    struct usbd_endpoint rgbraw_out_ep = {
        .ep_cb = usbd_hid_rgb_raw_out_callback,
        .ep_addr = RGBRAW_OUT_EP
    };
#endif

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

    // build descriptor according to the keyboard's name and type
    uint8_t hid_descriptor_scratch_2[USB_SIZEOF_INTERFACE_DESC + 0x09 + USB_SIZEOF_ENDPOINT_DESC * 2];

#ifdef NKRO_ENABLE
    if (keyboard_current_mode == KEYBOARD_MODE_NKRO) {
        // nkro mode
        memcpy(hid_descriptor_scratch_2, hid_descriptor_scratch_2_nkro, sizeof(hid_descriptor_scratch_2));
    } else
#endif
    {
        // bios mode
        memcpy(hid_descriptor_scratch_2, hid_descriptor_scratch_2_bios, sizeof(hid_descriptor_scratch_2));
    }

#if ESB_ENABLE == 2
    uint8_t keyboard_name[] = MACRO2STR(PRODUCT) " dongle";
#else
    uint8_t keyboard_name[] = MACRO2STR(PRODUCT);
#endif
    uint8_t hid_descriptor_scratch_4[((sizeof(keyboard_name) - 1) + 1) * 2] = {
        ///////////////////////////////////////
        /// string2 descriptor
        ///////////////////////////////////////
        ((sizeof(keyboard_name) - 1) + 1) * 2, /* bLength */
        USB_DESCRIPTOR_TYPE_STRING,            /* bDescriptorType */
    };

    for (uint8_t i = 0; i < (sizeof(keyboard_name) - 1); i++) {
        hid_descriptor_scratch_4[2 + i * 2] = keyboard_name[i];
        hid_descriptor_scratch_4[2 + i * 2 + 1] = 0x00;
    }

    if (hid_descriptor != NULL) {
        free(hid_descriptor);
    }
    hid_descriptor = (uint8_t *)malloc(sizeof(hid_descriptor_scratch_1) + sizeof(hid_descriptor_scratch_2) +
                                       sizeof(hid_descriptor_scratch_3) + sizeof(hid_descriptor_scratch_4) +
                                       sizeof(hid_descriptor_scratch_5));
    memcpy(hid_descriptor, hid_descriptor_scratch_1, sizeof(hid_descriptor_scratch_1));
    memcpy(hid_descriptor + sizeof(hid_descriptor_scratch_1),
           hid_descriptor_scratch_2, sizeof(hid_descriptor_scratch_2));
    memcpy(hid_descriptor + sizeof(hid_descriptor_scratch_1) + sizeof(hid_descriptor_scratch_2),
           hid_descriptor_scratch_3, sizeof(hid_descriptor_scratch_3));
    memcpy(hid_descriptor + sizeof(hid_descriptor_scratch_1) + sizeof(hid_descriptor_scratch_2) +
               sizeof(hid_descriptor_scratch_3),
           hid_descriptor_scratch_4, sizeof(hid_descriptor_scratch_4));
    memcpy(hid_descriptor + sizeof(hid_descriptor_scratch_1) + sizeof(hid_descriptor_scratch_2) +
               sizeof(hid_descriptor_scratch_3) + sizeof(hid_descriptor_scratch_4),
           hid_descriptor_scratch_5, sizeof(hid_descriptor_scratch_5));

    usbd_desc_register(hid_descriptor);

#ifdef NKRO_ENABLE
    if (keyboard_current_mode == KEYBOARD_MODE_NKRO) {
        usbd_add_interface(usbd_hid_init_intf(&keyboard_interface, NkroReport, sizeof(NkroReport)));
    } else
#endif
    {
        usbd_add_interface(usbd_hid_init_intf(&keyboard_interface, KeyboardReport, sizeof(KeyboardReport)));
    }
    usbd_add_endpoint(&keyboard_in_ep);
    usbd_add_endpoint(&keyboard_out_ep);

#ifdef RGB_RAW_ENABLE
    usbd_add_interface(usbd_hid_init_intf(&rgbraw_interface, RGBRawReport, HID_RGBRAW_REPORT_DESC_SIZE));
    usbd_add_endpoint(&rgbraw_in_ep);
    usbd_add_endpoint(&rgbraw_out_ep);
#endif

    usbd_add_interface(usbd_hid_init_intf(&extrakey_interface, ExtrakeyReport, HID_EXTRAKEY_REPORT_DESC_SIZE));
    usbd_add_endpoint(&exkey_in_ep);

#ifdef RAW_ENABLE
    usbd_add_interface(usbd_hid_init_intf(&qmkraw_interface, QMKRawReport, HID_QMKRAW_REPORT_DESC_SIZE));
    usbd_add_endpoint(&qmkraw_in_ep);
    usbd_add_endpoint(&qmkraw_out_ep);
#endif

    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_USB;
        sys_safe_access_disable();
    } while (R8_SLP_CLK_OFF1 & RB_SLP_CLK_USB);

    setPinInput(B10);
    setPinInput(B11);

    usbd_initialize();
}

void hid_keyboard_send_report(uint8_t mode, uint8_t *data, uint8_t len)
{
    if (mode != keyboard_current_mode) {
        keyboard_current_mode = mode;
        usbd_deinitialize();
        init_usb_driver();
    }

    if (!usb_remote_wakeup()) {
        return;
    }

    while (keyboard_state == HID_STATE_BUSY) {
        __nop();
    }

    int ret = usbd_ep_start_write(KBD_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    keyboard_state = HID_STATE_BUSY;
}

#ifdef RGB_RAW_ENABLE
void hid_rgb_raw_send_report(uint8_t *data, uint8_t len)
{
    if (!usb_remote_wakeup()) {
        return;
    }

    while (rgbraw_state == HID_STATE_BUSY) {
        __nop();
    }

    int ret = usbd_ep_start_write(RGBRAW_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    rgbraw_state = HID_STATE_BUSY;
}
#endif

inline void hid_exkey_send_report(uint8_t *data, uint8_t len)
{
    if (!usb_remote_wakeup()) {
        return;
    }

    while (extrakey_state == HID_STATE_BUSY) {
        __nop();
    }

    int ret = usbd_ep_start_write(EXKEY_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    extrakey_state = HID_STATE_BUSY;
}

#ifdef RAW_ENABLE
void hid_qmk_raw_send_report(uint8_t *data, uint8_t len)
{
    if (!usb_remote_wakeup()) {
        return;
    }

    while (qmkraw_state == HID_STATE_BUSY) {
        __nop();
    }

    int ret = usbd_ep_start_write(QMKRAW_IN_EP, data, len);

    if (ret < 0) {
        return;
    }
    qmkraw_state = HID_STATE_BUSY;
}
#endif

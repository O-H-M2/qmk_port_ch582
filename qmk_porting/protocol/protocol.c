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

#include "host.h"
#include "usb_device_state.h"
#include "keycode_config.h"
#include "platform_deps.h"

uint8_t keyboard_led_state;

__attribute__((weak)) void raw_hid_receive(uint8_t *data, uint8_t length)
{
    // placeholder in case VIA is disabled
}

__attribute__((weak)) void ezraw_hid_receive(uint8_t *data, uint8_t length)
{
    // placeholder in case openRGB or others is disabled
}

uint8_t keyboard_leds()
{
    return keyboard_led_state;
}

host_driver_t ch582_driver = {
    .keyboard_leds = keyboard_leds,
};
void (*ch582_driver_raw_hid_send)(uint8_t *, uint8_t);

void raw_hid_send(uint8_t *data, uint8_t length)
{
    ch582_driver_raw_hid_send(data, length);
}

void protocol_setup()
{
#ifdef USB_ENABLE
    if (kbd_protocol_type == kbd_protocol_usb) {
        usb_device_state_init();
    }
#endif
}

void protocol_pre_init()
{
#ifdef USB_ENABLE
    if (kbd_protocol_type == kbd_protocol_usb) {
        protocol_pre_init_usb();
    }
#endif
#ifdef BLE_ENABLE
    if (kbd_protocol_type == kbd_protocol_ble) {
        protocol_pre_init_ble();
    }
#endif
#ifdef ESB_ENABLE
    if (kbd_protocol_type == kbd_protocol_esb) {
        protocol_pre_init_esb();
    }
#endif
}

void protocol_post_init()
{
#ifdef USB_ENABLE
    if (kbd_protocol_type == kbd_protocol_usb) {
        ch582_driver.send_keyboard = send_keyboard_usb;
        ch582_driver.send_mouse = send_mouse_usb;
        ch582_driver.send_extra = send_extra_usb;
        ch582_driver_raw_hid_send = raw_hid_send_usb;
    }
#endif
#ifdef BLE_ENABLE
    if (kbd_protocol_type == kbd_protocol_ble) {
        ch582_driver.send_keyboard = send_keyboard_ble;
        ch582_driver.send_mouse = send_mouse_ble;
        ch582_driver.send_extra = send_extra_ble;
        ch582_driver_raw_hid_send = raw_hid_send_ble;
    }
#endif
#ifdef ESB_ENABLE
    if (kbd_protocol_type == kbd_protocol_esb) {
        ch582_driver.send_keyboard = send_keyboard_esb;
        ch582_driver.send_mouse = send_mouse_esb;
        ch582_driver.send_extra = send_extra_esb;
        ch582_driver_raw_hid_send = raw_hid_send_esb;
    }
#endif
    host_set_driver(&ch582_driver);
}

void protocol_toggle(uint8_t status)
{
    if (status) {
        host_set_driver(&ch582_driver);
    } else {
        host_set_driver(NULL);
    }
}

// void protocol_pre_task()
// {
// }

// void protocol_post_task()
// {
// }

void protocol_task()
{
#ifdef USB_ENABLE
    if (kbd_protocol_type == kbd_protocol_usb) {
        protocol_task_usb();
    }
#endif
#ifdef BLE_ENABLE
    if (kbd_protocol_type == kbd_protocol_ble) {
        protocol_task_ble();
    }
#endif
#ifdef ESB_ENABLE
    if (kbd_protocol_type == kbd_protocol_esb) {
        protocol_task_esb();
    }
#endif
}

void keyboard_post_init_user()
{
#ifdef PLF_DEBUG
    print("Set log output for QMK.\n");
#endif
}

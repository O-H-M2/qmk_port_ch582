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

#include "protocol.h"
#include "usb_device_state.h"
#include "keycode_config.h"
#include "platform_deps.h"
#include "usb_interface.h"
#include "quantum.h"

static uint8_t keyboard_led_state;

// make it overridable for dongle
__attribute__((weak)) void keyboard_leds_set(uint8_t state)
{
    keyboard_led_state = state;
}

uint8_t keyboard_leds()
{
    return keyboard_led_state;
}

void keyboard_check_protocol_mode()
{
#ifdef NKRO_ENABLE
#ifdef FORCE_NKRO
    static uint8_t last_mode = KEYBOARD_MODE_NKRO;
#else
    static uint8_t last_mode = KEYBOARD_MODE_BIOS;
#endif

    if (last_mode != keymap_config.nkro) {
        // send a keyboard report to trigger mode change
        report_keyboard_t dummy_report = {};

        host_keyboard_send(&dummy_report);
        last_mode = keymap_config.nkro;
    }
#endif
}

static ch582_interface_t ch582_interface = {};
static ch582_interface_t *last_interface_used = NULL;

void ch582_set_protocol_interface(const ch582_interface_t *interface)
{
    if (interface != NULL) {
        last_interface_used = (ch582_interface_t *)interface;
        memcpy(&ch582_interface, interface, sizeof(ch582_interface_t));
    }
}

ch582_interface_t *ch582_get_protocol_interface()
{
    if (ch582_interface.ch582_platform_initialize) {
        return &ch582_interface;
    } else {
        return NULL;
    }
}

void ch582_toggle_qmk_protocol(bool status)
{
    if (status) {
        if (last_interface_used) {
#ifdef RAW_ENABLE
            ch582_interface.send_qmk_raw = last_interface_used->send_qmk_raw;
            ch582_interface.receive_qmk_raw = last_interface_used->receive_qmk_raw;
#endif
#ifdef RGB_RAW_ENABLE
            ch582_interface.send_rgb_raw = last_interface_used->send_rgb_raw;
            ch582_interface.receive_rgb_raw = last_interface_used->receive_rgb_raw;
#endif
        }
        host_set_driver(&ch582_interface.ch582_common_driver);
    } else {
        host_set_driver(NULL);
#ifdef RAW_ENABLE
        ch582_interface.send_qmk_raw = NULL;
        ch582_interface.receive_qmk_raw = NULL;
#endif
#ifdef RGB_RAW_ENABLE
        ch582_interface.send_rgb_raw = NULL;
        ch582_interface.receive_rgb_raw = NULL;
#endif
    }
}

#ifdef RAW_ENABLE
void via_custom_value_command_kb(uint8_t *data, uint8_t length)
{
    // data = [ command_id, channel_id, value_id, value_data ]
    uint8_t *command_id = &(data[0]);
    uint8_t *channel_id = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if (*channel_id == id_custom_channel) {
#if defined ESB_ENABLE && ESB_ENABLE == 2
        if (*command_id == id_custom_set_value &&
            *(uint16_t *)value_id_and_data == 0xFFCA) {
            bootloader_jump();
        }
#endif
        if (*command_id == id_custom_get_value) {
            *(uint16_t *)value_id_and_data = 0xFECA;
        }
    }
}

void raw_hid_send(uint8_t *data, uint8_t length)
{
    if (ch582_interface.send_qmk_raw) {
        ch582_interface.send_qmk_raw(data, length);
    }
}

void receive_qmk_raw(uint8_t *data, uint8_t length)
{
    if (ch582_interface.receive_qmk_raw) {
        ch582_interface.receive_qmk_raw(data, length);
    }
}
#endif

#ifdef RGB_RAW_ENABLE
void rgb_raw_hid_send(uint8_t *data, uint8_t length)
{
    if (ch582_interface.send_rgb_raw) {
        ch582_interface.send_rgb_raw(data, length);
    }
}

void receive_rgb_raw(uint8_t *data, uint8_t length)
{
    if (ch582_interface.receive_rgb_raw) {
        ch582_interface.receive_rgb_raw(data, length);
    }
}
#endif

void protocol_setup()
{
    if (ch582_interface.ch582_protocol_setup) {
        ch582_interface.ch582_protocol_setup();
    }
}

void protocol_pre_init()
{
    // detach app with protocol interface before initialization is done
    ch582_toggle_qmk_protocol(false);
    if (ch582_interface.ch582_protocol_init) {
        ch582_interface.ch582_protocol_init();
    }
}

void protocol_post_init()
{
    ch582_toggle_qmk_protocol(true);
}

__HIGH_CODE void protocol_task()
{
    for (;;) {
        if (ch582_interface.ch582_protocol_task) {
            ch582_interface.ch582_protocol_task();
        }
    }
}

#if defined ESB_ENABLE && ESB_ENABLE == 2
void protocol_init()
{
    protocol_pre_init();

    // skip keyboard_init()

    protocol_post_init();
}
#endif

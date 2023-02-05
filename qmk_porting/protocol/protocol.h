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

#pragma once

#include <stdint.h>
#include "host.h"

#ifdef RAW_ENABLE
typedef void (*qmk_raw_handler_t)(uint8_t *, uint8_t);
#endif
#ifdef RGB_RAW_ENABLE
typedef void (*rgb_raw_handler_t)(uint8_t *, uint8_t);
#endif
typedef void (*ch582_driver_t)();

typedef struct _ch582_interface_t {
    host_driver_t ch582_common_driver;
#ifdef RAW_ENABLE
    qmk_raw_handler_t send_qmk_raw;
    qmk_raw_handler_t receive_qmk_raw;
#endif
#ifdef RGB_RAW_ENABLE
    rgb_raw_handler_t send_rgb_raw;
    rgb_raw_handler_t receive_rgb_raw;
#endif
    ch582_driver_t ch582_platform_initialize;
    ch582_driver_t ch582_protocol_setup;
    ch582_driver_t ch582_protocol_init; // hardware interface initialization
    ch582_driver_t ch582_protocol_task;
    ch582_driver_t ch582_platform_reboot;
} ch582_interface_t;

void keyboard_leds_set(uint8_t state);
uint8_t keyboard_leds();

void ch582_set_protocol_interface(const ch582_interface_t *interface);
ch582_interface_t *ch582_get_protocol_interface();
void ch582_toggle_qmk_protocol(bool status);

#ifdef RAW_ENABLE
void raw_hid_send(uint8_t *data, uint8_t length);
void receive_qmk_raw(uint8_t *data, uint8_t length);
#endif
#ifdef RGB_RAW_ENABLE
void rgb_raw_hid_send(uint8_t *data, uint8_t length);
void receive_rgb_raw(uint8_t *data, uint8_t length);
#endif

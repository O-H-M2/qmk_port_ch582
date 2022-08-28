/* Copyright 2021 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "CH58x_common.h"
#include "HAL.h"
#ifdef BLE_ENABLE
#include "ble.h"
#endif

enum {
    kbd_protocol_usb = 1,
#ifdef BLE_ENABLE
    kbd_protocol_ble,
#endif
    kbd_protocol_2g4,
};

extern volatile uint8_t kbd_protocol_type;

#ifdef BLE_ENABLE
void platform_setup_ble();
#endif
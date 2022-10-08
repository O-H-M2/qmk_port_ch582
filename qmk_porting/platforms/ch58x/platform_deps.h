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

#include "HAL.h"
#include "print.h"
#include "eeprom_reserved.h"
#ifdef BLE_ENABLE
#include "ble.h"
#endif
#ifdef ESB_ENABLE
#include "esb.h"
#endif
#if !defined BLE_ENABLE && !defined ESB_ENABLE
#include "rgb_led.h"
#endif

void platform_setup();

#ifdef USB_ENABLE
void platform_setup_usb();
#endif
#ifdef BLE_ENABLE
void platform_setup_ble();
#endif
#ifdef ESB_ENABLE
void platform_setup_esb();
#endif

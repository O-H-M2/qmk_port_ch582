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

#include "print.h"
#include "bootloader.h"
#include "rgb_led.h"
#if __BUILDING_APP__
#ifdef USB_ENABLE
#include "protocol_usb.h"
#endif
#ifdef BLE_ENABLE
#include "protocol_ble.h"
#endif
#ifdef ESB_ENABLE
#include "protocol_esb.h"
#endif
#endif

void platform_setup();

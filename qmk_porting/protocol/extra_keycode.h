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

#include "quantum_keycodes.h"
#include "action.h"
#include "quantum.h"
#include "battery_measure.h"
// #include "via.h"

enum {
#ifdef VIA_ENABLE
    CUSTOM_START = USER00 - 1,
#else
    CUSTOM_START = SAFE_RANGE,
#endif
#ifdef USB_ENABLE
    USB_MODE,
#endif
#ifdef BLE_ENABLE
    BLE_SLOT0,
    BLE_SLOT1,
    BLE_SLOT2,
    BLE_SLOT3,
    BLE_CLEAR_SLOT0,
    BLE_CLEAR_SLOT1,
    BLE_CLEAR_SLOT2,
    BLE_CLEAR_SLOT3,
    BLE_ALL_CLEAR,
#endif
#ifdef ESB_ENABLE
    ESB_MODE,
#endif
#if defined BLE_ENABLE || (defined ESB_ENABLE && ESB_ENABLE == 1)
    BATTERY_INDICATOR,
#endif
    CUSTOM_MAX,
};

_Static_assert(CUSTOM_MAX <= USER15 + 1, "");

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

#include "extra_keycode.h"
#include "quantum.h"

#ifdef BLE_ENABLE

bool process_record_ble_keycode(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed) {
        switch (keycode) {
            case USB_MODE:
                return false;
            case BLE_SLOT0:
            case BLE_SLOT1:
            case BLE_SLOT2:
            case BLE_SLOT3:
            case BLE_SLOT4:
            case BLE_SLOT5:
            case BLE_SLOT6:
            case BLE_SLOT7:
                if (keycode <= BLE_SLOT0 + BLE_SLOT_NUM - 1) {
                    hogp_slot_switch(keycode - BLE_SLOT0);
                }
                return false;
            case BLE_ALL_CLEAR:
                hogp_slot_clear(UINT8_MAX);
                return false;
            case ESB_MODE:
                return false;
            default:
                break;
        }
    }
    return process_record_user(keycode, record);
}

#endif
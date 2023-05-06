/*
Copyright 2022 Huckies <https://github.com/Huckies>
Copyright 2022 OctopusZ <https://github.com/OctopusZ>

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

#if defined BLE_ENABLE || (defined ESB_ENABLE && (ESB_ENABLE == 1 || ESB_ENABLE == 2))
bool wireless_process_record(uint16_t keycode, keyrecord_t *record);
#endif

bool process_record_kb(uint16_t keycode, keyrecord_t *record)
{
    bool ret = process_record_user(keycode, record);

    if (!ret) {
        return false;
    }

    switch (keycode) {
#ifdef USB_ENABLE
        case USB_MODE:
            if (record->event.pressed) {
                if (kbd_protocol_type != kbd_protocol_usb) {
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_USB);
                    soft_reset_keyboard();
                }
            }
            return false;
#endif
#if defined BLE_ENABLE || (defined ESB_ENABLE && (ESB_ENABLE == 1 || ESB_ENABLE == 2))
#ifdef BLE_ENABLE
        case BLE_SLOT0 ...(BLE_SLOT0 + BLE_SLOT_NUM - 1):
        case BLE_ALL_CLEAR:
#endif
#ifdef ESB_ENABLE
        case ESB_MODE:
#endif
#ifdef BATTERY_MEASURE_PIN
        case BATTERY_INDICATOR:
#endif
            wireless_process_record(keycode, record);
#endif
        default:
            break;
    }

    return true;
}

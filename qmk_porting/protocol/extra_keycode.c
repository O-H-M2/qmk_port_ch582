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
#include "quantum.h"

#ifdef BLE_ENABLE
extern bool process_ble_passcode_kb(uint16_t keycode, keyrecord_t *record);
#endif

bool process_record_kb(uint16_t keycode, keyrecord_t *record)
{
#ifdef BLE_ENABLE
    if (kbd_protocol_type == kbd_protocol_ble) {
        bool ret = process_ble_passcode_kb(keycode, record);

        if (!ret) {
            return false;
        }
    }
#endif

    bool ret = process_record_user(keycode, record);

    if (!ret) {
        return false;
    }

    if (record->event.pressed) {
        switch (keycode) {
#ifdef USB_ENABLE
            case USB_MODE:
                if (kbd_protocol_type != kbd_protocol_usb) {
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_USB);
                    soft_reset_keyboard();
                }
                return false;
#endif
#ifdef BLE_ENABLE
            case BLE_SLOT0 ... BLE_SLOT3:
                hogp_slot_switch(keycode - BLE_SLOT0);
                if (kbd_protocol_type != kbd_protocol_ble) {
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_BLE);
                    soft_reset_keyboard();
                }
                return false;
            case BLE_CLEAR_SLOT0 ... BLE_CLEAR_SLOT3:
                if (kbd_protocol_type == kbd_protocol_ble) {
                    hogp_slot_clear(keycode - BLE_CLEAR_SLOT0);
                }
                return false;
            case BLE_ALL_CLEAR:
                if (kbd_protocol_type == kbd_protocol_ble) {
                    hogp_slot_clear(UINT8_MAX);
                }
                return false;
#endif
#ifdef ESB_ENABLE
            case ESB_MODE:
                if (kbd_protocol_type != kbd_protocol_esb) {
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_ESB);
                    soft_reset_keyboard();
                }
                return false;
#endif
#if defined BLE_ENABLE || (defined ESB_ENABLE && ESB_ENABLE == 1)
            case BATTERY_INDICATOR:
#ifdef BLE_ENABLE
                if (kbd_protocol_type == kbd_protocol_ble) {
                    battery_indicator_toggle(true);
                }
#endif
#ifdef ESB_ENABLE
                if (kbd_protocol_type == kbd_protocol_esb) {
                    battery_indicator_toggle(true);
                }
#endif
                return false;
#endif
            default:
                break;
        }
    }

    return true;
}

void post_process_record_kb(uint16_t keycode, keyrecord_t *record)
{
#if (defined(HAL_SLEEP)) && (HAL_SLEEP == TRUE)
    lowpower_update_last_keypress();
#endif
    post_process_record_user(keycode, record);
}

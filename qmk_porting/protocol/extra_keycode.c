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

extern bool process_ble_passcode_kb(uint16_t keycode, keyrecord_t *record);

bool process_ble_keycode_kb(uint16_t keycode, keyrecord_t *record)
{
    if (record->event.pressed) {
        switch (keycode) {
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
            default:
                break;
        }
    }
}

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

    if (record->event.pressed) {
        switch (keycode) {
            case USB_MODE:
#ifdef USB_ENABLE
                if (kbd_protocol_type != kbd_protocol_usb) {
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_USB);
                    soft_reset_keyboard();
                }
#endif
                return false;
            case BLE_MODE:
#ifdef BLE_ENABLE
                if (kbd_protocol_type != kbd_protocol_ble) {
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_BLE);
                    soft_reset_keyboard();
                }
#endif
                return false;
            case ESB_MODE:
#ifdef ESB_ENABLE
                if (kbd_protocol_type != kbd_protocol_esb) {
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_ESB);
                    soft_reset_keyboard();
                }
#endif
                return false;
            default:
                break;
        }
    }

#ifdef BLE_ENABLE
    if (kbd_protocol_type == kbd_protocol_ble) {
        bool ret = process_ble_keycode_kb(keycode, record);

        if (!ret) {
            return false;
        }
    }
#endif

    return process_record_user(keycode, record);
}

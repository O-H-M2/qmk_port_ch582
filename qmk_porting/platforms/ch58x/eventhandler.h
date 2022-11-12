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

#include "bootloader.h"

#ifdef USB_ENABLE
#include "eventhandler_usb.h"
_Static_assert(BLE_EVENT_START_INDEX > USB_EVENT_MAX, "");
#endif
#ifdef BLE_ENABLE
#include "eventhandler_ble.h"
_Static_assert(ESB_EVENT_START_INDEX > BLE_EVENT_MAX, "");
#endif
#ifdef ESB_ENABLE
#include "eventhandler_esb.h"
#endif

extern volatile uint8_t kbd_protocol_type;

__attribute__((always_inline)) inline void event_propagate(uint8_t event, void *param)
{
    if (event == PLATFORM_EVENT_MODE_SELECT) {
        uint8_t mode = bootloader_boot_mode_get();

        if (mode == BOOTLOADER_BOOT_MODE_IAP) {
#ifdef USB_ENABLE
            PRINT("Successfully booted after IAP, default to USB.\n");
            mode = BOOTLOADER_BOOT_MODE_USB;
            bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_USB);
#elif defined BLE_ENABLE
            PRINT("Successfully booted after IAP, default to BLE.\n");
            mode = BOOTLOADER_BOOT_MODE_BLE;
            bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_BLE);
#elif defined ESB_ENABLE
            PRINT("Successfully booted after IAP, default to ESB.\n");
            mode = BOOTLOADER_BOOT_MODE_ESB;
            bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_ESB);
#endif
        }

        //! TODO: for test only!
        //mode = BOOTLOADER_BOOT_MODE_ESB;

        switch (mode) {
#ifdef USB_ENABLE
            case BOOTLOADER_BOOT_MODE_USB:
                // cable mode
                kbd_protocol_type = kbd_protocol_usb;
                break;
#endif
#ifdef BLE_ENABLE
            case BOOTLOADER_BOOT_MODE_BLE:
                // bluetooth mode
                kbd_protocol_type = kbd_protocol_ble;
                break;
#endif
#ifdef ESB_ENABLE
            case BOOTLOADER_BOOT_MODE_ESB:
                // 2.4g mode
                kbd_protocol_type = kbd_protocol_esb;
                break;
#endif
            default:
                PRINT("Invalid mode record detected, ");
                if (mode == BOOTLOADER_BOOT_MODE_IAP) {
                    PRINT("will reside in IAP.\n");
                } else {
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_IAP);
                    mode = bootloader_boot_mode_get();
                    PRINT("set to IAP... %s\n", mode == BOOTLOADER_BOOT_MODE_IAP ? "done" : "fail");
                }
                PRINT("Reboot execute.\n");
#ifdef PLF_DEBUG
                while ((R8_UART1_LSR & RB_LSR_TX_ALL_EMP) == 0) {
                    __nop();
                }
#endif
                SYS_ResetExecute();
        }
        return;
    }

#ifdef USB_ENABLE
    if (kbd_protocol_type == kbd_protocol_usb) {
        event_handler_usb(event, param);
    }
#endif
#ifdef BLE_ENABLE
    if (kbd_protocol_type == kbd_protocol_ble) {
        event_handler_ble(event, param);
    }
#endif
#ifdef ESB_ENABLE
    if (kbd_protocol_type == kbd_protocol_esb) {
        event_handler_esb(event, param);
    }
#endif
}

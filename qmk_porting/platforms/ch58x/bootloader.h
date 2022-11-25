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

enum {
    BOOTLOADER_BOOT_MODE_IAP = 0x05,
    BOOTLOADER_BOOT_MODE_USB = 0x0A,
    BOOTLOADER_BOOT_MODE_BLE = 0x50,
    BOOTLOADER_BOOT_MODE_ESB = 0xA0,
};

extern volatile uint8_t kbd_protocol_type;

void bootloader_boot_mode_set(uint8_t mode);
uint8_t bootloader_boot_mode_get();
void bootloader_jump();
void mcu_reset();

__attribute__((always_inline)) inline uint8_t bootloader_set_to_default_mode(const char *reason)
{
    PRINT("%s, ", reason);
#ifdef USB_ENABLE
    PRINT("default to USB.\n");
    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_USB);
    return BOOTLOADER_BOOT_MODE_USB;
#elif defined BLE_ENABLE
    PRINT("default to BLE slot 0.\n");
    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_BLE);
    return BOOTLOADER_BOOT_MODE_BLE;
#else
    PRINT("default to ESB.\n");
    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_ESB);
    return BOOTLOADER_BOOT_MODE_ESB;
#endif
}

__attribute__((always_inline)) inline void bootloader_select_boot_mode()
{
    uint8_t mode = bootloader_boot_mode_get();

    if (mode == BOOTLOADER_BOOT_MODE_IAP) {
        mode = bootloader_set_to_default_mode("Successfully booted from IAP");
    }

    //! TODO: for test only!
    // mode = BOOTLOADER_BOOT_MODE_BLE;

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
            WAIT_FOR_DBG;
            SYS_ResetExecute();
    }
}

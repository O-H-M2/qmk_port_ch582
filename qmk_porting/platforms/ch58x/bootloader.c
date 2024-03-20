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

#include "eeprom_driver.h"
#include "eeconfig.h"
#include "platform_deps.h"
#include "gpio.h"

bool bootmagic_allow_jump()
{
#ifdef POWER_DETECT_PIN
    if (!gpio_read_pin(POWER_DETECT_PIN)) {
        // cable removed
        return false;
    }
#endif
    return true;
}

void bootmagic_reset_eeprom(void)
{
    if (!bootmagic_allow_jump()) {
        return;
    }

    eeconfig_disable();
}

void bootloader_boot_mode_set(uint8_t mode)
{
    if (mode != BOOTLOADER_BOOT_MODE_IAP && mode != BOOTLOADER_BOOT_MODE_USB &&
        mode != BOOTLOADER_BOOT_MODE_BLE && mode != BOOTLOADER_BOOT_MODE_ESB) {
        PRINT("Invalid mode select, will ignore.\n");
        return;
    }
    if (0
#ifndef USB_ENABLE
        || (mode == BOOTLOADER_BOOT_MODE_USB)
#endif
#ifndef BLE_ENABLE
        || (mode == BOOTLOADER_BOOT_MODE_BLE)
#endif
#ifndef ESB_ENABLE
        || (mode == BOOTLOADER_BOOT_MODE_ESB)
#endif
    ) {
        PRINT("Mode %d is not enabled, will ignore.\n", mode);
        return;
    }

    uint8_t buffer[EEPROM_PAGE_SIZE], ret;

    do {
        ret = EEPROM_READ(QMK_EEPROM_RESERVED_START_POSITION, buffer, sizeof(buffer));
    } while (ret);
    buffer[0] = mode;
    do {
        ret = EEPROM_ERASE(QMK_EEPROM_RESERVED_START_POSITION, EEPROM_PAGE_SIZE) ||
              EEPROM_WRITE(QMK_EEPROM_RESERVED_START_POSITION, buffer, EEPROM_PAGE_SIZE);
    } while (ret);
}

uint8_t bootloader_boot_mode_get()
{
    uint8_t buffer, ret;

    do {
        ret = EEPROM_READ(QMK_EEPROM_RESERVED_START_POSITION, &buffer, sizeof(buffer));
    } while (ret);
    return buffer;
}

#if __BUILDING_APP__
void bootloader_select_boot_mode()
{
    uint8_t mode = bootloader_boot_mode_get();

    if (mode == BOOTLOADER_BOOT_MODE_IAP) {
        PRINT("Fatal: Boot mode tampering detected!\n");
        WAIT_FOR_DBG;
        __builtin_trap();
    } else
#if !defined ESB_ENABLE || ESB_ENABLE == 1
        // usb模式只因io状态进入
        if (mode == BOOTLOADER_BOOT_MODE_USB) {
            PRINT("unexpected usb mode .\n");
#ifdef ESB_ENABLE
            bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_ESB);
            mode = BOOTLOADER_BOOT_MODE_ESB;
            PRINT("Default to ESB.\n");
#elif defined BLE_ENABLE
            bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_BLE);
            mode = BOOTLOADER_BOOT_MODE_BLE;
            PRINT("Default to BLE.\n");
#endif
        }
#else
        if (mode != BOOTLOADER_BOOT_MODE_ESB) {
        PRINT("Dongle has fixed mode, will correct.\n");
        mode = BOOTLOADER_BOOT_MODE_ESB;
    }
#endif
#if ESB_ENABLE != 2
#ifdef MODE_DETECT_PIN
    if (gpio_read_pin(MODE_DETECT_PIN)) { // 如果为USB模式
        if (mode != BOOTLOADER_BOOT_MODE_USB) {
            bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_USB);//为kb层级可以正常读取设备所处BOOT状态
        }
        mode = BOOTLOADER_BOOT_MODE_USB;
        PRINT("USB mode triger by MODE_DETECT_PIN\n");
    }
#else
#warning "MODE_DETECT_PIN undefined."
#endif
#endif
    //! TODO: for test only!
    // mode = BOOTLOADER_BOOT_MODE_BLE;
    // mode = BOOTLOADER_BOOT_MODE_ESB;

    switch (mode) {
#ifdef USB_ENABLE
        case BOOTLOADER_BOOT_MODE_USB:
            // cable mode
            kbd_protocol_type = kbd_protocol_usb;
            ch582_set_protocol_interface(&ch582_protocol_usb);
            PRINT("Protocol select: USB\n");
            break;
#endif
#ifdef BLE_ENABLE
        case BOOTLOADER_BOOT_MODE_BLE:
            // bluetooth mode
            kbd_protocol_type = kbd_protocol_ble;
            ch582_set_protocol_interface(&ch582_protocol_ble);
            PRINT("Protocol select: BLE\n");
            break;
#endif
#ifdef ESB_ENABLE
        case BOOTLOADER_BOOT_MODE_ESB:
            // 2.4g mode
            kbd_protocol_type = kbd_protocol_esb;
            ch582_set_protocol_interface(&ch582_protocol_esb);
            PRINT("Protocol select: ESB\n");
            break;
#endif
        default:
            PRINT("IAP incomplete, will reboot back to IAP.\n");
            retention_register_set_iap();
            WAIT_FOR_DBG;
            SYS_ResetExecute();
            __builtin_unreachable();
    }
}
#endif

uint8_t bootloader_set_to_default_mode(const char *reason)
{
    PRINT("%s, ", reason);
#if !defined ESB_ENABLE || ESB_ENABLE == 1
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
#else
    PRINT("default to ESB.\n");
    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_ESB);
    return BOOTLOADER_BOOT_MODE_ESB;
#endif
}

void bootloader_jump()
{
    if (!bootmagic_allow_jump()) {
        return;
    }

    PRINT("Jumping IAP...\n");
    retention_register_set_iap();
    mcu_reset();
}

void mcu_reset()
{
#if __BUILDING_IAP__
    SYS_ResetExecute();
#endif
#if __BUILDING_APP__
    ch582_interface_t *interface = ch582_get_protocol_interface();

    if (interface) {
        interface->ch582_platform_reboot();
    }
#endif
}

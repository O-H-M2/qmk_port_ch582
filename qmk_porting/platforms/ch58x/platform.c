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

#include "platform_deps.h"
#include "gpio.h"
#include "quantum_keycodes.h"
#include "battery_measure.h"

volatile uint8_t kbd_protocol_type = 0;

_PUTCHAR_CLAIM;

int8_t sendchar(uint8_t c)
{
    _putchar(c);
    return 0;
}

void shutdown_user()
{
    rgbled_power_off();

#ifdef ENCODER_ENABLE
    pin_t encoders_pad_a[] = ENCODERS_PAD_A, encoders_pad_b[] = ENCODERS_PAD_B;

    for (uint8_t i = 0; i < sizeof(encoders_pad_a); i++) {
        setPinInputLow(encoders_pad_a[i]);
    }
    for (uint8_t i = 0; i < sizeof(encoders_pad_b); i++) {
        setPinInputLow(encoders_pad_b[i]);
    }
#endif
}

void platform_setup()
{
#if LSE_ENABLE
    R16_PIN_ANALOG_IE |= RB_PIN_XT32K_IE;
#endif
#ifdef POWER_DETECT_PIN
    setPinInputLow(POWER_DETECT_PIN);
#endif
#ifdef BATTERY_MEASURE_PIN
    uint16_t adc;

    battery_init();
    adc = battery_measure();
    battery_calculate(adc);
    PRINT("Battery level: %d\n", adc);
#endif
#if 0
    PRINT("EEPROM dump: \n");
    for (uint8_t i = 0; i < 8; i++) {
        PRINT("Page: %d\n", i);
        uint8_t eeprom_dump[0x1000] = {};
        EEPROM_READ(i * 0x1000, eeprom_dump, 0x1000);
        for (uint16_t j = 0; j < 0x1000; j++) {
            PRINT("0x%02x ", eeprom_dump[j]);
            DelayUs(20);
        }
        PRINT("\n\n");
        DelayMs(1);
    }
    PRINT("End of EEPROM dump.\n\n");
#endif
    bootloader_select_boot_mode();
#ifdef USB_ENABLE
    if (kbd_protocol_type == kbd_protocol_usb) {
        platform_initialize_usb();
    }
#endif
#ifdef BLE_ENABLE
    if (kbd_protocol_type == kbd_protocol_ble) {
        platform_initialize_ble();
    }
#endif
#ifdef ESB_ENABLE
    if (kbd_protocol_type == kbd_protocol_esb) {
        platform_initialize_esb();
    }
#endif
}

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
#include <stdio.h>
#include "HAL.h"
#include "gpio.h"
#include "quantum_keycodes.h"
#include "battery_measure.h"

volatile uint8_t kbd_protocol_type = 0;
#if defined BLE_ENABLE || (defined ESB_ENABLE && (ESB_ENABLE == 1 || ESB_ENABLE == 2))
extern void wireless_indicator_status_reset();
#else
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];
#endif

__HIGH_CODE _PUTCHAR_CLAIM;

int8_t sendchar(uint8_t c)
{
    _putchar(c);
    return 0;
}

#if !defined ESB_ENABLE || ESB_ENABLE == 1
void shutdown_user()
{
    rgbled_power_off();
#if defined BLE_ENABLE || (defined ESB_ENABLE && (ESB_ENABLE == 1 || ESB_ENABLE == 2))
    wireless_indicator_status_reset();
#endif

#ifdef ENCODER_ENABLE
    pin_t encoders_pad_a[] = ENCODERS_PAD_A, encoders_pad_b[] = ENCODERS_PAD_B;

    for (uint8_t i = 0; i < sizeof(encoders_pad_a) / sizeof(encoders_pad_a[0]); i++) {
        setPinInputLow(encoders_pad_a[i]);
    }
    for (uint8_t i = 0; i < sizeof(encoders_pad_b) / sizeof(encoders_pad_b[0]); i++) {
        setPinInputLow(encoders_pad_b[i]);
    }
#endif
}
#endif

void platform_setup()
{
#if LSE_ENABLE
    R16_PIN_ANALOG_IE |= RB_PIN_XT32K_IE;
#endif
#ifdef PLF_DEBUG
    DBG_INIT;
    PRINT("App " MACRO2STR(__GIT_VERSION__) ", build on %s\n", QMK_BUILDDATE);
#else
    writePinHigh(A9);
    setPinOutput(A9);
    setPinInputHigh(A8);
    UART1_DefInit();
    UART1_BaudRateCfg(DEBUG_BAUDRATE);

    char buffer[UINT8_MAX];
    uint8_t len = sprintf(buffer, "App " MACRO2STR(__GIT_VERSION__) ", build on %s\n", QMK_BUILDDATE);

    while (len) {
        if (R8_UART1_TFC != UART_FIFO_SIZE) {
            R8_UART1_THR = buffer[strlen(buffer) - len];
            len--;
        }
    }
    while ((R8_UART1_LSR & RB_LSR_TX_ALL_EMP) == 0) {
        __nop();
    }
    R8_UART1_IER = RB_IER_RESET;
    setPinInputLow(A8);
    setPinInputLow(A9);
#endif

    {
        // preserve BOOTMAGIC_LITE_ROW and BOOTMAGIC_LITE_COLUMN to eeprom for future use
        uint8_t buffer[EEPROM_PAGE_SIZE], ret;

        do {
            ret = EEPROM_READ(QMK_EEPROM_RESERVED_START_POSITION, buffer, sizeof(buffer));
        } while (ret);
        if (buffer[1] != BOOTMAGIC_LITE_ROW || buffer[2] != BOOTMAGIC_LITE_COLUMN) {
            buffer[1] = BOOTMAGIC_LITE_ROW;
            buffer[2] = BOOTMAGIC_LITE_COLUMN;
            do {
                ret = EEPROM_ERASE(QMK_EEPROM_RESERVED_START_POSITION, EEPROM_PAGE_SIZE) ||
                      EEPROM_WRITE(QMK_EEPROM_RESERVED_START_POSITION, buffer, EEPROM_PAGE_SIZE);
            } while (ret);
        }
    }

    bootloader_select_boot_mode();

    CH58X_BLEInit();
    HAL_Init();

    ch582_interface_t *interface = ch582_get_protocol_interface();

    if (interface) {
        interface->ch582_platform_initialize();
    }
#ifdef BLE_ENABLE
    if (kbd_protocol_type != kbd_protocol_ble) {
        ch582_protocol_ble.ch582_protocol_setup();
    }
#endif
}

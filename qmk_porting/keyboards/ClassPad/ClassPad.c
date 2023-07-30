/*
Copyright 2023 OctopusZ <https://github.com/OctopusZ>

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

#include QMK_KEYBOARD_H
#include "gpio.h"
#include "extra_keycode.h"

#ifdef RGB_MATRIX_ENABLE
/* clang-format off */
led_config_t g_led_config = {
	{
        {0     , 1     , 2     , 3     , NO_LED, NO_LED, NO_LED},
        {NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED},
        {NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED},
        {NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED},
        {NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED},
	}, {
        {0,0},{12,0},{24,0},{35,0},
	}, {
		2, 2, 2, 2, 
	}
};
/* clang-format on */

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max)
{
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }
    if (led_min <= 0 && led_max > 0 && host_keyboard_led_state().num_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(0, 0xFF, 0x00, 0x00);
    } else {
        RGB_MATRIX_INDICATOR_SET_COLOR(0, 0x00, 0x00, 0x00);
    }
    if (led_min <= 1 && led_max > 1) {
        RGB_MATRIX_INDICATOR_SET_COLOR(1, 0x00, 0x00, 0x00);
    }
    if (led_min <= 2 && led_max > 2) {
        RGB_MATRIX_INDICATOR_SET_COLOR(2, 0x00, 0x00, 0x00);
    }
    if (led_min <= 3 && led_max > 3) {
        RGB_MATRIX_INDICATOR_SET_COLOR(3, 0x00, 0x00, 0x00);
    }

#if defined BATTERY_MEASURE_PIN || defined BLE_ENABLE
    extern void wireless_rgb_indicator_task(uint8_t led_min, uint8_t led_max);

    wireless_rgb_indicator_task(led_min, led_max);
#endif
    return true;
}

#endif
bool dip_switch_update_kb(uint8_t index, bool active)
{
    switch (index) {
        case 0:
            if (active) // run once when tirggle
            {
                // writePinHigh(B12); // for debug
                // setPinOutput(B12);
                if (kbd_protocol_type != kbd_protocol_ble) {
                    wait_ms(50); // for debounce
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_BLE);
                    soft_reset_keyboard();
                }
            } else {
                // writePinLow(B12); // for debug
                // setPinOutput(B12);
                if (kbd_protocol_type != kbd_protocol_usb) {
                    wait_ms(50); // for debounce
                    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_USB);
                    soft_reset_keyboard();
                }
            }
            break;
    }
    return true;
}

int main()
{
    extern void protocol_setup();
    extern void protocol_init();
    extern void protocol_task();

    platform_setup();

    protocol_setup();
#if !defined ESB_ENABLE || ESB_ENABLE != 2
    keyboard_setup();
#endif

    protocol_init();

    /* Main loop */
    for (;;) {
        protocol_task();
        //! housekeeping_task() is handled by platform
    }
}

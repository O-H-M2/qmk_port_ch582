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

#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE

/* clang-format off */
led_config_t g_led_config = {
	{
		{0,  1,  2,  3}, 
		{7,  6,  5,   NO_LED}, 
		{8,  9,  10,  4}, 
		{13,  12,  11,   NO_LED}, 
		{14,   NO_LED,  15,  16}, 

	}, {
		{0,0},{75,0},{149,0},{224,0},
		{0,16},{75,16},{149,16},       
		{0,32},{75,32},{149,32},{224,32},
		{0,48},{75,48},{149,48},       
		{0,64},       {149,64},{224,64},

	}, {
		4, 4, 4, 4,
		4, 4, 4,
		4, 4, 4, 4,
		4, 4, 4,
		4, 4, 4,
	}
};
/* clang-format on */

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max)
{
    if (host_keyboard_led_state().num_lock) {
        if (g_led_config.flags[0] & LED_FLAG_KEYLIGHT) {
            rgb_matrix_set_color(0, RGB_RED);
        }
    }
}

#endif

#if !defined ESB_ENABLE || ESB_ENABLE != 2

int main()
{
    extern void protocol_setup();
    extern void protocol_init();
    extern void protocol_task();

    platform_setup();

    protocol_setup();
    keyboard_setup();

    protocol_init();

    /* Main loop */
    for (;;) {
        protocol_task();
        //! housekeeping_task() is handled by platform
    }
}

#else

__HIGH_CODE int main()
{
    platform_setup();

    for (;;) {
        TMOS_SystemProcess();
    }
}

#endif

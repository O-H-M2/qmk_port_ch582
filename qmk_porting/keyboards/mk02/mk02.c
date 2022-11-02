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

const aw_led g_aw_leds[DRIVER_LED_TOTAL] = {
    {0, CS1_SW1, CS2_SW1, CS3_SW1},        //  0, ESC, k13
    {0, CS4_SW1, CS5_SW1, CS6_SW1},        //  1, ~, k16
    {0, CS7_SW1, CS8_SW1, CS9_SW1},        //  2, Tab, k11
    {0, CS10_SW1, CS11_SW1, CS12_SW1},     //  3, Caps, k21
    {0, CS13_SW1, CS14_SW1, CS15_SW1},     //  4, Sh_L, k00
    {0, CS16_SW1, CS17_SW1, CS18_SW1},     //  5, Ct_L, k06
    {0, CS1_SW2, CS2_SW2, CS3_SW2},        //  6, F1, k26
    {0, CS4_SW2, CS5_SW2, CS6_SW2},        //  7, 1, k17
    {0, CS7_SW2, CS8_SW2, CS9_SW2},        //  8, Q, k10
    {0, CS10_SW2, CS11_SW2, CS12_SW2},     //  9, A, k12
    {0, CS13_SW2, CS14_SW2, CS15_SW2},     //  10, Z, k14
    {0, CS16_SW2, CS17_SW2, CS18_SW2},     //  11, Win_L, k90
    {0, CS1_SW3, CS2_SW3, CS3_SW3},        //  12, F2, k36
    {0, CS4_SW3, CS5_SW3, CS6_SW3},        //  13, 2, k27
    {0, CS7_SW3, CS8_SW3, CS9_SW3},        //  14, W, k20
    {0, CS10_SW3, CS11_SW3, CS12_SW3},     //  15, S, k22
    {0, CS13_SW3, CS14_SW3, CS15_SW3},     //  16, X, k24
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

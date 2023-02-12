/*
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

#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE

#ifdef AW20216
const aw_led PROGMEM g_aw_leds[DRIVER_LED_TOTAL] = {
    { 0, CS1_SW1, CS2_SW1, CS3_SW1 },
    { 0, CS4_SW1, CS5_SW1, CS6_SW1 },
    { 0, CS7_SW1, CS8_SW1, CS9_SW1 },
    { 0, CS10_SW1, CS11_SW1, CS12_SW1 },
    { 0, CS13_SW1, CS14_SW1, CS15_SW1 },
    { 0, CS16_SW1, CS17_SW1, CS18_SW1 },

    { 0, CS1_SW2, CS2_SW2, CS3_SW2 },
    { 0, CS4_SW2, CS5_SW2, CS6_SW2 },
    { 0, CS7_SW2, CS8_SW2, CS9_SW2 },
    { 0, CS10_SW2, CS11_SW2, CS12_SW2 },
    { 0, CS13_SW2, CS14_SW2, CS15_SW2 },
    { 0, CS16_SW2, CS17_SW2, CS18_SW2 },

    { 0, CS1_SW3, CS2_SW3, CS3_SW3 },
    { 0, CS4_SW3, CS5_SW3, CS6_SW3 },
    { 0, CS7_SW3, CS8_SW3, CS9_SW3 },
    { 0, CS10_SW3, CS11_SW3, CS12_SW3 },
    { 0, CS13_SW3, CS14_SW3, CS15_SW3 },
    { 0, CS16_SW3, CS17_SW3, CS18_SW3 },

    { 0, CS1_SW4, CS2_SW4, CS3_SW4 },
    { 0, CS4_SW4, CS5_SW4, CS6_SW4 },
    { 0, CS7_SW4, CS8_SW4, CS9_SW4 },
    { 0, CS10_SW4, CS11_SW4, CS12_SW4 },
    { 0, CS13_SW4, CS14_SW4, CS15_SW4 },
    { 0, CS16_SW4, CS17_SW4, CS18_SW4 },

    { 0, CS1_SW5, CS2_SW5, CS3_SW5 },
    { 0, CS4_SW5, CS5_SW5, CS6_SW5 },
    { 0, CS7_SW5, CS8_SW5, CS9_SW5 },
    { 0, CS10_SW5, CS11_SW5, CS12_SW5 },
    { 0, CS13_SW5, CS14_SW5, CS15_SW5 },
    { 0, CS16_SW5, CS17_SW5, CS18_SW5 },

    { 0, CS1_SW6, CS2_SW6, CS3_SW6 },
    { 0, CS4_SW6, CS5_SW6, CS6_SW6 },
    { 0, CS7_SW6, CS8_SW6, CS9_SW6 },
    { 0, CS10_SW6, CS11_SW6, CS12_SW6 },
    { 0, CS13_SW6, CS14_SW6, CS15_SW6 },
    { 0, CS16_SW6, CS17_SW6, CS18_SW6 },

    { 0, CS1_SW7, CS2_SW7, CS3_SW7 },
    { 0, CS4_SW7, CS5_SW7, CS6_SW7 },
    { 0, CS7_SW7, CS8_SW7, CS9_SW7 },
    { 0, CS10_SW7, CS11_SW7, CS12_SW7 },
    { 0, CS13_SW7, CS14_SW7, CS15_SW7 },
    { 0, CS16_SW7, CS17_SW7, CS18_SW7 },

    { 0, CS1_SW8, CS2_SW8, CS3_SW8 },
    { 0, CS4_SW8, CS5_SW8, CS6_SW8 },
    { 0, CS7_SW8, CS8_SW8, CS9_SW8 },
    { 0, CS10_SW8, CS11_SW8, CS12_SW8 },
    { 0, CS13_SW8, CS14_SW8, CS15_SW8 },
    { 0, CS16_SW8, CS17_SW8, CS18_SW8 },

    { 0, CS1_SW9, CS2_SW9, CS3_SW9 },
    { 0, CS4_SW9, CS5_SW9, CS6_SW9 },
    { 0, CS7_SW9, CS8_SW9, CS9_SW9 },
    { 0, CS10_SW9, CS11_SW9, CS12_SW9 },
    { 0, CS13_SW9, CS14_SW9, CS15_SW9 },
    { 0, CS16_SW9, CS17_SW9, CS18_SW9 },

    { 0, CS1_SW10, CS2_SW10, CS3_SW10 },
    { 0, CS4_SW10, CS5_SW10, CS6_SW10 },
    { 0, CS7_SW10, CS8_SW10, CS9_SW10 },
    { 0, CS10_SW10, CS11_SW10, CS12_SW10 },
    { 0, CS13_SW10, CS14_SW10, CS15_SW10 },
    { 0, CS16_SW10, CS17_SW10, CS18_SW10 },

    { 0, CS1_SW11, CS2_SW11, CS3_SW11 },
    { 0, CS4_SW11, CS5_SW11, CS6_SW11 },
    { 0, CS7_SW11, CS8_SW11, CS9_SW11 },
    { 0, CS10_SW11, CS11_SW11, CS12_SW11 },
    // { 0, CS13_SW11, CS14_SW11, CS15_SW11 },
    // { 0, CS16_SW11, CS17_SW11, CS18_SW11 },

    // { 0, CS1_SW12, CS2_SW12, CS3_SW12 },
    // { 0, CS4_SW12, CS5_SW12, CS6_SW12 },
    // { 0, CS7_SW12, CS8_SW12, CS9_SW12 },
    // { 0, CS10_SW12, CS11_SW12, CS12_SW12 },
    // { 0, CS13_SW12, CS14_SW12, CS15_SW12 },
    // { 0, CS16_SW12, CS17_SW12, CS18_SW12 },
};

void spi_master_pre_transmit_cb()
{
    aw20216_power_check();
}

void spi_master_post_transmit_cb()
{
    aw20216_delayed_power_off_excute();
}

#endif

/* clang-format off */
led_config_t g_led_config = {
	{
        {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13}, 
        {27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16,  15,  14}, 
        {28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,   NO_LED}, 
        {54,  53,  52,  51,  50,  49,  48,  47,  46,  45,  44,  43,  42,  41}, 
        {55,  56,  57,  58,  59,   NO_LED,   NO_LED,   NO_LED,  60,   NO_LED,   NO_LED,  61,  62,  63}, 
	}, {
        {0,0},{17,0},{34,0},{52,0},{69,0},{86,0},{103,0},{121,0},{138,0},{155,0},{172,0},{190,0},{207,0},{224,0},
        {224,16},{207,16},{190,16},{172,16},{155,16},{138,16},{121,16},{103,16},{86,16},{69,16},{52,16},{34,16},{17,16},{0,16},
        {0,32},{17,32},{34,32},{52,32},{69,32},{86,32},{103,32},{121,32},{138,32},{155,32},{172,32},{190,32},{207,32},       
        {224,48},{207,48},{190,48},{172,48},{155,48},{138,48},{121,48},{103,48},{86,48},{69,48},{52,48},{34,48},{17,48},{0,48},
        {0,64},{17,64},{34,64},{52,64},{69,64},                     {138,64},              {190,64},{207,64},{224,64},
	}, {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
		4, 4, 4, 4, 4, 4, 4, 4, 4,
	}
};
/* clang-format on */

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max)
{
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }
    if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(28, 0xFF, 0x00, 0x00);
    }
#if defined BATTERY_MEASURE_PIN || defined BLE_ENABLE
    extern void wireless_rgb_indicator_task(uint8_t led_min, uint8_t led_max);

    wireless_rgb_indicator_task(led_min, led_max);
#endif
    return true;
}

#endif

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

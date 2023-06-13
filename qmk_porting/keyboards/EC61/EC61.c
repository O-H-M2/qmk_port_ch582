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
#include "battery_measure.h"

#ifdef RGB_MATRIX_ENABLE

#ifdef AW20216
const aw_led PROGMEM g_aw_leds[DRIVER_LED_TOTAL] = {

    { 0, CS1_SW12, CS3_SW12, CS2_SW12 },
    { 0, CS1_SW11, CS3_SW11, CS2_SW11 },
    { 0, CS1_SW10, CS3_SW10, CS2_SW10 },
    { 0, CS1_SW9, CS3_SW9, CS2_SW9 },
    { 0, CS1_SW8, CS3_SW8, CS2_SW8 },
    { 0, CS1_SW7, CS3_SW7, CS2_SW7 },
    { 0, CS1_SW6, CS3_SW6, CS2_SW6 },
    { 0, CS1_SW5, CS3_SW5, CS2_SW5 },
    { 0, CS1_SW4, CS3_SW4, CS2_SW4 },
    { 0, CS1_SW3, CS3_SW3, CS2_SW3 },
    { 0, CS1_SW2, CS3_SW2, CS2_SW2 },
    { 0, CS1_SW1, CS3_SW1, CS2_SW1 },

    { 0, CS16_SW1, CS18_SW1, CS17_SW1 },
    { 0, CS16_SW2, CS18_SW2, CS17_SW2 },

    { 0, CS4_SW12, CS6_SW12, CS5_SW12 },
    { 0, CS4_SW11, CS6_SW11, CS5_SW11 },
    { 0, CS4_SW10, CS6_SW10, CS5_SW10 },
    { 0, CS4_SW9, CS6_SW9, CS5_SW9 },
    { 0, CS4_SW8, CS6_SW8, CS5_SW8 },
    { 0, CS4_SW7, CS6_SW7, CS5_SW7 },
    { 0, CS4_SW6, CS6_SW6, CS5_SW6 },
    { 0, CS4_SW5, CS6_SW5, CS5_SW5 },
    { 0, CS4_SW4, CS6_SW4, CS5_SW4 },
    { 0, CS4_SW3, CS6_SW3, CS5_SW3 },
    { 0, CS4_SW2, CS6_SW2, CS5_SW2 },
    { 0, CS4_SW1, CS6_SW1, CS5_SW1 },

    { 0, CS16_SW4, CS18_SW4, CS17_SW4 },
    { 0, CS16_SW3, CS18_SW3, CS17_SW3 },

    { 0, CS7_SW12, CS9_SW12, CS8_SW12 },
    { 0, CS7_SW11, CS9_SW11, CS8_SW11 },
    { 0, CS7_SW10, CS9_SW10, CS8_SW10 },
    { 0, CS7_SW9, CS9_SW9, CS8_SW9 },
    { 0, CS7_SW8, CS9_SW8, CS8_SW8 },
    { 0, CS7_SW7, CS9_SW7, CS8_SW7 },
    { 0, CS7_SW6, CS9_SW6, CS8_SW6 },
    { 0, CS7_SW5, CS9_SW5, CS8_SW5 },
    { 0, CS7_SW4, CS9_SW4, CS8_SW4 },
    { 0, CS7_SW3, CS9_SW3, CS8_SW3 },
    { 0, CS7_SW2, CS9_SW2, CS8_SW2 },
    { 0, CS7_SW1, CS9_SW1, CS8_SW1 },

    { 0, CS16_SW5, CS18_SW5, CS17_SW5 },

    { 0, CS10_SW12, CS12_SW12, CS11_SW12 },
    { 0, CS10_SW11, CS12_SW11, CS11_SW11 },
    { 0, CS10_SW10, CS12_SW10, CS11_SW10 },
    { 0, CS10_SW9, CS12_SW9, CS11_SW9 },
    { 0, CS10_SW8, CS12_SW8, CS11_SW8 },
    { 0, CS10_SW7, CS12_SW7, CS11_SW7 },
    { 0, CS10_SW6, CS12_SW6, CS11_SW6 },
    { 0, CS10_SW5, CS12_SW5, CS11_SW5 },
    { 0, CS10_SW4, CS12_SW4, CS11_SW4 },
    { 0, CS10_SW3, CS12_SW3, CS11_SW3 },
    { 0, CS10_SW2, CS12_SW2, CS11_SW2 },
    { 0, CS10_SW1, CS12_SW1, CS11_SW1 },

    { 0, CS13_SW12, CS15_SW12, CS14_SW12 },
    { 0, CS13_SW11, CS15_SW11, CS14_SW11 },
    { 0, CS13_SW10, CS15_SW10, CS14_SW10 },

    { 0, CS13_SW7, CS15_SW7, CS14_SW7 },

    { 0, CS13_SW4, CS15_SW4, CS14_SW4 },
    { 0, CS13_SW3, CS15_SW3, CS14_SW3 },
    { 0, CS13_SW2, CS15_SW2, CS14_SW2 },
    { 0, CS13_SW1, CS15_SW1, CS14_SW1 },

};

void spi_master_pre_transmit_cb()
{
    aw20216_power_check();
}

#endif

/* clang-format off */
led_config_t g_led_config = {
	{
        {     0,       1,       2,       3,       4,        5,        6,        7,        8,        9,       10,       11,       12,      13}, 
        {    14,      15,      16,      17,      18,       19,       20,       21,       22,       23,       24,       25,       26,      27}, 
        {    28,      29,      30,      31,      32,       33,       34,       35,       36,       37,       38,       39,       40,  NO_LED}, 
        {    41,      42,      43,      44,      45,       46,       47,       48,       49,       50,       51,   NO_LED,       52,  NO_LED}, 
        {    53,      54,      55,  NO_LED,  NO_LED,       56,   NO_LED,   NO_LED,   NO_LED,       57,       58,       59,       60,  NO_LED}, 
	}, {
          {7,3},  {22,3},  {37,3},  {52,3},  {67,3},   {82,3},   {97,3},  {112,3},  {127,3},  {142,3},  {157,3},  {172,3},  {187,3},  {209,3},
        {11,16}, {30,16}, {45,16}, {60,16}, {75,16},  {90,16}, {105,16}, {119,16}, {134,16}, {149,16}, {164,16}, {179,16}, {194,16}, {213,16},
        {13,29}, {34,29}, {49,29}, {63,29}, {78,29},  {93,29}, {108,29}, {123,29}, {138,29}, {153,29}, {168,29}, {183,29}, {207,29},
        {17,41}, {41,41}, {56,41}, {71,41}, {86,41}, {101,41}, {116,41}, {131,41}, {146,41}, {161,41}, {175,41},           {203,41},
         {9,54}, {28,54}, {47,54},                   {103,54},                                         {159,54}, {177,54}, {196,54}, {215,54},
	}, {
		      4,       4,       4,       4,       4,        4,        4,        4,        4,        4,        4,        4,        4,        4,
		      4,       4,       4,       4,       4,        4,        4,        4,        4,        4,        4,        4,        4,        4,
		      4,       4,       4,       4,       4,        4,        4,        4,        4,        4,        4,        4,        4, 
		      4,       4,       4,       4,       4,        4,        4,        4,        4,        4,        4,                  4,
		      4,       4,       4,                          4,                                      4,        4,        4,        4,
	}
};
/* clang-format on */
uint8_t lowpower_state = 0;
uint32_t lowpower_tick;
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max)
{
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }
    if (lowpower_state == 1) { // low power indicator breathing
        for (size_t i = led_min; i < led_max; i++) {
            if (i != 0) {
                RGB_MATRIX_INDICATOR_SET_COLOR(i, 0, 0, 0);
            }
        }
    }
    if (led_min <= 28 && led_max > 28 && host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(28, 0xFF, 0x00, 0x00);
    }
#if defined BATTERY_MEASURE_PIN || defined BLE_ENABLE
    extern void wireless_rgb_indicator_task(uint8_t led_min, uint8_t led_max);

    wireless_rgb_indicator_task(led_min, led_max);
#endif
    return true;
}

#endif
void housekeeping_task_kb()
{
    if (battery_get_last_percentage() < 5) {
        switch (lowpower_state) {
            case 0:
                lowpower_state = 1;
                lowpower_tick = timer_read();
                rgb_matrix_mode_noeeprom(RGB_MATRIX_BREATHING);
                rgb_matrix_set_speed_noeeprom(255);
                rgb_matrix_sethsv_noeeprom(0, 255, 255);
                break;
            case 1:
                if (timer_elapsed(lowpower_tick) > 8000) {
                    lowpower_state = 2;
                    rgb_matrix_disable_noeeprom();
                }
                break;
            default:
                break;
        }
    } else {
        if (lowpower_state == 2) {
            rgb_matrix_reload_from_eeprom();
        }
        lowpower_state = 0;
    }
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

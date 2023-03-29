// Copyright 2023 Y&R-Biu (@jiaxin96)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#ifdef RGB_MATRIX_ENABLE

/* clang-format off */
led_config_t g_led_config = {
	{
        { 7,      6,  5,  4      }, 
        { 8,      9,  10, 11     },
        { 15,     14, 13, 12     },
        { 16,     17, 18, NO_LED },
        { NO_LED, 20, 19, NO_LED }
	}, {
        {0,0},{74,0},{148,0},{224,0},

        {224,12},{148,12},{74,12},{0,12},
        {0,25},{74,25},{148,25},{224,25},
        {224,37},{148,25},{74,25},{0,25},
        {0,49},{74,25},{148,25},
        {148,64},{74,64}
	}, {
        2,2,2,2,
        4,4,4,4,
        4,4,4,4,
        4,4,4,4,
        4,4,4,
          4,4
	}
};
/* clang-format on */

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max)
{
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }
    if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(0, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(1, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(2, 0xFF, 0x00, 0x00);
        RGB_MATRIX_INDICATOR_SET_COLOR(3, 0xFF, 0x00, 0x00);
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

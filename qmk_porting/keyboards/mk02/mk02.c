#include "mk02.h"

#ifdef RGB_MATRIX_ENABLE

/* clang-format off */
led_config_t g_led_config = {
	{
		{0, 1, 2, 3},
		{4, 5, 6},
		{7, 8, 9, 10},
		{11, 12, 13},
		{14, 15, 16},
	}, {
		{0, 0}, {6, 0}, {12, 0}, {18, 0},
		{0, 8}, {6, 8}, {12, 8},
		{0, 16}, {6, 16}, {12, 16}, {18, 12},
		{0, 24}, {6, 24}, {12, 24},
		{3, 32}, {12, 32}, {18, 28},
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

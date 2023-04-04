// Copyright 2023 alin elena <https://github.com/alinelena> <https://gitlab.com/drFaustroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H


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

void toggle_leds(const bool toggle_lwr, const bool toggle_rse) {
    led_lwr(toggle_lwr);
    led_rse(toggle_rse);
    if (layer_state_is(_ADJ)) {
        led_lwr(true);
        led_rse(true);
    }
}


void keyboard_post_init_kb(void) {

    init_lwr_rse_led();
    keyboard_post_init_user();
}

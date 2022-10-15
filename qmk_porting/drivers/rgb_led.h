#pragma once

#include <stdint.h>
#include <stdbool.h>
#ifdef WS2812
#include "ws2812.h"
#endif

__attribute__((always_inline)) inline bool rgbled_status_check()
{
#ifdef RGBLIGHT_ENABLE
    extern bool rgblight_is_enabled();
    return rgblight_is_enabled();
#elif defined RGB_MATRIX_ENABLE
    extern uint8_t rgb_matrix_is_enabled();
    return rgb_matrix_is_enabled();
#else
    return false;
#endif
}

__attribute__((always_inline)) inline void rgbled_power_off()
{
    // power off rgb leds or the controller
#ifdef WS2812
    ws2812_power_toggle(false);
#endif
}

#pragma once

#include <stdint.h>
#include <stdbool.h>

__attribute__((always_inline)) inline void rgbled_power_check()
{
    // power off rgb leds or the controller when led is toggled off
#ifdef RGBLIGHT_ENABLE
    extern bool rgblight_is_enabled();
    if (rgblight_is_enabled()) {
#elif defined RGB_MATRIX_ENABLE
    extern uint8_t rgb_matrix_is_enabled();
    if (rgb_matrix_is_enabled()) {
#else
    {
#endif
#ifdef WS2812
        ws2812_deinit();
#endif
    }
}

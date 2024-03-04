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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#ifdef WS2812
#include "ws2812.h"
#include "ws2812_supplement.h"
#endif
#ifdef AW20216S
#include "aw20216s_supplement.h"
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
#ifdef AW20216S
    aw20216s_power_toggle(false);
#endif
}

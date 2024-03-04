/*
Copyright 2024 Huckies <https://github.com/Huckies>

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

#include "ws2812_supplement.h"

static bool ws2812_powered_on = false;

bool ws2812_power_get()
{
    return ws2812_powered_on;
}

void ws2812_power_toggle(bool status)
{
    if (ws2812_powered_on == status) {
        return;
    }

#ifdef WS2812_EN_PIN
    if (status) {
        gpio_write_pin(WS2812_EN_PIN, WS2812_EN_LEVEL);
        gpio_set_pin_output(WS2812_EN_PIN);
    } else {
        gpio_write_pin(WS2812_EN_PIN, WS2812_EN_LEVEL ? 0 : 1);
        gpio_set_pin_output(WS2812_EN_PIN);
    }
#endif

    ws2812_powered_on = status;
}

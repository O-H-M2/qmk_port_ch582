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

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "timer.h"

#define BATTERY_INDICATOR_TIMEOUT 5000 // in unit of ms

uint16_t battery_get_min();
uint16_t battery_get_max();
void battery_init();
uint16_t battery_measure();
uint8_t battery_calculate(uint16_t adcVal);
void battery_indicator_toggle(bool status);
bool battery_indicator_state();
bool battery_indicator_timeout();

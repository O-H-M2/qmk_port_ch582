/*
Copyright 2022 Huckies <https://github.com/Huckies>
Copyright 2022 puterjam<puterjam@gmail.com>

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

#include "color.h"

#ifndef RGB_RAW_USAGE_PAGE
#define RGB_RAW_USAGE_PAGE 0xFF60
#endif

#ifndef RGB_RAW_USAGE_ID
#define RGB_RAW_USAGE_ID 0x61
#endif

#if !defined(AUXILIARY_RGB_STARTUP_RED)
#define AUXILIARY_RGB_STARTUP_RED 0
#endif

#if !defined(AUXILIARY_RGB_STARTUP_GREEN)
#define AUXILIARY_RGB_STARTUP_GREEN 167
#endif

#if !defined(AUXILIARY_RGB_STARTUP_BLUE)
#define AUXILIARY_RGB_STARTUP_BLUE 224
#endif

extern uint8_t auxiliary_rgb_tx_buffer[];

void rgb_raw_hid_receive(uint8_t *data, uint8_t length);
void rgb_raw_hid_send(uint8_t *data, uint8_t length);

bool auxiliary_rgb_init_buffer();
void auxiliary_rgb_deinit_buffer();

void auxiliary_rgb_set_color_buffer(int index, uint8_t red, uint8_t green, uint8_t blue);
// void auxiliary_rgb_reload_openrgb_colors();
void auxiliary_rgb_flush();
RGB *auxiliary_rgb_get_color_buffer();
void auxiliary_mode_confirm();
// RGB auxiliary_rgb_get_color_buffer_element(int index);
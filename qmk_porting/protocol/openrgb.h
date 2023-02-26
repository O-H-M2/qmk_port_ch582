/*
Copyright 2020 Kasper
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

#include "usb_interface.h"
#include "rgb_matrix.h"
#include "auxiliary_rgb.h"

// This is changed only when the command IDs change,
// so OpenRGB can detect compatible firmware.
#define OPENRGB_PROTOCOL_VERSION 0xE //add openRGB Version
#define OPENRGB_EPSIZE           RGBRAW_EP_SIZE

enum openrgb_command_id {
    OPENRGB_GET_PROTOCOL_VERSION = 1,
    OPENRGB_GET_QMK_VERSION,
    OPENRGB_GET_DEVICE_INFO,
    OPENRGB_GET_MODE_INFO,
    OPENRGB_GET_LED_INFO,
    OPENRGB_GET_ENABLED_MODES,

    OPENRGB_SET_MODE,
    OPENRGB_DIRECT_MODE_SET_SINGLE_LED,
    OPENRGB_DIRECT_MODE_SET_LEDS,
};

enum openrgb_responses {
    OPENRGB_FAILURE = 25,
    OPENRGB_SUCCESS = 50,
    OPENRGB_END_OF_MESSAGE = 100,
};

bool openrgb_command_handler(uint8_t *data, uint8_t length);

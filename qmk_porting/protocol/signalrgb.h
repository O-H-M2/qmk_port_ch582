/*
Copyright 2023 OctupusZ <https://github.com/OctupusZ>
Copyright 2020 Kasper

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

#include "usb_main.h"
#include "rgb_matrix.h"
#include "auxiliary_rgb.h"

#define SINGALRGB_EPSIZE 64
enum signalrgb_commands {
    SIGNALRGB_GET_QMK_VERSION = 0x21,
    SIGNALRGB_GET_PROTOCOL_VERSION = 0x22,
    SIGNALRGB_GET_UNIQUE_IDENTIFIER = 0x23,
    SIGNALRGB_STREAM_RGB_DATA = 0x24,
    SIGNALRGB_SET_SIGNALRGB_MODE_ENABLE = 0x25,
    SIGNALRGB_SET_SIGNALRGB_MODE_DISABLE = 0x26,
    SIGNALRGB_GET_TOTAL_LEDS = 0x27,
    SIGNALRGB_GET_FIRMWARE_TYPE = 0x28,
};

enum signalrgb_responses //These are a bit clunky right now. Could use improvement.
{
    PROTOCOL_VERSION_BYTE_1 = 1,
    PROTOCOL_VERSION_BYTE_2 = 0,
    PROTOCOL_VERSION_BYTE_3 = 4,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_1 = 0,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_2 = 0,
    DEVICE_UNIQUE_IDENTIFIER_BYTE_3 = 0,
    FIRMWARE_TYPE_BYTE = 1,
    DEVICE_ERROR_LEDS = 255, //Error code to show that there are more leds than a packet will allow.
};
bool signal_rgb_command_handler(uint8_t *data, uint8_t length);

//Changelogs for Firmware Versions------------------------------------
//V1.0.1 added detection for the total number of LEDs a board has. Plugins need a rewrite to make use of this change. Rewritten plugins will not function with older firmware.
//V1.0.2 added another detection byte for which fork of SignalRGB firmware device is running. This means we can keep overlap Unique Identifiers between Sonix and Mainline.
//V1.0.3 unifies the command IDs between Mainline, Sonix, and VIA. All commands have a 0x20 offset now.
//V1.0.4 improves detection handling, and has a complete rewrite of the plugins. Also merges Mainline and VIA branches. VIA Branch is deprecated.
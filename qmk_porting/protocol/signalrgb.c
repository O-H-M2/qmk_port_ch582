/*
Copyright 2020 Kasper
Copyright 2023 OctupusZ <https://github.com/OctupusZ>
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

#include "quantum.h"
#include <string.h>
#include "signalrgb.h"

static uint8_t *signalrgb_buffer = NULL;

static void get_qmk_version() //Grab the QMK Version the board's firmware is built off of
{
    signalrgb_buffer[0] = SIGNALRGB_GET_QMK_VERSION;

    uint8_t current_byte = 1;
    uint8_t qmk_ver = 0;

    for (uint8_t i = 0; (current_byte < (SIGNALRGB_EPSIZE - 2)) && (QMK_VERSION[i] != 0); i++) {
        if (QMK_VERSION[i] == '.') {
            signalrgb_buffer[current_byte] = qmk_ver;
            qmk_ver = 0;
            current_byte++;
        } else {
            qmk_ver = qmk_ver * 10 + (QMK_VERSION[i] - '0');
        }
    }
    signalrgb_buffer[current_byte] = qmk_ver;
}

static void get_signalrgb_protocol_version()
{
    signalrgb_buffer[0] = SIGNALRGB_GET_PROTOCOL_VERSION;
    signalrgb_buffer[1] = PROTOCOL_VERSION_BYTE_1;
    signalrgb_buffer[2] = PROTOCOL_VERSION_BYTE_2;
    signalrgb_buffer[3] = PROTOCOL_VERSION_BYTE_3;
}

static void get_unique_identifier() //Grab the unique identifier for each specific model of keyboard.
{
    signalrgb_buffer[0] = SIGNALRGB_GET_UNIQUE_IDENTIFIER;
    signalrgb_buffer[1] = DEVICE_UNIQUE_IDENTIFIER_BYTE_1;
    signalrgb_buffer[2] = DEVICE_UNIQUE_IDENTIFIER_BYTE_2;
    signalrgb_buffer[3] = DEVICE_UNIQUE_IDENTIFIER_BYTE_3;
}

static void led_streaming(uint8_t *data) //Stream data from HID Packets to Keyboard.
{
    uint8_t index = data[1];
    uint8_t numberofleds = data[2];

    if (numberofleds >= 10) {
        signalrgb_buffer[1] = DEVICE_ERROR_LEDS;
        return;
    }

    for (uint8_t i = 0; i < numberofleds; i++) {
        uint8_t offset = (i * 3) + 3;
        uint8_t r = data[offset];
        uint8_t g = data[offset + 1];
        uint8_t b = data[offset + 2];

        auxiliary_rgb_set_color_buffer(index + i, r, g, b);
    }
}

void signalrgb_mode_enable()
{
    rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_AUXILIARY_RGB); //Set RGB Matrix to SignalRGB Compatible Mode
    auxiliary_rgb_flush();
}

void signalrgb_mode_disable()
{
    rgb_matrix_reload_from_eeprom(); //Reloading last effect from eeprom
}

static void get_total_leds() //Grab total number of leds that a board has.
{
    signalrgb_buffer[0] = SIGNALRGB_GET_TOTAL_LEDS;
    signalrgb_buffer[1] = RGB_MATRIX_LED_COUNT;
}

static void get_firmware_type() //Grab which fork of qmk a board is running.
{
    signalrgb_buffer[0] = SIGNALRGB_GET_FIRMWARE_TYPE;
    signalrgb_buffer[1] = FIRMWARE_TYPE_BYTE;
}
bool signalrgb_command_handler(uint8_t *data, uint8_t length)
{
    bool need_response = (*data != SIGNALRGB_STREAM_RGB_DATA);

    signalrgb_buffer = data;

    switch (*data) {
        case SIGNALRGB_GET_QMK_VERSION:
            get_qmk_version();
            break;
        case SIGNALRGB_GET_PROTOCOL_VERSION:
            get_signalrgb_protocol_version();
            break;
        case SIGNALRGB_GET_UNIQUE_IDENTIFIER:
            get_unique_identifier();
            break;
        case SIGNALRGB_STREAM_RGB_DATA:
            auxiliary_mode_confirm();
            led_streaming(data);
            break;
        case SIGNALRGB_SET_SIGNALRGB_MODE_ENABLE:
            signalrgb_mode_enable();
            break;
        case SIGNALRGB_SET_SIGNALRGB_MODE_DISABLE:
            signalrgb_mode_disable();
            break;
        case SIGNALRGB_GET_TOTAL_LEDS:
            get_total_leds();
            break;
        case SIGNALRGB_GET_FIRMWARE_TYPE:
            get_firmware_type();
            break;
        default:
            break;
    }

    if (need_response) {
        return true;
    }
    return false;
}
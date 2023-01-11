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

#include "quantum.h"
#include "rgb_matrix.h"
#include "usb_main.h"
#include "auxiliary_rgb.h"
#include "openrgb.h"
// #ifdef RAW_ENABLE
// #include "raw_hid.h"
// #endif

// uint16_t g_auxiliary_rgb_timer = 0;
// bool g_auxiliary_rgb_anim_playing = false;
// static uint8_t auxiliary_rgb_mode = HID_MODE_OPENRGB;
static RGB auxiliary_rgb_color_buffer[RGB_MATRIX_LED_COUNT] = { [0 ... RGB_MATRIX_LED_COUNT - 1] = { AUXILIARY_RGB_STARTUP_GREEN, AUXILIARY_RGB_STARTUP_RED, AUXILIARY_RGB_STARTUP_BLUE } };

extern bool openrgb_command_handler(uint8_t *data, uint8_t length);
// extern bool signal_rgb_command_handler(uint8_t *data, uint8_t length);

void rgb_raw_hid_receive(uint8_t *data, uint8_t length)
{
    bool send = false;

    switch (*data) {
        case OPENRGB_GET_PROTOCOL_VERSION ... OPENRGB_DIRECT_MODE_SET_LEDS:
            send = openrgb_command_handler(data, length);
            break;
        // case 0x21 ... 0x28:
        //     send = signal_rgb_command_handler(data, length);
        //     break;
        default:
#ifdef RAW_ENABLE
            PRINT("\n **** Unhandled! ****\n\n");
            mcu_reset();
// pass it to RAW interface
// raw_hid_receive(data, QMKRAW_OUT_EP_SIZE);
#endif
            break;
    }

    if (send) {
        rgb_raw_hid_send(data, length);
    }
}

void auxiliary_rgb_set_color_buffer(int index, uint8_t red, uint8_t green, uint8_t blue)
{
    if (rgb_matrix_get_mode() != RGB_MATRIX_CUSTOM_AUXILIARY_RGB) {
        return;
    }

    // if (auxiliary_rgb_get_mode() == HID_MODE_OPENRGB) {
    auxiliary_rgb_color_buffer[index].r = red;
    auxiliary_rgb_color_buffer[index].g = green;
    auxiliary_rgb_color_buffer[index].b = blue;
    //     } else {
    // #ifdef AUXILIARY_RGB_USE_UNIVERSAL_BRIGHTNESS
    //         float brightness = (float)rgb_matrix_config.hsv.v / UINT8_MAX;

    //         rgb_matrix_set_color(index, brightness * red, brightness * green, brightness * blue);
    // #else
    //         rgb_matrix_set_color(index, red, green, blue);
    // #endif
    //     }
}

// void auxiliary_rgb_reload_openrgb_colors()
// {
//     // if (auxiliary_rgb_get_mode() == HID_MODE_OPENRGB) {
//         for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
//             auxiliary_rgb_set_color_buffer(i, auxiliary_rgb_color_buffer[i].r, auxiliary_rgb_color_buffer[i].g, auxiliary_rgb_color_buffer[i].b);
//         }
//     // }
// }

void auxiliary_rgb_flush()
{
    // if (!g_auxiliary_rgb_anim_playing) {
    //     g_auxiliary_rgb_timer = 0;
    // }
    rgb_matrix_set_color_all(0, 0, 0);
}

RGB *auxiliary_rgb_get_color_buffer()
{
    return auxiliary_rgb_color_buffer;
}

RGB auxiliary_rgb_get_color_buffer_element(int index)
{
    return auxiliary_rgb_color_buffer[index];
}

// void auxiliary_rgb_set_mode(int mode)
// {
//     auxiliary_rgb_mode = mode;
// }

// uint8_t auxiliary_rgb_get_mode()
// {
//     return auxiliary_rgb_mode;
// }

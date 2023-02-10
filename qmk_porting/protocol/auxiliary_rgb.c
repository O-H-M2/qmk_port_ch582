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

// uint16_t g_auxiliary_rgb_timer = 0;
// bool g_auxiliary_rgb_anim_playing = false;

static RGB *auxiliary_rgb_color_buffer = NULL;

extern bool openrgb_command_handler(uint8_t *data, uint8_t length);
// extern bool signal_rgb_command_handler(uint8_t *data, uint8_t length);

void rgb_raw_hid_receive(uint8_t *data, uint8_t length)
{
    bool send = false;
    extern int usbd_deinitialize();

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
#ifdef USB_ENABLE
            if (kbd_protocol_type == kbd_protocol_usb) {
                usbd_deinitialize();
                init_usb_driver();
            }
#endif
#ifdef ESB_ENABLE
            if (kbd_protocol_type == kbd_protocol_esb) {
#if ESB_ENABLE == 1
                reenumerate_dongle_esb();
#elif ESB_ENABLE == 2
                usbd_deinitialize();
                init_usb_driver();
#endif
            }
#endif
#endif
            return;
    }

    if (send) {
        rgb_raw_hid_send(data, length);
    }
}

bool auxiliary_rgb_init_buffer()
{
    if (auxiliary_rgb_color_buffer != NULL) {
        free(auxiliary_rgb_color_buffer);
    }

    auxiliary_rgb_color_buffer = (RGB *)malloc(sizeof(RGB) * RGB_MATRIX_LED_COUNT);
    for (uint16_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        auxiliary_rgb_color_buffer[i].r = AUXILIARY_RGB_STARTUP_RED;
        auxiliary_rgb_color_buffer[i].g = AUXILIARY_RGB_STARTUP_GREEN;
        auxiliary_rgb_color_buffer[i].b = AUXILIARY_RGB_STARTUP_BLUE;
    }

    return (auxiliary_rgb_color_buffer != NULL);
}

void auxiliary_rgb_deinit_buffer()
{
    if (auxiliary_rgb_color_buffer == NULL) {
        return;
    }

    free(auxiliary_rgb_color_buffer);
    auxiliary_rgb_color_buffer = NULL;
}

void auxiliary_rgb_set_color_buffer(int index, uint8_t red, uint8_t green, uint8_t blue)
{
    if (rgb_matrix_get_mode() != RGB_MATRIX_CUSTOM_AUXILIARY_RGB) {
        return;
    }
    if (auxiliary_rgb_color_buffer == NULL) {
        return;
    }

    auxiliary_rgb_color_buffer[index].r = red;
    auxiliary_rgb_color_buffer[index].g = green;
    auxiliary_rgb_color_buffer[index].b = blue;
}

// void auxiliary_rgb_reload_openrgb_colors()
// {
//     for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
//         auxiliary_rgb_set_color_buffer(i, auxiliary_rgb_color_buffer[i].r, auxiliary_rgb_color_buffer[i].g, auxiliary_rgb_color_buffer[i].b);
//     }
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

void auxiliary_mode_confirm()
{
    if (rgb_matrix_get_mode() != RGB_MATRIX_CUSTOM_AUXILIARY_RGB) {
        rgb_matrix_mode_noeeprom(RGB_MATRIX_CUSTOM_AUXILIARY_RGB);
        auxiliary_rgb_flush();
    }
}

// RGB auxiliary_rgb_get_color_buffer_element(int index)
// {
//     return auxiliary_rgb_color_buffer[index];
// }

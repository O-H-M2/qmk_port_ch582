/*
Copyright 2022 Huckies <https://github.com/Huckies>
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

#include "quantum.h"
#include <string.h>
#include "openrgb.h"

// #if !defined(OPENRGB_DIRECT_MODE_STARTUP_RED)
// #define OPENRGB_DIRECT_MODE_STARTUP_RED 0
// #endif

// #if !defined(OPENRGB_DIRECT_MODE_STARTUP_GREEN)
// #define OPENRGB_DIRECT_MODE_STARTUP_GREEN 167
// #endif

// #if !defined(OPENRGB_DIRECT_MODE_STARTUP_BLUE)
// #define OPENRGB_DIRECT_MODE_STARTUP_BLUE 224
// #endif

#if !defined(OPENRGB_DEFAULT_KEYMAP_ID)
#define OPENRGB_DEFAULT_KEYMAP_ID 0 // default keyamp id，read keymap by id from u keyboard
#endif

extern const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS];
// RGB g_openrgb_direct_mode_colors[RGB_MATRIX_LED_COUNT] = { [0 ... RGB_MATRIX_LED_COUNT - 1] = { OPENRGB_DIRECT_MODE_STARTUP_GREEN, OPENRGB_DIRECT_MODE_STARTUP_RED, OPENRGB_DIRECT_MODE_STARTUP_BLUE } };
static const uint8_t openrgb_rgb_matrix_effects_indexes[] = {
    1,
    2,
#ifdef ENABLE_RGB_MATRIX_ALPHAS_MODS
    3,
#endif
#ifdef ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    4,
#endif
#ifdef ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    5,
#endif
#ifdef ENABLE_RGB_MATRIX_BREATHING
    6,
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_SAT
    7,
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_VAL
    8,
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    9,
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    10,
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    11,
#endif
#ifdef ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    12,
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_ALL
    13,
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    14,
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    15,
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    16,
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    17,
#endif
#ifdef ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    18,
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    19,
#endif
#ifdef ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    20,
#endif
#ifdef ENABLE_RGB_MATRIX_DUAL_BEACON
    21,
#endif
#ifdef ENABLE_RGB_MATRIX_RAINBOW_BEACON
    22,
#endif
#ifdef ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    23,
#endif
#ifdef ENABLE_RGB_MATRIX_RAINDROPS
    24,
#endif
#ifdef ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    25,
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_BREATHING
    26,
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_PENDULUM
    27,
#endif
#ifdef ENABLE_RGB_MATRIX_HUE_WAVE
    28,
#endif
#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_TYPING_HEATMAP)
    29,
#endif
#if defined(RGB_MATRIX_FRAMEBUFFER_EFFECTS) && defined(ENABLE_RGB_MATRIX_DIGITAL_RAIN)
    30,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    31,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_REACTIVE
    32,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    33,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    34,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    35,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    36,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    37,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    38,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SPLASH
    39,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_MULTISPLASH
    40,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_SPLASH
    41,
#endif
#if defined RGB_MATRIX_KEYREACTIVE_ENABLED && defined ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
    42,
#endif
#ifdef ENABLE_RGB_MATRIX_PIXEL_RAIN
    43,
#endif
#ifdef ENABLE_RGB_MATRIX_PIXEL_FLOW
    44,
#endif
#ifdef ENABLE_RGB_MATRIX_PIXEL_FRACTAL
    45,
#endif
};
static uint8_t openrgb_hid_buffer[OPENRGB_EPSIZE];

bool openrgb_command_handler(uint8_t *data, uint8_t length)
{
    switch (*data) {
        case OPENRGB_GET_PROTOCOL_VERSION:
            openrgb_get_protocol_version();
            break;
        case OPENRGB_GET_QMK_VERSION:
            openrgb_get_qmk_version();
            break;
        case OPENRGB_GET_DEVICE_INFO:
            openrgb_get_device_info();
            break;
        case OPENRGB_GET_MODE_INFO:
            openrgb_get_mode_info();
            break;
        case OPENRGB_GET_LED_INFO:
            openrgb_get_led_info(data);
            break;
        case OPENRGB_GET_ENABLED_MODES:
            openrgb_get_enabled_modes();
            break;
        case OPENRGB_SET_MODE:
            openrgb_set_mode(data);
            break;
        case OPENRGB_DIRECT_MODE_SET_SINGLE_LED:
            openrgb_direct_mode_set_single_led(data);
            break;
        case OPENRGB_DIRECT_MODE_SET_LEDS:
            openrgb_direct_mode_set_leds(data);
            break;
        default:
            break;
    }

    if (*data != OPENRGB_DIRECT_MODE_SET_LEDS) {
        openrgb_hid_buffer[OPENRGB_EPSIZE - 1] = OPENRGB_END_OF_MESSAGE;
        memcpy(data, openrgb_hid_buffer, OPENRGB_EPSIZE);
        memset(openrgb_hid_buffer, 0x00, OPENRGB_EPSIZE);
        return true;
    }
    return false;
}

void openrgb_get_protocol_version()
{
    openrgb_hid_buffer[0] = OPENRGB_GET_PROTOCOL_VERSION;
    openrgb_hid_buffer[1] = OPENRGB_PROTOCOL_VERSION;
}

void openrgb_get_qmk_version()
{
    openrgb_hid_buffer[0] = OPENRGB_GET_QMK_VERSION;

    uint8_t current_byte = 1;

    for (uint8_t i = 0; (current_byte < (OPENRGB_EPSIZE - 2)) && (QMK_VERSION[i] != 0); i++) {
        openrgb_hid_buffer[current_byte] = QMK_VERSION[i];
        current_byte++;
    }
}

void openrgb_get_device_info()
{
    openrgb_hid_buffer[0] = OPENRGB_GET_DEVICE_INFO;
    openrgb_hid_buffer[1] = RGB_MATRIX_LED_COUNT;
    openrgb_hid_buffer[2] = MATRIX_COLS * MATRIX_ROWS;

#define MASSDROP_VID 0x04D8
#if VENDOR_ID == MASSDROP_VID
#define PRODUCT_STRING      PRODUCT
#define MANUFACTURER_STRING MANUFACTURER
#else
#define PRODUCT_STRING      STR(PRODUCT)
#define MANUFACTURER_STRING STR(MANUFACTURER)
#endif

    uint8_t current_byte = 3;

    for (uint8_t i = 0; (current_byte < ((OPENRGB_EPSIZE - 2) / 2)) && (PRODUCT_STRING[i] != 0); i++) {
        openrgb_hid_buffer[current_byte] = PRODUCT_STRING[i];
        current_byte++;
    }
    openrgb_hid_buffer[current_byte] = 0;
    current_byte++;
    for (uint8_t i = 0; (current_byte + 2 < OPENRGB_EPSIZE) && (MANUFACTURER_STRING[i] != 0); i++) {
        openrgb_hid_buffer[current_byte] = MANUFACTURER_STRING[i];
        current_byte++;
    }
}

void openrgb_get_mode_info()
{
    const HSV hsv_color = rgb_matrix_get_hsv();

    openrgb_hid_buffer[0] = OPENRGB_GET_MODE_INFO;
    openrgb_hid_buffer[1] = rgb_matrix_get_mode();
    openrgb_hid_buffer[2] = rgb_matrix_get_speed();
    openrgb_hid_buffer[3] = hsv_color.h;
    openrgb_hid_buffer[4] = hsv_color.s;
    openrgb_hid_buffer[5] = hsv_color.v;
}

void openrgb_get_led_info(uint8_t *data)
{
    const uint8_t first_led = data[1];
    const uint8_t number_leds = data[2];

    const RGB *openrgb_colors = auxiliary_rgb_get_color_buffer();

    openrgb_hid_buffer[0] = OPENRGB_GET_LED_INFO;
    for (uint8_t i = 0; i < number_leds; i++) {
        const uint8_t led_idx = first_led + i;
        const uint8_t data_idx = i * 7;

        if (led_idx >= RGB_MATRIX_LED_COUNT) {
            openrgb_hid_buffer[data_idx + 3] = OPENRGB_FAILURE;
        } else {
            openrgb_hid_buffer[data_idx + 1] = g_led_config.point[led_idx].x;
            openrgb_hid_buffer[data_idx + 2] = g_led_config.point[led_idx].y;
            openrgb_hid_buffer[data_idx + 3] = g_led_config.flags[led_idx];

            openrgb_hid_buffer[data_idx + 4] = openrgb_colors[led_idx].r;
            openrgb_hid_buffer[data_idx + 5] = openrgb_colors[led_idx].g;
            openrgb_hid_buffer[data_idx + 6] = openrgb_colors[led_idx].b;
        }

        uint8_t row = 0;
        uint8_t col = 0;
        uint8_t found = 0;

        for (row = 0; row < MATRIX_ROWS; row++) {
            for (col = 0; col < MATRIX_COLS; col++) {
                if (g_led_config.matrix_co[row][col] == led_idx) {
                    found = 1;
                    break;
                }
            }

            if (found == 1) {
                break;
            }
        }

        if (col >= MATRIX_COLS || row >= MATRIX_ROWS) {
            openrgb_hid_buffer[data_idx + 7] = KC_NO;
        } else {
            openrgb_hid_buffer[data_idx + 7] = pgm_read_byte(&keymaps[OPENRGB_DEFAULT_KEYMAP_ID][row][col]);
        }
    }
}

void openrgb_get_enabled_modes()
{
    openrgb_hid_buffer[0] = OPENRGB_GET_ENABLED_MODES;

    const uint8_t size = sizeof openrgb_rgb_matrix_effects_indexes / sizeof openrgb_rgb_matrix_effects_indexes[0];

    for (int i = 0; i < size; i++) {
        openrgb_hid_buffer[i + 1] = openrgb_rgb_matrix_effects_indexes[i];
    }
}

void openrgb_set_mode(uint8_t *data)
{
    const uint8_t h = data[1];
    const uint8_t s = data[2];
    const uint8_t v = data[3];
    const uint8_t mode = data[4];
    const uint8_t speed = data[5];
    const uint8_t save = data[6];

    openrgb_hid_buffer[0] = OPENRGB_SET_MODE;

    if (h > 255 || s > 255 || v > 255 || mode >= RGB_MATRIX_EFFECT_MAX || speed > 255) {
        openrgb_hid_buffer[OPENRGB_EPSIZE - 2] = OPENRGB_FAILURE;
        return;
    }

    // auxiliary_rgb_set_mode(HID_MODE_OPENRGB);

    if (save == 1) {
        rgb_matrix_mode(mode);
        rgb_matrix_set_speed(speed);
        rgb_matrix_sethsv(h, s, v);
    } else {
        rgb_matrix_mode_noeeprom(mode);
        rgb_matrix_set_speed_noeeprom(speed);
        rgb_matrix_sethsv_noeeprom(h, s, v);
    }

    if (rgb_matrix_get_mode() == RGB_MATRIX_CUSTOM_AUXILIARY_RGB) {
        auxiliary_rgb_flush();
    }

    openrgb_hid_buffer[OPENRGB_EPSIZE - 2] = OPENRGB_SUCCESS;
}

void openrgb_direct_mode_set_single_led(uint8_t *data)
{
    // if (auxiliary_rgb_get_mode() != HID_MODE_OPENRGB) {
    //     openrgb_hid_buffer[OPENRGB_EPSIZE - 2] = OPENRGB_FAILURE;
    //     return;
    // }

    const uint8_t led = data[1];
    const uint8_t r = data[2];
    const uint8_t g = data[3];
    const uint8_t b = data[4];

    openrgb_hid_buffer[0] = OPENRGB_DIRECT_MODE_SET_SINGLE_LED;

    if (led >= RGB_MATRIX_LED_COUNT || r > 255 || g > 255 || b > 255) {
        openrgb_hid_buffer[OPENRGB_EPSIZE - 2] = OPENRGB_FAILURE;
        return;
    }

    auxiliary_rgb_set_color_buffer(led, r, g, b);

    openrgb_hid_buffer[OPENRGB_EPSIZE - 2] = OPENRGB_SUCCESS;
}

void openrgb_direct_mode_set_leds(uint8_t *data)
{
    // if (auxiliary_rgb_get_mode() != HID_MODE_OPENRGB) {
    //     openrgb_hid_buffer[OPENRGB_EPSIZE - 2] = OPENRGB_FAILURE;
    //     return;
    // }

    const uint8_t number_leds = data[1];

    for (uint8_t i = 0; i < number_leds; i++) {
        const uint8_t data_idx = i * 4;
        const uint8_t color_idx = data[data_idx + 2];

        auxiliary_rgb_set_color_buffer(color_idx, data[data_idx + 3], data[data_idx + 4], data[data_idx + 5]);
    }
}

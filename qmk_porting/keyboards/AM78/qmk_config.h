/*
Copyright 2022 OctopusZ <https://github.com/OctopusZ>

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

//* platform settings
// #define DEBUG        Debug_UART1
#define DCDC_ENABLE  1
#define FREQ_SYS     40000000
#define LSE_ENABLE   1
#define BLE_SLOT_NUM 4
// #define HSE_LOAD_CAPACITANCE 7.5 // in pF unit
// #define LSE_LOAD_CAPACITANCE 7   // in pF unit
// #define I2C_IO_REMAPPING
#define SPI_IO_REMAPPING

/* USB Device descriptor parameter */
#define VENDOR_ID    0xCAFE
#define PRODUCT_ID   0xCAFE
#define DEVICE_VER   0x0001
#define MANUFACTURER ezytest
#define PRODUCT      AM78

#define QMK_VERSION    "0.0.1"
#define QMK_BUILDDATE  __DATE__
#define QMK_KEYBOARD_H "AM78.h"

#define MATRIX_ROWS 6
#define MATRIX_COLS 15
#define MATRIX_ROW_PINS            \
    {                              \
        B9, B8, B17, B16, B18, B19 \
    }
#define MATRIX_COL_PINS                                                \
    {                                                                  \
        A6, A0, A1, A2, A3, A15, A14, A13, A12, A7, B7, B6, B5, B4, B3 \
    }

#define DYNAMIC_KEYMAP_LAYER_COUNT 10

#define DIODE_DIRECTION       COL2ROW
#define BOOTMAGIC_LITE_ROW    0
#define BOOTMAGIC_LITE_COLUMN 0
// #define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS

#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP FALSE

#define WS2812_EN_PIN   A5
#define WS2812_EN_LEVEL 1

#define BATTERY_MEASURE_PIN A4
//#define POWER_DETECT_PIN    B12

#ifdef ENCODER_ENABLE
#define ENCODERS_PAD_A \
    {                  \
        B2             \
    }
#define ENCODERS_PAD_B \
    {                  \
        B1             \
    }
#define ENCODER_RESOLUTION 4
#endif

#ifdef RGBLIGHT_ENABLE
#define WS2812_SPI_DRIVER 2
#define RGBLED_NUM        10
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_RGB
// #define RGBLIGHT_ANIMATIONS
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_RGB_TEST
#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_TWINKLE
#define RGBLIGHT_LIMIT_VAL   128
#define RGBLIGHT_HUE_STEP    20
#define RGBLIGHT_SAT_STEP    20
#define RGBLIGHT_VAL_STEP    20
#define RGBLIGHT_DEFAULT_HUE 191
#define RGBLIGHT_DEFAULT_SAT 255
#define RGBLIGHT_DEFAULT_VAL 10
#define RGBLIGHT_DEFAULT_SPD 1
#endif

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 10

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

#include "pre_handler.h"
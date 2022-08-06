/*
Copyright 2015 Jun Wako <wakojun@gmail.com>

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

// platform settings
#define DEBUG       1
#define DCDC_ENABLE 1

#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP FALSE

/* USB Device descriptor parameter */
#define VENDOR_ID    0xFEED
#define PRODUCT_ID   0x0B91
#define DEVICE_VER   0x0001
#define MANUFACTURER Xiaomi
#define PRODUCT      MK02

#define QMK_VERSION    "0.0.1"
#define QMK_BUILDDATE  "Today"
#define QMK_KEYBOARD_H "mk02.h"

#define MATRIX_ROWS 5
#define MATRIX_COLS 14
#define MATRIX_ROW_PINS      \
    {                        \
        B9, A7, B8, B16, B18 \
    }
#define MATRIX_COL_PINS                                          \
    {                                                            \
        A4, A5, A6, A0, A1, A3, B17, B5, B4, B3, B2, B1, B0, B21 \
    }
#define DIODE_DIRECTION       COL2ROW
#define BOOTMAGIC_LITE_ROW    1
#define BOOTMAGIC_LITE_COLUMN 1

#define MOUSE_ENABLE    1
#define RAW_ENABLE      1
#define EEPROM_ENABLE   1
#define RGBLIGHT_ENABLE 1
#define COMMAND_ENABLE  1

#define RGBLED_NUM 61

#ifdef RGBLIGHT_ENABLE
// #define RGBLIGHT_LIMIT_VAL 100
#define RGB_DI_PIN        A14
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_RGB
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_TWINKLE
#define RGBLIGHT_LIMIT_VAL 100
#endif

/* define if matrix has ghost */
//#define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

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

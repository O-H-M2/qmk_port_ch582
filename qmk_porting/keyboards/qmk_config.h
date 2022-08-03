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

#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP FALSE

/* USB Device descriptor parameter */
#define VENDOR_ID    0xFEED
#define PRODUCT_ID   0x0B91
#define DEVICE_VER   0x0001
#define MANUFACTURER Xiaomi
#define PRODUCT      MK02

#define MATRIX_ROW_PINS      \
    {                        \
        B9, A7, B8, B16, B18 \
    }
#define MATRIX_COL_PINS                                          \
    {                                                            \
        A4, A5, A6, A0, A1, A3, B17, B5, B4, B3, B2, B1, B0, B21 \
    }
#define DIODE_DIRECTION COL2ROW

#ifdef RGBLIGHT_ENABLE
#define RGBLIGHT_USE_TIMER
#define RGBLIGHT_EFFECT_RGB_TEST
#define RGBLIGHT_LIMIT_VAL 200
#define RGB_DI_PIN         A14
#define WS2812_BYTE_ORDER  WS2812_BYTE_ORDER_RGB
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

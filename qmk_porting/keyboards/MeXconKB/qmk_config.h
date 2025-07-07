/*
Copyright 2022 Huckies <https://github.com/Huckies>
Copyright 2023 OctopusZ <https://github.com/OctopusZ>

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

/* USB Device descriptor parameter */
#define VENDOR_ID    0xCAFE
#define PRODUCT_ID   0x0B92
#define DEVICE_VER   0x0001
#define MANUFACTURER MeXdiy
#define PRODUCT      MeXconKB

#define ANALOG_JOYSTICK_X_AXIS_PIN A4
#define ANALOG_JOYSTICK_Y_AXIS_PIN A6
#define POINTING_DEVICE_INVERT_Y
#define ANALOG_JOYSTICK_AUTO_AXIS 1
#define ANALOG_JOYSTICK_CLICK_PIN A5

#define ANALOG_JOYSTICK_WEIGHTS {1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 10, 10, 10, 11, 11, 12, 13, 13, 14, 14, 15, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28, 29, 30, 32, 33, 35, 36, 38, 40, 42, 44, 46, 48, 50, 52, 55, 58, 60, 63, 66, 69, 72, 76, 79, 83, 87, 91, 95, 100}

#define MATRIX_ROWS 7
#define MATRIX_COLS 10
#define MATRIX_ROW_PINS \
    {                   \
        A3, A0, B20, B1, B5, B6, B7}
#define MATRIX_COL_PINS \
    {                   \
        B2, B3, B4, B19, B0, A1, A2, B21, A11, A10}
// #define DYNAMIC_KEYMAP_LAYER_COUNT 10

#define DIODE_DIRECTION  COL2ROW
#define BOOTMAGIC_ROW    0
#define BOOTMAGIC_COLUMN 0
// #define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS

#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP FALSE

#ifdef ENCODER_ENABLE
#define ENCODER_A_PINS \
    {                  \
        B5}
#define ENCODER_B_PINS \
    {                  \
        B6}
#define ENCODER_RESOLUTION 4
#endif

#define WS2812_EN_PIN   A3
#define WS2812_EN_LEVEL 1

#ifdef RGB_MATRIX_ENABLE
// #define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_REACTIVE
#define RGBLED_NUM                    64
#define RGB_MATRIX_LED_COUNT          RGBLED_NUM
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 32
#define RGB_MATRIX_STARTUP_VAL        RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define RGB_MATRIX_HUE_STEP           10
#define RGB_MATRIX_SAT_STEP           8
#define RGB_MATRIX_VAL_STEP           4
#define RGB_MATRIX_SPD_STEP           10
#define RGB_MATRIX_SLEEP
#define ENABLE_RGB_MATRIX_ALPHAS_MODS
#define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
#define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_BREATHING
#define ENABLE_RGB_MATRIX_BAND_SAT
#define ENABLE_RGB_MATRIX_BAND_VAL
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
#define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
#define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
#define ENABLE_RGB_MATRIX_CYCLE_ALL
#define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
#define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
#define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
#define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
#define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
#define ENABLE_RGB_MATRIX_DUAL_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_BEACON
#define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
#define ENABLE_RGB_MATRIX_RAINDROPS
#define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
#define ENABLE_RGB_MATRIX_HUE_BREATHING
#define ENABLE_RGB_MATRIX_HUE_PENDULUM
#define ENABLE_RGB_MATRIX_HUE_WAVE
#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#define ENABLE_RGB_MATRIX_SPLASH
#define ENABLE_RGB_MATRIX_MULTISPLASH
#define ENABLE_RGB_MATRIX_SOLID_SPLASH
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#define ENABLE_RGB_MATRIX_PIXEL_RAIN
#define ENABLE_RGB_MATRIX_PIXEL_FLOW
#define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
#endif
#ifdef RGB_RAW_ENABLE
#define AUXILIARY_RGB_USE_UNIVERSAL_BRIGHTNESS
#endif

#ifdef AW20216S
#define AW_GLOBAL_CURRENT_MAX 255
#define AW_SCALING_MAX        255

#define DRIVER_1_CS B20
#define DRIVER_1_EN B21

#define DRIVER_COUNT       1
#define DRIVER_1_LED_TOTAL 64
#define DRIVER_LED_TOTAL   (DRIVER_1_LED_TOTAL)
#endif

/* define if matrix has ghost */
// #define MATRIX_HAS_GHOST

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 10

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
// #define NO_DEBUG

/* disable print */
// #define NO_PRINT

/* disable action features */
// #define NO_ACTION_LAYER
// #define NO_ACTION_TAPPING
// #define NO_ACTION_ONESHOT

/* define for OLED resolution  */
#ifdef OLED_ENABLE
#define OLED_DISPLAY_128X64
#endif

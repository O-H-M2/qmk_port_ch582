/*

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

#define DCDC_ENABLE  1
#define FREQ_SYS     40000000
#define LSE_ENABLE   0
#define BLE_SLOT_NUM 4
// #define HSE_LOAD_CAPACITANCE 7.5 // in pF unit
// #define LSE_LOAD_CAPACITANCE 7   // in pF unit
// #define I2C_IO_REMAPPING
// #define SPI_IO_REMAPPING

#define TAPPING_TOGGLE 2
#define PRODUCT_ID 0x6071
#define VENDOR_ID 0xBABA
#define PRODUCT "mlego/m65 rev2"
#define MANUFACTURER "Alin Elena"
#define DEVICE_VER 0x0001

#define MATRIX_ROW_PINS  {A4, A5}
#define MATRIX_COL_PINS  {B22, B23}

#define QMK_VERSION    "19.9"
#define QMK_BUILDDATE  __DATE__
#define QMK_KEYBOARD_H "m4rev1.h"

#define MATRIX_ROWS 2
#define MATRIX_COLS 2

#define DIODE_DIRECTION       COL2ROW
#define BOOTMAGIC_LITE_ROW    0
#define BOOTMAGIC_LITE_COLUMN 0

// #define PERMISSIVE_HOLD
#define HOLD_ON_OTHER_KEY_PRESS
#ifdef ENCODER_ENABLE
#define ENCODERS_PAD_A \
    {                  \
        A10            \
    }
#define ENCODERS_PAD_B \
    {                  \
        A11            \
    }
#define ENCODER_RESOLUTION 4
#endif
#define EARLY_INIT_PERFORM_BOOTLOADER_JUMP FALSE

/*
#define BATTERY_MEASURE_PIN A2
#define POWER_DETECT_PIN    B12
*/
#define LED_CAPS_LOCK_PIN A8

#define DEBUG_MATRIX_SCAN_RATE

#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX, UNICODE_MODE_MACOS, UNICODE_MODE_WINDOWS, UNICODE_MODE_WINCOMPOSE


#define DEBOUNCE 5

#include "pre_handler.h"

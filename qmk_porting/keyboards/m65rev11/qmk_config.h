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

#    define TAPPING_TOGGLE 2
#    define PRODUCT_ID 0x6071
#    define VENDOR_ID 0xBABA
#    define PRODUCT "mlego/m65 rev11"
#    define MANUFACTURER "Alin Elena"
#    define DEVICE_VER 0x0001

#define QMK_VERSION    "0.0.1"
#define QMK_BUILDDATE  __DATE__
#define QMK_KEYBOARD_H "m65rev11.h"

#define MATRIX_ROWS 5
#define MATRIX_COLS 14

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


#define BATTERY_MEASURE_PIN A2
#define POWER_DETECT_PIN    B12


#define SPI_DRIVER SPID0

#define SPI_LATCH_PIN A12
#define SPI_SCK_PIN A13
#define SPI_MISO_PIN A15
#define SPI_MOSI_PIN A14

#define SPI_DIVISOR 8

#define DEBUG_MATRIX_SCAN_RATE


//0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
// 1QA     1QB     1QC     1QD     1QE     1QF     1QG     1QH     2QA     2QB     2QC     2QD     2QE     2QF     2QG     2QH

//                  C0      C1      C2       C3      C4      C5     C6       C7      C8      C9     C10     C11     C12     C13
//                 2QB     2QC     2QD      2QE     2QF     2QG    2QH      2QA     1QH     1QG     1QF     1QE     1QA     1QD
#define COLS {  0x0200, 0x0400, 0x0800,  0x1000, 0x2000, 0x4000, 0x8000, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0001, 0x0008 }

/*
#define WS2812_EN_PIN   B6
#define WS2812_EN_LEVEL 1
#define RGBLED_NUM        15
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_RGB
*/
/* Set 0 if debouncing isn't needed */
#define DEBOUNCE 5

#include "pre_handler.h"

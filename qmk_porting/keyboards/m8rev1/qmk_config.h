// Copyright 2022 Huckies <https://github.com/Huckies>
// Copyright 2023 alin elena <https://github.com/alinelena> <https://gitlab.com/drFaustroll>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define DCDC_ENABLE  1
#define FREQ_SYS     40000000
#define LSE_ENABLE   0
#define BLE_SLOT_NUM 4
// #define HSE_LOAD_CAPACITANCE 7.5 // in pF unit
// #define LSE_LOAD_CAPACITANCE 7   // in pF unit
// #define I2C_IO_REMAPPING
#undef SPI_IO_REMAPPING
// #define SPI_IO_REMAPPING

#define TAPPING_TOGGLE 2
#define PRODUCT_ID 0x6071
#define VENDOR_ID 0xBABA
#define PRODUCT "mlego/m8 rev1"
#define MANUFACTURER "Alin Elena"
#define DEVICE_VER 0x0001

#define ROWS  {A4, A5}

#define QMK_VERSION    "19.9"
#define QMK_BUILDDATE  __DATE__
#define QMK_KEYBOARD_H "m8rev1.h"

#define MATRIX_ROWS 4
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

#define SPI_DRIVER SPID0

#define SPI_CS_PIN A12
#define SPI_SCK_PIN A13
#define SPI_MOSI_PIN A14
#define SPI_MISO_PIN A15
#define SPI_DIVISOR 32
#define SPI_MODE 3
#define SPI_LSBFIRST false

#define LED_CAPS_LOCK_PIN A8

#define DEBUG_MATRIX_SCAN_RATE
//0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000
// 1QA     1QB     1QC     1QD     1QE     1QF     1QG     1QH     2QA     2QB     2QC     2QD     2QE     2QF     2QG     2QH

//              C0      C1
//              1QA     2QA
// #define COLS { 0x0001, 0x0100}
// the test revision had a mistake and all cols are on one register
#define COLS { 0x0008, 0x0004}
#define ROWS {3,2,1,0}

#define DEBOUNCE 5

/*
#define DEBUG                Debug_UART1
#define DEBUG_BAUDRATE       460800
*/
#ifdef OLED_ENABLE
#    define OLED_DISPLAY_128X32
#define I2C1_SCL_PIN        GP7
#define I2C1_SDA_PIN        GP6
#define I2C_DRIVER I2CD2
#define OLED_BRIGHTNESS 128
#define OLED_FONT_H "keyboards/mlego/m65/lib/glcdfont.c"
#endif

#include "pre_handler.h"

/*
Copyright 2022 Huckies <https://github.com/Huckies>

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

#include "CH58x_common.h"
#include "eeprom_partition_table.h"
#include "partition_table.h"

#if !defined(UINT8_MAX)
#define UINT8_MAX ((uint8_t)-1)
#endif

#if !defined(UINT16_MAX)
#define UINT16_MAX ((uint16_t)-1)
#endif

#if !defined(UINT32_MAX)
#define UINT32_MAX ((uint32_t)-1)
#endif

#ifdef DEBUG
#include <stdio.h>
#include "printf.h"
#define PRINT(X...) printf(X)
#ifndef DEBUG_BAUDRATE
#define DEBUG_BAUDRATE 460800
#endif
#if DEBUG == Debug_UART0
#define PLF_DEBUG Debug_UART0
#define DBG_INIT         \
    writePinHigh(B7);    \
    setPinInputHigh(B4); \
    setPinOutput(B7);    \
    UART0_DefInit();     \
    UART0_BaudRateCfg(DEBUG_BAUDRATE);
#define DBG_BAUD_RECONFIG \
    UART0_BaudRateCfg(DEBUG_BAUDRATE);
#define WAIT_FOR_DBG                                  \
    while ((R8_UART0_LSR & RB_LSR_TX_ALL_EMP) == 0) { \
        __nop();                                      \
    }
#define _PUTCHAR_CLAIM                           \
    void _putchar(char character)                \
    {                                            \
        while (R8_UART0_TFC == UART_FIFO_SIZE) { \
            __nop();                             \
        }                                        \
        R8_UART0_THR = (uint8_t)character;       \
    }
#elif DEBUG == Debug_UART1
#define PLF_DEBUG Debug_UART1
#define DBG_INIT         \
    writePinHigh(A9);    \
    setPinInputHigh(A8); \
    setPinOutput(A9);    \
    UART1_DefInit();     \
    UART1_BaudRateCfg(DEBUG_BAUDRATE);
#define DBG_BAUD_RECONFIG \
    UART1_BaudRateCfg(DEBUG_BAUDRATE);
#define WAIT_FOR_DBG                                  \
    while ((R8_UART1_LSR & RB_LSR_TX_ALL_EMP) == 0) { \
        __nop();                                      \
    }
#define _PUTCHAR_CLAIM                           \
    void _putchar(char character)                \
    {                                            \
        while (R8_UART1_TFC == UART_FIFO_SIZE) { \
            __nop();                             \
        }                                        \
        R8_UART1_THR = (uint8_t)character;       \
    }
#elif DEBUG == Debug_UART2
#define PLF_DEBUG Debug_UART2
#define DBG_INIT         \
    writePinHigh(A7);    \
    setPinInputHigh(A6); \
    setPinOutput(A7);    \
    UART2_DefInit();     \
    UART2_BaudRateCfg(DEBUG_BAUDRATE);
#define DBG_BAUD_RECONFIG \
    UART2_BaudRateCfg(DEBUG_BAUDRATE);
#define WAIT_FOR_DBG                                  \
    while ((R8_UART2_LSR & RB_LSR_TX_ALL_EMP) == 0) { \
        __nop();                                      \
    }
#define _PUTCHAR_CLAIM                           \
    void _putchar(char character)                \
    {                                            \
        while (R8_UART2_TFC == UART_FIFO_SIZE) { \
            __nop();                             \
        }                                        \
        R8_UART2_THR = (uint8_t)character;       \
    }
#elif DEBUG == Debug_UART3
#define PLF_DEBUG Debug_UART3
#define DBG_INIT         \
    writePinHigh(A5);    \
    setPinInputHigh(A4); \
    setPinOutput(A5);    \
    UART3_DefInit();     \
    UART3_BaudRateCfg(DEBUG_BAUDRATE);
#define DBG_BAUD_RECONFIG \
    UART3_BaudRateCfg(DEBUG_BAUDRATE);
#define WAIT_FOR_DBG                                  \
    while ((R8_UART3_LSR & RB_LSR_TX_ALL_EMP) == 0) { \
        __nop();                                      \
    }
#define _PUTCHAR_CLAIM                           \
    void _putchar(char character)                \
    {                                            \
        while (R8_UART3_TFC == UART_FIFO_SIZE) { \
            __nop();                             \
        }                                        \
        R8_UART3_THR = (uint8_t)character;       \
    }
#else
#error "Invalid DEBUG UART selection!"
#endif
#undef DEBUG
#else
#define PRINT(X...)
#define NO_PRINT
#ifndef DEBUG_BAUDRATE
#define DEBUG_BAUDRATE 460800
#endif
#define WAIT_FOR_DBG
#define _PUTCHAR_CLAIM                              \
    void _putchar(char character)                   \
    {                                               \
        /* set a placeholder to avoid link error */ \
    }
#endif

#if defined BLE_ENABLE || (defined ESB_ENABLE && ESB_ENABLE == 1)
#define NO_USB_STARTUP_CHECK
#ifndef BATTERY_MEASURE_PIN
#warning "Battery measure pin undefined."
#else
#endif
#ifndef POWER_DETECT_PIN
#warning "Power detect pin undefined."
#endif
#ifndef LSE_ENABLE
#define LSE_ENABLE 1
#endif
#else
#ifdef BATTERY_MEASURE_PIN
#undef BATTERY_MEASURE_PIN
#endif
#ifdef POWER_DETECT_PIN
#undef POWER_DETECT_PIN
#endif
#ifndef LSE_ENABLE
#define LSE_ENABLE 0
#endif
#endif

#ifdef BLE_ENABLE
#ifndef DCDC_ENABLE
#define DCDC_ENABLE 1
#endif
#ifndef HAL_SLEEP
#define HAL_SLEEP 1
#endif
#ifndef QMK_TASK_INTERVAL_MAX
#define QMK_TASK_INTERVAL_MAX SYS_TICK_MS(15)
#endif
#ifndef QMK_TASK_INTERVAL_LED
#define QMK_TASK_INTERVAL_LED 2
#endif
#ifndef BLE_SLOT_NUM
#define BLE_SLOT_NUM 4
#endif
#endif

#ifdef ESB_ENABLE
#if ESB_ENABLE == 1
#ifndef DCDC_ENABLE
#define DCDC_ENABLE 1
#endif
#ifndef HAL_SLEEP
#define HAL_SLEEP 1
#endif
#ifndef QMK_TASK_INTERVAL_MAX
#define QMK_TASK_INTERVAL_MAX SYS_TICK_MS(20) // 1.25ms
#endif
#ifndef QMK_TASK_INTERVAL_LED
#define QMK_TASK_INTERVAL_LED QMK_TASK_INTERVAL_MAX
#endif
#elif ESB_ENABLE == 2
#ifdef FREQ_SYS
#undef FREQ_SYS
#endif
#ifdef DCDC_ENABLE
#undef DCDC_ENABLE
#endif
#ifdef HAL_SLEEP
#undef HAL_SLEEP
#endif
#ifdef BLE_TX_POWER
#undef BLE_TX_POWER
#endif
#define FREQ_SYS     60000000
#define DCDC_ENABLE  0
#define HAL_SLEEP    0
#define BLE_TX_POWER LL_TX_POWEER_6_DBM
#endif
#endif

#if FREQ_SYS == 80000000
#define Fsys CLK_SOURCE_PLL_80MHz
#elif FREQ_SYS == 60000000
#define Fsys CLK_SOURCE_PLL_60MHz
#elif FREQ_SYS == 48000000
#define Fsys CLK_SOURCE_PLL_48MHz
#elif FREQ_SYS == 40000000
#define Fsys CLK_SOURCE_PLL_40MHz
#elif FREQ_SYS == 32000000
#define Fsys CLK_SOURCE_PLL_32MHz
#elif FREQ_SYS == 24000000
#define Fsys CLK_SOURCE_PLL_24MHz
#elif FREQ_SYS == 16000000
#define Fsys CLK_SOURCE_HSE_16MHz
#elif FREQ_SYS == 8000000
#define Fsys CLK_SOURCE_HSE_8MHz
#elif FREQ_SYS == 4000000
#define Fsys CLK_SOURCE_HSE_4MHz
#elif FREQ_SYS == 2000000
#define Fsys CLK_SOURCE_HSE_2MHz
#elif FREQ_SYS == 1000000
#define Fsys CLK_SOURCE_HSE_1MHz
#else
#error "Illegal CPU clock!"
#endif

#if (defined RGBLIGHT_ENABLE || defined RGB_MATRIX_ENABLE) && FREQ_SYS <= 32000000
#error "CPU clock speed too low!"
#endif

#ifndef EXTRAKEY_ENABLE
#define EXTRAKEY_ENABLE
#endif

#ifndef MAGIC_KEYCODE_ENABLE
#define MAGIC_KEYCODE_ENABLE
#endif

#ifndef SEND_STRING_ENABLE
#define SEND_STRING_ENABLE
#endif

#ifdef NKRO_ENABLE
// a special trick
#define PROTOCOL_LUFA
#ifdef RGB_RAW_ENABLE
#define ENDPOINT_TOTAL_ENDPOINTS 8
#else
#define ENDPOINT_TOTAL_ENDPOINTS 5
#endif
#else
#ifdef FORCE_NKRO
#undef FORCE_NKRO
#endif
#endif

#if defined LSE_ENABLE && LSE_ENABLE
#define FREQ_RTC   32768
#define CLK_OSC32K 0
#ifdef WS2812_DRIVER_PWM
#warning "WS2812 PWM driver can not be used with LSE!"
#undef WS2812_DRIVER_PWM
#endif
#ifdef WS2812_PWM_DRIVER
#warning "WS2812 PWM driver can not be used with LSE!"
#undef WS2812_PWM_DRIVER
#endif
#else
#ifdef LSE_LOAD_CAPACITANCE
#undef LSE_LOAD_CAPACITANCE
#endif
#define CLK_OSC32K 1
#define FREQ_RTC   32000
#endif

#define SLEEP_RTC_MAX_TIME (RTC_TIMER_MAX_VALUE - TMOS_TIME_VALID)

#if defined AW20216 && !defined AW_SPI_DIVISOR
#define AW_SPI_DIVISOR (FREQ_SYS / 5000000)
#endif

#ifndef WS2812
#ifdef WS2812_EN_PIN
#undef WS2812_EN_PIN
#endif
#ifdef WS2812_EN_LEVEL
#undef WS2812_EN_LEVEL
#endif
#endif

#ifdef WS2812_DRIVER_SPI
#ifdef SPI_IO_REMAPPING
#define WS2812_DI_PIN B14
#else
#define WS2812_DI_PIN A14
#endif
#elif defined WS2812_DRIVER_PWM
#if WS2812_PWM_DRIVER == 1
#define WS2812_DI_PIN A10
#elif WS2812_PWM_DRIVER == 2
#define WS2812_DI_PIN A11
#endif
#endif

#define MACRO2STR_(_macro) #_macro
#define MACRO2STR(_macro)  MACRO2STR_(_macro)

// ms to 1 tick (625us)
#define SYS_TICK_MS(x) MS1_TO_SYSTEM_TIME(x)
// ms to 1.25ms (connection interval)
#define MS_UNIT_1_25(x) ((int)(x / 1.25))

enum {
    PLATFORM_EVENT_MODE_SELECT = 1,
    PLATFORM_EVENT_INITIALIZE,
    PROTOCOL_EVENT_PRE_INIT,
    PROTOCOL_EVENT_RUN,
    PLATFORM_EVENT_REBOOT,
    USB_EVENT_START_INDEX = 10,
    BLE_EVENT_START_INDEX = 20,
    ESB_EVENT_START_INDEX = 30,
};

enum {
    kbd_protocol_start = 0,
    kbd_protocol_usb,
    kbd_protocol_ble,
    kbd_protocol_esb,
    kbd_protocol_max,
};

#if BLE_SLOT_NUM > 16
#error "Too many BLE slots! Cap: 16"
#endif

#ifdef BATTERY_MEASURE_PIN
#if BATTERY_MEASURE_PIN != A7 && BATTERY_MEASURE_PIN != A8 && BATTERY_MEASURE_PIN != A9 &&   \
    BATTERY_MEASURE_PIN != A4 && BATTERY_MEASURE_PIN != A5 && BATTERY_MEASURE_PIN != A6 &&   \
    BATTERY_MEASURE_PIN != A0 && BATTERY_MEASURE_PIN != A1 && BATTERY_MEASURE_PIN != A2 &&   \
    BATTERY_MEASURE_PIN != A3 && BATTERY_MEASURE_PIN != A15 && BATTERY_MEASURE_PIN != A14 && \
    BATTERY_MEASURE_PIN != A13 && BATTERY_MEASURE_PIN != A12
#error "Invalid battery measure pin!"
#endif
#endif

#if !defined USB_ENABLE && !defined BLE_ENABLE && !defined ESB_ENABLE
#error "No interface enabled!"
#endif

#define memcmp(...)              tmos_memcmp(__VA_ARGS__) ? 0 : 1
#define strlen(pString)          tmos_strlen((char *)(pString))
#define memset(pDst, Value, len) tmos_memset(pDst, (int)(Value), (size_t)(len))
#define memcpy                   tmos_memcpy

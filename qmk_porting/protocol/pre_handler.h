#pragma once

#if !defined(UINT32_MAX)
#define UINT32_MAX ((uint32_t)-1)
#endif

#ifdef DEBUG
#define PLF_DEBUG DEBUG
#else
#define NO_PRINT
#endif

#if defined RGBLIGHT_ENABLE && defined RGB_MATRIX_ENABLE
#error "RGB_LIGHT can not be used with RGB_MATRIX simultaneously"
#endif

#ifdef BLE_ENABLE
#ifdef DCDC_ENABLE
#undef DCDC_ENABLE
#endif
#ifdef HAL_SLEEP
#undef HAL_SLEEP
#endif
#define DCDC_ENABLE 1
#define HAL_SLEEP   1
#ifndef QMK_TASK_INTERVAL
#define QMK_TASK_INTERVAL QMK_TASK_INTERVAL_MAX
#endif
#ifndef QMK_TASK_INTERVAL_LED
#define QMK_TASK_INTERVAL_LED SYS_TICK_MS(5)
#endif
#ifndef BLE_SLOT_NUM
#define BLE_SLOT_NUM 4
#endif
#endif

#ifdef ESB_ENABLE
#ifdef LSE_FREQ
#undef LSE_FREQ
#endif
#ifdef DCDC_ENABLE
#undef DCDC_ENABLE
#endif
#ifdef HAL_SLEEP
#undef HAL_SLEEP
#endif
#ifdef QMK_TASK_INTERVAL
#undef QMK_TASK_INTERVAL
#endif
#if ESB_ENABLE == 1
#define LSE_FREQ          32768
#define DCDC_ENABLE       1
#define HAL_SLEEP         1
#define QMK_TASK_INTERVAL 5
#elif ESB_ENABLE == 2
#define DCDC_ENABLE 0
#define HAL_SLEEP   0
#endif

// #ifndef HAL_SLEEP
// #define HAL_SLEEP 1
// #endif
// #ifndef QMK_TASK_INTERVAL
// #define QMK_TASK_INTERVAL QMK_TASK_INTERVAL_MAX
// #endif
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
#error "Illegal Fsys!"
#endif

#ifndef EXTRAKEY_ENABLE
#define EXTRAKEY_ENABLE
#endif

#ifndef SEND_STRING_ENABLE
#define SEND_STRING_ENABLE
#endif

#ifdef NKRO_ENABLE
// a special trick
#define PROTOCOL_LUFA
#define ENDPOINT_TOTAL_ENDPOINTS 8
#else
#ifdef FORCE_NKRO
#undef FORCE_NKRO
#endif
#endif

#ifdef LSE_FREQ
#define FREQ_RTC   LSE_FREQ
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
#define FREQ_RTC 32000
#if ESB_ENABLE == 2
#define CLK_OSC32K 0x81
#else
#define CLK_OSC32K 1
#endif
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
#define RGB_DI_PIN A14
#elif defined WS2812_DRIVER_PWM
#if WS2812_PWM_DRIVER == 1
#define RGB_DI_PIN A10
#elif WS2812_PWM_DRIVER == 2
#define RGB_DI_PIN A11
#endif
#endif

#define MACRO2STR_(_macro) #_macro
#define MACRO2STR(_macro)  MACRO2STR_(_macro)

// ms to 1 tick (625us)
#define SYS_TICK_MS(x) MS1_TO_SYSTEM_TIME(x)
// ms to 1.25ms (connection interval)
#define MS_UNIT_1_25(x) ((int)(x / 1.25))

enum {
    kbd_protocol_start = 0,
#ifdef USB_ENABLE
    kbd_protocol_usb,
#endif
#ifdef BLE_ENABLE
    kbd_protocol_ble,
#endif
#ifdef ESB_ENABLE
    kbd_protocol_esb,
#endif
    kbd_protocol_max,
};

#if BLE_SLOT_NUM > 8
#error "Too many BLE slots! Cap: 8"
#endif

_Static_assert(kbd_protocol_max > 1, "No interface enabled!");
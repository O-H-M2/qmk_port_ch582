/*
Copyright 2022 Huckies <https://github.com/Huckies>
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

#include "battery_measure.h"
#include "quantum.h"

#ifdef BATTERY_MEASURE_PIN

static volatile uint8_t last_percentage = 100;
static uint32_t last_measure = 0;

__attribute__((weak)) const uint16_t battery_map[] = {
    // 2704 3.654V 0%
    2704, 2710, 2716, 2722, 2728, 2734, 2739, 2745, 2751, 2757,
    2763, 2769, 2775, 2780, 2786, 2792, 2798, 2804, 2810, 2815,
    2821, 2827, 2833, 2839, 2845, 2851, 2856, 2862, 2868, 2874,
    2880, 2886, 2892, 2897, 2903, 2909, 2915, 2921, 2927, 2932,
    2938, 2944, 2950, 2956, 2962, 2968, 2973, 2979, 2985, 2991,
    2997, 3003, 3009, 3014, 3020, 3026, 3032, 3038, 3044, 3049,
    3055, 3061, 3067, 3073, 3079, 3085, 3090, 3096, 3102, 3108,
    3114, 3120, 3126, 3131, 3137, 3143, 3149, 3155, 3161, 3166,
    3172, 3178, 3184, 3190, 3196, 3202, 3207, 3213, 3219, 3225,
    3231, 3237, 3243, 3248, 3254, 3260, 3266, 3272, 3278, 3283
    // 3283 4.1V 100%
};

static inline void battery_config_channel(pin_t pin)
{
    switch (pin) {
        case A7:
            ADC_ChannelCfg(11);
            break;
        case A8:
            ADC_ChannelCfg(12);
            break;
        case A9:
            ADC_ChannelCfg(13);
            break;
        case A4:
            ADC_ChannelCfg(0);
            break;
        case A5:
            ADC_ChannelCfg(1);
            break;
        case A6:
            ADC_ChannelCfg(10);
            break;
        case A0:
            ADC_ChannelCfg(9);
            break;
        case A1:
            ADC_ChannelCfg(8);
            break;
        case A2:
            ADC_ChannelCfg(7);
            break;
        case A3:
            ADC_ChannelCfg(6);
            break;
        case A15:
            ADC_ChannelCfg(5);
            break;
        case A14:
            ADC_ChannelCfg(4);
            break;
        case A13:
            ADC_ChannelCfg(3);
            break;
        case A12:
            ADC_ChannelCfg(2);
            break;
    }
}

uint16_t battery_get_min()
{
    return battery_map[0];
}

uint16_t battery_get_max()
{
    return battery_map[BATTERY_MAP_SIZE - 1];
}

// just in case there is no gpio interrupt handler defined
__attribute__((weak)) __INTERRUPT __HIGH_CODE void GPIOA_IRQHandler()
{
    R16_PA_INT_IF = R16_PA_INT_IF;
    R16_PB_INT_IF = R16_PB_INT_IF;
}

__attribute__((weak)) __INTERRUPT __HIGH_CODE void GPIOB_IRQHandler()
{
    R16_PA_INT_IF = R16_PA_INT_IF;
    R16_PB_INT_IF = R16_PB_INT_IF;
}

__attribute__((noinline)) static void battery_critical_prerequisite()
{
#if __BUILDING_APP__
    shutdown_user();
#endif
    battery_critical_gpio_prerequisite();

    uint8_t temp = RB_WAKE_EV_MODE;

#ifdef POWER_DETECT_PIN
    temp |= RB_SLP_GPIO_WAKE;
    if (POWER_DETECT_PIN & 0x80000000) {
        PFIC_DisableIRQ(GPIO_A_IRQn);
        PFIC_EnableIRQ(GPIO_B_IRQn);
    } else {
        PFIC_EnableIRQ(GPIO_A_IRQn);
        PFIC_DisableIRQ(GPIO_B_IRQn);
    }
    setPinInputLow(POWER_DETECT_PIN);
    setPinInterruptRisingEdge(POWER_DETECT_PIN);
#endif
    do {
        sys_safe_access_enable();
        R8_SLP_WAKE_CTRL = temp;
        sys_safe_access_disable();
    } while (R8_SLP_WAKE_CTRL != temp);
}

__attribute__((noreturn)) __HIGH_CODE static void battery_handle_critical()
{
    battery_critical_prerequisite();

    uint8_t x32Kpw, x32Mpw;

    FLASH_ROM_SW_RESET();
    x32Kpw = R8_XT32K_TUNE;
    x32Mpw = R8_XT32M_TUNE;
    x32Mpw = (x32Mpw & 0xfc) | 0x03;     // 150%额定电流
    if (R16_RTC_CNT_32K > 0x3fff) {      // 超过500ms
        x32Kpw = (x32Kpw & 0xfc) | 0x01; // LSE驱动电流降低到额定电流
    }

    WAIT_FOR_DBG;

    sys_safe_access_enable();
    R8_BAT_DET_CTRL = 0;
    sys_safe_access_enable();
    R8_XT32K_TUNE = x32Kpw;
    R8_XT32M_TUNE = x32Mpw;
    sys_safe_access_disable();
    SetSysClock(CLK_SOURCE_HSE_6_4MHz);

    PFIC->SCTLR |= (1 << 2); //deep sleep

    sys_safe_access_enable();
    R8_SLP_POWER_CTRL = RB_RAM_RET_LV | 0x01;
    sys_safe_access_enable();
    R16_POWER_PLAN = RB_PWR_PLAN_EN | RB_PWR_MUST_0010;
    __WFI();
    __nop();
    __nop();
    FLASH_ROM_SW_RESET();
    sys_safe_access_enable();
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET;
    sys_safe_access_disable();

    __builtin_unreachable();
}

__attribute__((weak)) void battery_init()
{
    switch (BATTERY_MEASURE_PIN) {
        case A0:
        case A1:
        case A2:
        case A3:
        case A12:
        case A13:
        case A14:
        case A15:
            setPinInput(BATTERY_MEASURE_PIN);
            break;
        default:
            break;
    }
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_2);
}

__attribute__((weak)) uint16_t battery_measure()
{
    uint16_t adcBuff[15];
    int16_t RoughCalib_Value = ADC_DataCalib_Rough();

    battery_config_channel(BATTERY_MEASURE_PIN);
    for (uint8_t i = 0; i < 15; i++) {
        adcBuff[i] = ADC_ExcutSingleConver();
        if ((adcBuff[i] + RoughCalib_Value) > 0) {
            adcBuff[i] += RoughCalib_Value;
        } else {
            adcBuff[i] = 0;
        }
    }
    R8_ADC_CFG &= ~(RB_ADC_BUF_EN | RB_ADC_POWER_ON);

    uint16_t adc_data = 0;

    for (uint8_t i = 0; i < 10; i++) {
        adc_data += adcBuff[i + 5];
    }

    return adc_data;
}

__attribute__((weak)) uint8_t battery_calculate(uint16_t adcVal)
{
    uint8_t percent;

    if ((adcVal < battery_map[0] * 10)
#ifdef POWER_DETECT_PIN
        && !readPin(POWER_DETECT_PIN)
#endif
    ) {
        PRINT("Battery level critical.\n");
        battery_handle_critical();
    }

    for (uint32_t i = 1; i < BATTERY_MAP_SIZE; i++) {
        if (adcVal < battery_map[i] * 10) {
            percent = (uint8_t)(i * 100 / BATTERY_MAP_SIZE);
            goto done;
        }
    }
    percent = 100;

done:
#ifdef POWER_DETECT_PIN
    if (!readPin(POWER_DETECT_PIN)) {
        // with cable unpluged, battery level should get lower only
        if (percent < last_percentage) {
            last_percentage = percent;
        }
    } else {
        last_percentage = percent;
    }
#else
    last_percentage = percent;
#endif
#if __BUILDING_APP__
    last_measure = timer_read32();
#endif

    return last_percentage;
}

uint8_t battery_get_last_percentage()
{
    return last_percentage;
}

uint32_t battery_get_last_measure()
{
    return last_measure;
}

#endif

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

static const uint16_t battery_map[100] = {
    2515, 2528, 2541, 2554, 2567, 2580, 2593, 2606, 2619, 2632,
    2645, 2658, 2672, 2685, 2698, 2711, 2724, 2737, 2750, 2763,
    2776, 2789, 2802, 2816, 2829, 2842, 2855, 2868, 2881, 2894,
    2907, 2920, 2933, 2946, 2960, 2973, 2986, 2999, 3012, 3025,
    3038, 3051, 3064, 3077, 3090, 3104, 3117, 3130, 3143, 3156,
    3169, 3182, 3195, 3208, 3221, 3234, 3248, 3261, 3274, 3287,
    3300, 3313, 3326, 3339, 3352, 3365, 3378, 3392, 3405, 3418,
    3431, 3444, 3457, 3470, 3483, 3496, 3509, 3522, 3536, 3549,
    3562, 3575, 3588, 3601, 3614, 3627, 3640, 3653, 3666, 3680,
    3693, 3706, 3719, 3732, 3745, 3758, 3771, 3784, 3797, 3810
};

static bool battery_indicator_enable = 0;
static uint16_t battery_indicator_timer = 0;

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

// static int16_t battery_measure_calibrate()
// {
//     uint32_t sum = 0;
//     uint8_t ch = R8_ADC_CHANNEL;
//     uint8_t ctrl = R8_ADC_CFG;

//     R8_ADC_CFG = 0;
//     battery_config_channel(BATTERY_MEASURE_CALIBRATION_PIN);

//     R8_ADC_CFG |= RB_ADC_OFS_TEST | RB_ADC_POWER_ON | (2 << 4); // 进入测试模式
//     R8_ADC_CONVERT = RB_ADC_START;
//     while (R8_ADC_CONVERT & RB_ADC_START) {
//         ;
//     }
//     for (uint16_t i = 0; i < 16; i++) {
//         R8_ADC_CONVERT = RB_ADC_START;
//         while (R8_ADC_CONVERT & RB_ADC_START) {
//             ;
//         }
//         sum += (~R16_ADC_DATA) & RB_ADC_DATA;
//     }
//     sum = (sum + 8) >> 4;
//     R8_ADC_CFG &= ~RB_ADC_OFS_TEST; // 关闭测试模式

//     R8_ADC_CHANNEL = ch;
//     R8_ADC_CFG = ctrl;
//     return (2048 - sum);
// }

__attribute__((weak)) uint16_t battery_get_min()
{
    return battery_map[0];
}

__attribute__((weak)) uint16_t battery_get_max()
{
    return battery_map[sizeof(battery_map) - 1];
}

__attribute__((weak)) void battery_init()
{
    setPinInput(BATTERY_MEASURE_PIN);
    ADC_ExtSingleChSampInit(SampleFreq_3_2, ADC_PGA_2);
}

__attribute__((weak)) uint16_t battery_measure()
{
    uint16_t abcBuff[15];
    int16_t RoughCalib_Value = ADC_DataCalib_Rough();

    battery_config_channel(BATTERY_MEASURE_PIN);
    for (uint8_t i = 0; i < 15; i++) {
        abcBuff[i] = ADC_ExcutSingleConver() + RoughCalib_Value;
    }
    R8_ADC_CFG &= ~(RB_ADC_BUF_EN | RB_ADC_POWER_ON);

    uint16_t adc_data = 0;

    for (uint8_t i = 0; i < 10; i++) {
        adc_data += abcBuff[i + 5];
    }

    return adc_data;
}

__attribute__((weak)) uint8_t battery_calculate(uint16_t adcVal)
{
    for (uint8_t i = 0; i < 100; i++) {
        if (adcVal < battery_map[i] * 10) {
            return i;
        }
    }
    return battery_get_max();
}

void battery_indicator_toggle(bool status)
{
    if (status) {
        battery_indicator_enable = true;
        battery_indicator_timer = timer_read();
    } else {
        battery_indicator_enable = false;
    }
}

bool battery_indicator_state()
{
    return battery_indicator_enable;
}

bool battery_indicator_timeout()
{
    return (timer_elapsed(battery_indicator_timer) > BATTERY_INDICATOR_TIMEOUT);
}
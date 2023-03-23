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

#include "gpio.h"

__HIGH_CODE void gpio_strap()
{
    pin_t pin_a = GPIO_Pin_All & 0x7FFFFFFF, pin_b = GPIO_Pin_All;

#if defined LSE_ENABLE && LSE_ENABLE
    pin_a &= ~bX32KI;
    pin_a &= ~bX32KO;
#endif
#ifdef WS2812
    if (WS2812_EN_PIN & 0x80000000) {
        pin_b &= ~(WS2812_EN_PIN & 0x7FFFFFFF);
    } else {
        pin_a &= ~(WS2812_EN_PIN & 0x7FFFFFFF);
    }
#elif defined AW20216
    if (DRIVER_1_CS & 0x80000000) {
        pin_b &= ~(DRIVER_1_CS & 0x7FFFFFFF);
    } else {
        pin_a &= ~(DRIVER_1_CS & 0x7FFFFFFF);
    }
    // if (DRIVER_1_EN & 0x80000000) {
    //     pin_b &= ~(DRIVER_1_EN & 0x7FFFFFFF);
    // } else {
    //     pin_a &= ~(DRIVER_1_EN & 0x7FFFFFFF);
    // }
    setPinInputHigh(DRIVER_1_CS);
#ifdef DRIVER_2_EN
    if (DRIVER_2_CS & 0x80000000) {
        pin_b &= ~(DRIVER_2_CS & 0x7FFFFFFF);
    } else {
        pin_a &= ~(DRIVER_2_CS & 0x7FFFFFFF);
    }
    // if (DRIVER_2_EN & 0x80000000) {
    //     pin_b &= ~(DRIVER_2_EN & 0x7FFFFFFF);
    // } else {
    //     pin_a &= ~(DRIVER_2_EN & 0x7FFFFFFF);
    // }
    setPinInputHigh(DRIVER_2_CS);
#endif
#endif
#ifdef I2C_MASTER_ENABLE
#ifdef I2C_IO_REMAPPING
    pin_b &= ~((B20 | B21) & 0x7FFFFFFF);
    setPinInputHigh(B20 | B21);
#else
    pin_b &= ~((B12 | B13) & 0x7FFFFFFF);
    setPinInputHigh(B12 | B13);
#endif
#endif
    setPinInputLow(pin_a);
    setPinInputLow(pin_b);
#ifdef BATTERY_MEASURE_PIN
    setPinInputHigh(BATTERY_MEASURE_PIN);
#endif
#ifdef POWER_DETECT_PIN
    setPinInput(POWER_DETECT_PIN);
#endif
}

__attribute__((noinline)) __attribute__((weak)) void battery_critical_gpio_prerequisite()
{
    pin_t pin_a = GPIO_Pin_All & 0x7FFFFFFF, pin_b = GPIO_Pin_All;

#if defined LSE_ENABLE && LSE_ENABLE
    pin_a &= ~bX32KI;
    pin_a &= ~bX32KO;
#endif
#ifdef WS2812
    if (WS2812_EN_PIN & 0x80000000) {
        pin_b &= ~(WS2812_EN_PIN & 0x7FFFFFFF);
    } else {
        pin_a &= ~(WS2812_EN_PIN & 0x7FFFFFFF);
    }
#elif defined AW20216
    if (DRIVER_1_EN & 0x80000000) {
        pin_b &= ~(DRIVER_1_EN & 0x7FFFFFFF);
    } else {
        pin_a &= ~(DRIVER_1_EN & 0x7FFFFFFF);
    }
#ifdef DRIVER_2_EN
    if (DRIVER_2_EN & 0x80000000) {
        pin_b &= ~(DRIVER_2_EN & 0x7FFFFFFF);
    } else {
        pin_a &= ~(DRIVER_2_EN & 0x7FFFFFFF);
    }
#endif
#endif
#ifdef I2C_MASTER_ENABLE
#ifdef I2C_IO_REMAPPING
    pin_b &= ~((B20 | B21) & 0x7FFFFFFF);
    setPinInputHigh(B20 | B21);
#else
    pin_b &= ~((B12 | B13) & 0x7FFFFFFF);
    setPinInputHigh(B12 | B13);
#endif
#endif
#ifdef BATTERY_MEASURE_PIN
    if (BATTERY_MEASURE_PIN & 0x80000000) {
        pin_b &= ~(BATTERY_MEASURE_PIN & 0x7FFFFFFF);
    } else {
        pin_a &= ~(BATTERY_MEASURE_PIN & 0x7FFFFFFF);
    }
#endif
    setPinInputLow(pin_a);
    setPinInputLow(pin_b);
}

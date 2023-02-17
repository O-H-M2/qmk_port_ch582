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
    setPinInputHigh(DRIVER_1_CS);
#ifdef DRIVER_2_EN
    if (DRIVER_2_CS & 0x80000000) {
        pin_b &= ~(DRIVER_2_CS & 0x7FFFFFFF);
    } else {
        pin_a &= ~(DRIVER_2_CS & 0x7FFFFFFF);
    }
    setPinInputHigh(DRIVER_2_CS);
#endif
#endif
    pin_b &= ~bUDP;
    pin_b &= ~bUDM;
    pin_b &= ~bU2DP;
    pin_b &= ~bU2DM;
    setPinInputLow(pin_a);
    setPinInputLow(pin_b);
#ifdef BATTERY_MEASURE_PIN
    setPinInputHigh(BATTERY_MEASURE_PIN);
#endif
#ifdef POWER_DETECT_PIN
    setPinInput(POWER_DETECT_PIN);
#endif
}

__attribute__((noinline)) void battery_critical_gpio_prerequisite()
{
    pin_t pin_a_mask = GPIO_Pin_All, pin_b_mask = GPIO_Pin_All;
    pin_t pin_a_status = GPIOA_ReadPort(), pin_b_status = GPIOB_ReadPort();

#if defined LSE_ENABLE && LSE_ENABLE
    pin_a_mask &= ~bX32KI;
    pin_a_mask &= ~bX32KO;
#endif
#ifdef BATTERY_MEASURE_PIN
    if (BATTERY_MEASURE_PIN & 0x80000000) {
        pin_b_mask &= ~(BATTERY_MEASURE_PIN & 0x7FFFFFFF);
    } else {
        pin_a_mask &= ~(BATTERY_MEASURE_PIN & 0x7FFFFFFF);
    }
#endif
    for (uint8_t i = 0; i < 32; i++) {
        pin_t pin = (1UL << i);

        if (pin & pin_a_mask) {
            if (pin & pin_a_status) {
                setPinInputHigh(pin);
            } else {
                setPinInputLow(pin);
            }
        }
        if (pin & pin_b_mask) {
            if (pin & pin_b_status) {
                setPinInputHigh(pin | 0x80000000);
            } else {
                setPinInputLow(pin | 0x80000000);
            }
        }
    }
}

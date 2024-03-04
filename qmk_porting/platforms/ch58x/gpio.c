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

static gpio_pin_record_t pin_record = {};

bool gpio_record_pin_config(pin_t pin)
{
    if (pin_record.pin != NO_PIN) {
        return false;
    }

    pin_record.pin = pin;

    if (pin_record.pin & 0x80000000) {
        pin_record.pd_drv = GPIO_FIELD_TO_BIT(R32_PB_PD_DRV & (pin & 0x7FFFFFFF));
        pin_record.pu = GPIO_FIELD_TO_BIT(R32_PB_PU & (pin & 0x7FFFFFFF));
        pin_record.dir = GPIO_FIELD_TO_BIT(R32_PB_DIR & (pin & 0x7FFFFFFF));
    } else {
        pin_record.pd_drv = GPIO_FIELD_TO_BIT(R32_PA_PD_DRV & pin);
        pin_record.pu = GPIO_FIELD_TO_BIT(R32_PA_PU & pin);
        pin_record.dir = GPIO_FIELD_TO_BIT(R32_PA_DIR & pin);
    }

    return true;
}

void gpio_recover_pin_config()
{
    if (pin_record.pin == NO_PIN) {
        return;
    }

    if (pin_record.pin & 0x80000000) {
        if (pin_record.pd_drv) {
            R32_PB_PD_DRV |= pin_record.pin & 0x7FFFFFFF;
        } else {
            R32_PB_PD_DRV &= ~(pin_record.pin & 0x7FFFFFFF);
        }
        if (pin_record.pu) {
            R32_PB_PU |= pin_record.pin & 0x7FFFFFFF;
        } else {
            R32_PB_PU &= ~(pin_record.pin & 0x7FFFFFFF);
        }
        if (pin_record.dir) {
            R32_PB_DIR |= pin_record.pin & 0x7FFFFFFF;
        } else {
            R32_PB_DIR &= ~(pin_record.pin & 0x7FFFFFFF);
        }
    } else {
        if (pin_record.pd_drv) {
            R32_PA_PD_DRV |= pin_record.pin;
        } else {
            R32_PA_PD_DRV &= ~pin_record.pin;
        }
        if (pin_record.pu) {
            R32_PA_PU |= pin_record.pin;
        } else {
            R32_PA_PU &= ~pin_record.pin;
        }
        if (pin_record.dir) {
            R32_PA_DIR |= pin_record.pin;
        } else {
            R32_PA_DIR &= ~pin_record.pin;
        }
    }

    pin_record.pin = NO_PIN;
}

__HIGH_CODE void gpio_strap()
{
    pin_t pin_a = GPIO_Pin_All & 0x7FFFFFFF, pin_b = GPIO_Pin_All;

#if defined LSE_ENABLE && LSE_ENABLE
    pin_a &= ~bX32KI;
    pin_a &= ~bX32KO;
#endif
#ifdef POWER_DETECT_PIN
    gpio_set_pin_input_low(POWER_DETECT_PIN);
#endif
#if defined WS2812 && defined WS2812_EN_PIN
    if (WS2812_EN_PIN & 0x80000000) {
        pin_b &= ~(WS2812_EN_PIN & 0x7FFFFFFF);
    } else {
        pin_a &= ~(WS2812_EN_PIN & 0x7FFFFFFF);
    }
#elif defined AW20216S
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
    gpio_set_pin_input_high(DRIVER_1_CS);
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
    gpio_set_pin_input_high(DRIVER_2_CS);
#endif
#endif
#ifdef I2C_MASTER_ENABLE
#ifdef I2C_IO_REMAPPING
    pin_b &= ~((B20 | B21) & 0x7FFFFFFF);
    gpio_set_pin_input_high(B20 | B21);
#else
    pin_b &= ~((B12 | B13) & 0x7FFFFFFF);
    gpio_set_pin_input_high(B12 | B13);
#endif
#endif
#ifdef UART_ENABLE
    pin_b &= ~((B4 | B7) & 0x7FFFFFFF);
    gpio_set_pin_input_high(B4 | B7);
#endif
    gpio_set_pin_input_low(pin_a);
    gpio_set_pin_input_low(pin_b);
#ifdef BATTERY_MEASURE_PIN
    gpio_set_pin_input_high(BATTERY_MEASURE_PIN);
#endif
#ifdef POWER_DETECT_PIN
    gpio_set_pin_input(POWER_DETECT_PIN);
#endif
}

__attribute__((noinline)) __attribute__((weak)) void battery_critical_gpio_prerequisite()
{
    pin_t pin_a = GPIO_Pin_All & 0x7FFFFFFF, pin_b = GPIO_Pin_All;

#if defined LSE_ENABLE && LSE_ENABLE
    pin_a &= ~bX32KI;
    pin_a &= ~bX32KO;
#endif
#if defined WS2812 && defined WS2812_EN_PIN
    if (WS2812_EN_PIN & 0x80000000) {
        pin_b &= ~(WS2812_EN_PIN & 0x7FFFFFFF);
    } else {
        pin_a &= ~(WS2812_EN_PIN & 0x7FFFFFFF);
    }
#elif defined AW20216S
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
    gpio_set_pin_input_high(B20 | B21);
#else
    pin_b &= ~((B12 | B13) & 0x7FFFFFFF);
    gpio_set_pin_input_high(B12 | B13);
#endif
#endif
#ifdef UART_ENABLE
    pin_b &= ~((B4 | B7) & 0x7FFFFFFF);
    gpio_set_pin_input_high(B4 | B7);
#endif
#ifdef BATTERY_MEASURE_PIN
    if (BATTERY_MEASURE_PIN & 0x80000000) {
        pin_b &= ~(BATTERY_MEASURE_PIN & 0x7FFFFFFF);
    } else {
        pin_a &= ~(BATTERY_MEASURE_PIN & 0x7FFFFFFF);
    }
#endif
    gpio_set_pin_input_low(pin_a);
    gpio_set_pin_input_low(pin_b);
}

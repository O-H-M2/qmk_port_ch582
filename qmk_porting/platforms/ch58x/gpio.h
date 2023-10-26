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

#include <stdbool.h>
#include "pin_defs.h"

typedef uint32_t pin_t;
typedef struct _gpio_pin_record_t {
    pin_t pin;
    uint8_t pd_drv : 1;
    uint8_t pu     : 1;
    uint8_t dir    : 1;
} gpio_pin_record_t;

#define GPIO_FIELD_TO_BIT(_field) \
    ((_field) ? 1 : 0)

#define setPinInput(pin)                          \
    do {                                          \
        if ((pin)&0x80000000) {                   \
            R32_PB_PD_DRV &= ~((pin)&0x7FFFFFFF); \
            R32_PB_PU &= ~((pin)&0x7FFFFFFF);     \
            R32_PB_DIR &= ~((pin)&0x7FFFFFFF);    \
        } else {                                  \
            R32_PA_PD_DRV &= ~(pin);              \
            R32_PA_PU &= ~(pin);                  \
            R32_PA_DIR &= ~(pin);                 \
        }                                         \
    } while (0);
#define setPinInputHigh(pin)                      \
    do {                                          \
        if ((pin)&0x80000000) {                   \
            R32_PB_PD_DRV &= ~((pin)&0x7FFFFFFF); \
            R32_PB_PU |= ((pin)&0x7FFFFFFF);      \
            R32_PB_DIR &= ~((pin)&0x7FFFFFFF);    \
        } else {                                  \
            R32_PA_PD_DRV &= ~(pin);              \
            R32_PA_PU |= (pin);                   \
            R32_PA_DIR &= ~(pin);                 \
        }                                         \
    } while (0);
#define setPinInputLow(pin)                      \
    do {                                         \
        if ((pin)&0x80000000) {                  \
            R32_PB_PD_DRV |= ((pin)&0x7FFFFFFF); \
            R32_PB_PU &= ~((pin)&0x7FFFFFFF);    \
            R32_PB_DIR &= ~((pin)&0x7FFFFFFF);   \
        } else {                                 \
            R32_PA_PD_DRV |= (pin);              \
            R32_PA_PU &= ~(pin);                 \
            R32_PA_DIR &= ~(pin);                \
        }                                        \
    } while (0);
#define setPinOutputPushPull(pin)                 \
    do {                                          \
        if ((pin)&0x80000000) {                   \
            R32_PB_PD_DRV &= ~((pin)&0x7FFFFFFF); \
            R32_PB_DIR |= ((pin)&0x7FFFFFFF);     \
        } else {                                  \
            R32_PA_PD_DRV &= ~(pin);              \
            R32_PA_DIR |= (pin);                  \
        }                                         \
    } while (0);
#define setPinOutputOpenDrain(pin) _Static_assert(0, "WCH platform does not implement an open-drain output")
#define setPinOutput(pin)          setPinOutputPushPull(pin)

#define writePinHigh(pin)    (((pin)&0x80000000) ? GPIOB_SetBits((pin)&0x7FFFFFFF) : GPIOA_SetBits(pin))
#define writePinLow(pin)     (((pin)&0x80000000) ? GPIOB_ResetBits((pin)&0x7FFFFFFF) : GPIOA_ResetBits(pin))
#define writePin(pin, level) ((level) ? writePinHigh(pin) : writePinLow(pin))

#define readPin(pin)   (GPIO_FIELD_TO_BIT(((pin)&0x80000000) ? GPIOB_ReadPortPin((pin)&0x7FFFFFFF) : GPIOA_ReadPortPin(pin)))
#define togglePin(pin) (((pin)&0x80000000) ? GPIOB_InverseBits((pin)&0x7FFFFFFF) : GPIOA_InverseBits(pin))

/* self-defined functions */
#define setPinInterruptRisingEdge(pin)                                                                       \
    do {                                                                                                     \
        if ((pin)&0x80000000) {                                                                              \
            pin_t pin_int = ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14); \
                                                                                                             \
            R16_PB_INT_MODE |= pin_int;                                                                      \
            R32_PB_OUT |= ((pin)&0x7FFFFFFF);                                                                \
            R16_PB_INT_IF = pin_int;                                                                         \
            R16_PB_INT_EN |= pin_int;                                                                        \
        } else {                                                                                             \
            R16_PA_INT_MODE |= (pin);                                                                        \
            R32_PA_OUT |= (pin);                                                                             \
            R16_PA_INT_IF = (uint16_t)(pin);                                                                 \
            R16_PA_INT_EN |= (uint16_t)(pin);                                                                \
        }                                                                                                    \
    } while (0);
#define setPinInterruptFallingEdge(pin)                                                                      \
    do {                                                                                                     \
        if ((pin)&0x80000000) {                                                                              \
            pin_t pin_int = ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14); \
                                                                                                             \
            R16_PB_INT_MODE |= pin_int;                                                                      \
            R32_PB_CLR |= ((pin)&0x7FFFFFFF);                                                                \
            R16_PB_INT_IF = pin_int;                                                                         \
            R16_PB_INT_EN |= pin_int;                                                                        \
        } else {                                                                                             \
            R16_PA_INT_MODE |= (pin);                                                                        \
            R32_PA_CLR |= (pin);                                                                             \
            R16_PA_INT_IF = (uint16_t)(pin);                                                                 \
            R16_PA_INT_EN |= (uint16_t)(pin);                                                                \
        }                                                                                                    \
    } while (0);
#define setPinInterruptHighLevel(pin)                                                                        \
    do {                                                                                                     \
        if ((pin)&0x80000000) {                                                                              \
            pin_t pin_int = ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14); \
                                                                                                             \
            R16_PB_INT_MODE &= ~(pin_int);                                                                   \
            R32_PB_OUT |= ((pin)&0x7FFFFFFF);                                                                \
            R16_PB_INT_IF = pin_int;                                                                         \
            R16_PB_INT_EN |= pin_int;                                                                        \
        } else {                                                                                             \
            R16_PA_INT_MODE &= ~(pin);                                                                       \
            R32_PA_OUT |= (pin);                                                                             \
            R16_PA_INT_IF = (uint16_t)(pin);                                                                 \
            R16_PA_INT_EN |= (uint16_t)(pin);                                                                \
        }                                                                                                    \
    } while (0);
#define setPinInterruptLowLevel(pin)                                                                         \
    do {                                                                                                     \
        if ((pin)&0x80000000) {                                                                              \
            pin_t pin_int = ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14); \
                                                                                                             \
            R16_PB_INT_MODE &= ~(pin_int);                                                                   \
            R32_PB_CLR |= ((pin)&0x7FFFFFFF);                                                                \
            R16_PB_INT_IF = pin_int;                                                                         \
            R16_PB_INT_EN |= pin_int;                                                                        \
        } else {                                                                                             \
            R16_PA_INT_MODE &= ~(pin);                                                                       \
            R32_PA_CLR |= (pin);                                                                             \
            R16_PA_INT_IF = (uint16_t)(pin);                                                                 \
            R16_PA_INT_EN |= (uint16_t)(pin);                                                                \
        }                                                                                                    \
    } while (0);

bool gpio_record_pin_config(pin_t pin);
void gpio_recover_pin_config();
void gpio_strap();
void battery_critical_gpio_prerequisite();

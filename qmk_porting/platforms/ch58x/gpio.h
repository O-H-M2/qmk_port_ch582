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

#include "pin_defs.h"

typedef uint32_t pin_t;

#define setPinInput(pin)                                                                       \
    (((pin)&0x80000000) ? (R32_PB_PD_DRV &= ~((pin)&0x7FFFFFFF)) : (R32_PA_PD_DRV &= ~(pin))); \
    (((pin)&0x80000000) ? (R32_PB_PU &= ~((pin)&0x7FFFFFFF)) : (R32_PA_PU &= ~(pin)));         \
    (((pin)&0x80000000) ? (R32_PB_DIR &= ~((pin)&0x7FFFFFFF)) : (R32_PA_DIR &= ~(pin)));
#define setPinInputHigh(pin)                                                                   \
    (((pin)&0x80000000) ? (R32_PB_PD_DRV &= ~((pin)&0x7FFFFFFF)) : (R32_PA_PD_DRV &= ~(pin))); \
    (((pin)&0x80000000) ? (R32_PB_PU |= ((pin)&0x7FFFFFFF)) : (R32_PA_PU |= (pin)));           \
    (((pin)&0x80000000) ? (R32_PB_DIR &= ~((pin)&0x7FFFFFFF)) : (R32_PA_DIR &= ~(pin)));
#define setPinInputLow(pin)                                                                  \
    (((pin)&0x80000000) ? (R32_PB_PD_DRV |= ((pin)&0x7FFFFFFF)) : (R32_PA_PD_DRV |= (pin))); \
    (((pin)&0x80000000) ? (R32_PB_PU &= ~((pin)&0x7FFFFFFF)) : (R32_PA_PU &= ~(pin)));       \
    (((pin)&0x80000000) ? (R32_PB_DIR &= ~((pin)&0x7FFFFFFF)) : (R32_PA_DIR &= ~(pin)));
#define setPinOutputPushPull(pin)                                                              \
    (((pin)&0x80000000) ? (R32_PB_PD_DRV &= ~((pin)&0x7FFFFFFF)) : (R32_PA_PD_DRV &= ~(pin))); \
    (((pin)&0x80000000) ? (R32_PB_DIR |= ((pin)&0x7FFFFFFF)) : (R32_PA_DIR |= (pin)));
#define setPinOutputOpenDrain(pin) _Static_assert(0, "WCH platform does not implement an open-drain output")
#define setPinOutput(pin)          setPinOutputPushPull((pin))

#define writePinHigh(pin)    (((pin)&0x80000000) ? GPIOB_SetBits((pin)&0x7FFFFFFF) : GPIOA_SetBits((pin)))
#define writePinLow(pin)     (((pin)&0x80000000) ? GPIOB_ResetBits((pin)&0x7FFFFFFF) : GPIOA_ResetBits((pin)))
#define writePin(pin, level) ((level) ? writePinHigh((pin)) : writePinLow((pin)))

#define readPin(pin)   ((((pin)&0x80000000) ? GPIOB_ReadPortPin((pin)&0x7FFFFFFF) : GPIOA_ReadPortPin((pin))) ? 1 : 0)
#define togglePin(pin) (((pin)&0x80000000) ? GPIOB_InverseBits((pin)&0x7FFFFFFF) : GPIOA_InverseBits((pin)))

/* self-defined functions */
#define setPinInterruptRisingEdge(pin)                                                                                                                        \
    (((pin)&0x80000000) ? (R16_PB_INT_MODE |= ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_MODE |= (pin))); \
    (((pin)&0x80000000) ? (R32_PB_OUT |= ((pin)&0x7FFFFFFF)) : (R32_PA_OUT |= (pin)));                                                                        \
    (((pin)&0x80000000) ? (R16_PB_INT_IF = ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_IF = (pin)));       \
    (((pin)&0x80000000) ? (R16_PB_INT_EN |= ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_EN |= (pin)));

#define setPinInterruptFallingEdge(pin)                                                                                                                       \
    (((pin)&0x80000000) ? (R16_PB_INT_MODE |= ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_MODE |= (pin))); \
    (((pin)&0x80000000) ? (R32_PB_CLR |= ((pin)&0x7FFFFFFF)) : (R32_PA_CLR |= (pin)));                                                                        \
    (((pin)&0x80000000) ? (R16_PB_INT_IF = ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_IF = (pin)));       \
    (((pin)&0x80000000) ? (R16_PB_INT_EN |= ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_EN |= (pin)));

#define setPinInterruptHighLevel(pin)                                                                                                                             \
    (((pin)&0x80000000) ? (R16_PB_INT_MODE &= ~(((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14))) : (R16_PA_INT_MODE &= ~(pin))); \
    (((pin)&0x80000000) ? (R32_PB_OUT |= ((pin)&0x7FFFFFFF)) : (R32_PA_OUT |= (pin)));                                                                            \
    (((pin)&0x80000000) ? (R16_PB_INT_IF = ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_IF = (pin)));           \
    (((pin)&0x80000000) ? (R16_PB_INT_EN |= ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_EN |= (pin)));

#define setPinInterruptLowLevel(pin)                                                                                                                              \
    (((pin)&0x80000000) ? (R16_PB_INT_MODE &= ~(((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14))) : (R16_PA_INT_MODE &= ~(pin))); \
    (((pin)&0x80000000) ? (R32_PB_CLR |= ((pin)&0x7FFFFFFF)) : (R32_PA_CLR |= (pin)));                                                                            \
    (((pin)&0x80000000) ? (R16_PB_INT_IF = ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_IF = (pin)));           \
    (((pin)&0x80000000) ? (R16_PB_INT_EN |= ((pin)&0x7FFFFFFF) | ((((pin)&0x7FFFFFFF) & (GPIO_Pin_22 | GPIO_Pin_23)) >> 14)) : (R16_PA_INT_EN |= (pin)));

void gpio_strap();
void battery_critical_gpio_prerequisite();

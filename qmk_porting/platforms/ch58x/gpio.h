/* Copyright 2021 QMK
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "CH58x_common.h"
#include "core_riscv.h"
#include "pin_defs.h"

typedef uint8_t pin_t;

/* Operation of GPIO by pin. */
/* GPIOA 0 ~ 15
   GPIOB 16 ~ 39 */

#define determineGPIONum(pin)     ((uint32_t)(0x01 << (pin)))
#define GPIOModeConfig(pin, mode) ((pin >= A0) && (pin <= A15)) ? GPIOA_ModeCfg(determineGPIONum(pin), mode) : (((pin >= B0) && (pin <= B23)) ? GPIOB_ModeCfg(determineGPIONum(pin - 16), mode) : 0)

#define setPinInput(pin)           GPIOModeConfig(pin, GPIO_ModeIN_Floating)
#define setPinInputHigh(pin)       GPIOModeConfig(pin, GPIO_ModeIN_PU)
#define setPinInputLow(pin)        GPIOModeConfig(pin, GPIO_ModeIN_PD)
#define setPinOutputPushPull(pin)  GPIOModeConfig(pin, GPIO_ModeOut_PP_5mA)
#define setPinOutputOpenDrain(pin) _Static_assert(0, "WCH platform does not implement an open-drain output")
#define setPinOutput(pin)          setPinOutputPushPull(pin)

#define writePinHigh(pin)    ((pin >= A0) && (pin <= A15)) ? GPIOA_SetBits(determineGPIONum(pin)) : (((pin >= B0) && (pin <= B23)) ? GPIOB_SetBits(determineGPIONum(pin - 16)) : 0)
#define writePinLow(pin)     ((pin >= A0) && (pin <= A15)) ? GPIOA_ResetBits(determineGPIONum(pin)) : (((pin >= B0) && (pin <= B23)) ? GPIOB_ResetBits(determineGPIONum(pin - 16)) : 0)
#define writePin(pin, level) ((level) ? writePinHigh(pin) : writePinLow(pin))

#define readPin(pin)   ((pin >= A0) && (pin <= A15)) ? GPIOA_ReadPortPin(determineGPIONum(pin)) : (((pin >= B0) && (pin <= B23)) ? GPIOB_ReadPortPin(determineGPIONum(pin - 16)) : 0)
#define togglePin(pin) ((pin >= A0) && (pin <= A15)) ? GPIOA_InverseBits(determineGPIONum(pin)) : (((pin >= B0) && (pin <= B23)) ? GPIOB_InverseBits(determineGPIONum(pin - 16)) : 0)
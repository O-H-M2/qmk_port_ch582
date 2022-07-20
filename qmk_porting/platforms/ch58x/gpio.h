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

#define determineGPIOGroup(pin) ((pin >= 0) && (pin <= 15)) ? true : false

#define setPinInput(pin)           determineGPIOGroup(pin) ? GPIOA_ModeCfg(pin, GPIO_ModeIN_Floating) : GPIOB_ModeCfg(pin - 16, GPIO_ModeIN_Floating)
#define setPinInputHigh(pin)       determineGPIOGroup(pin) ? GPIOA_ModeCfg(pin, GPIO_ModeIN_PU) : GPIOB_ModeCfg(pin - 16, GPIO_ModeIN_PU)
#define setPinInputLow(pin)        determineGPIOGroup(pin) ? GPIOA_ModeCfg(pin, GPIO_ModeIN_PD) : GPIOB_ModeCfg(pin - 16, GPIO_ModeIN_PD)
#define setPinOutputPushPull(pin)  determineGPIOGroup(pin) ? GPIOA_ModeCfg(pin, GPIO_ModeOut_PP_5mA) : GPIOB_ModeCfg(pin - 16, GPIO_ModeOut_PP_5mA)
#define setPinOutputOpenDrain(pin) _Static_assert(0, "WCH platform does not implement an open-drain output")
#define setPinOutput(pin)          setPinOutputPushPull(pin)

#define writePinHigh(pin)    determineGPIOGroup(pin) ? GPIOA_SetBits(pin) : GPIOB_SetBits(pin - 16)
#define writePinLow(pin)     determineGPIOGroup(pin) ? GPIOA_ResetBits(pin) : GPIOB_ResetBits(pin - 16)
#define writePin(pin, level) ((level) ? writePinHigh(pin) : writePinLow(pin))

#define readPin(pin) determineGPIOGroup(pin) ? GPIOA_ReadPortPin(pin) : GPIOB_ReadPortPin(pin - 16)

#define togglePin(pin) determineGPIOGroup(pin) ? GPIOA_InverseBits(pin) : GPIOB_InverseBits(pin - 16)

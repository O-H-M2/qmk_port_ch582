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

#define GPIOModeConfig(pin, mode) ((pin & 0x80000000) ? GPIOB_ModeCfg(pin & 0x7FFFFFFF, mode) : (GPIOA_ModeCfg(pin & 0x7FFFFFFF, mode)))

#define setPinInput(pin)           GPIOModeConfig(pin, GPIO_ModeIN_Floating)
#define setPinInputHigh(pin)       GPIOModeConfig(pin, GPIO_ModeIN_PU)
#define setPinInputLow(pin)        GPIOModeConfig(pin, GPIO_ModeIN_PD)
#define setPinOutputPushPull(pin)  GPIOModeConfig(pin, GPIO_ModeOut_PP_5mA)
#define setPinOutputOpenDrain(pin) _Static_assert(0, "WCH platform does not implement an open-drain output")
#define setPinOutput(pin)          setPinOutputPushPull(pin)

#define writePinHigh(pin)    ((pin & 0x80000000) ? GPIOB_SetBits(pin & 0x7FFFFFFF) : GPIOA_SetBits(pin & 0x7FFFFFFF))
#define writePinLow(pin)     ((pin & 0x80000000) ? GPIOB_ResetBits(pin & 0x7FFFFFFF) : GPIOA_ResetBits(pin & 0x7FFFFFFF))
#define writePin(pin, level) ((level) ? writePinHigh(pin) : writePinLow(pin))

#define readPin(pin)   ((pin & 0x80000000) ? GPIOB_ReadPortPin(pin & 0x7FFFFFFF) : GPIOA_ReadPortPin(pin & 0x7FFFFFFF))
#define togglePin(pin) ((pin & 0x80000000) ? GPIOB_InverseBits(pin & 0x7FFFFFFF) : GPIOA_InverseBits(pin & 0x7FFFFFFF))
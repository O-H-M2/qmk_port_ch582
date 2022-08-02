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

#include "CH58x_gpio.h"

#define NO_PIN UINT8_MAX

#define GPIO_A_CONVERT(index) (1U << index)
#define GPIO_B_CONVERT(index) (0x80000000 & (1U << index))

#define A0  GPIO_A_CONVERT(0)
#define A1  GPIO_A_CONVERT(1)
#define A2  GPIO_A_CONVERT(2)
#define A3  GPIO_A_CONVERT(3)
#define A4  GPIO_A_CONVERT(4)
#define A5  GPIO_A_CONVERT(5)
#define A6  GPIO_A_CONVERT(6)
#define A7  GPIO_A_CONVERT(7)
#define A8  GPIO_A_CONVERT(8)
#define A9  GPIO_A_CONVERT(9)
#define A10 GPIO_A_CONVERT(10)
#define A11 GPIO_A_CONVERT(11)
#define A12 GPIO_A_CONVERT(12)
#define A13 GPIO_A_CONVERT(13)
#define A14 GPIO_A_CONVERT(14)
#define A15 GPIO_A_CONVERT(15)

#define B0  GPIO_B_CONVERT(0)
#define B1  GPIO_B_CONVERT(1)
#define B2  GPIO_B_CONVERT(2)
#define B3  GPIO_B_CONVERT(3)
#define B4  GPIO_B_CONVERT(4)
#define B5  GPIO_B_CONVERT(5)
#define B6  GPIO_B_CONVERT(6)
#define B7  GPIO_B_CONVERT(7)
#define B8  GPIO_B_CONVERT(8)
#define B9  GPIO_B_CONVERT(9)
#define B10 GPIO_B_CONVERT(10)
#define B11 GPIO_B_CONVERT(11)
#define B12 GPIO_B_CONVERT(12)
#define B13 GPIO_B_CONVERT(13)
#define B14 GPIO_B_CONVERT(14)
#define B15 GPIO_B_CONVERT(15)
#define B16 GPIO_B_CONVERT(16)
#define B17 GPIO_B_CONVERT(17)
#define B18 GPIO_B_CONVERT(18)
#define B19 GPIO_B_CONVERT(19)
#define B20 GPIO_B_CONVERT(20)
#define B21 GPIO_B_CONVERT(21)
#define B22 GPIO_B_CONVERT(22)
#define B23 GPIO_B_CONVERT(23)
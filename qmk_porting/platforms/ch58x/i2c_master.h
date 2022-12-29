/* Copyright 2022 OctopusZ
 * Copyright 2022 Huckies <https://github.com/Huckies>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/* Library made by: g4lvanix
 * GitHub repository: https://github.com/g4lvanix/I2C-master-lib
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

typedef int16_t i2c_status_t;

#define TxAdderss 0x52

#define I2C_STATUS_SUCCESS (0)
#define I2C_STATUS_ERROR   (-1)
#define I2C_STATUS_TIMEOUT (-2)

void i2c_init();
i2c_status_t i2c_start(uint8_t address, uint16_t timeout);
void i2c_stop();
i2c_status_t i2c_transmit(uint8_t address, const uint8_t *data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_receive(uint8_t address, uint8_t *data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, const uint8_t *data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_writeReg16(uint8_t devaddr, uint16_t regaddr, const uint8_t *data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t *data, uint16_t length, uint16_t timeout);
i2c_status_t i2c_readReg16(uint8_t devaddr, uint16_t regaddr, uint8_t *data, uint16_t length, uint16_t timeout);

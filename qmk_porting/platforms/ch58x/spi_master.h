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

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

#define SPI_STATUS_SUCCESS (0)
#define SPI_STATUS_ERROR   (-1)
#define SPI_STATUS_TIMEOUT (-2)

#define SPI_TIMEOUT_IMMEDIATE (0)
#define SPI_TIMEOUT_INFINITE  (0xFFFF)

#ifndef SPI_TIMEOUT
#define SPI_TIMEOUT 100
#endif

typedef int16_t spi_status_t;

#ifdef __cplusplus
extern "C" {
#endif

void spi_master_pre_transmit_cb();
void spi_master_post_transmit_cb();
void spi_init();
bool spi_start(pin_t slavePin, bool lsbFirst, uint8_t mode, uint16_t divisor);
spi_status_t spi_write(uint8_t data);
spi_status_t spi_read();
spi_status_t spi_transmit(const uint8_t *data, uint16_t length);
spi_status_t spi_receive(uint8_t *data, uint16_t length);
void spi_stop();

#ifdef __cplusplus
}
#endif

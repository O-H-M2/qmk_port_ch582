#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

#ifndef SPI_ALTER
#define SPI_ALTER 0
#endif

#define SPI_STATUS_SUCCESS (0)
#define SPI_STATUS_ERROR   (-1)
#define SPI_STATUS_TIMEOUT (-2)

#define SPI_TIMEOUT_IMMEDIATE (0)
#define SPI_TIMEOUT_INFINITE  (0xFFFF)

typedef int8_t spi_status_t;

#ifdef __cplusplus
extern "C" {
#endif

extern spi_status_t spi_status;

void spi_init(bool pin_alter);
bool spi_start(pin_t slavePin, bool lsbFirst, uint8_t mode, uint16_t divisor);
spi_status_t spi_write(uint8_t data);
spi_status_t spi_read();
spi_status_t spi_transmit(const uint8_t *data, uint16_t length);
spi_status_t spi_receive(uint8_t *data, uint16_t length);
void spi_stop();

#ifdef __cplusplus
}
#endif
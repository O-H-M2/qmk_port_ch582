#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

void uart_init(uint32_t baud);
void uart_write(uint8_t data);
uint8_t uart_read();
void uart_transmit(const uint8_t *data, uint16_t length);
void uart_receive(uint8_t *data, uint16_t length);
bool uart_available();

#pragma once

#include <stdint.h>
#include <stdbool.h>

void rgb_raw_hid_receive(uint8_t *data, uint8_t length);
void rgb_raw_hid_send(uint8_t *data, uint8_t length);
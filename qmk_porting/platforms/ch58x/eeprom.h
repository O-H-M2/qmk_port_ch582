// Copyright 2018-2022 QMK
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "CH58x_common.h"

// we use EEPROM_CUSTOM
#define EEPROM_SIZE             0x8000
#define TOTAL_EEPROM_BYTE_COUNT (EEPROM_SIZE)
#ifndef QMK_EEPROM_START_POSITION
#define QMK_EEPROM_START_POSITION 0
#endif

uint8_t eeprom_read_byte(const uint8_t *__p);
uint16_t eeprom_read_word(const uint16_t *__p);
uint32_t eeprom_read_dword(const uint32_t *__p);
void eeprom_read_block(void *__dst, const void *__src, size_t __n);
void eeprom_write_byte(uint8_t *__p, uint8_t __value);
void eeprom_write_word(uint16_t *__p, uint16_t __value);
void eeprom_write_dword(uint32_t *__p, uint32_t __value);
void eeprom_write_block(const void *__src, void *__dst, size_t __n);
void eeprom_update_byte(uint8_t *__p, uint8_t __value);
void eeprom_update_word(uint16_t *__p, uint16_t __value);
void eeprom_update_dword(uint32_t *__p, uint32_t __value);
void eeprom_update_block(const void *__src, void *__dst, size_t __n);
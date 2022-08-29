// Copyright 2022 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Start at the start of the data flash
#ifndef WEAR_LEVELING_EEPROM_BASE_ADDR
#define WEAR_LEVELING_EEPROM_BASE_ADDR QMK_EEPROM_START_POSITION
#endif

// Work out how many bytes per write to data flash
#ifndef BACKING_STORE_WRITE_SIZE
#define BACKING_STORE_WRITE_SIZE 2
#endif

// 4kB backing space allocated
#ifndef WEAR_LEVELING_BACKING_SIZE
#define WEAR_LEVELING_BACKING_SIZE 0x1000
#endif

// 2kB logical EEPROM
#ifndef WEAR_LEVELING_LOGICAL_SIZE
#define WEAR_LEVELING_LOGICAL_SIZE 0x800
#endif

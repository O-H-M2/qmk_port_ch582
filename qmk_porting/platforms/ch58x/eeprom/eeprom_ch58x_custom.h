#pragma once

#ifndef EEPROM_PAGE_SIZE
#define EEPROM_PAGE_SIZE 0x100 // 256bytes
#endif

#ifndef EEPROM_PAGE_COUNT
#define EEPROM_PAGE_COUNT 8 // 8pages
#endif

#define EEPROM_SIZE (EEPROM_PAGE_SIZE * EEPROM_PAGE_COUNT) // 2k
_Static_assert(QMK_EEPROM_START_POSITION + EEPROM_SIZE <= BLE_SNV_ADDR, "Overlap detected in EEPROM, between QMK sector and BLE sector.");

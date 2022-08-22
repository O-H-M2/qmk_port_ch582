#pragma once

// we use EEPROM_CUSTOM

#ifndef EEPROM_PAGE_SIZE
#define EEPROM_PAGE_SIZE  0x100                              // 256bytes
#endif

#ifndef EEPROM_PAGE_COUNT
#define EEPROM_PAGE_COUNT 8                                  // 8pages  
#endif

#define EEPROM_SIZE (EEPROM_PAGE_SIZE * EEPROM_PAGE_COUNT)   // 2k

#ifndef EEPROM_BASE_ADDR 
#define EEPROM_BASE_ADDR QMK_EEPROM_START_POSITION
#endif

#define EEPROM_END_ADDR (EEPROM_BASE_ADDR + EEPROM_SIZE)

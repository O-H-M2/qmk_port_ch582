/*
Copyright 2022 Huckies <https://github.com/Huckies>
               zhaqian <https://github.com/zhaqian12>

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

#ifndef EEPROM_PAGE_SIZE
#define EEPROM_PAGE_SIZE 0x100 // 256bytes
#endif

#ifndef EEPROM_PAGE_COUNT
#define EEPROM_PAGE_COUNT 8 // 8pages
#endif

#define EEPROM_SIZE (EEPROM_PAGE_SIZE * EEPROM_PAGE_COUNT) // 2k
_Static_assert(QMK_EEPROM_START_POSITION + EEPROM_SIZE <= BLE_SNV_ADDR, "Overlap detected in EEPROM, between QMK sector and BLE sector.");

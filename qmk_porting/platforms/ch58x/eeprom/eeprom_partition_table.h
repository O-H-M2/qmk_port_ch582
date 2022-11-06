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

enum {
    RESERVED_ = 0x0,
    WHITELIST_EEPROM_START_POSITION_ = 0x400,
    USER_EEPROM_START_POSITION_ = 0x500,
    QMK_EEPROM_RESERVED_START_POSITION_ = 0x4f00,
    QMK_EEPROM_START_POSITION_ = 0x5000,
    BLE_EEPROM_START_POSITION_ = 0x7000,
};

#define WHITELIST_EEPROM_START_POSITION    WHITELIST_EEPROM_START_POSITION_
#define USER_EEPROM_START_POSITION         USER_EEPROM_START_POSITION_
#define QMK_EEPROM_RESERVED_START_POSITION QMK_EEPROM_RESERVED_START_POSITION_
#define QMK_EEPROM_START_POSITION          QMK_EEPROM_START_POSITION_
#define BLE_SNV_ADDR                       BLE_EEPROM_START_POSITION_

_Static_assert(WHITELIST_EEPROM_START_POSITION >= 0x400, "EEPROM: Reserve area too small!");
_Static_assert(WHITELIST_EEPROM_START_POSITION % 0x100 == 0, "EEPROM: Whitelist data sector not aligned!");
_Static_assert(USER_EEPROM_START_POSITION - WHITELIST_EEPROM_START_POSITION == 0x100, "EEPROM: Whitelist sector should be exactly 256-byte large!");
_Static_assert(USER_EEPROM_START_POSITION % 0x100 == 0, "EEPROM: User data sector not aligned!");
_Static_assert(QMK_EEPROM_START_POSITION - QMK_EEPROM_RESERVED_START_POSITION == 0x100, "EEPROM: QMK reserved sector should be exactly 256-byte large!");
_Static_assert(QMK_EEPROM_START_POSITION % 0x100 == 0, "EEPROM: QMK data sector not aligned!");
_Static_assert(BLE_SNV_ADDR % 0x100 == 0, "EEPROM: BLE data sector not aligned!");

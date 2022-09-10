// Copyright 2022 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#include <stdbool.h>
#include "wear_leveling.h"
#include "wear_leveling_internal.h"
#include "eeprom.h"

static bool backing_store_locked = false;

bool backing_store_init(void)
{
    return true;
}

bool backing_store_unlock(void)
{
    backing_store_locked = false;
    return true;
}

bool backing_store_erase(void)
{
    if (backing_store_locked) {
        return false;
    }

    uint8_t status = EEPROM_ERASE(QMK_EEPROM_START_POSITION, WEAR_LEVELING_BACKING_SIZE);

    if (status != 0) {
        return false;
    }
    return true;
}

bool backing_store_write(uint32_t address, backing_store_int_t value)
{
    if (backing_store_locked) {
        return false;
    }

    uint32_t offset = QMK_EEPROM_START_POSITION + address;
    backing_store_int_t buf = (~value);
    uint8_t status = EEPROM_WRITE(offset, &buf, sizeof(backing_store_int_t));

    if (status != 0) {
        return false;
    }
    return true;
}

bool backing_store_lock(void)
{
    backing_store_locked = true;
    return true;
}

bool backing_store_read(uint32_t address, backing_store_int_t *value)
{
    if (backing_store_locked) {
        return false;
    }

    uint32_t offset = QMK_EEPROM_START_POSITION + address;
    backing_store_int_t buf;
    uint8_t status = EEPROM_READ(offset, &buf, sizeof(backing_store_int_t));

    *value = (~buf);
    if (status != 0) {
        return false;
    }
    return true;
}

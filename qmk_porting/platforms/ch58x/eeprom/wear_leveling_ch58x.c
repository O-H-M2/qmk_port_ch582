// Copyright 2022 Nick Brassel (@tzarc)
// SPDX-License-Identifier: GPL-2.0-or-later
#include <stdbool.h>
#include "wear_leveling.h"
#include "wear_leveling_internal.h"
#include "eeprom.h"
#include "CH58x_common.h"

bool backing_store_init(void)
{
    return true;
}

bool backing_store_unlock(void)
{
    return true;
}

bool backing_store_erase(void)
{
    uint8_t status = 1;
    status = EEPROM_ERASE(WEAR_LEVELING_EEPROM_BASE_ADDR, WEAR_LEVELING_BACKING_SIZE);
    if (status != 0)
        return false;
    return true;
}

bool backing_store_write(uint32_t address, backing_store_int_t value)
{
    uint32_t offset = (WEAR_LEVELING_EEPROM_BASE_ADDR + address);
    backing_store_int_t buf = (~value);
    uint8_t status = 1;
    status = EEPROM_WRITE(offset, &buf, sizeof(backing_store_int_t));
    if (status != 0)
        return false;
    return true;
}

bool backing_store_lock(void)
{
    return true;
}

bool backing_store_read(uint32_t address, backing_store_int_t *value)
{
    uint32_t offset = ((WEAR_LEVELING_EEPROM_BASE_ADDR) + address);
    backing_store_int_t buf;
    uint8_t status = 1;
    status = EEPROM_READ(offset, &buf, sizeof(backing_store_int_t));
    *value = (~buf);
    if (status != 0)
        return false;
    return true;
}

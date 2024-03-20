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

#include "eeprom.h"
#include "eeprom_driver.h"

void eeprom_driver_init(void)
{
    print("eeprom_driver_init\n");
}

void eeprom_driver_erase(void)
{
#if __BUILDING_IAP__
    PRINT("eeprom_driver_erase\n");
#endif
#if __BUILDING_APP__
    print("eeprom_driver_erase\n");
#endif
    EEPROM_ERASE(QMK_EEPROM_START_POSITION, EEPROM_SIZE);
    EEPROM_ERASE(BLE_SNV_ADDR, EEPROM_MAX_SIZE - BLE_SNV_ADDR);
}

void eeprom_read_block(void *buf, const void *addr, size_t len)
{
    uint32_t offset = (const uint32_t)addr;

    if (offset > EEPROM_SIZE) {
        print("eeprom_driver_erase: OVERSTEP detected\n");
        return;
    }
    if ((offset + len - 1) > EEPROM_SIZE) {
        len = EEPROM_SIZE - offset + 1;
    }

    uint8_t data_buf[(len / 4 + (size_t)(len % 4 ? 1 : 0)) * 4];
    EEPROM_READ(QMK_EEPROM_START_POSITION + offset, data_buf, len);
    memcpy(buf, data_buf, len);
}

void eeprom_write_block(const void *buf, void *addr, size_t len)
{
    uint32_t offset = (const uint32_t)addr;

    if (offset > EEPROM_SIZE) {
        print("eeprom_driver_write: OVERSTEP detected\n");
        return;
    }
    if ((offset + len - 1) > EEPROM_SIZE) {
        len = EEPROM_SIZE - offset + 1;
    }

    uint8_t data_buf[(len / 4 + (size_t)(len % 4 ? 1 : 0)) * 4];
    // make sure all the bits are clear
    EEPROM_READ(QMK_EEPROM_START_POSITION + offset, data_buf, len);
    bool flag = true;
    for (size_t i = 0; i < len; i++) {
        if (data_buf[i] != 0xFF) {
            flag = false;
            break;
        }
    }
    if (flag) {
        // we just write the bytes
        memcpy(data_buf, buf, len);
        EEPROM_WRITE(QMK_EEPROM_START_POSITION + offset, data_buf, len);
    } else {
        // we need to erase the whole page before writing
        uint8_t buffer[(1 + (offset + len) / EEPROM_PAGE_SIZE - offset / EEPROM_PAGE_SIZE) * EEPROM_PAGE_SIZE];

        EEPROM_READ(QMK_EEPROM_START_POSITION + offset / EEPROM_PAGE_SIZE * EEPROM_PAGE_SIZE, buffer, sizeof(buffer));
        EEPROM_ERASE(QMK_EEPROM_START_POSITION + offset / EEPROM_PAGE_SIZE * EEPROM_PAGE_SIZE, sizeof(buffer));
        memcpy(buffer + offset % EEPROM_PAGE_SIZE, buf, len);
        EEPROM_WRITE(QMK_EEPROM_START_POSITION + offset / EEPROM_PAGE_SIZE * EEPROM_PAGE_SIZE, buffer, sizeof(buffer));
    }
}

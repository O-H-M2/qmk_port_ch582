#include "eeprom.h"
#include "eeprom_driver.h"

void eeprom_driver_init(void)
{
    print("eeprom_driver_init\n");
}

void eeprom_driver_erase(void)
{
    print("eeprom_driver_erase\n");
    EEPROM_ERASE(QMK_EEPROM_START_POSITION, EEPROM_SIZE);
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

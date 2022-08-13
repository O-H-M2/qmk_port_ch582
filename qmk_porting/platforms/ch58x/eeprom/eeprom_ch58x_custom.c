#include "eeprom.h"
#include "eeprom_driver.h"

#define GET_ADDRESS(addr) (EEPROM_BASE_ADDR + (uint32_t)addr)
#define GET_PAGE(addr) (uint32_t)((addr >> 8))
#define GET_PAGE_ADDRESS(addr) (uint32_t)((addr >> 8) << 8)
#define IS_EEPROM_ADDRESS(addr) ((((addr) >= EEPROM_BASE_ADDR) && ((addr) < EEPROM_END_ADDR)))

// eeprom data buffer
static uint8_t data_buf[EEPROM_SIZE];

void eeprom_driver_init(void) {
    PRINT("eeprom_driver_init\n");
    EEPROM_READ(EEPROM_BASE_ADDR, data_buf, EEPROM_SIZE);
}

void eeprom_driver_erase(void) {
    PRINT("eeprom_driver_erase\n");
    EEPROM_ERASE(EEPROM_BASE_ADDR, EEPROM_SIZE);
}

void eeprom_read_block(void *__dst, const void *__src, size_t __n)
{
    const uint8_t *src  = (const uint8_t *)__src;
    uint8_t *      dest = (uint8_t *)__dst;
    while (__n > 0) {
        *dest++ = data_buf[(uint32_t)src];
        ++src;
        --__n;
    }
}

void eeprom_write_block(const void *__src, void *__dst, size_t __n)
{
    uint8_t *      dest = (uint8_t *)__dst;
    const uint8_t *src  = (const uint8_t *)__src;
    uint32_t start_page_addr = GET_PAGE_ADDRESS(GET_ADDRESS(dest));
    uint32_t end_page_addr = GET_PAGE_ADDRESS(GET_ADDRESS(dest + (__n - 1)));
    if (IS_EEPROM_ADDRESS(end_page_addr)) {
        // eeprom erase
        uint8_t aligned = (start_page_addr == end_page_addr);
        uint8_t changed = 0;
        EEPROM_ERASE(start_page_addr, EEPROM_PAGE_SIZE);

        // eeprom data buffer write
        uint8_t value;
        while (__n > 0) {
            value = *src;
            data_buf[(uint32_t)dest] = value;
            ++dest;
            ++src;
            --__n;
        }

        // eeprom write
        uint8_t *tmp_buf = &data_buf[start_page_addr];
        uint8_t  ret     = 1;
        while ((ret = EEPROM_WRITE(start_page_addr, tmp_buf, EEPROM_PAGE_SIZE)) != 0);
        if (!aligned) {
            EEPROM_ERASE(end_page_addr, EEPROM_PAGE_SIZE);
            tmp_buf = &data_buf[end_page_addr];
            while ((ret = EEPROM_WRITE(end_page_addr, tmp_buf, EEPROM_PAGE_SIZE)) != 0);
        }
    }
}

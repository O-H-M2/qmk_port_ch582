#include "eeprom.h"

uint8_t eeprom_read_byte(const uint8_t *__p)
{
    uint8_t ret = 0;

    EEPROM_READ(QMK_EEPROM_START_POSITION + (uint32_t)__p, &ret, sizeof(ret));
}

uint16_t eeprom_read_word(const uint16_t *__p)
{
    uint16_t ret = 0;

    EEPROM_READ(QMK_EEPROM_START_POSITION + (uint32_t)__p, &ret, sizeof(ret));
}

uint32_t eeprom_read_dword(const uint32_t *__p)
{
    uint32_t ret = 0;

    EEPROM_READ(QMK_EEPROM_START_POSITION + (uint32_t)__p, &ret, sizeof(ret));
}

// void eeprom_read_block(void *__dst, const void *__src, size_t __n)
// {
// }

void eeprom_write_byte(uint8_t *__p, uint8_t __value)
{
    uint8_t temp = __value;

    EEPROM_WRITE(QMK_EEPROM_START_POSITION + (uint32_t)__p, &temp, sizeof(temp));
}

void eeprom_write_word(uint16_t *__p, uint16_t __value)
{
    uint16_t temp = __value;

    EEPROM_WRITE(QMK_EEPROM_START_POSITION + (uint32_t)__p, &temp, sizeof(temp));
}

void eeprom_write_dword(uint32_t *__p, uint32_t __value)
{
    uint32_t temp = __value;

    EEPROM_WRITE(QMK_EEPROM_START_POSITION + (uint32_t)__p, &temp, sizeof(temp));
}

// void eeprom_write_block(const void *__src, void *__dst, size_t __n)
// {
// }

void eeprom_update_byte(uint8_t *__p, uint8_t __value)
{
    eeprom_write_byte(__p, __value);
}

void eeprom_update_word(uint16_t *__p, uint16_t __value)
{
    eeprom_write_word(__p, __value);
}

void eeprom_update_dword(uint32_t *__p, uint32_t __value)
{
    eeprom_write_dword(__p, __value);
}

// void eeprom_update_block(const void *__src, void *__dst, size_t __n)
// {
// }

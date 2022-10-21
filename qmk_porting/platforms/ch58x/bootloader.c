#include "bootloader.h"
#include "eeprom_driver.h"

void bootmagic_lite_reset_eeprom(void)
{
}

void bootloader_jump()
{
}

void mcu_reset()
{
    SYS_ResetExecute();
}
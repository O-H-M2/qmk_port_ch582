#include "bootloader.h"
#include "eeprom_reserved.h"
#include "eeprom_driver.h"

void bootmagic_lite_reset_eeprom(void)
{
}

void bootloader_jump()
{
}

__HIGH_CODE void mcu_reset()
{
    FLASH_ROM_SW_RESET();
    sys_safe_access_enable();
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET;
    sys_safe_access_disable();
}
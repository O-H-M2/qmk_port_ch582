#include "bootloader.h"

__HIGH_CODE
void bootloader_jump()
{
    FLASH_ROM_ERASE(0, 4096);
    sys_safe_access_enable();
    R16_INT32K_TUNE = 0xFFFF;
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET;
    sys_safe_access_disable();
    while (1) {
        ;
    }
}

void mcu_reset()
{
    SYS_ResetExecute();
}
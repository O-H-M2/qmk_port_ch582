#include "bootloader.h"

__HIGH_CODE
void bootloader_jump()
{
    FLASH_ROM_ERASE(0, 4096);
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG1;
    R8_SAFE_ACCESS_SIG = SAFE_ACCESS_SIG2;
    R16_INT32K_TUNE = 0xFFFF;
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET;
    R8_SAFE_ACCESS_SIG = 0;
    while (1) {
        ;
    }
}

void mcu_reset()
{
    SYS_ResetExecute();
}
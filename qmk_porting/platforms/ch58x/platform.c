#include "platform_deps.h"
#include "quantum_keycodes.h"

__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if (defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = { 0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02 };
#endif

volatile uint8_t kbd_protocol_type = 0;

#ifndef PLF_DEBUG
/* platform uart log output is disabled
 * so we fake a _putchar() to avoid link error */
void _putchar(char character)
{
}
#endif

int8_t sendchar(uint8_t c)
{
    _putchar(c);
    return 0;
}

void shutdown_user()
{
    rgbled_power_off();
}

void platform_setup()
{
    _Static_assert(kbd_protocol_max > 1, "No interface enabled!");

#if FREQ_SYS != 60000000
    SetSysClock(Fsys);
    DelayMs(5);
#ifdef PLF_DEBUG
    UART1_BaudRateCfg(460800);
#endif
#endif
    PowerMonitor(ENABLE, HALevel_2V1);
    UserOptionByteConfig(ENABLE, ENABLE, DISABLE, 112);
#if 0
    PRINT("EEPROM dump: \n");
    for (uint8_t i = 0; i < 8; i++) {
        PRINT("Page: %d\n", i);
        uint8_t eeprom_dump[0x1000] = {};
        EEPROM_READ(i * 0x1000, eeprom_dump, 0x1000);
        for (uint16_t j = 0; j < 0x1000; j++) {
            PRINT("0x%02x ", eeprom_dump[j]);
            DelayUs(20);
        }
        PRINT("\n\n");
        DelayMs(1);
    }
    PRINT("End of EEPROM dump.\n\n");
#endif
    event_propagate(PLATFORM_EVENT_MODE_SELECT, NULL);
    event_propagate(PLATFORM_EVENT_INITIALIZE, NULL);
}

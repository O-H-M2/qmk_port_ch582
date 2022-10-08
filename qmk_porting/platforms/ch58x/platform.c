#include "platform_deps.h"
#include "quantum_keycodes.h"

volatile uint8_t kbd_protocol_type = 0;
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if (defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = { 0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02 };
#endif

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

void platform_setup()
{
    _Static_assert(kbd_protocol_max > 1, "No interface enabled!");

#if FREQ_SYS != 60000000
    SetSysClock(Fsys);
#endif
    DelayMs(5);
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
}

#ifdef USB_ENABLE

void platform_setup_usb()
{
    Calibration_LSI(Level_64);
}

#endif

#ifdef BLE_ENABLE

void platform_setup_ble()
{
    _Static_assert(KC_VENDOR_BT1 >= SAFE_RANGE, "Error: overlap detected between QMK and Vendor defined keycodes!");

    CH58X_BLEInit();
    HAL_Init();
    GAPRole_PeripheralInit();
}

#endif

#ifdef ESB_ENABLE

void platform_setup_esb()
{
    CH58X_BLEInit();
    HAL_Init();
    RF_RoleInit();
    RF_Init();
#if ESB_ENABLE == 2
    init_usb_driver();
    kbd_protocol_type = kbd_protocol_esb;
#endif
}

#endif
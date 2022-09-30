#include "platform_deps.h"
#include "quantum_keycodes.h"

volatile uint8_t kbd_protocol_type = 0;
__attribute__((aligned(4))) uint32_t MEM_BUF[BLE_MEMHEAP_SIZE / 4];

#if (defined(BLE_MAC)) && (BLE_MAC == TRUE)
const uint8_t MacAddr[6] = { 0x84, 0xC2, 0xE4, 0x03, 0x02, 0x02 };
#endif

/* we have already set the correct _putchar() function 
 * disable this to avoid further modification */
void print_set_sendchar(sendchar_func_t func)
{
}

#ifndef PLF_DEBUG
/* platform uart log output is disabled
 * so we fake a _putchar() to avoid link error */
void _putchar(char character)
{
}
#endif

void platform_setup()
{
#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg(ENABLE);
#endif
    SetSysClock(Fsys);
    DelayMs(5);
    PowerMonitor(ENABLE, HALevel_2V1);
#ifdef PLF_DEBUG
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
    UART1_BaudRateCfg(460800);
#endif
#ifdef LSE_FREQ
    sys_safe_access_enable();
    R8_CK32K_CONFIG |= RB_CLK_OSC32K_XT | RB_CLK_INT32K_PON | RB_CLK_XT32K_PON;
    sys_safe_access_disable();
#else
    sys_safe_access_enable();
    R8_CK32K_CONFIG &= ~(RB_CLK_OSC32K_XT | RB_CLK_XT32K_PON);
    R8_CK32K_CONFIG |= RB_CLK_INT32K_PON;
    sys_safe_access_disable();
    Calibration_LSI(Level_64);
#endif
    // TODO: set some GPIO for mode decision

    PRINT("Chip start, %s\n", VER_LIB);
    PRINT("Reason of last reset:  ");
    switch (R8_RESET_STATUS & RB_RESET_FLAG) {
        case 0b000:
            PRINT("Software\n");
            break;
        case 0b001:
            PRINT("Power on\n");
            break;
        case 0b010:
            PRINT("Watchdog timeout\n");
            break;
        case 0b011:
            PRINT("Manual\n");
            break;
        case 0b101:
            PRINT("Wake from shutdown\n");
            break;
        default:
            PRINT("Wake from sleep\n");
            break;
    }

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
}

#endif
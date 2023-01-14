/*
Copyright 2022 Huckies <https://github.com/Huckies>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "iap.h"

static struct usbd_interface intf0;
static volatile uint16_t usb_counter = 0;
static uint8_t msc_ram_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0200, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_CONFIG_SIZE, 0x01, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    MSC_DESCRIPTOR_INIT(0x00, MSC_OUT_EP, MSC_IN_EP, 0x02),
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x10,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'H', 0x00,                  /* wcChar0 */
    'u', 0x00,                  /* wcChar1 */
    'c', 0x00,                  /* wcChar2 */
    'k', 0x00,                  /* wcChar3 */
    'i', 0x00,                  /* wcChar4 */
    'e', 0x00,                  /* wcChar5 */
    's', 0x00,                  /* wcChar6 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x18,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'H', 0x00,                  /* wcChar0 */
    'u', 0x00,                  /* wcChar1 */
    'c', 0x00,                  /* wcChar2 */
    'K', 0x00,                  /* wcChar3 */
    'e', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    's', 0x00,                  /* wcChar6 */
    ' ', 0x00,                  /* wcChar7 */
    'D', 0x00,                  /* wcChar8 */
    'F', 0x00,                  /* wcChar9 */
    'U', 0x00,                  /* wcChar10 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x02,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    // '2', 0x00,                  /* wcChar0 */

    0x00
};
static WriteState _wr_state = { 0 };

/**
 * "Reload" some functions into IRAM to increase speed
*/
#if 1
__HIGH_CODE static void my_delay_us(uint16_t t)
{
    uint32_t i = t * 15;

    do {
        __nop();
    } while (--i);
}

__HIGH_CODE static void my_delay_ms(uint16_t t)
{
    for (uint16_t i = 0; i < t; i++) {
        my_delay_us(1000);
    }
}

__HIGH_CODE void my_memcpy(void *dst, const void *src, uint32_t l)
{
    uint32_t len = l;

    uint8_t *pdst = (uint8_t *)dst;
    uint8_t *psrc = (uint8_t *)src;
    while (len) {
        *pdst++ = *psrc++;
        len--;
    }
}

__HIGH_CODE void my_memset(void *dst, int c, uint32_t n)
{
    for (uint32_t i = 0; i < n; i++) {
        *((uint8_t *)dst + i) = c;
    }
}

__HIGH_CODE uint32_t my_get_sys_clock()
{
    uint16_t rev;

    rev = R16_CLK_SYS_CFG & 0xff;
    if ((rev & 0x40) == (0 << 6)) {
        return (32000000 / (rev & 0x1f));
    } else if ((rev & RB_CLK_SYS_MOD) == (1 << 6)) {
        return (480000000 / (rev & 0x1f));
    } else {
        return (32000);
    }
}
#endif

/**
 * Flash APIs for uf2 library
*/
#if 1
__HIGH_CODE void board_flash_init()
{
    PRINT("Erasing application...\n");
    bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_IAP_ONGOING);
    FLASH_ROM_ERASE(APP_CODE_START_ADDR, APP_CODE_END_ADDR - APP_CODE_START_ADDR);
}

__HIGH_CODE uint32_t board_flash_size()
{
    return (APP_CODE_END_ADDR - APP_CODE_START_ADDR);
}

__HIGH_CODE void board_flash_read(uint32_t addr, void *buffer, uint32_t len)
{
    PRINT("Reading flash address 0x%04x, len 0x%04x...\n", addr, len);
    usb_counter = 0;
    my_memset(buffer, 0xff, len);
}

__HIGH_CODE void board_flash_flush()
{
    PRINT("Flashing done.\n");
    bootloader_set_to_default_mode("DFU done");
    iap_cleanup();
    usb_counter = 60000;
}

__HIGH_CODE void board_flash_write(uint32_t addr, void const *data, uint32_t len)
{
    if (addr < APP_SECTOR_START_ADDR || addr % sizeof(uint32_t) != 0) {
        PRINT("Flash violation.\n");
        return;
    }

    PRINT("Writing flash address 0x%04x, len 0x%04x... ", addr, len);
    usb_counter = 0;
    for (;;) {
        FLASH_ROM_WRITE(addr, (void *)data, len);
        if (FLASH_ROM_VERIFY(addr, (void *)data, len) == SUCCESS) {
            PRINT("done\n");
            break;
        }
        PRINT("fail\n");
        my_delay_ms(10);
        PRINT("Retry... ");
    }
}
#endif

/**
 * Callbacks for CherryUSB stack
*/
#if 1
__HIGH_CODE void usbd_configure_done_callback(void)
{
    /* do nothing */
}

__HIGH_CODE void usb_dc_low_level_init()
{
    my_delay_ms(100);
    PFIC_EnableIRQ(USB_IRQn);
}

__HIGH_CODE void usbd_msc_get_cap(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
    *block_num = MAX_BLOCKS; // bluffing
    *block_size = 512;
}

__HIGH_CODE int usbd_msc_sector_read(uint32_t sector, uint8_t *buffer, uint32_t length)
{
    uint32_t count = 0;

    my_memset(buffer, 0x00, length);
    while (count < length) {
        uf2_read_block(sector, buffer);
        buffer += 512;
        count += 512;
    }
    return 0;
}

__HIGH_CODE int usbd_msc_sector_write(uint32_t sector, uint8_t *buffer, uint32_t length)
{
    uint32_t count = 0;

    while (count < length) {
        // Consider non-uf2 block write as successful
        // only break if write_block is busy with flashing (return 0)
        if (0 == uf2_write_block(sector, buffer, &_wr_state)) {
            PRINT("Flash busy.\n");
            break;
        }
        buffer += 512;
        count += 512;
    }
    return 0;
}
#endif

__HIGH_CODE static void gpio_strap()
{
    uint32_t pin_a = GPIO_Pin_All & 0x7FFFFFFF, pin_b = GPIO_Pin_All;

#if defined LSE_ENABLE && LSE_ENABLE
    pin_a &= ~bX32KI;
    pin_a &= ~bX32KO;
#endif
#ifdef WS2812
    if (WS2812_EN_PIN & 0x80000000) {
        pin_b &= ~((WS2812_EN_PIN & 0x7FFFFFFF));
    } else {
        pin_a &= ~((WS2812_EN_PIN & 0x7FFFFFFF));
    }
#elif defined AW20216
    if (DRIVER_1_EN & 0x80000000) {
        pin_b &= ~((DRIVER_1_EN & 0x7FFFFFFF));
    } else {
        pin_a &= ~((DRIVER_1_EN & 0x7FFFFFFF));
    }
#ifdef DRIVER_2_EN
    if (DRIVER_2_EN & 0x80000000) {
        pin_b &= ~((DRIVER_2_EN & 0x7FFFFFFF));
    } else {
        pin_a &= ~((DRIVER_2_EN & 0x7FFFFFFF));
    }
#endif
#endif
    pin_b &= ~bUDP;
    pin_b &= ~bUDM;
    pin_b &= ~bU2DP;
    pin_b &= ~bU2DM;
    setPinInputLow(pin_a);
    setPinInputLow(pin_b);
#ifdef BATTERY_MEASURE_PIN
    setPinInputHigh(BATTERY_MEASURE_PIN);
#endif
#ifdef POWER_DETECT_PIN
    setPinInput(POWER_DETECT_PIN);
#endif
}

__HIGH_CODE _PUTCHAR_CLAIM;

__HIGH_CODE static void iap_handle_new_chip()
{
    uint8_t ret = UserOptionByteConfig(DISABLE, ENABLE, DISABLE, 128);

    PRINT("Setting user config... %s\n", ret == SUCCESS ? "done" : "fail");
    if (ret == SUCCESS) {
        bootloader_set_to_default_mode("New chip with wireless support");
    } else {
        PRINT("Will reboot and try again.\n");
    }
    WAIT_FOR_DBG;
    // construct a power on reset to make the config effective
    FLASH_ROM_SW_RESET();
    sys_safe_access_enable();
    R16_INT32K_TUNE = 0xFFFF;
    sys_safe_access_enable();
    R8_RST_WDOG_CTRL |= RB_SOFTWARE_RESET;
    sys_safe_access_disable();
    __builtin_unreachable();
}

__HIGH_CODE static void iap_decide_jump()
{
    static uint8_t first_in = true;
    uint8_t mode = bootloader_boot_mode_get();

    switch (mode) {
        case UINT8_MAX:
            iap_handle_new_chip();
            jumpApp();
            __builtin_unreachable();
        case BOOTLOADER_BOOT_MODE_IAP:
            // we stay in dfu mode for a while
            if (first_in) {
                first_in = false;
                return;
            } else {
                PRINT("Leaving DFU...\n");
                iap_cleanup();
            }
        case BOOTLOADER_BOOT_MODE_USB:
        case BOOTLOADER_BOOT_MODE_BLE:
        case BOOTLOADER_BOOT_MODE_ESB:
            // ready to go
            jumpApp();
            __builtin_unreachable();
        case BOOTLOADER_BOOT_MODE_IAP_ONGOING:
            PRINT("IAP unaccomplished, will reside.\n");
            return;
        default:
            PRINT("Invalid mode record detected, will take as interrupted IAP procedure.\n");
            bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_IAP_ONGOING);
            return;
    }
}

__HIGH_CODE static void Main_Circulation()
{
    static uint8_t second = 0;

    for (;;) {
        my_delay_ms(1);
        usb_counter++;
        if (usb_counter / 1000 > second) {
            second = usb_counter / 1000;
            PRINT("Exiting DFU in %d seconds...\n", 60 - second);
        } else if (usb_counter / 1000 < second) {
            PRINT("Fatal: Operation interrupted.\n");
            second = 0;
        }
        if (second >= 60) {
            iap_decide_jump();
            // jump must have failed, reset the counters
            usb_counter = 0;
            second = 0;
        }
    }
}

__HIGH_CODE int main()
{
#ifdef HSE_LOAD_CAPACITANCE
#if HSE_LOAD_CAPACITANCE == 10
    HSECFG_Capacitance(HSECap_10p);
#elif HSE_LOAD_CAPACITANCE == 12
    HSECFG_Capacitance(HSECap_12p);
#elif HSE_LOAD_CAPACITANCE == 14
    HSECFG_Capacitance(HSECap_14p);
#elif HSE_LOAD_CAPACITANCE == 16
    HSECFG_Capacitance(HSECap_16p);
#elif HSE_LOAD_CAPACITANCE == 18
    HSECFG_Capacitance(HSECap_18p);
#elif HSE_LOAD_CAPACITANCE == 20
    HSECFG_Capacitance(HSECap_20p);
#elif HSE_LOAD_CAPACITANCE == 22
    HSECFG_Capacitance(HSECap_22p);
#elif HSE_LOAD_CAPACITANCE == 24
    HSECFG_Capacitance(HSECap_24p);
#else
#error "Invalid HSE capacitance!"
#endif
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);
    gpio_strap();
#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    uint16_t adj = R16_AUX_POWER_ADJ;
    uint16_t plan = R16_POWER_PLAN;

    adj |= RB_DCDC_CHARGE;
    plan |= RB_PWR_DCDC_PRE;
    sys_safe_access_enable();
    R16_AUX_POWER_ADJ = adj;
    R16_POWER_PLAN = plan;
    my_delay_us(10);
    sys_safe_access_enable();
    R16_POWER_PLAN |= RB_PWR_DCDC_EN;
    sys_safe_access_disable();
#endif
#ifdef PLF_DEBUG
    DBG_INIT;
    PRINT("\n\n\nBootloader " MACRO2STR(__GIT_VERSION__) "\n");
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
#else
    // manually initialize uart1 and send some debug information
    writePinHigh(A9);
    setPinOutput(A9);
    setPinInputHigh(A8);

    uint32_t x;

    x = 75000000 / DEBUG_BAUDRATE;
    x = (x + 5) / 10;
    R16_UART1_DL = (uint16_t)x;
    R8_UART1_FCR = (2 << 6) | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN;
    R8_UART1_LCR = RB_LCR_WORD_SZ;
    R8_UART1_IER = RB_IER_TXD_EN;
    R8_UART1_DIV = 1;

    char buffer[UINT8_MAX];
    uint8_t len = sprintf(buffer, "\n\n\nBootloader " MACRO2STR(__GIT_VERSION__) "\nReason of last reset: %d\n", R8_RESET_STATUS & RB_RESET_FLAG);

    while (len) {
        if (R8_UART1_TFC != UART_FIFO_SIZE) {
            R8_UART1_THR = buffer[strlen(buffer) - len];
            len--;
        }
    }
    while ((R8_UART1_LSR & RB_LSR_TX_ALL_EMP) == 0) {
        __nop();
    }
    R8_UART1_IER = RB_IER_RESET;
    setPinInputLow(A8);
    setPinInputLow(A9);
#endif
#ifdef BATTERY_MEASURE_PIN
    // do a power check

    setPinInput(BATTERY_MEASURE_PIN);
    battery_init();

    uint16_t adc = battery_measure();

    adc = battery_calculate(adc);
    PRINT("Battery level: %d\n", adc);
#endif

#if !defined ESB_ENABLE || ESB_ENABLE != 2
    iap_decide_jump();
#else
    // TODO: implement judging condition for 2.4g dongle
    iap_decide_jump();
#endif

    uf2_init();

    usbd_desc_register(msc_ram_descriptor);
    usbd_add_interface(usbd_msc_init_intf(&intf0, MSC_OUT_EP, MSC_IN_EP));
    usbd_initialize();

    Main_Circulation();
}

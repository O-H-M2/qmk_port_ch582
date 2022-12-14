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

#include "bootloader.h"
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
 * Use a dedicate section to reconstruct the start up sequence
*/
#if 1
extern uint32_t _highcode_lma;
extern uint32_t _highcode_vma_start;
extern uint32_t _highcode_vma_end;

extern uint32_t _data_lma;
extern uint32_t _data_vma;
extern uint32_t _edata;

extern uint32_t _sbss;
extern uint32_t _ebss;

__attribute__((section(".highcode_copy"))) static void __attribute__((noinline)) copy_section(uint32_t *p_load, uint32_t *p_vma, uint32_t *p_vma_end)
{
    while (p_vma <= p_vma_end) {
        *p_vma = *p_load;
        ++p_load;
        ++p_vma;
    }
}

__attribute__((section(".highcode_copy"))) static void __attribute__((noinline)) zero_section(uint32_t *start, uint32_t *end)
{
    uint32_t *p_zero = start;

    while (p_zero <= end) {
        *p_zero = 0;
        ++p_zero;
    }
}

__attribute__((section(".highcode_copy"))) void mySystemInit()
{
    sys_safe_access_enable();
    R8_PLL_CONFIG &= ~(1 << 5); //
    sys_safe_access_disable();
    // PLL div
    if (!(R8_HFCK_PWR_CTRL & RB_CLK_PLL_PON)) {
        sys_safe_access_enable();
        R8_HFCK_PWR_CTRL |= RB_CLK_PLL_PON; // PLL power on
        for (uint32_t i = 0; i < 2000; i++) {
            __nop();
            __nop();
        }
    }
    sys_safe_access_enable();
    R16_CLK_SYS_CFG = (1 << 6) | (CLK_SOURCE_PLL_60MHz & 0x1f);
    __nop();
    __nop();
    __nop();
    __nop();
    sys_safe_access_disable();
    sys_safe_access_enable();
    R8_FLASH_CFG = 0X52;
    sys_safe_access_disable();
    //更改FLASH clk的驱动能力
    sys_safe_access_enable();
    R8_PLL_CONFIG |= 1 << 7;
    sys_safe_access_disable();

    copy_section(&_highcode_lma, &_highcode_vma_start, &_highcode_vma_end);
    copy_section(&_data_lma, &_data_vma, &_edata);
    zero_section(&_sbss, &_ebss);
}

void SystemInit()
{
    mySystemInit();
}
#endif

/**
 * "Reload" two basic functions into IRAM to increase speed
*/
#if 1
__HIGH_CODE void my_delay_us(uint16_t t)
{
    uint32_t i = t * 15;

    do {
        __nop();
    } while (--i);
}

__HIGH_CODE void my_delay_ms(uint16_t t)
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
#endif

/**
 * Flash APIs for uf2 library
*/
#if 1
__HIGH_CODE void board_flash_init()
{
    PRINT("Erasing application...\n");
    FLASH_ROM_ERASE(APP_CODE_START_ADDR, APP_CODE_END_ADDR - APP_CODE_START_ADDR);
}

__HIGH_CODE uint32_t board_flash_size()
{
    return (APP_CODE_END_ADDR - APP_CODE_START_ADDR);
}

__HIGH_CODE void board_flash_read(uint32_t addr, void *buffer, uint32_t len)
{
    PRINT("Reading flash address 0x%04x...\n", addr);
    usb_counter = 0;
    my_memset(buffer, 0xFF, len);
}

__HIGH_CODE void board_flash_flush()
{
    PRINT("Flashing done.\n");
    usb_counter = 58000;
}

__HIGH_CODE void board_flash_write(uint32_t addr, void const *data, uint32_t len)
{
    if (addr < APP_CODE_START_ADDR || addr % EEPROM_PAGE_SIZE != 0) {
        PRINT("Flash violation.\n");
        return;
    }

    PRINT("Writing flash address 0x%04x... ", addr);
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

__HIGH_CODE void gpio_strap()
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
#ifdef BATTERY_MEASURE_PIN
    if (BATTERY_MEASURE_PIN & 0x80000000) {
        pin_b &= ~((BATTERY_MEASURE_PIN & 0x7FFFFFFF));
    } else {
        pin_a &= ~((BATTERY_MEASURE_PIN & 0x7FFFFFFF));
    }
#endif
    pin_b &= ~bUDP;
    pin_b &= ~bUDM;
    pin_b &= ~bU2DP;
    pin_b &= ~bU2DM;
    setPinInputLow(pin_a);
    setPinInputLow(pin_b);
}

__HIGH_CODE _PUTCHAR_CLAIM;

__HIGH_CODE void Main_Circulation()
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
            iap_leave_dfu();
            jumpApp_prerequisite();
            jumpApp();
        }
    }
}

int main()
{
#ifdef HSE_LOAD_CAPACITANCE
    {
        uint8_t capacitance = HSE_LOAD_CAPACITANCE;

        switch (capacitance) {
            case 10:
                HSECFG_Capacitance(HSECap_10p);
                break;
            case 12:
                HSECFG_Capacitance(HSECap_12p);
                break;
            case 14:
                HSECFG_Capacitance(HSECap_14p);
                break;
            case 16:
                HSECFG_Capacitance(HSECap_16p);
                break;
            case 18:
                HSECFG_Capacitance(HSECap_18p);
                break;
            case 20:
                HSECFG_Capacitance(HSECap_20p);
                break;
            case 22:
                HSECFG_Capacitance(HSECap_22p);
                break;
            case 24:
                HSECFG_Capacitance(HSECap_24p);
                break;
            default:
                PRINT("Fatal: Invalid HSE capacitance!\n");
                while (1) {
                    __nop();
                }
        }
    }
#endif
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
    PRINT("Chip start, %s\n", VER_LIB);
    PRINT("Build on %s %s - " MACRO2STR(__GIT_VERSION__) "\n", __DATE__, __TIME__);
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
    setPinInputHigh(A8);
    setPinOutput(A9);
    UART1_DefInit();
    UART1_BaudRateCfg(DEBUG_BAUDRATE);

    char buffer[128];
    uint8_t len = sprintf(buffer, "Chipstart, %s\nBuild on %s %s - " MACRO2STR(__GIT_VERSION__) "\nReason of last reset: %d\n", VER_LIB, __DATE__, __TIME__, R8_RESET_STATUS & RB_RESET_FLAG);

    while (len) {
        if (R8_UART1_TFC != UART_FIFO_SIZE) {
            R8_UART1_THR = buffer[strlen(buffer) - len];
            len--;
        }
    }
    while ((R8_UART1_LSR & RB_LSR_TX_ALL_EMP) == 0) {
        __nop();
    }
#endif

#if !defined ESB_ENABLE || ESB_ENABLE != 2
    if (bootloader_boot_mode_get() != BOOTLOADER_BOOT_MODE_IAP) {
        jumpApp_prerequisite();
        jumpApp();
    }
#else
    // TODO: implement judging condition for 2.4g dongle
    if (1) {
        jumpApp_prerequisite();
        jumpApp();
    }
#endif

    uf2_init();

    usbd_desc_register(msc_ram_descriptor);
    usbd_add_interface(usbd_msc_init_intf(&intf0, MSC_OUT_EP, MSC_IN_EP));
    usbd_initialize();

    Main_Circulation();
}

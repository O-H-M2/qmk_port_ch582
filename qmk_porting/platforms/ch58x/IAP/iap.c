#include "iap.h"

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
            jumpPre;
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
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
    UART1_BaudRateCfg(460800);
#endif
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

#ifdef BOOTMAGIC_ENABLE
    PRINT("Bootmagic!\n");
#if !defined ESB_ENABLE || ESB_ENABLE != 2
    bool jump_app = false;
    pin_t rows[] = MATRIX_ROW_PINS;
    pin_t cols[] = MATRIX_COL_PINS;

#if DIODE_DIRECTION == COL2ROW
    pin_t input_pin = cols[BOOTMAGIC_LITE_COLUMN];
    pin_t output_pin = rows[BOOTMAGIC_LITE_ROW];
#else
    pin_t input_pin = rows[BOOTMAGIC_LITE_ROW];
    pin_t output_pin = cols[BOOTMAGIC_LITE_COLUMN];
#endif
    setPinInputHigh(input_pin);
    writePinLow(output_pin);
    setPinOutput(output_pin);
    do {
        if (readPin(input_pin)) {
            jump_app = true;
            break;
        }
        my_delay_ms(DEBOUNCE * 3);
        if (readPin(input_pin)) {
            jump_app = true;
            break;
        }
    } while (0);
    if (jump_app) {
        PRINT("Entering APP...\n");
        jumpApp();
    } else {
        PRINT("Entering DFU...\n");
        eeprom_driver_erase();
    }
#else
    // TODO: implement judging condition for 2.4g dongle
    jumpApp();
#endif
#else
    PRINT("Entering APP...\n");
    jumpApp();
#endif

    uf2_init();

    usbd_desc_register(msc_ram_descriptor);
    usbd_add_interface(usbd_msc_alloc_intf(MSC_OUT_EP, MSC_IN_EP));
    usbd_initialize();

    Main_Circulation();
}

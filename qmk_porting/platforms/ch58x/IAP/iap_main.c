#include "iap.h"

/*********************************************************************
 * @fn      SetSysClock
 *
 * @brief   配置系统运行时钟60Mhz, 0x48
 *
 * @param   sc      - 系统时钟源选择 refer to SYS_CLKTypeDef
 *
 * @return  none
 */
__HIGH_CODE void mySetSysClock()
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
}

/*********************************************************************
 * @fn      Main_Circulation
 *
 * @brief   IAP主循环,程序放ram中运行，提升速度.
 *
 * @param   None.
 *
 * @return  None.
 */
__HIGH_CODE void Main_Circulation()
{
    uint32_t j = 0;

    while (1) {
        j++;
        if (j > 5) //100us处理一次数据
        {
            j = 0;
            USB_DevTransProcess(); //采用查询方式进行usb操作，不使用中断。
        }
        DelayUs(20);
        g_tcnt++;
        if (g_tcnt > 3000000) {
            //1分钟没有操作，进入app
            jumpAppPre;
            jumpApp();
        }
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main()
{
#if (defined(DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    uint16_t adj = R16_AUX_POWER_ADJ;
    uint16_t plan = R16_POWER_PLAN;

    adj |= RB_DCDC_CHARGE;
    plan |= RB_PWR_DCDC_PRE;
    sys_safe_access_enable();
    R16_AUX_POWER_ADJ = adj;
    R16_POWER_PLAN = plan;
    DelayUs(10);
    sys_safe_access_enable();
    R16_POWER_PLAN |= RB_PWR_DCDC_EN;
    sys_safe_access_disable();
#endif
    mySetSysClock(); //为了精简程序体积，该函数比普通库的初始化函数有修改，只可以将时钟设置为60M
    DelayMs(5);
#ifdef PLF_DEBUG
    GPIOA_SetBits(GPIO_Pin_9);
    GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
    UART1_BaudRateCfg(460800);
#endif

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
        DelayMs(DEBOUNCE * 3);
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
#endif
#else
    PRINT("Entering APP...\n");
    jumpApp();
#endif

    /* USB初始化 */
    R8_USB_CTRL = 0x00; // 先设定模式,取消 RB_UC_CLR_ALL

    R8_UEP4_1_MOD = RB_UEP4_RX_EN | RB_UEP4_TX_EN | RB_UEP1_RX_EN | RB_UEP1_TX_EN; // 端点4 OUT+IN,端点1 OUT+IN
    R8_UEP2_3_MOD = RB_UEP2_RX_EN | RB_UEP2_TX_EN | RB_UEP3_RX_EN | RB_UEP3_TX_EN; // 端点2 OUT+IN,端点3 OUT+IN

    R16_UEP0_DMA = (uint16_t)(uint32_t)EP0_Databuf;
    R16_UEP1_DMA = (uint16_t)(uint32_t)EP1_Databuf;
    R16_UEP2_DMA = (uint16_t)(uint32_t)EP2_Databuf;
    R16_UEP3_DMA = (uint16_t)(uint32_t)EP3_Databuf;

    R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
    R8_UEP1_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
    R8_UEP2_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
    R8_UEP3_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK | RB_UEP_AUTO_TOG;
    R8_UEP4_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;

    R8_USB_DEV_AD = 0x00;
    R8_USB_CTRL = RB_UC_DEV_PU_EN | RB_UC_INT_BUSY | RB_UC_DMA_EN; // 启动USB设备及DMA，在中断期间中断标志未清除前自动返回NAK
    R16_PIN_ANALOG_IE |= RB_PIN_USB_IE | RB_PIN_USB_DP_PU;         // 防止USB端口浮空及上拉电阻
    R8_USB_INT_FG = 0xFF;                                          // 清中断标志
    R8_UDEV_CTRL = RB_UD_PD_DIS | RB_UD_PORT_EN;                   // 允许USB端口
    R8_USB_INT_EN = 0;                                             //禁止usb中断，采用查询方式

    /* 进入highcode主循环 */
    Main_Circulation();
}

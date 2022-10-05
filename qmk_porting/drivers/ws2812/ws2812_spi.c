#include "quantum.h"
#include "ws2812.h"

// Define SPI config speed
#ifndef WS2812_SPI_DIVISOR
#define WS2812_SPI_DIVISOR FREQ_SYS / 1000000 / 3.2 + 1 //target 3.2MHz
#endif

#define BYTES_FOR_LED_BYTE 4
#ifdef RGBW
#define WS2812_CHANNELS 4
#else
#define WS2812_CHANNELS 3
#endif
#define BYTES_FOR_LED (BYTES_FOR_LED_BYTE * WS2812_CHANNELS)
#define DATA_SIZE     (BYTES_FOR_LED * RGBLED_NUM)
#define RESET_SIZE    (1000 * WS2812_TRST_US / (2 * WS2812_TIMING))
#define PREAMBLE_SIZE 4

__attribute__((aligned(4))) static uint8_t txbuf[PREAMBLE_SIZE + DATA_SIZE + RESET_SIZE] = { 0 };
static volatile bool ws2812_inited = false, ws2812_powered_on = false;
static volatile uint8_t spi_transfering = false;

static void ws2812_init()
{
    // we have only one spi controller
    setPinOutput(RGB_DI_PIN);

    R8_SPI0_CLOCK_DIV = WS2812_SPI_DIVISOR;
    R8_SPI0_CTRL_MOD = RB_SPI_ALL_CLEAR;
    R8_SPI0_CTRL_MOD = RB_SPI_MOSI_OE;
    R8_SPI0_CTRL_CFG |= RB_SPI_AUTO_IF;
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
    PFIC_EnableIRQ(SPI0_IRQn);
    ws2812_inited = true;
}

__INTERRUPT __HIGH_CODE void SPI0_IRQHandler()
{
    R8_SPI0_INTER_EN = 0;
    if (R8_SPI0_INT_FLAG & RB_SPI_IF_CNT_END) {
        R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
        R8_SPI0_INT_FLAG |= RB_SPI_IF_CNT_END;
        spi_transfering = false;
    }
}

static void SPI0_StartDMA(uint8_t *pbuf, uint16_t len)
{
    if (!spi_transfering) {
        spi_transfering = true;
        R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
        R16_SPI0_DMA_BEG = (uint32_t)pbuf;
        R16_SPI0_DMA_END = (uint32_t)(pbuf + len);
        R16_SPI0_TOTAL_CNT = len;
        R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END | RB_SPI_IF_DMA_END;
        R8_SPI0_CTRL_CFG |= RB_SPI_DMA_ENABLE;
        R8_SPI0_INTER_EN |= RB_SPI_IF_CNT_END;
    }
}

/*
 * As the trick here is to use the SPI to send a huge pattern of 0 and 1 to
 * the ws2812b protocol, we use this helper function to translate bytes into
 * 0s and 1s for the LED (with the appropriate timing).
 */
static uint8_t get_protocol_eq(uint8_t data, int pos)
{
    uint8_t eq = 0;
    if (data & (1 << (2 * (3 - pos))))
        eq = 0b1110;
    else
        eq = 0b1000;
    if (data & (2 << (2 * (3 - pos))))
        eq += 0b11100000;
    else
        eq += 0b10000000;
    return eq;
}

static void set_led_color_rgb(LED_TYPE color, int pos)
{
    uint8_t *tx_start = &txbuf[PREAMBLE_SIZE];

#if (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_GRB)
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + j] = get_protocol_eq(color.g, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE + j] = get_protocol_eq(color.r, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE * 2 + j] = get_protocol_eq(color.b, j);
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_RGB)
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + j] = get_protocol_eq(color.r, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE + j] = get_protocol_eq(color.g, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE * 2 + j] = get_protocol_eq(color.b, j);
#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_BGR)
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + j] = get_protocol_eq(color.b, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE + j] = get_protocol_eq(color.g, j);
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE * 2 + j] = get_protocol_eq(color.r, j);
#endif
#ifdef RGBW
    for (int j = 0; j < 4; j++)
        tx_start[BYTES_FOR_LED * pos + BYTES_FOR_LED_BYTE * 4 + j] = get_protocol_eq(color.w, j);
#endif
}

void ws2812_setleds(LED_TYPE *ledarray, uint16_t leds)
{
    if (!rgbled_status_check()) {
        return;
    }

    if (!ws2812_inited) {
        ws2812_init();
    }
    if (!ws2812_powered_on) {
        ws2812_power_toggle(true);
    }

    for (uint8_t i = 0; i < leds; i++) {
        set_led_color_rgb(ledarray[i], i);
    }

    // Send async - each led takes ~0.03ms, 50 leds ~1.5ms, animations flushing faster than send will cause issues.
    // Instead spiSend can be used to send synchronously (or the thread logic can be added back).
    SPI0_StartDMA(txbuf, sizeof(txbuf) / sizeof(txbuf[0]));
}

__HIGH_CODE void ws2812_power_toggle(bool status)
{
    if (ws2812_powered_on == status) {
        return;
    }

    if (status) {
        writePin(WS2812_EN_PIN, WS2812_EN_LEVEL);
        setPinOutput(WS2812_EN_PIN);
        sys_safe_access_enable();
        R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_SPI0;
    } else {
#if WS2812_EN_LEVEL
        setPinInputLow(WS2812_EN_PIN);
#else
        setPinInputHigh(WS2812_EN_PIN);
#endif
        sys_safe_access_enable();
        R8_SLP_CLK_OFF1 |= RB_SLP_CLK_SPI1 | RB_SLP_CLK_SPI0;
    }
    ws2812_powered_on = status;
}

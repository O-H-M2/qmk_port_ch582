#include "quantum.h"
#include "ws2812.h"

/* Adapted from https://github.com/joewa/WS2812-LED-Driver_ChibiOS/ */

#ifdef RGBW
#define WS2812_CHANNELS 4
#else
#define WS2812_CHANNELS 3
#endif

#ifndef WS2812_PWM_DRIVER
#define WS2812_PWM_DRIVER 1 // TMR1
#define WS2812_DI_PIN     A10
#endif

static volatile bool ws2812_inited = false, ws2812_powered_on = false;

#if WS2812_PWM_DRIVER == 1
#define WS2812_PWM_CNT_END_REG R32_TMR1_CNT_END
#define WS2812_DMA_CONFIG(en, start, end)       \
    TMR1_DMACfg(en, (uint16_t)(uint32_t)&start, \
                (uint16_t)(uint32_t)&end, Mode_LOOP)
#define WS2812_PWM_INIT(level)          TMR1_PWMInit(level, PWM_Times_1);
#define WS2812_PWM_DMA_INTERRUPT_ENABLE PFIC_EnableIRQ(TMR1_IRQn);
#define WS2812_PWM_DMA_INTERRUPT_SET    TMR1_ITCfg(ENABLE, RB_TMR_IE_DMA_END);

__INTERRUPT __HIGH_CODE void TMR1_IRQHandler()
{
    R8_TMR1_INTER_EN = 0;
    TMR1_ClearITFlag(RB_TMR_IF_DMA_END);
    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF0 |= RB_SLP_CLK_TMR1;
        sys_safe_access_disable();
    } while (!(R8_SLP_CLK_OFF0 & RB_SLP_CLK_TMR1));
}
#elif WS2812_PWM_DRIVER == 2
#define WS2812_PWM_CNT_END_REG R32_TMR2_CNT_END
#define WS2812_DMA_CONFIG(en, start, end)       \
    TMR2_DMACfg(en, (uint16_t)(uint32_t)&start, \
                (uint16_t)(uint32_t)&end, Mode_LOOP)
#define WS2812_PWM_INIT(level)          TMR2_PWMInit(level, PWM_Times_1);
#define WS2812_PWM_DMA_INTERRUPT_ENABLE PFIC_EnableIRQ(TMR2_IRQn);
#define WS2812_PWM_DMA_INTERRUPT_SET    TMR2_ITCfg(ENABLE, RB_TMR_IE_DMA_END);

__INTERRUPT __HIGH_CODE void TMR2_IRQHandler()
{
    R8_TMR2_INTER_EN = 0;
    TMR2_ClearITFlag(RB_TMR_IF_DMA_END);
    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF0 |= RB_SLP_CLK_TMR2;
        sys_safe_access_disable();
    } while (!(R8_SLP_CLK_OFF0 & RB_SLP_CLK_TMR2));
}
#else
// Only TMR1 and TMR2 support DMA and PWM
// Only pins A10, A11. B10, B11 is used for USB.
#error Unsupported PWM Driver.
#endif

#ifndef WS2812_PWM_TARGET_PERIOD
// #    define WS2812_PWM_TARGET_PERIOD 800000 // Original code is 800k...?
#define WS2812_PWM_TARGET_PERIOD 80000 // TODO: work out why 10x less on f303/f4x1
#endif

/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define WS2812_PWM_FREQUENCY FREQ_SYS                                          /**< Clock frequency of PWM, must be valid with respect to system clock! */
#define WS2812_PWM_PERIOD    (WS2812_PWM_FREQUENCY / WS2812_PWM_TARGET_PERIOD) /**< Clock period in ticks. 1 / 800kHz = 1.25 uS (as per datasheet) */

/**
 * @brief   Number of bit-periods to hold the data line low at the end of a frame
 *
 * The reset period for each frame is defined in WS2812_TRST_US.
 * Calculate the number of zeroes to add at the end assuming 1.25 uS/bit:
 */
#define WS2812_COLOR_BITS  (WS2812_CHANNELS * 8)
#define WS2812_RESET_BIT_N (1000 * WS2812_TRST_US / WS2812_TIMING)
#define WS2812_COLOR_BIT_N (RGBLED_NUM * WS2812_COLOR_BITS)          /**< Number of data bits */
#define WS2812_BIT_N       (WS2812_COLOR_BIT_N + WS2812_RESET_BIT_N) /**< Total number of bits in a frame */

/**
 * @brief   High period for a zero, in ticks
 *
 * Per the datasheet:
 * WS2812:
 * - T0H: 200 nS to 500 nS, inclusive
 * - T0L: 650 nS to 950 nS, inclusive
 * WS2812B:
 * - T0H: 200 nS to 500 nS, inclusive
 * - T0L: 750 nS to 1050 nS, inclusive
 *
 * The duty cycle is calculated for a high period of 350 nS.
 */
#define WS2812_DUTYCYCLE_0 (WS2812_PWM_FREQUENCY / (1000000000 / 350))

/**
 * @brief   High period for a one, in ticks
 *
 * Per the datasheet:
 * WS2812:
 * - T1H: 550 nS to 850 nS, inclusive
 * - T1L: 450 nS to 750 nS, inclusive
 * WS2812B:
 * - T1H: 750 nS to 1050 nS, inclusive
 * - T1L: 200 nS to 500 nS, inclusive
 *
 * The duty cycle is calculated for a high period of 800 nS.
 * This is in the middle of the specifications of the WS2812 and WS2812B.
 */
#define WS2812_DUTYCYCLE_1 (WS2812_PWM_FREQUENCY / (1000000000 / 800))

/* --- PRIVATE MACROS ------------------------------------------------------- */

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given bit
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] byte:                 The byte number [0, 2]
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_BIT(led, byte, bit) (WS2812_COLOR_BITS * (led) + 8 * (byte) + (7 - (bit)))

#if (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_GRB)
/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given red bit
 *
 * @note    The red byte is the middle byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_RED_BIT(led, bit) WS2812_BIT((led), 1, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given green bit
 *
 * @note    The red byte is the first byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_GREEN_BIT(led, bit) WS2812_BIT((led), 0, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given blue bit
 *
 * @note    The red byte is the last byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit index [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_BLUE_BIT(led, bit) WS2812_BIT((led), 2, (bit))

#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_RGB)
/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given red bit
 *
 * @note    The red byte is the middle byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_RED_BIT(led, bit)   WS2812_BIT((led), 0, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given green bit
 *
 * @note    The red byte is the first byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_GREEN_BIT(led, bit) WS2812_BIT((led), 1, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given blue bit
 *
 * @note    The red byte is the last byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit index [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_BLUE_BIT(led, bit)  WS2812_BIT((led), 2, (bit))

#elif (WS2812_BYTE_ORDER == WS2812_BYTE_ORDER_BGR)
/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given red bit
 *
 * @note    The red byte is the middle byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_RED_BIT(led, bit)   WS2812_BIT((led), 2, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given green bit
 *
 * @note    The red byte is the first byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit number [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_GREEN_BIT(led, bit) WS2812_BIT((led), 1, (bit))

/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given blue bit
 *
 * @note    The red byte is the last byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref RGBLED_NUM)
 * @param[in] bit:                  The bit index [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_BLUE_BIT(led, bit)  WS2812_BIT((led), 0, (bit))
#endif

#ifdef RGBW
/**
 * @brief   Determine the index in @ref ws2812_frame_buffer "the frame buffer" of a given white bit
 *
 * @note    The white byte is the last byte in the color packet
 *
 * @param[in] led:                  The led index [0, @ref WS2812_LED_N)
 * @param[in] bit:                  The bit index [0, 7]
 *
 * @return                          The bit index
 */
#define WS2812_WHITE_BIT(led, bit) WS2812_BIT((led), 3, (bit))
#endif

/* --- PRIVATE VARIABLES ---------------------------------------------------- */

static uint32_t ws2812_frame_buffer[WS2812_BIT_N + 1]; /**< Buffer for a frame */

/* --- PUBLIC FUNCTIONS ----------------------------------------------------- */
/*
 * Gedanke: Double-buffer type transactions: double buffer transfers using two memory pointers for
the memory (while the DMA is reading/writing from/to a buffer, the application can
write/read to/from the other buffer).
 */

static void ws2812_init(void)
{
    // Initialize led frame buffer
    uint32_t i;

    for (i = 0; i < WS2812_COLOR_BIT_N; i++)
        ws2812_frame_buffer[i] = WS2812_DUTYCYCLE_0; // All color bits are zero duty cycle
    for (i = 0; i < WS2812_RESET_BIT_N; i++)
        ws2812_frame_buffer[i + WS2812_COLOR_BIT_N] = 0; // All reset bits are zero

    setPinOutput(WS2812_DI_PIN);

    WS2812_PWM_CNT_END_REG = WS2812_PWM_PERIOD;
    WS2812_DMA_CONFIG(ENABLE, ws2812_frame_buffer[0], ws2812_frame_buffer[WS2812_BIT_N + 1]);
    WS2812_PWM_INIT(High_Level);
    WS2812_PWM_DMA_INTERRUPT_ENABLE;
    ws2812_inited = true;
}

static void ws2812_write_led(uint16_t led_number, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
#if WS2812_PWM_DRIVER == 1
    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF0 &= ~RB_SLP_CLK_TMR1;
        sys_safe_access_disable();
    } while (R8_SLP_CLK_OFF0 & RB_SLP_CLK_TMR1);
#elif WS2812_PWM_DRIVER == 2
    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF0 &= ~RB_SLP_CLK_TMR2;
        sys_safe_access_disable();
    } while (R8_SLP_CLK_OFF0 & RB_SLP_CLK_TMR2);
#endif

    // Write color to frame buffer
    for (uint8_t bit = 0; bit < 8; bit++) {
        ws2812_frame_buffer[WS2812_RED_BIT(led_number, bit)] = ((r >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
        ws2812_frame_buffer[WS2812_GREEN_BIT(led_number, bit)] = ((g >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
        ws2812_frame_buffer[WS2812_BLUE_BIT(led_number, bit)] = ((b >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
#ifdef RGBW
        ws2812_frame_buffer[WS2812_WHITE_BIT(led_number, bit)] = ((w >> bit) & 0x01) ? WS2812_DUTYCYCLE_1 : WS2812_DUTYCYCLE_0;
#endif
    }
}

// Setleds for standard RGB
void ws2812_setleds(rgb_led_t *ledarray, uint16_t leds)
{
    if (!ws2812_inited) {
        ws2812_init();
    }
    if (!ws2812_powered_on) {
        ws2812_power_toggle(true);
    }

    for (uint16_t i = 0; i < leds; i++) {
#ifdef RGBW
        ws2812_write_led(i, ledarray[i].r, ledarray[i].g, ledarray[i].b, ledarray[i].w);
#else
        ws2812_write_led(i, ledarray[i].r, ledarray[i].g, ledarray[i].b, 0);
#endif
    }
    WS2812_PWM_DMA_INTERRUPT_SET;
}

void ws2812_power_toggle(bool status)
{
    if (ws2812_powered_on == status) {
        return;
    }

#ifdef WS2812_EN_PIN
    if (status) {
        writePin(WS2812_EN_PIN, WS2812_EN_LEVEL);
        setPinOutput(WS2812_EN_PIN);
    } else {
        writePin(WS2812_EN_PIN, WS2812_EN_LEVEL ? 0 : 1);
        setPinOutput(WS2812_EN_PIN);
    }
#endif

    ws2812_powered_on = status;
}

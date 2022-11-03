#include "quantum.h"
#include "spi_master.h"

static pin_t currentSlavePin = NO_PIN;
static volatile bool spi_delayed_stop = false;

static void spi_stop_internal()
{
    if (currentSlavePin != NO_PIN) {
        setPinInputHigh(currentSlavePin);
        // disable interrupt
        R8_SPI0_INTER_EN = 0;
        R8_SPI0_INT_FLAG = RB_SPI_IF_FST_BYTE | RB_SPI_IF_FIFO_OV | RB_SPI_IF_DMA_END | RB_SPI_IF_FIFO_HF | RB_SPI_IF_BYTE_END | RB_SPI_IF_CNT_END;
        // disable DMA
        R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
        // unload all the data from fifo
        R8_SPI0_CTRL_MOD |= RB_SPI_FIFO_DIR;
        while (R8_SPI0_FIFO_COUNT) {
            volatile uint8_t discard = R8_SPI0_FIFO;

            (void)discard;
        }

        sys_safe_access_enable();
        R8_SLP_CLK_OFF1 |= RB_SLP_CLK_SPI1 | RB_SLP_CLK_SPI0;
        sys_safe_access_disable();

        currentSlavePin = NO_PIN;
    }
}

void spi_init()
{
    static bool spi_initialized = false;

    if (spi_initialized) {
        return;
    }
#if SPI_ALTER
    R16_PIN_ALTERNATE |= RB_PIN_SPI0;
    setPinOutput(B13);
    setPinOutput(B14);
#else
    R16_PIN_ALTERNATE &= ~RB_PIN_SPI0;
    setPinOutput(A13);
    setPinOutput(A14);
#endif
    spi_stop();
    R8_SPI0_CTRL_MOD = RB_SPI_MOSI_OE | RB_SPI_SCK_OE;
    R8_SPI0_CTRL_CFG |= RB_SPI_AUTO_IF;
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
    PFIC_EnableIRQ(SPI0_IRQn);
    spi_initialized = true;
}

__INTERRUPT __HIGH_CODE void SPI0_IRQHandler()
{
    R8_SPI0_INTER_EN = 0;
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
    R8_SPI0_INT_FLAG |= RB_SPI_IF_CNT_END;
    if (spi_delayed_stop) {
        spi_stop_internal();
        spi_delayed_stop = false;
    }
#ifdef AW20216
    aw20216_delayed_power_off_excute();
#endif
}

bool spi_start(pin_t slavePin, bool lsbFirst, uint8_t mode, uint16_t divisor)
{
    if (currentSlavePin != NO_PIN) {
        PRINT("Former SPI transfer hasn't finished yet.\n");

        uint16_t timeout_timer = timer_read();

        while (!(R8_SPI0_INT_FLAG & RB_SPI_FREE)) {
            if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
                spi_stop_internal();
                PRINT("Timeout, skipping anyway...\n");
                break;
            }
        }
    } else if (slavePin == NO_PIN) {
        PRINT("Invalid SPI chip select pin!\n");
        return false;
    }

    sys_safe_access_enable();
    R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_SPI0;
    sys_safe_access_disable();

#ifdef AW20216
    AW20216_POWER_CHECK;
#endif

    currentSlavePin = slavePin;

    switch (mode) {
        case 0:
            if (lsbFirst) {
                // Mode0_LowBitINFront
                R8_SPI0_CTRL_MOD &= ~RB_SPI_MST_SCK_MOD;
                R8_SPI0_CTRL_CFG |= RB_SPI_BIT_ORDER;
            } else {
                // Mode0_HighBitINFront
                R8_SPI0_CTRL_MOD &= ~RB_SPI_MST_SCK_MOD;
                R8_SPI0_CTRL_CFG &= ~RB_SPI_BIT_ORDER;
            }
            break;
        case 1:
            PRINT("Unsupported SPI mode: %d\n", mode);
            return false;
        case 2:
            PRINT("Unsupported SPI mode: %d\n", mode);
            return false;
        case 3:
            if (lsbFirst) {
                // Mode3_LowBitINFront
                R8_SPI0_CTRL_MOD |= RB_SPI_MST_SCK_MOD;
                R8_SPI0_CTRL_CFG |= RB_SPI_BIT_ORDER;
            } else {
                // Mode3_HighBitINFront
                R8_SPI0_CTRL_MOD |= RB_SPI_MST_SCK_MOD;
                R8_SPI0_CTRL_CFG &= ~RB_SPI_BIT_ORDER;
            }
            break;
        default:
            PRINT("Unsupported SPI mode: %d\n", mode);
            return false;
    }

    if (divisor >= 2 && divisor <= 255) {
        R8_SPI0_CLOCK_DIV = divisor;
    } else {
        PRINT("Invalid SPI divisor!\n");
        return false;
    }

    writePinLow(currentSlavePin);
    setPinOutput(currentSlavePin);

    return true;
}

spi_status_t spi_write(uint8_t data)
{
    if (R8_SPI0_FIFO_COUNT) {
        return SPI_STATUS_ERROR;
    }

    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI0_BUFFER = data;

    uint16_t timeout_timer = timer_read();

    while (!(R8_SPI0_INT_FLAG & RB_SPI_FREE)) {
        if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
            return SPI_STATUS_TIMEOUT;
        }
    }

    return SPI_STATUS_SUCCESS;
}

spi_status_t spi_read()
{
    if (R8_SPI0_FIFO_COUNT) {
        return SPI_STATUS_ERROR;
    }

    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI0_BUFFER = 0xFF; // send a dummy byte

    uint16_t timeout_timer = timer_read();

    while (!(R8_SPI0_INT_FLAG & RB_SPI_FREE)) {
        if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
            return SPI_STATUS_TIMEOUT;
        }
    }

    return (R8_SPI0_BUFFER);
}

spi_status_t spi_transmit(const uint8_t *data, uint16_t length)
{
    if (length > 4095) {
        return SPI_STATUS_ERROR;
    }
    if (length == 1) {
        return spi_write(data[0]);
    }

    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;

    R16_SPI0_DMA_BEG = (uint32_t)data;
    R16_SPI0_DMA_END = (uint32_t)(data + length);
    R16_SPI0_TOTAL_CNT = length;
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END | RB_SPI_IF_DMA_END;

    R8_SPI0_INTER_EN |= RB_SPI_IF_CNT_END;
    R8_SPI0_CTRL_CFG |= RB_SPI_DMA_ENABLE;

    return SPI_STATUS_SUCCESS;
}

spi_status_t spi_receive(uint8_t *data, uint16_t length)
{
    if (length > 4095) {
        return SPI_STATUS_ERROR;
    }
    if (length == 1) {
        spi_status_t ret = spi_read();

        if (ret > 0) {
            data[0] = ret;
        }
        return ret;
    }

    R8_SPI0_CTRL_MOD |= RB_SPI_FIFO_DIR;

    R16_SPI0_DMA_BEG = (uint32_t)data;
    R16_SPI0_DMA_END = (uint32_t)(data + length);
    R16_SPI0_TOTAL_CNT = length;
    R8_SPI0_INT_FLAG = RB_SPI_IF_CNT_END | RB_SPI_IF_DMA_END;

    R8_SPI0_INTER_EN |= RB_SPI_IF_CNT_END;
    R8_SPI0_CTRL_CFG |= RB_SPI_DMA_ENABLE;

    return SPI_STATUS_SUCCESS;
}

void spi_stop()
{
    if (!(R8_SPI0_INT_FLAG & RB_SPI_FREE)) {
        spi_delayed_stop = true;
    } else {
        spi_stop_internal();
    }
}

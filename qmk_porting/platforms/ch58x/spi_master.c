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

#include "quantum.h"
#include "spi_master.h"

static volatile pin_t currentSlavePin = NO_PIN;
static volatile bool spi_transfering = false, spi_delayed_stop = false;

__attribute__((weak)) void spi_master_pre_transmit_cb()
{
}

__attribute__((weak)) __HIGH_CODE void spi_master_post_transmit_cb()
{
}

__HIGH_CODE static void spi_stop_internal()
{
    if (currentSlavePin != NO_PIN) {
        gpio_write_pin_high(currentSlavePin);
    }
    // disable interrupt
    R8_SPI0_INT_FLAG = RB_SPI_IF_FST_BYTE | RB_SPI_IF_FIFO_OV | RB_SPI_IF_DMA_END | RB_SPI_IF_FIFO_HF | RB_SPI_IF_BYTE_END | RB_SPI_IF_CNT_END;
    // disable DMA
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
    // unload all the data from fifo
    R8_SPI0_CTRL_MOD |= RB_SPI_FIFO_DIR;
    while (R8_SPI0_FIFO_COUNT) {
        volatile uint8_t discard = R8_SPI0_FIFO;

        (void)discard;
    }

    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF1 |= RB_SLP_CLK_SPI0;
        sys_safe_access_disable();
    } while (!(R8_SLP_CLK_OFF1 & RB_SLP_CLK_SPI0));

    currentSlavePin = NO_PIN;
}

void spi_init()
{
    static bool spi_initialized = false;

    if (spi_initialized) {
        return;
    }
#ifdef SPI_IO_REMAPPING
    R16_PIN_ALTERNATE |= RB_PIN_SPI0;
    gpio_set_pin_output(B13);
    gpio_set_pin_output(B14);
    gpio_set_pin_input_high(B15);
#else
    R16_PIN_ALTERNATE &= ~RB_PIN_SPI0;
    gpio_set_pin_output(A13);
    gpio_set_pin_output(A14);
    gpio_set_pin_input_high(A15);
#endif
    R8_SPI0_CTRL_MOD = RB_SPI_ALL_CLEAR;
    R8_SPI0_CTRL_MOD = RB_SPI_MOSI_OE | RB_SPI_SCK_OE;
    R8_SPI0_CTRL_CFG |= RB_SPI_AUTO_IF;
    R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
    PFIC_EnableIRQ(SPI0_IRQn);
    spi_initialized = true;
}

__INTERRUPT __HIGH_CODE void SPI0_IRQHandler()
{
    R8_SPI0_INTER_EN = 0;
    if (R8_SPI0_INT_FLAG & RB_SPI_IF_CNT_END) {
        R8_SPI0_CTRL_CFG &= ~RB_SPI_DMA_ENABLE;
        R8_SPI0_INT_FLAG |= RB_SPI_IF_CNT_END;
        if (spi_delayed_stop) {
            spi_stop_internal();
            spi_delayed_stop = false;
        }
        spi_master_post_transmit_cb();
        spi_transfering = false;
    }
}

bool spi_start(pin_t slavePin, bool lsbFirst, uint8_t mode, uint16_t divisor)
{
    if (currentSlavePin != NO_PIN) {
        PRINT("Former SPI transfer hasn't finished yet!\n");

        uint16_t timeout_timer = timer_read();

        while (currentSlavePin != NO_PIN) {
            if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
                PRINT("Timeout, skipping anyway...\n");
                spi_stop_internal();
                break;
            }
        }
    }
    if (slavePin == NO_PIN) {
        PRINT("Invalid SPI chip select pin!\n");
        return false;
    }

    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_SPI0;
        sys_safe_access_disable();
    } while (R8_SLP_CLK_OFF1 & RB_SLP_CLK_SPI0);

    switch (mode) {
        case 0:
            R8_SPI0_CTRL_MOD &= ~RB_SPI_MST_SCK_MOD;
            if (lsbFirst) {
                // Mode0_LowBitINFront
                R8_SPI0_CTRL_CFG |= RB_SPI_BIT_ORDER;
            } else {
                // Mode0_HighBitINFront
                R8_SPI0_CTRL_CFG &= ~RB_SPI_BIT_ORDER;
            }
            break;
        case 3:
            R8_SPI0_CTRL_MOD |= RB_SPI_MST_SCK_MOD;
            if (lsbFirst) {
                // Mode3_LowBitINFront
                R8_SPI0_CTRL_CFG |= RB_SPI_BIT_ORDER;
            } else {
                // Mode3_HighBitINFront
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

    spi_master_pre_transmit_cb();

    currentSlavePin = slavePin;
    gpio_write_pin_low(currentSlavePin);
    gpio_set_pin_output(currentSlavePin);

    return true;
}

spi_status_t spi_write(uint8_t data)
{
    uint16_t timeout_timer = timer_read();

    while (spi_transfering) {
        if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
            PRINT("SPI write timeout.\n");
            return SPI_STATUS_TIMEOUT;
        }
    }

    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI0_BUFFER = data;

    while (!(R8_SPI0_INT_FLAG & RB_SPI_FREE)) {
        if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
            PRINT("SPI write timeout.\n");
            return SPI_STATUS_TIMEOUT;
        }
    }

    return SPI_STATUS_SUCCESS;
}

spi_status_t spi_read()
{
    uint16_t timeout_timer = timer_read();

    while (spi_transfering) {
        if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
            PRINT("SPI read timeout.\n");
            return SPI_STATUS_TIMEOUT;
        }
    }

    R8_SPI0_CTRL_MOD &= ~RB_SPI_FIFO_DIR;
    R8_SPI0_BUFFER = 0xFF; // send a dummy byte

    while (!(R8_SPI0_INT_FLAG & RB_SPI_FREE)) {
        if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
            PRINT("SPI read timeout.\n");
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

    uint16_t timeout_timer = timer_read();

    while (spi_transfering) {
        if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
            PRINT("SPI transmit timeout.\n");
            return SPI_STATUS_TIMEOUT;
        }
    }

    spi_transfering = true;

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

    uint16_t timeout_timer = timer_read();

    while (spi_transfering) {
        if (timer_elapsed(timeout_timer) >= SPI_TIMEOUT) {
            PRINT("SPI receive timeout.\n");
            return SPI_STATUS_TIMEOUT;
        }
    }

    spi_transfering = true;

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
    if (currentSlavePin == NO_PIN) {
        PRINT("No ongoing SPI transmission, will skip.\n");
        return;
    }
    if (spi_transfering) {
        spi_delayed_stop = true;
    } else {
        spi_stop_internal();
    }
}

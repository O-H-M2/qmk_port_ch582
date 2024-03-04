/* Copyright 2022 OctopusZ
 * Copyright 2022 Huckies <https://github.com/Huckies>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the  terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* This library is only valid for STM32 processors.
 * This library follows the convention of the AVR i2c_master library.
 * As a result addresses are expected to be already shifted (addr << 1).
 * I2CD1 is the default driver which corresponds to pins B6 and B7. This
 * can be changed.
 * Please ensure that HAL_USE_I2C is TRUE in the halconf.h file and that
 * STM32_I2C_USE_I2C1 is TRUE in the mcuconf.h file. Pins B6 and B7 are used
 * but using any other I2C pins should be trivial.
 */

#include "i2c_master.h"
#include "quantum.h"

static uint8_t i2c_address;

static void i2c_power_toggle(bool state)
{
    if (state) {
        while (R8_SLP_CLK_OFF1 & RB_SLP_CLK_I2C) {
            sys_safe_access_enable();
            R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_I2C;
        }
        sys_safe_access_disable();
    } else {
        while (!(R8_SLP_CLK_OFF1 & RB_SLP_CLK_I2C)) {
            sys_safe_access_enable();
            R8_SLP_CLK_OFF1 |= RB_SLP_CLK_I2C;
        }
        sys_safe_access_disable();
    }
}

void i2c_init()
{
#ifdef I2C_IO_REMAPPING
    R16_PIN_ALTERNATE |= RB_PIN_I2C;
    gpio_set_pin_input_high(B20);
    gpio_set_pin_input_high(B21);
#else
    R16_PIN_ALTERNATE &= ~RB_PIN_I2C;
    gpio_set_pin_input_high(B12);
    gpio_set_pin_input_high(B13);
#endif
    I2C_Init(I2C_Mode_I2C, I2C_CLOCK_SPEED, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, TxAdderss);
    while (I2C_GetFlagStatus(I2C_FLAG_BUSY)) {
        __nop();
    }

    wait_us(200);
    i2c_power_toggle(false);
}

i2c_status_t i2c_start(uint8_t address, uint16_t timeout)
{
    i2c_power_toggle(true);

    uint16_t timeout_timer = timer_read();

    i2c_address = address;
    I2C_GenerateSTART(ENABLE);

    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    return I2C_STATUS_SUCCESS;
}

void i2c_stop()
{
    I2C_GenerateSTOP(ENABLE);
}

i2c_status_t i2c_transmit(uint8_t address, const uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_status_t status = i2c_start(address, timeout);

    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    uint16_t timeout_timer = timer_read();

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        I2C_SendData(data[i]);
    }

    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    i2c_stop();

    wait_us(200);
    i2c_power_toggle(false);

    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_receive(uint8_t address, uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_status_t status = i2c_start(address, timeout);

    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    uint16_t timeout_timer = timer_read();

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) {
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        if (i == length - 1) {
            I2C_AcknowledgeConfig(DISABLE);
            i2c_stop();
        }
        data[i] = I2C_ReceiveData();
    }

    I2C_AcknowledgeConfig(ENABLE);

    wait_us(200);
    i2c_power_toggle(false);

    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, const uint8_t *data, uint16_t length, uint16_t timeout)
{
    uint8_t buffer[length + 1];

    buffer[0] = regaddr;
    for (uint16_t i = 0; i < length; i++) {
        buffer[i + 1] = data[i];
    }

    return i2c_transmit(devaddr, (const uint8_t *)buffer, length + 1, timeout);
}

i2c_status_t i2c_writeReg16(uint8_t devaddr, uint16_t regaddr, const uint8_t *data, uint16_t length, uint16_t timeout)
{
    uint8_t buffer[length + 2];

    buffer[0] = (uint8_t)((regaddr & 0xFF00) >> 8);
    buffer[1] = (uint8_t)(regaddr & 0xFF);
    for (uint16_t i = 0; i < length; i++) {
        buffer[i + 2] = data[i];
    }

    return i2c_transmit(devaddr, (const uint8_t *)buffer, length + 2, timeout);
}

i2c_status_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_status_t status = i2c_start(devaddr, timeout);

    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    uint16_t timeout_timer = timer_read();

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData(regaddr);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    // just resend a start signal to initiate read procedure
    status = i2c_start(devaddr, timeout - timer_elapsed(timeout_timer));
    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_GenerateSTOP(DISABLE);

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) {
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        if (i == length - 1) {
            I2C_AcknowledgeConfig(DISABLE);
            i2c_stop();
        }
        data[i] = I2C_ReceiveData();
    }

    I2C_AcknowledgeConfig(ENABLE);

    wait_us(200);
    i2c_power_toggle(false);

    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_readReg16(uint8_t devaddr, uint16_t regaddr, uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_status_t status = i2c_start(devaddr, timeout);

    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    uint16_t timeout_timer = timer_read();

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData((uint8_t)((regaddr & 0xFF00) >> 8));
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData((uint8_t)(regaddr & 0xFF));
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    // just resend a start signal to initiate read procedure
    status = i2c_start(devaddr, timeout - timer_elapsed(timeout_timer));
    if (status != I2C_STATUS_SUCCESS) {
        return status;
    }

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Receiver);
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_GenerateSTOP(DISABLE);

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) {
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        if (i == length - 1) {
            I2C_AcknowledgeConfig(DISABLE);
            i2c_stop();
        }
        data[i] = I2C_ReceiveData();
    }

    I2C_AcknowledgeConfig(ENABLE);

    wait_us(200);
    i2c_power_toggle(false);

    return I2C_STATUS_SUCCESS;
}
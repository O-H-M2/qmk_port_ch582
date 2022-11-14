/* Copyright 2022 OctopusZ
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
#define TxAdderss 0x52
#ifndef I2C_IO_REMAPPING
#define I2C_IO_REMAPPING 0
#endif
static uint8_t i2c_address;
void i2c_init(void)
{
#ifndef I2C_IO_REMAPPING
    GPIOB_ModeCfg(GPIO_Pin_21 | GPIO_Pin_20, GPIO_ModeIN_PU);
    R16_PIN_ALTERNATE |= RB_PIN_I2C;
#else
    GPIOB_ModeCfg(GPIO_Pin_13 | GPIO_Pin_12, GPIO_ModeIN_PU);
#endif
    I2C_Init(I2C_Mode_I2C, 400000, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, TxAdderss);
}

i2c_status_t i2c_start(uint8_t address, uint16_t timeout)
{
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

void i2c_stop(void)
{
    I2C_GenerateSTOP(ENABLE);
}

i2c_status_t i2c_transmit(uint8_t address, const uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_address = address;
    i2c_status_t status = i2c_start(i2c_address, timeout);

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter); //发送地址+最低位0表示为“写”
    uint16_t timeout_timer = timer_read();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) { //获取TxE的状态    数据寄存器为空标志位,可以向其中写数据
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        I2C_SendData(data[i]); //发送数据
    }

    return status;
}

i2c_status_t i2c_receive(uint8_t address, uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_address = address;
    i2c_start(i2c_address, timeout);

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter); //发送地址+最低位0表示为“写”
    uint16_t timeout_timer = timer_read();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    for (uint8_t i = 0; i < length; i++) {
        if (i == length - 1)
            I2C_AcknowledgeConfig(DISABLE);
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) { //获取RxEN的状态，等待收到数据
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
            data[i] = I2C_ReceiveData(); //获得从机的寄存器中的数据
        }

        I2C_GenerateSTOP(ENABLE);      //使能停止信号
        I2C_AcknowledgeConfig(ENABLE); //传输完毕，再次打开ACK使能
        return I2C_STATUS_SUCCESS;
    }
}
i2c_status_t i2c_writeReg(uint8_t devaddr, uint8_t regaddr, const uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_address = devaddr;
    i2c_start(i2c_address, timeout);

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter); //发送地址+最低位0表示为“写”
    uint16_t timeout_timer = timer_read();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_SendData(regaddr); //发送寄存器地址
    timeout_timer = timer_read();
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) { //获取TxE的状态    数据寄存器为空标志位,可以向其中写数据
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        I2C_SendData(data[i]); //发送数据
    }

    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_writeReg16(uint8_t devaddr, uint16_t regaddr, const uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_address = devaddr;
    i2c_start(i2c_address, timeout);

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter); //发送地址+最低位0表示为“写”
    uint16_t timeout_timer = timer_read();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_SendData((uint8_t)(regaddr >> 8)); //发送寄存器地址
    timeout_timer = timer_read();
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    I2C_SendData(regaddr); //发送寄存器地址
    timeout_timer = timer_read();
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) { //获取TxE的状态    数据寄存器为空标志位,可以向其中写数据
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
        }
        I2C_SendData(data[i]); //发送数据
    }

    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_address = devaddr;
    i2c_start(i2c_address, timeout);

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter); //发送地址+最低位0表示为“写”
    uint16_t timeout_timer = timer_read();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData(regaddr);                     //发送内存地址
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) { //获取TxE的状态
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    i2c_start(i2c_address, timeout);                        //重起始信号
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) { //判断BUSY, MSL and SB flags
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_Send7bitAddress(regaddr, I2C_Direction_Receiver);
    timeout_timer = timer_read();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        if (i == length - 1)
            I2C_AcknowledgeConfig(DISABLE);
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) {
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
            data[i] = I2C_ReceiveData(); //获得从机的寄存器中的数据
        }
    }
    return I2C_STATUS_SUCCESS;
}

i2c_status_t i2c_readReg16(uint8_t devaddr, uint16_t regaddr, uint8_t *data, uint16_t length, uint16_t timeout)
{
    i2c_address = devaddr;
    i2c_start(i2c_address, timeout);

    I2C_Send7bitAddress(i2c_address, I2C_Direction_Transmitter); //发送地址+最低位0表示为“写”
    uint16_t timeout_timer = timer_read();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData((uint8_t)(regaddr >> 8));     //发送内存地址
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) { //获取TxE的状态
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_SendData((uint8_t)regaddr);            //发送内存地址
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) { //获取TxE的状态
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    i2c_start(i2c_address, timeout);                        //重起始信号
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) { //判断BUSY, MSL and SB flags
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }
    I2C_Send7bitAddress(regaddr, I2C_Direction_Receiver); //发送地址+最低位1表示为“读”
    timeout_timer = timer_read();
    while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
        if ((timer_elapsed(timeout_timer) > timeout)) {
            return I2C_STATUS_TIMEOUT;
        }
    }

    for (uint8_t i = 0; i < length; i++) {
        if (i == length - 1)
            I2C_AcknowledgeConfig(DISABLE);
        while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) { //获取RxEN的状态，等待收到数据
            if ((timer_elapsed(timeout_timer) > timeout)) {
                return I2C_STATUS_TIMEOUT;
            }
            data[i] = I2C_ReceiveData(); //获得从机的寄存器中的数据
        }
    }
    return I2C_STATUS_SUCCESS;
}
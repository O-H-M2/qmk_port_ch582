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

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "compile_date.h"
#include "battery_measure.h"
#include "gpio.h"
#include "eeprom_driver.h"
#include "config.h"
#include "usbd_core.h"
#include "usbd_msc.h"
#include "uf2.h"
#include "usb_ch58x_usbfs_reg.h"

#define jumpApp ((void (*)(void))((int *)APP_CODE_START_ADDR))

#define MSC_IN_EP          0x81
#define MSC_OUT_EP         0x02
#define USBD_VID           VENDOR_ID
#define USBD_PID           PRODUCT_ID
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define USB_CONFIG_SIZE (9 + MSC_DESCRIPTOR_LEN)

void my_memcpy(void *dst, const void *src, uint32_t l);
void my_memset(void *dst, int c, uint32_t n);
uint32_t my_get_sys_clock();
void board_flash_init();
uint32_t board_flash_size();
void board_flash_read(uint32_t addr, void *buffer, uint32_t len);
void board_flash_flush();
void board_flash_write(uint32_t addr, void const *data, uint32_t len);

__attribute__((always_inline)) inline void jumpApp_Pre()
{
#if FREQ_SYS != 60000000
    WAIT_FOR_DBG;
    SetSysClock(Fsys);
    DelayMs(5);
#ifdef PLF_DEBUG
    DBG_BAUD_RECONFIG;
#else
    UART1_BaudRateCfg(DEBUG_BAUDRATE);
#endif
#endif
    PRINT("Current system clock: %d Hz\n", my_get_sys_clock());
}

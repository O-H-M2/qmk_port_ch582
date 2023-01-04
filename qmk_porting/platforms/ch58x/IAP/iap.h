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
#include "bootloader.h"
#include "config.h"
#include "usbd_core.h"
#include "usbd_msc.h"
#include "uf2.h"
#include "bootutil/bootutil.h"
#include "bootutil/image.h"
#include "usb_ch58x_usbfs_reg.h"

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

__attribute__((always_inline)) inline void iap_cleanup()
{
    PFIC_DisableIRQ(USB_IRQn);
    R16_PIN_ANALOG_IE &= ~(RB_PIN_USB_IE | RB_PIN_USB_DP_PU);
    R32_USB_CONTROL = 0;
    R8_USB_CTRL |= RB_UC_RESET_SIE | RB_UC_CLR_ALL;
    my_delay_ms(10);
    R8_USB_CTRL &= ~(RB_UC_RESET_SIE | RB_UC_CLR_ALL);
}

__attribute__((always_inline)) inline void jumpApp()
{
    struct boot_rsp rsp;
    fih_int rc = boot_go(&rsp);

    if (rc == 0) {
        uint32_t image_off = rsp.br_image_off, header_size = rsp.br_hdr->ih_hdr_size;

        PRINT("Image found.\n");
#if FREQ_SYS != 60000000
        WAIT_FOR_DBG;
        SetSysClock(Fsys);
        DelayMs(5);
#ifdef PLF_DEBUG
        DBG_BAUD_RECONFIG;
#else
        UART1_BaudRateCfg(DEBUG_BAUDRATE);
#endif
        PRINT("Resetting system clock to %d Hz before entering APP.\n", my_get_sys_clock());
#endif
        WAIT_FOR_DBG;
        ((void (*)(void))((int *)(image_off + header_size)))();
    } else {
        PRINT("Failed searching for a bootable image.\n");
        bootloader_boot_mode_set(BOOTLOADER_BOOT_MODE_IAP_ONGOING);
        WAIT_FOR_DBG;
        mcu_reset();
    }

    while (1) {
        __nop();
    }
    __builtin_unreachable();
}

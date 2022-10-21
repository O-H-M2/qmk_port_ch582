#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "compile_date.h"
#include "gpio.h"
#include "eeprom_driver.h"
#include "config.h"
#include "usbd_core.h"
#include "usbd_msc.h"
#include "uf2.h"

#define jumpPre                                               \
    PRINT("Leaving DFU...\n");                                \
    PFIC_DisableIRQ(USB_IRQn);                                \
    R16_PIN_ANALOG_IE &= ~(RB_PIN_USB_IE | RB_PIN_USB_DP_PU); \
    R32_USB_CONTROL = 0;                                      \
    R8_USB_CTRL |= RB_UC_RESET_SIE | RB_UC_CLR_ALL;           \
    my_delay_ms(10);                                          \
    R8_USB_CTRL &= ~(RB_UC_RESET_SIE | RB_UC_CLR_ALL);
#define jumpIAP ((void (*)(void))((int *)IAP_CODE_START_ADDR))
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
void board_flash_init();
uint32_t board_flash_size();
void board_flash_read(uint32_t addr, void *buffer, uint32_t len);
void board_flash_flush();
void board_flash_write(uint32_t addr, void const *data, uint32_t len);

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "eeprom_driver.h"

/* you can change the following definitions below, just keep them same in app and iap. */
#define APP_CODE_START_ADDR 0x00007000
#define APP_CODE_END_ADDR   0x00070000

#define USE_EEPROM_FLAG 1

#define jumpAppPre                                            \
    PRINT("Leaving DFU...\n");                                \
    R16_PIN_ANALOG_IE &= ~(RB_PIN_USB_IE | RB_PIN_USB_DP_PU); \
    R32_USB_CONTROL = 0;                                      \
    R8_USB_CTRL |= RB_UC_RESET_SIE | RB_UC_CLR_ALL;           \
    DelayMs(10);                                              \
    R8_USB_CTRL &= ~(RB_UC_RESET_SIE | RB_UC_CLR_ALL);
#define jumpApp ((void (*)(void))((int *)APP_CODE_START_ADDR))

#define FLAG_USER_CALL_IAP 0x55
#define FLAG_USER_CALL_APP 0xaa

/* you should not change the following definitions below. */
#define CMD_IAP_PROM   0x80
#define CMD_IAP_ERASE  0x81
#define CMD_IAP_VERIFY 0x82
#define CMD_IAP_END    0x83

/* usb data length is 64 */
#define IAP_LEN 64

typedef union _IAP_CMD_STRUCT {
    struct
    {
        uint8_t cmd;
        uint8_t len;
        uint8_t addr[4];
    } erase;
    struct
    {
        uint8_t cmd;
        uint8_t len;
        uint8_t status[2];
    } end;
    struct
    {
        uint8_t cmd;
        uint8_t len;
        uint8_t addr[4];
        uint8_t buf[IAP_LEN - 6];
    } verify;
    struct
    {
        uint8_t cmd;
        uint8_t len;
        uint8_t buf[IAP_LEN - 2];
    } program;
    struct
    {
        uint8_t buf[IAP_LEN];
    } other;
} iap_cmd_t;

extern uint8_t EP0_Databuf[64 + 64 + 64]; //ep0(64)+ep4_out(64)+ep4_in(64)
extern uint8_t EP1_Databuf[64 + 64];      //ep1_out(64)+ep1_in(64)
extern uint8_t EP2_Databuf[64 + 64];      //ep2_out(64)+ep2_in(64)
extern uint8_t EP3_Databuf[64 + 64];      //ep3_out(64)+ep3_in(64)
extern uint32_t g_tcnt;

void myDevEP2_IN_Deal(uint8_t s);
void myDevEP2_OUT_Deal(uint8_t l);

extern void my_memcpy(void *dst, const void *src, uint32_t l);
extern void USB_DevTransProcess(void);

#include "uart.h"

void uart_init(uint32_t baud)
{
    uint32_t x;

    x = 10 * GetSysClock() / 8 / baud;
    x = (x + 5) / 10;
    R16_UART0_DL = (uint16_t)x;

    R8_UART0_FCR = (2 << 6) | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN;
    R8_UART0_LCR = RB_LCR_WORD_SZ;
    R8_UART0_IER = RB_IER_TXD_EN;
    R8_UART0_DIV = 1;
}

void uart_start()
{
    writePinHigh(B7);
    setPinInputHigh(B4);
    setPinOutput(B7);
    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF0 &= ~RB_SLP_CLK_UART0;
        sys_safe_access_enable();
    } while (R8_SLP_CLK_OFF0 & RB_SLP_CLK_UART0);
}

void uart_stop()
{
    setPinInputHigh(B7);
    setPinInputHigh(B4);
    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF0 |= RB_SLP_CLK_UART0;
        sys_safe_access_enable();
    } while (~(R8_SLP_CLK_OFF0 & RB_SLP_CLK_UART0));
}

void uart_write(uint8_t data)
{
    while (R8_UART0_TFC == UART_FIFO_SIZE) {
        __nop();
    }

    R8_UART0_THR = data;
}

uint8_t uart_read()
{
    if (R8_UART0_RFC) {
        return R8_UART0_RBR;
    }

    return 0;
}

void uart_transmit(const uint8_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        uart_write(data[i]);
    }
}

void uart_receive(uint8_t *data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        data[i] = uart_read();
    }
}

bool uart_available()
{
    return !(!R8_UART0_RFC);
}

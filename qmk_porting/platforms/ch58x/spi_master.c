#include "quantum.h"
#include "spi_master.h"

spi_status_t spi_status;
static volatile bool spi_initialized = false, spi_transfering = false;

void spi_init(bool pin_alter)
{
    if (spi_initialized) {
        return;
    }
#if SPI_ALTER
    R16_PIN_ALTERNATE |= RB_PIN_SPI0;
    setPinOutput(B13 | B14);
#else
    R16_PIN_ALTERNATE &= ~RB_PIN_SPI0;
    setPinOutput(A13 | A14);
#endif
}

bool spi_start(pin_t slavePin, bool lsbFirst, uint8_t mode, uint16_t divisor)
{
}

spi_status_t spi_write(uint8_t data)
{
}

spi_status_t spi_read()
{
}

spi_status_t spi_transmit(const uint8_t *data, uint16_t length)
{
}

spi_status_t spi_receive(uint8_t *data, uint16_t length)
{
}

void spi_stop()
{
    if (!spi_initialized) {
        return;
    }
}

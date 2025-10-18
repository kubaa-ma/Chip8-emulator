#include "spi.h"

void spi_init(void) {
    SPI_DDR |= _BV(SPI_SCK) | _BV(SPI_MOSI) | _BV(SPI_SS);
    SPI_DDR |= _BV(SPI_MISO);
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);
}

uint8_t spi_transfer(uint8_t byte) {
    SPDR = byte;

    loop_until_bit_is_set(SPSR, SPIF);

    return SPDR;
}

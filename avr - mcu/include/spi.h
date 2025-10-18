#pragma once

#include <avr/io.h>

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_SCK PB5
#define SPI_MISO PB4
#define SPI_MOSI PB3
#define SPI_SS PB2

#define spi_slave_select() SPI_PORT &= ~_BV(SPI_SS)
#define spi_slave_deselect() SPI_PORT |= _BV(SPI_SS)

void spi_init(void);
uint8_t spi_transfer(uint8_t byte);

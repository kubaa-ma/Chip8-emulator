#ifndef STORAGE_H
#define STORAGE_H

#include <stdint.h>

// SPI funkce
void SPI_init(void);
uint8_t SPI_transfer(uint8_t data);


#define SD_CS_PIN PB2
#define SD_SELECT() PORTB &= ~(1<<SD_CS_PIN)
#define SD_DESELECT() PORTB |= (1<<SD_CS_PIN)

#endif

#include "storage.h"
#include "ff.h" 
#include <avr/io.h>
#include <util/delay.h>

void SPI_init(void) {
    DDRB |= (1<<PB3)|(1<<PB5)|(1<<SD_CS_PIN);
    DDRB &= ~(1<<PB4);                       
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);     
    SD_DESELECT();                           
}

uint8_t SPI_transfer(uint8_t data) {
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

DWORD get_fattime(void) {
    return ((DWORD)(2025-1980) << 25)
         | ((DWORD)10 << 21)
         | ((DWORD)18 << 16)
         | ((DWORD)12 << 11)
         | ((DWORD)0 << 5)
         | ((DWORD)0 >> 1);
}

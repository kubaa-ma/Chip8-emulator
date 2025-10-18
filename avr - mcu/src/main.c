#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "oled.h"
#include "sdcard.h"
#include "spi.h"

void central_init(void){
    TWI_init();
    USART_Init(MYUBRR);   
    Oled_init();
    spi_init();          
}

void usart_print_u8(uint8_t byte) {
    /* Converts a byte to a string of decimal text, sends it */
    UART_transmit('0' + (byte / 100));       /* Hundreds */
    UART_transmit('0' + ((byte / 10) % 10)); /* Tens */
    UART_transmit('0' + (byte % 10));        /* Ones */
}

int main(void){

    _delay_ms(102);

    central_init();
    uint8_t display[OLED_SIZE];
    memset(display, 0xFF, OLED_SIZE);
    Oled_draw_buffer(display);
    SdCard sd = {0};

        if (sdcard_init(&sd)) {
        printUsart("OK successfully initialized card");
    } else {
        printUsart("ERR failed initialize card");

        while(1);
    }


    uint8_t block[512];

    for (uint8_t i = 0; i < 10; ++i) {
        printUsart("Block 0x");
        usart_print_u8(i);
        printUsart(":");

        if (!sdcard_read_block(&sd, i, block)) {
            printUsart("ERR cannot read 512 bytes from 0x00");

            for (;;);
        }

        for (uint16_t i = 0; i < 512; ++i) {
            usart_print_u8(block[i]);
        }

        printUsart("");
    }

    while(1);

    return 0;
}

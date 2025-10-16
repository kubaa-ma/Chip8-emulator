#include <avr/io.h>
#include <util/delay.h>
#include "oled.h"
#include "usart.h"

void central_init(){
    TWI_init();
    USART_Init(MYUBRR);
    Oled_init();
}



int main(){
    central_init();
    uint8_t data[OLED_SIZE];


    for (uint8_t page = 0; page < 8; page++) {
        for (uint8_t col = 0; col < 128; col++) {
            if(page % 2 == 0 && col % 8 == 0)
                data[page*128 + col] = 0xFF;
            else{
                data[page*128 + col] = 0x00;
            }
        }
    }
    Oled_draw_buffer(data);


    while(1);


    return 0;
}
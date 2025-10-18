#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "oled.h"
#include "usart.h"
#include "cpu.h"

void central_init(){
    TWI_init();
    USART_Init(MYUBRR);
    Oled_init();
    
}



int main(){
    uint8_t DisplayOled[OLED_SIZE];
    memset(DisplayOled, 0, OLED_SIZE);
    cpu Cpu;
    init_cpu(&Cpu);
    central_init();
    Oled_draw_buffer(DisplayOled);
    
    _delay_ms(100);



    
    
    while(1){
        execute_opcode(&Cpu);
        _delay_ms(10);

    }


    return 0;
}
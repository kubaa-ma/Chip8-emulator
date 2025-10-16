#include "usart.h"

volatile uint8_t data_uart = 1;

void USART_Init(uint16_t ubrr){

    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0) | (1<<RXCIE0);
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void UART_transmit(uint8_t d){
    while(!(UCSR0A & (1 << UDRE0)));

    UDR0 = d;
}

ISR(USART_RX_vect){
    data_uart = UDR0;
}
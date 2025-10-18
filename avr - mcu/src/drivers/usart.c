#include "usart.h"

volatile uint8_t data_uart = 1;

void USART_Init(uint16_t ubrr){
    UBRR0H = (uint8_t)(ubrr>>8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
    UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00);
}

void UART_transmit(uint8_t d){
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = d;
}

void printUsart(const char *text){
    while(*text)
        UART_transmit(*text++);
}

void uart_print(const char *text) {
    while (*text) {
        UART_transmit(*text++);
    }
}

void uart_println(const char *text) {
    uart_print(text);
    UART_transmit('\r');
    UART_transmit('\n');
}

void uart_print_hex(uint8_t value) {
    const char hex[] = "0123456789ABCDEF";
    UART_transmit(hex[(value >> 4) & 0x0F]); 
    UART_transmit(hex[value & 0x0F]);         
}


ISR(USART0_RX_vect){
    data_uart = UDR0;
}

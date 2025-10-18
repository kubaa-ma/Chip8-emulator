#pragma once

#include <avr/interrupt.h>

#ifndef F_CPU
#warning "F_CPU not defined"
#endif

#define BAUD 9600
#define MYUBRR ((F_CPU/16/BAUD)-1)

extern volatile uint8_t data_uart;

void USART_Init(uint16_t ubrr);
void UART_transmit(uint8_t d);
void printUsart(const char *text);
void uart_print(const char *text);
void uart_println(const char *text);
void uart_print_hex(uint8_t value);

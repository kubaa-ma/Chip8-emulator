#pragma once

#include <avr/io.h>

#ifndef F_CPU
#warning "F_CPU not defined"
#endif

#define SCL PC5
#define SDA PC4

#define SLAVE_ADDR 0x3C
#define OLED_SIZE 1024

void TWI_init();
void TWI_start();
void TWI_stop();
void TWI_write(uint8_t data);
uint8_t TWI_read_ACK();
uint8_t TWI_read_NACK();

void Oled_send_commands(const uint8_t *cmds, uint8_t len);
void Oled_init();
void Oled_draw_buffer(uint8_t *buff);
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROM_PATH "roms/games/Tetris [Fran Dachille, 1991].ch8"
#define MEMORY_SIZE 4096
#define PROGRAM_START 0x200

#define FONTBYTES_PER_CHAR 5
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE DISPLAY_WIDTH * DISPLAY_HEIGHT\

#define V_REGISTERS 16
#define STACK 16
#define KEYS 16
#define FONT_START_ADDRESS 0x50


#define UNKNOWN_OPCDE printf("Unknown opcode: 0x%X\n", opcode)\

typedef struct cpu{
    uint8_t memory[MEMORY_SIZE];
    uint8_t V[V_REGISTERS];

    uint16_t I;
    uint16_t PC;
    
    uint16_t stack[STACK];
    uint8_t SP;
    
    uint8_t sound_timer;
    uint8_t delay_timer;

    uint8_t display[DISPLAY_SIZE];
    bool render; //zahajit vykreslovani
    
    uint8_t keypad[KEYS];
    bool waiting_key_press;
}cpu;

void init_cpu(cpu *data_cpu);
int load_rom(uint8_t *memory);
void Cpu_dump(cpu Cpu);
void execute_opcode(cpu *Cpu);
void sprite_draw(cpu *Cpu, uint8_t Vx, uint8_t Vy, uint8_t height);
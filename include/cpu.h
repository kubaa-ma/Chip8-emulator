#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ROM_PATH "roms/games/15 Puzzle [Roger Ivie].ch8"
#define MEMORY_SIZE 4096
#define PROGRAM_START 0x200

#define FONTBYTES_PER_CHAR 5
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE DISPLAY_WIDTH * DISPLAY_HEIGHT\

#define UNKNOWN_OPCDE printf("Unknown opcode: 0x%X\n", opcode)\

typedef struct cpu{
    uint8_t memory[MEMORY_SIZE];
    uint8_t V[16];

    uint16_t I;
    uint16_t PC;
    
    uint16_t stack[16];
    uint8_t SP;
    
    uint8_t sound_timer;
    uint8_t delay_timer;

    uint8_t display[DISPLAY_SIZE];
    bool render; //zahajit vykreslovani
    
    uint8_t keypad[16];
}cpu;

void init_cpu(cpu *data_cpu);
int load_rom(uint8_t *memory);
void Cpu_dump(cpu Cpu);
void execute_opcode(cpu *Cpu);
void sprite_draw(cpu *Cpu, uint8_t Vx, uint8_t Vy, uint8_t height);
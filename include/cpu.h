#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ROM_PATH "roms/games/15 Puzzle [Roger Ivie].ch8"
#define MEMORY_SIZE 4096
#define PROGRAM_START 0x200

#define DISPLAY_SIZE 64 * 32\

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
}cpu;

void init_cpu(cpu *data_cpu);

int load_rom(uint8_t *memory);

void Cpu_dump(cpu Cpu);

bool execute_opcode(cpu *Cpu);
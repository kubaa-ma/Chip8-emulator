#include "../include/cpu.h"


void init_cpu(cpu *Cpu) {
    memset(Cpu->memory, 0, sizeof(Cpu->memory));
    memset(Cpu->V, 0, sizeof(Cpu->V));
    memset(Cpu->stack, 0, sizeof(Cpu->stack));
    Cpu->I = 0;
    Cpu->SP = 0;
    Cpu->PC = PROGRAM_START;
    Cpu->sound_timer = 0;
    Cpu->delay_timer = 0;
}


int load_rom(uint8_t *memory) {
    FILE *f = fopen(ROM_PATH, "rb");
    if (f == NULL) {
        printf("Error: file not found\n");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);
    rewind(f);

    if (file_size > MEMORY_SIZE - PROGRAM_START) {
        printf("Error: ROM too large (%zu bytes)\n", file_size);
        fclose(f);
        return 1;
    }

    size_t bytes_read = fread(&memory[PROGRAM_START], 1, file_size, f);
    fclose(f);

    printf("ROM loaded successfully (%zu bytes)\n", bytes_read);
    return 0;
}



void Cpu_dump(cpu Cpu){
    printf("---");
    printf("Program Counter: 0x%X\n", Cpu.PC);
    printf("Index Register: %d\n", Cpu.I);
    printf("Stack Pointer: %d\n", Cpu.SP);
    printf("Sound timer: 0x%X\rDelay timer: 0x%X", Cpu.sound_timer, Cpu.delay_timer);

    for(int i = 0; i < 16; i++) 
        printf("Register V%d: 0x%X\n", i, Cpu.V[i]);
    for(int i = 0; i < 16; i++)
        printf("Stack%d: 0x%X\n", i,Cpu.stack[i]);
    
    for(int i = 0; i < MEMORY_SIZE; i++)
        printf(" %X", Cpu.memory[i]);
    
    printf("\n---");
    
}

/*  Load fonts    */
/*  Load ROM      */
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

static uint16_t fetch_opcode(cpu *Cpu){

    uint16_t opcode = (Cpu->memory[Cpu->PC] << 8) | Cpu->memory[Cpu->PC + 1];
    Cpu->PC += 2;

    return opcode;
}

bool execute_opcode(cpu *Cpu) {

    /* Fetch */
    uint16_t opcode = fetch_opcode(Cpu);
    uint8_t X,Y,n, kk;
    uint16_t nnn;
    X = (opcode >> 8) & 0x000F; // Spodni 4 bity vysiho bajtu
    Y = (opcode >> 4) & 0x000F; // Vyssi 4 bity nizsiho bajtu
    n = opcode & 0x000F; //spodni 4 bity nizsiho bajtu
    kk = opcode & 0x00FF; // spodni bajt
    nnn = opcode & 0x0FFF; // spodnich 12 bitu


    switch (opcode & 0xF000) {

        case 0x0000: {
            switch (kk){
                case 0x00E0:{ // CLS
                        memset(Cpu->display, 0, DISPLAY_SIZE);
                    break;
                }

                case 0x00EE:{   //RET
                        Cpu->PC = Cpu->stack[--Cpu->SP];
                    break;
                }
                default:
                    UNKNOWN_OPCDE;
            }
        break;
        }
        case 0x1000: { //JP addr
                Cpu->PC = nnn;
            break;
        }
        case 0x2000: { // Call addr
            Cpu->stack[Cpu->SP++] = Cpu->PC;
            Cpu->PC = nnn;

            break;
        }
        case 0x3000: { // Se (preskocit pokud Vx == NN)
            if(Cpu->V[X] == kk){
                Cpu->PC += 2;
            }
            break;
        }

        default:
            UNKNOWN_OPCDE;
            break;
    }

    return true;
}


/*  Load fonts    */
/*  Load ROM      */
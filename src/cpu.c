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

static uint8_t byte_random(){
    return (rand() % 256);
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
        case 0x3000: { // Se (preskocit Vx == NN)
            if(Cpu->V[X] == kk){
                Cpu->PC += 2;
            }
            break;
        }
        case 0x4000: { // Se (preskocit Vx != NN)
            if(Cpu->V[X] != kk){
                Cpu->PC += 2;
            }

            break;
        }
        case 0x5000: { // Se (preskocit Vx == Vy)
            if(Cpu->V[X] == Cpu->V[Y]){
                Cpu->PC += 2;
            }

            break;
        }
        case 0x6000: { // Nahrat do registru V s indexem X, kk
            Cpu->V[X] = kk;

            break;
        }
        case 0x7000: { // Pricist k registru V s indexem x, kk
            Cpu->V[X] += kk;

            break;
        }
        case 0x8000: { // 8XYN - Aritmeticke zalezitosti

            switch(n){
                case 0x0: {
                    Cpu->V[X] = Cpu->V[Y];
                    
                    break;
                }
                case 0x1: {
                    Cpu->V[X] |= Cpu->V[Y];

                    break;
                }
                case 0x2: {
                    Cpu->V[X] &= Cpu->V[Y];

                    break;
                }
                case 0x3: {
                    Cpu->V[X] ^= Cpu->V[Y];

                    break;
                }
                case 0x4: {
                    Cpu->V[0xF] = ((int)Cpu->V[X] + (int)Cpu->V[Y]) > 255 ? 0 : 1;
                    Cpu->V[X] += Cpu->V[Y];
                    break;
                }
                case 0x5: {
                    Cpu->V[0xF] = (Cpu->V[X] > Cpu->V[Y]) ? 1 : 0;
                    Cpu->V[X] -= Cpu->V[Y];
                    break;
                }
                case 0x6: {
                    Cpu->V[0xF] = Cpu->V[X] & 0x1;
                    Cpu->V[X] >>= 1;
                    
                    break;
                }
                case 0x7: {
                    Cpu->V[0xF] = (Cpu->V[Y] > Cpu->V[X]) ? 1 : 0;
                    Cpu->V[X] = Cpu->V[Y] - Cpu->V[X];

                    break;
                }
                case 0xE: {
                    Cpu->V[0xF] = (Cpu->V[X] >> 7) & 0x1;
                    Cpu->V[X] = (Cpu->V[X] << 1);
                    break;
                }

            default:
                UNKNOWN_OPCDE;
                break;
            }
        }
        case 0x9000: { //preskocit Vx != Vy
            if(n == 0){
                (Cpu->V[X] != Cpu->V[Y]) ? 2 : 0;
            }
        
            break;
        }
        case 0xA000: { //Annn: nastavit I na adresu nnn            
            Cpu->I = nnn;

            break;
        }
        case 0xB000: { // Skocit na adresu (V[0] + nnn)
            Cpu->PC = nnn + Cpu->V[0];
            
            break;
        }
        case 0xC000: { // Do Vx nahodny bajt AND kk
            Cpu->V[X] = byte_random() & kk;

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
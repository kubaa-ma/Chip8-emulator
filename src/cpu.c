#include "../include/cpu.h"

unsigned char chip8_fontset[80] = 
{ 
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F 
};


void init_cpu(cpu *Cpu) {
    memset(Cpu->memory, 0, MEMORY_SIZE);
    memset(Cpu->V, 0, V_REGISTERS);
    memset(Cpu->stack, 0, STACK*2);
    memset(Cpu->display, 0, DISPLAY_SIZE);
    memcpy(&Cpu->memory[0x50], chip8_fontset, sizeof(chip8_fontset));
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
    printf("---\n");
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
    
    printf("\n---\n");
    
}

void sprite_draw(cpu *Cpu, uint8_t Vx, uint8_t Vy, uint8_t height){
    Cpu->V[0xF] = 0;

    for (uint8_t by_i = 0; by_i < height; by_i++) {
        uint8_t sprite_byte = Cpu->memory[Cpu->I + by_i];

        for (uint8_t bi_i = 0; bi_i < 8; bi_i++) {
            uint8_t pixel = (sprite_byte >> (7 - bi_i)) & 0x1;

            uint16_t screen_x = (Vx + bi_i) % DISPLAY_WIDTH;
            uint16_t screen_y = (Vy + by_i) % DISPLAY_HEIGHT;
            uint16_t index = screen_y * DISPLAY_WIDTH + screen_x;

            if (pixel) {
                if (Cpu->display[index] == 1) {
                    Cpu->V[0xF] = 1;
                }
                Cpu->display[index] ^= 1;
            }
        }
    }
    Cpu->render = true;
}

static uint8_t byte_random(){
    return (rand() % 256);
}

static uint16_t fetch_opcode(cpu *Cpu){

    uint16_t opcode = (Cpu->memory[Cpu->PC] << 8) | Cpu->memory[Cpu->PC + 1];
    Cpu->PC += 2;

    return opcode;
}

void execute_opcode(cpu *Cpu) {

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
                        Cpu->render = true;
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
                    Cpu->V[0xF] = ((int)Cpu->V[X] + (int)Cpu->V[Y]) > 255 ? 1 : 0;
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
                    Cpu->V[X] = (Cpu->V[X] << 1) & 0xFF;
                    break;
                }

            default:
                UNKNOWN_OPCDE;
                break;
            }
            break;
        }
        case 0x9000: { //preskocit Vx != Vy
            if (n == 0 && Cpu->V[X] != Cpu->V[Y]) {
                Cpu->PC += 2;
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
        case 0xD000: {
            sprite_draw(Cpu,Cpu->V[X], Cpu->V[Y], n);
            Cpu->render = true;
            break;
        }
        case 0xE000: { // stisknuti tlacitka -> udalost
            switch(kk) {
                case 0x9E: { // Preskocit instrukci Vx je stisknuto
                    Cpu->PC += (Cpu->keypad[Cpu->V[X]]) ? 2 : 0;

                    break;
                }
                case 0xA1: { //Preskocit instrukci pokud neni stisknuto Vx
                    Cpu->PC += (!Cpu->keypad[Cpu->V[X]]) ? 2 : 0;

                    break;
                }
                default:
                    UNKNOWN_OPCDE;
                    break;
            }

            break;
        }
        case 0xF000:{
            switch (kk){
                case 0x07:{
                    Cpu->V[X] = Cpu->delay_timer;
                    
                    break;
                }
                case 0x0A: { // FX0A – cekani na stisknuti tlacitka
                    bool key_pressed = false;
                    for (uint8_t i = 0; i < 16; i++) {
                        if (Cpu->keypad[i]) {
                            Cpu->V[X] = i;
                            key_pressed = true;
                            break;
                        }
                    }
                    if (!key_pressed)
                        return;

                    break;
                }
                case 0x15: {
                    Cpu->delay_timer = Cpu->V[X];

                    break;
                }
                case 0x18: {
                    Cpu->sound_timer = Cpu->V[X];
                    
                    break;
                }
                case 0x1E: {
                    Cpu->V[0xF] = (Cpu->I + Cpu->V[X] > 0xFFF) ? 1 : 0;
                    Cpu->I += Cpu->V[X];

                    break;
                }
                case 0x29: {
                    Cpu->I = FONTBYTES_PER_CHAR * Cpu->V[X];


                    break;
                }
                case 0x33: {
                    Cpu->memory[Cpu->I] = (Cpu->V[X] % 1000) / 100;
                    Cpu->memory[Cpu->I + 1] = (Cpu->V[X] % 100) / 10;
                    Cpu->memory[Cpu->I + 2] = (Cpu->V[X] % 10);

                    break;
                }
                case 0x55: {
                    for(int i = 0; i <= X; i++) {
                        Cpu->memory[Cpu->I + i] = Cpu->V[i];
                    }
                    Cpu->I += X + 1;

                    break;
                }
                case 0x65: {
                    for (int i = 0; i <= X; i++) {
                        Cpu->V[i] = Cpu->memory[Cpu->I + i];
                    }
                    Cpu->I += X + 1;


                    break;
                }
                default:{
                    UNKNOWN_OPCDE;
                    break;
                }
    
            }
            break;
        }


        default:
            UNKNOWN_OPCDE;
            break;
    }

}
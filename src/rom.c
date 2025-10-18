#include "rom.h"


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

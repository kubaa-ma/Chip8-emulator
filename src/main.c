#include "cpu.h"
#include "rom.h"
#include "SDL.h"

#define PIXEL_SCALE 10

void handle_input(cpu *Cpu, SDL_Event *event, bool pressed) {
    SDL_Keycode key = event->key.key;

    switch (key) {
        case SDLK_1: Cpu->keypad[0x1] = pressed; break;
        case SDLK_2: Cpu->keypad[0x2] = pressed; break;
        case SDLK_3: Cpu->keypad[0x3] = pressed; break;
        case SDLK_4: Cpu->keypad[0xC] = pressed; break;

        case SDLK_Q: Cpu->keypad[0x4] = pressed; break;
        case SDLK_W: Cpu->keypad[0x5] = pressed; break;
        case SDLK_E: Cpu->keypad[0x6] = pressed; break;
        case SDLK_R: Cpu->keypad[0xD] = pressed; break;

        case SDLK_A: Cpu->keypad[0x7] = pressed; break;
        case SDLK_S: Cpu->keypad[0x8] = pressed; break;
        case SDLK_D: Cpu->keypad[0x9] = pressed; break;
        case SDLK_F: Cpu->keypad[0xE] = pressed; break;

        case SDLK_Z: Cpu->keypad[0xA] = pressed; break;
        case SDLK_X: Cpu->keypad[0x0] = pressed; break;
        case SDLK_C: Cpu->keypad[0xB] = pressed; break;
        case SDLK_V: Cpu->keypad[0xF] = pressed; break;

        default:
            break;
    }
}


void draw_display(SDL_Renderer *renderer, cpu *Cpu) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int y = 0; y < DISPLAY_HEIGHT; y++) {
        for(int x = 0; x < DISPLAY_WIDTH; x++) {
            uint16_t index = y * DISPLAY_WIDTH + x;
            uint8_t byte = index / 8;
            uint8_t bit = index % 8;

            if (Cpu->display[byte] & (1 << bit)) {
                SDL_FRect rect = {
                    .x = x * PIXEL_SCALE,
                    .y = y * PIXEL_SCALE,
                    .w = PIXEL_SCALE,
                    .h = PIXEL_SCALE
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
    Cpu->render = false;
    SDL_RenderPresent(renderer);
}


int main(){
    cpu Cpu;
    SDL_Window *window = SDL_CreateWindow("Display", DISPLAY_WIDTH * PIXEL_SCALE, DISPLAY_HEIGHT * PIXEL_SCALE, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    init_cpu(&Cpu);
    load_rom(Cpu.memory);

    bool quit = false;

    while (!quit) {
        SDL_Event ev;
        
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_QUIT)
                quit = true;
            else if (ev.type == SDL_EVENT_KEY_DOWN)
                handle_input(&Cpu, &ev, true);
            else if (ev.type == SDL_EVENT_KEY_UP)
                handle_input(&Cpu, &ev, false);
        }

        SDL_Delay(4);   
        execute_opcode(&Cpu);

        for(int i = 0; i < KEYS; i++)
            if(Cpu.keypad[i])
                printf("Button[%d]: %d\n", i, Cpu.keypad[i]);

        if (Cpu.render)
            draw_display(renderer, &Cpu);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#include "../include/cpu.h"
#include "..\x86_64-w64-mingw32\include\SDL3\SDL.h"

#define PIXEL_SCALE 10

void draw_display(SDL_Renderer *renderer, uint8_t *display) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int y = 0; y < DISPLAY_HEIGHT; y++) {
        for(int x = 0; x < DISPLAY_WIDTH; x++) {

            if (display[y * DISPLAY_WIDTH + x]) {
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

    SDL_RenderPresent(renderer);
}

int main(){
    cpu Cpu;
    SDL_Window *window = SDL_CreateWindow("Display", DISPLAY_WIDTH * PIXEL_SCALE, DISPLAY_HEIGHT * PIXEL_SCALE, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);


    
    init_cpu(&Cpu);
    
    load_rom(Cpu.memory);
    
    //Cpu_dump(Cpu);
    
    while(true){
        execute_opcode(&Cpu);
        draw_display(renderer, Cpu.display);
    }



    //Cpu_dump(Cpu);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
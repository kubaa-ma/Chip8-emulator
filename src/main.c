#include "../include/cpu.h"
#include "..\x86_64-w64-mingw32\include\SDL3\SDL.h"

#define PIXEL_SCALE 10

void draw_display(SDL_Renderer *renderer, cpu *Cpu) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int y = 0; y < DISPLAY_HEIGHT; y++) {
        for(int x = 0; x < DISPLAY_WIDTH; x++) {

            if (Cpu->display[y * DISPLAY_WIDTH + x]) {
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
    
    Cpu_dump(Cpu);
    bool quit = false;
    
    while (!quit) {
        SDL_Event ev;
        
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
            case SDL_EVENT_QUIT:
                quit = true;

                break;
            default:
                break;
            }
        }
        
        SDL_Delay(1);
        execute_opcode(&Cpu);
        if(Cpu.render)
            draw_display(renderer, &Cpu);
    }
    
    Cpu_dump(Cpu);



    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
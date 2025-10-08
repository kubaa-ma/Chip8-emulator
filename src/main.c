#include "../include/cpu.h"

int main(){
    cpu Cpu;

    init_cpu(&Cpu);

    load_rom(Cpu.memory);

    Cpu_dump(Cpu);

    return 0;
}
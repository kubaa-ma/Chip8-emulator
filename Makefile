CC = gcc
EXTERN = -Wall
SRC = src/cpu.c src/main.c src/rom.c
OUTPUT = cpu.exe

INCLUDE = x86_64-w64-mingw32/include/SDL3/
LIB = -Lx86_64-w64-mingw32/lib -lSDL3
INCLUDE_CHIP8 = include/

all:
	$(CC) $(SRC) $(EXTERN) $(addprefix -I,$(INCLUDE)) -I$(INCLUDE_CHIP8) $(LIB) -o $(OUTPUT)

clean:
	-rm -f $(OUTPUT)

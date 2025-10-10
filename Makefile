CC = gcc
EXTERN = -Wall
SRC = src/cpu.c src/main.c
INC = cpu.h 
OUTPUT = cpu.exe
LIB = -Ix86_64-w64-mingw32/include -Lx86_64-w64-mingw32/lib -lSDL3
all: 
	$(CC) $(SRC) -I $(INC) $(LIB) $(EXTERN) -o $(OUTPUT)

clean:
	-rm $(OUTPUT)
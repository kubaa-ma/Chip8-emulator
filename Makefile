CC = gcc
EXTERN = -Wall
SRC = src/cpu.c src/main.c
INC = cpu.h 
OUTPUT = cpu.exe

all: 
	$(CC) $(SRC) -I $(INC) $(EXTERN) -o $(OUTPUT)

clean:
	-rm $(OUTPUT)
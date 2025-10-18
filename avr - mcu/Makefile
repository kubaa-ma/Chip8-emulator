CC = avr-gcc
MCU = atmega644
F_CPU = 16000000UL
CFLAGS = -Wall -DF_CPU=$(F_CPU) -mmcu=$(MCU) -std=gnu99 -O2
OBJCOPY = avr-objcopy
PROGRAMATOR = USBASP

SRC = \
    src/main.c \
    src/drivers/usart.c src/drivers/oled.c \
    src/fat/sdcard.c src/fat/spi.c \
    ../src/cpu.c

TARGET = main.elf
HEX = main.hex
INCLUDE = include/
INCLUDE_CHIP8 = ../include/

all: $(HEX)

$(HEX): $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -I$(INCLUDE) -I$(INCLUDE_CHIP8) -o $@ $^

flash: $(HEX)
	avrdude \
		-c $(PROGRAMATOR) \
		-p $(MCU) \
		-U flash:w:$(HEX):i \
		-B 125Khz

clean:
	rm -f $(HEX) $(TARGET)

fuse:
	avrdude -c $(PROGRAMATOR) -p $(MCU) -B 125 \
		-U lfuse:w:0xFF:m \
		-U hfuse:w:0xD9:m \
		-U efuse:w:0xFF:m

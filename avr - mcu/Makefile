CC = avr-gcc
MCU = atmega328p
F_CPU = 16000000UL
CFLAGS = -Wall -DF_CPU=$(F_CPU) -mmcu=$(MCU) -std=gnu99 -O2
OBJCOPY = avr-objcopy
PROGRAMATOR = USBASP

SRC = src/main.c src/usart.c src/oled.c
TARGET = main.elf
HEX = main.hex
INCLUDE = include/

all: $(HEX)

$(HEX): $(TARGET)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -I $(INCLUDE) -o $@ $^

flash: $(HEX)
	avrdude \
		-c $(PROGRAMATOR) \
		-p $(MCU) \
		-U flash:w:$(HEX):i \
		-B 125Khz

clean:
	rm -f $(HEX) $(TARGET)

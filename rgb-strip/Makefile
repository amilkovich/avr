MCU = atmega328p
CPU = 16000000

all: rgb-strip.hex

rgb-strip.hex: rgb-strip.elf
	avr-objcopy -O ihex -R .eeprom rgb-strip.elf rgb-strip.hex

rgb-strip.elf: rgb-strip.o
	avr-gcc -g -mmcu=$(MCU) -o rgb-strip.elf rgb-strip.o

rgb-strip.o: rgb-strip.c
	avr-gcc -g -Os -mmcu=$(MCU) -DF_CPU=$(CPU) -I/root/projects/microcontroller/avr -c rgb-strip.c

flash: all
	avrdude -p $(MCU) -c usbtiny -U flash:w:rgb-strip.hex

clean:
	$(RM) rgb-strip.o rgb-strip.elf rgb-strip.hex

.PHONY: all flash clean

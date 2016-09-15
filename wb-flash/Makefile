MCU = atmega328p
CPU = 16000000

all: wb-flash.hex

wb-flash.hex: wb-flash.elf
	avr-objcopy -O ihex -R .eeprom wb-flash.elf wb-flash.hex

wb-flash.elf: wb-flash.o
	avr-gcc -g -mmcu=$(MCU) -o wb-flash.elf wb-flash.o

wb-flash.o: wb-flash.c
	avr-gcc -g -Os -mmcu=$(MCU) -DF_CPU=$(CPU) -I/root/projects/microcontroller/avr -c wb-flash.c

flash: all
	avrdude -p $(MCU) -P /dev/ttyUSB0 -b 57600 -c arduino -U flash:w:wb-flash.hex

clean:
	$(RM) wb-flash.o wb-flash.elf wb-flash.hex

.PHONY: all flash clean

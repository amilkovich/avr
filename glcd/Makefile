MCU = atmega328p
CPU = 16000000

PROGRAMMER = arduino

all: glcd.hex

glcd.hex: glcd.elf
	avr-objcopy -O ihex -R .eeprom glcd.elf glcd.hex

glcd.elf: glcd.o
	avr-gcc -g -mmcu=$(MCU) -o glcd.elf glcd.o

glcd.o: glcd.c
	avr-gcc -g -Os -mmcu=$(MCU) -DF_CPU=$(CPU) -I/root/projects/microcontroller/avr -c glcd.c

flash: all
	avrdude -p $(MCU) -P /dev/ttyUSB0 -c $(PROGRAMMER) -b 57600 -U flash:w:glcd.hex

clean:
	$(RM) glcd.o glcd.elf glcd.hex

.PHONY: all flash clean

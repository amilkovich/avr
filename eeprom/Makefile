MCU = atmega328p
CPU = 8000000

all: eeprom.hex

eeprom.hex: eeprom.elf
	avr-objcopy -O ihex -R .eeprom eeprom.elf eeprom.hex

eeprom.elf: eeprom.o
	avr-gcc -g -mmcu=$(MCU) -o eeprom.elf eeprom.o

eeprom.o: eeprom.c
	avr-gcc -g -Os -mmcu=$(MCU) -DF_CPU=$(CPU) -I/root/projects/microcontroller/avr -c eeprom.c

flash: all
	avrdude -p $(MCU) -c usbtiny -U flash:w:eeprom.hex

clean:
	$(RM) eeprom.o eeprom.elf eeprom.hex

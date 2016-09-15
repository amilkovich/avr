MCU = atmega328p
CPU = 16000000

PROGRAMMER = arduino

all: bcm.hex

bcm.hex: bcm.elf
	avr-objcopy -O ihex -R .eeprom bcm.elf bcm.hex

bcm.elf: bcm.o
	avr-gcc -g -mmcu=$(MCU) -o bcm.elf bcm.o

bcm.o: bcm.c
	avr-gcc -g -Os -mmcu=$(MCU) -DF_CPU=$(CPU) -I/root/projects/microcontroller/avr -c bcm.c

flash: all
	avrdude -p $(MCU) -P /dev/ttyUSB0 -c $(PROGRAMMER) -b 57600 -U flash:w:bcm.hex

clean:
	$(RM) bcm.o bcm.elf bcm.hex

.PHONY: all flash clean

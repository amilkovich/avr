#MCU = attiny85
#CPU = 1000000

#MCU = at90usb1286
#CPU = 16000000

#MCU = atmega2560
#CPU = 16000000

MCU = atmega328p
CPU = 8000000

#MCU = atmega8515
#CPU = 16000000

#MCU = atmega32u4
#CPU = 16000000

#PROGRAMMER = avrispmkII
#PROGRAMMER = arduino
#PROGRAMMER = avr109

all: blink.hex

blink.hex: blink.elf
	avr-objcopy -O ihex -R .eeprom blink.elf blink.hex

blink.elf: blink.o
	avr-gcc -g -mmcu=$(MCU) -o blink.elf blink.o

blink.o: blink.c
	avr-gcc -g -Os -mmcu=$(MCU) -DF_CPU=$(CPU) -I/root/projects/microcontroller/avr -c blink.c

flash: all
	#avrdude -p $(MCU) -P /dev/ttyUSB0 -c $(PROGRAMMER) -b 57600 -U flash:w:blink.hex
	avrdude -p $(MCU) -c usbtiny -U flash:w:blink.hex

clean:
	$(RM) blink.o blink.elf blink.hex

.PHONY: all flash clean

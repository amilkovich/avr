MCU = atmega328p
CPU = 16000000

PROGRAMMER = arduino

all: lcd.hex

lcd.hex: lcd.elf
	avr-objcopy -O ihex -R .eeprom lcd.elf lcd.hex

lcd.elf: lcd.o
	avr-gcc -g -mmcu=$(MCU) -o lcd.elf lcd.o

lcd.o: lcd.c
	avr-gcc -g -Os -mmcu=$(MCU) -DF_CPU=$(CPU) -I/root/projects/microcontroller/avr -c lcd.c

flash: all
	avrdude -p $(MCU) -P /dev/ttyUSB0 -c $(PROGRAMMER) -b 57600 -U flash:w:lcd.hex

clean:
	$(RM) lcd.o lcd.elf lcd.hex

.PHONY: all flash clean

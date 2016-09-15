MCU = attiny85
CPU = 16500000

all: pwm.hex

pwm.hex: pwm.elf
	avr-objcopy -O ihex -R .eeprom pwm.elf pwm.hex

pwm.elf: pwm.o
	avr-gcc -g -mmcu=$(MCU) -o pwm.elf pwm.o

pwm.o: pwm.c
	avr-gcc -g -Os -mmcu=$(MCU) -DF_CPU=$(CPU) -I/root/projects/microcontroller/avr -c pwm.c

flash: all
	avrdude -p $(MCU) -c usbtiny -U flash:w:pwm.hex

clean:
	$(RM) pwm.o pwm.elf pwm.hex

.PHONY: all flash clean

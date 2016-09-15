#ifndef HD44780_H
#define HD44780_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>

#include "portability.h"

#define HD44780_CLEAR_DISPLAY            0x01
#define HD44780_RETURN_HOME              0x02
#define HD44780_ENTRY_MODE_SET           0x04
#define HD44780_ENTRY_MODE_SET_INCREMENT 0x02
#define HD44780_DISPLAY_CONTROL          0x08
#define HD44780_DISPLAY_ON               0x04
#define HD44780_DISPLAY_OFF              0x00
#define HD44780_CURSOR                   0x02
#define HD44780_BLINK                    0x01
#define HD44780_FUNCTION_SET             0x32
#define HD44780_FUNCTION_SET_8BIT        0x16
#define HD44780_FUNCTION_SET_2LINE       0x08
#define HD44780_SHIFT                    0x16
#define HD44780_SHIFT_CURSOR             0x00
#define HD44780_SHIFT_RIGHT              0x04
#define HD44780_SHIFT_LEFT               0xfb
#define HD44780_SET_CGRAM_ADDRESS        0x40

void hd44780_command(uint8_t cmd) {
	(cmd & (1 << 0)) == 0 ? (PORT(D0_PORT) &= ~(1 << D0_PIN)) :
				(PORT(D0_PORT) |= (1 << D0_PIN));
	(cmd & (1 << 1)) == 0 ? (PORT(D1_PORT) &= ~(1 << D1_PIN)) :
				(PORT(D1_PORT) |= (1 << D1_PIN));
	(cmd & (1 << 2)) == 0 ? (PORT(D2_PORT) &= ~(1 << D2_PIN)) :
				(PORT(D2_PORT) |= (1 << D2_PIN));
	(cmd & (1 << 3)) == 0 ? (PORT(D3_PORT) &= ~(1 << D3_PIN)) :
				(PORT(D3_PORT) |= (1 << D3_PIN));
	(cmd & (1 << 4)) == 0 ? (PORT(D4_PORT) &= ~(1 << D4_PIN)) :
				(PORT(D4_PORT) |= (1 << D4_PIN));
	(cmd & (1 << 5)) == 0 ? (PORT(D5_PORT) &= ~(1 << D5_PIN)) :
				(PORT(D5_PORT) |= (1 << D5_PIN));
	(cmd & (1 << 6)) == 0 ? (PORT(D6_PORT) &= ~(1 << D6_PIN)) :
				(PORT(D6_PORT) |= (1 << D6_PIN));
	(cmd & (1 << 7)) == 0 ? (PORT(D7_PORT) &= ~(1 << D7_PIN)) :
				(PORT(D7_PORT) |= (1 << D7_PIN));

	PORT(E_PORT) |=  (1 << E_PIN);
	_delay_ms(1);
	PORT(E_PORT) &= ~(1 << E_PIN);
}

void hd44780_data(uint8_t data) {
	(data & (1 << 0)) == 0 ? (PORT(D0_PORT) &= ~(1 << D0_PIN)) :
				 (PORT(D0_PORT) |= (1 << D0_PIN));
	(data & (1 << 1)) == 0 ? (PORT(D1_PORT) &= ~(1 << D1_PIN)) :
				 (PORT(D1_PORT) |= (1 << D1_PIN));
	(data & (1 << 2)) == 0 ? (PORT(D2_PORT) &= ~(1 << D2_PIN)) :
				 (PORT(D2_PORT) |= (1 << D2_PIN));
	(data & (1 << 3)) == 0 ? (PORT(D3_PORT) &= ~(1 << D3_PIN)) :
				 (PORT(D3_PORT) |= (1 << D3_PIN));
	(data & (1 << 4)) == 0 ? (PORT(D4_PORT) &= ~(1 << D4_PIN)) :
				 (PORT(D4_PORT) |= (1 << D4_PIN));
	(data & (1 << 5)) == 0 ? (PORT(D5_PORT) &= ~(1 << D5_PIN)) :
				 (PORT(D5_PORT) |= (1 << D5_PIN));
	(data & (1 << 6)) == 0 ? (PORT(D6_PORT) &= ~(1 << D6_PIN)) :
				 (PORT(D6_PORT) |= (1 << D6_PIN));
	(data & (1 << 7)) == 0 ? (PORT(D7_PORT) &= ~(1 << D7_PIN)) :
				 (PORT(D7_PORT) |= (1 << D7_PIN));

	PORT(RS_PORT) |=  (1 << RS_PIN);
	PORT(E_PORT)  |=  (1 << E_PIN);
	_delay_ms(1);
	PORT(E_PORT)  &= ~(1 << E_PIN);
	PORT(RS_PORT) &= ~(1 << RS_PIN);
}

void hd44780_initialize(void) {
	DDR(RS_PORT) |= (1 << RS_PIN);
	DDR(RW_PORT) |= (1 << RW_PIN);
	DDR(E_PORT)  |= (1 << E_PIN);
	DDR(BL_PORT) |= (1 << BL_PIN);
	DDR(D0_PORT) |= (1 << D0_PIN);
	DDR(D1_PORT) |= (1 << D1_PIN);
	DDR(D2_PORT) |= (1 << D2_PIN);
	DDR(D3_PORT) |= (1 << D3_PIN);
	DDR(D4_PORT) |= (1 << D4_PIN);
	DDR(D5_PORT) |= (1 << D5_PIN);
	DDR(D6_PORT) |= (1 << D6_PIN);
	DDR(D7_PORT) |= (1 << D7_PIN);

	_delay_ms(100);

	hd44780_command(HD44780_FUNCTION_SET | HD44780_FUNCTION_SET_8BIT);
	_delay_ms(4);

	hd44780_command(HD44780_FUNCTION_SET | HD44780_FUNCTION_SET_8BIT);
	_delay_us(100);

	hd44780_command(HD44780_FUNCTION_SET | HD44780_FUNCTION_SET_8BIT |
			HD44780_FUNCTION_SET_2LINE);
	_delay_us(100);

	hd44780_command(HD44780_DISPLAY_CONTROL | HD44780_DISPLAY_OFF);
	_delay_ms(53);

	hd44780_command(HD44780_CLEAR_DISPLAY);
	_delay_ms(3);

	hd44780_command(HD44780_ENTRY_MODE_SET |
			HD44780_ENTRY_MODE_SET_INCREMENT);
	_delay_us(53);

	hd44780_command(HD44780_DISPLAY_CONTROL | HD44780_DISPLAY_ON);
	_delay_us(53);

	hd44780_command(HD44780_RETURN_HOME);
}

void hd44780_backlight(bool on) {
	if (on) PORT(BL_PORT) |= (1 << BL_PIN);
	else PORT(BL_PORT) &= ~(1 << BL_PIN);
}

void hd44780_putc(uint8_t c) {
	PORT(RS_PORT) |= (1 << RS_PIN);
	hd44780_command(c);
	PORT(RS_PORT) &= ~(1 << RS_PIN);
}

void hd44780_puts(char *s) {
	for (uint8_t i = 0; i < strlen(s); i++)
		hd44780_putc(s[i]);
}

void hd44780_custom_character(uint8_t location, uint8_t data[8]) {
	hd44780_command(HD44780_SET_CGRAM_ADDRESS + location * 0x08);
	hd44780_data(data[0]);
	hd44780_data(data[1]);
	hd44780_data(data[2]);
	hd44780_data(data[3]);
	hd44780_data(data[4]);
	hd44780_data(data[5]);
	hd44780_data(data[6]);
	hd44780_data(data[7]);
	hd44780_command(HD44780_RETURN_HOME);
}

#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define RS_PORT D
#define RS_PIN  5
#define RW_PORT D
#define RW_PIN  6
#define E_PORT  D
#define E_PIN   7
#define D0_PORT C
#define D0_PIN  0
#define D1_PORT C
#define D1_PIN  1
#define D2_PORT C
#define D2_PIN  2
#define D3_PORT C
#define D3_PIN  3
#define D4_PORT C
#define D4_PIN  4
#define D5_PORT C
#define D5_PIN  5
#define D6_PORT B
#define D6_PIN  0
#define D7_PORT B
#define D7_PIN  1
#define BL_PORT D
#define BL_PIN  3

#include "hd44780.h"

#include "usart.h"

uint8_t lcd_text[4 * 20];
uint8_t lcd_text_pos = 0;
bool lcd_update_text = false;

int main(void) {
	usart_initialize(57600);

	hd44780_initialize();
	hd44780_backlight(true);

	hd44780_puts("      LCD v0.5      ");
	hd44780_puts("                    ");
	hd44780_puts("                    ");
	hd44780_puts("     by  Andrew     ");
	_delay_ms(5000);

	uint8_t custom[8];

	custom[0] = 0b00000;
	custom[1] = 0b00000;
	custom[2] = 0b00000;
	custom[3] = 0b00000;
	custom[4] = 0b00000;
	custom[5] = 0b00000;
	custom[6] = 0b00000;
	custom[7] = 0b11111;
	hd44780_custom_character(0x00, custom);

	custom[0] = 0b00000;
	custom[1] = 0b00000;
	custom[2] = 0b00000;
	custom[3] = 0b00000;
	custom[4] = 0b00000;
	custom[5] = 0b00000;
	custom[6] = 0b11111;
	custom[7] = 0b11111;
	hd44780_custom_character(0x01, custom);

	custom[0] = 0b00000;
	custom[1] = 0b00000;
	custom[2] = 0b00000;
	custom[3] = 0b00000;
	custom[4] = 0b00000;
	custom[5] = 0b11111;
	custom[6] = 0b11111;
	custom[7] = 0b11111;
	hd44780_custom_character(0x02, custom);

	custom[0] = 0b00000;
	custom[1] = 0b00000;
	custom[2] = 0b00000;
	custom[3] = 0b00000;
	custom[4] = 0b11111;
	custom[5] = 0b11111;
	custom[6] = 0b11111;
	custom[7] = 0b11111;
	hd44780_custom_character(0x03, custom);

	custom[0] = 0b00000;
	custom[1] = 0b00000;
	custom[2] = 0b00000;
	custom[3] = 0b11111;
	custom[4] = 0b11111;
	custom[5] = 0b11111;
	custom[6] = 0b11111;
	custom[7] = 0b11111;
	hd44780_custom_character(0x04, custom);

	custom[0] = 0b00000;
	custom[1] = 0b00000;
	custom[2] = 0b11111;
	custom[3] = 0b11111;
	custom[4] = 0b11111;
	custom[5] = 0b11111;
	custom[6] = 0b11111;
	custom[7] = 0b11111;
	hd44780_custom_character(0x05, custom);

	custom[0] = 0b00000;
	custom[1] = 0b11111;
	custom[2] = 0b11111;
	custom[3] = 0b11111;
	custom[4] = 0b11111;
	custom[5] = 0b11111;
	custom[6] = 0b11111;
	custom[7] = 0b11111;
	hd44780_custom_character(0x06, custom);

	custom[0] = 0b11111;
	custom[1] = 0b11111;
	custom[2] = 0b11111;
	custom[3] = 0b11111;
	custom[4] = 0b11111;
	custom[5] = 0b11111;
	custom[6] = 0b11111;
	custom[7] = 0b11111;
	hd44780_custom_character(0x07, custom);

	UCSR0B |= (1 << RXCIE0);
	sei();

	while (1) {
		if (lcd_text[0] != '\0') {
			if (lcd_update_text) {
				hd44780_command(HD44780_CLEAR_DISPLAY);
				hd44780_command(HD44780_RETURN_HOME);
				for (uint8_t i = 0; i < lcd_text_pos; i++)
					hd44780_putc(lcd_text[i]);
				lcd_update_text = false;
			}
			continue;
		}

		static float scroll = 0.0f;
		static const float scale = 0.25f;
		for (uint8_t i = 0; i < 20; i++) {
			float sine = 0.5f + sin(scroll + i * scale) / 2.0f;
			if (sine > 0.75f)
				hd44780_putc((sine - 0.75f) / 0.25f * 7.0f);
			else
				hd44780_putc(' ');
		}
		for (uint8_t i = 0; i < 20; i++) {
			float sine = 0.5f + sin(scroll + i * scale) / 2.0f;
			if (sine > 0.25f) {
				float value = (sine - 0.25f) / 0.25f * 7.0f;
				if (value > 7.0f) value = 7.0f;
				hd44780_putc(value);
			} else
				hd44780_putc(' ');
		}
		for (uint8_t i = 0; i < 20; i++) {
			float sine = 0.5f + sin(scroll + i * scale) / 2.0f;
			if (sine > 0.5f) {
				float value = (sine - 0.5f) / 0.25f * 7.0f;
				if (value > 7.0f) value = 7.0f;
				hd44780_putc(value);
			} else
				hd44780_putc(' ');
		}
		for (uint8_t i = 0; i < 20; i++) {
			float sine = 0.5f + sin(scroll + i * scale) / 2.0f;
			if (sine > 0.0f) {
				float value = (sine - 0.0f) / 0.25f * 7.0f;
				if (value > 7.0f) value = 7.0f;
				hd44780_putc(value);
			} else
				hd44780_putc(' ');
		}

		scroll += 0.15f;
	}
}

ISR(USART_RX_vect) {
	uint8_t received_byte = UDR0;
	if (received_byte == '\0') {
		if (lcd_text_pos == 0) return;
		hd44780_command(HD44780_CLEAR_DISPLAY);
		hd44780_command(HD44780_RETURN_HOME);
		lcd_text[0] = '\0';
		lcd_text_pos = 0;
		return;
	}
	lcd_text_pos %= 4 * 20;
	lcd_text[lcd_text_pos++] = received_byte;
	lcd_update_text = true;
}

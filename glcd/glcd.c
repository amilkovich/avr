// scribble
// http://www.batsocks.co.uk/downloads/tms_scribble_002.ino

#include <avr/io.h>
#include <util/delay.h>

#define D0_PORT  B
#define D0_PIN   0
#define D1_PORT  B
#define D1_PIN   1
#define D2_PORT  B
#define D2_PIN   2
#define D3_PORT  B
#define D3_PIN   3
#define D4_PORT  D
#define D4_PIN   4
#define D5_PORT  D
#define D5_PIN   5
#define D6_PORT  D
#define D6_PIN   6
#define D7_PORT  D
#define D7_PIN   7
#define CS1_PORT C
#define CS1_PIN  0
#define CS2_PORT C
#define CS2_PIN  1
#define RW_PORT  C
#define RW_PIN   2
#define DI_PORT  C
#define DI_PIN   3
#define E_PORT   C
#define E_PIN    4

#include "adm12864h.h"

#define LED_PORT B
#define LED_PIN  5

static uint8_t text[] = {
	0b01110000, // 'G'
	0b10001000,
	0b10000000,
	0b10000000,
	0b10111000,
	0b10001000,
	0b10001000,
	0b01110000,

	0b10000000, // 'L'
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b11111000,

	0b01110000, // 'C'
	0b10001000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10001000,
	0b01110000,

	0b11100000, // 'D'
	0b10010000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10010000,
	0b11100000,

	0b00000000, // ' '
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,

	0b00000000, // 'v'
	0b00000000,
	0b10001000,
	0b10001000,
	0b01010000,
	0b01010000,
	0b00100000,
	0b00100000,

	0b00100000, // '1'
	0b01100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b01110000,
};

uint8_t function_x(float tick) {
	static const uint8_t width = 128;
	return (uint8_t)(width / 2 + (width / 2 - 1) *
			 sin(tick * 1.8) * cos(tick * 3.2));
}

uint8_t function_y(float tick) {
	static const uint8_t height = 64;
	return (uint8_t)(height / 2 + (height / 2 - 1) *
			 cos(tick * 1.2) * sin(tick * 3.1));
}

int main(void) {
	DDR(LED_PORT) |= (1 << LED_PIN);

	adm12864h_initialize();
	adm12864h_clear();

	while (1) {
		static const float tick = 1.0f / 128.0f;
		static float head_pos = 0.0f;
		uint8_t x, y;

		head_pos += tick;

		// set the pixels at the 'head' of the line
		float head = head_pos;
		x = function_x(head);
		y = function_y(head);
		adm12864h_set_pixel(x + 0, y + 0, true);
		adm12864h_set_pixel(x - 1, y + 0, true);
		adm12864h_set_pixel(x + 1, y + 0, true);
		adm12864h_set_pixel(x + 0, y - 1, true);
		adm12864h_set_pixel(x + 0, y + 1, true);

		// clear the pixels at the 'tail' of the line
		float tail = head - (256 * tick);
		x = function_x(tail);
		y = function_y(tail);
		adm12864h_set_pixel(x + 0, y + 0, false);
		adm12864h_set_pixel(x - 1, y + 0, false);
		adm12864h_set_pixel(x + 1, y + 0, false);
		adm12864h_set_pixel(x + 0, y - 1, false);
		adm12864h_set_pixel(x + 0, y + 1, false);

		// write some sample text
		for (uint8_t j = 0; j < 7; j++) {
			adm12864h_write_command(ADM12864H_SET_X | 8 * 7, ADM12864H_IC2);
			adm12864h_write_command(ADM12864H_SET_Y | 7 - j, ADM12864H_IC2);
			for (uint8_t i = 0; i < 8; i++) {
				adm12864h_write_data(text[i + (j * 8)], ADM12864H_IC2);
			}
		}
	}
}

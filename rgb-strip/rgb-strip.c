#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define GSCLK_PORT B
#define GSCLK_PIN  0
#define SIN_PORT   B
#define SIN_PIN    3
#define SCLK_PORT  B
#define SCLK_PIN   5
#define BLANK_PORT B
#define BLANK_PIN  2
#define DCPRG_PORT D
#define DCPRG_PIN  4
#define VPRG_PORT  D
#define VPRG_PIN   7
#define XLAT_PORT  B
#define XLAT_PIN   1

#define TLC5940_N 3

#include "tlc5940.h"

void rgb_strip_set(uint8_t led, uint16_t red, uint16_t green, uint16_t blue) {
	if (led == 15) {
		tlc5940_set(15, red);
		tlc5940_set(31, green);
		tlc5940_set(47, blue);
	} else if (led >= 10) {
		tlc5940_set(led * 3 + 2, red);
		tlc5940_set(led * 3 + 3, green);
		tlc5940_set(led * 3 + 4, blue);
	} else if (led >= 5) {
		tlc5940_set(led * 3 + 1, red);
		tlc5940_set(led * 3 + 2, green);
		tlc5940_set(led * 3 + 3, blue);
	} else {
		tlc5940_set(led * 3 + 0, red);
		tlc5940_set(led * 3 + 1, green);
		tlc5940_set(led * 3 + 2, blue);
	}
}

int main(void) {
	tlc5940_initialize();
	tlc5940_set_all_dc(0x0f);
	tlc5940_update_dc();
	tlc5940_set_all(0x0000);

	sei();

	while (1) {
		static const uint8_t delay = 25;

		for (int8_t i = 0; i < 16; i++) {
			rgb_strip_set(i, 4095, 0, 0);
			_delay_ms(delay);
			rgb_strip_set(i, 0, 0, 0);
		}

		for (int8_t i = 15; i >= 0; i--) {
			rgb_strip_set(i, 0, 4095, 0);
			_delay_ms(delay);
			rgb_strip_set(i, 0, 0, 0);
		}

		for (int8_t i = 0; i < 16; i++) {
			rgb_strip_set(i, 0, 0, 4095);
			_delay_ms(delay);
			rgb_strip_set(i, 0, 0, 0);
		}

		for (int8_t i = 15; i >= 0; i--) {
			rgb_strip_set(i, 4095, 4095, 4095);
			_delay_ms(delay);
			rgb_strip_set(i, 0, 0, 0);
		}

		for (int8_t i = 0; i < 16; i++) {
			rgb_strip_set(i, 4095, 0, 0);
			_delay_ms(delay);
		}

		for (int8_t i = 0; i < 16; i++) {
			rgb_strip_set(i, 0, 0, 0);
			_delay_ms(delay);
		}

		for (int8_t i = 15; i >= 0; i--) {
			rgb_strip_set(i, 0, 4095, 0);
			_delay_ms(delay);
		}

		for (int8_t i = 15; i >= 0; i--) {
			rgb_strip_set(i, 0, 0, 0);
			_delay_ms(delay);
		}

		for (int8_t i = 0; i < 16; i++) {
			rgb_strip_set(i, 0, 0, 4095);
			_delay_ms(delay);
		}

		for (int8_t i = 0; i < 16; i++) {
			rgb_strip_set(i, 0, 0, 0);
			_delay_ms(delay);
		}

		for (int8_t i = 15; i >= 0; i--) {
			rgb_strip_set(i, 4095, 4095, 4095);
			_delay_ms(delay);
		}

		for (int8_t i = 15; i >= 0; i--) {
			rgb_strip_set(i, 0, 0, 0);
			_delay_ms(delay);
		}
	}
}

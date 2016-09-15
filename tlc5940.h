// implemented and inspired by 'Demystifying the TLC5940' by Matthew T. Pandina

#ifndef TLC5940_H
#define TLC5940_H

#include <avr/io.h>

#include "portability.h"

#define set_output(ddr, pin) ((ddr) |= (1 << (pin)))
#define set_low(port, pin) ((port) &= ~(1 << (pin)))
#define set_high(port, pin) ((port) |= (1 << (pin)))
#define pulse(port, pin)\
	{\
		set_high((port), (pin));\
		set_low((port), (pin));\
	}
#define output_state(port, pin) ((port) & (1 << (pin)))

uint8_t dc_data[TLC5940_N * 12];
uint8_t gs_data[TLC5940_N * 24];

void tlc5940_initialize(void) {
	set_output(DDR(GSCLK_PORT), GSCLK_PIN);
	set_output(DDR(SIN_PORT), SIN_PIN);
	set_output(DDR(SCLK_PORT), SCLK_PIN);
	set_output(DDR(BLANK_PORT), BLANK_PIN);
	set_output(DDR(DCPRG_PORT), DCPRG_PIN);
	set_output(DDR(VPRG_PORT), VPRG_PIN);
	set_output(DDR(XLAT_PORT), XLAT_PIN);

	set_low(PORT(GSCLK_PORT), GSCLK_PIN);
	set_high(DDR(SIN_PORT), SIN_PIN);
	set_low(PORT(SCLK_PORT), SCLK_PIN);
	set_high(PORT(BLANK_PORT), BLANK_PIN);
	set_low(PORT(DCPRG_PORT), DCPRG_PIN);
	set_high(PORT(VPRG_PORT), VPRG_PIN);
	set_low(PORT(XLAT_PORT), XLAT_PIN);

	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | (1 << CS00);
	OCR0A = 3;
	TIMSK0 |= (1 << OCIE0A);

	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR = (1 << SPI2X);
}

void tlc5940_update_dc(void) {
	set_high(PORT(DCPRG_PORT), DCPRG_PIN);
	set_high(PORT(VPRG_PORT), VPRG_PIN);

	for (uint8_t i = 0; i < TLC5940_N * 12; i++) {
		SPDR = dc_data[i];
		while (!(SPSR & (1 << SPIF)));
	}

	pulse(PORT(XLAT_PORT), XLAT_PIN);
}

void tlc5940_set_dc(uint8_t channel, uint8_t value) {
	channel = (TLC5940_N * 16) - 1 - channel;
	uint16_t i = (uint16_t)channel * 3 / 4;

	switch (channel % 4) {
		case 0:
			dc_data[i] = (dc_data[i] & 0x03) | (uint8_t)(value << 2);
		break;

		case 1:
			dc_data[i] = (dc_data[i] & 0xfc) | (value >> 4);
			i++;
			dc_data[i] = (dc_data[i] & 0x0f) | (uint8_t)(value << 4);
		break;

		case 2:
			dc_data[i] = (dc_data[i] & 0xf0) | (value >> 2);
			i++;
			dc_data[i] = (dc_data[i] & 0x3f) | (uint8_t)(value << 6);
		break;

		default:
			dc_data[i] = (dc_data[i] & 0xc0) | (value);
		break;
	}
}

void tlc5940_set_all_dc(uint8_t value) {
	uint8_t tmp1 = (uint8_t)(value << 2);
	uint8_t tmp2 = (uint8_t)(tmp1  << 2);
	uint8_t tmp3 = (uint8_t)(tmp2  << 2);
	tmp1 |= (value >> 4);
	tmp2 |= (value >> 2);
	tmp3 |= value;

	uint8_t i = 0;
	do {
		dc_data[i++] = tmp1;
		dc_data[i++] = tmp2;
		dc_data[i++] = tmp3;
	} while (i < TLC5940_N * 12);
}

void tlc5940_set(uint8_t channel, uint16_t value) {
	channel = (TLC5940_N * 16) - 1 - channel;

	uint16_t i = (uint16_t)channel * 3 / 2;

	switch (channel % 2) {
		case 0:
			gs_data[i] = (value >> 4);
			i++;
			gs_data[i] = (gs_data[i] & 0x0f) | (uint8_t)(value << 4);
		break;

		default:
			gs_data[i] = (gs_data[i] & 0xf0) | (value >> 8);
			i++;
			gs_data[i] = (uint8_t)value;
		break;
	}
}

void tlc5940_set_all(uint16_t value) {
	uint8_t tmp1 = (value >> 4);
	uint8_t tmp2 = (uint8_t)(value << 4) | (tmp1 >> 4);
	uint8_t i = 0;
	do {
		gs_data[i++] = tmp1;
		gs_data[i++] = tmp2;
		gs_data[i++] = (uint8_t)value;
	} while (i < TLC5940_N * 24);
}

ISR(TIMER0_COMPA_vect) {
	static uint8_t xlat_needs_pulse = 0;

	set_high(PORT(BLANK_PORT), BLANK_PIN);

	if (output_state(PORT(VPRG_PORT), VPRG_PIN)) {
		set_low(PORT(VPRG_PORT), VPRG_PIN);

		if (xlat_needs_pulse) {
			pulse(PORT(XLAT_PORT), XLAT_PIN);
			xlat_needs_pulse = 0;
		}

		pulse(PORT(SCLK_PORT), SCLK_PIN);
	} else if (xlat_needs_pulse) {
		pulse(PORT(XLAT_PORT), XLAT_PIN);
		xlat_needs_pulse = 0;
	}

	set_low(PORT(BLANK_PORT), BLANK_PIN);

	for (uint8_t i = 0; i < TLC5940_N * 24; i++) {
		SPDR = gs_data[i];
		while (!(SPSR & (1 << SPIF)));
	}

	xlat_needs_pulse = 1;
}

#endif

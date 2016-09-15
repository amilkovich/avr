#ifndef ADM12864H_H
#define ADM12864H_H

#include <avr/io.h>
#include <stdbool.h>

#include "portability.h"

#define ADM12864H_ON_CMD      0x3f
#define ADM12864H_START_LINE  0xc0
#define ADM12864H_STATUS_BUSY 0x80

// swapped x and y for pcb landscape screen layout
#define ADM12864H_SET_X 0x40 // | (0 - 63)
#define ADM12864H_SET_Y 0xb8 // | (0 - 7)

#define ADM12864H_IC1 0x00
#define ADM12864H_IC2 0x01

#define ADM12864H_TC   1.000
#define ADM12864H_TWH  0.450
#define ADM12864H_TWL  0.450
#define ADM12864H_TR   0.025
#define ADM12864H_TF   0.025
#define ADM12864H_TASU 0.140
#define ADM12864H_TAH  0.010
#define ADM12864H_TSU  0.200
#define ADM12864H_TD   0.320
#define ADM12864H_TDHW 0.010
#define ADM12864H_TDHR 0.020

uint8_t adm12864h_read_status(uint8_t ic) {
	DDR(D0_PORT) &= ~(1 << D0_PIN);
	DDR(D1_PORT) &= ~(1 << D1_PIN);
	DDR(D2_PORT) &= ~(1 << D2_PIN);
	DDR(D3_PORT) &= ~(1 << D3_PIN);
	DDR(D4_PORT) &= ~(1 << D4_PIN);
	DDR(D5_PORT) &= ~(1 << D5_PIN);
	DDR(D6_PORT) &= ~(1 << D6_PIN);
	DDR(D7_PORT) &= ~(1 << D7_PIN);

	PORT(RW_PORT) |= (1 << RW_PIN);
	if (ic == ADM12864H_IC1) PORT(CS1_PORT) |= (1 << CS1_PIN);
	else 			 PORT(CS2_PORT) |= (1 << CS2_PIN);
	_delay_us(ADM12864H_TASU);
	PORT(E_PORT)  |= (1 << E_PIN);
	_delay_us(ADM12864H_TD);

	uint8_t status = ((PIN(D0_PORT) & (1 << D0_PIN) ? 1 : 0) << 0) |
			 ((PIN(D1_PORT) & (1 << D1_PIN) ? 1 : 0) << 1) |
			 ((PIN(D2_PORT) & (1 << D2_PIN) ? 1 : 0) << 2) |
			 ((PIN(D3_PORT) & (1 << D3_PIN) ? 1 : 0) << 3) |
			 ((PIN(D4_PORT) & (1 << D4_PIN) ? 1 : 0) << 4) |
			 ((PIN(D5_PORT) & (1 << D5_PIN) ? 1 : 0) << 5) |
			 ((PIN(D6_PORT) & (1 << D6_PIN) ? 1 : 0) << 6) |
			 ((PIN(D7_PORT) & (1 << D7_PIN) ? 1 : 0) << 7);

	PORT(E_PORT)  &= ~(1 << E_PIN);
	if (ic == ADM12864H_IC1) PORT(CS1_PORT) &= ~(1 << CS1_PIN);
	else 			 PORT(CS2_PORT) &= ~(1 << CS2_PIN);
	PORT(RW_PORT) &= ~(1 << RW_PIN);

	return status;
}

uint8_t adm12864h_read_data(uint8_t ic) {
	while (adm12864h_read_status(ic) & ADM12864H_STATUS_BUSY);

	DDR(D0_PORT) &= ~(1 << D0_PIN);
	DDR(D1_PORT) &= ~(1 << D1_PIN);
	DDR(D2_PORT) &= ~(1 << D2_PIN);
	DDR(D3_PORT) &= ~(1 << D3_PIN);
	DDR(D4_PORT) &= ~(1 << D4_PIN);
	DDR(D5_PORT) &= ~(1 << D5_PIN);
	DDR(D6_PORT) &= ~(1 << D6_PIN);
	DDR(D7_PORT) &= ~(1 << D7_PIN);

	PORT(RW_PORT) |= (1 << RW_PIN);
	PORT(DI_PORT) |= (1 << DI_PIN);
	if (ic == ADM12864H_IC1) PORT(CS1_PORT) |= (1 << CS1_PIN);
	else 			 PORT(CS2_PORT) |= (1 << CS2_PIN);
	_delay_us(ADM12864H_TASU);
	PORT(E_PORT)  |= (1 << E_PIN);
	_delay_us(ADM12864H_TD);

	uint8_t data = ((PIN(D0_PORT) & (1 << D0_PIN) ? 1 : 0) << 0) |
		       ((PIN(D1_PORT) & (1 << D1_PIN) ? 1 : 0) << 1) |
		       ((PIN(D2_PORT) & (1 << D2_PIN) ? 1 : 0) << 2) |
		       ((PIN(D3_PORT) & (1 << D3_PIN) ? 1 : 0) << 3) |
		       ((PIN(D4_PORT) & (1 << D4_PIN) ? 1 : 0) << 4) |
		       ((PIN(D5_PORT) & (1 << D5_PIN) ? 1 : 0) << 5) |
		       ((PIN(D6_PORT) & (1 << D6_PIN) ? 1 : 0) << 6) |
		       ((PIN(D7_PORT) & (1 << D7_PIN) ? 1 : 0) << 7);

	PORT(E_PORT)  &= ~(1 << E_PIN);
	if (ic == ADM12864H_IC1) PORT(CS1_PORT) &= ~(1 << CS1_PIN);
	else 			 PORT(CS2_PORT) &= ~(1 << CS2_PIN);
	PORT(DI_PORT) &= ~(1 << DI_PIN);
	PORT(RW_PORT) &= ~(1 << RW_PIN);

	return data;
}

void adm12864h_write_command(uint8_t cmd, uint8_t ic) {
	while (adm12864h_read_status(ic) & ADM12864H_STATUS_BUSY);

	DDR(D0_PORT) |= (1 << D0_PIN);
	DDR(D1_PORT) |= (1 << D1_PIN);
	DDR(D2_PORT) |= (1 << D2_PIN);
	DDR(D3_PORT) |= (1 << D3_PIN);
	DDR(D4_PORT) |= (1 << D4_PIN);
	DDR(D5_PORT) |= (1 << D5_PIN);
	DDR(D6_PORT) |= (1 << D6_PIN);
	DDR(D7_PORT) |= (1 << D7_PIN);

	if (ic == ADM12864H_IC1) PORT(CS1_PORT) |= (1 << CS1_PIN);
	else 			 PORT(CS2_PORT) |= (1 << CS2_PIN);
	_delay_us(ADM12864H_TASU);
	PORT(E_PORT) |=  (1 << E_PIN);

	(cmd & (1 << 0)) == 0 ? (PORT(D0_PORT) &= ~(1 << D0_PIN)) :
				(PORT(D0_PORT) |=  (1 << D0_PIN));
	(cmd & (1 << 1)) == 0 ? (PORT(D1_PORT) &= ~(1 << D1_PIN)) :
				(PORT(D1_PORT) |=  (1 << D1_PIN));
	(cmd & (1 << 2)) == 0 ? (PORT(D2_PORT) &= ~(1 << D2_PIN)) :
				(PORT(D2_PORT) |=  (1 << D2_PIN));
	(cmd & (1 << 3)) == 0 ? (PORT(D3_PORT) &= ~(1 << D3_PIN)) :
				(PORT(D3_PORT) |=  (1 << D3_PIN));
	(cmd & (1 << 4)) == 0 ? (PORT(D4_PORT) &= ~(1 << D4_PIN)) :
				(PORT(D4_PORT) |=  (1 << D4_PIN));
	(cmd & (1 << 5)) == 0 ? (PORT(D5_PORT) &= ~(1 << D5_PIN)) :
				(PORT(D5_PORT) |=  (1 << D5_PIN));
	(cmd & (1 << 6)) == 0 ? (PORT(D6_PORT) &= ~(1 << D6_PIN)) :
				(PORT(D6_PORT) |=  (1 << D6_PIN));
	(cmd & (1 << 7)) == 0 ? (PORT(D7_PORT) &= ~(1 << D7_PIN)) :
				(PORT(D7_PORT) |=  (1 << D7_PIN));

	_delay_us(ADM12864H_TWH);

	PORT(E_PORT) &= ~(1 << E_PIN);
	if (ic == ADM12864H_IC1) PORT(CS1_PORT) &= ~(1 << CS1_PIN);
	else 			 PORT(CS2_PORT) &= ~(1 << CS2_PIN);
}

void adm12864h_write_data(uint8_t data, uint8_t ic) {
	while (adm12864h_read_status(ic) & ADM12864H_STATUS_BUSY);

	DDR(D0_PORT) |= (1 << D0_PIN);
	DDR(D1_PORT) |= (1 << D1_PIN);
	DDR(D2_PORT) |= (1 << D2_PIN);
	DDR(D3_PORT) |= (1 << D3_PIN);
	DDR(D4_PORT) |= (1 << D4_PIN);
	DDR(D5_PORT) |= (1 << D5_PIN);
	DDR(D6_PORT) |= (1 << D6_PIN);
	DDR(D7_PORT) |= (1 << D7_PIN);

	if (ic == ADM12864H_IC1) PORT(CS1_PORT) |= (1 << CS1_PIN);
	else 			 PORT(CS2_PORT) |= (1 << CS2_PIN);
	PORT(DI_PORT) |=  (1 << DI_PIN);
	_delay_us(ADM12864H_TASU);
	PORT(E_PORT)  |=  (1 << E_PIN);

	(data & (1 << 0)) == 0 ? (PORT(D0_PORT) &= ~(1 << D0_PIN)) :
				 (PORT(D0_PORT) |=  (1 << D0_PIN));
	(data & (1 << 1)) == 0 ? (PORT(D1_PORT) &= ~(1 << D1_PIN)) :
				 (PORT(D1_PORT) |=  (1 << D1_PIN));
	(data & (1 << 2)) == 0 ? (PORT(D2_PORT) &= ~(1 << D2_PIN)) :
				 (PORT(D2_PORT) |=  (1 << D2_PIN));
	(data & (1 << 3)) == 0 ? (PORT(D3_PORT) &= ~(1 << D3_PIN)) :
				 (PORT(D3_PORT) |=  (1 << D3_PIN));
	(data & (1 << 4)) == 0 ? (PORT(D4_PORT) &= ~(1 << D4_PIN)) :
				 (PORT(D4_PORT) |=  (1 << D4_PIN));
	(data & (1 << 5)) == 0 ? (PORT(D5_PORT) &= ~(1 << D5_PIN)) :
				 (PORT(D5_PORT) |=  (1 << D5_PIN));
	(data & (1 << 6)) == 0 ? (PORT(D6_PORT) &= ~(1 << D6_PIN)) :
				 (PORT(D6_PORT) |=  (1 << D6_PIN));
	(data & (1 << 7)) == 0 ? (PORT(D7_PORT) &= ~(1 << D7_PIN)) :
				 (PORT(D7_PORT) |=  (1 << D7_PIN));

	_delay_us(ADM12864H_TWH);

	PORT(E_PORT)  &= ~(1 << E_PIN);
	PORT(DI_PORT) &= ~(1 << DI_PIN);
	if (ic == ADM12864H_IC1) PORT(CS1_PORT) &= ~(1 << CS1_PIN);
	else 			 PORT(CS2_PORT) &= ~(1 << CS2_PIN);
}

void adm12864h_initialize(void) {
	DDR(D0_PORT)  |= (1 << D0_PIN);
	DDR(D1_PORT)  |= (1 << D1_PIN);
	DDR(D2_PORT)  |= (1 << D2_PIN);
	DDR(D3_PORT)  |= (1 << D3_PIN);
	DDR(D4_PORT)  |= (1 << D4_PIN);
	DDR(D5_PORT)  |= (1 << D5_PIN);
	DDR(D6_PORT)  |= (1 << D6_PIN);
	DDR(D7_PORT)  |= (1 << D7_PIN);
	DDR(CS1_PORT) |= (1 << CS1_PIN);
	DDR(CS2_PORT) |= (1 << CS2_PIN);
	DDR(RW_PORT)  |= (1 << RW_PIN);
	DDR(DI_PORT)  |= (1 << DI_PIN);
	DDR(E_PORT)   |= (1 << E_PIN);

	adm12864h_write_command(ADM12864H_ON_CMD, ADM12864H_IC1);
	adm12864h_write_command(ADM12864H_ON_CMD, ADM12864H_IC2);
}

void adm12864h_clear(void) {
	for (uint8_t y = 0; y < 8; y++) {
		adm12864h_write_command(ADM12864H_SET_X | 0, ADM12864H_IC1);
		adm12864h_write_command(ADM12864H_SET_Y | y, ADM12864H_IC1);
		adm12864h_write_command(ADM12864H_SET_X | 0, ADM12864H_IC2);
		adm12864h_write_command(ADM12864H_SET_Y | y, ADM12864H_IC2);
		for (uint8_t x = 0; x < 64; x++) {
			adm12864h_write_data(0x00, ADM12864H_IC1);
			adm12864h_write_data(0x00, ADM12864H_IC2);
		}
	}
}

void adm12864h_set_pixel(uint8_t x, uint8_t y, bool on) {
	uint8_t ic;
	uint8_t tmp;

	if (x > 127 || y > 63) return;

	if (x < 64) {
		ic = ADM12864H_IC1;
	} else {
		ic = ADM12864H_IC2;
		x -= 64;
	}

	adm12864h_write_command(ADM12864H_SET_X | x, ic);
	adm12864h_write_command(ADM12864H_SET_Y | (y / 8), ic);
	tmp = adm12864h_read_data(ic);

	adm12864h_write_command(ADM12864H_SET_X | x, ic);
	adm12864h_write_command(ADM12864H_SET_Y | (y / 8), ic);
	tmp = adm12864h_read_data(ic);

	adm12864h_write_command(ADM12864H_SET_X | x, ic);
	adm12864h_write_command(ADM12864H_SET_Y | (y / 8), ic);
	if (on) tmp |=  (1 << (y % 8));
	else	tmp &= ~(1 << (y % 8));
	adm12864h_write_data(tmp, ic);
}

#endif

#ifndef TWI_MASTER_H
#define TWI_MASTER_H

#include <avr/io.h>
#include <util/twi.h>
#include <stdbool.h>

void twi_initialize(uint32_t frequency) {
	static const uint8_t PRESCALER = 1;
	TWBR = (uint8_t)((((F_CPU / frequency) / PRESCALER) - 16) / 2);
}

bool twi_start(uint8_t address) {
	TWCR = 0;
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)));

	if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
		return false;

	TWDR = address;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)));

	if (TW_STATUS != TW_MT_SLA_ACK && TW_STATUS != TW_MR_SLA_ACK)
		return false;

	return true;
}

void twi_start_wait(uint8_t address) {
	while (1) {
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

		while (!(TWCR & (1 << TWINT)));

		if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START)
			continue;

		TWDR = address;
		TWCR = (1 << TWINT) | (1 << TWEN);

		while (!(TWCR & (1 << TWINT)));

		if (TW_STATUS != TW_MT_SLA_ACK &&TW_STATUS != TW_MR_SLA_ACK) {
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

			while (TWCR & (1 << TWSTO));

			continue;
		}

		break;
	}
}

void twi_stop(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

	while (TWCR & (1 << TWSTO));
}

bool twi_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)));

	if (TW_STATUS != TW_MT_DATA_ACK)
		return false;

	return true;
}

uint8_t twi_read_ack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

	while (!(TWCR & (1 << TWINT)));

	return TWDR;
}

uint8_t twi_read_nack(void) {
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)));

	return TWDR;
}

#endif

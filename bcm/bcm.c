// binary code modulation
// http://www.batsocks.co.uk/readme/art_bcm_1.htm

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#define BCM_USE_INTERRUPT false

typedef struct {
	volatile uint8_t *ddr;
	volatile uint8_t *port;
	uint8_t bit;
	uint8_t value;
} bcm;

bcm bcm_data[] = {
	{ &DDRC, &PORTC, (1 << PC0),   2 },
	{ &DDRC, &PORTC, (1 << PC1),   4 },
	{ &DDRC, &PORTC, (1 << PC2),   8 },
	{ &DDRC, &PORTC, (1 << PC3),  16 },
	{ &DDRC, &PORTC, (1 << PC4),  32 },
	{ &DDRC, &PORTC, (1 << PC5),  64 },
	{ &DDRD, &PORTD, (1 << PD2), 128 },
	{ &DDRD, &PORTD, (1 << PD3), 255 },
};

void bcm_initialize(void) {
	for (uint8_t i = 0; i < sizeof(bcm_data) / sizeof(bcm_data[0]); i++) {
		*(bcm_data[i].ddr)  |=  bcm_data[i].bit;
		*(bcm_data[i].port) &= ~(bcm_data[i].bit);
	}

	#ifdef BCM_USE_INTERRUPT
	TCCR0A |= (1 << WGM01); // ctc mode
	TCCR0B |= ((1 << CS01) | (1 << CS00)); // use clock/32 tickrate
	OCR0A = 1; // initial delay
	TIMSK0 |= (1 << OCIE0A); // enable the compare match interrupt
	sei();
	#endif
}

void bcm_update(uint8_t bit_pos) {
	for (uint8_t i = 0; i < sizeof(bcm_data) / sizeof(bcm_data[0]); i++) {
		if (bcm_data[i].value & (1 << bit_pos))
			*(bcm_data[i].port) |=  (bcm_data[i].bit);
		else
			*(bcm_data[i].port) &= ~(bcm_data[i].bit);
	}
}

#ifdef BCM_USE_INTERRUPT
// timer interrupt handler, called once per bit position
ISR(TIMER0_COMPA_vect) {
	static uint8_t bit_pos = 0;

	bit_pos++;
	bit_pos &= 0x07;

	bcm_update(bit_pos);

	// set delay
	TCNT0 = 0;
	OCR0A <<= 1;

	// reset compare match value
	if (bit_pos == 0) OCR0A = 1;
}
#endif

int main(void) {
	bcm_initialize();

	#ifdef BCM_USE_INTERRUPT
	while (1);
	#else
	while (1) {
		static uint8_t bit_pos = 0;

		bit_pos++;
		bit_pos &= 0x07;

		bcm_update(bit_pos);

		for (uint8_t i = 0; i < (1 << bit_pos); i++)
			_delay_us(8);
	}
	#endif
}

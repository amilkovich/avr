#ifndef USART_H
#define USART_H

#include <avr/io.h>

#ifdef __AVR_ATmega32U4__
	#define BAUD_RATE_REGH UBRR1H
	#define BAUD_RATE_REGL UBRR1L
	#define CONTROL_STATUS_REGA UCSR1A
	#define CONTROL_STATUS_REGB UCSR1B
	#define CONTROL_STATUS_REGC UCSR1C
	#define DATA_REG UDR1
	#define DATA_REG_EMPTY UDRE1
	#define CHARACTER_SIZE UCSZ10
	#define RECEIVER_ENABLE RXEN1
	#define TRANSMITTER_ENABLE TXEN1
	#define RECEIVE_COMPLETE RXC1
#else
	#define BAUD_RATE_REGH UBRR0H
	#define BAUD_RATE_REGL UBRR0L
	#define CONTROL_STATUS_REGA UCSR0A
	#define CONTROL_STATUS_REGB UCSR0B
	#define CONTROL_STATUS_REGC UCSR0C
	#define DATA_REG UDR0
	#define DATA_REG_EMPTY UDRE0
	#define CHARACTER_SIZE UCSZ00
	#define RECEIVER_ENABLE RXEN0
	#define TRANSMITTER_ENABLE TXEN0
	#define RECEIVE_COMPLETE RXC0
#endif

void usart_initialize(uint32_t baud_rate) {
	uint32_t baud_prescaler = (((F_CPU / (baud_rate * 16UL))) - 1);
	BAUD_RATE_REGH = (uint8_t)(baud_prescaler >> 8);
	BAUD_RATE_REGL = (uint8_t)(baud_prescaler);
	CONTROL_STATUS_REGB = (1 << RECEIVER_ENABLE) | (1 << TRANSMITTER_ENABLE);
	CONTROL_STATUS_REGC = (3 << CHARACTER_SIZE);
}

void usart_send(uint8_t data) {
	while (!(CONTROL_STATUS_REGA & (1 << DATA_REG_EMPTY)));
	DATA_REG = data;
}

void usart_put_string(char *msg) {
	while (*msg != '\0') {
		usart_send(*msg);
		msg++;
	}
}

uint8_t usart_receive(void) {
	while ((CONTROL_STATUS_REGA & (1 << RECEIVE_COMPLETE)) == 0);
	return DATA_REG;
}

void usart_get_string(char *str, uint8_t max_len) {
	uint8_t byte, len = 0;
	while ((byte = usart_receive()) != '\n') {
		if (len < max_len - 1) {
			str[len] = byte;
			len++;
		}
	}
	str[len] = '\0';
}

#endif

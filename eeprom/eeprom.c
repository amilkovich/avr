#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include <stdio.h>

#include "mcp24lc512.h"

int main(void) {
	usart_initialize(9600);
	usart_put_string("Hello from ATmega328P!\n");

	DDRB = (1 << PB0);

	mcp24lc512_initialize();

	/*uint8_t write_data[MCP24LC512_PAGE_SIZE];
	memset(write_data, 0xff, MCP24LC512_PAGE_SIZE);
	write_data[0] = 0xde;
	write_data[1] = 0xad;
	write_data[2] = 0xbe;
	write_data[3] = 0xef;
	write_data[124] = 0xca;
	write_data[125] = 0xfe;
	write_data[126] = 0xba;
	write_data[127] = 0xbe;
	mcp24lc512_write_page(MCP24LC512_BASE_ADDRESS, 0x0000, write_data);*/

	//mcp24lc512_erase(MCP24LC512_BASE_ADDRESS);

	uint8_t read_data[MCP24LC512_PAGE_SIZE];
	mcp24lc512_read_page(MCP24LC512_BASE_ADDRESS, 0x0000, read_data);
	for (uint8_t j = 0; j < 8; j++) {
		for (uint8_t i = 0; i < 16; i++) {
			char buffer[16];
			sprintf(buffer, "%02x ", read_data[j * 16 + i]);
			usart_put_string(buffer);
		}
		usart_put_string("\n");
	}

	while (1) {
		PORTB ^= (1 << PB0);
		_delay_ms(100);
	}
}

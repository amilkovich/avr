#ifndef MCP24LC512_H
#define MCP24LC512_H

#include "twi_master.h"
#include <string.h>

#define MCP24LC512_BASE_ADDRESS 0x50
#define MCP24LC512_PAGE_SIZE    0x80
#define MCP24LC512_PAGES        0x1ff

void mcp24lc512_initialize(void) {
	twi_initialize(400000);
}

uint8_t mcp24lc512_read(uint8_t address, uint16_t eeprom_address) {
	twi_start_wait((address << 1) | TW_WRITE);
	twi_write((uint8_t)(eeprom_address >> 8));
	twi_write((uint8_t)(eeprom_address >> 0));
	twi_start((address << 1) | TW_READ);
	uint8_t data = twi_read_nack();
	twi_stop();

	return data;
}

void mcp24lc512_read_page(uint8_t address, uint16_t page_address,
			  uint8_t data[MCP24LC512_PAGE_SIZE]) {
	if (page_address > MCP24LC512_PAGES)
		return;

	uint16_t byte_address =
		(uint16_t)(page_address * (uint16_t)MCP24LC512_PAGE_SIZE);

	twi_start_wait((address << 1) | TW_WRITE);
	twi_write((uint8_t)(byte_address >> 8));
	twi_write((uint8_t)(byte_address >> 0));
	twi_start((address << 1) | TW_READ);
	for (uint8_t i = 0; i < MCP24LC512_PAGE_SIZE - 1; i++)
		data[i] = twi_read_ack();
	data[MCP24LC512_PAGE_SIZE - 1] = twi_read_nack();
	twi_stop();
}

void mcp24lc512_write(uint8_t address, uint16_t eeprom_address, uint8_t data) {
	twi_start_wait((address << 1) | TW_WRITE);
	twi_write((uint8_t)(eeprom_address >> 8));
	twi_write((uint8_t)(eeprom_address >> 0));
	twi_write(data);
	twi_stop();
}

void mcp24lc512_write_page(uint8_t address, uint16_t page_address,
			   uint8_t data[MCP24LC512_PAGE_SIZE]) {
	if (page_address > MCP24LC512_PAGES)
		return;

	uint16_t byte_address =
		(uint16_t)(page_address * (uint16_t)MCP24LC512_PAGE_SIZE);

	twi_start_wait((address << 1) | TW_WRITE);
	twi_write((uint8_t)(byte_address >> 8));
	twi_write((uint8_t)(byte_address >> 0));
	for (uint8_t i = 0; i < MCP24LC512_PAGE_SIZE; i++)
		twi_write(data[i]);
	twi_stop();
}

void mcp24lc512_erase(uint8_t address) {
	uint8_t data[MCP24LC512_PAGE_SIZE];

	memset(data, 0xff, MCP24LC512_PAGE_SIZE);

	for (uint16_t i = 0; i < MCP24LC512_PAGES; i++)
		mcp24lc512_write_page(address, i, data);
}

#endif

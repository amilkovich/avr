#ifndef MCP24LC22A_H
#define MCP24LC22A_H

#include "twi_master.h"
#include "portability.h"
#include <string.h>

#define MCP24LC22A_PAGE_SIZE 0x08
#define MCP24LC22A_PAGES     0x1f

void mcp24lc22a_initialize(void) {
	twi_initialize(400000);

	DDR(VCLK_PORT) |= (1 << VCLK_PIN);

	// 9 vclk pulses for internal synchronization
	for (uint8_t i = 0; i < 9; i++) {
		PORT(VCLK_PORT) |=  (1 << VCLK_PIN);
		PORT(VCLK_PORT) &= ~(1 << VCLK_PIN);
	}
}

uint8_t mcp24lc22a_read(uint8_t eeprom_address) {
	twi_start_wait((0x50 << 1) | TW_WRITE);
	twi_write(eeprom_address);
	twi_start((0x50 << 1) | TW_READ);
	uint8_t data = twi_read_nack();
	twi_stop();

	return data;
}

void mcp24lc22a_read_page(uint8_t page_address,
			  uint8_t data[MCP24LC22A_PAGE_SIZE]) {
	if (page_address > MCP24LC22A_PAGES)
		return;

	uint8_t byte_address = page_address * MCP24LC22A_PAGE_SIZE;

	twi_start_wait((0x50 << 1) | TW_WRITE);
	twi_write(byte_address);
	twi_start((0x50 << 1) | TW_READ);
	for (uint8_t i = 0; i < MCP24LC22A_PAGE_SIZE - 1; i++)
		data[i] = twi_read_ack();
	data[MCP24LC22A_PAGE_SIZE - 1] = twi_read_nack();
	twi_stop();
}

void mcp24lc22a_write(uint8_t eeprom_address, uint8_t data) {
	PORT(VCLK_PORT) |= (1 << VCLK_PIN);
	twi_start_wait((0x50 << 1) | TW_WRITE);
	twi_write(eeprom_address);
	twi_write(data);
	twi_stop();
	PORT(VCLK_PORT) &= ~(1 << VCLK_PIN);
}

void mcp24lc22a_write_page(uint8_t page_address,
			   uint8_t data[MCP24LC22A_PAGE_SIZE]) {
	if (page_address > MCP24LC22A_PAGES)
		return;

	uint8_t byte_address = page_address * MCP24LC22A_PAGE_SIZE;

	PORT(VCLK_PORT) |= (1 << VCLK_PIN);
	twi_start_wait((0x50 << 1) | TW_WRITE);
	twi_write(byte_address);
	for (uint8_t i = 0; i < MCP24LC22A_PAGE_SIZE; i++)
		twi_write(data[i]);
	twi_stop();
	PORT(VCLK_PORT) &= ~(1 << VCLK_PIN);
}

void mcp24lc22a_erase(void) {
	uint8_t data[MCP24LC22A_PAGE_SIZE];

	memset(data, 0xff, MCP24LC22A_PAGE_SIZE);

	for (uint16_t i = 0; i < MCP24LC22A_PAGES; i++)
		mcp24lc22a_write_page(i, data);
}

#endif

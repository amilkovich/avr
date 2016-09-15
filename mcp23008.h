#ifndef MCP23008_H
#define MCP23008_H

#include "twi_master.h"

#define MCP23008_BASE_ADDRESS 0x20

#define MCP23008_IODIR   0x00
#define MCP23008_IPOL    0x01
#define MCP23008_GPINTEN 0x02
#define MCP23008_DEFVAL  0x03
#define MCP23008_INTCON  0x04
#define MCP23008_IOCON   0x05
#define MCP23008_GPPU    0x06
#define MCP23008_INTF    0x07
#define MCP23008_INTCAP  0x08
#define MCP23008_GPIO    0x09
#define MCP23008_OLAT    0x0a

void mcp23008_write_register(uint8_t address, uint8_t reg, uint8_t data) {
	twi_start_wait((address << 1) | TW_WRITE);
	twi_write(reg);
	twi_write(data);
	twi_stop();
}

void mcp23008_initialize(uint8_t address) {
	twi_initialize(400000);
	mcp23008_write_register(address, MCP23008_IODIR, 0x00);
}

void mcp23008_ddr(uint8_t address, uint8_t ddr) {
	mcp23008_write_register(address, MCP23008_IODIR, ddr);
}

void mcp23008_set(uint8_t address, uint8_t data) {
	mcp23008_write_register(address, MCP23008_GPIO, data);
}

uint8_t mcp23008_get(uint8_t address) {
	twi_start_wait((address << 1) | TW_WRITE);
	twi_write(MCP23008_GPIO);
	twi_start((address << 1) | TW_READ);
	uint8_t data = twi_read_nack();
	twi_stop();

	return data;
}

#endif

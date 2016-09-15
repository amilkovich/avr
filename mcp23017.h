#ifndef MCP23017_H
#define MCP23017_H

#include "twi_master.h"

#define MCP23017_BASE_ADDRESS 0x20

#define MCP23017_IODIRA   0x00
#define MCP23017_IODIRB   0x01
#define MCP23017_IPOLA    0x02
#define MCP23017_IPOLB    0x03
#define MCP23017_GPINTENA 0x04
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALA  0x06
#define MCP23017_DEFVALB  0x07
#define MCP23017_INTCONA  0x08
#define MCP23017_INTCONB  0x09
#define MCP23017_IOCON    0x0a
#define MCP23017_GPPUA    0x0c
#define MCP23017_GPPUB    0x0d
#define MCP23017_INTFA    0x0e
#define MCP23017_INTFB    0x0f
#define MCP23017_INTCAPA  0x10
#define MCP23017_INTCAPB  0x11
#define MCP23017_GPIOA    0x12
#define MCP23017_GPIOB    0x13
#define MCP23017_OLATA    0x14
#define MCP23017_OLATB    0x15

void mcp23017_write_register(uint8_t address, uint8_t reg, uint8_t data) {
	twi_start_wait((address << 1) | TW_WRITE);
	twi_write(reg);
	twi_write(data);
	twi_stop();
}

void mcp23017_initialize(uint8_t address) {
	twi_initialize(400000);
	mcp23017_write_register(address, MCP23017_IODIRA, 0x00);
	mcp23017_write_register(address, MCP23017_IODIRB, 0x00);
}

void mcp23017_ddr(uint8_t address, uint16_t ddr) {
	mcp23017_write_register(address, MCP23017_IODIRA, ddr >> 8);
	mcp23017_write_register(address, MCP23017_IODIRB, ddr >> 0);
}

void mcp23017_set(uint8_t address, uint16_t data) {
	mcp23017_write_register(address, MCP23017_GPIOA, data >> 8);
	mcp23017_write_register(address, MCP23017_GPIOB, data >> 0);
}

uint16_t mcp23017_get(uint8_t address) {
	uint16_t data;

	twi_start_wait((address << 1) | TW_WRITE);
	twi_write(MCP23017_GPIOA);
	twi_start((address << 1) | TW_READ);
	data = ((uint16_t)twi_read_nack()) << 8;
	twi_stop();

	twi_start_wait((address << 1) | TW_WRITE);
	twi_write(MCP23017_GPIOB);
	twi_start((address << 1) | TW_READ);
	data |= ((uint16_t)twi_read_nack()) << 0;
	twi_stop();

	return data;
}

#endif

#ifndef W25Q80BV_H
#define W25Q80BV_H

#include <avr/io.h>

#include "spi_master.h"

#define W25Q80BV_MANUFACTURER_ID  0xef
#define W25Q80BV_DEVICE_ID        0x13
#define W25Q80BV_ID               0x90
#define W25Q80BV_READ_STATUS_REG1 0x05
#define W25Q80BV_READ_DATA        0x03
#define W25Q80BV_WRITE_ENABLE     0x06
#define W25Q80BV_PAGE_PROGRAM     0x02
#define W25Q80BV_CHIP_ERASE       0xc7
#define W25Q80BV_SECTOR_ERASE     0x20

//#define W25Q80BV_PAGES            4096
//#define W25Q80BV_BYTES_PER_PAGE   256
//#define W25Q80BV_TOTAL_BYTES      (PAGES * BYTES_PER_PAGE)
//#define W25Q80BV_SECTORS          256
//#define W25Q80BV_BYTES_PER_SECTOR 4096
//#define W25Q80BV_PAGES_PER_SECTOR (SECTOR_SIZE / BYTES_PER_PAGE)

void w25q80bv_initialize(void) {
	spi_initialize(SPI_MODE_0, SPI_DATA_ORDER_MSB_FIRST, SPI_SPEED_DIV_128);
}

void w25q80bv_select(void) {
	PORT(SS_PORT) &= ~(1 << SS_PIN);
}

void w25q80bv_deselect(void) {
	PORT(SS_PORT) |= (1 << SS_PIN);
}

void w25q80bv_id(uint8_t *manufacturer, uint8_t *device) {
	w25q80bv_select();
	spi_transfer(W25Q80BV_ID);
	spi_transfer(0x00);
	spi_transfer(0x00);
	spi_transfer(0x00);
	*manufacturer = spi_transfer(0x00);
	*device = spi_transfer(0x00);
	w25q80bv_deselect();
}

void w25q80bv_read_page(uint16_t page, uint8_t data[256]) {
	w25q80bv_select();
	spi_transfer(W25Q80BV_READ_DATA);
	spi_transfer((page >> 8) & 0xff);
	spi_transfer((page >> 0) & 0xff);
	spi_transfer(0x00);
	for (uint16_t page_address = 0; page_address < 256; page_address++)
		data[page_address] = spi_transfer(0x00);
	w25q80bv_deselect();
}

void w25q80bv_wait(void) {
	w25q80bv_select();
	spi_transfer(W25Q80BV_READ_STATUS_REG1);
	while (spi_transfer(0x00) & 0x01);
	w25q80bv_deselect();
}

void w25q80bv_write_page(uint16_t page, uint8_t data[256]) {
	w25q80bv_select();
	spi_transfer(W25Q80BV_WRITE_ENABLE);
	w25q80bv_deselect();
	w25q80bv_select();
	spi_transfer(W25Q80BV_PAGE_PROGRAM);
	spi_transfer((page >> 8) & 0xff);
	spi_transfer((page >> 0) & 0xff);
	spi_transfer(0x00);
	for (uint16_t page_address = 0; page_address < 256; page_address++)
		spi_transfer(data[page_address]);
	w25q80bv_deselect();
	w25q80bv_wait();
}

void w25q80bv_erase_chip(void) {
	w25q80bv_select();
	spi_transfer(W25Q80BV_WRITE_ENABLE);
	w25q80bv_deselect();
	w25q80bv_select();
	spi_transfer(W25Q80BV_CHIP_ERASE);
	w25q80bv_deselect();
	w25q80bv_wait();
}

void w25q80bv_erase_sector(uint16_t sector) {
	w25q80bv_select();
	w25q80bv_deselect();
	spi_transfer(W25Q80BV_WRITE_ENABLE);
	w25q80bv_deselect();
	w25q80bv_select();
	spi_transfer(W25Q80BV_SECTOR_ERASE);
	sector = sector * 16;
	spi_transfer((sector >> 8) & 0xff);
	spi_transfer((sector >> 0) & 0xff);
	spi_transfer(0x00);
	w25q80bv_deselect();
	w25q80bv_wait();
}

#endif

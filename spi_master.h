#ifndef SPI_H
#define SPI_H

#include "portability.h"

/*
SPCR
SPIE SPE DORD MSTR CPOL CPHA SPR1 SPR0

SPIE
0    spi interrupt disable
1    spi interrupt enable

SPE
0   spi disable
1   spi enable

MSTR
0    slave
1    master

CPOL CPHA Sample                  spi mode
0    0    leading (rising) edge   0
0    1    trailing (falling) edge 1
1    0    leading (falling) edge  2
1    1    trailing (rising) edge  3

DORD
0    MSB first
1    LSB first

SPI2X SPR1 SPR0 SCK frequency
1     0    0    fosc/2
0     0    0    fosc/4
1     0    1    fosc/8
0     0    1    fosc/16
1     1    0    fosc/32
0     1    0    fosc/64
1     1    1    fosc/64
0     1    1    fosc/128
*/

typedef enum {
	SPI_MODE_0 = (0 << CPOL) | (0 << CPHA),
	SPI_MODE_1 = (0 << CPOL) | (1 << CPHA),
	SPI_MODE_2 = (1 << CPOL) | (0 << CPHA),
	SPI_MODE_3 = (1 << CPOL) | (1 << CPHA)
} SPI_MODE;

typedef enum {
	SPI_DATA_ORDER_MSB_FIRST = (0 << DORD),
	SPI_DATA_ORDER_LSB_FIRST = (1 << DORD)
} SPI_DATA_ORDER;

typedef enum uint16_t {
	SPI_SPEED_DIV_2   = ((1 << SPI2X) << 8) | ((0 << SPR1) | (0 << SPR0)),
	SPI_SPEED_DIV_4   = ((0 << SPI2X) << 8) | ((0 << SPR1) | (0 << SPR0)),
	SPI_SPEED_DIV_8   = ((1 << SPI2X) << 8) | ((0 << SPR1) | (1 << SPR0)),
	SPI_SPEED_DIV_16  = ((0 << SPI2X) << 8) | ((0 << SPR1) | (1 << SPR0)),
	SPI_SPEED_DIV_32  = ((1 << SPI2X) << 8) | ((1 << SPR1) | (0 << SPR0)),
	SPI_SPEED_DIV_64  = ((1 << SPI2X) << 8) | ((1 << SPR1) | (1 << SPR0)),
	SPI_SPEED_DIV_128 = ((0 << SPI2X) << 8) | ((1 << SPR1) | (1 << SPR0))
} SPI_SPEED;

void spi_initialize(SPI_MODE mode, SPI_DATA_ORDER order, SPI_SPEED speed) {
	DDR(SS_PORT)   |=  (1 << SS_PIN);
	DDR(MOSI_PORT) |=  (1 << MOSI_PIN);
	DDR(SCK_PORT)  |=  (1 << SCK_PIN);
	DDR(MISO_PORT) &= ~(1 << MISO_PIN);
	PORT(SS_PORT)  |=  (1 << SS_PIN);

	SPCR  = (0 << SPIE) | (1 << SPE) | order | (1 << MSTR) | mode |
		(speed & 0xff);
	SPSR  = (speed >> 8);
}

void spi_set_speed(SPI_SPEED speed) {
	SPCR  = (SPCR & ~((1 << SPR1) | (1 << SPR0))) |
		(speed & 0xff);
	SPSR  = (SPSR & ~(1 << SPI2X)) |
		(speed >> 8);
}

uint8_t spi_transfer(uint8_t byte) {
	SPDR = byte;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}

#endif

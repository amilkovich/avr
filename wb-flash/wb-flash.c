#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#define SS_PORT   B
#define SS_PIN    2
#define MOSI_PORT B
#define MOSI_PIN  3
#define MISO_PORT B
#define MISO_PIN  4
#define SCK_PORT  B
#define SCK_PIN   5

#include "w25q80bv.h"
#include "usart.h"

int main(void) {
	usart_initialize(57600);

	w25q80bv_initialize();

	uint8_t man_id, dev_id;
	w25q80bv_id(&man_id, &dev_id);
	if (man_id != W25Q80BV_MANUFACTURER_ID ||
	    dev_id != W25Q80BV_DEVICE_ID) {
		usart_put_string("Error initializing W25Q80BV\n");
		while (1);
	} else {
		char man_id_str[5];
		char dev_id_str[5];
		usart_put_string("Initialized W25Q80BV!\n");
		usart_put_string("Manufacturer ID: ");
		sprintf(man_id_str, "0x%02x", man_id);
		usart_put_string(man_id_str);
		usart_put_string("\n");

		usart_put_string("Device ID: ");
		sprintf(dev_id_str, "0x%02x", dev_id);
		usart_put_string(dev_id_str);
		usart_put_string("\n");
		while (1);
	}
}

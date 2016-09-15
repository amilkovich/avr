#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN (1 << PB0)
#define LED_PWM OCR0A

int main(void) {
	uint8_t pwm = 0;

	DDRB = LED_PIN;

	TCCR0A = (1 << WGM00) | (1 << WGM01) | (1 << COM0A1);
	TCCR0B = (1 << CS00);
	LED_PWM = 0;

	while (1) {
		while (pwm < 255) {
			LED_PWM = pwm++;
			_delay_ms(4);
		}

		while (pwm > 0) {
			LED_PWM = pwm--;
			_delay_ms(4);
		}
	}
}

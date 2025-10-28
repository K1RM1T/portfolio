#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LEDS 4
#define BUTTON1 PD2
#define BUTTON2 PD3
#define BUTTON3 PD4

#define MIN_DELAY 50
#define MAX_DELAY 500
#define DELAY_STEP 50

volatile uint8_t is_on = 0;
volatile uint8_t mode = 0;
volatile uint16_t delay_time = 200;

void init_ports() {
	DDRB = 0b00001111;
	PORTB = 0b00000000;
	DDRD &= ~((1 << BUTTON1) | (1 << BUTTON2) | (1 << BUTTON3));
	PORTD |= (1 << BUTTON1) | (1 << BUTTON2) | (1 << BUTTON3);
}

void init_interrupts() {
	// Для ATmega16 используем GICR и SREG
	GICR |= (1 << INT0);    // Разрешаем прерывание INT0
	MCUCR |= (1 << ISC01);  // Настраиваем на срабатывание по спаду
}

ISR(INT0_vect) {
	cli();
	is_on = !is_on;
	sei();
}

void delay_ms(uint16_t delay) {
	for (uint16_t i = 0; i < delay; i++) {
		_delay_ms(1);
	}
}

void handle_button() {
	if (!(PIND & (1 << BUTTON2))) {
		_delay_ms(50);
		if (!(PIND & (1 << BUTTON2))) {
			mode = (mode + 1) % 3;
			while (!(PIND & (1 << BUTTON2)));
		}
	}
	
	if (!(PIND & (1 << BUTTON3))) {
		_delay_ms(50);
		if (!(PIND & (1 << BUTTON3))) {
			delay_time += DELAY_STEP;
			if (delay_time > MAX_DELAY) {
				delay_time = MIN_DELAY;
			}
			while (!(PIND & (1 << BUTTON3)));
		}
	}
}

void mode_chase() {
	for (uint8_t i = 0; i < NUM_LEDS; i++) {
		PORTB = (1 << i);
		delay_ms(delay_time);
	}
}

void mode_all_blink() {
	PORTB = (1 << NUM_LEDS) - 1;
	delay_ms(delay_time);
	PORTB = 0;
	delay_ms(delay_time);
}

void mode_random_blink() {
	PORTB = rand() % (1 << NUM_LEDS);
	delay_ms(delay_time);
}

int main(void) {
	srand(time(NULL));
	init_ports();
	init_interrupts();
	sei();
	
	while (1) {
		handle_button();
		
		if (is_on) {
			switch (mode) {
				case 0:
				mode_chase();
				break;
				case 1:
				mode_all_blink();
				break;
				case 2:
				mode_random_blink();
				break;
				default:
				mode = 0;
				break;
			}
			} else {
			PORTB = 0;
		}
	}
	return 0;
}
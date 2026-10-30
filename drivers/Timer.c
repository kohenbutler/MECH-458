#include "Timer.h"
#include <avr/io.h>

void timer1_init(void){
	TCCR1B |= _BV(WGM12); // Auto clear timer on match
	OCR1A = 1000; // Timer counts to 1000 (1ms)
	TCNT1 = 0x0000; // Reset counter to 0 on match
	TIFR1 = _BV(OCF1A); // Clear any pending compare-match flag
	TCCR1B |= _BV(CS11); // Start Timer1 with a /8 prescaler (1MHz)
	return;
}

void timer_delay_ms(int ms){
	while (ms > 0){
		while (!(TIFR1 & _BV(OCF1A))){} // Wait for the next compare match
		TIFR1 |= _BV(OCF1A); // Clear the flag by writing a 1
		ms -= 1; // Decrement the counter
	}
	return;
}
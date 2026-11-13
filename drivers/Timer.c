#include "Timer.h"
#include <avr/io.h>

void timer1_init(){
	TCCR1B |= _BV(WGM12); // Auto clear timer on match
	OCR1A = 1000;         // Timer counts to 1000 (1ms)
	TCNT1 = 0x0000;       // Reset counter to 0
	TIFR1 |= _BV(OCF1A);  // Clear any pending compare-match flag
	TCCR1B |= _BV(CS11);  // Start Timer1 with a /8 prescaler (1MHz)
	return;
}

void timer_delay_ms(int ms){
	TCNT1 = 0x0000;      // Reset counter so we start a fresh 1ms
	TIFR1 |= _BV(OCF1A); // Clear any pending flag
	while (ms > 0){
		while (!(TIFR1 & _BV(OCF1A))){} // Wait for the next compare match
		TIFR1 |= _BV(OCF1A); // Clear the flag by writing a 1
		ms -= 1; // Decrement the counter
	}
	return;
}

void timer_delay_us(int us){
	// Handle full milliseconds first
	if (us >= 1000) {
		timer_delay_ms(us/1000);
		us %= 1000; // Get the remaining microseconds
	}
	TCNT1 = 0x0000;
	TIFR1 |= _BV(OCF1A); // Clear any pending flag
	// Wait until TCNT1 reaches the target microsecond count
	while (TCNT1 < us) {
		// Busy wait
	}
	return;
}
#include "Timer.h"
#include "Config.h"
#include "DCmotor.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int timer3_ms_counter = 0;
volatile unsigned int timer4_ms_counter = 0;
volatile int ramp_down_flag = 0;

void timer1_init(){
	TCCR1B |= _BV(WGM12); // Auto clear timer on match
	OCR1A = 1000;         // Timer counts to 1000 (1ms)
	TCNT1 = 0;            // Reset counter to 0
	TIFR1 |= _BV(OCF1A);  // Clear any pending compare-match flag
	TCCR1B |= _BV(CS11);  // Start Timer1 with a /8 prescaler (1MHz)
	return;
}

void timer3_init(){
	TCCR3B |= _BV(WGM32);     // Auto clear timer on match
	OCR3A = 1000;             // Timer counts to 1000 (1ms)
	TCNT3 = 0;                // Reset counter to 0
	TIFR3 |= _BV(OCF3A);      // Clear any pending compare-match flag
	TIMSK3 &= ~(_BV(OCIE3A)); // Disable interrupts until timer starts
	TCCR3B &= 0b11111000;     // Stop timer
}

void timer4_init(){
	TCCR4B |= _BV(WGM42);     // Auto clear timer on match
	OCR4A = 1000;             // Timer counts to 1000 (1ms)
	TCNT4 = 0;                // Reset counter to 0
	TIFR4 |= _BV(OCF4A);      // Clear any pending compare-match flag
	TIMSK4 &= ~(_BV(OCIE4A)); // Disable interrupts until timer starts
	TCCR4B &= 0b11111000;     // Stop timer
}

void timer3_start(){
	timer3_ms_counter = 0;  // Reset ms counter
	TCNT3 = 0;              // Reset timer3 counter
	TIFR3 = _BV(OCF3A);     // Clear pending compare match
	TIMSK3 |= _BV(OCIE3A);  // Enable Interrupts
	TCCR3B |= _BV(CS31);    // Start timer with prescaler /8
}

void timer4_start(){
	timer4_ms_counter = 0;  // Reset ms counter
	TCNT4 = 0;              // Reset timer3 counter
	TIFR4 = _BV(OCF4A);     // Clear pending compare match
	TIMSK4 |= _BV(OCIE4A);  // Enable Interrupts
	TCCR4B |= _BV(CS41);    // Start timer with prescaler /8
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

int is_ramp_down_flag(){
	return ramp_down_flag;
}

ISR(TIMER3_COMPA_vect)
{
	timer3_ms_counter += 1;                    // After 1ms increment ms counter
	if(timer3_ms_counter >= BRAKING_TIME_MS){  // if ms counter > set delay
		TCCR3B &= 0b11111000;                  // Stop Timer3
		TIMSK3 &= ~(_BV(OCIE3A));              // Disable interrupt
		set_block_at_end_flag();               // Braking interval complete
	}
}

ISR(TIMER4_COMPA_vect)
{
	timer4_ms_counter += 1;                      // After 1ms increment ms counter
	if(timer4_ms_counter >= RAMP_DOWN_TIME_MS){  // if ms counter > set delay
		TCCR4B &= 0b11111000;                    // Stop Timer3
		TIMSK4 &= ~(_BV(OCIE4A));                // Disable interrupt
		ramp_down_flag = 1;
	}
}

#include <avr/io.h>
#include "pwm.h"
#include "Config.h"
#include "Board.h"

void pwm_init() {
	// Fast PWM Mode (WGM bits = 011)
	TCCR0A |= 0b00000011;
	TCCR0B &= 0b11110111;
	
	// Clear OC0A on Compare Match
	TCCR0A |= 0b10000000;
	TCCR0A &= 0b10111111;
	
	// Set clock prescaler to /64
	TCCR0B |= 0b00000011;
	TCCR0B &= 0b11111011;
	
	// Set duty cycle
	OCR0A = DC_MOTOR_SPEED;
	
	// Set PB7 to output PWM
	PWM_DDR = PWM_MASK;
}

void pwm_set_duty_cycle(unsigned int duty) {
	// Update the Output Compare Register directly
	OCR0A = duty;
}
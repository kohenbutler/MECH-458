#include <avr/io.h>
#include <avr/interrupt.h>
#include "Buttons.h"
#include "Board.h"
#include "Config.h"
#include "DCmotor.h"
#include "Timer.h"

int left_button_pressed = 0;
int right_button_pressed = 0;

void buttons_init(){
	BUTTON_DDR &= ~(BUTTON_MASK);
	
	EIMSK |= _BV(INT1);                   // Enable INT1
	EICRA |= _BV(ISC11);                  // Falling edge trigger for active low
	
	EIMSK |= _BV(INT2);                   // Enable INT2
	EICRA |= (_BV(ISC21) | _BV(ISC20));   // Rising edge trigger for active high
}

void Enable_INT1(){
	EIFR |= _BV(INTF1);   // Clear pending flags
	EIMSK |= _BV(INT1);   // Enable INT1
}

void Disable_INT1(){
	EIMSK &= 0b11111101;  // Disable INT1
}

void Enable_INT2(){
	EIFR |= _BV(INTF2);   // Clear pending flags
	EIMSK |= _BV(INT2);   // Enable INT2
	return;
}

void Disable_INT2(){
	EIMSK &= 0b11111011;  // Disable INT2
	return;
}

void set_left_button_pressed(){
	left_button_pressed = 1;
	return;
}

void clear_left_button_pressed(){
	left_button_pressed = 0;
	return;
}

int was_left_button_pressed(){
	return left_button_pressed;
}

void set_right_button_pressed(){
	right_button_pressed = 1;
	return;
}

void clear_right_button_pressed(){
	right_button_pressed = 0;
	return;
}

int was_right_button_pressed(){
	return right_button_pressed;
}

ISR(INT1_vect) {
	set_left_button_pressed();
	Disable_INT1();       // Temporrily disable for debouncing
}

ISR(INT2_vect) {
	set_right_button_pressed();
	Disable_INT2();       // Temporarily disable for debouncing
}

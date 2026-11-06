#include <avr/io.h>
#include <avr/interrupt.h>
#include "Buttons.h"
#include "Board.h"
#include "Config.h"
#include "DCmotor.h"
#include "Timer.h"

int pause_button_pressed = 0;

void buttons_init(void){
	BUTTON_DDR = BUTTON_MASK;
	
	EIMSK |= _BV(INT1);                   // Enable INT1
	EICRA |= _BV(ISC11);                  // Falling edge trigger for active low
	
	EIMSK |= _BV(INT2);                   // Enable INT2
	EICRA |= (_BV(ISC21) | _BV(ISC20));   // Rising edge trigger for active high
}

void Enable_INT1(void){
	EIFR |= _BV(INTF1);   // Clear pending flags
	EIMSK |= _BV(INT1);   // Enable INT1
}

void Disable_INT1(void){
	EIMSK &= 0b11111101;  // Disable INT1
}

void Enable_INT2(void){
	EIFR |= _BV(INTF2);   // Clear pending flags
	EIMSK |= _BV(INT2);   // Enable INT2
	return;
}

void Disable_INT2(void){
	EIMSK &= 0b11111011;  // Disable INT2
	return;
}

void set_pause_button_pressed(){
	pause_button_pressed = 1;
	return;
}

void clear_pause_button_pressed(){
	pause_button_pressed = 0;
	return;
}

int was_pause_button_pressed(){
	return pause_button_pressed;
}

ISR(INT1_vect) {
	set_pause_button_pressed();
	Disable_INT1();
}

ISR(INT2_vect) {
	// right button action (PAUSE BUT DIFFERENT?)
	Disable_INT2();       // Temporarily disable for debouncing
}

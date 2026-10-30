#include <avr/io.h>
#include <avr/interrupt.h>
#include "Buttons.h"
#include "Board.h"
#include "Config.h"
#include "DCmotor.h"
#include "Timer.h"

void buttons_init(void){
	BUTTON_DDR = BUTTON_MASK;
	
	EIMSK |= _BV(INT1);                   // Enable INT1
	EICRA |= _BV(ISC11);                  // Falling edge trigger for active low
	
	EIMSK |= _BV(INT2);                   // Enable INT2
	EICRA |= (_BV(ISC21) | _BV(ISC20));   // Rising edge trigger for active high
}

void Enable_INT2(void){
	EIFR |= _BV(INTF2);   // Clear any pending flags
	EIMSK |= _BV(INT2);   // Re-enable INT2
	return;
}

void Disable_INT2(void){
	EIMSK &= 0b11111011;   // Temporarily disable INT2
	return;
}

ISR(INT1_vect) {
	set_kill_switch_flag();
}

ISR(INT2_vect) {
	set_change_direction_flag();
	Disable_INT2();
}

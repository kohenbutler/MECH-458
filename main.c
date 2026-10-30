#include <avr/io.h>
#include <avr/interrupt.h>
#include "Led.h"
#include "Timer.h"
#include "Stepper.h"
#include "pwm.h"
#include "adc.h"
#include "Board.h"
#include "DCmotor.h"

int main(void){
	cli(); // Disable interrupts during configuration
	
	/* Clock Configuration */
	CLKPR = 0x80; // unlock clock
	CLKPR = 0x01; // set CPU clock to 8MHz
	
	/* Interrupt Configuration */
	EIMSK |= _BV(INT2);                   // Enable INT2
	EICRA |= (_BV(ISC21) | _BV(ISC20));   // Rising edge trigger for active high
	
	EIMSK |= _BV(INT1);                   // Enable INT3
	EICRA |= _BV(ISC11);                  // Falling edge trigger for active low
	
	/* Initialize Hardware Modules */
	timer1_init();
	LED_init();
	stepper_init();
	pwm_init();
	adc_init();
	
	sei(); // Enable global interrupts
	
	adc_start_conversion();
	
	/* Tester Functions */
	LED_test();
	DCMotor_F();
	
	while (1){
		RED_LED_PORT = adc_get_result();
	}
	return 0;
}

ISR(INT1_vect) {
	timer_delay_ms(10); // Debounce Delay
	DCMotor_Brake();
	while(1){
		Lab1KnightRider();
	}
}

ISR(INT2_vect) {
	timer_delay_ms(10); // Debounce Delay
	if(DCmotor_get_state() == 'F'){
		DCMotor_Brake();
		timer_delay_ms(5);
		DCMotor_R();
	}
	else{
		DCMotor_Brake();
		timer_delay_ms(5);
		DCMotor_F();
	}
}
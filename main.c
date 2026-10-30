#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "Board.h"
#include "Buttons.h"
#include "Config.h"
#include "DCmotor.h"
#include "Led.h"
#include "LinkedList.h"
#include "pwm.h"
#include "Stepper.h"
#include "Timer.h"

int main(void){
	cli(); // Disable interrupts during configuration
	
	/* Clock Configuration */
	CLKPR = 0x80; // unlock clock
	CLKPR = 0x01; // set CPU clock to 8MHz
	
	/* Initialize Hardware Modules */
	timer1_init();
	LED_init();
	stepper_init();
	pwm_init();
	adc_init();
	buttons_init();
	DCMotor_init();
	
	sei(); // Enable global interrupts
	
	LED_test();
	adc_start_conversion(); // Start the first adc conversion
	DCMotor_F(); // Start the motor in forward
	
	while (1){
		if(adc_is_ready()){
			int adc_value = adc_get_result();
			pwm_set_duty_cycle(adc_value);
			clear_adc_result_flag();
		}
		
		if(is_change_direction()){
			timer_delay_ms(200); // Debouncing Delay
			switch(DCmotor_get_state()){
				case('F'):
					DCMotor_Brake();
					timer_delay_ms(5);
					DCMotor_R();
					break;
				case('R'):
					DCMotor_Brake();
					timer_delay_ms(5);
					DCMotor_F();
					break;
			}
			clear_change_direction_flag();
		}
		
		if(kill_switch_active()){
			while(1){
				Lab1KnightRider();
			}
		}
	}
	return 0;
}
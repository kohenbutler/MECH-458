#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "Board.h"
#include "Buttons.h"
#include "Config.h"
#include "DCmotor.h"
#include "lcd.h"
#include "Led.h"
#include "LinkedList.h"
#include "myutils.h"
#include "pwm.h"
#include "Stepper.h"
#include "Timer.h"

int adc_value = 0;

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
	InitLCD(LS_BLINK|LS_ULINE);
	LCDClear();
	
	sei(); // Enable global interrupts
	
	LED_test();
	adc_start_conversion(); // Start the first adc conversion
	
	while (1){

		if(adc_is_ready()){
			adc_value = adc_get_result();
			pwm_set_duty_cycle(adc_value);
			clear_adc_result_flag();
		}
		
		if(is_change_direction()){
			switch(DCmotor_get_state()){
				case(Forward):
					DCMotor_Brake();
					timer_delay_ms(BRAKING_DELAY);
					DCMotor_R();
					break;
				case(Reverse):
					DCMotor_Brake();
					timer_delay_ms(BRAKING_DELAY);
					DCMotor_F();
					break;
				case(Brake):
					DCMotor_F();
					break;
			}
			timer_delay_ms(DEBOUNCE_DELAY);
			while((BUTTON_INPUT_REGISTER & 0b00000100) == 0b00000100){
			}
			timer_delay_ms(DEBOUNCE_DELAY);
			Enable_INT2();
			clear_change_direction_flag();
		}
		
		if(kill_switch_active()){
			DCMotor_Brake();
			LCDClear();
			LCDWriteStringXY(0,0,"**KILL  SWITCH**");
			while(1){
				Lab1KnightRider();
			}
		}
		
		if(DCmotor_get_state() == Forward){
			LCDClear();
			LCDWriteStringXY(0,0,"Forward");
			LCDWriteIntXY(0,1,adc_value,3);
			LCDWriteIntXY(4,1,adc_value/2.55,3);
			LCDWriteStringXY(7,1,"%");
		}
		
		if(DCmotor_get_state() == Reverse){
			LCDClear();
			LCDWriteStringXY(0,0,"Reverse");
			LCDWriteIntXY(0,1,adc_value,3);
			LCDWriteIntXY(4,1,adc_value/2.55,3);
			LCDWriteStringXY(7,1,"%");
		}
		
		if(DCmotor_get_state() == Brake){
			LCDClear();
			LCDWriteStringXY(0,0,"Press to Start");
		}
		
		timer_delay_ms(10);
	}
	return 0;
}
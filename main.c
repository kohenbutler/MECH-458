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
	
	stepper_home();
	timer_delay_ms(2000);
	s_curve_demo();
	pwm_set_duty_cycle(128);
	LCDClear();
	LCDWriteStringXY(0,0,"PRESS TO START");
	
	while (1){
		
		if(was_pause_button_pressed()){
			switch(is_paused()){
				case(0):
					Pause();
					DCMotor_Brake();
					LCDClear();
					LCDWriteStringXY(0,0,"PAUSED");
					break;
				case(1):
					Unpause();
					DCMotor_F();
					LCDClear();
					LCDWriteStringXY(0,0,"FORWARD");
					break;
			}
			timer_delay_ms(DEBOUNCE_DELAY);
			while((BUTTON_INPUT_REGISTER & 0b00000010) == 0b00000000){}
			timer_delay_ms(DEBOUNCE_DELAY);
			Enable_INT1();
			clear_pause_button_pressed();
		}
		
		timer_delay_ms(20);
	}
	return 0;
}
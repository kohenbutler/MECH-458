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

int main(){
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
	
	stepper_home();                             // Initialize bin on black
	pwm_set_duty_cycle(PWM_DEFAULT_DUTY_CYCLE); // Run the motor at 50% for now
	fifo_append(BLACK);
	fifo_append(WHITE);
	fifo_append(ALUMINIUM);
	fifo_append(STEEL);
	
	/* Main Loop */
	while (1){
		
		/* Update Stepper Position*/
		if(get_bin_state() != get_next_block_type()){            // If bin state does not match next block
			Update_Stepper_Position(get_next_block_type());      // Move the bin to the correct state
		}
		
		/* If Block at end of belt*/
		if(is_block_at_end_flag()){
			if(get_bin_state() != get_next_block_type()){        // Check bin position
				DCMotor_Brake();                                 // If wrong, Stop belt
				Update_Stepper_Position(get_next_block_type());  //           Change position
				DCMotor_F();                                     //           Start belt
			}
			fifo_pop_front();                                    // Remove from fifo
			clear_block_at_end_flag();                           // Clear flag
		}
		
		/* Left Button Handler */
		if(was_left_button_pressed()){
			// Do Something
			timer_delay_ms(DEBOUNCE_DELAY);
			while((BUTTON_INPUT_REGISTER & 0b00000010) == 0b00000000){} // Stall if button is held (active low)
			timer_delay_ms(DEBOUNCE_DELAY);
			Enable_INT1();
			clear_left_button_pressed();
		}
		
		/* Right Button Handler */
		if(was_right_button_pressed()){
			// Do Something
			timer_delay_ms(DEBOUNCE_DELAY);
			while((BUTTON_INPUT_REGISTER & 0b00000100) == 0b00000100){} // Stall if button is held (active high)
			timer_delay_ms(DEBOUNCE_DELAY);
			Enable_INT2();
			clear_right_button_pressed();
		}
	}
	return 0;
}
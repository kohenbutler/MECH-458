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
	timer3_init();
	timer4_init();
	LED_init();
	stepper_init();
	pwm_init();
	adc_init();
	buttons_init();
	DCMotor_init();
	InitLCD(LS_BLINK|LS_ULINE);
	LCDClear();
	
	stepper_home(); // Initialize bin on black
	
	sei();          // Enable global interrupts
	
	LCDWriteStringXY(3, 0, "PRESS LEFT");  // Display start message
	LCDWriteStringXY(4, 1, "TO START");
	while(!was_left_button_pressed()){}    // wait until start button is pressed
	clear_left_button_pressed();           // Clear the button pressed flag
	set_left_button_wait_release();        // set the waiting for release flag (for release bouncing)
	LCDClear();
	LCDWriteStringXY(4, 0, "SORTING");     // Display sorting message
	DCMotor_F();                           // Start the conveyor belt
	unsigned int system_state = RUN;       // Set the system state to running
	
	/* Main Loop */
	while (1){
		if(system_state){
			/* If Block at Reflective Sensor */
			if((OR_INPUT_REGISTER & OR_MASK) == OR_MASK){                                   // Check for block present
				if(!is_OR_flag() && !(adc_in_progress()) && !(adc_conversion_complete())){  // If previous block classified, ADC not converting, and previous conversion was processed
					adc_start_conversion();                                                 // Then start another ADC conversion
				}
			}
			
			/* If Block has Left Sensor */
			if(is_OR_flag() && !adc_in_progress() && !adc_conversion_complete()){ // If block has left sensor, adc is not still converting, and last conversion was processed
				unsigned int min = adc_get_min();                                 // Then we can classify the block
				if(min < 1024){                                        // If min is 1024 this is a false reading and we should skip it
					int block_type = classify_block(min);              // classify the block based on it's minimum adc reading
					fifo_append(block_type);                           // add this block to the fifo
					LCDClear();
					LCDWriteIntXY(0, 0, min, 4);
					LCDWriteIntXY(7, 0, adc_get_num_readings(), 4);
					switch(block_type){
						case(BLACK):
						LCDWriteStringXY(0,1,"BLACK");
						break;
						case(ALUMINIUM):
						LCDWriteStringXY(0,1,"ALUMINIUM");
						break;
						case(WHITE):
						LCDWriteStringXY(0,1,"WHITE");
						break;
						case(STEEL):
						LCDWriteStringXY(0,1,"STEEL");
						break;
					}
				}
				clear_OR_flag();                                    // clear the block gone flag
				adc_reset_min();                                    // reset adc min to 1024
			}
			
			/* If Block at end of belt */
			if(is_block_at_end_flag()){
				Update_Stepper_Position(get_next_block_type());      // Update Stepper
				fifo_pop_front();                                    // Remove from fifo
				clear_block_at_end_flag();                           // Clear flag
				DCMotor_F();                                         // Restart the belt
			}
			
			/* If ADC Conversion Complete */
			if(adc_conversion_complete()){              // If ADC result is waiting to be processed
				adc_update_min(adc_get_result());       // Update the minimum adc reading for this block
				clear_adc_result_flag();                // clear the adc conversion complete flag
			}
		}
		
		// If Ramp Down Flag is set and no pieces left on belt
		if(is_ramp_down_flag() && get_num_partially_sorted() == 0){
			LCDClear();
			LCDWriteStringXY(1, 0, "BL AL WH ST PS");                // Write the number of pieces sorted
			LCDWriteIntXY(1, 1, get_num_sorted(BLACK), 2);
			LCDWriteIntXY(4, 1, get_num_sorted(ALUMINIUM), 2);
			LCDWriteIntXY(7, 1, get_num_sorted(WHITE), 2);
			LCDWriteIntXY(10, 1, get_num_sorted(STEEL), 2);
			LCDWriteIntXY(13, 1, get_num_partially_sorted(), 2);
			DCMotor_Brake();                                         // Stop the belt
			while(1){
				Lab1KnightRider();                                   // Freeze the system in knight rider
			}
		}
		
		/* Left Button Handler */
		if(was_left_button_pressed()){                               // If left button pressed
			if(system_state == PAUSE){                               // Toggle system state
				system_state = RUN;
				LCDClear();
				LCDWriteStringXY(4, 0, "SORTING");
				DCMotor_F();
			}
			else{
				system_state = PAUSE;
				LCDClear();                                                 // Clear the screen
				LCDWriteStringXY(1, 0, "BL AL WH ST PS");                   // Write the number of pieces sorted
				LCDWriteIntXY(1, 1, get_num_sorted(BLACK), 2);
				LCDWriteIntXY(4, 1, get_num_sorted(ALUMINIUM), 2);
				LCDWriteIntXY(7, 1, get_num_sorted(WHITE), 2);
				LCDWriteIntXY(10, 1, get_num_sorted(STEEL), 2);
				LCDWriteIntXY(13, 1, get_num_partially_sorted(), 2);
				DCMotor_Brake();
			}
			clear_left_button_pressed();                             // Clear the button pressed flag
			set_left_button_wait_release();                          // set the waiting for release flag (for release bouncing)
		}
		
		if(get_left_button_wait_release()){                          // If waiting for release
			if((BUTTON_INPUT_REGISTER & 0b00000010) == 0b00000010){  // Check if released (active low)
				timer_delay_ms(DEBOUNCE_DELAY);                      // Delay for debouncing
				Enable_INT1();                                       // Re-enable INT1
				clear_left_button_wait_release();                    // Clear the release flag
			}
		}
		
		/* Right Button Handler */
		if(was_right_button_pressed()){                              // If right button pressed
			LCDClear();                                              // Clear the screen
			LCDWriteStringXY(2, 0, "RAMPING DOWN");                  // Write Ramping Down
			timer4_start();                                          // Start the ramp down counter
			clear_right_button_pressed();                            // Clear the button pressed flag
			set_right_button_wait_release();                         // set the waiting for release flag (for release debouncing)
		}
		
		if(get_right_button_wait_release()){                         // If waiting for release
			if((BUTTON_INPUT_REGISTER & 0b00000100) == 0b00000000){  // Check if released (active high)
				timer_delay_ms(DEBOUNCE_DELAY);                      // Delay for debouncing
				Enable_INT2();                                       // Re-enable INT2
				clear_right_button_wait_release();                   // Clear the release flag
			}
		}
	}
	return 0;
}
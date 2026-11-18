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
	
	/* Initialize global variables */
	unsigned int min = 1024;
	unsigned int system_state;
	unsigned long int ramp_down_counter = 0;
	
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
	system_state = RUN;
	
	/* Main Loop */
	while (1){
		if(system_state){
			/* If Block at Reflective sensor */
			if(is_OR_flag()){
				if((OR_INPUT_REGISTER & OR_MASK) != OR_MASK){ // If block is gone
					int block_type = classify_block(min);     // classify the block based on it's minimum adc reading
					fifo_append(block_type);                  // add this block to the fifo
					LCDClear();
					LCDWriteIntXY(0,0,min,4);
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
					clear_OR_flag();                          // clear the block present flag
					min = 1024;                               // reset adc min to 1024
				}
				else{                                         // If block is still there
					adc_start_conversion();                   // Start another ADC conversion
				}
			}
		
			/* If ADC Conversion Complete */
			if(adc_is_ready()){
				unsigned int shiny = adc_get_result();  // Record the adc value
				if(shiny < min){                        // Compare to the minimum adc reading of the current block
					min = shiny;                        // If less, replace min with the adc reading
				}
				clear_adc_result_flag();                // clear the adc conversion complete flag
			}
		
			/* If Block at end of belt */
			if(is_block_at_end_flag()){
				if(get_bin_state() != get_next_block_type()){        // Check bin position
					Update_Stepper_Position(get_next_block_type());  // If wrong, Change position
				}
				DCMotor_F();                                         // Restart the belt
				fifo_pop_front();                                    // Remove from fifo
				clear_block_at_end_flag();                           // Clear flag
			}
		}
		
		if(system_state == RAMP_DOWN){
			if(get_num_partially_sorted() == 0 && ramp_down_counter > RAMP_DOWN_THRESHOLD){
				LCDClear();
				LCDWriteStringXY(1, 0, "BL AL WH ST PS");                   // Write the number of pieces sorted
				LCDWriteIntXY(1, 1, get_num_sorted(BLACK), 2);
				LCDWriteIntXY(4, 1, get_num_sorted(ALUMINIUM), 2);
				LCDWriteIntXY(7, 1, get_num_sorted(WHITE), 2);
				LCDWriteIntXY(10, 1, get_num_sorted(STEEL), 2);
				LCDWriteIntXY(13, 1, get_num_partially_sorted(), 2);
				DCMotor_Brake();
				while(1){
					Lab1KnightRider();
				}
			}
			ramp_down_counter += 1;
		}
		
		/* Left Button Handler */
		if(was_left_button_pressed()){                               // If left button pressed
			if(system_state == PAUSE){                               // Toggle system state
				system_state = RUN;
				LCDClear();
				LCDWriteStringXY(4, 0, "SORTING");
				DCMotor_F();
			}
			else if(system_state == RUN){
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
			system_state = RAMP_DOWN;
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
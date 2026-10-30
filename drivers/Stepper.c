#include "Stepper.h"
#include "Config.h"
#include "Timer.h"
#include "Board.h"

char step_states[4] = {0b00110000, 0b00000110, 0b00101000, 0b00000101};
int stepper_state = 0; // Tracks the last step

void stepper_init(){
	STEPPER_DDR |= STEPPER_MASK;  // Set stepper IO to outputs
	STEPPER_PORT &= ~(STEPPER_MASK); // Initialize outputs to 0
}

void step_cw(unsigned int steps){
	while(steps > 0){
		stepper_state = (stepper_state + 1) % 4; // Move 1 forward in the step chart
		STEPPER_PORT = step_states[stepper_state]; // Fire the step
		timer_delay_ms(STEPPER_DELAY_MS); // Speed Control
		steps -= 1;
	}
	return;
}

void step_ccw(unsigned int steps){
	while(steps > 0){
		stepper_state = (stepper_state + 3) % 4; // Move 1 backward in the step chart
		STEPPER_PORT = step_states[stepper_state]; // Fire the step
		timer_delay_ms(STEPPER_DELAY_MS); // Speed Control
		steps -= 1;
	}
	return;
}

void stepper_home(){
	step_cw(90);
	timer_delay_ms(STEPPER_DEMO_DELAY_MS);
	return;
}

void stepper_demo(){
	step_cw(17);   // CW 30°
	timer_delay_ms(STEPPER_DEMO_DELAY_MS);
	step_cw(33);   // CW 60°
	timer_delay_ms(STEPPER_DEMO_DELAY_MS);
	step_cw(100);  // CW 180°
	timer_delay_ms(STEPPER_DEMO_DELAY_MS);
	step_ccw(17);  // CCW 30°
	timer_delay_ms(STEPPER_DEMO_DELAY_MS);
	step_ccw(33);  // CCW 60°
	timer_delay_ms(STEPPER_DEMO_DELAY_MS);
	step_ccw(100); // CCW 180°
	timer_delay_ms(STEPPER_DEMO_DELAY_MS);
	return;
}

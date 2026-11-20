#include "Stepper.h"
#include "Config.h"
#include "Timer.h"
#include "Board.h"

volatile char step_states[4] = {0b00110000, 0b00000110, 0b00101000, 0b00000101};
volatile int stepper_state = 0; // Tracks the last step
volatile int bin_state;

int stepper_delays_90[] = {
	20000, 19766, 19100, 18094, 16870, 15546, 14219, 12955, 11791, 10745,
	9821, 9013, 8313, 7710, 7191, 6749, 6372, 6054, 5787, 5566,
	5386, 5243, 5135, 5060, 5015, 5015, 5060, 5135, 5243, 5386,
	5566, 5787, 6054, 6372, 6749, 7191, 7710, 8313, 9013, 9821,
	10745, 11791, 12955, 14219, 15546, 16870, 18094, 19100, 19766, 20000
};

int stepper_delays_180[] = {
	20000, 19766, 19100, 18094, 16870, 15546, 14219, 12955, 11791, 10745,
	9821, 9013, 8313, 7710, 7191, 6749, 6372, 6054, 5787, 5566,
	5386, 5243, 5135, 5060, 5015, 5000, 5000, 5000, 5000, 5000,
	5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
	5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
	5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
	5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
	5000, 5000, 5000, 5000, 5000, 5015, 5060, 5135, 5243, 5386,
	5566, 5787, 6054, 6372, 6749, 7191, 7710, 8313, 9013, 9821,
	10745, 11791, 12955, 14219, 15546, 16870, 18094, 19100, 19766, 20000
};

void stepper_init(){
	STEPPER_DDR |= STEPPER_MASK;  // Set stepper IO to outputs
	STEPPER_PORT &= ~(STEPPER_MASK); // Initialize outputs to 0
	STEPPER_HOME_PORT &= ~(STEPPER_HOME_MASK); // Set Stepper home to input
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
	step_cw(50);
	while((STEPPER_HOME_INPUT & STEPPER_HOME_MASK) == STEPPER_HOME_MASK){
		step_cw(1);
	}
	bin_state = BLACK;
	return;
}

void step_cw90(){
	for(int i = 0; i < 50; i += 1){
		stepper_state = (stepper_state + 1) % 4; // Move 1 forward in the step chart
		STEPPER_PORT = step_states[stepper_state]; // Fire the step
		timer_delay_us(stepper_delays_90[i]); // Speed Control
	}
	bin_state = (bin_state + 1) % 4;
	return;
}

void step_ccw90(){
	for(int i = 0; i < 50; i += 1){
		stepper_state = (stepper_state + 3) % 4; // Move 1 backward in the step chart
		STEPPER_PORT = step_states[stepper_state]; // Fire the step
		timer_delay_us(stepper_delays_90[i]); // Speed Control
	}
	bin_state = (bin_state + 3) % 4;
	return;
}

void step_180(){
	for(int i = 0; i < 100; i += 1){
		stepper_state = (stepper_state + 1) % 4; // Move 1 forward in the step chart
		STEPPER_PORT = step_states[stepper_state]; // Fire the step
		timer_delay_us(stepper_delays_180[i]); // Speed Control
	}
	bin_state = (bin_state + 2) % 4;
	return;
}

void s_curve_test(){
	step_180();
	step_ccw90();
	step_cw90();
	step_ccw90();
	step_180();
	step_ccw90();
	step_180();
}

int get_bin_state(){
	return bin_state;
}

void Update_Stepper_Position(int pos){
	if(pos < 0){
		return;
	}
	int i = (pos - bin_state + 4) % 4;
	switch(i){
		case 0:
			break;
		case 1:
			step_cw90();
			break;
		case 2:
			step_180();
			break;
		case 3:
			step_ccw90();
			break;
	}
}
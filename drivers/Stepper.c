#include "Stepper.h"
#include "Config.h"
#include "Timer.h"
#include "Board.h"

volatile char step_states[4] = {0b00110000, 0b00000110, 0b00101000, 0b00000101};
volatile int stepper_state = 0; // Tracks the last step
volatile int bin_state;

int stepper_delays_90[] = {
	20000, 19365, 17704, 15546, 13366, 11429, 9821, 8535, 7528, 6749,
	6154, 5708, 5386, 5168, 5041, 5000, 5000, 5000, 5000, 5000,
	5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000,
	5000, 5000, 5000, 5000, 5000, 5041, 5168, 5386, 5708, 6154,
	6749, 7528, 8535, 9821, 11429, 13366, 15546, 17704, 19365, 20000
};

int stepper_delays_180[] = {
	20000, 19613, 18544, 17013, 15276, 13537, 11922, 10491, 9257, 8212,
	7333, 6597, 5983, 5469, 5040, 4681, 4382, 4133, 3927, 3758,
	3622, 3514, 3434, 3378, 3344, 3333, 3333, 3333, 3333, 3333,
	3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333,
	3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333,
	3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333,
	3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333, 3333,
	3333, 3333, 3333, 3333, 3333, 3344, 3378, 3434, 3514, 3622,
	3758, 3927, 4133, 4382, 4681, 5040, 5469, 5983, 6597, 7333,
	8212, 9257, 10491, 11922, 13537, 15276, 17013, 18544, 19613, 20000
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
	}
}
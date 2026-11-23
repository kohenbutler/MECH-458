#include "Stepper.h"
#include "Config.h"
#include "Timer.h"
#include "Board.h"

volatile char step_states[4] = {
	0b00110110,  // Step 0: E1=1, L1=1, L2=0 | E2=1, L3=1, L4=0
	0b00101110,  // Step 1: E1=1, L1=0, L2=1 | E2=1, L3=1, L4=0
	0b00101101,  // Step 2: E1=1, L1=0, L2=1 | E2=1, L3=0, L4=1
	0b00110101   // Step 3: E1=1, L1=1, L2=0 | E2=1, L3=0, L4=1
};
volatile int stepper_state = 0; // Tracks the last step
volatile int bin_state;

int stepper_delays_90[] = {
	20000, 19113, 16903, 14249, 11789, 9769, 8202, 7018, 6132, 5475,
	4996, 4655, 4428, 4298, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4810, 7018, 12971, 20000
};

int stepper_delays_180[] = {
	20000, 19113, 16903, 14249, 11789, 9769, 8202, 7018, 6132, 5475,
	4996, 4655, 4428, 4298, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255, 4255,
	4255, 4255, 4255, 4255, 4255, 4255, 4810, 7018, 12971, 20000
};

void stepper_init(){
	STEPPER_DDR |= STEPPER_MASK;  // Set stepper IO to outputs
	STEPPER_PORT &= ~(STEPPER_MASK); // Initialize outputs to 0
	STEPPER_HOME_PORT &= ~(STEPPER_HOME_MASK); // Set Stepper home to input
	return;
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
	return;
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
	return;
}
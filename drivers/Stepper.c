#include "Stepper.h"
#include "Config.h"
#include "Timer.h"
#include "Board.h"

volatile char step_states[4] = {0b00110000, 0b00000110, 0b00101000, 0b00000101};
volatile int stepper_state = 0; // Tracks the last step
volatile int bin_state;

int stepper_delays_90[] = {
	20000, 19818, 19293, 18486, 17478, 16356, 15195, 14055, 12974, 11977,
	11074, 10267,  9554,  8929,  8385,  7914,  7509,  7164,  6872,  6629,
	6430,  6272,  6151,  6067,  6017,  6000,  6017,  6067,  6151,  6272,
	6430,  6629,  6872,  7164,  7509,  7914,  8385,  8929,  9554, 10267,
	11074, 11977, 12974, 14055, 15195, 16356, 17478, 18486, 19293, 19818,
	20000
};
	
int stepper_delays_180[] = {
	20000, 19818, 19293, 18486, 17478, 16356, 15195, 14055, 12974, 11977,
	11074, 10267,  9554,  8929,  8385,  7914,  7509,  7164,  6872,  6629,
	6430,  6272,  6151,  6067,  6017,  6000,  6000,  6000,  6000,  6000,
	6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,
	6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,
	6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,
	6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,  6000,
	6000,  6000,  6000,  6000,  6000,  6000,  6017,  6067,  6151,  6272,
	6430,  6629,  6872,  7164,  7509,  7914,  8385,  8929,  9554, 10267,
	11074, 11977, 12974, 14055, 15195, 16356, 17478, 18486, 19293, 19818,
	20000
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
	timer_delay_us(stepper_delays_90[50]);
	bin_state = (bin_state + 1) % 4;
	return;
}

void step_ccw90(){
	for(int i = 0; i < 50; i += 1){
		stepper_state = (stepper_state + 3) % 4; // Move 1 backward in the step chart
		STEPPER_PORT = step_states[stepper_state]; // Fire the step
		timer_delay_us(stepper_delays_90[i]); // Speed Control
	}
	timer_delay_us(stepper_delays_90[50]);
	bin_state = (bin_state + 3) % 4;
	return;
}

void step_180(){
	for(int i = 0; i < 100; i += 1){
		stepper_state = (stepper_state + 1) % 4; // Move 1 forward in the step chart
		STEPPER_PORT = step_states[stepper_state]; // Fire the step
		timer_delay_us(stepper_delays_180[i]); // Speed Control
	}
	timer_delay_us(stepper_delays_180[100]);
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
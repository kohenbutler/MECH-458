#include "Stepper.h"
#include "Config.h"
#include "Timer.h"
#include "Board.h"

volatile char step_states[4] = {0b00110000, 0b00000110, 0b00101000, 0b00000101};
volatile int stepper_state = 0; // Tracks the last step
volatile int bin_state;

int stepper_delays_90[] = {
	16001, 15826, 15326, 14568, 13638, 12624, 11598, 10612,  9697,  8868,
	8131,  7483,  6919,  6430,  6008,  5647,  5339,  5078,  4858,  4676,
	4528,  4410,  4321,  4259,  4222,  4209,  4222,  4259,  4321,  4410,
	4528,  4676,  4858,  5078,  5339,  5647,  6008,  6430,  6919,  7483,
	8131,  8868,  9697, 10612, 11598, 12624, 13638, 14568, 15326, 15826,
	16001
};
	
int stepper_delays_180[] = {
	16001, 15826, 15326, 14568, 13638, 12624, 11598, 10612,  9697,  8868,
	8131,  7483,  6919,  6430,  6008,  5647,  5339,  5078,  4858,  4676,
	4528,  4410,  4321,  4259,  4222,  4209,  4209,  4209,  4209,  4209,
	4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,
	4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,
	4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,
	4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,  4209,
	4209,  4209,  4209,  4209,  4209,  4209,  4222,  4259,  4321,  4410,
	4528,  4676,  4858,  5078,  5339,  5647,  6008,  6430,  6919,  7483,
	8131,  8868,  9697, 10612, 11598, 12624, 13638, 14568, 15326, 15826,
	16001
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
	step_cw(4);
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
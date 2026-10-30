#include <avr/io.h>
#include "Config.h"
#include "Board.h"
#include "Timer.h"

char DCMotor_states[3] = {'B', 'F', 'R'};
int DCMotor_state = 0;

void DCMotor_init(){
	DC_MOTOR_DDR |= DC_MOTOR_MASK; // Set IA and IB to outputs
	DC_MOTOR_PORT |= DC_MOTOR_MASK; // Initialize in Brake mode
	return;
}

void DCMotor_R(){
	DC_MOTOR_PORT = (DC_MOTOR_PORT & ~(DC_MOTOR_MASK)) | 0b00000001;
	DCMotor_state = 2;
	return;
}

void DCMotor_F(){
	DC_MOTOR_PORT = (DC_MOTOR_PORT & ~(DC_MOTOR_MASK)) | 0b00000010;
	DCMotor_state = 1;
	return;
}

void DCMotor_Brake(){
	DC_MOTOR_PORT = (DC_MOTOR_PORT & ~(DC_MOTOR_MASK)) | 0b00000011;
	DCMotor_state = 0;
	return;
}

void DCMotor_Test(){
	DCMotor_R();
	timer_delay_ms(10000);
	DCMotor_Brake();
	timer_delay_ms(1000);
	DCMotor_F();
	timer_delay_ms(10000);
	DCMotor_Brake();
	timer_delay_ms(1000);
	return;
}

char DCmotor_get_state(){
	return DCMotor_states[DCMotor_state];
}
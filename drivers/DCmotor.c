#include <avr/io.h>
#include "Config.h"
#include "Board.h"
#include "Timer.h"
#include "DCMotor.h"

int DCMotor_state = BRAKE;
int Paused = 1;

void DCMotor_init(){
	DC_MOTOR_DDR |= DC_MOTOR_MASK; // Set IA and IB to outputs
	DC_MOTOR_PORT |= DC_MOTOR_MASK; // Initialize in Brake mode
	return;
}

void DCMotor_Brake(){
	DC_MOTOR_PORT = (DC_MOTOR_PORT & ~(DC_MOTOR_MASK)) | 0b00000011;
	DCMotor_state = BRAKE;
	return;
}

void DCMotor_F(){
	DC_MOTOR_PORT = (DC_MOTOR_PORT & ~(DC_MOTOR_MASK)) | 0b00000010;
	DCMotor_state = FORWARD;
	return;
}

void DCMotor_R(){
	DC_MOTOR_PORT = (DC_MOTOR_PORT & ~(DC_MOTOR_MASK)) | 0b00000001;
	DCMotor_state = REVERSE;
	return;
}

char DCmotor_get_state(){
	return DCMotor_state;
}

void Pause(){
	Paused = 1;
	return;
}

void Unpause(){
	Paused = 0;
	return;
}

int is_paused(){
	return Paused;
}
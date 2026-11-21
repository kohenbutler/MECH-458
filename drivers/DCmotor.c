#include <avr/io.h>
#include <avr/interrupt.h>
#include "Config.h"
#include "Board.h"
#include "Timer.h"
#include "DCMotor.h"

volatile int DCMotor_state = BRAKE;
volatile int block_at_end_flag = 0;

void DCMotor_init(){
	DC_MOTOR_DDR |= DC_MOTOR_MASK;        // Set IA and IB to outputs
	DC_MOTOR_PORT |= DC_MOTOR_MASK;       // Initialize in Brake mode
	
	BELT_END_DDR &= ~(BELT_END_MASK);     

	EIMSK |= _BV(INT0);                   // Enable INT0
	EICRA |= _BV(ISC01);                  // Falling edge trigger for active low
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

void set_block_at_end_flag(){
	block_at_end_flag = 1;
	return;
}

void clear_block_at_end_flag(){
	block_at_end_flag = 0;
	return;
}

int is_block_at_end_flag(){
	return block_at_end_flag;
}

ISR(INT0_vect) {
	DCMotor_Brake();        // BRAKE IMMEDIATELY to prevent block from falling
	timer3_start();         // Start a 100ms timer to allow belt to come to a complete stop
}
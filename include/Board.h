#ifndef BOARD_H
#define BOARD_H

#include <avr/io.h>

// Yellow/green LED hardware
#define STATUS_LED_DDR   DDRL
#define STATUS_LED_PORT  PORTL
#define STATUS_LED_MASK  0b11110000

// Red LED hardware
#define RED_LED_DDR      DDRC
#define RED_LED_PORT     PORTC
#define RED_LED_MASK     0b11111111

// Stepper Motor hardware
#define STEPPER_DDR	     DDRA
#define STEPPER_PORT     PORTA
#define STEPPER_MASK     0b00111111

// PWM hardware
#define PWM_DDR          DDRB
#define PWM_PORT         PORTB
#define PWM_MASK         0b10000000

// ADC hardware
#define ADC_DDR          DDRF
#define ADC_PORT         PORTF
#define ADC_MASK         0b111111110

// DC Motor hardware
#define DC_MOTOR_DDR     DDRB
#define DC_MOTOR_PORT    PORTB
#define DC_MOTOR_MASK    0b00000011

// Button Hardware
#define BUTTON_DDR       DDRD
#define BUTTON_PORT      PORTD
#define BUTTON_MASK      0b00000000
#define BUTTON_INPUT_REGISTER PIND

#endif
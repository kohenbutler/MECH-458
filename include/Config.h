#ifndef CONFIG_H
#define CONFIG_H

// Timing Delays (milliseconds)
#define LED_KNIGHT_RIDER_DELAY_MS  150
#define LED_TEST_DELAY_MS         2000
#define STEPPER_DELAY_MS	        20
#define STEPPER_DEMO_DELAY_MS     1000

// Default Values
#define PWM_DEFAULT_DUTY_CYCLE     128
#define DEBOUNCE_DELAY              20
#define BRAKING_DELAY                5

// Conveyor States
#define BRAKE 0
#define FORWARD 1
#define REVERSE 2

// Bin States and Block Types
#define BLACK 0
#define ALUMINIUM 1
#define WHITE 2
#define STEEL 3

#endif
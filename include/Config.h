#ifndef CONFIG_H
#define CONFIG_H

// Timing Delays (milliseconds)
#define LED_KNIGHT_RIDER_DELAY_MS  150
#define LED_TEST_DELAY_MS         2000
#define STEPPER_DELAY_MS	        20
#define STEPPER_DEMO_DELAY_MS     1000
#define RAMP_DOWN_TIME_MS         2000
#define BRAKING_TIME_MS            100

// Default Values
#define DC_MOTOR_SPEED             100 // [0,255]
#define DEBOUNCE_DELAY              20

// Conveyor States
#define BRAKE 0
#define FORWARD 1
#define REVERSE 2

// Bin States and Block Types
#define BLACK 0
#define ALUMINIUM 1
#define WHITE 2
#define STEEL 3

// Shininess Thresholds
#define AS_THRESHOLD 450 // Just below min steel
#define SW_THRESHOLD 850 // Just below min white
#define WB_THRESHOLD 920 // Just below min black

// System States
#define PAUSE 0
#define RUN   1

#endif
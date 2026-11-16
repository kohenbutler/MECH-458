#include "Led.h"
#include "Board.h"
#include "Config.h"
#include "Timer.h"

void LED_init(){
	STATUS_LED_DDR |= STATUS_LED_MASK; // Configure yellow/green leds as output
	STATUS_LED_PORT &= ~(STATUS_LED_MASK); // Turn off yellow/green leds

	RED_LED_DDR |= RED_LED_MASK; // Configure red leds as output
	RED_LED_PORT &= ~(RED_LED_MASK); // Turn off red leds
	return;
}

void LED_test(){
	// Turn on the LEDs.
	STATUS_LED_PORT |= STATUS_LED_MASK; // Turn on yellow/green leds
	RED_LED_PORT |= RED_LED_MASK; // Turn on red leds
	timer_delay_ms(LED_TEST_DELAY_MS);
	STATUS_LED_PORT &= ~(STATUS_LED_MASK); // Turn off yellow/green leds
	RED_LED_PORT &= ~(RED_LED_MASK); // Turn off red leds
	timer_delay_ms(LED_TEST_DELAY_MS);
	return;
}

void Lab1KnightRider(){
	RED_LED_PORT = (RED_LED_MASK & 0b11000000);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b11100000);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b11110000);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b01111000);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00111100);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00011110);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00001111);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00000111);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00000011);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00000111);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00001111);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00011110);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b00111100);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b01111000);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b11110000);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	RED_LED_PORT = (RED_LED_MASK & 0b11100000);
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS);
	return;
}

void Lab2KnightRider(){
	RED_LED_PORT = (RED_LED_MASK & 0b11000000); // Initialize first 2 leds on
	timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS); // Delay 200ms
	for(int i = 0; i < 6; i += 1){
		RED_LED_PORT = RED_LED_PORT >> 1; // Shift on leds to the right by 1 bit and repeat 6 times
		timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS); // Delay 200ms each shift
	}
	for(int i = 0; i < 5; i += 1){
		RED_LED_PORT = RED_LED_PORT << 1; // Shift on leds to the left by 1 bit and repeat 5 times
		// 5 shifts instead of 6 because the final shift is completed
		// the next time knight_rider() is initialized
		timer_delay_ms(LED_KNIGHT_RIDER_DELAY_MS); // Delay 200ms each shift
	}
	return;
}
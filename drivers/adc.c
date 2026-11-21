#include "adc.h"
#include "Board.h"
#include "Config.h"

volatile unsigned int ADC_result = 0;
volatile unsigned int ADC_result_flag = 0;
volatile int OR_flag = 0;
volatile unsigned int min = 1024;
volatile unsigned int num_readings = 0;

void adc_init() {
	OR_DDR &= ~(OR_MASK);                 // Set OR as an input
	EIMSK |= _BV(INT3);                   // Enable INT3
	EICRA |= _BV(ISC31);                  // Falling edge trigger to detect when block leaves
	
	ADC_DDR &= ~(ADC_MASK);  // Set DDRF to inputs
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1); // Enable ADC, Enable ADC interrupt, and set Prescaler to 64
	ADMUX = _BV(REFS0); // Right adjust result and set AREF to AVCC
	DIDR0 |= _BV(ADC0D); // Disable digital input buffer
}

void set_OR_flag(){
	OR_flag = 1;
	return;
}

void clear_OR_flag(){
	OR_flag = 0;
	return;
}

int is_OR_flag(){
	return OR_flag;
}

void Enable_INT3(){
	EIFR |= _BV(INTF3);   // Clear pending flags
	EIMSK |= _BV(INT3);   // Enable INT3
	return;
}

void Disable_INT3(){
	EIMSK &= 0b11110111;  // Disable INT3
	return;
}

ISR(INT3_vect) {
	set_OR_flag();
}

void adc_start_conversion(){
	ADCSRA |= _BV(ADSC);
}

unsigned char adc_conversion_complete(){
	return ADC_result_flag;
}

unsigned int adc_get_result(){
	return ADC_result;
}

void clear_adc_result_flag(){
	ADC_result_flag = 0;
}

int classify_block(int val){
	if(val < AS_THRESHOLD){
		return ALUMINIUM;
	}
	if(val < SW_THRESHOLD){
		return STEEL;
	}
	if(val < WB_THRESHOLD){
		return WHITE;
	}
	return BLACK;
}

void adc_update_min(int val){
	if(val < min){
		min = val;
	}
	num_readings += 1;
	return;
}

void adc_reset_min(){
	min = 1024;
	num_readings = 0;
	return;
}

unsigned int adc_get_min(){
	return min;
}

unsigned int adc_get_num_readings(){
	return num_readings;
}

int adc_in_progress(){
	return ((ADCSRA & _BV(ADSC)) == _BV(ADSC));
}

ISR(ADC_vect) {
	ADC_result = ADC;
	ADC_result_flag = 1;
}
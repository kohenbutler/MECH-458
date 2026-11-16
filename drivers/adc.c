#include "adc.h"
#include "Board.h"

unsigned int ADC_result = 0;
unsigned int ADC_result_flag = 0;

void adc_init() {
	ADC_DDR &= ADC_MASK;  // Set DDRF to inputs
	ADCSRA = _BV(ADEN) | _BV(ADIE); // Enable ADC, Enable ADC interrupt
	ADCSRB &= 0b11111000; // Set to Free Running Mode
	ADMUX = _BV(ADLAR) | _BV(REFS0); // Left adjust result and set AREF to 5V
	DIDR0 |= _BV(ADC0D); // Disable digital input buffer
}

void adc_start_conversion(char channel){
	ADMUX = (ADMUX & 0b11100000) & channel;
	ADCSRA |= _BV(ADSC);
}

unsigned char adc_is_ready(){
	return ADC_result_flag;
}

unsigned char adc_get_result(){
	return ADC_result;
}

void clear_adc_result_flag(){
	ADC_result_flag = 0;
}

ISR(ADC_vect) {
	ADC_result = ADCH;
	ADC_result_flag = 1;
}
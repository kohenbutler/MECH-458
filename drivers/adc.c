#include "adc.h"
#include "Board.h"
#include "pwm.h"

static unsigned char ADC_result = 0;
static unsigned int ADC_result_flag = 0;

void adc_init(void) {
	ADC_DDR &= ADC_MASK;  // Set DDRF to inputs
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADATE) | _BV(ADPS2) | _BV(ADPS1); // Enable ADC, Enable ADC interrupt, Enable Auto Trigger, set prescaler /64
	ADCSRB &= 0b11111000; // Set to Free Running Mode
	ADMUX = _BV(ADLAR) | _BV(REFS0); // Left adjust result and set AREF to 5V
	DIDR0 |= _BV(ADC0D); // Disable digital input buffer
}

void adc_start_conversion(void) {
	ADCSRA |= _BV(ADSC);
}

unsigned char adc_is_ready(void){
	return ADC_result_flag;
}

unsigned char adc_get_result(void){
	return ADC_result;
}

void adc_clear_flag(void){
	ADC_result_flag = 0;
}

ISR(ADC_vect) {
	ADC_result = ADCH;
	ADC_result_flag = 1;
	pwm_set_duty_cycle(adc_get_result());
	adc_clear_flag();
}
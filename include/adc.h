#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <avr/interrupt.h>

void adc_init(void);
void adc_start_conversion(void);
unsigned char adc_is_ready(void);
unsigned char adc_get_result(void);
void clear_adc_result_flag(void);

#endif
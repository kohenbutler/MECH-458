#ifndef ADC_H
#define ADC_H

#include <avr/io.h>
#include <avr/interrupt.h>

void adc_init(void);
void set_OR_flag(void);
void clear_OR_flag(void);
int is_OR_flag(void);
void Enable_INT3(void);
void Disable_INT3(void);
void adc_start_conversion(void);
unsigned char adc_is_ready(void);
unsigned int adc_get_result(void);
void clear_adc_result_flag(void);
int classify_block(int val);

#endif
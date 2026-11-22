#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer1_init(void);
void timer_delay_ms(int ms);
void timer_delay_us(int us);
void timer3_init(void);
void timer3_start(void);
void timer4_init(void);
void timer4_start(void);
int is_ramp_down_flag(void);

#endif
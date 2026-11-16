#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer1_init(void);
void timer_delay_ms(int ms);
void timer_delay_us(int us);

#endif
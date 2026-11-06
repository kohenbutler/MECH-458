#ifndef STEPPER_H
#define STEPPER_H

void stepper_init(void);
void step_cw(unsigned int steps);
void step_ccw(unsigned int steps);
void stepper_home(void);
void stepper_demo(void);
void step_ccw90(void);
void step_cw90(void);
void step_180(void);
void s_curve_demo(void);

#endif
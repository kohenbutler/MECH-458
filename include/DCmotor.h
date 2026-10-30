#ifndef DCMOTOR_H
#define DCMOTOR_H

void DCMotor_init(void);
void DCMotor_R(void);
void DCMotor_F(void);
void DCMotor_Brake(void);
void DCMotor_Test(void);
char DCmotor_get_state(void);

#endif
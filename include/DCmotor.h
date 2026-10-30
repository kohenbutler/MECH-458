#ifndef DCMOTOR_H
#define DCMOTOR_H

#define Brake 0
#define Forward 1
#define Reverse 2

void DCMotor_init(void);
void DCMotor_R(void);
void DCMotor_F(void);
void DCMotor_Brake(void);
void DCMotor_Test(void);
char DCmotor_get_state(void);
void set_kill_switch_flag(void);
void clear_kill_switch_flag(void);
char kill_switch_active(void);
void set_change_direction_flag(void);
void clear_change_direction_flag(void);
char is_change_direction(void);

#endif
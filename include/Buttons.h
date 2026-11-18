#ifndef BUTTONS_H
#define BUTTONS_H

void buttons_init(void);
void Enable_INT1(void);
void Disable_INT1(void);
void Enable_INT2(void);
void Disable_INT2(void);
void set_left_button_pressed(void);
void clear_left_button_pressed(void);
int was_left_button_pressed(void);
void set_left_button_wait_release(void);
void clear_left_button_wait_release(void);
int get_left_button_wait_release(void);
void set_right_button_pressed(void);
void clear_right_button_pressed(void);
int was_right_button_pressed(void);
void set_right_button_wait_release(void);
void clear_right_button_wait_release(void);
int get_right_button_wait_release(void);

#endif
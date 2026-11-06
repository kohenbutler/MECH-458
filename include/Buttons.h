#ifndef BUTTONS_H
#define BUTTONS_H

void buttons_init(void);
void Enable_INT1(void);
void Disable_INT1(void);
void Enable_INT2(void);
void Disable_INT2(void);
void set_pause_button_pressed(void);
void clear_pause_button_pressed(void);
int was_pause_button_pressed(void);

#endif
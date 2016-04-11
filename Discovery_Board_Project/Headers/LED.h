#ifndef LED_H
#define LED_H

#include "stdint.h"

void toggle_PWM_flag(void);
void toggle_GPIO_mode_flag(void);
void LED_ON_PWM(void);
void LED_OFF_PWM(void);
void LED_CCW(void);
void LED_CW(void);
void LED_ON(void);
void LED_OFF(void);
void init_PWM(void);
void init_GPIO_PWM(void);
void init_GPIO_NO_PWM(void);
void LED_set_duty_cycle(int duty_cycle);
void set_LED_state(int state);


#define PERIOD 42000
#define ROTATION_COUNT 20

/* DIFFERENT LED STATES */
#define ALL_ON_PWM_ON			1
#define CCW_NO_PWM				2
#define CW_NO_PWM					3
#define ALL_OFF						4

/* GPIO MODES FOR CURRENT_GPIO_MODE FLAG */
#define GPIO_ALTERNATE_MODE		1
#define GPIO_PP_MODE					0

#endif

#ifndef LED_H
#define LED_H

#include "stdint.h"

void LED_ON_PWM(void);
void LED_OFF_PWM(void);
void LED_CCW(void);
void LED_CW(void);
void LED_ON(void);
void LED_OFF(void);
void init_PWM(void);
void init_GPIO_PWM(void);
void LED_SET_DUTY_CYCLE(int duty_cycle);
void init_GPIO_NO_PWM(void);

#define PERIOD 42000
#define PULSE  1000


#define ALL_ON_PWN_ON			1
#define CCW_NO_PWM				2
#define CW_NO_PWM				3
#define ALL_OFF					4

#endif

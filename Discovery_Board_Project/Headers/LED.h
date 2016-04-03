#ifndef LED_H
#define LED_H

void LED_ON_PWM(void);
void LED_OFF_PWM(void);
void LED_CCW(void);
void LED_CW(void);
void LED_ON(void);
void LED_OFF(void);
void init_tim3(void);
void init_GPIO_PWM(void);

#endif
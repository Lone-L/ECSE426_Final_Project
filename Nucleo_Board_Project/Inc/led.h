#ifndef LED_H
#define LED_H

#include "stdint.h"

void Led_SetPattern(uint16_t pattern);
void Led_SetDutyCycle(uint16_t duty_cycle);
void Led_SetSpeed(uint16_t speed);
#endif

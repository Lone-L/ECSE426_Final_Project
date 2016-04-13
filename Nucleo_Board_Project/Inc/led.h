#ifndef LED_H
#define LED_H

#include "stdint.h"

void Led_SetPattern(uint16_t pattern);
void Led_SetDutyCycle(int duty_cycle);

#endif

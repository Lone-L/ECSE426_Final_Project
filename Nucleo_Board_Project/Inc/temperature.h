#ifndef TEMPERATURE_H
#define TEMPERATURE_H

int Temperature_IsDataReady(void);
void Temperature_SetDatareadyFlag(void);
void Temperature_ResetDatareadyFlag(void);

void Temperature_Process(void);

#endif

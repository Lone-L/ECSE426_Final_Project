#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

int Accelerometer_IsDataReady(void);
void Accelerometer_SetDatareadyFlag(void);
void Accelerometer_ResetDatareadyFlag(void);
int Accelerometer_DoubleTap(void);
void Accelerometer_SetDoubletapFlag(void);
void Accelerometer_ResetDoubletapFlag(void);

void Accelerometer_Process(void);

#endif

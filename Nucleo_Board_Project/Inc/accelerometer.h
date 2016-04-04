#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

int Accelerometer_IsRollDataReady(void);
void Accelerometer_SetRollDatareadyFlag(void);
void Accelerometer_ResetRollDatareadyFlag(void);
int Accelerometer_IsPitchDataReady(void);
void Accelerometer_SetPitchDatareadyFlag(void);
void Accelerometer_ResetPitchDatareadyFlag(void);
int Accelerometer_DoubleTap(void);
void Accelerometer_SetDoubletapFlag(void);
void Accelerometer_ResetDoubletapFlag(void);

void Accelerometer_Process(void);

#endif

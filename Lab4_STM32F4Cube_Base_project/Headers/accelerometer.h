#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

void Accelerometer_Init(void);
void ACCELEROMETER_Callback(void);

extern int start_Thread_ACCELEROMETER (void);
void Accelerometer_Init               (void);

extern osThreadId tid_Thread_ACCELEROMETER;
extern osMutexId accel_mutex;

/* SIGNAL used to resume thread */
#define ACCELEROMETER_SIGNAL 0x0001

#endif

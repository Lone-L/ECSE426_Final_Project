#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

void Accelerometer_Init(void);
void ACCELEROMETER_Callback(void);

extern int start_Thread_ACCELEROMETER (void);
void Accelerometer_Init               (void);

float Accelerometer_GetCurrentRoll(void);
float Accelerometer_GetCurrentPitch(void);

extern osThreadId tid_Thread_ACCELEROMETER;
extern osMutexId accel_mutex;

/* Threshold in g's for doubletap detection */
#define DOUBLETAP_THRESHOLD		((float)1.2)

/* SIGNALS used to resume thread */
#define ACCELEROMETER_SIGNAL			0x0001		/* Accelerometer has new data */

/* Milliseconds to keep doubletap pin high upon detection. */
#define DOUBLETAP_TIMEOUT_MS			10
#define ACCEL_DATAREADY_TIMEOUT_MS		10

#endif

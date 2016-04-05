#include "lis3dsh.h"
#include "math.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "accelerometer.h"
#include "kalmanfilter.h"
#include "nucleo_spi.h"

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection

/* Private macros ------------------------------------------------------------*/
#define PI ((float)(3.141592653589793))
#define RAD_TO_DEG(x) ((x) * ((float)180.0) / PI)

void Thread_ACCELEROMETER (void const *argument);                 // thread function

/* Global variables ---------------------------------------------------------- */
osThreadId tid_Thread_ACCELEROMETER;                              // thread id
osThreadDef(Thread_ACCELEROMETER, osPriorityAboveNormal, 1, 0);		  // thread definition struct (last argument default stack size)

static void doubletap_timer_elapsed(const void *arg);
osTimerDef(doubletap_timer, doubletap_timer_elapsed);

/* Accelerometer mutex to protect shared flags */
osMutexId accel_mutex;
osMutexDef(accel_mutex);

/* Which angle (roll/pitch) to read in a NUCLEO_ACCEL_SIGNAL, set by NucleoSPI module. */
int angle_type;

/**
  * @brief  Initialize the accelerometer.
  * @param  None
  * @retval None
  */
void Accelerometer_Init(void)
{
	LIS3DSH_InitTypeDef accel_init;
	LIS3DSH_DRYInterruptConfigTypeDef it_config;
	
	accel_init.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;				/* Output data 25 times per second (25 Hz) as by requirements */
	accel_init.Axes_Enable = LIS3DSH_XYZ_ENABLE;												/* Enable x, y and z axes */
	accel_init.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;			/* Disable continuous update to be able to read the MSB of each reading */
	accel_init.AA_Filter_BW = LIS3DSH_AA_BW_50;													/* Anti-aliasing filter bandwidth set to 50Hz to remove frequencies above 2 * 25Hz */
	accel_init.Full_Scale = LIS3DSH_FULLSCALE_2;												/* Use full scale of +/- 2g */
	accel_init.Self_Test = LIS3DSH_SELFTEST_NORMAL;											/* Self test normal */
	
	/* Initialize LIS3DSH */
	LIS3DSH_Init(&accel_init);
	
	it_config.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;	/* Enable DRY interrupt */
	it_config.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;		/* Set interrupt to be active high as by requirements */
	it_config.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;					/* Set interrupt type to be pulsed as by requirements */
	
	/* Set LIS3DSH interrupt configuration */
	LIS3DSH_DataReadyInterruptConfig(&it_config);
	
	/* Set priority for EXTI0 IRQ */
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	
	/* Enable IRQ for the EXTI Line 0 (LSI3DSH generates INT1 interrupt on GPIO_PIN_0 PE0) */
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
  * @brief  Read acceleration values from accelerometer.
	* @param  ax, ay, az: pointers to float data to store result into.
  * @retval None
  */
static void Accelerometer_ReadAccel(float *ax, float *ay, float *az)
{
	float acc_components[3];
	
	LIS3DSH_ReadACC(acc_components);
	*ax = acc_components[0];
	*ay = acc_components[1];
	*az = acc_components[2];
}

/**
  * @brief  Calibrate acceleration values from accelerometer.
	* @param  ax, ay, az: pointers to float data to be calibrated.
  * @retval None
  */
static void Accelerometer_Calibrate(float *ax, float *ay, float *az)
{
	/* ACC calibration matrix obtained from least-squares method (Doc 15: Tilt angle application notes) */
	const float ACC[4][3] = {{-0.00100376740404743, -1.33049359765868e-05, -1.36545479938343e-05},
							 { 2.32874913430264e-05, 0.00101941787448905,  -3.78704315888482e-06},
							 {-9.92411997403039e-06, 1.08648230411970e-06,	0.00100305721715016},
							 {0.0163849855173971,   -0.00780422421706874,  -0.0367276451297443}};
	float new_x, new_y, new_z;

	new_x = ACC[0][0] * (*ax) + ACC[1][0] * (*ay) + ACC[2][0] * (*az) + ACC[3][0];
	new_y = ACC[0][1] * (*ax) + ACC[1][1] * (*ay) + ACC[2][1] * (*az) + ACC[3][1];
	new_z = ACC[0][2] * (*ax) + ACC[1][2] * (*ay) + ACC[2][2] * (*az) + ACC[3][2];
	
	*ax = new_x;
	*ay = new_y;
	*az = new_z;
}

/**
  * @brief  Get roll and pitch from measured acceleration components.
  * @param  ax, ay, az: float components of acceleration vector (in g)
  * @param  pitch, roll: pointers to floats where to store pitch and roll values
  * @retval None
  */
static void Accelerometer_GetRollPitch(float ax, float ay, float az, float *pitch, float *roll)
{
	/* Axes: the positive y-axis points toward the USB port,
			 the positive x-axis points toward the right (PA7 pin),
			 the positive z-axis points toward the bottom of the board.
	
		Pitch angle is the angle between our y-axis and the horizontal.
		Roll angle is the angle between our x-axis and the horizontal.
	*/
	
	*pitch = RAD_TO_DEG(atan2f(ay, sqrtf(ax * ax + az * az)));
			
	/* When az < 0, the board is past the point where the y-axis points downwards --> angle goes past 90 degrees.
	   But atan2 returns angles between -90 and 90, so we need to get the 180 - value. */
	if (az < 0)
		*pitch = ((float)180.0) - *pitch;
	
	if (*pitch < 0)
		*pitch += (float)360.0;	/* Return angles in the range 0 to 360 instead of -90 to 270 */
					
	*roll = RAD_TO_DEG(atan2f(ax, sqrtf(ay * ay + az * az)));
}

/* Detects Double-Taps */
static int Accelerometer_DetectDoubletap(float accel_mag)
{
	static int counter = 0;
	static int peaks = 0;
	static int in_a_peak = 0;
	static int debug = 0;
	
	if (in_a_peak) {
		if (accel_mag < DOUBLETAP_THRESHOLD) {
			++peaks;
			in_a_peak = 0;
			
			if (peaks == 2) {
				++debug;
				printf("Double-tap: %d!\n", debug);
				return 1;
			}
		}
	} else {
		if (accel_mag > DOUBLETAP_THRESHOLD)
			in_a_peak = 1;
	}
	
	++counter;
	
	if (counter == 30) {
		counter = 0;
		peaks = 0;
		in_a_peak = 0;
	}
	
	return 0;
}

static void doubletap_timer_elapsed(const void *arg)
{
	printf("elapsed double tap\n");
	NucleoSPI_ResetDoubletap();
}

/**
  * @brief  Accelerometer new data callback.
  * @param  None
  * @retval None
  */
void ACCELEROMETER_Callback(void)
{
	osSignalSet(tid_Thread_ACCELEROMETER, ACCELEROMETER_SIGNAL);	/* Set accelerometer signal to indicate new data is available on sensor */
	/* The DRY interrupt flag on the sensor is cleared when the data is read */
}

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_ACCELEROMETER (void) 
{
  tid_Thread_ACCELEROMETER = osThreadCreate(osThread(Thread_ACCELEROMETER), NULL); // Start ACCELEROMETER Thread
  if (!tid_Thread_ACCELEROMETER) return(-1);

  accel_mutex = osMutexCreate(osMutex(accel_mutex));
	
  return(0);
}

/*----------------------------------------------------------------------------
 *      Thread  'Accelerometer': Reads Accelerometer
 *---------------------------------------------------------------------------*/
void Thread_ACCELEROMETER (void const *argument) 
{		
	/* These are preserved between function calls (static) */
	KalmanState kstate_pitch = {0.001, 0.0032, 0.0, 0.0, 0.0};	/* Filter parameters obtained by experiment and variance calculations */
	KalmanState kstate_roll  = {0.001, 0.0032, 0.0, 0.0, 0.0};	/* Filter parameters obtained by experiment and variance calculations */
	
	float ax, ay, az;
	float roll, pitch;
	float filtered_roll, filtered_pitch;

	osTimerId doubletap_timer_id;
	osEvent evt;
	
	doubletap_timer_id = osTimerCreate(osTimer(doubletap_timer), osTimerOnce, NULL);
	
	while(1)
	{
		/* Wait for accelerometer new data signal or Nucleo board SPI signal for read request.
		   No need to send data all the time. */
		evt = osSignalWait(0x00, osWaitForever);	/* Waits for all signals */
		
		if (evt.status == osEventSignal) {
			if (evt.value.signals == ACCELEROMETER_SIGNAL) {
				printf("accelerometer_signal\n");
				Accelerometer_ReadAccel(&ax, &ay, &az);
				Accelerometer_Calibrate(&ax, &ay, &az);
					
				Accelerometer_GetRollPitch(ax, ay, az, &pitch, &roll);		
				Kalmanfilter_asm(&pitch, &filtered_pitch, 1, &kstate_pitch);	/* filter the pitch angle */
				Kalmanfilter_asm(&roll, &filtered_roll, 1, &kstate_roll);			/* filter the roll angle 	*/
				
				if (Accelerometer_DetectDoubletap(sqrtf(ax * ax + ay * ay + az * az))) {
					NucleoSPI_SetDoubletap();
					
					/* Use a pulsed interrupt. Reset the Doubletap pin after some time. */
					osTimerStart(doubletap_timer_id, DOUBLETAP_TIMEOUT_MS);
				}
				
				NucleoSPI_SetAccelDataready();
			} else if (evt.value.signals == NUCLEO_ACCEL_SIGNAL) {
				if (angle_type == ANGLE_TYPE_ROLL) {
					NucleoSPI_SendInt(0xdeadbeef);
				} else if (angle_type == ANGLE_TYPE_PITCH) {
					NucleoSPI_SendInt(0xcafebabe);
				}
				
				NucleoSPI_ResetAccelDataready();
			}
		}
	}
}
	
/*----------------------------------------------------------------------------
 *      
 *---------------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "kalmanfilter.h"
#include "temperature.h"
#include "nucleo_spi.h"

void Thread_TEMPERATURE (void const *argument);             // thread function
osThreadId tid_Thread_TEMPERATURE;                          // thread id
osThreadDef(Thread_TEMPERATURE, osPriorityHigh, 1, 0);	// thread definition struct (last argument default stack size)

static void temp_dataready_timer_elapsed(const void *arg);
osTimerDef(temp_dataready_timer, temp_dataready_timer_elapsed);

ADC_HandleTypeDef ADC1_handle;
static float filtered_temp = 0;

/*----------------------------------------------------------------------------
 *      Thread  'Temperature': Reads Temperature 
 *---------------------------------------------------------------------------*/
void Thread_TEMPERATURE(const void *argument)
{
	uint32_t v_sense;
	float tempinvolts = 0;
	float temperature = 0;
	KalmanState kstate = {0.01, 0.3, 0.0, 0.1, 0.0};

	osTimerId temp_dataready_timer_id;
	HAL_StatusTypeDef rc;

	temp_dataready_timer_id = osTimerCreate(osTimer(temp_dataready_timer), osTimerOnce, NULL);
	
	while (1) {
		/* Wait for temperature new data signal or Nucleo board SPI signal for read request.
		   No need to send data all the time. */
		osSignalWait(TEMPERATURE_SIGNAL, osWaitForever);
				
		rc = HAL_ADC_PollForConversion(&ADC1_handle, 1);
		
		if (rc != HAL_OK) {
			printf("not okay :(\n");
			continue;
			/* what should you do about it?? */
		}
		
		v_sense = HAL_ADC_GetValue(&ADC1_handle);	/* Read register from the ADC */
		
		tempinvolts = (float) v_sense * (V_REF / 4096);	/* Scale the reading into V (resolution is 12bits with VREF+ = 3.3V) */
		temperature = (float) (tempinvolts - V_25) / AVG_SLOPE + TEMP_REF;	/* Formula for temperature from doc_05 p.230 */
		
		Kalmanfilter_asm(&temperature, &filtered_temp, 1, &kstate);
		NucleoSPI_SetTempDataready(temp_dataready_timer_id, TEMP_DATAREADY_TIMEOUT_MS);
	}
}

static void temp_dataready_timer_elapsed(const void *arg)
{
	NucleoSPI_ResetTempDataready();
}

int start_Thread_TEMPERATURE(void)
{
	tid_Thread_TEMPERATURE = osThreadCreate(osThread(Thread_TEMPERATURE), NULL); // Start TEMPERATURE Thread
	if (!tid_Thread_TEMPERATURE) return(-1);
	return(0);
}

void initialize_ADC(void)
{
	/* Information on configuring the ADC found in p.104 doc 19 + doc 5 p.207 + doc 6 p.137*/
	ADC_ChannelConfTypeDef channel_config;	/* Channel configuration struct */
	
	ADC1_handle.Instance = ADC1;	/* Temperature sensor is in channel 16 of ADC1 */
	
	/* Set up the InitTypeDef sub-struct */
	/* The clock for the ADC is PCKL2,at 168 MHz. */
	ADC1_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;	/* Select division by 8 for prescaler. */
	ADC1_handle.Init.Resolution = ADC_RESOLUTION_12B;						/* Select 12 bits resolution */
	ADC1_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;						/* This places the padding zeros on the MS byte */
	ADC1_handle.Init.ScanConvMode = DISABLE;										/* Use single-channel mode */
	ADC1_handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;						/* wat */
	ADC1_handle.Init.ContinuousConvMode = ENABLE;								/* Want continuous conversion mode */
	ADC1_handle.Init.DMAContinuousRequests = DISABLE;						/* wat */
	ADC1_handle.Init.NbrOfConversion = 1;												/* 1 channel / 1 conversion. Simple as that. */
	ADC1_handle.Init.DiscontinuousConvMode = DISABLE;						/* why discontinuous? */
	/* don't even bother with NbrOfDiscConversion */
	ADC1_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;			/* Disable external trigger */
	ADC1_handle.Init.ExternalTrigConvEdge = ADC_SOFTWARE_START;
	/* We don't know what the NbrOfCurrentConversionRank */
	/* Don't know about the rest either... */
	
	/* Initialize global ADC parameters*/
	if (HAL_ADC_Init(&ADC1_handle) != HAL_OK) {
//		printf("Failed to initialize ADC\n");
	}
	
	/* Configure the channel */
	channel_config.Channel = ADC_CHANNEL_16;
	channel_config.Rank = 1;
	channel_config.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	channel_config.Offset = 0;
	
	if (HAL_ADC_ConfigChannel(&ADC1_handle, &channel_config) != HAL_OK) {
//		printf("Failed to config channel ADC.\n");
		/* handle the fail */
	}
	if (HAL_ADC_Start(&ADC1_handle) != HAL_OK){
		printf("Error Starting ADC");
		//Error_Handler(ADC_INIT_FAIL);
	}
	/*HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);*/
}

/* Hal MSP_Init */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	__ADC1_CLK_ENABLE();
}

void Temperature_Init(void)
{
	initialize_ADC();
}

float Temperature_GetCurrentTemp(void)
{
	return filtered_temp;
}

#include "stm32f4xx_hal.h"
#include "temperature.h"
#include "LED.h"

/* Global variables ----------------------------------------------------------*/
TIM_HandleTypeDef timmy3;	/* Needs to be global to be used by TIM3_IRQHandler */
TIM_HandleTypeDef timmy4;	/* Needs to be global to be used by TIM4_IRQHandler */
/**
  * @brief  Initialize the hardware timer 3.
  * @param  None
  * @retval None
  */
void HardwareTimer3_Init(void)
{
	__TIM3_CLK_ENABLE();	/* Enable TIM3 interface clock */
		
	/* Desired_timer_frequency = Timer_input_frequency / (prescaler * period)
		 Timer_input_frequency = 84 MHz (same as TIM3, TIM3 is also on APB1)
		 Desired_timer_frequency = 100Hz
		 ---> prescaler * period = 84 MHz / 100 Hz = 840 000 = 20000 * 42
		 ---> prescaler = 20000, period = 42 */
	
	timmy3.Instance = TIM3;										/* Use TIM3 */
	timmy3.Init.Prescaler = 20000;								/* Set prescaler to 20000 */
	timmy3.Init.CounterMode = TIM_COUNTERMODE_DOWN;				/* Count down */
	timmy3.Init.Period = 42;									/* Set period count register to 42 */
	timmy3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;			/* Set clock division to 1 */
	timmy3.Init.RepetitionCounter = 0;							/* Not valid for TIM3 */
	timmy3.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;			/* Not using channels */
	
	/* Initialize the TIM Time Base Unit */
	HAL_TIM_Base_Init(&timmy3);
	
	/* Start TIM3 in interrupt mode */
	HAL_TIM_Base_Start_IT(&timmy3);
	
	/* Set priority for TIM3 IRQ */
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	
	/* Enable IRQ for the TIM3 Timer*/
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

/**
  * @brief  The period elapsed callback is called when the counter underflows (UEV Update event).
  * @param  TIM_HandleTypeDef *htim: handle to TIM configuration struct
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3) {
		osSignalSet(tid_Thread_TEMPERATURE, TEMPERATURE_SIGNAL);
	} else if (htim->Instance == TIM4) {
		
	}
}

/**
  * @brief  Initialize the hardware timer 4.
  * @param  None
  * @retval None
  */
void HardwareTimer4_Init(void)
{
	__TIM4_CLK_ENABLE();	/* Enable TIM4 interface clock */
		
/*
		Desired_timer_frequency = Timer_input_frequency / (prescaler * period)
		Timer_input_frequency = 84 MHz
		Desired_timer_frequency = 1000 Hz (switch LED at this frequency to reduce flickering)
		---> prescaler * period = 84 MHz / 1000 Hz = 84000 = 20 * 42 000 
		---> prescaler = 21, period = 5000 (our choice, man) 
	*/

	timmy4.Instance = TIM4;
	timmy4.Init.Prescaler = 20;
	timmy4.Init.CounterMode = TIM_COUNTERMODE_UP; //count up mode
	timmy4.Init.RepetitionCounter = 0;
	timmy4.Init.Period = PERIOD;
	timmy4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // don't know what this do?
	

	
	/* Initialize the TIM Time Base Unit */
	HAL_TIM_Base_Init(&timmy4);
	
	/* Start TIM4 in interrupt mode */
	HAL_TIM_Base_Start_IT(&timmy4);
	//HAL_TIM_Base_Start(&timmy4);
	/* Set priority for TIM4 IRQ */
	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
	
	/* Enable IRQ for the TIM4 Timer*/ 
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

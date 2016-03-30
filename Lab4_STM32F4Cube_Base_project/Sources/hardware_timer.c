#include "stm32f4xx_hal.h"
#include "temperature.h"

/* Hardware Timer 4 is TIM4 for the Visuals LEDs */
	 
/* Global variables ----------------------------------------------------------*/
TIM_HandleTypeDef timmy4;	/* Needs to be global to be used by TIM4_IRQHandler */

/**
  * @brief  Initialize the hardware timer 4.
  * @param  None
  * @retval None
  */
void HardwareTimer4_Init(void)
{
	__TIM4_CLK_ENABLE();	/* Enable TIM4 interface clock */
		
	/* Desired_timer_frequency = Timer_input_frequency / (prescaler * period)
		 Timer_input_frequency = 84 MHz (same as TIM3, TIM4 is also on APB1)
		 Desired_timer_frequency = 100Hz
		 ---> prescaler * period = 84 MHz / 100 Hz = 840 000 = 20000 * 42
		 ---> prescaler = 20000, period = 42 */
	
	timmy4.Instance = TIM4;										/* Use TIM4 */
	timmy4.Init.Prescaler = 20000;								/* Set prescaler to 20000 */
	timmy4.Init.CounterMode = TIM_COUNTERMODE_DOWN;				/* Count down */
	timmy4.Init.Period = 42;									/* Set period count register to 42 */
	timmy4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;			/* Set clock division to 1 */
	timmy4.Init.RepetitionCounter = 0;							/* Not valid for TIM4 */
	timmy4.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;			/* Not using channels */
	
	/* Initialize the TIM Time Base Unit */
	HAL_TIM_Base_Init(&timmy4);
	
	/* Start TIM3 in interrupt mode */
	HAL_TIM_Base_Start_IT(&timmy4);
	
	/* Set priority for TIM3 IRQ */
	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
	
	/* Enable IRQ for the TIM3 Timer*/
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

/**
  * @brief  The period elapsed callback is called when the counter underflows (UEV Update event).
  * @param  TIM_HandleTypeDef *htim: handle to TIM configuration struct
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM4) {
		osSignalSet(tid_Thread_TEMPERATURE, TEMPERATURE_SIGNAL);
	}
}

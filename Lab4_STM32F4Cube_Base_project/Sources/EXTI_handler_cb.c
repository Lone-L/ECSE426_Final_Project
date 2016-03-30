#include "stm32f4xx_hal.h"
#include "accelerometer.h"
#include "EXTI_handler_cb.h"
/**
   * @brief  GPIO EXTI Callback. Calls Accelerometer callback.
   * @param  uint16_t GPIO_Pin: Which GPIO pin triggered the callback.
   * @retval None
   */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/* GPIO_PIN_0 corresponds to the Accelerometer (PE0 DRY event) */
	switch (GPIO_Pin)
	{
		case GPIO_PIN_0:
			osSignalSet(tid_Thread_ACCELEROMETER, ACCELEROMETER_SIGNAL);
			break;
		default:
			/* Shouldn't happen */
			break;
	}
}



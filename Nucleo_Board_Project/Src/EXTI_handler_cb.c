#include "stm32f4xx_hal.h"
#include "hci.h"
#include "accelerometer.h"
#include "temperature.h"
#include "discovery_spi.h"

/**
 * @brief  EXTI line detection callback.
 * @param  Specifies the pins connected EXTI line
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin) {
		case DISCOVERY_SPI_DOUBLETAP_PIN:
			__HAL_GPIO_EXTI_CLEAR_IT(DISCOVERY_SPI_DOUBLETAP_PIN);
			Accelerometer_SetDoubletapFlag();
			break;
		case DISCOVERY_SPI_ACCEL_DATAREADY_PIN:
			__HAL_GPIO_EXTI_CLEAR_IT(DISCOVERY_SPI_ACCEL_DATAREADY_PIN);
			Accelerometer_SetDatareadyFlag();
			break;
		case DISCOVERY_SPI_TEMP_DATAREADY_PIN:
			__HAL_GPIO_EXTI_CLEAR_IT(DISCOVERY_SPI_TEMP_DATAREADY_PIN);
			Temperature_SetDatareadyFlag();
			break;
		default:
			HCI_Isr();
			break;
	}
}

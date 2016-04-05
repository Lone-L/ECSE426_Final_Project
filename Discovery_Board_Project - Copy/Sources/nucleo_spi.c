#include "stdint.h"
#include "nucleo_spi.h"
#include "accelerometer.h"
#include "temperature.h"

SPI_HandleTypeDef nucleo_SpiHandle;

void NucleoSPI_RxISR(SPI_HandleTypeDef *hspi)
{
	uint16_t cmd;
	
	__HAL_SPI_DISABLE_IT(hspi, SPI_FLAG_RXNE);
	cmd = hspi->Instance->DR;

	switch (cmd) {
		case NUCLEO_SPI_READ_ROLL_CMD:
			NucleoSPI_SendFloat(1.9);
			NucleoSPI_ResetAccelDataready();
			break;
		case NUCLEO_SPI_READ_PITCH_CMD:
			NucleoSPI_SendFloat(0.5);
			NucleoSPI_ResetAccelDataready();
			break;
		case NUCLEO_SPI_READ_TEMP_CMD:
			NucleoSPI_SendFloat(12.1);
			NucleoSPI_ResetTempDataready();
			break;
		case NUCLEO_SPI_WRITE_LED_PATTERN_CMD:
			/* Signal LED thread */
			break;
		default:
			break;
	}

	__HAL_SPI_ENABLE_IT(hspi, SPI_FLAG_RXNE);
}

void NucleoSPI_SendFloat(float val)
{
	uint32_t x = *(uint32_t *)&val;
	uint16_t dummy;
	
	nucleo_SpiHandle.Instance->DR = x >> 16;
	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_TXE) == RESET);
	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_RXNE) == RESET);
	dummy = nucleo_SpiHandle.Instance->DR;
	nucleo_SpiHandle.Instance->DR = x & 0xffff;
	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_TXE) == RESET);
	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_RXNE) == RESET);
	dummy = nucleo_SpiHandle.Instance->DR;
	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_BSY) != RESET);
}

void NucleoSPI_Init(void)
{
	/* Configure the low level interface ---------------------------------------*/
	/* SPI configuration -------------------------------------------------------*/
	__HAL_RCC_SPI2_CLK_ENABLE();
	
  HAL_SPI_DeInit(&nucleo_SpiHandle);
  nucleo_SpiHandle.Instance 							  = SPI2;	/* SPI1 is used by the LIS3DSH driver. */
  nucleo_SpiHandle.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_4;
  nucleo_SpiHandle.Init.Direction 					= SPI_DIRECTION_2LINES;
  nucleo_SpiHandle.Init.CLKPhase 						= SPI_PHASE_1EDGE;
  nucleo_SpiHandle.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  nucleo_SpiHandle.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  nucleo_SpiHandle.Init.CRCPolynomial 			= 7;
  nucleo_SpiHandle.Init.DataSize 						= SPI_DATASIZE_16BIT;
  nucleo_SpiHandle.Init.FirstBit 						= SPI_FIRSTBIT_MSB;
  nucleo_SpiHandle.Init.NSS 								= SPI_NSS_SOFT;
  nucleo_SpiHandle.Init.TIMode 							= SPI_TIMODE_DISABLED;
  nucleo_SpiHandle.Init.Mode 								= SPI_MODE_SLAVE;				/* Sensor boardz iz slave to teh Nucleo boradz. */
	nucleo_SpiHandle.RxISR										= NucleoSPI_RxISR;
	
	if (HAL_SPI_Init(&nucleo_SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};
  
	__HAL_SPI_ENABLE(&nucleo_SpiHandle);
	__HAL_SPI_ENABLE_IT(&nucleo_SpiHandle, SPI_IT_RXNE);
	
	/* Enable SPI2 IRQn */
	HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SPI2_IRQn);
}

void NucleoSPI_SetAccelDataready(void)
{
	HAL_GPIO_WritePin(NUCLEO_SPI_ACCEL_DATAREADY_PORT, NUCLEO_SPI_ACCEL_DATAREADY_PIN, GPIO_PIN_SET);
}

void NucleoSPI_ResetAccelDataready(void)
{
	HAL_GPIO_WritePin(NUCLEO_SPI_ACCEL_DATAREADY_PORT, NUCLEO_SPI_ACCEL_DATAREADY_PIN, GPIO_PIN_RESET);
}

void NucleoSPI_SetTempDataready(void)
{
	HAL_GPIO_WritePin(NUCLEO_SPI_TEMP_DATAREADY_PORT, NUCLEO_SPI_TEMP_DATAREADY_PIN, GPIO_PIN_SET);
}

void NucleoSPI_ResetTempDataready(void)
{
	HAL_GPIO_WritePin(NUCLEO_SPI_TEMP_DATAREADY_PORT, NUCLEO_SPI_TEMP_DATAREADY_PIN, GPIO_PIN_RESET);
}

void NucleoSPI_SetDoubletap(void)
{
	HAL_GPIO_WritePin(NUCLEO_SPI_DOUBLETAP_GPIO_PORT, NUCLEO_SPI_DOUBLETAP_PIN, GPIO_PIN_SET);
}

void NucleoSPI_ResetDoubletap(void)
{
	HAL_GPIO_WritePin(NUCLEO_SPI_DOUBLETAP_GPIO_PORT, NUCLEO_SPI_DOUBLETAP_PIN, GPIO_PIN_RESET);
}

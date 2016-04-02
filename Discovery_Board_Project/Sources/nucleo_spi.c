#include "stdint.h"
#include "nucleo_spi.h"
#include "accelerometer.h"
#include "temperature.h"

SPI_HandleTypeDef nucleo_SpiHandle;

static void NucleoSPI_RxISR(SPI_HandleTypeDef *hspi)
{
	uint8_t cmd;
	
	cmd = hspi->Instance->DR;
	
	switch (cmd) {
		case NUCLEO_SPI_READ_ROLL_CMD:
			angle_type = ANGLE_TYPE_ROLL;
			osSignalSet(tid_Thread_ACCELEROMETER, NUCLEO_ACCEL_SIGNAL);
			break;
		case NUCLEO_SPI_READ_PITCH_CMD:
			angle_type = ANGLE_TYPE_PITCH;
			osSignalSet(tid_Thread_ACCELEROMETER, NUCLEO_ACCEL_SIGNAL);
			break;
		case NUCLEO_SPI_READ_TEMP_CMD:
			osSignalSet(tid_Thread_TEMPERATURE, NUCLEO_TEMP_SIGNAL);
			break;
		case NUCLEO_SPI_WRITE_LED_PATTERN_CMD:
			/* Signal LED thread */
			break;
		default:
			break;
	}
}

static void NucleoSPI_SendByte(uint8_t byte)
{
  /* Loop while DR register is not empty */
  while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_TXE) == RESET){}

  /* Send a Byte through the SPI peripheral */
  nucleo_SpiHandle.Instance->DR = byte;
}

static uint8_t NucleoSPI_ReadByte(void)
{
	/* Wait to receive a Byte */
  while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_RXNE) == RESET){}

  /* Return the Byte read from the SPI bus */ 
  return nucleo_SpiHandle.Instance->DR;
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
  nucleo_SpiHandle.Init.DataSize 						= SPI_DATASIZE_8BIT;
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

void NucleoSPI_SendFloatValue(float x)
{
	uint8_t *bytes;
	
	/* Cast &x to a uin8_t * to get the bytes. */
	bytes = (uint8_t *)&x;
	
	/* Send in little-endian order. */
	NucleoSPI_SendByte(bytes[0]);
	NucleoSPI_SendByte(bytes[1]);
	NucleoSPI_SendByte(bytes[2]);
	NucleoSPI_SendByte(bytes[3]);
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

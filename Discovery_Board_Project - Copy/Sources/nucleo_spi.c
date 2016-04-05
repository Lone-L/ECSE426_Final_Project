#include "stdint.h"
#include "nucleo_spi.h"
#include "accelerometer.h"
#include "temperature.h"

SPI_HandleTypeDef nucleo_SpiHandle;

void NucleoSPI_RxISR(SPI_HandleTypeDef *hspi)
{
	uint16_t cmd;
	
	cmd = hspi->Instance->DR;
	
	printf("%04x\n", cmd);
	
	switch (cmd) {
		case NUCLEO_SPI_READ_ROLL_CMD:
			angle_type = ANGLE_TYPE_ROLL;
			printf("read roll cmd\n");
			osSignalSet(tid_Thread_ACCELEROMETER, NUCLEO_ACCEL_SIGNAL);
			break;
		case NUCLEO_SPI_READ_PITCH_CMD:
			angle_type = ANGLE_TYPE_PITCH;
			printf("send pitch cmd\n");
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

void NucleoSPI_SendShort(uint16_t shrt)
{
  /* Loop while DR register is not empty */
//	printf("waiting BSY\n");
//  while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_BSY) != RESET){}
		
	printf("sending %04x\n", shrt);

  /* Send a Byte through the SPI peripheral */
	nucleo_SpiHandle.Instance->DR = shrt;
	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_TXE) != RESET){}
	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_BSY) != RESET){}
}

void NucleoSPI_SendInt(uint32_t val)
{
	printf("sending %08x\n", val);
	HAL_NVIC_DisableIRQ(SPI2_IRQn);
	__disable_irq();
	
	printf("Waiting TXE1\n");
	while (!__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_TXE));
	*(volatile uint16_t *)&(nucleo_SpiHandle.Instance->DR) = val & 0xffff;
	printf("Waiting RXNE1\n");
	while (!__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_RXNE));
	printf("%04x\n", nucleo_SpiHandle.Instance->DR);
	
	printf("Waiting TXE2\n");
	while (!__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_TXE));
	*(volatile uint16_t *)&(nucleo_SpiHandle.Instance->DR) = val >> 16;
	printf("Waiting RXNE\n");
	while (!__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_RXNE));
	printf("%04x\n", nucleo_SpiHandle.Instance->DR);
	
	__enable_irq();
	HAL_NVIC_EnableIRQ(SPI2_IRQn);
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

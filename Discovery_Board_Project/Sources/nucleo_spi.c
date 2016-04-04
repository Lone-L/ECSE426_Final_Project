#include "stdint.h"
#include "nucleo_spi.h"
#include "accelerometer.h"
#include "temperature.h"

SPI_HandleTypeDef nucleo_SpiHandle;

static void NucleoSPI_SendByte(uint8_t byte)
{
  /* Loop while DR register is not empty */
  while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_TXE) == RESET){}

	printf("byte %02x\n", byte);
		
  /* Send a Byte through the SPI peripheral */
	nucleo_SpiHandle.Instance->DR = byte;

	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_BSY) != RESET){}
	while (__HAL_SPI_GET_FLAG(&nucleo_SpiHandle, SPI_FLAG_TXE) == RESET);
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
  nucleo_SpiHandle.Init.Mode 								= SPI_MODE_MASTER;
	
	if (HAL_SPI_Init(&nucleo_SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};
  
	__HAL_SPI_ENABLE(&nucleo_SpiHandle);
}

void NucleoSPI_SendFloatValue(uint8_t type, float x)
{
	uint8_t *bytes;
	
	NucleoSPI_SendByte(type);
	
	/* Cast &x to a uin8_t * to get the bytes. */
	bytes = (uint8_t *)&x;
	
	/* Send in little-endian order. */
	NucleoSPI_SendByte(bytes[0]);
	NucleoSPI_SendByte(bytes[1]);
	NucleoSPI_SendByte(bytes[2]);
	NucleoSPI_SendByte(bytes[3]);
}

void NucleoSPI_SetDoubletap(void)
{
	HAL_GPIO_WritePin(NUCLEO_SPI_DOUBLETAP_GPIO_PORT, NUCLEO_SPI_DOUBLETAP_PIN, GPIO_PIN_SET);
}

void NucleoSPI_ResetDoubletap(void)
{
	HAL_GPIO_WritePin(NUCLEO_SPI_DOUBLETAP_GPIO_PORT, NUCLEO_SPI_DOUBLETAP_PIN, GPIO_PIN_RESET);
}

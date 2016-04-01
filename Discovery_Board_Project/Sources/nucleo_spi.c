#include "nucleo_spi.h"
 
SPI_HandleTypeDef    nucleo_SpiHandle;

static void NucleoSPI_RxISR(SPI_HandleTypeDef *hspi)
{
	
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
	if (HAL_SPI_Init(&nucleo_SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};
  
	__HAL_SPI_ENABLE(&nucleo_SpiHandle);
	__HAL_SPI_ENABLE_IT(&nucleo_SpiHandle, SPI_IT_RXNE);	/* Enable interrupts for slave (nucleo board will send data to us). */
	
	/* Might need to set the RxISR stuff to tell it what to do on rx. See example SPI_RxISR */
}

/* See stm32f4xx.c for definition of SPI2_IRQHandler */

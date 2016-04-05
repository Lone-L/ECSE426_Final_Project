#include "discovery_spi.h"

SPI_HandleTypeDef discovery_SpiHandle;

/**
 * @brief  Enable SPI IRQ.
 * @param  None
 * @retval None
 */
static void Enable_SPI_IRQ(void)
{
  HAL_NVIC_EnableIRQ(DISCOVERY_SPI_DOUBLETAP_IRQn);  
	HAL_NVIC_EnableIRQ(DISCOVERY_SPI_ACCEL_IRQn);
	HAL_NVIC_EnableIRQ(DISCOVERY_SPI_TEMP_IRQn);
}

/**
 * @brief  Disable SPI IRQ.
 * @param  None
 * @retval None
 */
static void Disable_SPI_IRQ(void)
{ 
	HAL_NVIC_EnableIRQ(DISCOVERY_SPI_DOUBLETAP_IRQn);  
	HAL_NVIC_EnableIRQ(DISCOVERY_SPI_ACCEL_IRQn);
	HAL_NVIC_EnableIRQ(DISCOVERY_SPI_TEMP_IRQn);
}

//void DebugSPI(uint16_t shrt)
//{
//	int i;
//	HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_GPIO_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_RESET);
//	
//	for (i = 0; i < 16; i++) {
//		if (shrt & (1 << (15 - i)))
//			HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_GPIO_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_SET);
//		else
//			HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_GPIO_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_RESET);
//	}
//	
//	HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_GPIO_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_RESET);
//}

static uint16_t DiscoverySPI_SendShort(uint16_t shrt)
{
  /* Send a Byte through the SPI peripheral */
  discovery_SpiHandle.Instance->DR = shrt;
	while(__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_TXE) == RESET) {}

	/* Wait to receive a Byte */
  while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE) == RESET) {}
			
  /* Return the Byte read from the SPI bus */ 
  return discovery_SpiHandle.Instance->DR;
}

void DiscoverySPI_Init(void)
{
	/* Configure the low level interface ---------------------------------------*/
	/* SPI configuration -------------------------------------------------------*/
	DISCOVERY_SPI_RCC_CLK_ENABLE();
	
  HAL_SPI_DeInit(&discovery_SpiHandle);
  discovery_SpiHandle.Instance 							  = DISCOVERY_SPI_INSTANCE;
  discovery_SpiHandle.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_4;
  discovery_SpiHandle.Init.Direction 					= SPI_DIRECTION_2LINES;
  discovery_SpiHandle.Init.CLKPhase 					= SPI_PHASE_1EDGE;
  discovery_SpiHandle.Init.CLKPolarity 				= SPI_POLARITY_LOW;
  discovery_SpiHandle.Init.CRCCalculation			= SPI_CRCCALCULATION_DISABLED;
  discovery_SpiHandle.Init.CRCPolynomial 			= 7;
  discovery_SpiHandle.Init.DataSize 					= SPI_DATASIZE_16BIT;
  discovery_SpiHandle.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
  discovery_SpiHandle.Init.NSS 								= SPI_NSS_SOFT;
  discovery_SpiHandle.Init.TIMode 						= SPI_TIMODE_DISABLED;
  discovery_SpiHandle.Init.Mode 							= SPI_MODE_MASTER;				/* I are teh MasterZ! */

	if (HAL_SPI_Init(&discovery_SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};

	__HAL_SPI_ENABLE(&discovery_SpiHandle);

}

uint16_t DiscoverySPI_ReadShortValue(uint16_t cmd)
{
	return DiscoverySPI_SendShort(cmd);
}

float DiscoverySPI_ReadFloatValue(uint16_t cmd)
{
	uint32_t upper, lower, x;
	uint16_t dummy;
	
	discovery_SpiHandle.Instance->DR = cmd;
	while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_TXE) == RESET);
	while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE) == RESET);
	dummy = discovery_SpiHandle.Instance->DR;

	/* maybe add more delay to make sure slave has time to put its first reply short. Some times communication stops randomly */
	for (volatile int i = 0; i < 100; i++);
	
	discovery_SpiHandle.Instance->DR = 0x0000;
	while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_TXE) == RESET);
	while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE) == RESET);
	upper = discovery_SpiHandle.Instance->DR;
	discovery_SpiHandle.Instance->DR = 0x0000;
	while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_TXE) == RESET);
	while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE) == RESET);
	lower = discovery_SpiHandle.Instance->DR;
	while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_BSY) != RESET);
	
	x = lower | (upper << 16);
	return *(float *)&x;
}

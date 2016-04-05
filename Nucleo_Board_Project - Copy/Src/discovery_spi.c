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

void DebugSPI(uint16_t shrt)
{
	int i;
	HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_GPIO_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_RESET);
	
	for (i = 0; i < 16; i++) {
		if (shrt & (1 << (15 - i)))
			HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_GPIO_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_GPIO_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_RESET);
	}
	
	HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_GPIO_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_RESET);
}

static uint16_t DiscoverySPI_SendShort(uint16_t shrt)
{
  /* Send a Byte through the SPI peripheral */
  discovery_SpiHandle.Instance->DR = shrt;
	while(__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_BSY) != RESET) {}

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
	DiscoverySPI_SendShort(cmd);
	return DiscoverySPI_SendShort(0x0000);
}

uint32_t DiscoverySPI_ReadIntValue(uint16_t cmd)
{
	uint32_t lower, upper;
	uint16_t temp;
	
  /* Send a Byte through the SPI peripheral */
	DebugSPI(0x0101);
	while (!__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_TXE));
  discovery_SpiHandle.Instance->DR = cmd;
	DebugSPI(0x0505);
	while (!__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE));
	temp = discovery_SpiHandle.Instance->DR;
	DebugSPI(0x1515);
	while (!__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_TXE));
  discovery_SpiHandle.Instance->DR = 0x0000;
	DebugSPI(0x5555);
	while (!__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE));
	lower = discovery_SpiHandle.Instance->DR;
	DebugSPI(0x1111);
	
	while (!__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_TXE));
  discovery_SpiHandle.Instance->DR = 0x0000;
	while (!__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE));
	upper = discovery_SpiHandle.Instance->DR;
	
  /* Return the Byte read from the SPI bus */ 
  return lower | (upper << 16);
}

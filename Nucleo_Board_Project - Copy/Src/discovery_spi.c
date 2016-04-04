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

static void toggle_SCK_pin(void)
{
	int i;
	
	for (i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(DISCOVERY_SPI_SCK_GPIO_PORT, DISCOVERY_SPI_SCK_PIN, GPIO_PIN_SET);
		
		for (volatile int j; j < 100; j++);
		
		HAL_GPIO_WritePin(DISCOVERY_SPI_SCK_GPIO_PORT, DISCOVERY_SPI_SCK_PIN, GPIO_PIN_RESET);
	}
}

static uint8_t DiscoverySPI_SendByte(volatile uint8_t byte)
{
  /* Send a Byte through the SPI peripheral */
  discovery_SpiHandle.Instance->DR = byte;
	while(__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_BSY) == RESET) {}

//	HAL_Delay(1);
	toggle_SCK_pin();

	/* Wait to receive a Byte */
  while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE) == RESET) {}
			
  /* Return the Byte read from the SPI bus */ 
  return discovery_SpiHandle.Instance->DR;
}

static uint8_t DiscoverySPI_ReadByte(void)
{
	/* Wait to receive a Byte */
  while (__HAL_SPI_GET_FLAG(&discovery_SpiHandle, SPI_FLAG_RXNE) == RESET);
		
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
  discovery_SpiHandle.Init.DataSize 					= SPI_DATASIZE_8BIT;
  discovery_SpiHandle.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
  discovery_SpiHandle.Init.NSS 								= SPI_NSS_SOFT;
  discovery_SpiHandle.Init.TIMode 						= SPI_TIMODE_DISABLED;
  discovery_SpiHandle.Init.Mode 							= SPI_MODE_MASTER;				/* I are teh MasterZ! */

	if (HAL_SPI_Init(&discovery_SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};

	__HAL_SPI_ENABLE(&discovery_SpiHandle);

}

float DiscoverySPI_ReadFloatValue(uint8_t cmd)
{
	float value;
	uint8_t *bytes;
	
	bytes = (uint8_t *)&value;
	Disable_SPI_IRQ();
	
	/* Set CS pin to low at start of transmission */
	DISCOVERY_SPI_CS_LOW();
	
//	HAL_SPI_TransmitReceive(&discovery_SpiHandle, &cmd, &bytes[0], 1, 15);
	bytes[0] = DiscoverySPI_SendByte(0xc9);//cmd);
	
//	HAL_Delay(2);
//	bytes[0] = DiscoverySPI_ReadByte();
	
	/* Set CS pin to high at end of transmission */
	DISCOVERY_SPI_CS_HIGH();

	Enable_SPI_IRQ();
	
	return value;
}

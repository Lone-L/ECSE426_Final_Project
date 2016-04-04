#include "discovery_spi.h"
#include "accelerometer.h"
#include "temperature.h"

SPI_HandleTypeDef discovery_SpiHandle;

static volatile uint8_t buffer[4];
static volatile int index = 0;
static volatile uint8_t type = 0;
static volatile int has_type = 0;

void DebugSPI(uint8_t byte)
{
	int i;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	
	for (i = 0; i < 8; i++) {
		if (byte & (1 << (7 - i)))
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	}
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
}

static void DiscoverySPI_RxISR(SPI_HandleTypeDef *hspi)
{
	uint8_t byte;
	
	byte = hspi->Instance->DR;
	
	if (has_type) {
		buffer[index++] = byte;
	} else {
		type = byte;
		has_type = 1;
	}
	
	if (index == sizeof buffer) {
		index = 0;
		has_type = 0;
		
		switch (type) {
			case DISCOVERY_SPI_ACCEL_ROLL_TYPE:
				Accelerometer_SetRollDatareadyFlag();
				break;				
			case DISCOVERY_SPI_ACCEL_PITCH_TYPE:
				Accelerometer_SetPitchDatareadyFlag();
				break;
			case DISCOVERY_SPI_TEMP_TYPE:
				Temperature_SetDatareadyFlag();
				break;
			default:
				break;
		}
	}
}

float DiscoverySPI_ReadFloatValue(void)
{	
  return *(float *)buffer;
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
  discovery_SpiHandle.Init.Mode 							= SPI_MODE_SLAVE;
	discovery_SpiHandle.RxISR										= DiscoverySPI_RxISR;
	
	if (HAL_SPI_Init(&discovery_SpiHandle) != HAL_OK) {printf ("ERROR: Error in initialising SPI2 \n");};

	__HAL_SPI_ENABLE(&discovery_SpiHandle);
	__HAL_SPI_ENABLE_IT(&discovery_SpiHandle, SPI_IT_RXNE);
	
	HAL_NVIC_SetPriority(DISCOVERY_SPI_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DISCOVERY_SPI_IRQn);
}

#include "temperature.h"
#include "discovery_spi.h"

static volatile int TEMPERATURE_DATAREADY_FLAG;

int Temperature_IsDataReady(void)
{
	return TEMPERATURE_DATAREADY_FLAG;
}

void Temperature_SetDatareadyFlag(void)
{
	TEMPERATURE_DATAREADY_FLAG = 1;
}

void Temperature_ResetDatareadyFlag(void)
{
	TEMPERATURE_DATAREADY_FLAG = 0;
}

void Temperature_Process(void)
{
	float temperature;
	
	if (Temperature_IsDataReady()) {
		temperature = DiscoverySPI_ReadFloatValue();

		if (temperature != (float)12.1)
			HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_SET);
//			DebugSPI(0xcc);
		
		Temperature_ResetDatareadyFlag();
	}
}

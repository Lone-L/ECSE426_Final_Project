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
	uint16_t temperature;
	
//	if (Temperature_IsDataReady()) {
//		DiscoverySPI_ReadShortValue(DISCOVERY_SPI_READ_TEMP_CMD);
//		temperature = DiscoverySPI_ReadShortValue(0x0000);
//		DebugSPI(temperature);
//		Temperature_ResetDatareadyFlag();
//	}
}

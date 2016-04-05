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
		uint8_t byte;
//		byte = DiscoverySPI_ReadByteValue(DISCOVERY_SPI_READ_TEMP_CMD);
//		DebugSPI(byte);
	}
}

#include "temperature.h"
#include "discovery_spi.h"
#include "sensor_service.h"

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

/* watchpoint!! */
float temperature;
extern int16_t tempNstuff;
void Temperature_Process(void)
{
	if (Temperature_IsDataReady()) {
		temperature = DiscoverySPI_ReadFloatValue(DISCOVERY_SPI_READ_TEMP_CMD);
		Temperature_ResetDatareadyFlag();
		tempNstuff = temperature;
	}
}

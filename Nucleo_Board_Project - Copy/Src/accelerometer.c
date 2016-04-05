#include "accelerometer.h"
#include "discovery_spi.h"

static volatile int ACCELEROMETER_DATAREADY_FLAG = 0;
static volatile int ACCELEROMETER_DOUBLETAP_FLAG = 0;

int Accelerometer_IsDataReady(void)
{
	return ACCELEROMETER_DATAREADY_FLAG;
}

void Accelerometer_SetDatareadyFlag(void)
{
	ACCELEROMETER_DATAREADY_FLAG = 1;
}

void Accelerometer_ResetDatareadyFlag(void)
{
	ACCELEROMETER_DATAREADY_FLAG = 0;
}

int Accelerometer_DoubleTap(void)
{
	return ACCELEROMETER_DOUBLETAP_FLAG;
}

void Accelerometer_SetDoubletapFlag(void)
{
	ACCELEROMETER_DOUBLETAP_FLAG = 1;
}

void Accelerometer_ResetDoubletapFlag(void)
{
	ACCELEROMETER_DOUBLETAP_FLAG = 0;
}

/* use watchpoints for debug!!! */
float roll, pitch;
int doubletapped = 0;

void Accelerometer_Process(void)
{
	
	uint32_t test;
	
	if (Accelerometer_IsDataReady()) {
		pitch = DiscoverySPI_ReadFloatValue(DISCOVERY_SPI_READ_PITCH_CMD);
		roll = DiscoverySPI_ReadFloatValue(DISCOVERY_SPI_READ_ROLL_CMD);
		Accelerometer_ResetDatareadyFlag();
	}

	if (Accelerometer_DoubleTap()) {
		doubletapped = !doubletapped;
	}
}

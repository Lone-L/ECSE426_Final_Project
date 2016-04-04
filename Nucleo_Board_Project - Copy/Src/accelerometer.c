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

void Accelerometer_Process(void)
{
	float roll, pitch;

	if (Accelerometer_IsDataReady()) {
		roll = DiscoverySPI_ReadFloatValue(DISCOVERY_SPI_READ_ROLL_CMD);
		pitch = DiscoverySPI_ReadFloatValue(*(uint8_t *)&roll);
		printf("Roll: %f, Pitch: %f\n", roll, pitch);
		Accelerometer_ResetDatareadyFlag();
	}

	if (Accelerometer_DoubleTap()) {
		printf("Double-tap!\n");
	}
}

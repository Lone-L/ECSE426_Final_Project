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
	uint16_t roll, pitch, cakes;
	uint32_t test;
	
	if (Accelerometer_IsDataReady()) {
//		test = DiscoverySPI_ReadIntValue(DISCOVERY_SPI_READ_ROLL_CMD);
		DiscoverySPI_ReadShortValue(DISCOVERY_SPI_READ_PITCH_CMD);
		pitch = DiscoverySPI_ReadShortValue(DISCOVERY_SPI_READ_ROLL_CMD);
		roll = DiscoverySPI_ReadShortValue(0x0000);
//		cakes = DiscoverySPI_ReadShortValue(0x0000);
//		DiscoverySPI_ReadShortValue(0xdead);
//		DiscoverySPI_ReadShortValue(0xbeef);
//		DebugSPI(test >> 16);
//		DebugSPI(test & 0xffff);
		DebugSPI(roll);
	  DebugSPI(pitch);
//		DebugSPI(cakes);
		Accelerometer_ResetDatareadyFlag();
	}

	if (Accelerometer_DoubleTap()) {
		printf("Double-tap!\n");
	}
}

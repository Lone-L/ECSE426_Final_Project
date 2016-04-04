#include "accelerometer.h"
#include "discovery_spi.h"

static volatile int ACCELEROMETER_ROLL_DATAREADY_FLAG = 0;
static volatile int ACCELEROMETER_PITCH_DATAREADY_FLAG = 0;
static volatile int ACCELEROMETER_DOUBLETAP_FLAG = 0;

int Accelerometer_IsRollDataReady(void)
{
	return ACCELEROMETER_ROLL_DATAREADY_FLAG;
}

void Accelerometer_SetRollDatareadyFlag(void)
{
	ACCELEROMETER_ROLL_DATAREADY_FLAG = 1;
}

void Accelerometer_ResetRollDatareadyFlag(void)
{
	ACCELEROMETER_ROLL_DATAREADY_FLAG = 0;
}

int Accelerometer_IsPitchDataReady(void)
{
	return ACCELEROMETER_PITCH_DATAREADY_FLAG;
}

void Accelerometer_SetPitchDatareadyFlag(void)
{
	ACCELEROMETER_PITCH_DATAREADY_FLAG = 1;
}

void Accelerometer_ResetPitchDatareadyFlag(void)
{
	ACCELEROMETER_PITCH_DATAREADY_FLAG = 0;
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
	float roll;
	float pitch;
	
	if (Accelerometer_IsRollDataReady()) {
		roll = DiscoverySPI_ReadFloatValue();

		if (roll != (float)0.5)
			HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_SET);
//			DebugSPI(0xaa);
		
		Accelerometer_ResetRollDatareadyFlag();
	}
	
	if (Accelerometer_IsPitchDataReady()) {
		pitch = DiscoverySPI_ReadFloatValue();
		
		if (pitch != (float)1.9)
			HAL_GPIO_WritePin(DISCOVERY_SPI_DEBUG_PORT, DISCOVERY_SPI_DEBUG_PIN, GPIO_PIN_SET);
//			DebugSPI(0x71);
		
		Accelerometer_ResetPitchDatareadyFlag();
	}
		
	if (Accelerometer_DoubleTap()) {
		Accelerometer_ResetDoubletapFlag();
		DebugSPI(0x44);
	}
}

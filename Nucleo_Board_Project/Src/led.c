#include "stdint.h"
#include "discovery_spi.h"

void Led_SetPattern(uint16_t pattern)
{
	DiscoverySPI_SendShort(DISCOVERY_SPI_WRITE_LED_PATTERN_CMD, pattern);
}

void Led_SetDutyCycle(int duty_cycle)
{
	DiscoverySPI_SendShort(DISCOVERY_SPI_WRITE_LED_DUTYCYCLE_CMD, duty_cycle);
}

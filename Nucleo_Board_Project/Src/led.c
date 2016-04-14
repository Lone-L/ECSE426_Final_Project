#include "stdint.h"
#include "discovery_spi.h"

void Led_SetPattern(uint16_t pattern)
{
	DiscoverySPI_SendShort(DISCOVERY_SPI_WRITE_LED_PATTERN_CMD, pattern);
}

void Led_SetDutyCycle(uint16_t duty_cycle)
{
	DiscoverySPI_SendShort(DISCOVERY_SPI_WRITE_LED_DUTYCYCLE_CMD, duty_cycle);
}

void Led_SetSpeed(uint16_t speed)
{
	DiscoverySPI_SendShort(DISCOVERY_SPI_WRITE_LED_SPEED_CMD, speed);
}

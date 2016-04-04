#ifndef NUCLEO_SPI_H
#define NUCLEO_SPI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"

void NucleoSPI_Init(void);
void NucleoSPI_SendFloatValue(uint8_t type, float x);

void NucleoSPI_SetDoubletap(void);
void NucleoSPI_ResetDoubletap(void);

/* Nucleo SPI pin defines */
#define NUCLEO_SPI_SCK_PIN               		GPIO_PIN_10                 /* PB.10 */		/* Blue */
#define NUCLEO_SPI_SCK_GPIO_PORT         		GPIOB                       /* GPIOB */

#define NUCLEO_SPI_MISO_PIN              		GPIO_PIN_2                  /* PC.2 */		/* Orange */
#define NUCLEO_SPI_MISO_GPIO_PORT        		GPIOC                       /* GPIOC */

#define NUCLEO_SPI_MOSI_PIN              		GPIO_PIN_3                  /* PC.3 */		/* Green */
#define NUCLEO_SPI_MOSI_GPIO_PORT        		GPIOC                       /* GPIOC */

#define NUCLEO_SPI_DOUBLETAP_PIN						GPIO_PIN_6									/* PC.6 */		/* Yellow */
#define NUCLEO_SPI_DOUBLETAP_GPIO_PORT			GPIOC												/* GPIOC */

#define NUCLEO_SPI_ACCEL_ROLL_TYPE					0x01
#define NUCLEO_SPI_ACCEL_PITCH_TYPE					0x02
#define NUCLEO_SPI_TEMP_TYPE								0x03

#endif

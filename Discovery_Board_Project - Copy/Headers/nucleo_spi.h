#ifndef NUCLEO_SPI_H
#define NUCLEO_SPI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"

void NucleoSPI_Init(void);
void NucleoSPI_SendFloatValue(float x);

void NucleoSPI_SetAccelDataready(void);
void NucleoSPI_ResetAccelDataready(void);
void NucleoSPI_SetTempDataready(void);
void NucleoSPI_ResetTempDataready(void);
void NucleoSPI_SetDoubletap(void);
void NucleoSPI_ResetDoubletap(void);

/* Nucleo SPI pin defines */
#define NUCLEO_SPI_SCK_PIN               		GPIO_PIN_10                 /* PC.10 */		/* Blue */
#define NUCLEO_SPI_SCK_GPIO_PORT         		GPIOB                       /* GPIOB */

#define NUCLEO_SPI_MISO_PIN              		GPIO_PIN_2                  /* PC.2 */		/* Orange */
#define NUCLEO_SPI_MISO_GPIO_PORT        		GPIOC                       /* GPIOC */

#define NUCLEO_SPI_MOSI_PIN              		GPIO_PIN_3                  /* PC.3 */		/* Green */
#define NUCLEO_SPI_MOSI_GPIO_PORT        		GPIOC                       /* GPIOC */

#define NUCLEO_SPI_CS_PIN										GPIO_PIN_5									/* PC.5 */		/* Brown */
#define NUCLEO_SPI_CS_GPIO_PORT							GPIOC												/* GPIOC */

#define NUCLEO_SPI_DOUBLETAP_PIN						GPIO_PIN_6									/* PC.6 */		/* Yellow */
#define NUCLEO_SPI_DOUBLETAP_GPIO_PORT			GPIOC												/* GPIOC */

#define NUCLEO_SPI_ACCEL_DATAREADY_PIN			GPIO_PIN_8									/* PC.8 */		/* Black */
#define NUCLEO_SPI_ACCEL_DATAREADY_PORT			GPIOC												/* GPIOC */

#define NUCLEO_SPI_TEMP_DATAREADY_PIN				GPIO_PIN_9									/* PC.9 */		/* Gray */
#define NUCLEO_SPI_TEMP_DATAREADY_PORT			GPIOC												/* GPIOC */

/* Nucleo SPI command defines */
#define NUCLEO_SPI_READ_ROLL_CMD						0x01
#define NUCLEO_SPI_READ_PITCH_CMD						0x02
#define NUCLEO_SPI_READ_TEMP_CMD						0x03
#define NUCLEO_SPI_WRITE_LED_PATTERN_CMD		0x04
// add commands for PWM...

#endif

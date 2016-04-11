#ifndef NUCLEO_SPI_H
#define NUCLEO_SPI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"
#include "cmsis_os.h"

void NucleoSPI_Init(void);
void NucleoSPI_SendFloat(float val);
uint16_t NucleoSPI_ReadShortValue(void);

void NucleoSPI_RxISR(SPI_HandleTypeDef *hspi);

void NucleoSPI_SetAccelDataready(osTimerId id, uint32_t millisec);
void NucleoSPI_ResetAccelDataready(void);
void NucleoSPI_SetTempDataready(osTimerId id, uint32_t millisec);
void NucleoSPI_ResetTempDataready(void);
void NucleoSPI_SetDoubletap(osTimerId id, uint32_t millisec);
void NucleoSPI_ResetDoubletap(void);

typedef void (*timeout_cb_fn)(void);

/* Nucleo SPI pin defines */
#define NUCLEO_SPI_SCK_PIN               		GPIO_PIN_10                 /* PB.10 */		/* Blue */
#define NUCLEO_SPI_SCK_GPIO_PORT         		GPIOB                       /* GPIOB */

#define NUCLEO_SPI_MISO_PIN              		GPIO_PIN_2                  /* PC.2 */		/* Orange */
#define NUCLEO_SPI_MISO_GPIO_PORT        		GPIOC                       /* GPIOC */

#define NUCLEO_SPI_MOSI_PIN              		GPIO_PIN_3                  /* PC.3 */		/* Green */
#define NUCLEO_SPI_MOSI_GPIO_PORT        		GPIOC                       /* GPIOC */

#define NUCLEO_SPI_DOUBLETAP_PIN						GPIO_PIN_6									/* PC.6 */		/* Yellow */
#define NUCLEO_SPI_DOUBLETAP_GPIO_PORT			GPIOC												/* GPIOC */

#define NUCLEO_SPI_ACCEL_DATAREADY_PIN			GPIO_PIN_8									/* PC.8 */		/* Black */
#define NUCLEO_SPI_ACCEL_DATAREADY_PORT			GPIOC												/* GPIOC */

#define NUCLEO_SPI_TEMP_DATAREADY_PIN				GPIO_PIN_1									/* PC.1 */		/* Gray */
#define NUCLEO_SPI_TEMP_DATAREADY_PORT			GPIOC												/* GPIOC */

//#define NUCLEO_SPI_ERROR_PIN
/* Nucleo SPI command defines */
#define NUCLEO_SPI_READ_ROLL_CMD						0x1111
#define NUCLEO_SPI_READ_PITCH_CMD						0x5555
#define NUCLEO_SPI_READ_TEMP_CMD						0x1234
#define NUCLEO_SPI_WRITE_LED_PATTERN_CMD		0xbeef
#define NUCLEO_SPI_WRITE_LED_DUTYCYCLE_CMD	0xdead

#define PATTERN_CMD_OFF			0x0001
#define PATTERN_CMD_CCW			0x0002
#define PATTERN_CMD_CW			0x0003
#define PATTERN_CMD_PWM			0x0004

/* Timeout for wait */
#define NUCLEO_SPI_TIMEOUT		100000
#define WAIT_FOR_FLAG_UNTIL_TIMEOUT(h,f,v,t) do {int n = (t); while (__HAL_SPI_GET_FLAG((h),(f)) == (v)) {if (!--n){NucleoSPI_ResetAccelDataready();NucleoSPI_ResetTempDataready(); goto timeout;}}} while (0)

#endif

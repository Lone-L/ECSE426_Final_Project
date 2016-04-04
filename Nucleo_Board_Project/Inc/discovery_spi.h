#ifndef DISCOVERY_SPI_H
#define DISCOVERY_SPI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"

void DiscoverySPI_Init(void);
float DiscoverySPI_ReadFloatValue();

void DebugSPI(uint8_t byte);

#define DISCOVERY_SPI_RCC_CLK_ENABLE()				 __HAL_RCC_SPI2_CLK_ENABLE();

// SPI Instance
#define DISCOVERY_SPI_INSTANCE          			 SPI2
#define DISCOVERY_SPI_CLK_ENABLE()      			 __SPI2_CLK_ENABLE()

/* Discovery SPI pin defines */

/* see http://stm32f4-discovery.com/2014/04/library-05-spi-for-stm32f4xx/ */
/* SCK Pin: PB.10 (Blue) */
#define DISCOVERY_SPI_SCK_PIN               		GPIO_PIN_10
#define DISCOVERY_SPI_SCK_MODE									GPIO_MODE_AF_PP
#define DISCOVERY_SPI_SCK_PULL									GPIO_PULLDOWN
#define DISCOVERY_SPI_SCK_SPEED									GPIO_SPEED_HIGH
#define DISCOVERY_SPI_SCK_ALTERNATE							GPIO_AF5_SPI2
#define DISCOVERY_SPI_SCK_GPIO_PORT         		GPIOB
#define DISCOVERY_SPI_SCK_CLK_ENABLE()					__GPIOB_CLK_ENABLE()

/* MISO Pin: PC.2 (Orange) */
#define DISCOVERY_SPI_MISO_PIN               		GPIO_PIN_2
#define DISCOVERY_SPI_MISO_MODE									GPIO_MODE_AF_PP
#define DISCOVERY_SPI_MISO_PULL									GPIO_NOPULL
#define DISCOVERY_SPI_MISO_SPEED								GPIO_SPEED_HIGH
#define DISCOVERY_SPI_MISO_ALTERNATE						GPIO_AF5_SPI2
#define DISCOVERY_SPI_MISO_GPIO_PORT         		GPIOC
#define DISCOVERY_SPI_MISO_CLK_ENABLE()					__GPIOC_CLK_ENABLE()

/* MOSI Pin: PC.3 (Green) */
#define DISCOVERY_SPI_MOSI_PIN               		GPIO_PIN_3
#define DISCOVERY_SPI_MOSI_MODE									GPIO_MODE_AF_PP
#define DISCOVERY_SPI_MOSI_PULL									GPIO_NOPULL
#define DISCOVERY_SPI_MOSI_SPEED								GPIO_SPEED_HIGH
#define DISCOVERY_SPI_MOSI_ALTERNATE						GPIO_AF5_SPI2
#define DISCOVERY_SPI_MOSI_GPIO_PORT         		GPIOC
#define DISCOVERY_SPI_MOSI_CLK_ENABLE()					__GPIOC_CLK_ENABLE()

/* DOUBLETAP Pin: PB.3 (Yellow) */
#define DISCOVERY_SPI_DOUBLETAP_PIN             GPIO_PIN_3
#define DISCOVERY_SPI_DOUBLETAP_MODE						GPIO_MODE_IT_RISING
#define DISCOVERY_SPI_DOUBLETAP_PULL						GPIO_NOPULL
#define DISCOVERY_SPI_DOUBLETAP_SPEED						GPIO_SPEED_HIGH
#define DISCOVERY_SPI_DOUBLETAP_ALTERNATE				0
#define DISCOVERY_SPI_DOUBLETAP_PORT       			GPIOB
#define DISCOVERY_SPI_DOUBLETAP_CLK_ENABLE()		__GPIOB_CLK_ENABLE()

/* DEBUG Pin: PB.2 (Black) */
#define DISCOVERY_SPI_DEBUG_PIN             		GPIO_PIN_2
#define DISCOVERY_SPI_DEBUG_MODE								GPIO_MODE_OUTPUT_PP
#define DISCOVERY_SPI_DEBUG_PULL								GPIO_NOPULL
#define DISCOVERY_SPI_DEBUG_SPEED								GPIO_SPEED_HIGH
#define DISCOVERY_SPI_DEBUG_ALTERNATE						0
#define DISCOVERY_SPI_DEBUG_PORT       					GPIOB
#define DISCOVERY_SPI_DEBUG_CLK_ENABLE()				__GPIOB_CLK_ENABLE()

/* Discovery SPI IRQn defines */
/* Do not use EXTI0 and EXTI15_10 as they are used by the BlueNRG and push-buttons. */
/* Doubletap interrupt */
#define DISCOVERY_SPI_DOUBLETAP_IRQn									EXTI3_IRQn
#define DISCOVERY_SPI_DOUBLETAP_EXTI_IRQHandler				EXTI3_IRQHandler

/* SPI RXNE interrupt */
#define DISCOVERY_SPI_IRQn														SPI2_IRQn
#define DISCOVERY_SPI_IRQHandler											SPI2_IRQHandler

#define DISCOVERY_SPI_ACCEL_ROLL_TYPE				0x01
#define DISCOVERY_SPI_ACCEL_PITCH_TYPE			0x02
#define DISCOVERY_SPI_TEMP_TYPE							0x03

#endif

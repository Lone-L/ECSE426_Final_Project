#ifndef NUCLEO_SPI_H
#define NUCLEO_SPI_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"

void NucleoSPI_Init(void);

#define NUCLEO_SPI_SCK_PIN               GPIO_PIN_1                  /* PC.1 */
#define NUCLEO_SPI_SCK_GPIO_PORT         GPIOC                       /* GPIOC */

#define NUCLEO_SPI_MISO_PIN              GPIO_PIN_2                  /* PC.2 */
#define NUCLEO_SPI_MISO_GPIO_PORT        GPIOC                       /* GPIOC */

#define NUCLEO_SPI_MOSI_PIN              GPIO_PIN_4                  /* PC.4 */
#define NUCLEO_SPI_MOSI_GPIO_PORT        GPIOC                       /* GPIOC */

#endif

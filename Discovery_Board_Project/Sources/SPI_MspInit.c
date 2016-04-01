#include "lis3dsh.h"
#include "nucleo_spi.h"

extern SPI_HandleTypeDef    SpiHandle;				/* SPI handle for LIS3DSH driver. */
extern SPI_HandleTypeDef		nucleo_SpiHandle;	/* SPI handle for Nucleo board. */

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if (hspi == &SpiHandle) {
		/* Enable the SPI periph */
		__SPI1_CLK_ENABLE();

		/* Enable SCK, MOSI and MISO GPIO clocks */
		__GPIOA_CLK_ENABLE();

		/* Enable CS, INT1, INT2  GPIO clock */
		__GPIOE_CLK_ENABLE();

		//GPIO_PinAFConfig(LIS3DSH_SPI_SCK_GPIO_PORT, LIS3DSH_SPI_SCK_SOURCE, LIS3DSH_SPI_SCK_AF);
		//GPIO_PinAFConfig(LIS3DSH_SPI_MISO_GPIO_PORT, LIS3DSH_SPI_MISO_SOURCE, LIS3DSH_SPI_MISO_AF);
		//GPIO_PinAFConfig(LIS3DSH_SPI_MOSI_GPIO_PORT, LIS3DSH_SPI_MOSI_SOURCE, LIS3DSH_SPI_MOSI_AF);

		GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
		GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
		GPIO_InitStructure.Alternate = GPIO_AF5_SPI1;

		/* SPI SCK pin configuration */
		GPIO_InitStructure.Pin = LIS3DSH_SPI_SCK_PIN;
		HAL_GPIO_Init(LIS3DSH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

		/* SPI  MOSI pin configuration */
		GPIO_InitStructure.Pin =  LIS3DSH_SPI_MOSI_PIN;
		HAL_GPIO_Init(LIS3DSH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

		/* SPI MISO pin configuration */
		GPIO_InitStructure.Pin = LIS3DSH_SPI_MISO_PIN;
		HAL_GPIO_Init(LIS3DSH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
		
		GPIO_InitStructure.Pin   = LIS3DSH_SPI_CS_PIN;
		GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(LIS3DSH_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

		/* Deselect : Chip Select high */
		HAL_GPIO_WritePin(LIS3DSH_SPI_CS_GPIO_PORT, LIS3DSH_SPI_CS_PIN, GPIO_PIN_SET);

		/* Configure GPIO PINs to detect Interrupts */
		GPIO_InitStructure.Pin   = LIS3DSH_SPI_INT1_PIN;
		GPIO_InitStructure.Mode  = GPIO_MODE_IT_FALLING;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(LIS3DSH_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.Pin = LIS3DSH_SPI_INT2_PIN;
		HAL_GPIO_Init(LIS3DSH_SPI_INT2_GPIO_PORT, &GPIO_InitStructure);
	} else if (hspi == &nucleo_SpiHandle) {
		/* Enable the SPI periph */
		__SPI2_CLK_ENABLE();

		/* Enable SCK, MOSI and MISO GPIO clocks */
		__GPIOC_CLK_ENABLE();

		GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull  = GPIO_PULLDOWN;
		GPIO_InitStructure.Speed = GPIO_SPEED_MEDIUM;
		GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;

		/* SPI SCK pin configuration */
		GPIO_InitStructure.Pin = NUCLEO_SPI_SCK_PIN;
		HAL_GPIO_Init(NUCLEO_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

		/* SPI  MOSI pin configuration */
		GPIO_InitStructure.Pin =  NUCLEO_SPI_MOSI_PIN;
		HAL_GPIO_Init(NUCLEO_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

		/* SPI MISO pin configuration */
		GPIO_InitStructure.Pin = NUCLEO_SPI_MISO_PIN;
		HAL_GPIO_Init(NUCLEO_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	}
}

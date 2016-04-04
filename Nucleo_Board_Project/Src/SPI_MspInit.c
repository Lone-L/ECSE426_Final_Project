#include "stm32_bluenrg_ble.h"
#include "discovery_spi.h"

extern SPI_HandleTypeDef discovery_SpiHandle;

/**
 * @brief  This function is used for low level initialization of the SPI 
 *         communication with the BlueNRG Expansion Board and the Discovery Board.
 * @param  hspi: SPI handle.
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	
  if(hspi->Instance==BNRG_SPI_INSTANCE)
	{
    /* Enable peripherals clock */
    /* Enable GPIO Ports Clock */  
    BNRG_SPI_RESET_CLK_ENABLE();
    BNRG_SPI_SCLK_CLK_ENABLE();
    BNRG_SPI_MISO_CLK_ENABLE();
    BNRG_SPI_MOSI_CLK_ENABLE();
    BNRG_SPI_CS_CLK_ENABLE();
    BNRG_SPI_IRQ_CLK_ENABLE();

    /* Enable SPI clock */
    BNRG_SPI_CLK_ENABLE();

    /* Reset */
    GPIO_InitStruct.Pin = BNRG_SPI_RESET_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_RESET_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_RESET_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_RESET_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_RESET_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_RESET_PORT, &GPIO_InitStruct);	
    HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);	/*Added to avoid spurious interrupt from the BlueNRG */

    /* SCLK */
    GPIO_InitStruct.Pin = BNRG_SPI_SCLK_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_SCLK_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_SCLK_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_SCLK_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_SCLK_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_SCLK_PORT, &GPIO_InitStruct); 

    /* MISO */
    GPIO_InitStruct.Pin = BNRG_SPI_MISO_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_MISO_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_MISO_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_MISO_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_MISO_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_MISO_PORT, &GPIO_InitStruct);

    /* MOSI */
    GPIO_InitStruct.Pin = BNRG_SPI_MOSI_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_MOSI_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_MOSI_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_MOSI_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_MOSI_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_MOSI_PORT, &GPIO_InitStruct);

    /* NSS/CSN/CS */
    GPIO_InitStruct.Pin = BNRG_SPI_CS_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_CS_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_CS_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_CS_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_CS_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_CS_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);

    /* IRQ -- INPUT */
    GPIO_InitStruct.Pin = BNRG_SPI_IRQ_PIN;
    GPIO_InitStruct.Mode = BNRG_SPI_IRQ_MODE;
    GPIO_InitStruct.Pull = BNRG_SPI_IRQ_PULL;
    GPIO_InitStruct.Speed = BNRG_SPI_IRQ_SPEED;
    GPIO_InitStruct.Alternate = BNRG_SPI_IRQ_ALTERNATE;
    HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStruct);

    /* Configure the NVIC for SPI */  
    HAL_NVIC_SetPriority(BNRG_SPI_EXTI_IRQn, 3, 0);    
    HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);
  } else if (hspi->Instance == DISCOVERY_SPI_INSTANCE) {
    /* Enable peripherals clock */
    /* Enable GPIO Ports Clock */ 
		DISCOVERY_SPI_SCK_CLK_ENABLE();
		DISCOVERY_SPI_MOSI_CLK_ENABLE();
		DISCOVERY_SPI_MISO_CLK_ENABLE();
		DISCOVERY_SPI_DOUBLETAP_CLK_ENABLE();
		DISCOVERY_SPI_DEBUG_CLK_ENABLE();
		
		/* Enable SPI clock */
		DISCOVERY_SPI_CLK_ENABLE();
		
		/* Configure SCK pin */
		GPIO_InitStruct.Pin		= DISCOVERY_SPI_SCK_PIN;
		GPIO_InitStruct.Mode  = DISCOVERY_SPI_SCK_MODE;
		GPIO_InitStruct.Pull  = DISCOVERY_SPI_SCK_PULL;
		GPIO_InitStruct.Speed = DISCOVERY_SPI_SCK_SPEED;
		GPIO_InitStruct.Alternate = DISCOVERY_SPI_SCK_ALTERNATE;
		HAL_GPIO_Init(DISCOVERY_SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

		/* Configure MOSI pin */
		GPIO_InitStruct.Pin		= DISCOVERY_SPI_MOSI_PIN;
		GPIO_InitStruct.Mode  = DISCOVERY_SPI_MOSI_MODE;
		GPIO_InitStruct.Pull  = DISCOVERY_SPI_MOSI_PULL;
		GPIO_InitStruct.Speed = DISCOVERY_SPI_MOSI_SPEED;
		GPIO_InitStruct.Alternate = DISCOVERY_SPI_MOSI_ALTERNATE;
		HAL_GPIO_Init(DISCOVERY_SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);

		/* Configure MISO pin */
		GPIO_InitStruct.Pin		= DISCOVERY_SPI_MISO_PIN;
		GPIO_InitStruct.Mode  = DISCOVERY_SPI_MISO_MODE;
		GPIO_InitStruct.Pull  = DISCOVERY_SPI_MISO_PULL;
		GPIO_InitStruct.Speed = DISCOVERY_SPI_MISO_SPEED;
		GPIO_InitStruct.Alternate = DISCOVERY_SPI_MISO_ALTERNATE;
		HAL_GPIO_Init(DISCOVERY_SPI_MISO_GPIO_PORT, &GPIO_InitStruct);
		
		/* DOUBLETAP pin configuration */
		GPIO_InitStruct.Pin   = DISCOVERY_SPI_DOUBLETAP_PIN;
		GPIO_InitStruct.Mode  = DISCOVERY_SPI_DOUBLETAP_MODE;
		GPIO_InitStruct.Pull	= DISCOVERY_SPI_DOUBLETAP_PULL;
		GPIO_InitStruct.Speed = DISCOVERY_SPI_DOUBLETAP_SPEED;
		GPIO_InitStruct.Alternate = DISCOVERY_SPI_DOUBLETAP_ALTERNATE;
		HAL_GPIO_Init(DISCOVERY_SPI_DOUBLETAP_PORT, &GPIO_InitStruct);
		
		/* DEBUG pin configuration */
		GPIO_InitStruct.Pin   = DISCOVERY_SPI_DEBUG_PIN;
		GPIO_InitStruct.Mode  = DISCOVERY_SPI_DEBUG_MODE;
		GPIO_InitStruct.Pull	= DISCOVERY_SPI_DEBUG_PULL;
		GPIO_InitStruct.Speed = DISCOVERY_SPI_DEBUG_SPEED;
		GPIO_InitStruct.Alternate = DISCOVERY_SPI_DEBUG_ALTERNATE;
		HAL_GPIO_Init(DISCOVERY_SPI_DEBUG_PORT, &GPIO_InitStruct);
		
		/* Enable interrupts from Discovery board. */
		HAL_NVIC_SetPriority(DISCOVERY_SPI_DOUBLETAP_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DISCOVERY_SPI_DOUBLETAP_IRQn);	
	}
}

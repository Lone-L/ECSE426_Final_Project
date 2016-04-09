#include "stm32f4xx_hal.h"
#include "LED.h"

static int led = 2;
extern TIM_HandleTypeDef timmy4;
static int LEDS[] = {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_12};
TIM_OC_InitTypeDef OC_CONFIG;
int LED_STATE = 1;
int IS_PWM_ON = 0;
int INIT_ALTERNATING_PWM = 0;
/**
   * @brief sets the duty cycle for particular channel.
   * @param duty_cycle : the PWM duty cycle in percentage (eg. 10, 20, 50, 100)
   * @retval None
   */

void LED_SET_DUTY_CYCLE(int duty_cycle)
{
	OC_CONFIG.Pulse = (int)(duty_cycle * PERIOD / 100);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_1);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_2);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_3);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_4);
}

/**
   * @brief Turns on PWM.
   * @param None
   * @retval None
   */
void LED_ON_PWM(void)
{
	HAL_TIM_PWM_Start(&timmy4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&timmy4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&timmy4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&timmy4,TIM_CHANNEL_4);
}
/**
   * @brief Stops PWM.
   * @param None
   * @retval None
   */
void LED_OFF_PWM(void)
{
	HAL_TIM_PWM_Stop(&timmy4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&timmy4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&timmy4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&timmy4,TIM_CHANNEL_4);
}
/**
   * @brief Turns on LEDS in counter clockwise direction.
   * @param None
   * @retval None
   */
void LED_CCW(void)
{
	LED_OFF();
	GPIOD->ODR = GPIOD->ODR | LEDS[led];
	led = (led + 1) % 4;
}
/**
   * @brief Turns on LEDS in clockwise direction.
   * @param None
   * @retval None
   */
void LED_CW(void)
{
	LED_OFF();
	GPIOD->ODR = GPIOD->ODR | LEDS[led];
	led = (led - 1) % 4;
}

/**
   * @brief Turns on all LEDs.
   * @param None
   * @retval None
   */
void LED_ON(void)
{
	GPIOD->ODR = GPIOD->ODR | 0xF000;	/* Put high bits 12 to 15 */
}
/**
   * @brief Turns off all LEDs.
   * @param None
   * @retval None
   */
void LED_OFF(void)
{
	GPIOD->ODR = GPIOD->ODR & 0x0FFF;	/* Clears bits 12 to 15 */
}
void init_PWM(void)
{
	
	
	OC_CONFIG.OCMode = TIM_OCMODE_PWM2; //set pulse to 1 when count is equal to CCRx 
	OC_CONFIG.OCIdleState = TIM_OCIDLESTATE_SET;
	OC_CONFIG.Pulse = PULSE; // ?
	OC_CONFIG.OCPolarity = TIM_OCPOLARITY_LOW;  //
	OC_CONFIG.OCFastMode = TIM_OCFAST_ENABLE;    // ?
	
	HAL_TIM_OC_Init(&timmy4);
	
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_1);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_2);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_3);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_4);
}

void init_GPIO_PWM(void)
{
	GPIO_InitTypeDef			LED_PINS_D;

	__GPIOD_CLK_ENABLE();

	LED_PINS_D.Pin	 = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 				/* LEDs are LD3:PD13, LD4:PD12, LD5:PD14, LD6:PD15 */
	LED_PINS_D.Mode  = GPIO_MODE_AF_PP;     												/* Push-Pull Alternating mode */
	LED_PINS_D.Pull  = GPIO_NOPULL;          												/* No pullup/pulldown*/
	LED_PINS_D.Speed = GPIO_SPEED_FREQ_HIGH;  												/* 100 MHz */
	LED_PINS_D.Alternate = GPIO_AF2_TIM4;													/*	TIM4 GPIO Configuration PD12 -> TIM4_CH1  | PD13 ----> TIM4_CH2  | PD14 ----> TIM4_CH3  | PD15 ----> TIM4_CH4  | */

	/* Initialize GPIO pins configuration */
	HAL_GPIO_Init(GPIOD, &LED_PINS_D);

}


void init_GPIO_NO_PWM(void)
{
	GPIO_InitTypeDef			LED_PINS_D;

	__GPIOD_CLK_ENABLE();

	LED_PINS_D.Pin	 = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 				/* LEDs are LD3:PD13, LD4:PD12, LD5:PD14, LD6:PD15 */
	LED_PINS_D.Mode  = GPIO_MODE_OUTPUT_PP;     												/* Push-Pull Alternating mode */
	LED_PINS_D.Pull  = GPIO_NOPULL;          												/* No pullup/pulldown*/
	LED_PINS_D.Speed = GPIO_SPEED_FREQ_HIGH;  												/* 100 MHz */
	
	/* Initialize GPIO pins configuration */
	HAL_GPIO_Init(GPIOD, &LED_PINS_D);

}
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	switch (LED_STATE)
	{
		case ALL_ON_PWM_ON:
			
			break;
		case CCW_NO_PWM:
			
		case CW_NO_PWM:
			
		case ALL_OFF:
			if (!PWM_IS_ON) {
				LED_OFF();
			}
			else 
			{
				if (!INIT_ALTERNATING_PWN)
				{
					
				}
			}
			
	}
}
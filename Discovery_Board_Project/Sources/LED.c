#include "stm32f4xx_hal.h"
#include "LED.h"

extern TIM_HandleTypeDef timmy4;
TIM_OC_InitTypeDef OC_CONFIG;
static int led = 2;
static int LEDS[] = {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_12};
static int LED_STATE = ALL_ON_PWM_ON;			//currently there are 4 different states, CW, CCW, ALL ON PWM, ALL OFF
static int IS_PWM_ON = 0; 						//used to indicate whether PWM is started or stopped
static int CURRENT_GPIO_MODE = 0;				//used to indicate whether GPIO's are currently initialized to alternate function (PWM) or PP mode (DC to drive CCW and CW LED)
static int pulse = (int)PERIOD/2;				//used to set the duty cycle

/**
   * @brief toggles IS_PWM_ON,
   * @param 
   * @retval None
   */
void toggle_pwm_flag(void)
{
	IS_PWM_ON = ~IS_PWM_ON;
}


/**
   * @brief TOGGLE GPIO MODE assuming there are only two modes (when its driving the LED via PWM or none PWM)
   * @param 
   * @retval None
   */
void toggle_gpio_mode_flag(void)
{
	CURRENT_GPIO_MODE = ~CURRENT_GPIO_MODE;
}

/**
   * @brief set LED state
   * @param state : can be any of states 1, 2, 3, 4
   * @retval None
   */
void set_led_state(int state)
{
	LED_STATE = state;
}

/**
   * @brief sets the duty cycle for particular channel.
   * @param duty_cycle : the PWM duty cycle in percentage (eg. 10, 20, 50, 100)
   * @retval None
   */

void LED_SET_DUTY_CYCLE(int duty_cycle)
{
	if (!CURRENT_GPIO_MODE)
	{
		toggle_gpio_mode();
		init_GPIO_PWM();
	}
	pulse = (int)(duty_cycle * PERIOD / 100);
	OC_CONFIG.Pulse = pulse;
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
	if (!PWM_IS_ON) toggle_pwm_flag();
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
	if (PWM_IS_ON) toggle_pwm_flag();
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

/**
   * @brief Configures the PWM parameters
   * @param None
   * @retval None
   */
void init_PWM(void)
{
	
	
	OC_CONFIG.OCMode = TIM_OCMODE_PWM2; //set pulse to 1 when count is equal to CCRx 
	OC_CONFIG.OCIdleState = TIM_OCIDLESTATE_SET;
	OC_CONFIG.Pulse = pulse; // ?
	OC_CONFIG.OCPolarity = TIM_OCPOLARITY_LOW;  //
	OC_CONFIG.OCFastMode = TIM_OCFAST_ENABLE;    // ?
	
	HAL_TIM_OC_Init(&timmy4);
	
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_1);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_2);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_3);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_4);
}

/**
   * @brief Initializes the GPIO PWM pins
   * @param None
   * @retval None
   */
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

/**
   * @brief Initializes the GPIO PP pins 
   * @param None
   * @retval None
   */
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


/**
   * @brief Callback function to process TIM4 interrupts caused due the compare function (not sure if this is the right callback)
   * @param None
   * @retval None
   */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	switch (LED_STATE)
	{
		case ALL_ON_PWM_ON:
			if (CURRENT_GPIO_MODE == GPIO_PP_MODE)
			{
				toggle_gpio_mode_flag();
				init_GPIO_PWM();
				LED_ON_PWM();
			}

			break;
		case CCW_NO_PWM:
			if (CURRENT_GPIO_MODE == GPIO_ALTERNATE_MODE)
			{
				LED_OFF_PWM();
				toggle_gpio_mode_flag();
				init_GPIO_NO_PWM();
			}

			LED_CCW();
			break;
		case CW_NO_PWM:
			if (CURRENT_GPIO_MODE == GPIO_ALTERNATE_MODE)
			{
				toggle_gpio_mode_flag();
				init_GPIO_NO_PWM(;)
			}
			LED_CW();
			break;
		case ALL_OFF:
			if (PWM_IS_ON) {
				LED_OFF_PWM();
			}
				LED_OFF();
	
	}
}
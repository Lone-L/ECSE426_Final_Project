#include "stm32f4xx_hal.h"
#include "LED.h"

extern TIM_HandleTypeDef timmy4;
TIM_OC_InitTypeDef OC_CONFIG;
static int led = 2;
static int LEDS[] = {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_12};
static int LED_STATE = ALL_OFF;								//currently there are 4 different states, CW, CCW, ALL ON PWM, ALL OFF
static int IS_PWM_ON = 0; 										//used to indicate whether PWM is started or stopped
static int CURRENT_GPIO_MODE = GPIO_PP_MODE;	//used to indicate whether GPIO's are currently initialized to alternate function (PWM) or PP mode (DC to drive CCW and CW LED)
static int pulse = (int)PERIOD/2;							//used to set the duty cycle
static int ROTATION_COUNT = DEFAULT_ROTATION_COUNT;

/**
   * @brief toggles IS_PWM_ON,
   * @param 
   * @retval None
   */
void toggle_PWM_flag(void)
{
	if (IS_PWM_ON == 1) 
	{
		IS_PWM_ON = 0;
	}
	else
	{
		IS_PWM_ON = 1;
	}
}


/**
   * @brief TOGGLE GPIO MODE assuming there are only two modes (when its driving the LED via PWM or none PWM)
   * @param 
   * @retval None
   */
void toggle_GPIO_mode_flag(void)
{
	if (CURRENT_GPIO_MODE == GPIO_PP_MODE)
		CURRENT_GPIO_MODE = GPIO_ALTERNATE_MODE;
	else
		CURRENT_GPIO_MODE = GPIO_PP_MODE;
}

/**
   * @brief set LED state
   * @param state : can be any of states 1, 2, 3, 4
   * @retval None
   */
void set_LED_state(int state)
{
	LED_STATE = state;
}

/**
   * @brief sets the duty cycle for particular channel.
   * @param duty_cycle : the PWM duty cycle in percentage (eg. 10, 20, 50, 100)
   * @retval None
   */

void LED_set_duty_cycle(int duty_cycle)
{
	if (duty_cycle > 100)
		duty_cycle = 100;
	
	pulse = (int)(duty_cycle * PERIOD / 100);
	OC_CONFIG.Pulse = pulse;
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_1);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_2);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_3);
	HAL_TIM_OC_ConfigChannel(&timmy4, &OC_CONFIG, TIM_CHANNEL_4);
}

void LED_set_speed(uint16_t speed)
{
	if (speed >= 100)
		speed = 99;
	if (speed <= 50)
		speed = 100 - speed;
	
	ROTATION_COUNT = (110-speed);
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
	if (!IS_PWM_ON) toggle_PWM_flag();
}
/**
   * @brief Stops PWM.
   * @param None
   * @retval None
   */
void LED_OFF_PWM(void)
{
	/* this also stops the hardware timer... :( */
	HAL_TIM_PWM_Stop(&timmy4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&timmy4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&timmy4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&timmy4,TIM_CHANNEL_4);
	if (IS_PWM_ON) toggle_PWM_flag();
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
	led = (led + 3) % 4;
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
	led = (led + 1) % 4;
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
	static int counter;
//	static uint8_t delay = ROTATION_COUNT;
//	static uint16_t yolo = 123;

	switch (LED_STATE)
	{
		case ALL_ON_PWM_ON:
			if (CURRENT_GPIO_MODE == GPIO_PP_MODE)
			{
				toggle_GPIO_mode_flag();
				init_GPIO_PWM();
				LED_ON_PWM();
			}

			break;
		case CCW_NO_PWM:
			if (CURRENT_GPIO_MODE == GPIO_ALTERNATE_MODE)
			{
				toggle_GPIO_mode_flag();
				init_GPIO_NO_PWM();
				counter = 0;
			}

			if (counter == 0)
				LED_CCW();
			

			counter = (counter + 1) % ROTATION_COUNT;
//			if (counter == 0) {
//					delay = (delay - 1) % ROTATION_COUNT;
//					if (delay == 2) {
//						if (yolo > 0) {
//							yolo--;
//							delay += 1;
//						} else {
//							yolo = 123;
//						}
//					}
//				
//					if (delay == 1) {
//						delay = ROTATION_COUNT;
//					}
//			}
			
			break;
		case CW_NO_PWM:
			if (CURRENT_GPIO_MODE == GPIO_ALTERNATE_MODE)
			{
				toggle_GPIO_mode_flag();
				init_GPIO_NO_PWM();
				counter = 0;
			}

			if (counter == 0)
				LED_CW();
			
			counter = (counter + 1) % ROTATION_COUNT;
			break;
		case ALL_OFF:
			if (IS_PWM_ON) {
				/* because otherwise PWM doesn't turn off :( */
				toggle_GPIO_mode_flag();
				init_GPIO_NO_PWM();
			}
			
			LED_OFF();
			break;
		default:
			break;
	}
}

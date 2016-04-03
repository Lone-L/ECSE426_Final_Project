#include "stm32f4xx_hal.h"
#include "LED.h"

static int led = 0;
static TIM_HandleTypeDef htim4;
static int LEDS[] = {GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_12};
static int pwm_duty_cycle = 0;

/**
   * @brief Sets pwm duty cycle.
   * @param None
   * @retval None
   */
void LED_SET_DUTY_CYCLE(int val)
{
	pwm_duty_cycle = val;
}
/**
   * @brief Turns on PWM.
   * @param None
   * @retval None
   */
void LED_ON_PWM(void)
{
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_ALL);
}
/**
   * @brief Stops PWM.
   * @param None
   * @retval None
   */
void LED_OFF_PWM(void)
{
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_ALL);
}
/**
   * @brief Turns on LEDS in counter clockwise direction.
   * @param None
   * @retval None
   */
void LED_CCW(void)
{
	LED_OFF_PWN();
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
	LED_OFF_PWN();
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
void init_tim3(void)
{

	M_MasterConfigTypeDef MASTER_CONFIG;
	TIM_OC_InitTypeDef OC_CONFIG;


	/*
		Desired_timer_frequency = Timer_input_frequency / (prescaler * period)
		Timer_input_frequency = 84 MHz
		Desired_timer_frequency = 800 Hz (switch LED at this frequency to reduce flickering)
		---> prescaler * period = 84 MHz / 800 Hz = 105000 = 21 * 5000
		---> prescaler = 21, period = 5000 (our choice, man) 
	*/

	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 21;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP; //count up mode
	htim4.Init.Period = 5000 - 1;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // don't know what this do?
	HAL_TIM_Base_Start(&htim4); 


	OC_CONFIG.OCMode = TIM_OCMODE_PWM1; //set pulse to 1 when CONT == CCRx
	OC_CONFIG.OCIdleState = TIM_OCIDLESTATE_SET; 
	OC_CONFIG.Pulse = 0; //initial duty cycle set to 0
	OC_CONFIG.OCPolarity = TIM_OCPOLARITY_HIGH;  //
	OC_CONFIG.OCFastMode = TIM_OCFAST_ENABLE;    // ?

	HAL_TIM_PWM_ConfigChannel(&htim4, &OC_CONFIG, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htim4, &OC_CONFIG, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htim4, &OC_CONFIG, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htim4, &OC_CONFIG, TIM_CHANNEL_4);
	
	//HAL_TIM_PWM_MspInit(&htim4);
	HAL_TIM_PWM_Init(&htim4);
	
}

void init_GPIO_PWM(void)
{
	GPIO_InitTypeDef			LED_PINS_D;

	LED_PINS_D.Pin	 = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 				/* LEDs are LD3:PD13, LD4:PD12, LD5:PD14, LD6:PD15 */
	LED_PINS_D.Mode  = GPIO_MODE_AF_PP;     												/* Push-Pull Alternating mode */
	LED_PINS_D.Pull  = GPIO_NOPULL;          												/* No pullup/pulldown*/
	LED_PINS_D.Speed = GPIO_SPEED_FREQ_HIGH;  												/* 100 MHz */
	LEF_PINS_D.Aleternate = GPIO_AF2_Tim4;													/*	TIM4 GPIO Configuration    
																							    PD12 ----> TIM4_CH1
																							    PD13 ----> TIM4_CH2
																							    PD14 ----> TIM4_CH3
																							    PD15 ----> TIM4_CH4
																							    */

	/* Initialize GPIO pins configuration */
	HAL_GPIO_Init(GPIOD, &LED_PINS_D);

}


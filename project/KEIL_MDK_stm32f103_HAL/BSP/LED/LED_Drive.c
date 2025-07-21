/**
 ******************************************************************************
 * @file    LED_Hard.c
 * @author  LQ
 * @version V3.0
 * @date    2024-11-12
 * @brief   GPIO驱动LED
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */
#include "LED_Drive.h"

/**
 * @brief  初始化LED的硬件GPIO接口
 *
 * 本函数旨在配置和初始化指定的GPIO端口和引脚，以驱动LED硬件。
 * 它首先使能相应的GPIO时钟，然后配置GPIO引脚为推挽输出模式。
 *
 * @param GPIO GPIO端口，例如GPIOC
 * @param PIN 需要初始化的GPIO引脚，例如GPIO_Pin_13
 */
void LED_Hard_GPIO_Init(GPIO_PORT_T GPIO, GPIO_PIN_T PIN)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin : PtPin */
	GPIO_InitStruct.Pin = PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIO, &GPIO_InitStruct);
}

void LED_Hard_Write_Pin(GPIO_PORT_T GPIO, GPIO_PIN_T PIN, uint8_t v)
{
	HAL_GPIO_WritePin(GPIO, PIN, (GPIO_PinState)v);
}

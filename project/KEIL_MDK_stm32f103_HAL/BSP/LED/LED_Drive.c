/**
 ******************************************************************************
 * @file    LED_Hard.c
 * @author  LQ
 * @version V3.0
 * @date    2024-11-12
 * @brief   GPIO����LED
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */
#include "LED_Drive.h"

/**
 * @brief  ��ʼ��LED��Ӳ��GPIO�ӿ�
 *
 * ������ּ�����úͳ�ʼ��ָ����GPIO�˿ں����ţ�������LEDӲ����
 * ������ʹ����Ӧ��GPIOʱ�ӣ�Ȼ������GPIO����Ϊ�������ģʽ��
 *
 * @param GPIO GPIO�˿ڣ�����GPIOC
 * @param PIN ��Ҫ��ʼ����GPIO���ţ�����GPIO_Pin_13
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

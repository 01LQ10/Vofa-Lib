/**
 ******************************************************************************
 * @file    LED.c
 * @author  LQ
 * @version V3.0
 * @date    2024-11-12
 * @brief   GPIO����LED
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#include "LED.h"

LED_t LED_PC13;

/**
 * ��ʼ��LEDģ��
 * 
 * @param LED LEDģ��Ľṹ��ָ�룬���ڴ洢LED���������
 * @param port LED���ӵ�GPIO�˿�
 * @param pin LED���ӵ�GPIO����
 * @param light_effect LED������Ч������һ��ö������
 * 
 * �˺�����LEDģ����г�ʼ������������GPIO��ʼ��������д������״̬�ĺ�����GPIO�˿ں����š��Լ�����Ч��
 * ���������GPIO��ʼ����������Ĭ�Ϲر�LED
 */
void LED_Init(LED_t *LED, GPIO_PORT_T port, GPIO_PIN_T pin, LED_Light_t light_effect)
{
    LED->GPIO_Init = LED_Hard_GPIO_Init;
    LED->Write_Pin = LED_Hard_Write_Pin;
    LED->GPIO_PORT = port;
    LED->GPIO_PIN = pin;
    LED->Light_effect = light_effect;

    LED->GPIO_Init(port, pin);
    LED_OFF(LED);
}

void LED_ON(LED_t *LED)
{
    LED->state = LED_STA_ON;
    if (0 == LED->Light_effect)
        LED->Write_Pin(LED->GPIO_PORT, LED->GPIO_PIN, 0);
    else
        LED->Write_Pin(LED->GPIO_PORT, LED->GPIO_PIN, 1);
}

void LED_OFF(LED_t *LED)
{
    LED->state = LED_STA_OFF;
    if (0 == LED->Light_effect)
        LED->Write_Pin(LED->GPIO_PORT, LED->GPIO_PIN, 1);
    else
        LED->Write_Pin(LED->GPIO_PORT, LED->GPIO_PIN, 0);
}

void LED_Toggle(LED_t *LED)
{
    if (LED->state == LED_STA_OFF)
    {
        LED_ON(LED);
        LED->state = LED_STA_ON;
    }
    else
    {
        LED_OFF(LED);
        LED->state = LED_STA_OFF;
    }
}

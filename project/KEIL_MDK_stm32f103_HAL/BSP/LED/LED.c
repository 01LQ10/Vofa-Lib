/**
 ******************************************************************************
 * @file    LED.c
 * @author  LQ
 * @version V3.0
 * @date    2024-11-12
 * @brief   GPIO驱动LED
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#include "LED.h"

LED_t LED_PC13;

/**
 * 初始化LED模块
 * 
 * @param LED LED模块的结构体指针，用于存储LED的相关配置
 * @param port LED连接的GPIO端口
 * @param pin LED连接的GPIO引脚
 * @param light_effect LED的亮灯效果，是一个枚举类型
 * 
 * 此函数对LED模块进行初始化，包括设置GPIO初始化函数、写入引脚状态的函数、GPIO端口和引脚、以及亮灯效果
 * 它还会调用GPIO初始化函数，并默认关闭LED
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

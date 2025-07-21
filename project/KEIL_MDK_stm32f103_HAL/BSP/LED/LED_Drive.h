/**
 ******************************************************************************
 * @file    LED_Drive.h
 * @author  LQ
 * @version V3.0
 * @date    2024-11-12
 * @brief   GPIO驱动LED
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#ifndef _LED_HARD_H
#define _LED_HARD_H

#include "main.h"

/*GPIO 类型定义*/
#define GPIO_PORT_T GPIO_TypeDef*
#define GPIO_PIN_T  uint16_t

//typedef unsigned char uint8_t;
//typedef unsigned short uint16_t;

void LED_Hard_GPIO_Init(GPIO_PORT_T GPIO, GPIO_PIN_T PIN);
void LED_Hard_Write_Pin(GPIO_PORT_T GPIO, GPIO_PIN_T PIN, uint8_t v);




#endif

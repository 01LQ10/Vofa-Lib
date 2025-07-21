/**
 ******************************************************************************
 * @file    LED.h
 * @author  LQ
 * @version V3.0
 * @date    2024-11-12
 * @brief   GPIOÇý¶¯LED
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#ifndef _LED_H
#define _LED_H

#include "LED_Drive.h"

typedef enum
{
    LED_STA_OFF = 0,
    LED_STA_ON,
} LED_State_t;

typedef enum
{
    LED_LIGHT_AT_L = 0,
    LED_LIGHT_AT_H,
} LED_Light_t;

typedef struct LED_Struct
{
    void (*GPIO_Init)(GPIO_PORT_T port, GPIO_PIN_T pin);
    void (*Write_Pin)(GPIO_PORT_T port, GPIO_PIN_T pin, uint8_t v);
    LED_State_t (*Read_Pin)(GPIO_PORT_T port, GPIO_PIN_T pin);
    GPIO_PORT_T GPIO_PORT;
    GPIO_PIN_T GPIO_PIN;
    LED_Light_t Light_effect;
    LED_State_t state;
} LED_t;

extern LED_t LED_PC13;

void LED_Init(LED_t *LED, GPIO_PORT_T port, GPIO_PIN_T pin, LED_Light_t Light_effect);
void LED_ON(LED_t *LED);
void LED_OFF(LED_t *LED);
void LED_Toggle(LED_t *LED);

#endif

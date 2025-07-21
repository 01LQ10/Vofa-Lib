/**
 ******************************************************************************
 * @file    LED.h
 * @author  LQ
 * @version V3.0
 * @date    2024-11-12
 * @brief   GPIO����LED
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#ifndef _MYBUTTON_H
#define _MYBUTTON_H

#include "main.h"
#include "multi_button.h"
#include "MyButton_task.h"

typedef enum
{
    BUTTON_ID_OK = 0,
    BUTTON_ID_UP,
    BUTTON_ID_DN,
    BUTTON_ID_LF,
    BUTTON_ID_RT,

} button_id_t;


extern struct Button btn[5];
extern float UP_v, DN_v, RT_v, LF_v, OK_v;

#define MyButton_ticks button_ticks
void MyButton_Init(void);

#endif

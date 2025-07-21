/**
 ******************************************************************************
 * @file    MyKEY.c
 * @author  LQ
 * @version V1.0
 * @date    2024-12-16
 * @brief   ����״̬��
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#include "MyButton.h"
#include "main.h"

struct Button btn[5];
float UP_v=0, DN_v=0, RT_v=0, LF_v=0, OK_v=0;

uint8_t BUTN_GPIO_read(uint8_t id)
{
    switch (id)
    {
    case BUTTON_ID_OK:
        return (uint8_t)OK_v;
    case BUTTON_ID_UP:
        return (uint8_t)UP_v;
    case BUTTON_ID_DN:
        return (uint8_t)DN_v;
    case BUTTON_ID_RT:
        return (uint8_t)RT_v;
    case BUTTON_ID_LF:
        return (uint8_t)LF_v;
    default:
        break;
    }
    return 255;
}

void MyButton_start(void)
{
    button_start(&btn[BUTTON_ID_OK]);
    button_start(&btn[BUTTON_ID_UP]);
    button_start(&btn[BUTTON_ID_DN]);
    button_start(&btn[BUTTON_ID_RT]);
    button_start(&btn[BUTTON_ID_LF]);
}

void MyButton_Init(void)
{
    button_init(&btn[BUTTON_ID_OK], BUTN_GPIO_read, 1, BUTTON_ID_OK);
    button_init(&btn[BUTTON_ID_UP], BUTN_GPIO_read, 1, BUTTON_ID_UP);
    button_init(&btn[BUTTON_ID_DN], BUTN_GPIO_read, 1, BUTTON_ID_DN);
    button_init(&btn[BUTTON_ID_RT], BUTN_GPIO_read, 1, BUTTON_ID_RT);
    button_init(&btn[BUTTON_ID_LF], BUTN_GPIO_read, 1, BUTTON_ID_LF);

    MyButton_task_init();

    MyButton_start();
}



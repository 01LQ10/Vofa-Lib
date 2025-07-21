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

#include "MyButton_task.h"
#include "MyButton.h"
#include "LED.h"
#include <stdio.h>

void BTN_OK_Handler(void *btn_p)
{
    PressEvent event = get_button_event(btn_p);
    if (event == SINGLE_CLICK)
    {
        printf("BTN_OK_SINGLE_CLICK\r\n");
    }
    if (event == DOUBLE_CLICK)
    {
        printf("BTN_OK_DOUBLE_CLICK\r\n");
    }
}

void MyButton_task_init(void)
{
    button_attach(&btn[BUTTON_ID_OK], DOUBLE_CLICK, BTN_OK_Handler);
    button_attach(&btn[BUTTON_ID_OK], PRESS_DOWN, restart_game);
    button_attach(&btn[BUTTON_ID_UP], PRESS_DOWN, move_up);
    button_attach(&btn[BUTTON_ID_DN], PRESS_DOWN, move_down);
    button_attach(&btn[BUTTON_ID_LF], PRESS_DOWN, move_left);
    button_attach(&btn[BUTTON_ID_RT], PRESS_DOWN, move_right);

}

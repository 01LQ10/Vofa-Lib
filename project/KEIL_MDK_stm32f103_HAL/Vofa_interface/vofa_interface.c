#include "vofa_interface.h"
#include "usart.h"

#define RECV_BUFF_SIZE 1024

uint8_t recv_buff[RECV_BUFF_SIZE];
volatile uint16_t write_p = 0, read_p = 0;

uint8_t vofa_interface_init(void)
{
    HAL_UART_Receive_IT(&huart1, (uint8_t *)(&recv_buff[0]), 1);
    return 0;
}
uint8_t vofa_interface_send(uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit(&huart1, data, len, 0xFFFFFF);
    return len;
}

uint8_t vofa_interface_recv(uint8_t *data, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        if (read_p == write_p)
            return i;
        *(data + i) = recv_buff[read_p];
        read_p = (read_p + 1) & (RECV_BUFF_SIZE - 1); // 使用位操作代替取模
        // read_p++;
        // if (read_p >= RECV_BUFF_SIZE)
        //     read_p = 0;
    }

    return len;
}

// 中断
void recv_buff_write(void)
{
    uint16_t next_write_p = (write_p + 1) & (RECV_BUFF_SIZE - 1);

    // 检查缓冲区是否已满
    if (next_write_p == read_p)
    {
        // 缓冲区满，丢弃数据或处理错误
        return;
    }
    else
    {
        write_p = next_write_p;
        // write_p++;
        // if (write_p >= RECV_BUFF_SIZE)
        //     write_p = 0;
        HAL_UART_Receive_IT(&huart1, (uint8_t *)(&recv_buff[write_p]), 1);
    }
}

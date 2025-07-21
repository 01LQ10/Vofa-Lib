#include "vofa_decode_text.h"

#define BUFFER_SIZE 128

uint8_t vofa_decode_text_handle(vofa_t *handle)
{
    static char buf[BUFFER_SIZE];
    static uint8_t pos = 0; // 已处理数据位置

    uint8_t len = 0;                  // 接受数据长度
    uint8_t end;                      // 数据结束位置
    uint8_t line_start = 0, line_end; // 行开始/结束位置
    char *name;
    float value;

    len = handle->recv((uint8_t *)buf + pos, BUFFER_SIZE - pos);
    if (len == 0)
        return 1; // 无数据
    end = pos + len;
    for (int i = pos; i < end; i++)
    {
        // 找 \n
        if (buf[i] == '\n')
        {
            line_end = i;

            // 解码
            buf[line_end] = '\0';
            char *colon = strchr(&buf[line_start], ':');
            *colon = '\0';

            // 处理数据
            name = &buf[line_start];
            // value = (float)atof(colon + 1);
            char *endptr;
            value = strtof(colon + 1, &endptr);
            if (endptr == &buf[line_end]) // 转换成功
            {
                // 赋值
                // handle->protocol_send_txt(handle, "%s:%.2f\n", name, value);
                handle->set_widget(handle, name, value);
            }

            // 准备下一个
            line_start = line_end + 1;
            pos = line_start;
        }
    }
    if (pos != end)
    {
        memmove(buf, &buf[pos], len - pos);
        pos = len - pos;
    }
    else
    {
        pos = 0;
    }
    return 0; // 成功
}

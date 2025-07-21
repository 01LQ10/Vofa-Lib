#include "vofa_protocol_firewater.h"

uint8_t vofa_protocol_firewater_send_data(vofa_t *handle)
{

    if (handle->count == 0)
    {
        return 1;
    }
    char str[32];
    int len;

    handle->send("d:", 2);
    for (int i = 0; i < handle->count; i++)
    {
        switch (((handle->base) + i)->type)
        {
        case VOFA_SAMPLE_INT:
            len = sprintf(str, "%d", *(int *)(((handle->base) + i)->data_p));
            break;
        case VOFA_SAMPLE_UINT:
            len = sprintf(str, "%u", *(uint32_t *)(((handle->base) + i)->data_p));
            break;
        case VOFA_SAMPLE_FLOAT:
            len = sprintf(str, "%.3f", *(float *)(((handle->base) + i)->data_p));
            break;
        case VOFA_SAMPLE_DOUBLE:
            len = sprintf(str, "%.4lf", *(double *)(((handle->base) + i)->data_p));
            break;
        default:
            break;
        }
        handle->send((uint8_t *)str, len);
        if (i < handle->count - 1)
            handle->send(",", 1);
    }

    handle->send("\n\r", 2);

    return 0;
}

uint8_t vofa_protocol_firewater_send_image(vofa_t *handle)
{
    char str[64];
    int len;
    /*发送前导帧*/
    len = sprintf(str, "image:%d,%d,%d,%d,%d\n\r",
                  handle->img.id,
                  handle->img.size,
                  handle->img.width,
                  handle->img.height,
                  handle->img.format);
    handle->send((uint8_t *)str, len);
    /*发送图片数据*/
    handle->send((uint8_t *)handle->img.data, handle->img.size);

    return 0;
}

uint8_t vofa_protocol_firewater_send_txt(vofa_t *handle, const char *const fmt, ...)
{
    int len;
    char buf[64];
    va_list args;
    va_start(args, fmt);

    // 安全格式化，自动截断
    len = vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (len < 0)
        return 1; // 格式化失败

    // if (buf[len - 1] == '\n')
    // {
    //     handle->send((uint8_t *)buf, len - 1);
    //     handle->send("::\n\n", 4);
    // }
    // else
    // {
    //     handle->send((uint8_t *)buf, len);
    // }
    handle->send((uint8_t *)buf, len);

    if (len >= sizeof(buf))
        return 2; // 格式化长度超出限制,但发送截断后的数据
    else
        return 0; // 成功
}

#include "vofa_protocol_justfloat.h"

const uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};

uint8_t vofa_protocol_justfloat_send_data(vofa_t *handle)
{
    if (handle->count == 0)
    {
        return 1;
    }
    float temp;


    for (int i = 0; i < handle->count; i++)
    {
        switch (((handle->base) + i)->type)
        {
        case VOFA_SAMPLE_FLOAT:
            temp = *(float *)((handle->base) + i)->data_p;
            break;
        case VOFA_SAMPLE_INT:
            temp = (float)(*(int *)((handle->base) + i)->data_p);
            break;

        case VOFA_SAMPLE_UINT:
            temp = (float)(*(uint32_t *)((handle->base) + i)->data_p);
            break;

        case VOFA_SAMPLE_DOUBLE:
            temp = (float)(*(double *)((handle->base) + i)->data_p);
            break;
        }
        handle->send((uint8_t *)&temp, 4);
    }

    handle->send((uint8_t *)tail, 4);

    return 0;
}

uint8_t vofa_protocol_justfloat_send_image(vofa_t *handle)
{
    handle->send((uint8_t *)&(handle->img), 4 * 5);

    handle->send((uint8_t *)tail, 4);
    handle->send((uint8_t *)tail, 4);

    handle->send((uint8_t *)(handle->img.data), (uint16_t)(handle->img.size));

    // uint8_t data[4] = {0x00, 0x00, 0x00, 0x00};
    // handle->send((uint8_t *)data, 4);
    // handle->send((uint8_t *)tail, 4);

    return 0;
}

uint8_t vofa_protocol_justfloat_send_txt(vofa_t *handle, const char *const fmt, ...)
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

    handle->send((uint8_t *)buf, len);

    if (len % 4 == 0)
    {
        handle->send("\0", 1);
    }
    handle->send((uint8_t *)tail, 4);

    if (len >= sizeof(buf))
        return 2; // 格式化长度超出限制,但发送截断后的数据
    else
        return 0; // 成功
}

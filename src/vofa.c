#include "vofa.h"
#include "vofa_protocol_firewater.h"
#include "vofa_protocol_justfloat.h"
#include "vofa_decode_text.h"

// 内部使用的日志打印
#define a_vofa_log(str, ...) (handle)->protocol_send_txt(handle, str, ##__VA_ARGS__)

// 获取数据区域的基地址和数量
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
extern const unsigned int vofaData$$Base;
extern const unsigned int vofaData$$Limit;
#elif defined(__ICCARM__) || defined(__ICCRX__)
#pragma section = "vofaData"
#elif defined(__GNUC__)
extern const unsigned int _shell_command_start;
extern const unsigned int _shell_command_end;
#endif

/**
 * @brief 初始化vofa模块
 *
 * 该函数根据编译器的不同，初始化vofa模块的数据处理。
 * 它配置了数据处理的基地址和数量，根据指定的协议配置了相应的数据收发函数。
 *
 * @param handle 模块的句柄，用于存储初始化后的配置
 * @return  0 表示成功，其他表示失败
 *          1 表示 init 函数为空
 *          2 表示 send 函数为空
 *          3 表示 recv 函数为空
 *          4 表示 protocol 发送协议未配置
 */
uint8_t vofa_init(vofa_t *handle)
{
    float temp_float = 0;
    vofa_sample_t temp_sample = {NULL, VOFA_SAMPLE_FLOAT, &temp_float};

    // 根据不同的编译器，获取数据区域的基地址和大小
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 6000000)
    handle->base = (vofa_sample_t *)(&vofaData$$Base);
    handle->count = ((size_t)(&vofaData$$Limit) - (size_t)(&vofaData$$Base)) / sizeof(vofa_sample_t);
#elif defined(__ICCARM__) || defined(__ICCRX__)
    shell->commandList.base = (vofa_sample_t *)(__section_begin("vofaData"));
    shell->commandList.count = ((size_t)(__section_end("vofaData")) - (size_t)(__section_begin("vofaData"))) / sizeof(vofa_sample_t);
#elif defined(__GNUC__)
    shell->commandList.base = (vofa_sample_t *)(&_shell_command_start);
    shell->commandList.count = ((size_t)(&_shell_command_end) - (size_t)(&_shell_command_start)) / sizeof(vofa_sample_t);
#else
#error not supported compiler, please use command table mode
#endif

    // 检查通信接口并初始化
    if (NULL == handle->init)
        return 1;
    if (NULL == handle->send)
        return 2;
    if (NULL == handle->recv)
        return 3;

    handle->init();

    // 根据协议类型配置相应的数据发送函数
    if (handle->protocol == VOFA_PROTOCOL_FIREWATER)
    {
        handle->protocol_send_data = vofa_protocol_firewater_send_data;
        handle->protocol_send_image = vofa_protocol_firewater_send_image;
        handle->protocol_send_txt = vofa_protocol_firewater_send_txt;
    }
    else if (handle->protocol == VOFA_PROTOCOL_JUSTFLOAT)
    {
        handle->protocol_send_data = vofa_protocol_justfloat_send_data;
        handle->protocol_send_image = vofa_protocol_justfloat_send_image;
        handle->protocol_send_txt = vofa_protocol_justfloat_send_txt;
    }
    else
    {
        // 如果是其他协议，确保必要的数据发送函数已经被配置
        if (handle->protocol_send_data == NULL ||
            handle->protocol_send_image == NULL ||
            handle->protocol_send_txt == NULL)
            return 4;
    }

    // 对图片数据进行必要的处理
    if (handle->img.size != 0) // 如果存在图片数据
    {
        if (handle->count != 0) // 使用现有采样数据进行填充
        {
            for (int i = 0; i < handle->img.size / (handle->count * 5); i++)
            {
                handle->protocol_send_data(handle);
            }
        }
        else // 创建新的采样数据进行填充
        {
            handle->base = &temp_sample;
            handle->count = 1;
            for (int i = 0; i < handle->img.size / 8; i++)
            {
                handle->protocol_send_data(handle);
            }
            handle->base = NULL;
            handle->count = 0;
        }
    }

    // 默认配置接收解码相关函数
    if (NULL == handle->recv_handle)
        handle->recv_handle = vofa_decode_text_handle;

    // 默认配置控件相关函数
    if (NULL == handle->set_widget)
        handle->set_widget = vofa_widget_set;

    // 输出协议配置信息
    vofa_data_info_out(handle);

    // widget 信息输出
    vofa_widget_info_out(handle);

    return 0;
}

/**
 * @brief 输出voaf数据信息
 *
 * 输出vofa数据的基本信息，包括数据数量、每个数据的详细信息（ID、类型、名称），
 * 以及图片信息（如果存在）。这对于调试和验证数据结构的正确性非常有用。
 *
 * @param handle 指向vofa数据结构的指针通过此指针可以访问和操作vofa数据结构中的成员
 */
void vofa_data_info_out(vofa_t *handle)
{
    // 输出数据信息
    a_vofa_log("[Data]\n");
    if (handle->count > 0)
    {
        a_vofa_log("[ID]\t[type]\tname\n");
        for (uint8_t i = 0; i < handle->count; i++)
        {
            a_vofa_log("[%02u]\t[%c]\t%s\n",
                       i, ((handle->base) + i)->type, ((handle->base) + i)->name);
        }
    }
    else
    {
        a_vofa_log("No data\n");
    }
    // 输出图片信息
    a_vofa_log("[Image]\n");

    // 检查并输出图片信息，如果图片大小不为0，则说明存在图片数据
    if (handle->img.size != 0)
        vofa_log(handle, "[id|%d] [sz|%d] [w|%d]x[h|%d] [f|%d]\n",
                 handle->img.id,
                 handle->img.size,
                 handle->img.width,
                 handle->img.height,
                 handle->img.format);
    else
        a_vofa_log("No image\n");
}

void vofa_add_image(vofa_t *handle, int _id,
                    int _size, int _width, int _height,
                    int _format, uint8_t *_data)
{
    handle->img.id = _id;
    handle->img.size = _size;
    handle->img.width = _width;
    handle->img.height = _height;
    handle->img.format = _format;
    handle->img.data = _data;
}

void vofa_widget_init(vofa_widget_t *h_widget, char *_name, float *_value)
{
    h_widget->name = _name;
    h_widget->value = _value;
}

uint8_t vofa_add_widget(vofa_t *handle, vofa_widget_t *h_widget)
{
    vofa_widget_t *target = handle->widget_head;
    while (target)
    {
        if (target == h_widget)
            return 1; // already exist.
        target = target->next;
    }
    h_widget->next = handle->widget_head;
    handle->widget_head = h_widget;
    return 0;
}

uint8_t vofa_widget_set(vofa_t *handle, char *_name, float _value)
{
    vofa_widget_t *target = handle->widget_head;
    uint8_t t = 0;
    while (target != NULL)
    {
        if (strcmp(target->name, _name) == 0)
        {
            *(target->value) = _value;
            return 0; // success
        }
        target = target->next;
        t++;
        if (t >= 200)
            return 1; // too many widgets
    }
    return 2; // not found
}


uint8_t vofa_widget_info_out(vofa_t *handle)
{
    vofa_widget_t *target = handle->widget_head;
    uint8_t t = 0;
    a_vofa_log("[Widget]\n");

    if (target == NULL)
    {
        a_vofa_log("No widget\n");
    }
    else
    {
        while (target != NULL)
        {
            a_vofa_log("[%u]%s\n", t, target->name);

            target = target->next;
            t++;
            if (t >= 200)
                return 1; // too many widgets
        }
    }
    return 0;
}

// uint8_t vofa_get_widget(vofa_t *handle, char *name, float *value)
// {
// }

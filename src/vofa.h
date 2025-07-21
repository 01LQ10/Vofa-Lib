#ifndef __VOFA_H
#define __VOFA_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"

/*
 * 采样数据类型
 */
typedef enum
{
    VOFA_SAMPLE_INT = 'i',
    VOFA_SAMPLE_UINT = 'u',
    VOFA_SAMPLE_FLOAT = 'f',
    VOFA_SAMPLE_DOUBLE = 'd',
    VOFA_SAMPLE_MAX = 0x04
} vofa_data_type;

/*
 * 采样数据结构
 */
typedef struct vofa_sample_s
{
    const char *name;
    vofa_data_type type;
    void *data_p;
} vofa_sample_t;

/*
 * 图片格式
 */
typedef enum
{
    Format_Invalid,
    Format_Mono, // The image is stored using 1-bit per pixel. Bytes are packed with the most significant bit (MSB) first.
    Format_MonoLSB,
    Format_Indexed8,
    Format_RGB32,
    Format_ARGB32,
    Format_ARGB32_Premultiplied,
    Format_RGB16,
    Format_ARGB8565_Premultiplied,
    Format_RGB666,
    Format_ARGB6666_Premultiplied,
    Format_RGB555,
    Format_ARGB8555_Premultiplied,
    Format_RGB888,
    Format_RGB444,
    Format_ARGB4444_Premultiplied,
    Format_RGBX8888,
    Format_RGBA8888,
    Format_RGBA8888_Premultiplied,
    Format_BGR30,
    Format_A2BGR30_Premultiplied,
    Format_RGB30,
    Format_A2RGB30_Premultiplied,
    Format_Alpha8,
    Format_Grayscale8,
    // 以下格式发送时，IMG_WIDTH和IMG_HEIGHT不需要强制指定，设置为-1即可
    Format_BMP,
    Format_GIF,
    Format_JPG,
    Format_PNG,
    Format_PBM,
    Format_PGM,
    Format_PPM,
    Format_XBM,
    Format_XPM,
    Format_SVG,
} ImgFormat_t;

/*
 * 图片数据结构
 */
typedef struct vofa_image_s
{
    int id;
    int size;
    int width;
    int height;
    int format; // from ImgFormat_t
    uint8_t *data;
} vofa_image_t;

/*
 * 数据协议
 */
typedef enum
{
    VOFA_PROTOCOL_FIREWATER,
    VOFA_PROTOCOL_JUSTFLOAT,
    VOFA_PROTOCOL_RAWDATA,
    VOFA_PROTOCOL_OTHER,
} vofa_protocol_t;

/*
 * 发送数据类型
 */
typedef enum
{
    VOFA_SEND_DATA,
    VOFA_SEND_IMAGE,
    VOFA_SEND_TEXT,
} vofa_send_type_t;

/*
 * 控件数据结构
 */
typedef struct vofa_widget_s vofa_widget_t;
struct vofa_widget_s
{
    uint8_t id;
    char *name;
    uint8_t type;
    float *value;
    vofa_widget_t *next;
};

typedef struct vofa_s vofa_t;
struct vofa_s
{
    // 数据接口
    vofa_sample_t *base;
    unsigned short count;
    vofa_image_t img;

    // 通讯接口
    uint8_t (*init)(void);
    uint8_t (*send)(uint8_t *data, uint16_t len);
    uint8_t (*recv)(uint8_t *data, uint16_t len);

    // 发送协议
    vofa_protocol_t protocol;
    uint8_t (*protocol_send_data)(vofa_t *handle);
    uint8_t (*protocol_send_image)(vofa_t *handle);
    uint8_t (*protocol_send_txt)(vofa_t *handle, const char *const fmt, ...);

    // 接收协议
    uint8_t (*recv_handle)(vofa_t *handle);

    // 控件接口
    vofa_widget_t *widget_head;
    uint8_t (*set_widget)(vofa_t *handle, char *name, float value);
    // uint8_t (*get_widget)(vofa_t *handle, char *name, float value);
};

#ifndef VOFA_SECTION
#if defined(__CC_ARM) || defined(__CLANG_ARM)
#define VOFA_SECTION(x) __attribute__((section(x), aligned(1)))
#elif defined(__IAR_SYSTEMS_ICC__)
#define VOFA_SECTION(x) @x
#elif defined(__GNUC__)
#define VOFA_SECTION(x) __attribute__((section(x), aligned(1)))
#else
#define VOFA_SECTION(x)
#endif
#endif

#ifndef VOFA_USED
#if defined(__CC_ARM) || defined(__CLANG_ARM)
#define VOFA_USED __attribute__((used))
#elif defined(__IAR_SYSTEMS_ICC__)
#define VOFA_USED __root
#elif defined(__GNUC__)
#define VOFA_USED __attribute__((used))
#else
#define VOFA_USED
#endif
#endif

/**
 * @brief samples data 定义
 */
#define VOFA_ADD_DATA(_name, _type, _data)                  \
    const char vofa_data##_name[] = #_name;                 \
    VOFA_USED const vofa_sample_t                           \
    vofa_sample_data_##_name VOFA_SECTION("vofaData") =     \
    {                                                       \
                .name = vofa_data##_name,                   \
                .type = _type,                              \
                .data_p = (void *)_data,                    \
    }

#define vofa_log(handle, str, ...) (handle)->protocol_send_txt(handle, str, ##__VA_ARGS__)
#define vofa_send_samples(handle) (handle)->protocol_send_data(handle)
#define vofa_send_image(handle) (handle)->protocol_send_image(handle)

void vofa_add_image(vofa_t *handle, int _id,
                    int _size, int _width, int _height,
                    int _format, uint8_t *_data);

void vofa_widget_init(vofa_widget_t *h_widget, char *name, float *value);
uint8_t vofa_add_widget(vofa_t *handle, vofa_widget_t *h_widget);
uint8_t vofa_widget_set(vofa_t *handle, char *_name, float _value);
uint8_t vofa_widget_info_out(vofa_t *handle);

uint8_t vofa_init(vofa_t *handle);
void vofa_data_info_out(vofa_t *handle);

#endif

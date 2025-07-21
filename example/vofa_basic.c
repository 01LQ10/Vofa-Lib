#include "vofa_basic.h"
#include <math.h>
#include "img.h"

#define M_PI 3.1415926535f

vofa_t vofa_handle;

// 模拟采样数据
int a = 0;    // 方波
float b = 0;  // 三角波
float c = 0;  // 正弦
double d = 0; // 余弦
// 添加数据
VOFA_ADD_DATA(square, VOFA_SAMPLE_INT, &a);
VOFA_ADD_DATA(triang, VOFA_SAMPLE_FLOAT, &b);
VOFA_ADD_DATA(sin, VOFA_SAMPLE_FLOAT, &c);
VOFA_ADD_DATA(cos, VOFA_SAMPLE_DOUBLE, &d);
// 模拟图片数据
uint8_t image_16x2[4] = {
    0x00, 0xff, 0x00, 0xFf};
// 定义控件
vofa_widget_t my_widget[3];
// 模拟控件设置数据
float T = 2;   // 周期
float A = 1;   // 幅值
float BTN = 0; // 按钮

// 添加控件数据，用于回显确认，防止设置接收不同步，非必要
// VOFA_ADD_DATA(T, VOFA_SAMPLE_FLOAT, &T);
// VOFA_ADD_DATA(A, VOFA_SAMPLE_FLOAT, &A);
// VOFA_ADD_DATA(BTN, VOFA_SAMPLE_FLOAT, &BTN);

void vofa_basic_init(void)
{

    memset(&vofa_handle, 0, sizeof(vofa_t)); // 初始化句柄
    vofa_handle.init = vofa_interface_init;  // 挂载接口函数
    vofa_handle.send = vofa_interface_send;
    vofa_handle.recv = vofa_interface_recv;

    vofa_handle.protocol = VOFA_PROTOCOL_JUSTFLOAT;                                     // 设置协议类型 也可以填 VOFA_PROTOCOL_FIREWATER
    vofa_add_image(&vofa_handle, 1, 4608, 48, 48, Format_RGB16, (uint8_t *)gImage_img); // 添加图片

    // 初始化控件
    vofa_widget_init(&my_widget[0], "T", &T);
    vofa_widget_init(&my_widget[1], "A", &A);
    vofa_widget_init(&my_widget[2], "BTN", &BTN);

    vofa_add_widget(&vofa_handle, &my_widget[0]);
    vofa_add_widget(&vofa_handle, &my_widget[1]);
    vofa_add_widget(&vofa_handle, &my_widget[2]);

    vofa_init(&vofa_handle); // 初始化

    for (uint8_t i = 0; i < 10; i++)
    {
        vofa_send_image(&vofa_handle);
    }
}

// 更新数据
void vofa_basic_update(void)
{
    static float t = 0;
    t += 0.05; // 50ms

    // 方波
    a = (fmodf(t, T) < (T / 2)) ? A : -A;

    // 三角波
    float b_phase = fmodf(t, T) / T; // 归一化到[0,1]
    if (b_phase < 0.25f)
    {
        b = (4.0f * A * b_phase);
    }
    else if (b_phase < 0.75f)
    {
        b = (2.0f * A - 4.0f * A * b_phase);
    }
    else
    {
        b = (-4.0f * A + 4.0f * A * b_phase);
    }

    // 正弦波
    c = A * sinf(2.0f * M_PI * t / T);

    // 余弦波
    d = A * cosf(2.0f * M_PI * t / T);

    // 复位
    if (1 == BTN)
    {
        t = 0;
        a = 0;
        b = 0;
        c = 0;
        d = 0;
        b_phase = 0.0f;
    }
}

void vofa_basic_loop(void)
{
    vofa_handle.recv_handle(&vofa_handle);
    vofa_basic_update();
    vofa_send_samples(&vofa_handle);
}

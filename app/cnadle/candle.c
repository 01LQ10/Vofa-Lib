#include "candle.h"
#include <stdio.h>
#include <math.h>

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// 数据结构
typedef struct
{
    float x;        // x坐标
    float y;        // y坐标
    float r;        // 半径
    float d;        // 亮度
    uint16_t t;     // 出生时间
    float vx;       // x轴速度
    float vy;       // y轴速度
    uint8_t active; // 是否活跃(用于标记可重用)
} circle_t;

#define MAX_CIRCLES 50   // 最大小球数量
#define FLAME_BASE_X 3   // 火焰基部x坐标
#define FLAME_BASE_Y 3   // 火焰基部y坐标
#define LIFETIME_MS 2000 // 小球生命周期(毫秒)

circle_t circles[MAX_CIRCLES];

// 获取系统时间(毫秒)
uint32_t get_tick()
{
    return HAL_GetTick();
}

// 初始化小球系统
void init_flame()
{
    for (int i = 0; i < MAX_CIRCLES; i++)
    {
        circles[i].active = 0;
    }
}

// 创建新小球
void spawn_circle()
{
    static uint32_t last_spawn_time = 0;
    uint32_t current_time = get_tick();

    // 控制生成频率(每30毫秒生成一个)
    // if (current_time - last_spawn_time < 30)
    // {
    //     return;
    // }
    last_spawn_time = current_time;

    // 查找空闲的小球槽
    for (int i = 0; i < MAX_CIRCLES; i++)
    {
        if (!circles[i].active)
        {
            // 初始位置在火焰基部附近随机偏移
            circles[i].x = FLAME_BASE_X + (rand() % 2); // -5到+5的随机偏移
            circles[i].y = FLAME_BASE_Y;

            // 初始半径(3-6像素)
            circles[i].r = 2 + rand() % 3;

            // 初始亮度(200-255)
            circles[i].d = 200 + rand() % 56;

            // 记录出生时间
            circles[i].t = current_time;

            // 初始速度(向上为主，带随机水平分量)
            circles[i].vx = 0;                                  // (float)(rand() % 7 - 3) / 100.0f;    // -0.3到+0.3
            circles[i].vy = 0.8f + (float)(rand() % 4) / 10.0f; // -0.5到-0.8

            circles[i].active = 1;
            break;
        }
    }
}

// 更新所有小球状态
void update_circles()
{
    uint32_t current_time = get_tick();

    for (int i = 0; i < MAX_CIRCLES; i++)
    {
        if (circles[i].active)
        {
            // 计算小球年龄
            uint32_t age = current_time - circles[i].t;

            // 检查生命周期
            if (age > LIFETIME_MS)
            {
                circles[i].active = 0;
                continue;
            }

            // 计算生命周期进度(0.0-1.0)
            float life_progress = (float)age / LIFETIME_MS;

            // 更新位置
            circles[i].x += circles[i].vx;
            circles[i].y += circles[i].vy;

            // 添加随机扰动模拟湍流
            // circles[i].vx += (float)(rand() % 5 - 2) / 100.0f;

            // 随着上升逐渐减速
            circles[i].vy *= 0.9f;

            // 添加一些向上的浮力
            circles[i].vy += 0.02f;

            // 更新大小和亮度(随年龄衰减)
            float size_factor;
            // if (life_progress < 0.2)
            //     size_factor = 1.0f + life_progress * 0.2f;
            // else
                size_factor = 0.9f;

            circles[i].r = (uint8_t)(circles[i].r * size_factor);
            if (circles[i].r < 1)
                circles[i].r = 1;

            // 亮度衰减(非线性)
            float brightness_factor = 1.0f - life_progress * life_progress;
            circles[i].d = (uint8_t)(circles[i].d * brightness_factor);
        }
    }
}

#define DISPLAY_WIDTH 8
#define DISPLAY_HEIGHT 16
uint8_t g_ram[DISPLAY_WIDTH * DISPLAY_HEIGHT];

void drawpoint(uint8_t x, uint8_t y, uint8_t v)
{
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT)
        return;
    g_ram[y * DISPLAY_WIDTH + x] = v;
}

void draw_circle(uint8_t x0, uint8_t y0, int r)
{
    if (r <= 0)
        return;

    if (x0 >= DISPLAY_WIDTH || y0 >= DISPLAY_HEIGHT)
        return;
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y)
    {
        drawpoint(x0 + x, y0 + y, 0xFF);
        drawpoint(x0 + y, y0 + x, 0xFF);
        drawpoint(x0 - y, y0 + x, 0xFF);
        drawpoint(x0 - x, y0 + y, 0xFF);
        drawpoint(x0 - x, y0 - y, 0xFF);
        drawpoint(x0 - y, y0 - x, 0xFF);
        drawpoint(x0 + y, y0 - x, 0xFF);
        drawpoint(x0 + x, y0 - y, 0xFF);
        if (d < 0)
        {
            d += 4 * x + 6;
        }
        else
        {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

// 渐变填充圆函数
void fillcircle(int x0, int y0, int r, uint8_t d)
{
    // 参数检查
    if (r <= 0)
        return;
    // 全黑无意义
    if (d == 0)
        return;
    if (x0 >= DISPLAY_WIDTH || y0 >= DISPLAY_HEIGHT || x0 < 0 || y0 < 0)
        return;

    // 计算最大距离（中心到边缘）
    float max_distance = (float)r;

    // 遍历包含圆的正方形区域
    for (int y = -r; y <= r; y++)
    {
        for (int x = -r; x <= r; x++)
        {
            // 计算当前点到中心的距离
            float distance = sqrtf(x * x + y * y);

            // 如果在圆内
            if (distance <= max_distance)
            {
                // 计算渐变亮度：从中心的d线性衰减到边缘的0
                float ratio = 1.0f - (distance / max_distance);
                uint8_t brightness = (uint8_t)((d - 40) * ratio + 30);

                // 绘制点
                if (brightness > g_ram[(y0 + y) * DISPLAY_WIDTH + x0 + x])
                    drawpoint(x0 + x, y0 + y, brightness);
            }
        }
    }
}

void fillcircle_newaxes(int x, int y, int r, uint8_t d)
{

    fillcircle(x, DISPLAY_HEIGHT - y, r, d);
}

void clear_gram(void)
{
    memset(g_ram, 0, sizeof(g_ram));
}

void init_candle(void)
{
    init_flame();
    // srand(time(NULL));
}

void candle_render(void) // 绘制蜡烛
{
    clear_gram();
    for (uint8_t i = 0; i < MAX_CIRCLES; i++)
    {
        if (circles[i].active)
            fillcircle_newaxes((int)circles[i].x, (int)circles[i].y, (int)circles[i].r, (uint8_t)circles[i].d);
    }
}

void run_candle(void)
{
    spawn_circle();
    update_circles();
    candle_render();
}

vofa_t vofa_candle_handle;
void vofa_candle_init(void)
{
    memset(&vofa_candle_handle, 0, sizeof(vofa_t));
    // 挂载通信接口
    vofa_candle_handle.init = vofa_interface_init;
    vofa_candle_handle.send = vofa_interface_send;
    vofa_candle_handle.recv = vofa_interface_recv;
    // 选择通信协议 or 挂载自定义通信协议
    vofa_candle_handle.protocol = VOFA_PROTOCOL_FIREWATER; // VOFA_PROTOCOL_JUSTFLOAT;
    // 图片
    vofa_add_image(&vofa_candle_handle, 1,
                   DISPLAY_HEIGHT * DISPLAY_WIDTH, DISPLAY_WIDTH, DISPLAY_HEIGHT,
                   Format_Indexed8, (uint8_t *)g_ram);

    // 初始化vofa，内部对未自定义内容进行默认设置
    vofa_init(&vofa_candle_handle);
    init_candle();
}

void vofa_candle_run(void)
{
    // vofa_candle_handle.recv_handle(&vofa_candle_handle);
    vofa_candle_handle.protocol_send_data(&vofa_candle_handle);
    run_candle();
    vofa_candle_handle.protocol_send_image(&vofa_candle_handle);
}

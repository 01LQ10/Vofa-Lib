#ifndef __GAME_H__
#define __GAME_H__

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
// 游戏常量定义
#define WIDTH 32
#define HEIGHT 16
#define INIT_LENGTH 3

// 方向枚举
typedef enum
{
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// 蛇结构体
typedef struct
{
    int x;
    int y;
} Segment;

// 游戏状态
typedef struct
{
    Segment snake[WIDTH * HEIGHT];
    int length;
    Direction dir;
    Direction next_dir;
    int food_x;
    int food_y;
    bool game_over;
} GameState;

extern uint8_t game_buff[64];

void init_game(void);
void game_loop(void);
void restart_game(void *p);

void move_up(void *p);
void move_down(void *p);
void move_left(void *p);
void move_right(void *p);

#endif

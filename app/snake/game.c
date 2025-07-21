#include "game.h"
#include <stdio.h>

// 假设的画点和清屏函数
void draw_point(int x, int y);
void clear_screen(void);

uint8_t game_buff[WIDTH * HEIGHT / 8];

GameState game;



// 生成食物
void generate_food(void)
{
    bool valid;
    do
    {
        valid = true;
        game.food_x = rand() % WIDTH;
        game.food_y = rand() % HEIGHT;

        // 检查食物是否与蛇身重叠
        for (int i = 0; i < game.length; i++)
        {
            if (game.snake[i].x == game.food_x && game.snake[i].y == game.food_y)
            {
                valid = false;
                break;
            }
        }
    } while (!valid);
}


// 更新游戏状态
void update_game(void)
{
    if (game.game_over)
        return;

    // 更新方向
    game.dir = game.next_dir;

    // 移动蛇
    Segment new_head = game.snake[0];
    switch (game.dir)
    {
    case DIR_UP:
        new_head.y--;
        break;
    case DIR_DOWN:
        new_head.y++;
        break;
    case DIR_LEFT:
        new_head.x--;
        break;
    case DIR_RIGHT:
        new_head.x++;
        break;
    }

    // 检查碰撞
    if (new_head.x < 0 || new_head.x >= WIDTH ||
        new_head.y < 0 || new_head.y >= HEIGHT)
    {
        game.game_over = true;
        return;
    }

    // 检查是否撞到自己
    for (int i = 0; i < game.length; i++)
    {
        if (game.snake[i].x == new_head.x && game.snake[i].y == new_head.y)
        {
            game.game_over = true;
            return;
        }
    }

    // 移动蛇身
    for (int i = game.length; i > 0; i--)
    {
        game.snake[i] = game.snake[i - 1];
    }
    game.snake[0] = new_head;

    // 检查是否吃到食物
    if (new_head.x == game.food_x && new_head.y == game.food_y)
    {
        game.length++;
        generate_food();
    }
}

// 渲染游戏
void render_game(void)
{
    clear_screen();

    if (game.game_over)
    {
        // 显示游戏结束
        // 这里简单画一个GAME OVER提示
        for (int x = WIDTH / 2 - 5; x < WIDTH / 2 + 5; x++)
        {
            draw_point(x, HEIGHT / 2 - 1);
            draw_point(x, HEIGHT / 2 + 1);
        }
        for (int y = HEIGHT / 2 - 1; y <= HEIGHT / 2 + 1; y++)
        {
            draw_point(WIDTH / 2 - 5, y);
            draw_point(WIDTH / 2 + 4, y);
        }
        return;
    }

    // 画食物
    draw_point(game.food_x, game.food_y);

    // 画蛇
    for (int i = 0; i < game.length; i++)
    {
        draw_point(game.snake[i].x, game.snake[i].y);
    }
}


// 初始化游戏
void init_game(void)
{
    // 初始化蛇
    game.length = INIT_LENGTH;
    for (int i = 0; i < game.length; i++)
    {
        game.snake[i].x = WIDTH / 2 - i;
        game.snake[i].y = HEIGHT / 2;
    }

    // 初始方向向右
    game.dir = DIR_RIGHT;
    game.next_dir = DIR_RIGHT;

    // 生成食物
    generate_food();

    game.game_over = false;

    render_game();
}

// 方向控制函数
void move_up(void*p)
{
    if (game.dir != DIR_DOWN)
    {
        game.next_dir = DIR_UP;
    }
    printf("move up\n");
}

void move_down(void*p)
{
    if (game.dir != DIR_UP)
    {
        game.next_dir = DIR_DOWN;
    }
}

void move_left(void*p)
{
    if (game.dir != DIR_RIGHT)
    {
        game.next_dir = DIR_LEFT;
    }
}

void move_right(void*p)
{
    if (game.dir != DIR_LEFT)
    {
        game.next_dir = DIR_RIGHT;
    }
}

// 重启游戏
void restart_game(void*p)
{
    if (game.game_over)
    {
        init_game();
    }
}

// 主游戏循环（需要由外部定时调用）
void game_loop(void)
{
    update_game();
    render_game();
}

void draw_point(int x, int y)
{
    int index = x + y * WIDTH;
    game_buff[index / 8] |= (0x80 >> (index % 8));
}
void clear_screen(void)
{
    memset(game_buff, 0, sizeof(game_buff));
}

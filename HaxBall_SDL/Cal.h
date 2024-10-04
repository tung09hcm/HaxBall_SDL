#pragma once
#include "Player.h"
#include "Ball.h"
using namespace std;

bool cal_distance(Player* player, Ball* ball)
{
    float delta_x = player->x + 32 - ball->x - 16;
    float delta_y = player->y + 32 - ball->y - 16;
    float distance = sqrt(delta_x * delta_x + delta_y * delta_y);


    // Kiểm tra nếu khoảng cách nhỏ hơn hoặc bằng tổng bán kính
    if (distance <= 48)
    {
        return true;
    }
    return false;
}
bool cal_distance_shoot(Player* player, Ball* ball)
{
    float delta_x = player->x + 32 - ball->x - 16;
    float delta_y = player->y + 32 - ball->y - 16;
    float distance = sqrt(delta_x * delta_x + delta_y * delta_y);


    // Kiểm tra nếu khoảng cách nhỏ hơn hoặc bằng tổng bán kính
    if (distance <= 70)
    {
        return true;
    }
    return false;
}
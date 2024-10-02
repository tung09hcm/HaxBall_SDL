#pragma once
#include "Player.h"
#include "Ball.h"
using namespace std;
float cal_distance(Player player, Ball ball)
{
    float delta_x = abs(player.x - ball.x);
    float delta_y = abs(player.y - ball.y);
    float result = sqrt(delta_x * delta_x - delta_y * delta_y);
    return result;
}
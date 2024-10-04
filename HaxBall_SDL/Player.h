#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Common.h"
using namespace std;
class Player {
public:
    float x, y;  // Tọa độ của Player
    int width, height;  // Kích thước ảnh của Player
    bool shoot;
    bool tar;
    bool collision;

    bool collision_left;
    bool collision_right;
    bool collision_top;
    bool collision_down;

    SDL_Texture* texture;  // Texture của Player
    string img_path;
    SDL_Texture* redTexture;
    SDL_Texture* grayTexture;

    // Constructor
    Player(float startX, float startY, string tex) : x(startX), y(startY), img_path(tex) {
        texture = loadTexture(tex, renderer);
        // Lấy kích thước từ texture
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        tar = false;
        shoot = false;
        collision = false;
        collision_left = false;
        collision_right = false;
        collision_down = false;
        collision_top = false;
        // Tải texture 'red.png' một lần khi khởi tạo
        redTexture = loadTexture("red.png", renderer);
        grayTexture = loadTexture("gray.png", renderer);

    }

    // Hàm vẽ player trên màn hình
    void render(SDL_Renderer* renderer, SDL_Texture* indicatorTexture) {
        // Vẽ nhân vật
        SDL_Rect dstRect = { x, y, width, height };
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);

        // Nếu player này đang được điều khiển, vẽ `red.png`
        if (tar) {
            int indicatorWidth, indicatorHeight;
            SDL_QueryTexture(indicatorTexture, NULL, NULL, &indicatorWidth, &indicatorHeight);

            // Tọa độ của chỉ định `red.png` (vẽ lệch lên 32 pixel theo yêu cầu)
            SDL_Rect dstRect1 = { x + 16, y - 32, indicatorWidth, indicatorHeight };

            // Vẽ `red.png` để chỉ định nhân vật đang được điều khiển
            SDL_RenderCopy(renderer, indicatorTexture, NULL, &dstRect1);
        }
    }


    // Hàm xử lý di chuyển
    void handleInput(const Uint8* keyState) {
        if (!tar) return;
        float speed = 0.5;
        if (keyState[SDL_SCANCODE_W]) {
            if(y - speed - 64 >= 0 && !collision_top)
                y -= speed;  // Di chuyển lên
        }
        if (keyState[SDL_SCANCODE_S]) {
            if ( y + speed + 32 <= screenHeight && !collision_down)
                y += speed;  // Di chuyển xuống
        }
        if (keyState[SDL_SCANCODE_A] && !collision_left) {
            if( x - speed >= 0 )
                x -= speed;  // Di chuyển sang trái
        }
        if (keyState[SDL_SCANCODE_D]) {
            if( x+ speed + 64 <= screenWidth && !collision_right)
                x += speed;  // Di chuyển sang phải
        }
        if (keyState[SDL_SCANCODE_F]) {
            tar = false;
        }
        if (keyState[SDL_SCANCODE_SPACE]) {
            // Đặt màu trắng cho renderer
            shoot = true;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // (r, g, b, a) màu trắng
            drawCircleWithBorder(renderer, x + 32, y + 32, 40, 5);
            SDL_RenderPresent(renderer);
            if (!collision) {
                shoot = false;
            }
        }
    }
    ~Player() {
        SDL_DestroyTexture(texture);
    }
};
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
    bool main;
    SDL_Texture* texture;  // Texture của Player
    string img_path;

    // Constructor
    Player(float startX, float startY, string tex) : x(startX), y(startY), img_path(tex) {
        texture = loadTexture(tex, renderer);
        // Lấy kích thước từ texture
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        main = false;
    }

    // Hàm vẽ player trên màn hình
    void render(SDL_Renderer* renderer) {
        SDL_Rect dstRect = { x, y, width, height };
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        // Đặt màu trắng cho renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // (r, g, b, a) màu trắng

        // Vẽ khung trắng quanh đối tượng
        SDL_RenderDrawRect(renderer, &dstRect);
    }

    // Hàm xử lý di chuyển
    void handleInput(const Uint8* keyState) {
        float speed = 0.5;
        if (keyState[SDL_SCANCODE_W]) {
            if(y - speed - 64 >= 0)
                y -= speed;  // Di chuyển lên
        }
        if (keyState[SDL_SCANCODE_S]) {
            if ( y + speed + 64 <= screenHeight)
                y += speed;  // Di chuyển xuống
        }
        if (keyState[SDL_SCANCODE_A]) {
            if( x - speed >= 0 )
                x -= speed;  // Di chuyển sang trái
        }
        if (keyState[SDL_SCANCODE_D]) {
            if( x+ speed + 64 <= screenWidth)
                x += speed;  // Di chuyển sang phải
        }
    }
    ~Player() {
        SDL_DestroyTexture(texture);
    }
};
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Common.h"
using namespace std;
class Player {
public:
    int x, y;  // Tọa độ của Player
    int width, height;  // Kích thước ảnh của Player
    SDL_Texture* texture;  // Texture của Player
    string img_path;

    // Constructor
    Player(int startX, int startY, string tex) : x(startX), y(startY), img_path(tex) {
        texture = loadTexture(tex, renderer);
        // Lấy kích thước từ texture
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    }

    // Hàm vẽ player trên màn hình
    void render(SDL_Renderer* renderer) {
        SDL_Rect dstRect = { x, y, width, height };
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    }

    // Hàm xử lý di chuyển
    void handleInput(const Uint8* keyState) {
        int speed = 1;
        if (keyState[SDL_SCANCODE_W]) {
            y -= speed;  // Di chuyển lên
        }
        if (keyState[SDL_SCANCODE_S]) {
            y += speed;  // Di chuyển xuống
        }
        if (keyState[SDL_SCANCODE_A]) {
            x -= speed;  // Di chuyển sang trái
        }
        if (keyState[SDL_SCANCODE_D]) {
            x += speed;  // Di chuyển sang phải
        }
    }
    ~Player() {
        SDL_DestroyTexture(texture);
    }
};
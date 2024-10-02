#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
const int screenWidth = 1216;
const int screenHeight = 832;
const int tileSize = 64;
// Hàm tải texture từ file PNG
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (newTexture == nullptr) {
        std::cerr << "Không thể tải ảnh: " << IMG_GetError() << std::endl;
    }
    return newTexture;
}
void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int offsetX = radius, offsetY = 0;
    int decisionOver2 = 1 - offsetX; // Quyết định cho vòng tròn (thuật toán Bresenham)

    while (offsetX >= offsetY) {
        // Vẽ 8 điểm đối xứng nhau của hình tròn
        SDL_RenderDrawPoint(renderer, centerX + offsetX, centerY + offsetY);
        SDL_RenderDrawPoint(renderer, centerX + offsetY, centerY + offsetX);
        SDL_RenderDrawPoint(renderer, centerX - offsetY, centerY + offsetX);
        SDL_RenderDrawPoint(renderer, centerX - offsetX, centerY + offsetY);
        SDL_RenderDrawPoint(renderer, centerX - offsetX, centerY - offsetY);
        SDL_RenderDrawPoint(renderer, centerX - offsetY, centerY - offsetX);
        SDL_RenderDrawPoint(renderer, centerX + offsetY, centerY - offsetX);
        SDL_RenderDrawPoint(renderer, centerX + offsetX, centerY - offsetY);

        offsetY++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * offsetY + 1;
        }
        else {
            offsetX--;
            decisionOver2 += 2 * (offsetY - offsetX) + 1;
        }
    }
}

void drawCircleWithBorder(SDL_Renderer* renderer, int centerX, int centerY, int radius, int borderThickness) {
    // Vẽ các hình tròn từ lớn đến nhỏ để tạo viền
    for (int i = 0; i < borderThickness; ++i) {
        drawCircle(renderer, centerX, centerY, radius - i);
    }
}

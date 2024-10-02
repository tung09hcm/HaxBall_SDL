#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
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

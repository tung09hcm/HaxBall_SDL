#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Common.h"
using namespace std;
SDL_Texture* tileTexture = nullptr;
void initMap(string tex)
{
    tileTexture = loadTexture(tex, renderer);
    if (tileTexture == nullptr) {
        std::cerr << "Tải ảnh thất bại!" << std::endl;
        return;
    }
}
void loadMap() {
    
    for (int y = 0; y < screenHeight; y += tileSize) {
        for (int x = 0; x < screenWidth; x += tileSize) {
            SDL_Rect dstRect = { x, y, tileSize, tileSize };
            SDL_RenderCopy(renderer, tileTexture, nullptr, &dstRect);
        }
    }
}


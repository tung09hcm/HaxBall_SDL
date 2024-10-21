#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Common.h"
using namespace std;
SDL_Texture* tileTexture = nullptr;
int width, height;
SDL_Rect dstRect_left;
SDL_Rect dstRect_right;
SDL_Texture* leftPlayer;
SDL_Texture* rightPlayer;
void initMap(string tex)
{
    leftPlayer = loadTexture("argentina.png", renderer);
    rightPlayer = loadTexture("germany.png", renderer);

    SDL_QueryTexture(leftPlayer, NULL, NULL, &width, &height);
    SDL_QueryTexture(rightPlayer, NULL, NULL, &width, &height);
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

void loadMap(int goalLeft, int goalRight) {




    for (int y = 0; y < screenHeight; y += tileSize) {
        for (int x = 0; x < screenWidth; x += tileSize) {
            SDL_Rect dstRect = { x, y, tileSize, tileSize };
            SDL_RenderCopy(renderer, tileTexture, nullptr, &dstRect);
        }
    }
    dstRect_left = { 358, 0, width, height };
    dstRect_right = { 858 - 64, 0, width, height };

    SDL_RenderCopy(renderer, leftPlayer, NULL, &dstRect_left);
    SDL_RenderCopy(renderer, rightPlayer, NULL, &dstRect_right);
}

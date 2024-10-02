#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Common.h"
using namespace std;
class Ball {
public:
	float x;
	float y;
	int width, height;
	bool main;
	SDL_Texture* texture;

	Ball(float x, float y)
	{
		this->x = x;
		this->y = y;
		main = false;
		texture = loadTexture("soccer-ball.png", renderer);
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}
	void render(SDL_Renderer* renderer) {
		SDL_Rect dstRect = { x, y, width, height };
		SDL_RenderCopy(renderer, texture, NULL, &dstRect);
		// Đặt màu trắng cho renderer
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // (r, g, b, a) màu trắng

		// Vẽ khung trắng quanh đối tượng
		SDL_RenderDrawRect(renderer, &dstRect);
	}
	void update() {

	}
	~Ball() {
		SDL_DestroyTexture(texture);
	}
};
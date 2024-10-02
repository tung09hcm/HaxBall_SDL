#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Common.h"
#include "Player.h"
using namespace std;
class Ball {
public:
	float x;
	float y;
	int width, height;
	float velocityX, velocityY;
	bool tar;
	SDL_Texture* texture;

	Ball(float x, float y)
	{
		this->x = x;
		this->y = y;
		velocityX = 0;
		velocityY = 0;
		tar = false;
		texture = loadTexture("soccer-ball.png", renderer);
		SDL_QueryTexture(texture, NULL, NULL, &width, &height);
	}
	void render(SDL_Renderer* renderer) {
		SDL_Rect dstRect = { x, y, width, height };
		SDL_RenderCopy(renderer, texture, NULL, &dstRect);
		// Đặt màu trắng cho renderer
		// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // (r, g, b, a) màu trắng

		// Vẽ khung trắng quanh đối tượng
		// SDL_RenderDrawRect(renderer, &dstRect);
	}
	void update(Player* player) {
		// cout << "ball_x: " << this->x + 16;
		// cout << "  ball_y: " << this->y + 16 << endl;
		if (player == nullptr) return;
		// Nếu người chơi thực hiện cú sút, tăng vận tốc quả bóng
		if (player->shoot && player->collision) {
			// Tăng vận tốc của bóng theo một hệ số, ví dụ là 1.5 lần
			cout << "BANG !!!" << endl;
			velocityX *= 2.0f;
			velocityY *= 2.0f;
			player->shoot = false;
			player->collision = false;
		}
		// Kiểm tra va chạm với biên
		if (x + velocityX - 16 <= 108 || x + velocityX + 16 >= 1108) {
			// Bật ngược lại theo trục X
			velocityX = -velocityX;
		}

		if (y + velocityY - 16 <= 100 || y + velocityY + 16 >= 800) {
			// Bật ngược lại theo trục Y
			velocityY = -velocityY;
		}

		// Cập nhật vị trí
		x += velocityX;
		y += velocityY;

		

		// Giảm dần vận tốc (mô phỏng ma sát)
		velocityX *= 0.995f;
		velocityY *= 0.995f;

		// Nếu vận tốc quá nhỏ, đặt nó về 0
		if (fabs(velocityX) < 0.01f) velocityX = 0;
		if (fabs(velocityY) < 0.01f) velocityY = 0;
	}


	void applyForce(float forceX, float forceY) {
		velocityX = forceX;
		velocityY = forceY;
	}
	~Ball() {
		SDL_DestroyTexture(texture);
	}
};
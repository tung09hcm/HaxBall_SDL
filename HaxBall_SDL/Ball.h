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
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		// Vẽ khung trắng quanh đối tượng
		SDL_RenderDrawRect(renderer, &dstRect);
	}
	void update(Player* player) {
		// cout << "ball_x: " << this->x + 16;
		// cout << "  ball_y: " << this->y + 16 << endl;
		if (player == nullptr) return;



		// Nếu người chơi thực hiện cú sút, tăng vận tốc quả bóng
		if (player->shoot && (player->collision)) {

			cout << "shoot" << endl;
			cout << "Pre: velocityX: " << velocityX << " velocityY: " << velocityY << endl;
			velocityX *= 2.5f;
			cout << "After: velocityX: " << velocityX << " velocityY: " << velocityY << endl;
			velocityY *= 1.5f;
			player->shoot = false;
			player->collision = false;
			//player->shoot = false;
		}
		// cout << "x + velocityX: " << x + velocityX;
		// cout << "    y + velocityY: " << y + velocityY << endl;
		if (x + velocityX <= 108 && y + velocityY >= 310 && y + velocityY + 16 <= 590)
		{
			// cout << "GOAL LEFT" << endl;
			// Kiểm tra va chạm với biên
			if (x + velocityX <= 78 ) {
				// Bật ngược lại theo trục X
				velocityX = -velocityX;
			}

			if (y + velocityY >= 310 || y + velocityY + 16 <= 590) {
				// Bật ngược lại theo trục Y
				velocityY = -velocityY;
			}
		}
		else if (x + velocityX + 16 >= 1108 && y + velocityY >= 310 && y + velocityY + 16 <= 590)
		{
			// cout << "GOAL RIGHT" << endl;
			// Kiểm tra va chạm với biên
			if (x + velocityX + 16 >= 1138) {
				// Bật ngược lại theo trục X
				velocityX = -velocityX;
			}

			if (y + velocityY >= 310 || y + velocityY + 16 <= 590) {
				// Bật ngược lại theo trục Y
				velocityY = -velocityY;
			}
		}
		else
		{
			// Kiểm tra va chạm với biên
			if (x + velocityX <= 108 || x + velocityX + 16 >= 1108) {

				// Bật ngược lại theo trục X
				velocityX = -velocityX;
			}

			if (y + velocityY <= 100 || y + velocityY + 16 >= 800) {
				// Bật ngược lại theo trục Y
				velocityY = -velocityY;
			}
		}
		
		// Cập nhật vị trí
		x += velocityX;
		y += velocityY;

		

		// Giảm dần vận tốc (mô phỏng ma sát)
		velocityX *= 0.999f;
		velocityY *= 0.999f;

		// Nếu vận tốc quá nhỏ, đặt nó về 0
		if (fabs(velocityX) < 0.01f) velocityX = 0;
		if (fabs(velocityY) < 0.01f) velocityY = 0;

	}

	void handleInput(const Uint8* keyState) {

		float speed = 1;

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

	void applyForce(float forceX, float forceY) {
		velocityX = forceX;
		velocityY = forceY;
	}
	~Ball() {
		SDL_DestroyTexture(texture);
	}
};
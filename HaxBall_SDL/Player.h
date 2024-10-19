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
    float velocityX, velocityY;
    bool collision_left;
    bool collision_right;
    bool collision_top;
    bool collision_down;
    Uint32 shootStartTime;
    SDL_Texture* texture;  // Texture của Player
    string img_path;
    SDL_Texture* redTexture;
    SDL_Texture* grayTexture;

    // Constructor
    Player(float startX, float startY, string tex) : x(startX), y(startY), img_path(tex) {
        shootStartTime = 0;
        velocityX = 0;
        velocityY = 0;
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
    void update(Player* player) {
        // cout << "ball_x: " << this->x + 16;
        // cout << "  ball_y: " << this->y + 16 << endl;
        if (player == nullptr) return;
        
        // cout << "x + velocityX: " << x + velocityX;
        // cout << "    y + velocityY: " << y + velocityY << endl;
        if (x + velocityX <= 108 && y + velocityY >= 310 && y + velocityY + 32 <= 590)
        {
            // Kiểm tra va chạm với biên
            if (x + velocityX <= 78) {
                // Bật ngược lại theo trục X
                velocityX = -velocityX;
            }

            if (y + velocityY >= 310 || y + velocityY + 32 <= 590) {
                // Bật ngược lại theo trục Y
                velocityY = -velocityY;
            }
        }
        else if (x + velocityX + 32 >= 1108 && y + velocityY >= 310 && y + velocityY + 32 <= 590)
        {
            // Kiểm tra va chạm với biên
            if (x + velocityX + 32 >= 1138) {
                // Bật ngược lại theo trục X
                velocityX = -velocityX;
            }

            if (y + velocityY >= 310 || y + velocityY + 32 <= 590) {
                // Bật ngược lại theo trục Y
                velocityY = -velocityY;
            }
        }
        else
        {
            // Kiểm tra va chạm với biên
            if (x + velocityX <= 108 || x + velocityX + 32 >= 1108) {

                // Bật ngược lại theo trục X
                velocityX = -velocityX;
            }

            if (y + velocityY <= 100 || y + velocityY + 32 >= 800) {
                // Bật ngược lại theo trục Y
                velocityY = -velocityY;
            }
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


    // Hàm xử lý di chuyển
    void handleInput(const Uint8* keyState) {
        if (!tar) return;
        float speed = 0.6;


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

        if (keyState[SDL_SCANCODE_SPACE]) {
            if (this->shootStartTime == 0) {
                this->shootStartTime = SDL_GetTicks();
            }
            // Đặt màu trắng cho renderer
            this->shoot = true;
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - this->shootStartTime >= 3000) {
                this->shoot = false;
                this->shootStartTime = 0;  // Reset the timer
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // (r, g, b, a) màu trắng
            drawCircleWithBorder(renderer, x + 32, y + 32, 40, 5);
            SDL_RenderPresent(renderer);

        }
    }

    void applyForce(float forceX, float forceY) {
        velocityX = forceX;
        velocityY = forceY;
    }

    ~Player() {
        SDL_DestroyTexture(texture);
    }

    // New function: make player aim and shoot the ball towards the goal
    void shootAtGoal(float ballX, float ballY, float goalX, float goalY) {
        // Calculate direction from player to the ball
        float dirX = ballX - x;
        float dirY = ballY - y;
        float distanceToBall = sqrt(dirX * dirX + dirY * dirY);

        if (distanceToBall < 32) {  // If player is near the ball
            // Calculate direction from ball to the goal
            float goalDirX = goalX - ballX;
            float goalDirY = goalY - ballY;
            float goalDistance = sqrt(goalDirX * goalDirX + goalDirY * goalDirY);

            // Normalize the direction vector (so the length is 1)
            float normGoalDirX = goalDirX / goalDistance;
            float normGoalDirY = goalDirY / goalDistance;

            // Apply force to simulate shooting the ball towards the goal
            float shootingForce = 2.0f;  // Adjust the shooting force
            applyForce(shootingForce * normGoalDirX, shootingForce * normGoalDirY);

            cout << "Player shooting towards goal!" << endl;
        }
        else {
            // Move the player towards the ball
            float speed = 1.0f;  // Speed to move towards the ball
            velocityX = (dirX / distanceToBall) * speed;
            velocityY = (dirY / distanceToBall) * speed;
        }
    }

    // Hàm để cầu thủ tự động di chuyển về phía quả bóng và sút khi đến gần
    void moveToBallAndShoot(float ballX, float ballY, float goalX, float goalY) {
        // Tính toán hướng di chuyển về phía quả bóng
        float dirX = ballX - x;
        float dirY = ballY - y;
        float distanceToBall = sqrt(dirX * dirX + dirY * dirY);
        float speed = 0.2f;  // Tốc độ di chuyển về phía quả bóng
        // Nếu cầu thủ ở gần quả bóng (khoảng cách nhỏ hơn 32 pixel)
        if (distanceToBall < 32) {
            // Tính toán hướng từ quả bóng đến gôn
            float goalDirX = goalX - ballX;
            float goalDirY = goalY - ballY;
            float goalDistance = sqrt(goalDirX * goalDirX + goalDirY * goalDirY);

            // Chuẩn hóa vector hướng đến gôn
            float normGoalDirX = goalDirX / goalDistance;
            float normGoalDirY = goalDirY / goalDistance;

            // Áp dụng lực để sút bóng về phía gôn
            float shootingForce = 2.0f;  // Điều chỉnh lực sút
            applyForce(shootingForce * normGoalDirX, shootingForce * normGoalDirY);

            cout << "Player is shooting towards the goal!" << endl;
        }
        else {
            // Di chuyển về phía quả bóng
            
            velocityX = (dirX / distanceToBall) * speed;
            velocityY = (dirY / distanceToBall) * speed;
        }
        x += velocityX;
        y += velocityY;
        if (y - speed - 64 >= 0 && !collision_top)
            y -= speed;  // Di chuyển lên
        if (y + speed + 32 <= screenHeight && !collision_down)
            y += speed;  // Di chuyển xuống
        if (x - speed >= 0)
            x -= speed;  // Di chuyển sang trái
        if (x + speed + 64 <= screenWidth && !collision_right)
            x += speed;  // Di chuyển sang phải
    }
};
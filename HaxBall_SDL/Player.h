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
    bool tar2;
    bool next;
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
    SDL_Texture* blueTexture;

    string player_type;
    float speed = 0.6f;

    // Constructor
    Player(float startX, float startY, string tex, string type) : x(startX), y(startY), img_path(tex), player_type(type) {
        tar2 = false;
        shootStartTime = 0;
        velocityX = 0;
        velocityY = 0;
        texture = loadTexture(tex, renderer);
        // Lấy kích thước từ texture
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);
        tar = false; next = false;
        shoot = false;
        collision = false;
        collision_left = false;
        collision_right = false;
        collision_down = false;
        collision_top = false;
        // Tải texture 'red.png' một lần khi khởi tạo
        redTexture = loadTexture("red.png", renderer);
        blueTexture = loadTexture("blue.png", renderer);

    }
    bool cal_distance_ex(float playerx, float playery, float x, float y)
    {
        float delta_x = playerx + 32 - x - 16;
        float delta_y = playery + 32 - y - 16;
        float distance = sqrt(delta_x * delta_x + delta_y * delta_y);


        // Kiểm tra nếu khoảng cách nhỏ hơn hoặc bằng tổng bán kính
        if (distance <= 48)
        {
            return true;
        }
        return false;
    }
    // Hàm vẽ player trên màn hình
    void render(SDL_Renderer* renderer, SDL_Texture* indicatorTexture) {
        // Vẽ nhân vật
        SDL_Rect dstRect = { x, y, width, height };
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);

        // Nếu player này đang được điều khiển, vẽ `red.png`
        if (tar || tar2) {
            int indicatorWidth, indicatorHeight;
            SDL_QueryTexture(indicatorTexture, NULL, NULL, &indicatorWidth, &indicatorHeight);

            // Tọa độ của chỉ định `red.png` (vẽ lệch lên 32 pixel theo yêu cầu)
            SDL_Rect dstRect1 = { x + 16, y - 32, indicatorWidth, indicatorHeight };

            // Vẽ `red.png` để chỉ định nhân vật đang được điều khiển
            SDL_RenderCopy(renderer, indicatorTexture, NULL, &dstRect1);
        }
    }
    void update(Player* player, float targetX, float targetY) {
        if (cal_distance_ex(x + velocityX, y + velocityY, targetX, targetY))
        {
            return;
        }
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
    void handleInput_P2(const Uint8* keyState, float targetX, float targetY) {
        if (!tar2) return;
        if (tar) return;


        if (keyState[SDL_SCANCODE_UP]) {
            if (y - speed - 64 >= 0 && !collision_top)
                y -= speed;  // Di chuyển lên
        }
        if (keyState[SDL_SCANCODE_DOWN]) {
            if (y + speed + 32 <= screenHeight && !collision_down)
                y += speed;  // Di chuyển xuống
        }
        if (keyState[SDL_SCANCODE_LEFT] && !collision_left) {
            if (x - speed >= 0)
                x -= speed;  // Di chuyển sang trái
        }
        if (keyState[SDL_SCANCODE_RIGHT]) {
            if (x + speed + 64 <= screenWidth && !collision_right)
                x += speed;  // Di chuyển sang phải
        }

        if (keyState[SDL_SCANCODE_KP_ENTER]) {
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
    void handleInput(const Uint8* keyState, float targetX, float targetY) {
        if (!tar) return;
        if (tar2) return;


        if (keyState[SDL_SCANCODE_W] && tar) {
            if(y - speed - 64 >= 0 && !collision_top)
                y -= speed;  // Di chuyển lên
        }
        if (keyState[SDL_SCANCODE_S] && tar) {
            if ( y + speed + 32 <= screenHeight && !collision_down)
                y += speed;  // Di chuyển xuống
        }
        if (keyState[SDL_SCANCODE_A] && !collision_left && tar) {
            if( x - speed >= 0 )
                x -= speed;  // Di chuyển sang trái
        }
        if (keyState[SDL_SCANCODE_D] && tar) {
            if( x+ speed + 64 <= screenWidth && !collision_right)
                x += speed;  // Di chuyển sang phải
        }

        if (keyState[SDL_SCANCODE_SPACE] && tar) {
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
        float speed = 0.6f;  // Tốc độ di chuyển về phía quả bóng
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

    void moveTowards(float targetX, float targetY) {

        // Tính khoảng cách từ vị trí hiện tại đến điểm đích
        float dx = targetX - this->x;
        float dy = targetY - this->y;

        // Tính độ dài của vector chỉ hướng
        float distance = std::sqrt(dx * dx + dy * dy);

        // Kiểm tra nếu đã tới gần điểm đích (hoặc đã tới)
        if (distance < 1e-3) {
            // Đặt vị trí chính xác là điểm đích nếu quá gần
            this->x = targetX;
            this->y = targetY;
            return;
        }

        // Tính hướng di chuyển (vector đơn vị)
        float directionX = dx / distance;
        float directionY = dy / distance;

        // Cập nhật vị trí dựa trên tốc độ
        this->x += directionX * speed;
        this->y += directionY * speed;

        // Kiểm tra nếu vượt qua điểm đích thì dừng tại điểm đích
        if (std::abs(targetX - this->x) < speed) this->x = targetX;
        if (std::abs(targetY - this->y) < speed) this->y = targetY;
    }
    
    void moveTowards(float targetX, float targetY, float speed) {
        if (cal_distance_ex(x, y, targetX, targetY) )
        {
            return;
        }
        // Tính khoảng cách từ vị trí hiện tại đến điểm đích
        float dx = targetX - this->x;
        float dy = targetY - this->y;

        // Tính độ dài của vector chỉ hướng
        float distance = std::sqrt(dx * dx + dy * dy);

        // Kiểm tra nếu đã tới gần điểm đích (hoặc đã tới)
        if (distance < 1e-3) {
            // Đặt vị trí chính xác là điểm đích nếu quá gần
            this->x = targetX;
            this->y = targetY;
            return;
        }

        // Tính hướng di chuyển (vector đơn vị)
        float directionX = dx / distance;
        float directionY = dy / distance;

        // Cập nhật vị trí dựa trên tốc độ
        this->x += directionX * speed;
        this->y += directionY * speed;

        // Kiểm tra nếu vượt qua điểm đích thì dừng tại điểm đích
        if (std::abs(targetX - this->x) < speed) this->x = targetX;
        if (std::abs(targetY - this->y) < speed) this->y = targetY;
    }

    
    void move(string nameArea, float targetX, float targetY)
    {
        // khúc này mệt vcl
        // nhớ xử lý cho bot hoặc P2 CM1 LCB1 RCB1 LW1 RW1
        if (cal_distance_ex(x, y, targetX, targetY))
        {
            return;
        }
        if (!tar && player_type != "CM" && player_type != "CM1" &&!tar2)
        {
            if (nameArea == "A")
            {
                // Xử lý cho khu vực A
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(233, 310);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(108, 310);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(108, 450);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(358, 205);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(358, 450);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(233, 150);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(483, 315);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(358, 450);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(733, 255);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(608, 450);
                }
            }
            else if (nameArea == "B")
            {
                // Xử lý cho khu vực B
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(388, 310);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(263,310);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(233, 450);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(388, 205);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(358, 450);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(483, 205);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(608, 590);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(733, 310);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(733, 205);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(855, 450);
                }
            }
            else if (nameArea == "C")
            {
                // Xử lý cho khu vực C
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(608, 450);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(358, 310);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(358, 450);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(733, 205);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(733, 590);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(858, 205);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(733, 310);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(858, 310);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(983, 310);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(983, 450);
                }
            }
            else if (nameArea == "D")
            {
                // Xử lý cho khu vực D
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(858, 310);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(733, 205);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(608, 450);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(983, 150);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(858, 590);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(858, 205);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(733, 310);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(983, 310);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(1108-64, 310-64);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(983, 450);
                }
            }
            else if (nameArea == "E")
            {
                // Xử lý cho khu vực E
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(108, 310);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(108, 400);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(108, 550);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(205, 258);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(233, 450);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(300, 310);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(300, 590);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(233, 450);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(608, 310);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(608, 590);
                }
            }
            else if (nameArea == "F")
            {
                // Xử lý cho khu vực F
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(358, 450);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(233, 310);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(108, 450);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(483, 310);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(483, 590);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(483, 310);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(483, 590);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(608, 450);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(733, 310);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(858, 590);
                }
            }
            else if (nameArea == "G")
            {
                // Xử lý cho khu vực G
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(608, 460);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(483, 320);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(358, 460);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(733, 320);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(858, 600);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(858, 310);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(858, 590);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(858-64, 450-64);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(983, 310);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(983, 590);
                }
            }
            else if (nameArea == "H")
            {
                // Xử lý cho khu vực H
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(858, 450);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(608, 360);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(608, 550);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(983, 310);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(983, 590);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(983+64, 310+64);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(983, 590);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(1000, 450);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(1108-64, 310-64);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(1108-64, 590-63);
                }
            }
            else if (nameArea == "I")
            {
                // Xử lý cho khu vực I
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(233, 590);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(108, 450);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(108, 590);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(233, 450);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(233, 695);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(358, 590);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(233, 695);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(358, 695);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(608, 450);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(608, 695);
                }
            }
            else if (nameArea == "J")
            {
                // Xử lý cho khu vực J
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(358, 590);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(250, 450);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(358, 695);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(483, 500);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(483, 700);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(483, 630);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(483, 750);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(608, 695);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(608, 590);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(733, 630);
                }
            }
            else if (nameArea == "K")
            {
                // Xử lý cho khu vực K
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(608, 450);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(358, 310);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(358, 450);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(733, 205);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(733, 590);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(608, 450);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(733, 685);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(733, 590);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(858, 450);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(983, 590);
                }
            }
            else if (nameArea == "L"){
                // Xử lý cho khu vực L
                if (player_type == "CM")
                {
                    // Xử lý cho player CM
                    moveTowards(733, 590);
                }
                else if (player_type == "LCB")
                {
                    // Xử lý cho player LCB
                    moveTowards(608, 450);
                }
                else if (player_type == "RCB")
                {
                    // Xử lý cho player RCB
                    moveTowards(608, 695);
                }
                else if (player_type == "LW")
                {
                    // Xử lý cho player LW
                    moveTowards(983, 590);
                }
                else if (player_type == "RW")
                {
                    // Xử lý cho player RW
                    moveTowards(983, 700);
                }
                else if (player_type == "LW1")
                {
                    moveTowards(858, 590);
                }
                else if (player_type == "RW1")
                {
                    moveTowards(858, 685);
                }
                else if (player_type == "CM1")
                {
                    moveTowards(983, 450);
                }
                else if (player_type == "LCB1")
                {
                    moveTowards(1108, 450);
                }
                else if (player_type == "RCB1")
                {
                    moveTowards(983, 630);
                }
            }

        }
        

    }

    void setPosition(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

}
;
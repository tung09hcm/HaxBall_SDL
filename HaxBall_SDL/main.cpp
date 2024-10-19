#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include "Common.h"
#include "Player.h"
#include "Map.h"
#include "Ball.h"
#include "Cal.h"
using namespace std;

// Hàm khởi tạo SDL
bool init(SDL_Window*& window, SDL_Renderer*& renderer, const int width, const int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Không thể khởi tạo SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("HaxBAll", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Không thể tạo cửa sổ: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Không thể tạo renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "Không thể khởi tạo SDL_image: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}
void drawThickRect(SDL_Renderer* renderer, SDL_Rect rect, int thickness) {
    // Vẽ nhiều hình chữ nhật giảm dần kích thước để tạo viền dày
    for (int i = 0; i < thickness; ++i) {
        SDL_RenderDrawRect(renderer, &rect);
        rect.x += 1;
        rect.y += 1;
        rect.w -= 2;
        rect.h -= 2;
    }
}
void switchPlayer(const Uint8* keyState, vector<Player*>& Players, bool& switchPressed) {  // Pass by reference
    if (keyState[SDL_SCANCODE_F]) {
        if (!switchPressed) {  // Only switch if the key was not pressed in the last frame
            cout << "PRESS F" << endl;
            cout << "Players size: " << Players.size() << endl;

            if (Players.size() <= 1) return;  // No need to switch if only one player

            for (size_t i = 0; i < Players.size(); i++) {
                if (Players[i]->tar == true) {  // Found the active player
                    Players[i]->tar = false;    // Deactivate current player

                    // Activate the next player, wrapping to the start if at the end of the list
                    if (i + 1 < Players.size()) {
                        Players[i + 1]->tar = true;
                    }
                    else {
                        Players[0]->tar = true;  // Wrap around to the first player
                    }

                    break;  // Exit after switching players
                }
            }
            switchPressed = true;  // Set the flag to indicate the key is held down
        }
    }
    else {
        switchPressed = false;  // Reset the flag when the key is released
    }
}





int main(int argc, char* args[]) {

    // Khởi tạo SDL
    if (!init(window, renderer, screenWidth, screenHeight)) {
        std::cerr << "Khởi tạo thất bại!" << std::endl;
        return -1;
    }
    if (renderer == nullptr)
    {
        cout << "ERROR renderer is nullptr" << endl;
    }



    SDL_Event e;
    SDL_Rect rect = { 108, 100, 1000, 700 };
    SDL_Rect rect_ext = { 108, 100, 500, 700 };
    SDL_Rect goal_left = { 68,300,50,300 };
    SDL_Rect goal_right = { 1098,300,50,300 };
    int thickness = 10;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Tạo đối tượng Player
    vector<Player*> Players;

    // 1 Goalkeeper
    Player* goalkeeper = new Player(100, screenHeight / 2 , "germany.png");  // Positioned near the left middle of the screen
    Players.push_back(goalkeeper);

    // 2 Defenders
    Player* defender1 = new Player(300, screenHeight / 3, "italy.png");  // Positioned slightly to the right and higher than the goalkeeper
    Player* defender2 = new Player(300, 2 * screenHeight / 3, "france.png");  // Positioned slightly to the right and lower than the goalkeeper
    Players.push_back(defender1);
    Players.push_back(defender2);

    // 2 Forwards
    Player* forward1 = new Player(500, screenHeight / 4, "spain.png");  // Positioned further to the right and higher
    Player* forward2 = new Player(500, 3 * screenHeight / 3 - screenHeight / 4, "argentina.png");  // Positioned further to the right and lower
    Players.push_back(forward1);
    Players.push_back(forward2);


    Ball* ball = new Ball(screenWidth / 2 - 64, screenHeight / 2 - 32);
    bool quit = false;
    // Init Map
    initMap("Green.png");
    forward1->tar = true;
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    bool switchPressed = false;
    // Game loop
    while (!quit) {
        // Kiểm tra sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        // Lấy trạng thái bàn phím
        for (Player* x : Players)
        {
            x->handleInput(keyState);
        }

        switchPlayer(keyState, Players, switchPressed);

        // Xử lý input
        

        
        /*-> dùng vòng lặp hết player */
        
        // Xóa màn hình
        SDL_RenderClear(renderer);
        // Vẽ Map
        loadMap();

        // Vẽ hình chữ nhật chỉ có viền
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu viền đỏ
        drawThickRect(renderer, rect, thickness);
        drawThickRect(renderer, goal_left, thickness);
        drawThickRect(renderer, goal_right, thickness);
        drawThickRect(renderer, rect_ext, thickness);
        drawCircleWithBorder(renderer, 604, 450, 100, 3);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // Vẽ đường thẳng (từ điểm x1, y1 đến x2, y2)
        SDL_RenderDrawLine(renderer, 108, 100, 108, 800);
        SDL_RenderDrawLine(renderer, 108, 100, 1108, 100);
        SDL_RenderDrawLine(renderer, 108, 800, 1108, 800);
        SDL_RenderDrawLine(renderer, 1108, 100, 1108, 800);

        SDL_RenderDrawLine(renderer, 78, 310, 1138, 310);
        SDL_RenderDrawLine(renderer, 78, 590, 1138, 590);
        SDL_RenderDrawLine(renderer, 78, 310, 78, 590);
        SDL_RenderDrawLine(renderer, 1138, 310, 1138, 590);

        //////////////////////////////////////////////////////////////////////////////
        // va chạm giữa người chơi và quả bóng
        for (Player* x : Players)
        {

            // Coordinates of the ball (replace with actual ball coordinates)
            float ballX = ball->x;
            float ballY = ball->y;

            // Coordinates of the goal (replace with actual goal coordinates)
            float goalX = screenWidth;  // Right side of the screen
            float goalY = screenHeight / 2;  // Center of the screen height

            // Make each player aim for the ball and shoot
            x->moveToBallAndShoot(ballX, ballY, goalX, goalY);

            if (cal_distance(x, ball)) {

                // player->tar = true;
                ball->tar = true;
                x->collision = true;

                // ballx và bally là tâm bóng
                float ballx = ball->x + 16;
                float bally = ball->y + 16;

                // xx và xy là tâm người chơi
                float xx = x->x + 32;
                float xy = x->y + 32;

                if (ballx > xx + 32) x->collision_right = true;
                if (ballx < xx - 32) x->collision_left = true;
                if (bally > xy + 32) x->collision_down = true;
                if (bally < xy - 32) x->collision_top = true;

                // Tính toán hướng từ Player đến Ball
                float dx = ball->x - x->x;
                float dy = ball->y - x->y;

                // Chuẩn hóa vector hướng
                float length = sqrt(dx * dx + dy * dy);

                if (length > 0) {
                    dx /= length;
                    dy /= length;
                }

                // Áp dụng lực đẩy cho Ball
                float pushForce = 0.5f;  // Điều chỉnh độ lớn lực đẩy theo ý muốn
                ball->applyForce(dx * pushForce, dy * pushForce);
                
                
            }
            else {
                // player->tar = false;
                ball->tar = false;
                x->collision = false;
                x->collision_left = false;
                x->collision_right = false;
                x->collision_down = false;
                x->collision_top = false;
            }
            // Chọn texture đúng cho từng player
            // hàm này chỉ vẽ cho x nào đg có thuộc tính tar là true
            ball->update(x);
            x->render(renderer, x->redTexture);
            
             
        }
        //////////////////////////////////////////////////////////////////////////////
        
        //////////////////////////////////////////////////////////////////////////////
        // va chạm giữa các người chơi
        for (size_t i = 0; i < Players.size(); i++) {
            for (size_t j = i + 1; j < Players.size(); ++j) {
                Player* player1 = Players[i];
                Player* player2 = Players[j];
                if (cal_distance(player1, player2))
                {
                    float player1x = player1->x + 32;
                    float player1y = player1->y + 32;

                    float player2x = player2->x + 32;
                    float player2y = player2->y + 32;

                    if (player2x > player1x + 32) player1->collision_right = true;
                    if (player2x < player1x - 32) player1->collision_left = true;
                    if (player2y > player1y + 32) player1->collision_down = true;
                    if (player2y < player1y - 32) player1->collision_top = true;

                    // Tính toán hướng từ các player
                    float dx = player1->x - player2->x;
                    float dy = player1->y - player2->y;

                    // Chuẩn hóa vector hướng
                    float length = sqrt(dx * dx + dy * dy);

                    if (length > 0) {
                        dx /= length;
                        dy /= length;
                    }

                    float pushForce = 0.99f;  
                    player1->applyForce(dx * pushForce, dy * pushForce);  // Đẩy player1 đi xa player2
                    player2->applyForce(-dx * pushForce, -dy * pushForce); // Đẩy player2 đi xa player1

                    // Cập nhật trạng thái của cả hai người chơi
                    player1->update(player2);
                    player2->update(player1);

                }
                else {
                    // player->tar = false;
                    player1->collision = false;
                    player1->collision_left = false;
                    player1->collision_right = false;
                    player1->collision_down = false;
                    player1->collision_top = false;
                }
            }
        }
        //////////////////////////////////////////////////////////////////////////////
        ball->render(renderer);
        SDL_RenderPresent(renderer);
        ;

        // Cập nhật màn hình
       
    }

    // Dọn dẹp
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    for (Player* x : Players) {
        delete x;
    }

    delete ball;
    return 0;
}

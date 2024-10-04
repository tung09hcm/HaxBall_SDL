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

    // Tạo đối tượng Player
    Player *player = new Player(screenWidth / 4 - 32, screenHeight / 2 - 32, "argentina.png");
    Player* player1 = new Player(screenWidth / 6 - 32, screenHeight / 2 - 32, "spain.png");

    vector<Player* > Players;
    Players.push_back(player);
    Players.push_back(player1);

    Ball *ball = new Ball(screenWidth / 2 - 64, screenHeight / 2 - 32);
    bool quit = false;

    SDL_Event e;
    SDL_Rect rect = { 108, 100, 1000, 700 };
    SDL_Rect rect_ext = { 108, 100, 500, 700 };
    SDL_Rect goal_left = { 68,300,50,300 };
    SDL_Rect goal_right = { 1098,300,50,300 };
    int thickness = 10;
    // Init Map
    initMap("Green.png");
    // Game loop
    while (!quit) {
        // Kiểm tra sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Lấy trạng thái bàn phím
        const Uint8* keyState = SDL_GetKeyboardState(NULL);

        // Xử lý input
        player->handleInput(keyState); player->tar = true;
        player1->handleInput(keyState);


        /*-> dùng vòng lặp hết player */
        ball->update(player);
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
        // SDL_RenderDrawLine(renderer, 108, 100, 108, 800);
        // SDL_RenderDrawLine(renderer, 108, 100, 1108, 100);
        // SDL_RenderDrawLine(renderer, 108, 800, 1108, 800);
        // SDL_RenderDrawLine(renderer, 1108, 100, 1108, 800);

        // SDL_RenderDrawLine(renderer, 78, 310, 1138, 310);
        // SDL_RenderDrawLine(renderer, 78, 590, 1138, 590);
        // SDL_RenderDrawLine(renderer, 78, 310, 78, 590);
        // SDL_RenderDrawLine(renderer, 1138, 310, 1138, 590);
        for (Player* x : Players)
        {
            if (cal_distance(x, ball)) {

                // player->tar = true;
                ball->tar = true;
                x->collision = true;

                float ballx = ball->x + 16;
                float bally = ball->y + 16;
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
                float pushForce = 0.98f;  // Điều chỉnh độ lớn lực đẩy theo ý muốn
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
            x->render(renderer, x->redTexture);
        }


        ball->render(renderer);

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);
    }

    // Dọn dẹp
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    delete player;
    delete ball;
    return 0;
}

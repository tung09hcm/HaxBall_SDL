#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Common.h"
#include "Player.h"
#include "Map.h"
#include "Ball.h"

using namespace std;
bool cal_distance(float player_x, float player_y, float ball_x, float ball_y)
{
    float delta_x = player_x + 32 - ball_x - 16;
    float delta_y = player_y + 32 - ball_y - 16;
    float distance = sqrt(delta_x * delta_x + delta_y * delta_y);

    cout << "distance: " << distance << endl;

    // Kiểm tra nếu khoảng cách nhỏ hơn hoặc bằng tổng bán kính
    if (distance <= 48 )
    {
        std::cout << "Collision detected" << std::endl;
        return true;
    }
    return false;
}
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
    Player player(screenWidth / 4 - 32, screenHeight / 2 - 32, "argentina.png");
    Ball ball(screenWidth / 2 - 64, screenHeight / 2 - 32);
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
        player.handleInput(keyState);
        ball.update();
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
        drawCircleWithBorder(renderer, screenWidth / 2 - 64 + 16, screenHeight / 2 - 32 + 16, 32, 3);
        drawCircleWithBorder(renderer, player.x + 32, player.y + 32, 64, 3);
        cal_distance(player.x,player.y, ball.x, ball.y);
        
        player.render(renderer);
        ball.render(renderer);
        

        // Vẽ player
        

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);
    }

    // Dọn dẹp
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}

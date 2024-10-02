#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Common.h"
#include "Player.h"
#include "Map.h"
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
// Hàm tải texture từ file PNG


int main(int argc, char* args[]) {

    // Khởi tạo SDL
    if (!init(window, renderer, screenWidth, screenHeight)) {
        std::cerr << "Khởi tạo thất bại!" << std::endl;
        return -1;
    }


    // Tạo đối tượng Player
    Player player(screenWidth / 2, screenHeight / 2, "argentina.png");

    bool quit = false;
    SDL_Event e;

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
        SDL_Rect rect = { 108, 100, 1000, 700 };
        // Xử lý input
        player.handleInput(keyState);

        // Xóa màn hình
        SDL_RenderClear(renderer);
        // Vẽ Map
        loadMap();
        // Đặt màu vẽ là màu trắng
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu trắng

        // Vẽ hình chữ nhật chỉ có viền
        SDL_RenderDrawRect(renderer, &rect);
        // Vẽ player
        player.render(renderer);

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

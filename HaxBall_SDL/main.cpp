#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cmath> 

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool isRunning = true;

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    // Vẽ hình tròn sử dụng thuật toán midpoint circle
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // khoảng cách theo chiều ngang từ tâm
            int dy = radius - h; // khoảng cách theo chiều dọc từ tâm
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                // Vẽ pixel tại (centerX + dx, centerY + dy) nếu nằm trong hình tròn
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}


bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(title, xpos, ypos, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        if (window) {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else {
                std::cout << "Renderer creation failed: " << SDL_GetError() << std::endl;
                return false;
            }
        }
        else {
            std::cout << "Window creation failed: " << SDL_GetError() << std::endl;
            return false;
        }
        return true;
    }
    else {
        std::cout << "SDL Initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }
}
void handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
        // Xử lý các sự kiện khác
    default:
        break;
    }
}
void update() {
    // Cập nhật trạng thái của game
}
void render() {
    // Xóa màn hình với màu đen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Đặt màu đen
    SDL_RenderClear(renderer); // Xóa màn hình

    // Thiết lập màu vẽ cho hình tròn (ví dụ: màu trắng)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Vẽ hình tròn với bán kính 32 pixel, tọa độ tâm (100, 100)
    drawCircle(renderer, 100, 100, 32);

    // Hiển thị lên màn hình
    SDL_RenderPresent(renderer);
}
void clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned" << std::endl;
}




int main(int argc, char* args[]) {
    if (init("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false)) {
        while (isRunning) {
            handleEvents();
            update();
            render();
        }
    }
    else {
        std::cout << "Game failed to initialize" << std::endl;
    }
    clean();
    return 0;
}

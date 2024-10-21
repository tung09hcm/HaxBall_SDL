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
#include <SDL_ttf.h>
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
bool isMouseInRect(int mouseX, int mouseY, SDL_Rect rect) {
    // Kiểm tra nếu vị trí chuột nằm trong hình chữ nhật
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
        mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

void switchPlayer(const Uint8* keyState, vector<Player*>& Players, bool& switchPressed, Ball* ball) {  // Thêm tham số screenWidth
    if (keyState[SDL_SCANCODE_F]) {
        if (!switchPressed) {  // Chỉ chuyển đổi nếu phím chưa được nhấn ở frame trước
            cout << "PRESS F" << endl;
            cout << "Players size: " << Players.size() << endl;

            if (Players.size() <= 1) return;  // Không cần chuyển đổi nếu chỉ có 1 player

            bool canSwitch = true;

            // Kiểm tra vị trí của bóng, chỉ cho phép chuyển nếu bóng > screenWidth / 2
            if (ball->x > screenWidth / 2) {
                canSwitch = false;

                // Tìm player hiện tại và chuyển sang player khác nếu không phải LCB hoặc RCB
                for (size_t i = 0; i < Players.size(); i++) {
                    if (Players[i]->tar == true) {  // Tìm player hiện tại
                        Players[i]->tar = false;    // Ngưng kích hoạt player hiện tại

                        // Tìm player tiếp theo có thuộc tính player_type != LCB và != RCB
                        size_t nextPlayerIndex = (i + 1) % Players.size();
                        for (size_t j = 0; j < Players.size(); j++) {
                            size_t candidateIndex = (nextPlayerIndex + j) % Players.size();
                            if (Players[candidateIndex]->player_type != "LCB" && Players[candidateIndex]->player_type != "RCB") {
                                Players[candidateIndex]->tar = true;  // Kích hoạt player mới
                                canSwitch = true;
                                break;  // Thoát khỏi vòng lặp sau khi tìm được player hợp lệ
                            }
                        }

                        break;  // Thoát khỏi vòng lặp sau khi xử lý xong player hiện tại
                    }
                }
            }
            else {
                // Chuyển đổi như bình thường nếu bóng không nằm ở phần phải màn hình
                for (size_t i = 0; i < Players.size(); i++) {
                    if (Players[i]->tar == true) {  // Tìm player hiện tại
                        Players[i]->tar = false;    // Ngưng kích hoạt player hiện tại

                        // Kích hoạt player tiếp theo
                        size_t nextPlayerIndex = (i + 1) % Players.size();
                        Players[nextPlayerIndex]->tar = true;

                        break;  // Thoát khỏi vòng lặp sau khi chuyển đổi
                    }
                }
            }

            if (canSwitch) {
                switchPressed = true;  // Đặt cờ để chỉ ra rằng phím đã được giữ
            }
        }
    }
    else {
        switchPressed = false;  // Reset cờ khi phím được nhả ra
    }
}
void switchPlayer_P2(const Uint8* keyState, vector<Player*>& Players, bool& switchPressed, Ball* ball) {  // Thêm tham số screenWidth
    if (keyState[SDL_SCANCODE_KP_0]) {
        if (!switchPressed) {  // Chỉ chuyển đổi nếu phím chưa được nhấn ở frame trước
            cout << "PRESS 0" << endl;
            cout << "Players size: " << Players.size() << endl;

            if (Players.size() <= 1) return;  // Không cần chuyển đổi nếu chỉ có 1 player

            bool canSwitch = true;

            // Kiểm tra vị trí của bóng, chỉ cho phép chuyển nếu bóng > screenWidth / 2
            if (ball->x < screenWidth / 2) {
                canSwitch = false;

                // Tìm player hiện tại và chuyển sang player khác nếu không phải LCB hoặc RCB
                for (size_t i = 0; i < Players.size(); i++) {
                    if (Players[i]->tar2 == true) {  // Tìm player hiện tại
                        Players[i]->tar2 = false;    // Ngưng kích hoạt player hiện tại

                        // Tìm player tiếp theo có thuộc tính player_type != LCB và != RCB
                        size_t nextPlayerIndex = (i + 1) % Players.size();
                        for (size_t j = 0; j < Players.size(); j++) {
                            size_t candidateIndex = (nextPlayerIndex + j) % Players.size();
                            if (Players[candidateIndex]->player_type != "LCB1" && Players[candidateIndex]->player_type != "RCB1") {
                                Players[candidateIndex]->tar2 = true;  // Kích hoạt player mới
                                canSwitch = true;
                                break;  // Thoát khỏi vòng lặp sau khi tìm được player hợp lệ
                            }
                        }

                        break;  // Thoát khỏi vòng lặp sau khi xử lý xong player hiện tại
                    }
                }
            }
            else {
                // Chuyển đổi như bình thường nếu bóng không nằm ở phần phải màn hình
                for (size_t i = 0; i < Players.size(); i++) {
                    if (Players[i]->tar2 == true) {  // Tìm player hiện tại
                        Players[i]->tar2 = false;    // Ngưng kích hoạt player hiện tại

                        // Kích hoạt player tiếp theo
                        size_t nextPlayerIndex = (i + 1) % Players.size();
                        Players[nextPlayerIndex]->tar2 = true;

                        break;  // Thoát khỏi vòng lặp sau khi chuyển đổi
                    }
                }
            }

            if (canSwitch) {
                switchPressed = true;  // Đặt cờ để chỉ ra rằng phím đã được giữ
            }
        }
    }
    else {
        switchPressed = false;  // Reset cờ khi phím được nhả ra
    }
}
bool isPointInsideRectangle(float x, float y, float rectLeft, float rectTop, float rectRight, float rectBottom) {
    // Kiểm tra nếu x nằm giữa rectLeft và rectRight
    if (x >= rectLeft && x <= rectRight) {
        // Kiểm tra nếu y nằm giữa rectTop và rectBottom
        if (y >= rectTop && y <= rectBottom) {
            return true;  // Điểm nằm trong hình chữ nhật
        }
    }
    return false;  // Điểm nằm ngoài hình chữ nhật
}
string getBallAreaName(float x, float y)
{
    if (isPointInsideRectangle(x, y, 108, 100, 358, 310)) return "A";
    if (isPointInsideRectangle(x, y, 358, 100, 608, 310)) return "B";
    if (isPointInsideRectangle(x, y, 608, 100, 858, 310)) return "C";
    if (isPointInsideRectangle(x, y, 858, 100, 1108, 310)) return "D";

    if (isPointInsideRectangle(x, y, 108, 310, 358, 590)) return "E";
    if (isPointInsideRectangle(x, y, 358, 310, 608, 590)) return "F";
    if (isPointInsideRectangle(x, y, 608, 310, 858, 590)) return "G";
    if (isPointInsideRectangle(x, y, 858, 310, 1108, 590)) return "H";

    if (isPointInsideRectangle(x, y, 108, 590, 358, 800)) return "I";
    if (isPointInsideRectangle(x, y, 358, 590, 608, 800)) return "J";
    if (isPointInsideRectangle(x, y, 608, 590, 858, 800)) return "K";
    if (isPointInsideRectangle(x, y, 858, 590, 1108, 800)) return "L";

}

bool goalCheckLeft(Ball* ball)
{
    float x = ball->x + 16;
    float y = ball->y + 16;
    // check GOAL LEFT
    if (x < 108)
    {
        cout << "Goal Left " << endl;
        return true;
    }
    else return false;
}
bool goalCheckRight(Ball* ball)
{
    float x = ball->x;
    float y = ball->y;
    // check GOAL LEFT
    if (x > 1108)
    {
        cout << "Goal Right " << endl;
        return true;
    }
    else return false;
}

void renderText(SDL_Renderer* renderer, const std::string& message, TTF_Font* font, SDL_Color color, int x, int y) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* messageTexture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect messageRect;
    messageRect.x = x;
    messageRect.y = y;
    messageRect.w = surfaceMessage->w;
    messageRect.h = surfaceMessage->h;

    SDL_RenderCopy(renderer, messageTexture, NULL, &messageRect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(messageTexture);
}
void reDraw(vector<Player*>& Players, Ball* ball)
{
    for (Player* x : Players)
    {
        if (x->player_type == "CM")
        {
            x->setPosition(400, screenHeight / 2);
        }
        else if (x->player_type == "RW")
        {
            x->setPosition(500, 3 * screenHeight / 3 - screenHeight / 4);
        }
        else if (x->player_type == "LW")
        {
            x->setPosition(500, screenHeight / 4);  
        }
        else if (x->player_type == "RCB")
        {
            x->setPosition(200, 2 * screenHeight / 3);
        }
        else if (x->player_type == "LCB")
        {
            x->setPosition(200, screenHeight / 3);
        }
        else if (x->player_type == "CM1")
        {
            x->setPosition(screenWidth - 400, screenHeight / 2);
        }
        else if (x->player_type == "RW1")
        {
            x->setPosition(screenWidth - 500, 3 * screenHeight / 3 - screenHeight / 4);
        }
        else if (x->player_type == "LW1")
        {
            x->setPosition(screenWidth - 500, screenHeight / 4);
        }
        else if (x->player_type == "RCB1")
        {
            x->setPosition(screenWidth - 200, 2 * screenHeight / 3);
        }
        else if (x->player_type == "LCB1")
        {
            x->setPosition(screenWidth - 200, screenHeight / 3);
        }
    }
    ball->setPosition(608 - 16, 450 - 16);
}
int main(int
    argc, char* args[]) {

    // Khởi tạo SDL
    if (!init(window, renderer, screenWidth, screenHeight)) {
        std::cerr << "Khởi tạo thất bại!" << std::endl;
        return -1;
    }
    if (renderer == nullptr)
    {
        cout << "ERROR renderer is nullptr" << endl;
    }
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("sport.ttf", 72);  // 72 is the font size for the title
    TTF_Font* font_small = TTF_OpenFont("sport.ttf", 50);  // 72 is the font size for the title
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return -1;
    }
    cout << "tín hiệu" << endl;
    SDL_Event e;
    SDL_Rect rect = { 108, 100, 1000, 700 };
    SDL_Rect rect_ext = { 108, 100, 500, 700 };
    SDL_Rect goal_left = { 68,300,50,300 };
    SDL_Rect goal_right = { 1098,300,50,300 };
    int thickness = 10;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Tạo đối tượng Player
    vector<Player*> Players;
    vector<Player*> P1;
    vector<Player*> P2;

    // 1 CentralMid
    Player* centralmid = new Player(400, screenHeight / 2 , "argentina.png" ,"CM");  // Positioned near the left middle of the screen
    Player* centralmid_ = new Player(screenWidth - 400, screenHeight / 2, "germany.png", "CM1");  // Positioned near the left middle of the screen
    Players.push_back(centralmid);
    Players.push_back(centralmid_);
    P1.push_back(centralmid);
    P2.push_back(centralmid_);

    // 2 Defenders
    Player* defender1 = new Player(200, screenHeight / 3, "argentina.png", "LCB");  // Positioned slightly to the right and higher than the goalkeeper
    Player* defender2 = new Player(200, 2 * screenHeight / 3, "argentina.png" , "RCB");  // Positioned slightly to the right and lower than the goalkeeper
    Player* defender1_ = new Player(screenWidth - 200, screenHeight / 3, "germany.png", "LCB1");  // Positioned slightly to the right and higher than the goalkeeper
    Player* defender2_ = new Player(screenWidth - 200, 2 * screenHeight / 3, "germany.png", "RCB1");  // Positioned slightly to the right and lower than the goalkeeper
    Players.push_back(defender1);
    Players.push_back(defender2);
    Players.push_back(defender1_);
    Players.push_back(defender2_);
    P1.push_back(defender1);
    P1.push_back(defender2);
    P2.push_back(defender1_);
    P2.push_back(defender2_);


    // 2 Forwards
    Player* forward1 = new Player(500, screenHeight / 4, "argentina.png", "LW");  // Positioned further to the right and higher
    Player* forward2 = new Player(500, 3 * screenHeight / 3 - screenHeight / 4, "argentina.png", "RW");  // Positioned further to the right and lower
    Player* forward1_ = new Player(screenWidth - 500, screenHeight / 4, "germany.png", "LW1");  // Positioned further to the right and higher
    Player* forward2_ = new Player(screenWidth - 500, 3 * screenHeight / 3 - screenHeight / 4, "germany.png", "RW1");  // Positioned further to the right and lower
    Players.push_back(forward1);
    Players.push_back(forward2);
    Players.push_back(forward1_);
    Players.push_back(forward2_);
    P1.push_back(forward1);
    P1.push_back(forward2);
    P2.push_back(forward1_);
    P2.push_back(forward2_);


    Ball* ball = new Ball(608 - 16, 450 - 16);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Init Map
    initMap("Green.png");



    forward1->tar = true;
    // forward1_->tar2 = true;
    bool player_vs_player = false;
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    bool switchPressed = false;
    bool switchPressed_P2 = false;
    // Game loop
    bool quit = false;
    bool game_menu_loop = true;
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color red = { 255, 0, 0, 255 }; // đỏ
    SDL_Rect buttonRect = { screenWidth / 2 - 240, screenWidth / 2 - 230, 515, 60 };
    SDL_Rect buttonRect_2 = { screenWidth / 2 - 240, screenWidth / 2 - 100, 515, 60 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    while (game_menu_loop)
    {
        

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                game_menu_loop = false;
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Lấy tọa độ chuột
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Kiểm tra nếu chuột nằm trong khu vực hình chữ nhật
                if (isMouseInRect(mouseX, mouseY, buttonRect)) {
                    std::cout << "Mouse clicked inside the rectangle!" << std::endl;
                    forward1_->tar2 = true;
                    game_menu_loop = false;
                    player_vs_player = true;
                }
                if (isMouseInRect(mouseX, mouseY, buttonRect_2)) {
                    std::cout << "Mouse clicked inside the rectangle 2!" << std::endl;
                    game_menu_loop = false;
                }
            }
        }
        
        // Vẽ Map
        loadMap();
        
        drawThickRect(renderer, buttonRect, 2);
        drawThickRect(renderer, buttonRect_2, 2);
        // drawCircleWithBorder(renderer, 604, 450, 100, 3);
        // Render title "HAXBALL"
        renderText(renderer, "HAXBALL", font, white, screenWidth / 2 - 150, screenHeight / 4);  // Hiển thị tiêu đề

        // Render buttons
        renderText(renderer, "Player vs Player", font_small , white, screenWidth / 2 - 220, screenWidth / 2 - 220);
        renderText(renderer, "Player vs BOT", font_small, white, screenWidth / 2 - 170, screenHeight / 2 + 100);

        // Update screen
        SDL_RenderPresent(renderer);    
        // Xóa màn hình
        SDL_RenderClear(renderer);
    }
    



    int goalLeft = 0;
    int goalRight = 0;
    Uint32 pauseStart = 0;  // Lưu thời gian bắt đầu tạm dừng
    // bool isPaused = false;  // Biến để theo dõi trạng thái tạm dừng
    while (!quit) {
        
        if (goalCheckLeft(ball))
        {
            pauseStart = SDL_GetTicks();
            //isPaused = true;
            cout << " 1GOAL LEFT" << endl;
            reDraw(Players,ball);
            goalRight++;
            // reset 
                
        }
        else if (goalCheckRight(ball))
        {
            pauseStart = SDL_GetTicks();
            // isPaused = true; 
            cout << " 2GOAL RIGHT" << endl;  
            reDraw(Players, ball);
            goalLeft++;
        }

        // Kiểm tra sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        
        // Lấy trạng thái bàn phím
        
        for (Player* x : P1)
        {
            x->handleInput(keyState);
        }
        if (player_vs_player)
        {
            for (Player* x : P2)
            {
                x->handleInput_P2(keyState);
            }
            switchPlayer_P2(keyState, P2, switchPressed_P2, ball);
        }
        
        
        // ball->handleInput(keyState);

        switchPlayer(keyState, P1, switchPressed, ball);
        
        // Xử lý input
        

        
        /*-> dùng vòng lặp hết player */
        
        // Xóa màn hình
        SDL_RenderClear(renderer);
        // Vẽ Map
        loadMap();

        // Vẽ hình chữ nhật chỉ có viền
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu viền trắng
        drawThickRect(renderer, rect, thickness);
        drawThickRect(renderer, goal_left, thickness);
        drawThickRect(renderer, goal_right, thickness);
        drawThickRect(renderer, rect_ext, thickness);
        drawCircleWithBorder(renderer, 604, 450, 100, 3);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // đỏ
        // Vẽ đường thẳng (từ điểm x1, y1 đến x2, y2)
        // Màu viền đỏ
        SDL_RenderDrawLine(renderer, 108, 100, 108, 800);
        SDL_RenderDrawLine(renderer, 108, 100, 1108, 100);
        SDL_RenderDrawLine(renderer, 108, 800, 1108, 800);
        SDL_RenderDrawLine(renderer, 1108, 100, 1108, 800);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // xanh dương

        SDL_RenderDrawLine(renderer, 78, 310, 1138, 310);
        SDL_RenderDrawLine(renderer, 78, 590, 1138, 590);
        SDL_RenderDrawLine(renderer, 78, 310, 78, 590);
        SDL_RenderDrawLine(renderer, 1138, 310, 1138, 590);


        string area = getBallAreaName(ball->x, ball->y);

        //////////////////////////////////////////////////////////////////////////////
        // va chạm giữa người chơi và quả bóng
        for (Player* x : Players)
        {
            x->move(area, ball->x, ball->y);
            x->render(renderer, x->redTexture);
            
            
            // Coordinates of the ball (replace with actual ball coordinates)
            float ballX = ball->x;
            float ballY = ball->y;

            // Coordinates of the goal (replace with actual goal coordinates)
            float goalX = screenWidth;  // Right side of the screen
            float goalY = screenHeight / 2;  // Center of the screen height

            // Make each player aim for the ball and shoot
            // if(!x->tar) x->moveToBallAndShoot(ballX, ballY, goalX, goalY);

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
                float pushForce = 0.2f;  // Điều chỉnh độ lớn lực đẩy theo ý muốn
                ball->applyForce(dx * pushForce, dy * pushForce);
                x->applyForce(-dx * pushForce, -dy * pushForce);
                x->update(x);
                
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
            if ((x->player_type == "CM") || (x->player_type == "CM1") )
            {
                x->moveTowards(ball->x, ball->y, 0.3);
                
            }

            // x->render(renderer, x->grayTexture);
             
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

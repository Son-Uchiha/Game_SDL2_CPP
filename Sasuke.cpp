#define SDL_MAIN_HANDLED // Định nghĩa để tránh lỗi SDL_main khi biên dịch
#include <SDL2/SDL.h> // Thư viện SDL cho đồ họa
#include <SDL2/SDL_image.h> // Thư viện SDL_image cho xử lý hình ảnh
#include <SDL2/SDL_mixer.h> // Thư viện SDL_mixer cho âm thanh
#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h> // Thư viện C++ chuẩn
using namespace std; // Sử dụng không gian tên chuẩn

// Kích thước cửa sổ và các đối tượng
const int SCREEN_WIDTH = 880; // Chiều rộng cửa sổ
const int SCREEN_HEIGHT = 750; // Chiều cao cửa sổ
const int SASUKE_WIDTH = 110; // Chiều rộng của Sasuke
const int SASUKE_HEIGHT = 110; // Chiều cao của Sasuke
const int SHURIKEN_WIDTH = 45; // Chiều rộng của shuriken
const int SHURIKEN_HEIGHT = 39; // Chiều cao của shuriken
const int GAME_OVER_WIDTH = 880; // Chiều rộng màn hình game over
const int GAME_OVER_HEIGHT = 750; // Chiều cao màn hình game over
const int INTRO_WIDTH = 880; // Chiều rộng màn hình giới thiệu
const int INTRO_HEIGHT = 750; // Chiều cao màn hình giới thiệu

// Cấu trúc dữ liệu để lưu thông tin Sasuke và shuriken
struct Sasuke {
    int x, y; // Vị trí của Sasuke
    bool jump; // Trạng thái nhảy
    int jumpHeight; // Chiều cao nhảy
};

struct Shuriken {
    int x, y; // Vị trí của shuriken
};

// Khai báo biến texture và nhạc
SDL_Texture* introTexture; // Texture cho màn hình giới thiệu
SDL_Texture* sasukeTexture; // Texture cho nhân vật Sasuke
SDL_Texture* backgroundTexture; // Texture cho nền
SDL_Texture* shurikenTexture; // Texture cho shuriken
SDL_Texture* gameOverTexture; // Texture cho màn hình game over
Mix_Music* backgroundMusic; // Biến cho nhạc nền
TTF_Font* font;

// Hàm khởi tạo SDL
void init(SDL_Window*& window, SDL_Renderer*& renderer) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); // Khởi động SDL với video và âm thanh
    IMG_Init(IMG_INIT_PNG); // Khởi động SDL_image cho định dạng PNG
    window = SDL_CreateWindow("Sasuke Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0); // Tạo cửa sổ
    renderer = SDL_CreateRenderer(window, -1, 0); // Tạo renderer cho cửa sổ

    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer không thể khởi động: " << Mix_GetError() << endl; // Kiểm tra lỗi khởi động âm thanh
    }

    if (TTF_Init() == -1) { // Khởi tạo SDL_ttf
        cout << "SDL_ttf không thể khởi động: " << TTF_GetError() << endl;
    }
}

// Hàm dọn dẹp tài nguyên
void cleanup(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer); // Giải phóng renderer
    SDL_DestroyWindow(window); // Giải phóng cửa sổ
    SDL_Quit(); // Thoát SDL
    IMG_Quit(); // Thoát SDL_image
     if (font) {
        TTF_CloseFont(font); // Giải phóng font
    }
    TTF_Quit(); 
    Mix_FreeMusic(backgroundMusic); // Giải phóng nhạc
    Mix_CloseAudio(); // Đóng SDL_mixer
}

// Hàm tải textures
void loadTextures(SDL_Renderer* renderer) {
    introTexture = IMG_LoadTexture(renderer, "intro.jpg"); // Tải texture cho màn hình giới thiệu
    sasukeTexture = IMG_LoadTexture(renderer, "sasuke.png"); // Tải texture cho Sasuke
    backgroundTexture = IMG_LoadTexture(renderer, "background.jpg"); // Tải texture cho nền
    shurikenTexture = IMG_LoadTexture(renderer, "shuriken.png"); // Tải texture cho shuriken
    gameOverTexture = IMG_LoadTexture(renderer, "gameover.jpg"); // Tải texture cho màn hình game over
}

// Hàm dọn dẹp textures
void cleanupTextures() {
    SDL_DestroyTexture(introTexture); // Giải phóng texture giới thiệu
    SDL_DestroyTexture(sasukeTexture); // Giải phóng texture Sasuke
    SDL_DestroyTexture(backgroundTexture); // Giải phóng texture nền
    SDL_DestroyTexture(shurikenTexture); // Giải phóng texture shuriken
    SDL_DestroyTexture(gameOverTexture); // Giải phóng texture game over
}

// Hàm tải nhạc
void loadMusic() {
    backgroundMusic = Mix_LoadMUS("y2mate.com - Bước Za Đi Xa nhưng là Anime Opening  Wxrdie Wibu_320kbps.mp3"); // Tải nhạc nền
    if (!backgroundMusic) {
        cout << "Không thể tải nhạc: " << Mix_GetError() << endl; // Kiểm tra lỗi tải nhạc
    }
}

void loadFont() {
    font = TTF_OpenFont("Japanese 3017.ttf", 43); // Tải font từ file
    if (!font) {
        cout << "Không thể tải font: " << TTF_GetError() << endl; // Kiểm tra lỗi tải font
    }
}
// Hàm vẽ màn hình giới thiệu
void drawIntro(SDL_Renderer* renderer) {
    SDL_Rect introRect = { 0, 0, INTRO_WIDTH, INTRO_HEIGHT }; // Vị trí và kích thước màn hình giới thiệu
    SDL_RenderCopy(renderer, introTexture, NULL, &introRect); // Vẽ texture giới thiệu
}

// Hàm vẽ nền  
void drawBackground(SDL_Renderer* renderer) {
    SDL_Rect bgRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // Vị trí và kích thước nền
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgRect); // Vẽ nền
}

// Hàm vẽ Sasuke
void drawSasuke(SDL_Renderer* renderer, const Sasuke& sasuke) {
    SDL_Rect sasukeRect = { sasuke.x, sasuke.y, SASUKE_WIDTH, SASUKE_HEIGHT }; // Vị trí và kích thước của Sasuke
    SDL_RenderCopy(renderer, sasukeTexture, NULL, &sasukeRect); // Vẽ Sasuke
}

// Hàm vẽ shuriken
void drawShuriken(SDL_Renderer* renderer, const Shuriken& shuriken, double angle) {
    SDL_Rect shurikenRect = { shuriken.x, shuriken.y, SHURIKEN_WIDTH, SHURIKEN_HEIGHT }; // Vị trí và kích thước shuriken
    SDL_RenderCopyEx(renderer, shurikenTexture, NULL, &shurikenRect, angle, NULL, SDL_FLIP_NONE); // Vẽ shuriken với góc quay
}

// Hàm kiểm tra va chạm giữa Sasuke và shuriken
bool checkSasukeCollision(const Sasuke& sasuke, const Shuriken& shuriken) {
    SDL_Rect sasukeRect = { sasuke.x, sasuke.y, SASUKE_WIDTH, SASUKE_HEIGHT }; // Hình chữ nhật của Sasuke
    SDL_Rect shurikenRect = { shuriken.x, shuriken.y, SHURIKEN_WIDTH, SHURIKEN_HEIGHT }; // Hình chữ nhật của shuriken
    return SDL_HasIntersection(&sasukeRect, &shurikenRect); // Kiểm tra va chạm
}

// Hàm vẽ màn hình game over
void drawGameOver(SDL_Renderer* renderer) {
    SDL_Rect gameOverRect = { (SCREEN_WIDTH - GAME_OVER_WIDTH) / 2, (SCREEN_HEIGHT - GAME_OVER_HEIGHT) / 2, GAME_OVER_WIDTH, GAME_OVER_HEIGHT }; // Vị trí và kích thước game over
    SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect); // Vẽ màn hình game over
}
void drawText(SDL_Renderer* renderer, const string& text, TTF_Font* font, SDL_Color color, int x, int y) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    
    SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
void gameOverScreen(SDL_Renderer* renderer, TTF_Font* font, Uint32 timeElapsed) {
    SDL_Color textColor = {40, 30, 100};
TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    // Hiển thị điểm trên nền màu xanh
    drawBackground(renderer); 
    // Xóa renderer
    drawText(renderer, "Final Score: " + to_string(timeElapsed), font, textColor, SCREEN_WIDTH / 2 - 230 , SCREEN_HEIGHT / 2 + 140);
    SDL_RenderPresent(renderer); // Cập nhật màn hình
    SDL_Delay(2900); // Dừng 2 giây trước khi chuyển sang màn hình game over

    // Hiển thị màn hình game over với nền xanh
  
    SDL_RenderClear(renderer); // Xóa renderer
    drawGameOver(renderer); // Vẽ màn hình game over

    // Cập nhật màn hình
    SDL_RenderPresent(renderer); // Cập nhật màn hình
    SDL_Delay(2600); // Dừng 3 giây trước khi thoát
}

// Hàm main
int main() {
    SDL_Window* window = nullptr; // Con trỏ đến cửa sổ
    SDL_Renderer* renderer = nullptr; // Con trỏ đến renderer
    init(window, renderer); // Khởi tạo SDL
    loadFont();
    loadTextures(renderer); // Tải textures
    loadMusic(); // Tải nhạc nền
    Mix_PlayMusic(backgroundMusic, -1); // Phát nhạc nền

    // Vẽ màn hình giới thiệu
    SDL_RenderClear(renderer); // Xóa renderer
    drawIntro(renderer); // Vẽ màn hình giới thiệu
    SDL_RenderPresent(renderer); // Cập nhật màn hình
    SDL_Delay(1500); // Dừng 1.5 giây
    SDL_Color textColor = {255, 255, 255};
    // Khởi tạo Sasuke và danh sách shuriken
    Sasuke sasuke = { 0, (SCREEN_HEIGHT - SASUKE_HEIGHT) / 2, false, 0 }; // Vị trí khởi đầu của Sasuke
    vector<Shuriken> shurikens; // Vector lưu trữ shurikens
    bool running = true; // Biến kiểm soát vòng lặp trò chơi

    int timeElapsed = 0; // Thời gian đã trôi qua
    float shurikenSpeed = 6.5; // Tốc độ di chuyển của shuriken
    int lastSpeedIncreaseTime = 500; // Thời gian tăng tốc độ
    int speedIncreaseInterval = 10000; // Khoảng thời gian tăng tốc độ
    int speedIncrement = 1; // Gia tăng tốc độ
    double shurikenAngle = 0.0; // Góc quay của shuriken
    int score = 0;
    srand(static_cast<unsigned int>(time(0))); // Khởi tạo hạt giống cho số ngẫu nhiên

    while (running) { // Vòng lặp trò chơi
        SDL_Event event; // Biến sự kiện
        while (SDL_PollEvent(&event)) { // Xử lý sự kiện
            if (event.type == SDL_QUIT) running = false; // Nếu đóng cửa sổ thì dừng trò chơi
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && !sasuke.jump) {
                sasuke.jump = true; // Bắt đầu nhảy khi nhấn phím space
                sasuke.jumpHeight = 51; // Chiều cao nhảy
            }
        }

        timeElapsed += 20; // Cập nhật thời gian đã trôi qua

        // Tăng tốc độ shuriken sau khoảng thời gian nhất định
        if (timeElapsed - lastSpeedIncreaseTime >= speedIncreaseInterval) {
            shurikenSpeed += speedIncrement; // Tăng tốc độ shuriken
            lastSpeedIncreaseTime = timeElapsed; // Cập nhật thời gian tăng tốc
        }

        // Cập nhật vị trí của Sasuke khi nhảy
        if (sasuke.jump) {
            if (sasuke.jumpHeight > 0) {
                sasuke.y -= 15; // Tăng vị trí Y khi nhảy
                sasuke.jumpHeight -= 15; // Giảm chiều cao nhảy
                sasuke.y -= 5;
            } else {
                sasuke.jump = false; // Kết thúc nhảy
            }
        } else {
                sasuke.y += 8; // Rơi xuống nếu không nhảy
        }

        // Giới hạn vị trí của Sasuke
        if (sasuke.y < 0) {
            sasuke.y = 0; // Không cho phép đi ra ngoài trên
        } else if (sasuke.y > SCREEN_HEIGHT - SASUKE_HEIGHT) {
            sasuke.y = SCREEN_HEIGHT - SASUKE_HEIGHT; // Không cho phép đi ra ngoài dưới
        }

        // Cập nhật vị trí shuriken
        for (auto& shuriken : shurikens) {
            shuriken.x -= shurikenSpeed; // Di chuyển shuriken sang trái
        }

        // Tạo shuriken mới ngẫu nhiên
        if (shurikens.empty() || shurikens.back().x < SCREEN_WIDTH - (rand() % 200 + 100)) {
            int shurikenY; // Vị trí Y của shuriken mới
            int randomZone = rand() % 3; // Chọn khu vực ngẫu nhiên

            // Chọn vị trí Y cho shuriken
            if (randomZone == 0) {
                shurikenY = rand() % (SCREEN_HEIGHT / 3);
            } else if (randomZone == 1) {
                shurikenY = SCREEN_HEIGHT / 3 + rand() % (SCREEN_HEIGHT / 3);
            } else {
                shurikenY = SCREEN_HEIGHT * 2 / 3 + rand() % (SCREEN_HEIGHT / 3);
            }

            shurikens.push_back({ SCREEN_WIDTH, shurikenY }); // Thêm shuriken mới vào danh sách
        }

        // Kiểm tra va chạm giữa Sasuke và shuriken
        for (const auto& shuriken : shurikens) {
          if (checkSasukeCollision(sasuke, shuriken)) {
               running = false; // Dừng trò chơi nếu có va chạm
               break;
            }
        }

        // Xóa shuriken ra ngoài màn hình
        shurikens.erase(remove_if(shurikens.begin(), shurikens.end(), [](const Shuriken& shuriken) { return shuriken.x < -SHURIKEN_WIDTH; }), shurikens.end());

        // Cập nhật góc quay của shuriken
        shurikenAngle -= 10.0; 
        if (shurikenAngle <= -360.0) {
            shurikenAngle += 360.0; // Giới hạn góc quay
        }

        // Vẽ các phần tử trò chơi
        SDL_RenderClear(renderer); // Xóa renderer
        drawBackground(renderer); // Vẽ nền
        drawSasuke(renderer, sasuke); // Vẽ Sasuke

        // Vẽ tất cả shuriken
        for (const auto& shuriken : shurikens) {
            drawShuriken(renderer, shuriken, shurikenAngle); // Vẽ từng shuriken
        }
        string scoreText = "Score: " + to_string(timeElapsed);
        drawText(renderer, scoreText, font, textColor, 20, 30);
        SDL_RenderPresent(renderer); // Cập nhật màn hình
        SDL_Delay(20); // Đợi để điều chỉnh tốc độ khung hình
    }
    gameOverScreen(renderer, font, timeElapsed);
    cleanupTextures(); // Giải phóng textures
    cleanup(window, renderer); // Dọn dẹp tài nguyên
    return 0; // Kết thúc chương trình
}

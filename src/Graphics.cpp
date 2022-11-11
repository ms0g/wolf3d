#include "Graphics.h"

Graphics::Graphics(SDL_Renderer* renderer) {
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA32,
                                SDL_TEXTUREACCESS_STREAMING,
                                WINDOW_WIDTH,
                                WINDOW_HEIGHT);
}

Graphics::~Graphics() {
    SDL_DestroyTexture(texture);
}

void Graphics::Render(SDL_Renderer* renderer) {
    SDL_UpdateTexture(texture,
                      nullptr,
                      colorBuffer.data(),
                      WINDOW_WIDTH * sizeof(color_t));

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Graphics::DrawPixel(int x, int y, color_t color) {
    colorBuffer[(WINDOW_WIDTH * y) + x] = color;
}

void Graphics::Clear(color_t color) {
    colorBuffer.fill(color);
}

void Graphics::DrawRect(int x, int y, int width, int height, color_t color) {
    for (int i = x; i <= (x + width); i++) {
        for (int j = y; j <= (y + height); j++) {
            DrawPixel(i, j, color);
        }
    }
}

void Graphics::DDA(int x0, int y0, int x1, int y1, color_t color) {
    int dx = x1 - x0;
    int dy = y1 - y0;

    int step = (abs(dx) >= abs(dy)) ? abs(dx) : abs(dy);

    float xInc = dx / static_cast<float>(step);
    float yInc = dy / static_cast<float>(step);

    float x = x0;
    float y = y0;

    for (int i = 0; i < step; ++i) {
        DrawPixel(round(x), round(y), color);
        x += xInc;
        y += yInc;
    }
}








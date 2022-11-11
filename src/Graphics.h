#pragma once

#include <array>
#include <SDL2/SDL.h>
#include "Constants.hpp"

class Graphics {
public:
    explicit Graphics(SDL_Renderer* renderer);

    ~Graphics();

    void Clear(color_t color);

    void Render(SDL_Renderer* renderer);

    void DrawRect(int x, int y, int width, int height, color_t color);

    void DDA(int x0, int y0, int x1, int y1, color_t color);

    void DrawPixel(int x, int y, color_t color);

private:
    SDL_Texture* texture{};
    std::array<color_t, WINDOW_WIDTH * WINDOW_HEIGHT> colorBuffer{};
};


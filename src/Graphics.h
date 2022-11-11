#pragma once

#include <array>
#include <SDL2/SDL.h>
#include "Constants.hpp"

class Graphics {
public:
    explicit Graphics(SDL_Renderer* renderer);

    ~Graphics();

    void Clear(uint32_t color);

    void Render(SDL_Renderer* renderer);

    void DrawRect(int x, int y, int width, int height, uint32_t color);

    void DDA(int x0, int y0, int x1, int y1, uint32_t color);

    void DrawPixel(int x, int y, uint32_t color);

private:
    SDL_Texture* texture{};
    std::array<uint32_t, WINDOW_WIDTH * WINDOW_HEIGHT> data{};
};


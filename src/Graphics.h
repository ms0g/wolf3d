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

    void SetColor(int x, int y, uint32_t color);

private:
    SDL_Texture* texture{};
    std::array<uint32_t, WINDOW_WIDTH * WINDOW_HEIGHT> data{};
};


#pragma once

#include <array>
#include <SDL2/SDL.h>
#include "Constants.hpp"

class ColorBuffer {
public:
    ColorBuffer() = default;

    ~ColorBuffer() {
        SDL_DestroyTexture(texture);
    }

    void Clear(uint32_t color);

    void CreateTexture(SDL_Renderer* renderer);

    void Render(SDL_Renderer* renderer);

    void SetColor(int x, int y, uint32_t color);

private:
    SDL_Texture* texture{};
    std::array<uint32_t, WINDOW_WIDTH * WINDOW_HEIGHT> data{};
};


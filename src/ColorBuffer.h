#pragma once

#include <array>
#include <SDL2/SDL.h>
#include "Constants.hpp"


class ColorBuffer {
public:
    ColorBuffer() = default;

    explicit ColorBuffer(uint32_t color);

    ~ColorBuffer();

    void Clear(uint32_t color);

    void CreateTexture(SDL_Renderer* renderer);

    void Render(SDL_Renderer* renderer);

private:
    std::array<uint32_t, WINDOW_WIDTH * WINDOW_HEIGHT> data{};
    SDL_Texture* texture{};

};


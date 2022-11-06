#include "ColorBuffer.h"

ColorBuffer::ColorBuffer(uint32_t color) {
    // Fill the buffer
    data.fill(color);
}

ColorBuffer::~ColorBuffer() {
    SDL_DestroyTexture(texture);
}

void ColorBuffer::Render(SDL_Renderer* renderer) {
    SDL_UpdateTexture(texture,
                      nullptr,
                      data.data(),
                      WINDOW_WIDTH * sizeof(uint32_t));

    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
}

void ColorBuffer::CreateTexture(SDL_Renderer* renderer) {
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ABGR8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                WINDOW_WIDTH,
                                WINDOW_HEIGHT);
}

void ColorBuffer::SetColor(int x, int y, uint32_t color) {
    data[(WINDOW_WIDTH * y) + x] = color;
}

void ColorBuffer::Clear(uint32_t color) {
    data.fill(color);
}





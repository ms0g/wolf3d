#include "Texture.h"

Texture::Texture() {
    for (int x = 0; x < TEXTURE_WIDTH; x++) {
        for (int y = 0; y < TEXTURE_HEIGHT; y++) {
            data[TEXTURE_WIDTH * y + x] = (x % 8 && y % 8) ? 0xFFFF0000 : 0xFF000000;
        }
    }
}

uint32_t Texture::GetColor(int x, int y) {
    return data[(TEXTURE_WIDTH * y) + x];
}

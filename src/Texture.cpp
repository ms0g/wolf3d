#include "Texture.h"

Texture::Texture() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_t* upng;

        upng = upng_new_from_file(textureFileNames[i]);
        if (upng != nullptr) {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK) {
                wallTextures[i].upngTexture = upng;
                wallTextures[i].width = upng_get_width(upng);
                wallTextures[i].height = upng_get_height(upng);
                wallTextures[i].texture_buffer = (uint32_t*) upng_get_buffer(upng);
            }
        }
    }
}

uint32_t Texture::GetColor(int x, int y, int texNum) {
    return wallTextures[texNum].texture_buffer[(TEXTURE_WIDTH * y) + x];
}

Texture::~Texture() {
    for (auto& wallTexture: wallTextures) {
        upng_free(wallTexture.upngTexture);
    }
}

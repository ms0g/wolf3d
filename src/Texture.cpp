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
                wallTextures[i].texture_buffer = reinterpret_cast<color_t*>(
                        const_cast<unsigned char*>(upng_get_buffer(upng)));
            }
        }
    }
}

color_t Texture::GetColor(int x, int y, int texNum) {
    auto& tex = wallTextures[texNum];
    return tex.texture_buffer[(tex.width * y) + x];
}

Texture::texture_t& Texture::GetTexture(int texNum) {
    return wallTextures[texNum];
}

Texture::~Texture() {
    for (auto& wallTexture: wallTextures) {
        upng_free(wallTexture.upngTexture);
    }
}



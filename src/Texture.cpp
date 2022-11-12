#include "Texture.h"
#include <iostream>

Texture::Texture() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_t* upng = upng_new_from_file(textureFileNames[i]);

        if (upng != nullptr) {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK)
                textures[i] = upng;
            else
                std::cerr << "Error decoding texture " << textureFileNames[i] << "\n";
        } else {
            std::cerr << "Error loading texture " << textureFileNames[i] << "\n";
        }
    }
}

color_t Texture::GetColor(int x, int y, int texNum) {
    auto tex = textures[texNum];
    auto width = upng_get_width(tex);
    auto* buffer = reinterpret_cast<color_t*>(const_cast<unsigned char*>(upng_get_buffer(tex)));

    return buffer[(width * y) + x];
}

int Texture::GetWidth(int texNum) {
    return upng_get_width(textures[texNum]);
}

int Texture::GetHeight(int texNum) {
    return upng_get_height(textures[texNum]);
}

Texture::~Texture() {
    for (auto texture: textures) {
        upng_free(texture);
    }
}





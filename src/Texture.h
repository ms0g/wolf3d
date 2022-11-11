#pragma once

#include <array>
#include "Constants.hpp"
#include "../upng/upng.h"

class Texture {
public:
    Texture();

    ~Texture();

    color_t GetColor(int x, int y, int texNum);

    typedef struct texture_t {
        upng_t* upngTexture;
        int width;
        int height;
        color_t* texture_buffer;
    } texture_t;

    texture_t& GetTexture(int texNum);

private:
    texture_t wallTextures[NUM_TEXTURES]{};

    const char* textureFileNames[NUM_TEXTURES] = {
            "../assets/redbrick.png",
            "../assets/purplestone.png",
            "../assets/mossystone.png",
            "../assets/graystone.png",
            "../assets/colorstone.png",
            "../assets/bluestone.png",
            "../assets/wood.png",
            "../assets/eagle.png",
    };
};


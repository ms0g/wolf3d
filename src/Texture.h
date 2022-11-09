#pragma once

#include <array>
#include "Constants.hpp"
#include "../lib/upng.h"

class Texture {
public:
    Texture();
    ~Texture();

    uint32_t GetColor(int x, int y, int texNum);
private:
    typedef struct {
        upng_t* upngTexture;
        int width;
        int height;
        uint32_t* texture_buffer;
    } texture_t;

    texture_t wallTextures[NUM_TEXTURES]{};

    const char* textureFileNames[NUM_TEXTURES] = {
            "../images/redbrick.png",
            "../images/purplestone.png",
            "../images/mossystone.png",
            "../images/graystone.png",
            "../images/colorstone.png",
            "../images/bluestone.png",
            "../images/wood.png",
            "../images/eagle.png",
    };

};


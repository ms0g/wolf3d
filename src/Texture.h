#pragma once

#include <array>
#include "Constants.hpp"
#include "../upng/upng.h"

class Texture {
public:
    Texture();

    ~Texture();

    color_t GetColor(int x, int y, int texNum);

    upng_t* GetTexture(int texNum);

private:
    static constexpr int NUM_TEXTURES{8};

    std::array<upng_t*, NUM_TEXTURES> textures{};

    const char* textureFileNames[NUM_TEXTURES] = {
            "../assets/textures/redbrick.png",
            "../assets/textures/purplestone.png",
            "../assets/textures/mossystone.png",
            "../assets/textures/graystone.png",
            "../assets/textures/colorstone.png",
            "../assets/textures/bluestone.png",
            "../assets/textures/wood.png",
            "../assets/textures/eagle.png",
    };
};


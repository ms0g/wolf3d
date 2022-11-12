#pragma once

#include <array>
#include "Constants.hpp"
#include "../upng/upng.h"

class Texture {
public:
    Texture();

    ~Texture();

    color_t GetColor(int x, int y, int texNum);

    int GetWidth(int texNum);

    int GetHeight(int texNum);

private:
    static constexpr int NUM_TEXTURES{13};

    std::array<upng_t*, NUM_TEXTURES> textures{};

    const char* textureFileNames[NUM_TEXTURES] = {
            "../assets/textures/redbrick.png",      // [0]
            "../assets/textures/purplestone.png",   // [1]
            "../assets/textures/mossystone.png",    // [2]
            "../assets/textures/graystone.png",     // [3]
            "../assets/textures/colorstone.png",    // [4]
            "../assets/textures/bluestone.png",     // [5]
            "../assets/textures/wood.png",          // [6]
            "../assets/textures/eagle.png",         // [7]
            "../assets/sprites/barrel.png",         // [8]
            "../assets/sprites/light.png",          // [9]
            "../assets/sprites/table.png",          // [10]
            "../assets/sprites/guard.png",          // [11]
            "../assets/sprites/armor.png",          // [12]
    };
};


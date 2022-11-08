#pragma once

#include <array>
#include "Constants.hpp"

class Texture {
public:
    Texture();
    uint32_t GetColor(int x, int y);
private:
    std::array<uint32_t, TEXTURE_WIDTH * TEXTURE_HEIGHT> data{};

};


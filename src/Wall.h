#pragma once

#include <memory>
#include "Constants.hpp"

class Ray;
class Player;
class Graphics;
class Texture;
class Wall {
public:
    void Render(std::array<Ray, NUM_RAYS>& rays,
                const std::unique_ptr<Player>& player,
                const std::unique_ptr<Graphics>& graphics,
                const std::unique_ptr<Texture>& texture);
};


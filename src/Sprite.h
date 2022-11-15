#pragma once

#include <array>
#include <memory>
#include "Constants.hpp"

class Ray;
class Player;
class Graphics;
class Texture;
class Sprite {
public:
    Sprite() = default;

    void Render(std::array<Ray, NUM_RAYS>& rays,
                std::unique_ptr<Player>& player,
                std::unique_ptr<Graphics>& graphics,
                std::unique_ptr<Texture>& texture);

    void RenderOnMap(std::unique_ptr<Graphics>& graphics);

private:
    typedef struct sprite_t {
        int x;
        int y;
        float distance;
        float angle;
        int texture;
        bool visible;
    } sprite_t;

    static constexpr int NUM_SPRITES{5};

    std::array<sprite_t, NUM_SPRITES> sprites = {
            {{640, 630, 0, 0, 8, false},    // barrel,
             {660, 690, 0, 0, 8, false},    // barrel
             {250, 600, 0, 0, 10, false},   // table
             {300, 400, 0, 0, 11, false},   // guard
             {90, 100, 0, 0, 12, false}     // armor
            }
    };
};

#pragma once

#include <array>
#include <memory>

class Player;

class Graphics;

class Texture;

class Sprite {
public:
    Sprite() = default;

    void Render(std::unique_ptr<Graphics>& graphics,
                std::unique_ptr<Texture>& texture,
                std::unique_ptr<Player>& player);

    void RenderOnMap(std::unique_ptr<Graphics>& graphics);

private:
    typedef struct sprite_t {
        int x;
        int y;
        float distance;
        float angle;
        int index;
        bool visible;
    } sprite_t;

    static constexpr int NUM_SPRITES{4};

    std::array<sprite_t, NUM_SPRITES> sprites = {
            {{640, 630, 0, 0, 8, false}, // barrel,
             {660, 690, 0, 0, 8, false}, // barrel
             {250, 600, 0, 0, 10, false}, // table
             {300, 400, 0, 0, 11, false} // guard
            }
    };
};

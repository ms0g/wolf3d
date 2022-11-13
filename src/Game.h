#pragma once

#include <memory>
#include <array>
#include <SDL2/SDL.h>
#include "Player.h"
#include "Map.h"
#include "Ray.h"
#include "Graphics.h"
#include "Texture.h"
#include "Wall.h"
#include "Sprite.h"
#include "Constants.hpp"

class Game {
public:
    Game();

    ~Game();

    void Initialize();

    void Run();

    void Destroy();

private:
    void ProcessInput();

    void Update();

    void Render();

    void Setup();

    SDL_Window* window{};
    SDL_Renderer* renderer{};

    std::unique_ptr<Player> player;
    std::unique_ptr<Map> map;

    std::unique_ptr<Graphics> graphics;
    std::unique_ptr<Texture> texture;
    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Wall> wall;

    std::array<Ray, NUM_RAYS> rays{};

    bool isRunning;
    uint32_t ticksLastFrame{};
};



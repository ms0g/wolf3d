#pragma once

#include <numbers>
#include <memory>
#include <array>
#include <SDL2/SDL.h>
#include "Player.h"
#include "Map.h"
#include "Ray.h"
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

    void CastRays();

    void RenderRays();

    SDL_Window* window{};
    SDL_Renderer* renderer{};
    std::unique_ptr<Player> player;
    std::unique_ptr<Map> map;
    std::array<Ray, NUM_RAYS> rays{};

    bool isRunning;
    uint32_t ticksLastFrame{};
};



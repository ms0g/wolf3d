#pragma once

#include <SDL2/SDL.h>
#include <numbers>
#include <memory>
#include "Player.h"
#include "Map.h"
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

    bool isRunning;
    uint32_t ticksLastFrame{};

    static constexpr double FOV_ANGLE{(60 * (std::numbers::pi / 180))};
    static constexpr int NUM_RAYS{WINDOW_WIDTH};

    static constexpr int FPS{30};
    static constexpr int FRAME_TIME_LENGTH{1000 / FPS};
};



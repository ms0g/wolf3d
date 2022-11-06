#include "Game.h"
#include <iostream>

Game::Game() :
        isRunning(false),
        player(std::make_unique<Player>()),
        map(std::make_unique<Map>()) {
    std::cout << "Game ctor called" << std::endl;
}

Game::~Game() {
    std::cout << "Game dtor called" << std::endl;
}

void Game::Initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL" << std::endl;
        return;
    }

    window = SDL_CreateWindow(
            nullptr,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_BORDERLESS);

    if (!window) {
        std::cerr << "Error creating SDL Window";
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Error creating SDL Renderer" << std::endl;
        return;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    isRunning = true;
}

void Game::Run() {
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    isRunning = false;
                if (event.key.keysym.sym == SDLK_UP)
                    player->walkDirection = 1;
                if (event.key.keysym.sym == SDLK_DOWN)
                    player->walkDirection = -1;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    player->turnDirection = 1;
                if (event.key.keysym.sym == SDLK_LEFT)
                    player->turnDirection = -1;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    player->walkDirection = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                    player->walkDirection = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                    player->turnDirection = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                    player->turnDirection = 0;
                break;
        }
    }
}

void Game::Setup() {
    // Set player's initial values
    player->x = WINDOW_WIDTH / 2;
    player->y = WINDOW_HEIGHT / 2;
    player->width = 5;
    player->height = 5;
    player->turnDirection = 0;
    player->walkDirection = 0;
    player->rotationAngle = std::numbers::pi / 2;
    player->walkSpeed = 200;
    player->turnSpeed = 60 * (std::numbers::pi / 180);

    // Fill rays
    rays.fill({0.0, 0.0, 0.0, 0.0, false, false, false, false, false, 0});

    // Create the color buffer
    colorBuffer = std::make_unique<ColorBuffer>(0xFF000000);
    colorBuffer->CreateTexture(renderer);
}

void Game::CastRays() {
    double rayAngle = player->rotationAngle - (FOV_ANGLE / 2);
    for (int stripId = 0; stripId < NUM_RAYS; stripId++) {
        rays[stripId].Cast(rayAngle, player, map);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

void Game::Update() {
    auto timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH)
        SDL_Delay(timeToWait);

    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
    ticksLastFrame = SDL_GetTicks();

    player->Move(deltaTime, map);
    CastRays();
}

void Game::RenderRays() {
    for (int i = 0; i < NUM_RAYS; i++) {
        rays[i].Render(renderer, player);
    }
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    colorBuffer->Render(renderer);
    colorBuffer->Clear(0xFF00EE30);

    map->Render(renderer);
    RenderRays();
    player->Render(renderer);

    SDL_RenderPresent(renderer);

}

void Game::Destroy() {
    // Clean up SDL
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}





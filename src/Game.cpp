#include "Game.h"
#include <iostream>

Game::Game() :
        isRunning(false),
        player(std::make_unique<Player>()),
        map(std::make_unique<Map>()),
        wall(std::make_unique<Wall>()),
        texture(std::make_unique<Texture>()),
        sprite(std::make_unique<Sprite>()) {
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

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    window = SDL_CreateWindow(
            nullptr,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            displayMode.w,
            displayMode.h,
            SDL_WINDOW_RESIZABLE);

    if (!window) {
        std::cerr << "Error creating SDL Window";
        return;
    }

    renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
    player->walkSpeed = 250;
    player->rotationAngle = std::numbers::pi / 2;
    player->turnSpeed = 50 * (std::numbers::pi / 180);

    // Create the color buffer
    graphics = std::make_unique<Graphics>(renderer);
}

void Game::Update() {
    auto timeToWait = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);
    if (timeToWait > 0 && timeToWait <= FRAME_TIME_LENGTH)
        SDL_Delay(timeToWait);

    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
    ticksLastFrame = SDL_GetTicks();

    player->Move(deltaTime, map);
    castRays(rays, player, map);
}

void Game::Render() {
    graphics->Clear(0xFF000000);

    wall->Render(rays, player, graphics, texture);
    sprite->Render(rays, player, graphics, texture);

    // minimap rendering
    map->Render(graphics);
    renderRays(rays, player, graphics);
    sprite->RenderOnMap(graphics);
    player->Render(graphics);

    graphics->Render(renderer);
}

void Game::Destroy() {
    // Clean up SDL
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}






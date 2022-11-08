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
    player->walkSpeed = 200;
    player->rotationAngle = std::numbers::pi / 2;
    player->turnSpeed = 45 * (std::numbers::pi / 180);

    // Create the color buffer
    colorBuffer = std::make_unique<ColorBuffer>();
    colorBuffer->CreateTexture(renderer);
}

void Game::CastRays() {
    double rayAngle = player->rotationAngle - (FOV_ANGLE / 2);
    for (int i = 0; i < NUM_RAYS; i++) {
        rays[i].Cast(rayAngle, player, map);
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

void Game::Generate3DProjection() {
    for (int i = 0; i < NUM_RAYS; ++i) {
        double perpendicularDistance = rays[i].Distance() * cos(rays[i].Angle() - player->rotationAngle);
        double distanceProjectionPlane = (WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2);
        double projectedWallHeight = (TILE_SIZE / perpendicularDistance) * distanceProjectionPlane;

        int wallStripHeight = static_cast<int>(projectedWallHeight);

        int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        // color of the ceiling
        for (int j = 0; j < wallTopPixel; ++j) {
            colorBuffer->SetColor(i, j, 0xFF333333);
        }

        // color of the wall
        for (int j = wallTopPixel; j < wallBottomPixel; ++j) {
            rays[i].WasHitVertical() ? colorBuffer->SetColor(i, j, 0xFFFFFFFF) :
                colorBuffer->SetColor(i, j, 0xFFCCCCCC);
        }

        // color of the floor
        for (int j = wallBottomPixel; j < WINDOW_HEIGHT; ++j) {
            colorBuffer->SetColor(i, j, 0xFF777777);
        }
    }
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Generate3DProjection();
    colorBuffer->Render(renderer);
    colorBuffer->Clear(0xFF000000);

    map->Render(renderer);
    player->Render(renderer);
    RenderRays();

    SDL_RenderPresent(renderer);

}

void Game::Destroy() {
    // Clean up SDL
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}






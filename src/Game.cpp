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
    graphics = std::make_unique<Graphics>(renderer);

    // Create texture
    texture = std::make_unique<Texture>();
}

void Game::CastRays() {
    for (int i = 0; i < NUM_RAYS; i++) {
        double rayAngle = player->rotationAngle + atan((i - NUM_RAYS / 2) / DIST_PROJ_PLANE);
        rays[i].Cast(rayAngle, player, map);
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
    for (int i = 0; i < NUM_RAYS; i += 50) {
        rays[i].Render(graphics, player);
    }
}

void Game::Generate3DProjection() {
    for (int x = 0; x < NUM_RAYS; ++x) {
        double perpendicularDistance = rays[x].Distance() * cos(rays[x].Angle() - player->rotationAngle);
        double projectedWallHeight = (TILE_SIZE / perpendicularDistance) * DIST_PROJ_PLANE;

        int wallStripHeight = static_cast<int>(projectedWallHeight);

        int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        // color of the ceiling
        for (int y = 0; y < wallTopPixel; ++y) {
            graphics->DrawPixel(x, y, 0xFF333333);
        }

        // get the texture id from map content
        int texNum = rays[x].WallHitContent() - 1;

        int textureWidht = texture->GetTexture(texNum).width;
        int textureHeight = texture->GetTexture(texNum).height;

        // calculate texture offset X
        int textureOffsetX = rays[x].WasHitVertical() ? static_cast<int>(rays[x].WallHitY()) % textureHeight :
                             static_cast<int>(rays[x].WallHitX()) % textureWidht;

        // color of the wall
        for (int y = wallTopPixel; y < wallBottomPixel; ++y) {
            // calculate texture offset Y
            int distanceFromTop = y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
            int textureOffsetY = distanceFromTop * (static_cast<float>(textureHeight) / wallStripHeight);

            // set the color of the wall based on the color from the texture
            uint32_t texelColor = texture->GetColor(textureOffsetX, textureOffsetY, texNum);
            graphics->DrawPixel(x, y, texelColor);
        }

        // color of the floor
        for (int y = wallBottomPixel; y < WINDOW_HEIGHT; ++y) {
            graphics->DrawPixel(x, y, 0xFF777777);
        }
    }
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Generate3DProjection();

    map->Render(graphics);

    RenderRays();
    player->Render(graphics);

    graphics->Render(renderer);
    graphics->Clear(0xFF000000);

    SDL_RenderPresent(renderer);

}

void Game::Destroy() {
    // Clean up SDL
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}






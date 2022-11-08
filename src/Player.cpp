#include "Player.h"
#include <cmath>
#include "Map.h"
#include "Constants.hpp"

void Player::Render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect playerRect = {
            static_cast<int>(x * MINIMAP_SCALE_FACTOR),
            static_cast<int>(y * MINIMAP_SCALE_FACTOR),
            static_cast<int>(width * MINIMAP_SCALE_FACTOR),
            static_cast<int>(height * MINIMAP_SCALE_FACTOR)
    };

    SDL_RenderFillRect(renderer, &playerRect);

    SDL_RenderDrawLine(
            renderer,
            static_cast<int>(x * MINIMAP_SCALE_FACTOR),
            static_cast<int>(y * MINIMAP_SCALE_FACTOR),
            static_cast<int>(x * MINIMAP_SCALE_FACTOR + cos(rotationAngle) * 40),
            static_cast<int>(y * MINIMAP_SCALE_FACTOR + sin(rotationAngle) * 40)
    );

}

void Player::Move(float deltaTime, std::unique_ptr<Map>& map) {
    rotationAngle += turnDirection * turnSpeed * deltaTime;

    float moveStep = walkDirection * walkSpeed * deltaTime;

    float newPlayerX = x + cos(rotationAngle) * moveStep;
    float newPlayerY = y + sin(rotationAngle) * moveStep;

    if (!map->HasWallAt(newPlayerX, newPlayerY)) {
        x = newPlayerX;
        y = newPlayerY;
    }
}

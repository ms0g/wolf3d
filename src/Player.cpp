#include "Player.h"
#include <cmath>
#include "Map.h"
#include "Graphics.h"
#include "Utils.h"
#include "Constants.hpp"

void Player::Render(std::unique_ptr<Graphics>& graphics) const {
    int xcoord = static_cast<int>(x * MINIMAP_SCALE_FACTOR);
    int ycoord = static_cast<int>(y * MINIMAP_SCALE_FACTOR);
    int w = static_cast<int>(width * MINIMAP_SCALE_FACTOR);
    int h = static_cast<int>(height * MINIMAP_SCALE_FACTOR);

    graphics->DrawRect(xcoord, ycoord, w, h, 0xFFFFFFFF);
}

void Player::Move(float deltaTime, std::unique_ptr<Map>& map) {
    rotationAngle += turnDirection * turnSpeed * deltaTime;

    normalizeAngle(rotationAngle);

    float moveStep = walkDirection * walkSpeed * deltaTime;

    float newPlayerX = x + cos(rotationAngle) * moveStep;
    float newPlayerY = y + sin(rotationAngle) * moveStep;

    if (!map->HasWallAt(newPlayerX, newPlayerY)) {
        x = newPlayerX;
        y = newPlayerY;
    }
}

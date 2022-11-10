#include "Player.h"
#include <cmath>
#include "Map.h"
#include "Constants.hpp"

void Player::Render(std::unique_ptr<ColorBuffer>& colorBuffer) const {
    int xCoord = static_cast<int>(x * MINIMAP_SCALE_FACTOR);
    int yCoord =static_cast<int>(y * MINIMAP_SCALE_FACTOR);
    int w = static_cast<int>(width * MINIMAP_SCALE_FACTOR);
    int h = static_cast<int>(height * MINIMAP_SCALE_FACTOR);

    // Draw the rectangles
    for (int i = xCoord; i <= (xCoord + w); i++) {
        for (int j = yCoord; j <= (yCoord + h); j++) {
            colorBuffer->SetColor(i, j, 0xFFFFFFFF);
        }
    }
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

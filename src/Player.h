#pragma once

#include <memory>
#include "ColorBuffer.h"

class Map;
class Player {
public:
    Player() = default;

    void Render(std::unique_ptr<ColorBuffer>& colorBuffer) const;

    void Move(float deltaTime, std::unique_ptr<Map>& map);

    float x;
    float y;
    float width;
    float height;
    int turnDirection;     /** -1 for left, +1 for right */
    int walkDirection;     /** -1 for back, +1 for front */
    float rotationAngle;
    float walkSpeed;
    float turnSpeed;
};

#pragma once

#include <memory>
#include "Graphics.h"

class Map;
class Player {
public:
    Player() = default;

    void Render(std::unique_ptr<Graphics>& graphics) const;

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

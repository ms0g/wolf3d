#pragma once

#include <memory>
#include <SDL2/SDL.h>

class Map;
class Player {
public:
    Player() = default;

    void Render(SDL_Renderer* renderer);
    void Move(float deltaTime, std::unique_ptr<Map>& map);

    Player(float x, float y, float width, float height, int turnDirection, int walkDirection, float rotationAngle,
           float walkSpeed, float turnSpeed) :
            x(x),
            y(y),
            width(width),
            height(height),
            turnDirection(turnDirection),
            walkDirection(walkDirection),
            rotationAngle(rotationAngle),
            walkSpeed(walkSpeed),
            turnSpeed(turnSpeed) {}

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

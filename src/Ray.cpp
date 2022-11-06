#include "Ray.h"
#include <cmath>
#include "Player.h"
#include "Map.h"
#include "Utils.h"
#include "Constants.hpp"

Ray::Ray(double rayAngle, double wallHitX, double wallHitY, double distance, bool wasHitVertical, bool isRayFacingUp,
         bool isRayFacingDown, bool isRayFacingLeft, bool isRayFacingRight, int wallHitContent) :
        m_rayAngle(rayAngle),
        m_wallHitX(wallHitX),
        m_wallHitY(wallHitY),
        m_distance(distance),
        m_wasHitVertical(wasHitVertical),
        m_isRayFacingUp(isRayFacingUp),
        m_isRayFacingDown(isRayFacingDown),
        m_isRayFacingLeft(isRayFacingLeft),
        m_isRayFacingRight(isRayFacingRight),
        m_wallHitContent(wallHitContent) {}

void Ray::Cast(double rayAngle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map) {
    rayAngle = normalizeAngle(rayAngle);

    auto isRayFacingDown = rayAngle > 0 && rayAngle < std::numbers::pi;
    auto isRayFacingUp = !isRayFacingDown;

    auto isRayFacingRight = rayAngle < 0.5 * std::numbers::pi || rayAngle > 1.5 * std::numbers::pi;
    auto isRayFacingLeft = !isRayFacingRight;

    double xintercept, yintercept;
    double xstep, ystep;

    /** 
     * HORIZONTAL RAY-GRID INTERSECTION CODE 
     */
    bool foundHorizontalWallHit = false;
    double horizontalWallHitX = 0;
    double horizontalWallHitY = 0;
    int horizontalWallContent = 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
    yintercept += isRayFacingDown ? TILE_SIZE : 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = player->x + (yintercept - player->y) / tan(rayAngle);

    // Calculate the increment xstep and ystep
    ystep = TILE_SIZE;
    ystep *= isRayFacingUp ? -1 : 1;

    xstep = TILE_SIZE / tan(rayAngle);
    xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

    double nextHorizontalTouchX = xintercept;
    double nextHorizontalTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX <= WINDOW_WIDTH && nextHorizontalTouchY >= 0 &&
           nextHorizontalTouchY <= WINDOW_HEIGHT) {
        double xToCheck = nextHorizontalTouchX;
        double yToCheck = nextHorizontalTouchY + (isRayFacingUp ? -1 : 0);

        if (map->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            horizontalWallHitX = nextHorizontalTouchX;
            horizontalWallHitY = nextHorizontalTouchY;
            horizontalWallContent = map->GetContent(floor(xToCheck / TILE_SIZE), floor(yToCheck / TILE_SIZE));
            foundHorizontalWallHit = true;
            break;
        } else {
            nextHorizontalTouchX += xstep;
            nextHorizontalTouchY += ystep;
        }
    }


    /** 
     * VERTICAL RAY-GRID INTERSECTION CODE
     */
    int foundVerticalWallHit = false;
    double verticalWallHitX = 0;
    double verticalWallHitY = 0;
    int verticalWallContent = 0;

    // Find the x-coordinate of the closest vertical grid intersection
    xintercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
    xintercept += isRayFacingRight ? TILE_SIZE : 0;

    // Find the y-coordinate of the closest vertical grid intersection
    yintercept = player->y + (xintercept - player->x) * tan(rayAngle);

    // Calculate the increment xstep and ystep
    xstep = TILE_SIZE;
    xstep *= isRayFacingLeft ? -1 : 1;

    ystep = TILE_SIZE * tan(rayAngle);
    ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

    double nextVerticalTouchX = xintercept;
    double nextVerticalTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextVerticalTouchX >= 0 && nextVerticalTouchX <= WINDOW_WIDTH && nextVerticalTouchY >= 0 &&
           nextVerticalTouchY <= WINDOW_HEIGHT) {
        double xToCheck = nextVerticalTouchX + (isRayFacingLeft ? -1 : 0);
        double yToCheck = nextVerticalTouchY;

        if (map->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            verticalWallHitX = nextVerticalTouchX;
            verticalWallHitY = nextVerticalTouchY;
            verticalWallContent = map->GetContent(floor(xToCheck / TILE_SIZE), floor(yToCheck / TILE_SIZE));
            foundVerticalWallHit = true;
            break;
        } else {
            nextVerticalTouchX += xstep;
            nextVerticalTouchY += ystep;
        }
    }

    // Calculate both horizontal and vertical hit distances and choose the smallest one
    double horizontalHitDistance = foundHorizontalWallHit
                            ? distanceBetweenPoints(player->x, player->y, horizontalWallHitX, horizontalWallHitY)
                            : FLT_MAX;
    double verticalHitDistance = foundVerticalWallHit
                            ? distanceBetweenPoints(player->x, player->y, verticalWallHitX, verticalWallHitY)
                            : FLT_MAX;

    if (verticalHitDistance < horizontalHitDistance) {
        m_distance = verticalHitDistance;
        m_wallHitX = verticalWallHitX;
        m_wallHitY = verticalWallHitY;
        m_wallHitContent = verticalWallContent;
        m_wasHitVertical = true;
    } else {
        m_distance = horizontalHitDistance;
        m_wallHitX = horizontalWallHitX;
        m_wallHitY = horizontalWallHitY;
        m_wallHitContent = horizontalWallContent;
        m_wasHitVertical = false;
    }
    m_rayAngle = rayAngle;
    m_isRayFacingDown = isRayFacingDown;
    m_isRayFacingUp = isRayFacingUp;
    m_isRayFacingLeft = isRayFacingLeft;
    m_isRayFacingRight = isRayFacingRight;
}

void Ray::Render(SDL_Renderer* renderer, std::unique_ptr<Player>& player) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(
            renderer,
            MINIMAP_SCALE_FACTOR * player->x,
            MINIMAP_SCALE_FACTOR * player->y,
            MINIMAP_SCALE_FACTOR * m_wallHitX,
            MINIMAP_SCALE_FACTOR * m_wallHitY
    );
}



#include "Ray.h"
#include <cmath>
#include "Player.h"
#include "Map.h"
#include "Utils.h"
#include "Constants.hpp"

Ray::Ray(float rayAngle, float wallHitX, float wallHitY, float distance, bool wasHitVertical, bool isRayFacingUp,
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

void Ray::Cast(float rayAngle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map) {
    rayAngle = normalizeAngle(rayAngle);

    auto isRayFacingDown = rayAngle > 0 && rayAngle < std::numbers::pi;
    auto isRayFacingUp = !isRayFacingDown;

    auto isRayFacingRight = rayAngle < 0.5 * std::numbers::pi || rayAngle > 1.5 * std::numbers::pi;
    auto isRayFacingLeft = !isRayFacingRight;

    float xintercept, yintercept;
    float xstep, ystep;

    ///////////////////////////////////////////
    // HORIZONTAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    bool foundHorzWallHit = false;
    float horzWallHitX = 0;
    float horzWallHitY = 0;
    int horzWallContent = 0;

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

    float nextHorzTouchX = xintercept;
    float nextHorzTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextHorzTouchX >= 0 && nextHorzTouchX <= WINDOW_WIDTH && nextHorzTouchY >= 0 &&
           nextHorzTouchY <= WINDOW_HEIGHT) {
        float xToCheck = nextHorzTouchX;
        float yToCheck = nextHorzTouchY + (isRayFacingUp ? -1 : 0);

        if (map->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            horzWallHitX = nextHorzTouchX;
            horzWallHitY = nextHorzTouchY;
            horzWallContent = map->map[(int) floor(yToCheck / TILE_SIZE)][(int) floor(xToCheck / TILE_SIZE)];
            foundHorzWallHit = true;
            break;
        } else {
            nextHorzTouchX += xstep;
            nextHorzTouchY += ystep;
        }
    }

    ///////////////////////////////////////////
    // VERTICAL RAY-GRID INTERSECTION CODE
    ///////////////////////////////////////////
    int foundVertWallHit = false;
    float vertWallHitX = 0;
    float vertWallHitY = 0;
    int vertWallContent = 0;

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

    float nextVertTouchX = xintercept;
    float nextVertTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextVertTouchX >= 0 && nextVertTouchX <= WINDOW_WIDTH && nextVertTouchY >= 0 &&
           nextVertTouchY <= WINDOW_HEIGHT) {
        float xToCheck = nextVertTouchX + (isRayFacingLeft ? -1 : 0);
        float yToCheck = nextVertTouchY;

        if (map->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            vertWallHitX = nextVertTouchX;
            vertWallHitY = nextVertTouchY;
            vertWallContent = map->map[(int) floor(yToCheck / TILE_SIZE)][(int) floor(xToCheck / TILE_SIZE)];
            foundVertWallHit = true;
            break;
        } else {
            nextVertTouchX += xstep;
            nextVertTouchY += ystep;
        }
    }

    // Calculate both horizontal and vertical hit distances and choose the smallest one
    float horzHitDistance = foundHorzWallHit
                            ? distanceBetweenPoints(player->x, player->y, horzWallHitX, horzWallHitY)
                            : FLT_MAX;
    float vertHitDistance = foundVertWallHit
                            ? distanceBetweenPoints(player->x, player->y, vertWallHitX, vertWallHitY)
                            : FLT_MAX;

    if (vertHitDistance < horzHitDistance) {
        m_distance = vertHitDistance;
        m_wallHitX = vertWallHitX;
        m_wallHitY = vertWallHitY;
        m_wallHitContent = vertWallContent;
        m_wasHitVertical = true;
    } else {
        m_distance = horzHitDistance;
        m_wallHitX = horzWallHitX;
        m_wallHitY = horzWallHitY;
        m_wallHitContent = horzWallContent;
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



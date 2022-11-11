#include "Ray.h"
#include <cmath>
#include "Player.h"
#include "Map.h"
#include "Utils.h"
#include "Constants.hpp"

void Ray::Cast(double angle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map) {
    angle = normalizeAngle(angle);

    auto isRayFacingDown = angle > 0 && angle < std::numbers::pi;
    auto isRayFacingUp = !isRayFacingDown;

    auto isRayFacingRight = angle < 0.5 * std::numbers::pi || angle > 1.5 * std::numbers::pi;
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
    xintercept = player->x + (yintercept - player->y) / tan(angle);

    // Calculate the increment xstep and ystep
    ystep = TILE_SIZE;
    ystep *= isRayFacingUp ? -1 : 1;

    xstep = TILE_SIZE / tan(angle);
    xstep *= (isRayFacingLeft && xstep > 0) ? -1 : 1;
    xstep *= (isRayFacingRight && xstep < 0) ? -1 : 1;

    double nextHorizontalTouchX = xintercept;
    double nextHorizontalTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX <= MAP_NUM_COLS * TILE_SIZE && nextHorizontalTouchY >= 0 &&
           nextHorizontalTouchY <= MAP_NUM_ROWS * TILE_SIZE) {
        double xToCheck = nextHorizontalTouchX;
        double yToCheck = nextHorizontalTouchY + (isRayFacingUp ? -1 : 0);

        if (map->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            horizontalWallHitX = nextHorizontalTouchX;
            horizontalWallHitY = nextHorizontalTouchY;
            horizontalWallContent = map->GetMapAt(floor(xToCheck / TILE_SIZE), floor(yToCheck / TILE_SIZE));
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
    bool foundVerticalWallHit = false;
    double verticalWallHitX = 0;
    double verticalWallHitY = 0;
    int verticalWallContent = 0;

    // Find the x-coordinate of the closest vertical grid intersection
    xintercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
    xintercept += isRayFacingRight ? TILE_SIZE : 0;

    // Find the y-coordinate of the closest vertical grid intersection
    yintercept = player->y + (xintercept - player->x) * tan(angle);

    // Calculate the increment xstep and ystep
    xstep = TILE_SIZE;
    xstep *= isRayFacingLeft ? -1 : 1;

    ystep = TILE_SIZE * tan(angle);
    ystep *= (isRayFacingUp && ystep > 0) ? -1 : 1;
    ystep *= (isRayFacingDown && ystep < 0) ? -1 : 1;

    double nextVerticalTouchX = xintercept;
    double nextVerticalTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextVerticalTouchX >= 0 && nextVerticalTouchX <= MAP_NUM_COLS * TILE_SIZE && nextVerticalTouchY >= 0 &&
           nextVerticalTouchY <= MAP_NUM_ROWS * TILE_SIZE) {
        double xToCheck = nextVerticalTouchX + (isRayFacingLeft ? -1 : 0);
        double yToCheck = nextVerticalTouchY;

        if (map->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            verticalWallHitX = nextVerticalTouchX;
            verticalWallHitY = nextVerticalTouchY;
            verticalWallContent = map->GetMapAt(floor(xToCheck / TILE_SIZE), floor(yToCheck / TILE_SIZE));
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
        m_angle = angle;
    } else {
        m_distance = horizontalHitDistance;
        m_wallHitX = horizontalWallHitX;
        m_wallHitY = horizontalWallHitY;
        m_wallHitContent = horizontalWallContent;
        m_wasHitVertical = false;
        m_angle = angle;
    }
}

void Ray::Render(std::unique_ptr<Graphics>& graphics, std::unique_ptr<Player>& player) const {
    int x = player->x * MINIMAP_SCALE_FACTOR;
    int y = player->y * MINIMAP_SCALE_FACTOR;
    int width = m_wallHitX * MINIMAP_SCALE_FACTOR;
    int height = m_wallHitY * MINIMAP_SCALE_FACTOR;

    graphics->DDA(x, y, width, height, 0xFF0000FF);
}



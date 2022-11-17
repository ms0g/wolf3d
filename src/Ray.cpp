#include "Ray.h"
#include <cmath>
#include "Player.h"
#include "Map.h"
#include "Utils.h"
#include "Graphics.h"

void Ray::Cast(float angle, const std::unique_ptr<Player>& player, const std::unique_ptr<Map>& map) {
    normalizeAngle(angle);

    float xintercept, yintercept;
    float xstep, ystep;

    /** 
     * HORIZONTAL RAY-GRID INTERSECTION CODE 
     */
    bool foundHorizontalWallHit = false;
    float horizontalWallHitX = 0;
    float horizontalWallHitY = 0;
    int horizontalWallTexture = 0;

    // Find the y-coordinate of the closest horizontal grid intersection
    yintercept = floor(player->y / TILE_SIZE) * TILE_SIZE;
    yintercept += IsRayFacingDown(angle) ? TILE_SIZE : 0;

    // Find the x-coordinate of the closest horizontal grid intersection
    xintercept = player->x + (yintercept - player->y) / tan(angle);

    // Calculate the increment xstep and ystep
    ystep = TILE_SIZE;
    ystep *= IsRayFacingUp(angle) ? -1 : 1;

    xstep = TILE_SIZE / tan(angle);
    xstep *= (IsRayFacingLeft(angle) && xstep > 0) ? -1 : 1;
    xstep *= (IsRayFacingRight(angle) && xstep < 0) ? -1 : 1;

    float nextHorizontalTouchX = xintercept;
    float nextHorizontalTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextHorizontalTouchX >= 0 && nextHorizontalTouchX <= MAP_NUM_COLS * TILE_SIZE && nextHorizontalTouchY >= 0 &&
           nextHorizontalTouchY <= MAP_NUM_ROWS * TILE_SIZE) {
        float xToCheck = nextHorizontalTouchX;
        float yToCheck = nextHorizontalTouchY + (IsRayFacingUp(angle) ? -1 : 0);

        if (map->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            horizontalWallHitX = nextHorizontalTouchX;
            horizontalWallHitY = nextHorizontalTouchY;
            horizontalWallTexture = map->GetMapAt(floor(xToCheck / TILE_SIZE), floor(yToCheck / TILE_SIZE));
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
    float verticalWallHitX = 0;
    float verticalWallHitY = 0;
    int verticalWallTexture = 0;

    // Find the x-coordinate of the closest vertical grid intersection
    xintercept = floor(player->x / TILE_SIZE) * TILE_SIZE;
    xintercept += IsRayFacingRight(angle) ? TILE_SIZE : 0;

    // Find the y-coordinate of the closest vertical grid intersection
    yintercept = player->y + (xintercept - player->x) * tan(angle);

    // Calculate the increment xstep and ystep
    xstep = TILE_SIZE;
    xstep *= IsRayFacingLeft(angle) ? -1 : 1;

    ystep = TILE_SIZE * tan(angle);
    ystep *= (IsRayFacingUp(angle) && ystep > 0) ? -1 : 1;
    ystep *= (IsRayFacingDown(angle) && ystep < 0) ? -1 : 1;

    float nextVerticalTouchX = xintercept;
    float nextVerticalTouchY = yintercept;

    // Increment xstep and ystep until we find a wall
    while (nextVerticalTouchX >= 0 && nextVerticalTouchX <= MAP_NUM_COLS * TILE_SIZE && nextVerticalTouchY >= 0 &&
           nextVerticalTouchY <= MAP_NUM_ROWS * TILE_SIZE) {
        float xToCheck = nextVerticalTouchX + (IsRayFacingLeft(angle) ? -1 : 0);
        float yToCheck = nextVerticalTouchY;

        if (map->HasWallAt(xToCheck, yToCheck)) {
            // found a wall hit
            verticalWallHitX = nextVerticalTouchX;
            verticalWallHitY = nextVerticalTouchY;
            verticalWallTexture = map->GetMapAt(floor(xToCheck / TILE_SIZE), floor(yToCheck / TILE_SIZE));
            foundVerticalWallHit = true;
            break;
        } else {
            nextVerticalTouchX += xstep;
            nextVerticalTouchY += ystep;
        }
    }

    // Calculate both horizontal and vertical hit distances and choose the smallest one
    float horizontalHitDistance = foundHorizontalWallHit
                                   ? distanceBetweenPoints(player->x, player->y, horizontalWallHitX, horizontalWallHitY)
                                   : FLT_MAX;
    float verticalHitDistance = foundVerticalWallHit
                                 ? distanceBetweenPoints(player->x, player->y, verticalWallHitX, verticalWallHitY)
                                 : FLT_MAX;

    if (verticalHitDistance < horizontalHitDistance) {
        m_distance = verticalHitDistance;
        m_wallHitX = verticalWallHitX;
        m_wallHitY = verticalWallHitY;
        m_texture = verticalWallTexture;
        m_wasHitVertical = true;
        m_angle = angle;
    } else {
        m_distance = horizontalHitDistance;
        m_wallHitX = horizontalWallHitX;
        m_wallHitY = horizontalWallHitY;
        m_texture = horizontalWallTexture;
        m_wasHitVertical = false;
        m_angle = angle;
    }
}

void Ray::Render(const std::unique_ptr<Player>& player, const std::unique_ptr<Graphics>& graphics) const {
    int x = player->x * MINIMAP_SCALE_FACTOR;
    int y = player->y * MINIMAP_SCALE_FACTOR;
    int width = m_wallHitX * MINIMAP_SCALE_FACTOR;
    int height = m_wallHitY * MINIMAP_SCALE_FACTOR;

    graphics->DDA(x, y, width, height, 0xFF0000FF);
}


void renderRays(std::array<Ray, NUM_RAYS>& rays,
                const std::unique_ptr<Player>& player,
                const std::unique_ptr<Graphics>& graphics) {
    for (int i = 0; i < NUM_RAYS; i += 50) {
        rays[i].Render(player, graphics);
    }
}

void castRays(std::array<Ray, NUM_RAYS>& rays,
              const std::unique_ptr<Player>& player,
              const std::unique_ptr<Map>& map) {
    for (int i = 0; i < NUM_RAYS; i++) {
        float rayAngle = player->rotationAngle + atan((i - NUM_RAYS / 2) / DIST_PROJ_PLANE);
        rays[i].Cast(rayAngle, player, map);
    }
}

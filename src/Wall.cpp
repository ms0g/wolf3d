#include "Wall.h"
#include "Ray.h"
#include "Player.h"
#include "Graphics.h"
#include "Texture.h"
#include "Utils.h"

void Wall::Render(std::array<Ray, NUM_RAYS>& rays,
                  std::unique_ptr<Player>& player,
                  std::unique_ptr<Graphics>& graphics,
                  std::unique_ptr<Texture>& texture) {
    for (int x = 0; x < NUM_RAYS; ++x) {
        // Calculate the perpendicular distance to avoid fish-eye distortion
        float perpendicularDistance = rays[x].Distance() * cos(rays[x].Angle() - player->rotationAngle);

        // Calculate the projected wall height
        float wallHeight = (TILE_SIZE / perpendicularDistance) * DIST_PROJ_PLANE;

        int wallTopY = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
        wallTopY = wallTopY < 0 ? 0 : wallTopY;

        int wallBottomY = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
        wallBottomY = wallBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomY;

        // color of the ceiling
        for (int y = 0; y < wallTopY; ++y) {
            graphics->DrawPixel(x, y, 0xFF333333);
        }

        // get the texture id from map content
        int texNum = rays[x].WallHitContent() - 1;

        int textureWidht = texture->GetWidth(texNum);
        int textureHeight = texture->GetHeight(texNum);

        // calculate texture offset X
        auto textureOffsetX = rays[x].WasHitVertical() ? static_cast<int>(rays[x].WallHitY()) % textureHeight :
                             static_cast<int>(rays[x].WallHitX()) % textureWidht;

        // color of the wall
        for (int y = wallTopY; y < wallBottomY; ++y) {
            // calculate texture offset Y
            int distanceFromTop = y + (wallHeight / 2) - (WINDOW_HEIGHT / 2);
            int textureOffsetY = distanceFromTop * (static_cast<float>(textureHeight) / wallHeight);

            // set the color of the wall based on the color from the texture
            color_t texelColor = texture->GetColor(textureOffsetX, textureOffsetY, texNum);

            if (rays[x].WasHitVertical()) {
                changeColorIntensity(texelColor, 0.7);
            }

            graphics->DrawPixel(x, y, texelColor);
        }

        // color of the floor
        for (int y = wallBottomY; y < WINDOW_HEIGHT; ++y) {
            graphics->DrawPixel(x, y, 0xFF777777);
        }
    }
}

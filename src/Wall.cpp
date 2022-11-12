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
        double perpendicularDistance = rays[x].Distance() * cos(rays[x].Angle() - player->rotationAngle);
        double projectedWallHeight = (TILE_SIZE / perpendicularDistance) * DIST_PROJ_PLANE;

        int wallStripHeight = static_cast<int>(projectedWallHeight);

        int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;

        int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        // color of the ceiling
        for (int y = 0; y < wallTopPixel; ++y) {
            graphics->DrawPixel(x, y, 0xFF333333);
        }

        // get the texture id from map content
        int texNum = rays[x].WallHitContent() - 1;

        int textureWidht = texture->GetTexture(texNum).width;
        int textureHeight = texture->GetTexture(texNum).height;

        // calculate texture offset X
        int textureOffsetX = rays[x].WasHitVertical() ? static_cast<int>(rays[x].WallHitY()) % textureHeight :
                             static_cast<int>(rays[x].WallHitX()) % textureWidht;

        // color of the wall
        for (int y = wallTopPixel; y < wallBottomPixel; ++y) {
            // calculate texture offset Y
            int distanceFromTop = y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
            int textureOffsetY = distanceFromTop * (static_cast<float>(textureHeight) / wallStripHeight);

            // set the color of the wall based on the color from the texture
            color_t texelColor = texture->GetColor(textureOffsetX, textureOffsetY, texNum);

            if (rays[x].WasHitVertical()) {
                changeColorIntensity(texelColor, 0.7);
            }

            graphics->DrawPixel(x, y, texelColor);
        }

        // color of the floor
        for (int y = wallBottomPixel; y < WINDOW_HEIGHT; ++y) {
            graphics->DrawPixel(x, y, 0xFF777777);
        }
    }
}

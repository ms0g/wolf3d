#include "Sprite.h"
#include <cmath>
#include <algorithm>
#include "Player.h"
#include "Graphics.h"
#include "Texture.h"
#include "Utils.h"
#include "Constants.hpp"

void Sprite::Render(std::unique_ptr<Graphics>& graphics,
                    std::unique_ptr<Texture>& texture,
                    std::unique_ptr<Player>& player) {
    std::array<sprite_t, NUM_SPRITES> visibleSprites{};
    int visibleSpriteCount = 0;

    for (auto& sprite: sprites) {
        float angleSpritePlayer = player->rotationAngle - atan2(sprite.y - player->y, sprite.x - player->x);

        if (angleSpritePlayer > std::numbers::pi) {
            angleSpritePlayer -= (2 * std::numbers::pi);
        }

        if (angleSpritePlayer < -std::numbers::pi) {
            angleSpritePlayer += (2 * std::numbers::pi);
        }

        angleSpritePlayer = fabs(angleSpritePlayer);

        if (angleSpritePlayer < (FOV_ANGLE / 2) + 0.2) {
            sprite.visible = true;
            sprite.angle = angleSpritePlayer;
            sprite.distance = distanceBetweenPoints(sprite.x, sprite.y, player->x, player->y);
            visibleSprites[visibleSpriteCount] = sprite;
            visibleSpriteCount++;
        } else {
            sprite.visible = false;
        }
    }

    // Sort the visible sprites to render properly
    std::sort(visibleSprites.begin(), visibleSprites.end(), [](sprite_t s1, sprite_t s2) {
        return s1.distance > s2.distance;
    });

    for (auto& vs: visibleSprites) {
        float spriteHeight = (TILE_SIZE / vs.distance) * DIST_PROJ_PLANE;
        float spriteWidth = spriteHeight;

        float spriteTopY = (WINDOW_HEIGHT / 2) - (spriteHeight / 2);
        spriteTopY = spriteTopY < 0 ? 0 : spriteTopY;

        float spriteBottomY = (WINDOW_HEIGHT / 2) + (spriteHeight / 2);
        spriteBottomY = spriteBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : spriteBottomY;

        // Calculate the sprite X position in the projection plane
        float spriteAngle = atan2(vs.y - player->y, vs.x - player->x) - player->rotationAngle;
        float spritePosX = tan(spriteAngle) * DIST_PROJ_PLANE;

        // Sprite Left X
        float spriteLeftX = (WINDOW_WIDTH / 2) + spritePosX - (spriteWidth / 2);

        // Sprite Right X
        float spriteRightX = spriteLeftX + spriteWidth;

        int textureWidth = texture->GetWidth(vs.index);
        int textureHeight = texture->GetHeight(vs.index);

        for (int i = spriteLeftX; i < spriteRightX; i++) {
            float texelWidth = (textureWidth / spriteWidth);
            int textureOffsetX = (i - spriteLeftX) * texelWidth;

            for (int j = spriteTopY; j < spriteBottomY; j++) {
                if (i > 0 && i < WINDOW_WIDTH && j > 0 && j < WINDOW_HEIGHT) {
                    int distanceFromTop = j + (spriteHeight / 2) - (WINDOW_HEIGHT / 2);
                    int textureOffsetY = distanceFromTop * (textureHeight / spriteHeight);

                    color_t texelColor = texture->GetColor(textureOffsetX, textureOffsetY, vs.index);

                    if (texelColor != 0xFFFF00FF)
                        graphics->DrawPixel(i, j, texelColor);
                }
            }
        }
    }
}

void Sprite::RenderOnMap(std::unique_ptr<Graphics>& graphics) {
    for (int i = 0; i < NUM_SPRITES; ++i) {
        graphics->DrawRect(sprites[i].x * MINIMAP_SCALE_FACTOR,
                           sprites[i].y * MINIMAP_SCALE_FACTOR,
                           2,
                           2,
                           (sprites[i].visible) ? 0xFF00FFFF : 0xFF444444
        );
    }
}

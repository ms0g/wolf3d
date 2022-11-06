#include "Map.h"
#include "Constants.hpp"

void Map::Render(SDL_Renderer* renderer) const {
    for (int i = 0; i < MAP_NUM_ROWS; i++) {
        for (int j = 0; j < MAP_NUM_COLS; j++) {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            int tileColor = map[i][j] != 0 ? 255 : 0;

            SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
            SDL_Rect mapTileRect = {
                    static_cast<int>(tileX * MINIMAP_SCALE_FACTOR),
                    static_cast<int>(tileY * MINIMAP_SCALE_FACTOR),
                    static_cast<int>(TILE_SIZE * MINIMAP_SCALE_FACTOR),
                    static_cast<int>(TILE_SIZE * MINIMAP_SCALE_FACTOR)
            };
            SDL_RenderFillRect(renderer, &mapTileRect);
        }
    }

}

bool Map::HasWallAt(double x, double y) const {
    if (x < 0 || x > WINDOW_WIDTH || y < 0 || y > WINDOW_HEIGHT) {
        return true;
    }

    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);

    return map[mapGridIndexY][mapGridIndexX] != 0;
}

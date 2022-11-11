#include "Map.h"
#include "Graphics.h"

void Map::Render(std::unique_ptr<Graphics>& graphics) const {
    for (int i = 0; i < MAP_NUM_ROWS; i++) {
        for (int j = 0; j < MAP_NUM_COLS; j++) {
            int x = j * TILE_SIZE * MINIMAP_SCALE_FACTOR;
            int y = i * TILE_SIZE * MINIMAP_SCALE_FACTOR;
            int width = TILE_SIZE * MINIMAP_SCALE_FACTOR;
            int height = TILE_SIZE * MINIMAP_SCALE_FACTOR;

            uint32_t tileColor = map[i][j] != 0 ? 0xFFFFFFFF : 0x00000000;

            // Draw the rectangles
            graphics->DrawRect(x, y, width, height, tileColor);
        }
    }
}

bool Map::HasWallAt(double x, double y) const {
    if (x < 0 || x > MAP_NUM_COLS * TILE_SIZE || y < 0 || y > MAP_NUM_ROWS * TILE_SIZE) {
        return true;
    }

    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);

    return map[mapGridIndexY][mapGridIndexX] != 0;
}

int Map::GetMapAt(float x, float y) const {
    return map[static_cast<int>(y)][static_cast<int>(x)];
}
#pragma once

#include <numbers>

constexpr int FPS{30};
constexpr int FRAME_TIME_LENGTH{1000 / FPS};

constexpr int TILE_SIZE{64};

constexpr int MAP_NUM_ROWS{13};
constexpr int MAP_NUM_COLS{20};

constexpr double MINIMAP_SCALE_FACTOR{0.3};

constexpr int WINDOW_WIDTH{MAP_NUM_COLS * TILE_SIZE};
constexpr int WINDOW_HEIGHT{MAP_NUM_ROWS * TILE_SIZE};

constexpr int NUM_RAYS{WINDOW_WIDTH};
constexpr double FOV_ANGLE{(60 * (std::numbers::pi / 180))};


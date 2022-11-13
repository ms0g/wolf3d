#pragma once

#include <numbers>
#include <cmath>

constexpr int FPS{30};
constexpr int FRAME_TIME_LENGTH{1000 / FPS};

constexpr int TILE_SIZE{64};

constexpr int MAP_NUM_ROWS{13};
constexpr int MAP_NUM_COLS{20};

constexpr float MINIMAP_SCALE_FACTOR{0.2};

constexpr int WINDOW_WIDTH{1280};
constexpr int WINDOW_HEIGHT{800};

constexpr int NUM_RAYS{WINDOW_WIDTH};

constexpr float FOV_ANGLE{(60 * (std::numbers::pi / 180))};

static const float DIST_PROJ_PLANE{static_cast<float>((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))};

using color_t = uint32_t;

#pragma once

#include <cstdint>

void normalizeAngle(float& angle);

float distanceBetweenPoints(float x1, float y1, float x2, float y2);

void changeColorIntensity(uint32_t& color, float factor);

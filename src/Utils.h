#pragma once

#include <cstdint>

void normalizeAngle(double& angle);

double distanceBetweenPoints(double x1, double y1, double x2, double y2);

void changeColorIntensity(uint32_t& color, float factor);

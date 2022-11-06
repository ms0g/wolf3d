#include "Utils.h"
#include <numbers>
#include <cmath>

float normalizeAngle(float angle) {
    angle = remainder(angle, 2 * std::numbers::pi);
    if (angle < 0) {
        angle = 2 * std::numbers::pi + angle;
    }
    return angle;
}

float distanceBetweenPoints(float x1, float y1, float x2, float y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

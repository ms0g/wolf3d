#include "Utils.h"
#include <numbers>
#include <cmath>

void normalizeAngle(double& angle) {
    angle = remainder(angle, 2 * std::numbers::pi);
    if (angle < 0) {
        angle = 2 * std::numbers::pi + angle;
    }
}

double distanceBetweenPoints(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void changeColorIntensity(uint32_t& color, float factor) {
    uint32_t a = color & 0xFF000000;
    uint32_t r = (color & 0x00FF0000) * factor;
    uint32_t g = (color & 0x0000FF00) * factor;
    uint32_t b = (color & 0x000000FF) * factor;

    color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

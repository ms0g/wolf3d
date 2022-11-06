#include "Utils.h"
#include <numbers>
#include <cmath>

double normalizeAngle(double angle) {
    angle = remainder(angle, 2 * std::numbers::pi);
    if (angle < 0) {
        angle = 2 * std::numbers::pi + angle;
    }
    return angle;
}

double distanceBetweenPoints(double x1, double y1, double x2, double y2) {
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

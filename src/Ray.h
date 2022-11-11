#pragma once

#include <memory>
#include <numbers>

class Map;
class Player;
class Graphics;
class Ray {
public:
    Ray() = default;

    [[nodiscard]] double Angle() const {
        return m_angle;
    }

    [[nodiscard]] double WallHitX() const {
        return m_wallHitX;
    }

    [[nodiscard]] double WallHitY() const {
        return m_wallHitY;
    }

    [[nodiscard]] int WallHitContent() const {
        return m_wallHitContent;
    }

    [[nodiscard]] double Distance() const {
        return m_distance;
    }

    [[nodiscard]] bool WasHitVertical() const {
        return m_wasHitVertical;
    }

private:
    bool IsRayFacingDown(double angle) {
        return angle > 0 && angle < std::numbers::pi;
    }

    bool IsRayFacingUp(double angle) {
        return !IsRayFacingDown(angle);
    }

    bool IsRayFacingRight(double angle) {
        return angle < 0.5 * std::numbers::pi || angle > 1.5 * std::numbers::pi;

    }

    bool IsRayFacingLeft(double angle) {
        return !IsRayFacingRight(angle);
    }

public:
    void Cast(double angle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map);

    void Render(std::unique_ptr<Graphics>& graphics, std::unique_ptr<Player>& player) const;

private:
    double m_angle;
    double m_wallHitX;
    double m_wallHitY;
    double m_distance;
    bool m_wasHitVertical;
    int m_wallHitContent;

};

#pragma once

#include <memory>
#include <numbers>

class Map;
class Player;
class Graphics;
class Ray {
public:
    Ray() = default;

    [[nodiscard]] float Angle() const {
        return m_angle;
    }

    [[nodiscard]] float WallHitX() const {
        return m_wallHitX;
    }

    [[nodiscard]] float WallHitY() const {
        return m_wallHitY;
    }

    [[nodiscard]] int WallHitContent() const {
        return m_wallHitContent;
    }

    [[nodiscard]] float Distance() const {
        return m_distance;
    }

    [[nodiscard]] bool WasHitVertical() const {
        return m_wasHitVertical;
    }

private:
    bool IsRayFacingDown(float angle) {
        return angle > 0 && angle < std::numbers::pi;
    }

    bool IsRayFacingUp(float angle) {
        return !IsRayFacingDown(angle);
    }

    bool IsRayFacingRight(float angle) {
        return angle < 0.5 * std::numbers::pi || angle > 1.5 * std::numbers::pi;

    }

    bool IsRayFacingLeft(float angle) {
        return !IsRayFacingRight(angle);
    }

public:
    void Cast(float angle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map);

    void Render(std::unique_ptr<Graphics>& graphics, std::unique_ptr<Player>& player) const;

private:
    float m_angle;
    float m_wallHitX;
    float m_wallHitY;
    float m_distance;
    bool m_wasHitVertical;
    int m_wallHitContent;

};

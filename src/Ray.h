#pragma once

#include <memory>

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

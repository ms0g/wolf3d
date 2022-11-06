#pragma once

#include <memory>
#include <SDL2/SDL.h>

class Map;
class Player;
class Ray {
public:
    Ray() = default;

    Ray(double rayAngle,
        double wallHitX,
        double wallHitY,
        double distance,
        bool wasHitVertical,
        bool isRayFacingUp,
        bool isRayFacingDown,
        bool isRayFacingLeft,
        bool isRayFacingRight,
        int wallHitContent);

    void Cast(double angle, std::unique_ptr<Player>& player, std::unique_ptr<Map>& map);

    void Render(SDL_Renderer* renderer, std::unique_ptr<Player>& player) const;

    double m_angle;
    double m_wallHitX;
    double m_wallHitY;
    double m_distance;
    bool m_wasHitVertical;
    bool m_isRayFacingUp;
    bool m_isRayFacingDown;
    bool m_isRayFacingLeft;
    bool m_isRayFacingRight;
    int m_wallHitContent;

};

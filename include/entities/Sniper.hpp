#pragma once
#include "Striker.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

namespace game
{
class Sniper : public Striker
{
    sf::CircleShape _circle;
    bool isAiming = false;
    float aimCD = 0;
    
  public:
    Sniper(float x, float y);
    Sniper();
    
    void show(sf::RenderWindow &win) const override;
    std::unique_ptr<Entity> copy() const override;

    void move(float delta) override;
    void stop(float delta, sf::Vector2f def) override;
    void setPos(float x, float y) override;

    std::optional<std::unique_ptr<game::Movable>> shot(float delta) override;
};
};

#pragma once
#include "Entity.hpp"

namespace game
{
class Wall : public Entity
{
    sf::RectangleShape _outputShape;

  public:
    Wall();
    Wall(float x, float y);

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
};


}

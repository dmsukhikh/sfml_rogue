#pragma once
#include "Movable.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace game
{
class Shot : public Movable
{
    sf::RectangleShape _sprite;
  public:
    Shot(float x, float y);
    Shot();

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;
    void move(float delta) override;
    void rotate(float angle) override;

    void collideHandling(Entity &op) override;
    void collideHandling(Movable &op) override;
};
}

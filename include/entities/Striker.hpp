#pragma once
#include "AbstractEnemy.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace game
{
class Striker : public AbstractEnemy
{
    sf::RectangleShape _sprite;

  public:
    Striker(float x, float y);
    Striker();

    // Entity overridings
    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;

    // Movable overridings
    void rotate(float x, float y) override;
    void move(float delta) override;
    void stop(float delta, sf::Vector2f def) override;
    void setPos(float x, float y) override;
};
} // namespace game

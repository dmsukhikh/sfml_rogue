#pragma once
#include "AbstractEnemy.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <random>

namespace game 
{
class Bigboy : public AbstractEnemy
{
  protected:
    sf::CircleShape _sprite;
    sf::RectangleShape _hitbox;
    std::mt19937 gen;
    float damageCD = 0;

  public:
    Bigboy(float x, float y);
    Bigboy();

    // Entity overridings
    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;

    // Movable overridings
    void collideHandling(Movable &op) override;
    void rotate(float x, float y) override;
    void move(float delta) override;
    void stop(float delta, sf::Vector2f def) override;
    void setPos(float x, float y) override;

    std::optional<std::unique_ptr<game::Movable>> shot(float delta) override;
    void processMoving(sf::Vector2f gamerPos) override;
};
} // namespace game

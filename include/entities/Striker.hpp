#pragma once
#include "AbstractEnemy.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <random>

namespace game
{
class Striker : public AbstractEnemy
{
  protected:
    sf::RectangleShape _sprite, _hitbox;
    static std::random_device seed;
    std::mt19937 gen;
    std::uniform_real_distribution<float> randShotCD;
    float povx = 0.3, povy = 0.3;
    float addshotCD;

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

    std::optional<std::unique_ptr<game::Movable>> shot(float delta) override;
    void processMoving(sf::Vector2f gamerPos) override;
};
} // namespace game

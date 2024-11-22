#pragma once
#include "Movable.hpp"
#include "Bullets.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>
#include <map>

namespace game
{
class Fireball : public AbstractShot 
{
    sf::CircleShape _sprite;
    float lifetime = 0;
    bool isDamageable = false, isMoving = true;
    void _explode();
    std::map<int, float> damageCDs;

  public:
    static const float movingTime, beatingTime;
    sf::Color col = sf::Color::Red;

    // Иметь публичные конструкторы - эщкере или не эщкере, если есть фабрика?
    Fireball(float x, float y);
    Fireball();

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

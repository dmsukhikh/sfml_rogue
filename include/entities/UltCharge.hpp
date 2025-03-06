#pragma once
#include "Bullets.hpp"
#include <memory>
#include <map>

namespace game
{
class UltCharge : public AbstractShot 
{
    sf::CircleShape _sprite;
    float lifetime = 0;
    bool isDamageable = false, isMoving = true;
    std::map<int, float> damageCDs;

  public:
    const static float expandTime, beatingTime;
    sf::Color col = sf::Color::Red;

    UltCharge(float x, float y);
    UltCharge();

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;
    void move(float delta) override;

    void collideHandling(Entity &op) override;
    void collideHandling(Movable &op) override;
};

}

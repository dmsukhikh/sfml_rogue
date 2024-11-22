#pragma once
#include "Movable.hpp"
#include "../../include/entities/Gamer.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace game
{
// class Gamer;

class AbstractShot : public Movable
{
  protected:
    static Gamer *gamerLink;

  public:
    int damage = 1;
    int cd = 1;

    AbstractShot(float x, float y);
    AbstractShot();

    static void setGamer(Gamer *newLink);
    void giveDamage(Movable &op);
};

class Shot : public AbstractShot 
{
  protected:
    sf::RectangleShape _sprite;

  public:
    Shot(float x, float y);
    Shot();

    sf::Color col = sf::Color::Red;

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;
    void move(float delta) override;
    void rotate(float angle) override;

    void collideHandling(Entity &op) override;
    void collideHandling(Movable &op) override;
    void setSpeed(float maxSpeedAbs);
};
}

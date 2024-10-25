#pragma once
#include "Entity.hpp"
#include <memory>

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
    std::unique_ptr<Entity> copy() const override;
};

class Floor : public Entity
{
    // Класс-заглушка для пустых тайлов на карте. Мб позже тут появится
    // какая-нибудь 
  public:
    Floor();
    Floor(float x, float y);

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;
};
}

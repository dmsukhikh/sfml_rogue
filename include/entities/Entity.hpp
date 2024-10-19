#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace game
{

enum class EntityType
{
    Wall,
    None
};

class Entity
{
  protected:
    float _x, _y;
    EntityType type;

  public:
    const static float BLOCK_SIZE;

    Entity();
    Entity(float x, float y);

    virtual void show(sf::RenderWindow &win) const = 0;
    virtual std::vector<sf::FloatRect> getHitboxes() const = 0;
    virtual bool collide(const Entity &op) const = 0;
};
}

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace game
{

enum class EntityType
{
    Wall,
    Port,
    None
};

class Entity
{
  protected:
    float _x, _y;
    EntityType type = EntityType::None;

  public:
    const static float BLOCK_SIZE;

    Entity();
    Entity(float x, float y);

    EntityType getType() const;
    virtual void show(sf::RenderWindow &win) const = 0;
    virtual std::vector<sf::FloatRect> getHitboxes() const = 0;
    virtual bool collide(const Entity &op) const = 0;
    virtual std::unique_ptr<Entity> copy() const = 0;
    sf::Vector2f getPos() const;
};
}

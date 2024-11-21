#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>

namespace game
{

enum class EntityType
{
    // !!!!!!!!!!!!
    // Определенная сущность может иметь разный type: например, сущность
    // Port может принимать значение type = EntityType::Wall, потому что 
    // телепорт наследуется от стены, НО сущность Wall НЕ ДОЛЖНА НИКОГДА
    // принимать значение Port. В коде используется reinterpret_cast, поэтому
    // небрежная инициализация поля type может все сломать.

    Wall,
    Port,
    Shot,
    Gamer,
    Enemy,
    LevelPort,
    Item,
    DisabledItem,
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

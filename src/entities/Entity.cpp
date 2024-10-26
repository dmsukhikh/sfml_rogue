#include "../../include/entities/Entity.hpp"
#include <SFML/System/Vector2.hpp>

game::Entity::Entity() : _x(0), _y(0) {}
game::Entity::Entity(float x, float y) : _x(x), _y(y) {}
game::EntityType game::Entity::getType() const
{
    return type;
}

const float game::Entity::BLOCK_SIZE = 50.;

sf::Vector2f game::Entity::getPos() const
{
    return {_x, _y};
}

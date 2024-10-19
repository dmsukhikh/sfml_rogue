#include "../../include/entities/Entity.hpp"

game::Entity::Entity() : _x(0), _y(0) {}
game::Entity::Entity(float x, float y) : _x(x), _y(y) {}

const float game::Entity::BLOCK_SIZE = 20.;

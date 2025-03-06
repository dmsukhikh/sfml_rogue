#include "../../include/entities/Wolf.hpp"

game::Wolf::Wolf(float x, float y) : Bigboy(x, y)
{
    bounty = 150;
    _MAXSPEEDABS *= 3.f;
    _sprite.setScale({0.7, 0.7});
    _hitbox.setScale({0.7, 0.7});
    _hp = 3;
}

game::Wolf::Wolf() : game::Wolf(0, 0) {}

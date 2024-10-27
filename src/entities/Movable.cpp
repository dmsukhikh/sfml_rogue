#include "../../include/entities/Movable.hpp"
#include "../../include/vecmath.hpp"
#include <cmath>
#include <cstdint>

game::Movable::Movable(float x, float y) : Entity(x, y) {}
game::Movable::Movable() : game::Movable(0, 0) {}

void game::Movable::rotate(float angle)
{
    _angle = angle;
}

void game::Movable::move(float delta)
{
    _speed.x = _speed.x + sgn(_xmovement)*_ACCABS*delta;
    _speed.y = _speed.y + sgn(_ymovement)*_ACCABS*delta;

    if (vabs(_speed) > _MAXSPEEDABS) 
    {
        _speed *= _MAXSPEEDABS/vabs(_speed);
    }
    
    if (!sgn(_xmovement))
    {
        _speed.x -= sgn(_speed.x)*_SLOWDOWNABS*delta;
        if (std::fabs(_speed.x) < 7) _speed.x = 0; 
    }
    if (!sgn(_ymovement))
    {
        _speed.y -= sgn(_speed.y)*_SLOWDOWNABS*delta;
        if (std::fabs(_speed.y) < 7) _speed.y = 0; 
    }

    _x += _speed.x*delta;
    _y += _speed.y*delta;
}


void game::Movable::setPos(float x, float y)
{
    _x = x;
    _y = y;
}


void game::Movable::stop(float delta, sf::Vector2f def)
{
    if (_speed == sf::Vector2f{0, 0})
    {
        _speed = {sgn(def.x)*1.f, sgn(def.y)*1.f};
    }
    // delta для мощного отбрасывания игрока назад
    _speed = static_cast<float>(-4)*_speed;
    _x += _speed.x*delta;
    _y += _speed.y*delta;
}

void game::Movable::collideHandling(Entity &op) {}

void game::Movable::collideHandling(Movable &op) {}

void game::Movable::decreaseHp(uint16_t i)
{
    if (i > _hp) _hp = 0;
    else _hp -= i;
}

float game::Movable::getAngle() const
{
    return _angle;
}

void game::Movable::setGamerState(bool i)
{
    _isGamerEntity = true;
}


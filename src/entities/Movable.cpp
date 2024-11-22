#include "../../include/entities/Movable.hpp"
#include "../../include/vecmath.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdint>

int game::Movable::IDGEN = 0;
float game::Movable::SHOCKCD = 3.f;
float game::Movable::POISONCD = 1.f;
float game::Movable::POISONLIMIT = 3;


game::Movable::Movable(float x, float y) : Entity(x, y) 
{
    id = IDGEN++;
}
game::Movable::Movable() : game::Movable(0, 0) {}

void game::Movable::rotate(float x, float y)
{
    sf::Vector2f view = sf::Vector2f(x, y) - getPos(),
                 basis = {1, 0};

    _angle = vdeg(basis, view);
}

void game::Movable::rotate(float angle)
{
    _angle = angle;
}

void game::Movable::move(float delta)
{
    if (hasShocked)
    {
        _xmovement = 0;
        _ymovement = 0;
        shockClock += delta;
        if (shockClock >= SHOCKCD)
        {
            shockClock = 0;
            hasShocked = false;
        }
    }

    if (hasPoisoned)
    {
        poisonClock += delta;
        if (poisonClock >= POISONCD)
        {
            poisonClock = 0;
            poisonDamage += 1;
            _hp -= 1;
            if (_hp == 0) isExisted = false;
            if (poisonDamage == POISONLIMIT)
            {
                poisonDamage = 0;
                hasPoisoned = true;
            }
        }
    }

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
    sf::Vector2f basis = {1, 0};
    float c = vsin(basis, def);
    if (fabs(c) < sqrt(2)/2 - 0)
    {
        setPos(getPos().x - sgn(def.x), getPos().y);
        _speed.x = 0;
    }
    if (fabs(c) > sqrt(2)/2 + 0)
    {
        setPos(getPos().x, getPos().y - sgn(def.y));
        _speed.y = 0;
    }
}

void game::Movable::collideHandling(Entity &op) {}

void game::Movable::collideHandling(Movable &op) {}

void game::Movable::decreaseHp(uint16_t i)
{
    if (i > _hp)
        _hp = 0;
    else
        _hp -= i;
}

float game::Movable::getAngle() const
{
    return _angle;
}

uint16_t game::Movable::getHp() const
{
    return _hp;
}

int game::Movable::getBounty() const
{
    return bounty;
}

int game::Movable::getId() const
{
    return id;
}

sf::Vector2f game::Movable::getSpeed() const
{
    return _speed;
}

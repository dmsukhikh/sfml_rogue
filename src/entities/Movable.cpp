#include "../../include/entities/Movable.hpp"
#include "../../include/vecmath.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdint>

game::Movable::Movable(float x, float y) : Entity(x, y) {}
game::Movable::Movable() : game::Movable(0, 0) {}

void game::Movable::rotate(float x, float y)
{
    sf::Vector2f view = sf::Vector2f(x, y) - getPos(),
                 basis = {0, -1};

    _angle = vdeg(basis, view);
}

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
    sf::Vector2f basis = {1, 0};
    float c = vsin(basis, def);
    if (fabs(c) < sqrt(2)/2 - 0.0)
    {
        setPos(getPos().x - sgn(def.x), getPos().y);
        _speed.x = 0;
    }

    // Получается, все коллизии, для которых |fabs(c) - sqrt(2)/2| < 0.1 - 
    // "ложные", и они учитываться не будут. Понятное дело, что можно обмануть
    // эту систему, и если наискосок влететь в угол одиноко стоящего блока,
    // то вы заедете в спрайт его. Однако 
    //      1. За карту вы точно не выедете
    //      2. Вас все равно потом выкинет оттуда
    //      3. Вы тпшиться за стены энивей не сможете

    if (fabs(c) > sqrt(2)/2 + 0.0)
    {
        setPos(getPos().x, getPos().y - sgn(def.y));
        _speed.y = 0;
    }
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

uint16_t game::Movable::getHp() const
{
    return _hp;
}


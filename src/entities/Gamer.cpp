#include "../../include/entities/Gamer.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <memory>
#include "../../include/vecmath.hpp"


game::Gamer::Gamer(float x, float y) : Entity(x, y)
{
    _sprite.setPointCount(3);
    _sprite.setPoint(0, {0, _SIZE*fsqrt(3)/2});
    _sprite.setPoint(1, {_SIZE/2, 0});
    _sprite.setPoint(2, {_SIZE, _SIZE*fsqrt(3)/2});
    _sprite.setFillColor({180, 220, 180});
    _sprite.setOrigin({_SIZE/2, _SIZE*fsqrt(3)/3});
    _sprite.setPosition(_x, _y);
    _view = _sprite.getPosition();
}

game::Gamer::Gamer() : game::Gamer(0, 0) {}

void game::Gamer::show(sf::RenderWindow &win) const
{
    win.draw(_sprite);
}

std::vector<sf::FloatRect> game::Gamer::getHitboxes() const
{
    return {_sprite.getGlobalBounds()};
}

bool game::Gamer::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _sprite.getGlobalBounds().intersects(i);
    }
    return out;
}

void game::Gamer::rotate(float x, float y)
{
    sf::Vector2f view = sf::Vector2f(x, y) - _sprite.getPosition(),
                 basis = {0, -1};

    _prevrotation = _sprite.getRotation();
    _sprite.setRotation(vdeg(basis, view));
    // Запоминаем координаты мыши, чтобы менять взгляд не только при
    // передвижении мыши, но и при движении игрока
    _view = {x, y};
}

void game::Gamer::move(float delta)
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

    _sprite.move(_speed*delta);
    _x += _speed.x*delta;
    _y += _speed.y*delta;
    rotate(_view.x, _view.y);
}


void game::Gamer::setPos(float x, float y)
{
    _x = x;
    _y = y;
    _sprite.setPosition(x, y);
}

std::unique_ptr<game::Entity> game::Gamer::copy() const
{
    return std::make_unique<game::Gamer>(*this);
}

void game::Gamer::stopFrom(const Entity& another, float delta)
{
    // delta для мощного отбрасывания игрока назад
    _speed = -_speed;
    _sprite.move(_speed*delta);
    _speed = {0, 0};
}

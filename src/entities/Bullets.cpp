#include "../../include/entities/Bullets.hpp"
#include <memory>
#include "../../include/vecmath.hpp"
#include <cmath>

game::Shot::Shot(float x, float y) : Movable(x, y)
{
    _sprite = sf::RectangleShape({10.f, 5.f});
    _sprite.setOrigin({10.f, 2.5f});
    _sprite.setFillColor(sf::Color::Red);
    _speed = {400.f, 400.f};
    _sprite.setPosition(x, y);
    type = EntityType::Shot;
}

game::Shot::Shot() : game::Shot(0, 0) {}

void game::Shot::show(sf::RenderWindow &sf) const
{
    sf.draw(_sprite); 
}

std::vector<sf::FloatRect> game::Shot::getHitboxes() const
{
    return {_sprite.getGlobalBounds()};
}

bool game::Shot::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _sprite.getGlobalBounds().intersects(i);
    }
    return out;
}

std::unique_ptr<game::Entity> game::Shot::copy() const
{
    return std::make_unique<game::Shot>(*this);
}

void game::Shot::move(float delta) 
{
    _x += _speed.x*delta;
    _y += _speed.y*delta;
    _sprite.move(_speed*delta);
}

void game::Shot::rotate(float angle)
{
    game::Movable::rotate(angle);
    _speed = {_speed.x * std::cos(_angle / RADTODEG),
              _speed.y * std::sin(_angle / RADTODEG)};
    _sprite.rotate(_angle);
}

void game::Shot::collideHandling(Entity &op)
{
    if (isExisted && collide(op))
    {
        switch (op.getType())
        {
        case game::EntityType::Wall:
            isExisted = false;
            break;

        case game::EntityType::Port:
            isExisted = false;
            break;

        default:
            break;
        }
    }
}

void game::Shot::collideHandling(Movable &op)
{
    if (isExisted && collide(op))
    {
        switch (op.getType())
        {
        case game::EntityType::Gamer:
            if (masterType != EntityType::Gamer)
            {
                op.decreaseHp(1);
                isExisted = false;
            }
            break;

        case game::EntityType::Enemy:
            if (masterType != EntityType::Enemy)
            {
                op.decreaseHp(1);
                if (op.getHp() == 0)
                    op.isExisted = false;
                isExisted = false;
            }
            break;

        default:
            break;
        }
    }
}

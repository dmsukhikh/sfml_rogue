#include "../../include/entities/Fireball.hpp"
#include "../../include/vecmath.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <memory>

const float game::Fireball::movingTime = 5;
const float game::Fireball::beatingTime = 5;

game::Fireball::Fireball(float x, float y) : AbstractShot(x, y)
{
    _MAXSPEEDABS = 150;
    _sprite = sf::CircleShape(BLOCK_SIZE);
    _sprite.setFillColor(sf::Color::Red);
    _sprite.setOrigin({BLOCK_SIZE/2.f, BLOCK_SIZE/2.f});
    _sprite.setScale({0.2f, 0.2f});
    _sprite.setPosition(x, y);
    type = EntityType::Shot;
}

game::Fireball::Fireball() : game::Fireball(0, 0) {}

void game::Fireball::show(sf::RenderWindow &win) const
{
   win.draw(_sprite); 
}

std::vector<sf::FloatRect> game::Fireball::getHitboxes() const
{
    return {_sprite.getGlobalBounds()}; 
}

bool game::Fireball::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _sprite.getGlobalBounds().intersects(i);
    }
    return out;
}

std::unique_ptr<game::Entity> game::Fireball::copy() const
{
    return std::make_unique<game::Fireball>(*this);
}

void game::Fireball::move(float delta) 
{
    _sprite.setFillColor(col);
    if (isMoving)
    {
        _speed = _MAXSPEEDABS * sf::Vector2f{std::cos(_angle / RADTODEG),
                                             std::sin(_angle / RADTODEG)};
        _sprite.move(_speed*delta);
        _x += _speed.x * delta;
        _y += _speed.x * delta;
    }
    
    if (lifetime > movingTime)
    {
        if (isMoving)
        {
            _explode();
            isMoving = false;
        }
        else
        {
            auto newcol = _sprite.getFillColor();
            newcol.a = 255 - 255*(lifetime - movingTime)/beatingTime; 
            _sprite.setFillColor(newcol);
        }
    }
    
    if (lifetime > movingTime + beatingTime)
    {
        isExisted = false;
    }

    for (auto &[k, v]: damageCDs) v += delta;
    lifetime += delta;
}

void game::Fireball::rotate(float angle) 
{
    game::Movable::rotate(angle);
}

void game::Fireball::collideHandling(Entity &op)
{

}

void game::Fireball::collideHandling(Movable &op) 
{
    if (isExisted && collide(op))
    {
        if (damageCDs.find(op.getId()) == damageCDs.end())
        {
            damageCDs[op.getId()] = cd;
        }

        if (op.getType() != masterType && op.masterType != EntityType::Gamer &&
            op.getType() != EntityType::DashedGamer)
        {
            if (isMoving)
            {
                _explode();
                lifetime = movingTime;
            }
            else
            {
                if (damageCDs[op.getId()] >= cd)
                {
                    giveDamage(op);
                    damageCDs[op.getId()] = 0;
                }
            }
        }
    }
}

void game::Fireball::_explode()
{
    isMoving = false;
    _sprite.setScale({2.f, 2.f});
}

#include "../../include/entities/Striker.hpp"
#include "../../include/vecmath.hpp"
#include <memory>
#include <cmath>

game::Striker::Striker(float x, float y) : AbstractEnemy(x, y) 
{
    _sprite.setSize({BLOCK_SIZE, BLOCK_SIZE}); 
    _sprite.setFillColor({255, 190, 190});
    _sprite.setOrigin({BLOCK_SIZE/2.f, BLOCK_SIZE/2.f});
    _sprite.setRotation(45);
    _sprite.setPosition(x, y);
}

game::Striker::Striker() : game::Striker(0, 0) {}

void game::Striker::show(sf::RenderWindow &sf) const
{
    sf.draw(_sprite);
}

std::vector<sf::FloatRect> game::Striker::getHitboxes() const
{
    return {_sprite.getGlobalBounds()};
}

bool game::Striker::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _sprite.getGlobalBounds().intersects(i);
    }
    return out;
}

std::unique_ptr<game::Entity> game::Striker::copy() const
{
    return std::make_unique<game::Striker>(*this);
}


void game::Striker::rotate(float x, float y)
{
    game::Movable::rotate(x, y);
    _sprite.setRotation(_angle+45);
}

void game::Striker::move(float delta)
{

    if (stopState.first)
    {
        game::Movable::stop(delta, stopState.second);
        _speed *= -1.7f;
        stopState.first = false;
    }
    sf::Vector2f dir = {0, 0};
    if (!path.empty())
    {
        dir = coordsData[*path.rbegin()] - getPos();
    }

    _xmovement = fabs(dir.x) >= BLOCK_SIZE / 2.f ? sgn(dir.x) : 0;
    _ymovement = fabs(dir.y) >= BLOCK_SIZE / 2.f ? sgn(dir.y) : 0;
    game::Movable::move(delta);
    _sprite.move(_speed * delta);
}

void game::Striker::setPos(float x, float y)
{
    game::Movable::setPos(x, y);
    _sprite.setPosition(x, y);
}

void game::Striker::stop(float delta, sf::Vector2f def)
{
    game::Movable::stop(delta, def);
    _sprite.move(_speed*delta);
}

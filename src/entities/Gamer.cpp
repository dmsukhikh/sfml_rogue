#include "../../include/entities/Gamer.hpp"
#include "../../include/vecmath.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <memory>


game::Gamer::Gamer(float x, float y) : Movable(x, y)
{
    _sprite.setPointCount(3);
    _sprite.setPoint(0, {0, _SIZE*fsqrt(3)/2});
    _sprite.setPoint(1, {_SIZE/2, 0});
    _sprite.setPoint(2, {_SIZE, _SIZE*fsqrt(3)/2});
    _sprite.setFillColor({180, 220, 180});
    _sprite.setOrigin({_SIZE/2, _SIZE*fsqrt(3)/3});
    _sprite.setPosition(_x, _y);
    type = game::EntityType::Gamer;
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
    game::Movable::rotate(x, y);
    _sprite.setRotation(_angle);
}

void game::Gamer::move(float delta)
{
    game::Movable::move(delta);
    _sprite.move(_speed*delta);
}


void game::Gamer::setPos(float x, float y)
{
    game::Movable::setPos(x, y);
    _sprite.setPosition(x, y);
}

std::unique_ptr<game::Entity> game::Gamer::copy() const
{
    return std::make_unique<game::Gamer>(*this);
}

void game::Gamer::stop(float delta, sf::Vector2f def)
{
    game::Movable::stop(delta, def);
    _sprite.move(_speed*delta);
}


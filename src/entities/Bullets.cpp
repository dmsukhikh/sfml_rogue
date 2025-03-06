#include "../../include/entities/Bullets.hpp"
#include <memory>
#include "../../include/vecmath.hpp"
#include <cmath>

game::Gamer *game::AbstractShot::gamerLink = nullptr;

void game::AbstractShot::setGamer(game::Gamer *newGamer)
{
    gamerLink = newGamer;
}

game::AbstractShot::AbstractShot(float x, float y) : Movable(x, y) 
{
    type = EntityType::Shot;
}

game::AbstractShot::AbstractShot() : game::AbstractShot(0, 0) {}

game::Shot::Shot(float x, float y) : AbstractShot(x, y)
{
    _sprite = sf::RectangleShape({10.f, 5.f});
    _sprite.setOrigin({10.f, 2.5f});
    _sprite.setFillColor(sf::Color::Red);
    _speed = {_MAXSPEEDABS, _MAXSPEEDABS};
    _sprite.setPosition(x, y);
    type = EntityType::Shot;
    play(soundManager.shotBuf);
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
    _sprite.setFillColor(col);
    _x += _speed.x*delta;
    _y += _speed.y*delta;
    _sprite.move(_speed*delta);
}

void game::Shot::rotate(float angle)
{
    game::Movable::rotate(angle);
    _speed = {_speed.x * std::cos(_angle / RADTODEG),
              _speed.y * std::sin(_angle / RADTODEG)};
    _sprite.setRotation(_angle);
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
    if (isExisted && collide(op) && op.getType() != masterType &&
        op.getType() != EntityType::DashedGamer &&
        op.getType() != EntityType::Shot)
    {
        giveDamage(op);
        isExisted = false;
    }
}
 
void game::Shot::setSpeed(float maxSpeedAbs)
{
    _speed = {maxSpeedAbs, maxSpeedAbs};
    _MAXSPEEDABS = maxSpeedAbs;
    rotate(_angle);
}

void game::AbstractShot::giveDamage(game::Movable &op)
{
    if (hasShocked) op.hasShocked = true;
    if (hasPoisoned) op.hasPoisoned = true;
    op.decreaseHp(damage);
    if (masterType == EntityType::Gamer && gamerLink->getVampirism())
    {
        gamerLink->vampireCnt++;
    }
    if (op.getHp() == 0)
        op.isExisted = false;
}

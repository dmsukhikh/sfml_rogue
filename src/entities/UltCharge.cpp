#include "../../include/entities/UltCharge.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <vector>

const float game::UltCharge::expandTime = 3.f;
const float game::UltCharge::beatingTime = 4.f;

game::UltCharge::UltCharge(float x, float y) : AbstractShot(x, y)
{
    damage = 3;
    _sprite = sf::CircleShape(10);
    col = {200, 200, 255};
    _sprite.setFillColor(col);
    _sprite.setOrigin(_sprite.getLocalBounds().getSize()/2.f);
    _sprite.setPosition({x, y});
    play(soundManager.ultBuf);
}

game::UltCharge::UltCharge() : game::UltCharge(0, 0) {}

void game::UltCharge::show(sf::RenderWindow &win) const
{
    win.draw(_sprite);
}

std::vector<sf::FloatRect> game::UltCharge::getHitboxes() const
{
    return {_sprite.getGlobalBounds()};
}

bool game::UltCharge::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _sprite.getGlobalBounds().intersects(i);
    }
    return out;
}

std::unique_ptr<game::Entity> game::UltCharge::copy() const
{
    return std::make_unique<game::UltCharge>(*this);
}

void game::UltCharge::move(float delta)
{
    if (lifetime <= expandTime)
    {
        _sprite.setRadius(4*BLOCK_SIZE*(lifetime/expandTime));
        _sprite.setOrigin(_sprite.getLocalBounds().getSize()/2.f);
    }

    if (lifetime > expandTime && lifetime - expandTime <= beatingTime)
    {
        col.a = 255 * (1 - (lifetime-expandTime)/beatingTime);
        _sprite.setFillColor(col);
    }

    if (lifetime > beatingTime + expandTime)
    {
        isExisted = false;
    }

    setPos(gamerLink->getPos().x, gamerLink->getPos().y);
    _sprite.setPosition(gamerLink->getPos());

    lifetime += delta;
    for (auto &[k, v]: damageCDs) v+=delta;
}

void game::UltCharge::collideHandling(Entity &op)
{}

void game::UltCharge::collideHandling(game::Movable &op)
{
    if (isExisted && collide(op))
    {
        if (damageCDs.find(op.getId()) == damageCDs.end())
        {
            damageCDs[op.getId()] = cd;
        }

        if (op.getType() == EntityType::Shot &&
            op.masterType != EntityType::Gamer)
        {
            op.isExisted = false;
        }

        if (op.getType() == EntityType::Enemy && damageCDs[op.getId()] >= cd)
        {
            giveDamage(op);
            damageCDs[op.getId()] = 0;
        }
    }
}

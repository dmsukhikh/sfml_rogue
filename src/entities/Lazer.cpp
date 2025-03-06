#include "../../include/entities/Lazer.hpp"
#include <memory>
#include "../../include/vecmath.hpp"
#include <cmath>

const float game::Lazer::appearingTime = 1;
const float game::Lazer::beatingTime = 2.5;

const std::vector<std::unique_ptr<game::Entity>> *game::Lazer::map = nullptr;

void game::Lazer::setMap(const std::vector<std::unique_ptr<Entity>> *anmap)
{
    map = anmap;
}

game::Lazer
game::Lazer::getLazer(sf::Vector2f pos, float angle)
{
    Lazer t(pos.x, pos.y);
    float x = pos.x, y = pos.y, r = Entity::BLOCK_SIZE/2.f, ab = 0;
    bool isProcessed = true;
    
    while (isProcessed)
    {
        isProcessed = false;
        x += r*cos(angle/RADTODEG);
        y += r*sin(angle/RADTODEG);
        ab += r;

        for (auto &i: *map)
        {
            if (i->getType() == EntityType::None &&
                fabs(i->getPos().x - x) < Entity::BLOCK_SIZE/2.f &&
                fabs(i->getPos().y - y) < Entity::BLOCK_SIZE/2.f)
            {
                isProcessed = true;
            }
        }

        sf::FloatRect part({x, y}, {5, 5});
        t._hitboxes.push_back(part);
    }

    t._sprite = sf::RectangleShape({ab, 5});
    t._sprite.setFillColor(sf::Color::Red);
    t._sprite.setPosition(pos);
    t.rotate(angle);
    return t;
}

    game::Lazer::Lazer(float x, float y)
    : AbstractShot(x, y)
{
    play(soundManager.laserBuf);
    type = EntityType::Shot;
}

game::Lazer::Lazer() : game::Lazer(0, 0) {}

void game::Lazer::show(sf::RenderWindow &sf) const
{
    sf.draw(_sprite); 
}

std::vector<sf::FloatRect> game::Lazer::getHitboxes() const
{
    return _hitboxes;
}

bool game::Lazer::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        for (const auto &j: _hitboxes)
        {
            out = out || i.intersects(j);
        }
    }
    return out;
}

std::unique_ptr<game::Entity> game::Lazer::copy() const
{
    return std::make_unique<game::Lazer>(*this);
}

void game::Lazer::move(float delta) 
{
    // Работает скорее как update
    if (lifetime <= appearingTime)
    {
        _sprite.setFillColor(appearingCol);
        _sprite.setScale({1.f, 2.f*std::pow(lifetime/appearingTime, 0.5f)});
    }

    else if (lifetime >= appearingTime &&
        lifetime - appearingTime <= beatingTime)
    {
        auto col = damagingCol;
        col.a = 255 - 255 * std::pow((lifetime-appearingTime)/beatingTime, 2);
        _sprite.setFillColor(col);
    }
    
    else if (lifetime >= appearingTime + beatingTime)
    {
        isExisted = false;
    }

    for (auto &[k, v]: damageCDs) v += delta;
    lifetime += delta;
}

void game::Lazer::rotate(float angle)
{
    game::Movable::rotate(angle);
    _speed = {_speed.x * std::cos(_angle / RADTODEG),
              _speed.y * std::sin(_angle / RADTODEG)};
    _sprite.rotate(_angle);
}

void game::Lazer::collideHandling(Entity &op) {}

void game::Lazer::collideHandling(Movable &op)
{
    if (isExisted && collide(op))
    {
        if (damageCDs.find(op.getId()) == damageCDs.end())
        {
            damageCDs[op.getId()] = cd;
        }

        if (op.getType() != masterType && damageCDs[op.getId()] >= cd &&
            lifetime >= appearingTime && op.getType() != EntityType::DashedGamer)
        {
            giveDamage(op);
            damageCDs[op.getId()] = 0;
        }
    }
}


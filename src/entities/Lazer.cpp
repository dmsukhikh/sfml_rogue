#include "../../include/entities/Lazer.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
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
    : Movable(x, y)
{
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
        _sprite.setScale({1.f, 2.f*std::pow(lifetime/appearingTime, 0.5f)});
    }

    else if (lifetime >= appearingTime &&
        lifetime - appearingTime <= beatingTime)
    {
        auto col = sf::Color::White;
        col.a = 255 - 255 * std::pow((lifetime-appearingTime)/beatingTime, 2);
        _sprite.setFillColor(col);
    }
    
    else if (lifetime >= appearingTime + beatingTime)
    {
        isExisted = false;
    }

    if (cd >= 0.7)
    {
        isDamageable = true;
    }

    lifetime += delta;
    cd += delta; 
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
    if (isExisted && isDamageable && cd > 0.7 && collide(op))
    {
        switch (op.getType())
        {
        case game::EntityType::Gamer:
            if (masterType != EntityType::Gamer)
            {
                cd = 0;
                isDamageable = false;
                op.decreaseHp(1);
            }
            break;

        case game::EntityType::Enemy:
            if (masterType != EntityType::Enemy)
            {
                cd = 0;
                isDamageable = false;
                op.decreaseHp(1);
                if (op.getHp() == 0)
                    op.isExisted = false;
            }
            break;

        default:
            break;
        }
    }
}

#include "../../include/entities/Sniper.hpp"
#include "../../include/entities/Bullets.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include "../../include/vecmath.hpp"
#include <memory>
#include <optional>
#include <random>


game::Sniper::Sniper(float x, float y) : Striker(x, y)
{
    _MAXSPEEDABS = 228;
    addshotCD = randShotCD(gen);
    _circle = sf::CircleShape(Entity::BLOCK_SIZE/5.f);
    _circle.setOrigin(_circle.getLocalBounds().width / 2.f,
                      _circle.getLocalBounds().height / 2.f);
    _circle.setPosition({x, y});
    _circle.setFillColor({90, 90, 90});
    povx = 0.45;
    povy = 0.45;
}

game::Sniper::Sniper() : game::Sniper(0, 0) {}

void game::Sniper::show(sf::RenderWindow &win) const
{
    game::Striker::show(win); 
    win.draw(_circle);
}

std::unique_ptr<game::Entity> game::Sniper::copy() const
{
    return std::make_unique<game::Sniper>(*this);
}

void game::Sniper::move(float delta)
{
    if (stopState.first)
    {
        stop(delta, stopState.second);
        return ;
    }

    sf::Vector2f dir = {0, 0};
    if (!path.empty())
    {
        dir = coordsData[*path.rbegin()] - getPos();
    }

    _xmovement = fabs(dir.x) >= BLOCK_SIZE / 2.f ? sgn(dir.x) : 0;
    _ymovement = fabs(dir.y) >= BLOCK_SIZE / 2.f ? sgn(dir.y) : 0;

    if (isAiming)
    {
        _xmovement = 0;
        _ymovement = 0;
        aimCD += delta;
    }

    if (aimCD >= Lazer::appearingTime + Lazer::beatingTime / 2.f)
    {
        isAiming = false;
        aimCD = 0;
    }
    shotCD += delta; 

    game::Movable::move(delta);
    _sprite.move(_speed * delta);
    _hitbox.move(_speed * delta);
    _circle.move(_speed * delta);
}

void game::Sniper::stop(float delta, sf::Vector2f def)
{
    game::Striker::stop(delta, def);
    _circle.move(_speed*delta);
}

void game::Sniper::setPos(float x, float y)
{
    game::Striker::setPos(x, y); 
    _circle.setPosition(x, y);
}

std::optional<std::unique_ptr<game::Movable>> game::Sniper::shot(float delta)
{
    if (shotCD >= 3.5 && !isAiming)
    {
        isAiming = true;
    }

    if (shotCD >= 4 + addshotCD)
    {
        shotCD = 0;
        addshotCD = randShotCD(gen);
        auto t = Lazer::getLazer(getPos(), _angle);
        auto it = std::make_unique<Lazer>(t);
        it->masterType = EntityType::Enemy;
        return it;
    }
    return std::nullopt;
}

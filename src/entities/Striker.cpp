#include "../../include/entities/Striker.hpp"
#include "../../include/entities/Bullets.hpp"
#include "../../include/vecmath.hpp"
#include <cstdlib>
#include <memory>
#include <cmath>
#include <random>

std::random_device game::Striker::seed{};

game::Striker::Striker(float x, float y) : AbstractEnemy(x, y), gen(seed()),
    randShotCD(0, 1)
{
    bounty = 150;
    _sprite.setSize({BLOCK_SIZE, BLOCK_SIZE}); 
    _sprite.setFillColor({255, 190, 190});
    _sprite.setOrigin({BLOCK_SIZE/2.f, BLOCK_SIZE/2.f});
    _sprite.setPosition(x, y);
    _hitbox = _sprite;
    _hitbox.setScale({0.9, 0.9});
    _sprite.setRotation(45);
}

game::Striker::Striker() : game::Striker(0, 0) {}

void game::Striker::show(sf::RenderWindow &sf) const
{
    sf.draw(_sprite);
}

std::vector<sf::FloatRect> game::Striker::getHitboxes() const
{
    return {_hitbox.getGlobalBounds()};
}

bool game::Striker::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _hitbox.getGlobalBounds().intersects(i);
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
    game::Movable::move(delta);
    _sprite.move(_speed * delta);
    _hitbox.move(_speed * delta);

    shotCD += delta; 

    if (hasShocked || hasPoisoned)
    {
        auto col = _sprite.getFillColor();
        col.a = 160;
        _sprite.setFillColor(col);
    }
    else
    {
        auto col = _sprite.getFillColor();
        col.a = 255;
        _sprite.setFillColor(col);
    }
}

void game::Striker::setPos(float x, float y)
{
    game::Movable::setPos(x, y);
    _sprite.setPosition(x, y);
    _hitbox.setPosition(x, y);
}

void game::Striker::stop(float delta, sf::Vector2f def)
{
    stopState.first = false;
    game::Movable::stop(delta, def);
    _sprite.move(_speed*delta);
    _hitbox.move(_speed * delta);
}

std::optional<std::unique_ptr<game::Movable>> game::Striker::shot(float delta)
{
    if (shotCD >= 1 + addshotCD && !hasShocked)
    {
        shotCD = 0;
        addshotCD = randShotCD(gen);
        auto it = std::make_unique<game::Shot>(getPos().x, getPos().y);
        it->masterType = EntityType::Enemy;
        it->rotate(_angle);
        play(soundManager.shotBuf);
        return it;
    }
    return std::nullopt;
}

void game::Striker::processMoving(sf::Vector2f gamerPos)
{
    if (fabs(gamerPos.x - getPos().x) >= screenSize.x*povx ||  
        fabs(gamerPos.y - getPos().y) >= screenSize.y*povy)
    {
        findPathToPoint(gamerPos.x, gamerPos.y);
    }
    else
    {
        findPathToPoint(panicPoint.x, panicPoint.y);
    }
}

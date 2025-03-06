#include "../../include/entities/Bigboy.hpp"
#include <memory>
#include "../../include/vecmath.hpp"

game::Bigboy::Bigboy(float x, float y) : AbstractEnemy(x, y)
{
    bounty = 250;
    _MAXSPEEDABS *= 0.75f;
    _hp = 15;
    _sprite = sf::CircleShape(BLOCK_SIZE*0.66f, 5);
    _sprite.setOrigin({BLOCK_SIZE/2.f, BLOCK_SIZE/2.f});
    _sprite.setFillColor({255, 190, 190});
    _hitbox = sf::RectangleShape({BLOCK_SIZE, BLOCK_SIZE});
    _hitbox.setOrigin({BLOCK_SIZE/2.f, BLOCK_SIZE/2.f});
    _sprite.setPosition(x, y);
    _hitbox.setPosition(x, y);
}

game::Bigboy::Bigboy() : game::Bigboy(0, 0) {}

void game::Bigboy::show(sf::RenderWindow &win) const
{
    win.draw(_sprite); 
}

std::vector<sf::FloatRect> game::Bigboy::getHitboxes() const
{
    return {_hitbox.getGlobalBounds()};
}

bool game::Bigboy::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _hitbox.getGlobalBounds().intersects(i);
    }
    return out;
}


std::unique_ptr<game::Entity> game::Bigboy::copy() const
{
    return std::make_unique<game::Bigboy>(*this);
}

void game::Bigboy::rotate(float x, float y)
{
    game::Movable::rotate(x, y);
    _sprite.setRotation(_angle);
}

void game::Bigboy::move(float delta)
{
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

    damageCD += delta; 
}

void game::Bigboy::setPos(float x, float y)
{
    game::Movable::setPos(x, y);
    _sprite.setPosition(x, y);
    _hitbox.setPosition(x, y);
}

void game::Bigboy::stop(float delta, sf::Vector2f def)
{
    stopState.first = false;
    game::Movable::stop(delta, def);
    _sprite.move(_speed*delta);
    _hitbox.move(_speed * delta);
}

std::optional<std::unique_ptr<game::Movable>> game::Bigboy::shot(float delta) 
{
    return std::nullopt;
}

void game::Bigboy::processMoving(sf::Vector2f gamerPos)
{
    findPathToPoint(gamerPos.x, gamerPos.y);
}

void game::Bigboy::collideHandling(game::Movable &op)
{
    if (collide(op))
    {
        switch (op.getType())
        {
            case game::EntityType::Gamer:
                if (damageCD > 1 && !hasShocked)
                {
                    damageCD = 0;
                    op.decreaseHp(1);
                }
                break;

            case game::EntityType::Shot:
                op.collideHandling(*this);
                break;

            default:
                break;
        }
    }
}

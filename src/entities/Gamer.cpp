#include "../../include/entities/Gamer.hpp"
#include "../../include/vecmath.hpp"
#include <SFML/System/Vector2.hpp>
#include "../../include/entities/Bullets.hpp"
#include "../../include/entities/Lazer.hpp"
#include "../../include/entities/Item.hpp"
#include <cmath>
#include <memory>
#include <cstdlib>
#include <iostream>


std::random_device game::Gamer::r{};

game::Gamer::Gamer(float x, float y) : Movable(x, y), gen(r())
{
    _sprite.setPointCount(3);
    _sprite.setPoint(0, {0, _SIZE*fsqrt(3)/2});
    _sprite.setPoint(1, {_SIZE/2, 0});
    _sprite.setPoint(2, {_SIZE, _SIZE*fsqrt(3)/2});
    _sprite.setFillColor({180, 220, 180});
    _sprite.setOrigin({_SIZE/2, _SIZE*fsqrt(3)/3});
    _sprite.setPosition(_x, _y);
    _hitbox = _sprite;
    maxHP = 10;
    _hp = maxHP;
    type = game::EntityType::Gamer;

    AbstractShot::setGamer(this);

    for (int i = 0; i < Item::ITEMSCOUNT; ++i)
    {
        items[i] = 0;
    }
}

game::Gamer::Gamer() : game::Gamer(0, 0) {}

void game::Gamer::show(sf::RenderWindow &win) const
{
    win.draw(_sprite);
}

std::vector<sf::FloatRect> game::Gamer::getHitboxes() const
{
    return {_hitbox.getGlobalBounds()};
}

bool game::Gamer::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _hitbox.getGlobalBounds().intersects(i);
    }
    return out;
}

void game::Gamer::rotate(float x, float y)
{
    game::Movable::rotate(x, y);
    _sprite.setRotation(_angle-30);
}

void game::Gamer::move(float delta)
{   
    if (isDashed)
    {
        type = EntityType::DashedGamer;
        inDashClock += delta;

        auto col = _sprite.getFillColor();
        col.a = 160;
        _sprite.setFillColor(col);

        if (inDashClock >= 0.3f)
        {
            inDashClock = 0;
            _MAXSPEEDABS /= 2.f;
            type = EntityType::Gamer;
            dashCDClock = 0;
            isDashed = false;

            auto col = _sprite.getFillColor();
            col.a = 255;
            _sprite.setFillColor(col);
        }
    }

    game::Movable::move(delta);
    _sprite.move(_speed*delta);
    _hitbox.move(_speed*delta);

    // В моем коде есть соглашение, согласно которому всякий update происходит 
    // в функции move
    shotCDclock += delta;
    if (getDashCharge() < 100) dashCDClock += delta;
    if (withVampirism && vampireCnt >= vampireLimit && _hp < maxHP)
    {
        vampireCnt = 0;
        _hp++;
    }


    for (auto &[k, v]: items)
    {
        if (v > 0)
        {
            switch (k)
            {
            case Item::ADD_HP:
                maxHP++;
                _hp++;
                break;

            case Item::SPEED_UP:
                _MAXSPEEDABS = _MAXSPEEDABS * (1 + 0.1 * items[Item::SPEED_UP]);
                _SLOWDOWNABS = _SLOWDOWNABS * (1 + 0.1 * items[Item::SPEED_UP]);
                _ACCABS = _ACCABS * (1 + 0.1 * items[Item::SPEED_UP]);
                break;

            case Item::SHOOTSPEED_UP:
                shotCD = shotCD * (std::pow(0.9, items[Item::SHOOTSPEED_UP]));
                break;

            case Item::HEALING:
                if (maxHP - _hp > 0)
                {
                    _hp++;
                }
                break;

            case Item::DAMAGE_UP:
                plainDamage++;
                break;

            case Item::ULT_IMPROVE:
                // будет в будущем
                break;

            case Item::DASH_IMPROVE:
                dashCD *= 0.75;
                break;

            case Item::POISON:
                poisonProb += 0.1;
                break;

            case Item::SHOCK:
                shockProb += 0.1;
                break;

            case Item::EXPLODE:
                explodeProb += 0.1;
                break;

            case Item::LAZER:
                if (withLazer)
                {
                    lazerCD = lazerCD * (std::pow(0.9, items[Item::LAZER]));
                }
                else
                {
                    lazerCD = 5 * shotCD;
                    withLazer = true;
                }
                break;

            case Item::NEW_LIVE:
                lives++;
                break;

            case Item::VAMPIRE:
                if (!withVampirism) withVampirism = true;
                else
                {
                    vampireLimit -= 5;
                }
                break;

            default:
                break;
            }
            --v;
        }
    }
}


void game::Gamer::setPos(float x, float y)
{
    game::Movable::setPos(x, y);
    _sprite.setPosition(x, y);
    _hitbox.setPosition(x, y);
}

std::unique_ptr<game::Entity> game::Gamer::copy() const
{
    return std::make_unique<game::Gamer>(*this);
}

void game::Gamer::stop(float delta, sf::Vector2f def)
{
    sf::Vector2f basis = {1, 0};
    float c = vsin(basis, def);
    if (fabs(c) < sqrt(2)/2 - 0.1)
    {
        setPos(getPos().x - sgn(def.x), getPos().y);
        _speed.x = 0;
    }
    if (fabs(c) > sqrt(2)/2 + 0.1)
    {
        setPos(getPos().x, getPos().y - sgn(def.y));
        _speed.y = 0;
    }
    _sprite.move(_speed*delta);
    _hitbox.move(_speed*delta);
}

std::optional<std::unique_ptr<game::Movable>> game::Gamer::shot(float delta)
{
    if (!withLazer)
    {
        if (shotCDclock > shotCD)
        {
            shotCDclock = 0;
            auto ret = std::make_unique<game::Shot>(getPos().x, getPos().y);
            ret->col = sf::Color::Green;
            ret->rotate(_angle);
            ret->setSpeed(1500);
            ret->masterType = EntityType::Gamer;
            ret->damage = plainDamage;

            float prob = randProb(gen);
            if (prob < shockProb) ret->hasShocked = true;
            if (prob < explodeProb) ret->hasExplosive = true;
            if (prob < poisonProb) ret->hasPoisoned = true;

            return ret;
        }
    }
    else
    {
        if (shotCDclock > lazerCD)
        {
            shotCDclock = 0;
            auto ret = std::make_unique<game::Lazer>(
                game::Lazer::getLazer(getPos(), _angle));
            ret->appearingCol = sf::Color::Green;
            ret->damagingCol = {220, 255, 220};
            ret->masterType = EntityType::Gamer;
            ret->damage = plainDamage;

            float prob = randProb(gen);
            if (prob < shockProb) ret->hasShocked = true;
            if (prob < explodeProb) ret->hasExplosive = true;
            if (prob < poisonProb) ret->hasPoisoned = true;

            return ret;
        }
 
    }
    return std::nullopt;
}

int game::Gamer::getMaxHp() const { return maxHP; }

void game::Gamer::addItem(Item::ItemType itype)
{
    items[static_cast<int>(itype)]++;
}

bool game::Gamer::getVampirism() const
{
    return withVampirism;
}

void game::Gamer::dash()
{
    if (dashCDClock >= dashCD)
    {
        dashCDClock = 0;
        if (vabs(_speed) < 10e-4)
        {
            _speed = {-std::cos(_angle), std::sin(_angle)};
            _speed *= 2*_MAXSPEEDABS;
        }
        else
        {
            _speed *= 2 * _MAXSPEEDABS / vabs(_speed);
        }
        _MAXSPEEDABS *= 2;
        isDashed = true;
    }
}

int game::Gamer::getDashCharge() const
{
    return 100 * (dashCDClock / dashCD);
}

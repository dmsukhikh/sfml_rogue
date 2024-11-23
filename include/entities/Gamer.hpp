#pragma once
#include "Movable.hpp"
#include "Item.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <optional>
#include <map>
#include <random>

namespace game
{
class Gamer : public Movable
{
    sf::ConvexShape _sprite, _hitbox;
    std::map<int, int> items;

    float shotCD = 0.5, lazerCD = 2, shotCDclock = 0, dashCD = 2,
          dashCDClock = 2, inDashClock = 0, ultCD = 20, ultCDClock = 0;
    int maxHP, plainDamage = 1, vampireLimit = 100;
    float poisonProb = 0, explodeProb = 0, shockProb = 0;
    bool withLazer = false, withVampirism = false, isDashed = false;

    static std::random_device r;
    std::mt19937_64 gen;
    std::uniform_real_distribution<float> randProb;

  public:
    bool isShooting = false, isUlting = false;
    int score = 0, vampireCnt = 0, lives = 0;

    Gamer(float x, float y, bool setGamerLink = false);
    Gamer();

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    
    void rotate(float x, float y) override;
    void move(float delta) override;
    void stop(float delta, sf::Vector2f def) override;
    void setPos(float x, float y) override;
    
    int getMaxHp() const;
    void addItem(Item::ItemType itype);
    bool getVampirism() const;

    std::unique_ptr<Entity> copy() const override;
    std::optional<std::unique_ptr<game::Movable>> shot(float delta);
    std::optional<std::unique_ptr<game::Movable>> ult(float delta);
    void dash();
    void ult();

    int getDashCharge() const;
    int getUltCharge() const;
    bool getUlting() const;
};
}

#include "Movable.hpp"
#include "Item.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <optional>
#include <map>

namespace game
{
class Gamer : public Movable
{
    sf::ConvexShape _sprite, _hitbox;
    std::map<int, int> items;
    float shotCD = 0.4, lazerCD = 0.5;
    float shotCDclock = 0;
    int maxHP, lives = 0, plainDamage = 1;
    float poisonProb = 0, explodeProb = 0, shockProb = 0;
    bool withLazer = false;

  public:
    bool isShooting = false;
    int score = 0;

    Gamer(float x, float y);
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

    std::unique_ptr<Entity> copy() const override;
    std::optional<std::unique_ptr<game::Movable>> shot(float delta);
};
}

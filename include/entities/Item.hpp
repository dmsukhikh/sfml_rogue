#pragma once
#include "Entity.hpp"

namespace game
{
class Item : public Entity
{
    sf::Sprite _sprite, _dissprite;
    sf::Texture _t, _dist;

  public:
    enum ItemType
    {
        SPEED_UP = 0,
        SHOOTSPEED_UP,
        ADD_HP,
        DAMAGE_UP,
        DASH_IMPROVE,
        ULT_IMPROVE,
        SHOCK,
        VAMPIRE,
        LAZER,
        EXPLODE,
        POISON,
        NEW_LIVE,
        HEALING
    };

    static const size_t ITEMSCOUNT;
    const ItemType itemType;

    Item(float x, float y, ItemType type = SPEED_UP);
    Item();

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;
    void activate();
};
} // namespace game

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
        SPEED_UP = 0,    // done
        SHOOTSPEED_UP,   // done
        ADD_HP,          // done
        DAMAGE_UP,       // done
        DASH_IMPROVE,    // 
        ULT_IMPROVE,     //
        SHOCK,           // done
        VAMPIRE,         // done
        LAZER,           // done
        EXPLODE,         // done
        POISON,          // 
        NEW_LIVE,        // done
        HEALING          // done
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

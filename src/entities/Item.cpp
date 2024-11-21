#include "../../include/entities/Item.hpp"
#include <memory>

const size_t game::Item::ITEMSCOUNT = 13;

game::Item::Item(float x, float y, ItemType t) : Entity(x, y), itemType(t)
{
    type = EntityType::DisabledItem;

    switch (itemType)
    {
        case Item::ADD_HP:
            _t.loadFromFile("resources/itemsprites/heart.png");
            _dist.loadFromFile("resources/itemsprites/heart_disabled.png");
            break;
        case Item::SPEED_UP:
            _t.loadFromFile("resources/itemsprites/speedboot.png");
            _dist.loadFromFile("resources/itemsprites/speedboot_disabled.png");
            break;
        case Item::SHOOTSPEED_UP:
            _t.loadFromFile("resources/itemsprites/coke.png");
            _dist.loadFromFile("resources/itemsprites/coke_disabled.png");
            break;

        case Item::DAMAGE_UP:
            _t.loadFromFile("resources/itemsprites/blade.png");
            _dist.loadFromFile("resources/itemsprites/blade_disabled.png");
            break;

        case Item::DASH_IMPROVE:
            _t.loadFromFile("resources/itemsprites/book.png");
            _dist.loadFromFile("resources/itemsprites/book_disabled.png");
            break;

        case Item::ULT_IMPROVE:
            _t.loadFromFile("resources/itemsprites/mask.png");
            _dist.loadFromFile("resources/itemsprites/mask_disabled.png");
            break;

        case Item::SHOCK:
            _t.loadFromFile("resources/itemsprites/jumbo.png");
            _dist.loadFromFile("resources/itemsprites/jumbo_disabled.png");
            break;

        case Item::POISON:
            _t.loadFromFile("resources/itemsprites/poison.png");
            _dist.loadFromFile("resources/itemsprites/poison_disabled.png");
            break;

        case Item::EXPLODE:
            _t.loadFromFile("resources/itemsprites/magma.png");
            _dist.loadFromFile("resources/itemsprites/magma_disabled.png");
            break;

        case Item::HEALING:
            _t.loadFromFile("resources/itemsprites/blue_heart.png");
            _dist.loadFromFile("resources/itemsprites/blue_heart_disabled.png");
            break;

        case Item::VAMPIRE:
            _t.loadFromFile("resources/itemsprites/sickle.png");
            _dist.loadFromFile("resources/itemsprites/sickle_disabled.png");
            break;

        case Item::LAZER:
            _t.loadFromFile("resources/itemsprites/diamond.png");
            _dist.loadFromFile("resources/itemsprites/diamond_disabled.png");
            break;

        case Item::NEW_LIVE:
            _t.loadFromFile("resources/itemsprites/plusone.png");
            _dist.loadFromFile("resources/itemsprites/plusone.png");
            break;
    }

    _sprite.setTexture(_t);
    _dissprite.setTexture(_dist);
    _sprite.setScale({_sprite.getTextureRect().width / BLOCK_SIZE,
                      _sprite.getTextureRect().height / BLOCK_SIZE});
    _sprite.setPosition(x*BLOCK_SIZE, y*BLOCK_SIZE);
    _dissprite.setScale({_sprite.getTextureRect().width / BLOCK_SIZE,
                      _sprite.getTextureRect().height / BLOCK_SIZE});
    _dissprite.setPosition(x*BLOCK_SIZE, y*BLOCK_SIZE);
}

void game::Item::show(sf::RenderWindow &win) const
{
    if (type == EntityType::DisabledItem)
    {
        win.draw(_dissprite);
    }
    else
    {
        win.draw(_sprite);
    }
}

std::vector<sf::FloatRect> game::Item::getHitboxes() const
{
    return {_sprite.getGlobalBounds()};
}

bool game::Item::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _sprite.getGlobalBounds().intersects(i);
    }
    return out;

}

std::unique_ptr<game::Entity> game::Item::copy() const
{
    return std::make_unique<Item>(*this);
}

void game::Item::activate()
{
    type = EntityType::Item;
}

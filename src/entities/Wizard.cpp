#include "../../include/entities/Wizard.hpp"
#include "../../include/entities/Fireball.hpp"

game::Wizard::Wizard(float x, float y) : Striker(x, y)
{
    bounty = 300;
    addshotCD = randShotCD(gen);
    _hat = sf::RectangleShape({Entity::BLOCK_SIZE/3.f, Entity::BLOCK_SIZE/3.f});
    _hat.setOrigin({_hat.getLocalBounds().width / 2.f,
                    _hat.getLocalBounds().height / 2.f});
    _hat.setPosition({x, y});
    _hat.setFillColor({90, 90, 250});
    _hat.setRotation(45);
    povx = 0.45;
    povy = 0.45;
}

game::Wizard::Wizard() : game::Wizard(0, 0) {}

void game::Wizard::show(sf::RenderWindow &win) const
{
    game::Striker::show(win); 
    win.draw(_hat);
}

std::unique_ptr<game::Entity> game::Wizard::copy() const
{
    return std::make_unique<game::Wizard>(*this);
}

void game::Wizard::move(float delta)
{
    game::Striker::move(delta);
    _hat.move(_speed * delta);
}

void game::Wizard::stop(float delta, sf::Vector2f def)
{
    game::Striker::stop(delta, def);
    _hat.move(_speed*delta);
}

void game::Wizard::rotate(float x, float y)
{
    game::Striker::rotate(x, y);
    _hat.setRotation(_angle+45);
}

void game::Wizard::setPos(float x, float y)
{
    game::Striker::setPos(x, y); 
    _hat.setPosition(x, y);
}

std::optional<std::unique_ptr<game::Movable>> game::Wizard::shot(float delta)
{
    if (shotCD >= 2 + addshotCD && !hasShocked)
    {
        shotCD = 0;
        addshotCD = randShotCD(gen);
        auto t = Fireball(getPos().x, getPos().y);
        t.rotate(_angle);
        auto it = std::make_unique<Fireball>(t);
        it->masterType = EntityType::Enemy;
        return it;
    }
    return std::nullopt;
}

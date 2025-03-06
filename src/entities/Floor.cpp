#include "../../include/entities/Tiles.hpp"

game::Floor::Floor(float x, float y) : Entity(x, y) 
{
    type = EntityType::None;
}
game::Floor::Floor() : game::Floor(0, 0) {}

void game::Floor::show(sf::RenderWindow &sf) const {}
std::vector<sf::FloatRect> game::Floor::getHitboxes() const
{
    return {};
}
bool game::Floor::collide(const Entity &op) const
{
    return false;
}

std::unique_ptr<game::Entity> game::Floor::copy() const
{
    return std::make_unique<Floor>(*this);
}

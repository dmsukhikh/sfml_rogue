#include "../../include/entities/Tiles.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

game::Wall::Wall(float x, float y) : Entity(x, y) 
{
    type = game::EntityType::Wall;
    _outputShape.setSize({game::Entity::BLOCK_SIZE, game::Entity::BLOCK_SIZE});
    _outputShape.setFillColor(sf::Color::White);
    _outputShape.setPosition(_x, _y);
}

game::Wall::Wall() : game::Wall(0, 0) {}

void game::Wall::show(sf::RenderWindow &op) const
{
    op.draw(_outputShape);
}

std::vector<sf::FloatRect> game::Wall::getHitboxes() const
{
    return {_outputShape.getGlobalBounds()};
}

bool game::Wall::collide(const Entity &op) const
{
    bool out = false;
    for (const auto &i : op.getHitboxes())
    {
        out = out || _outputShape.getGlobalBounds().intersects(i); 
    }
    return out;
}

std::unique_ptr<game::Entity> game::Wall::copy() const
{
    return std::make_unique<Wall>(*this);
}

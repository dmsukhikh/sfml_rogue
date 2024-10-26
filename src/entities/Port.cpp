#include "../../include/entities/Tiles.hpp"
#include <cstdint>
#include <memory>

const sf::Color game::Port::COL = {200, 0, 200};
const sf::Color game::Port::DISABLED_COL = {100, 0, 100};

game::Port::Port(float x, float y) : Wall(x, y)
{
    changeActiveness(false);
}

void game::Port::setIdx(uint64_t idx) 
{ 
    _idx = idx; 
    isLinked = true;
} 

bool game::Port::getLinked() const
{
    return isLinked;
}

std::unique_ptr<game::Entity> game::Port::copy() const
{
    return std::make_unique<Port>(*this);
}

uint64_t game::Port::getIdx() const
{
    return _idx;
}

void game::Port::changeActiveness(bool isActive)
{
    if (isActive)
    {
        _outputShape.setFillColor(COL);
        type = game::EntityType::Port;
    }
    else
    {
        _outputShape.setFillColor(DISABLED_COL);
        type = game::EntityType::Wall;
    }
}

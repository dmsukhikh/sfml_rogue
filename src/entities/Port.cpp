#include "../../include/entities/Tiles.hpp"
#include <iostream>
#include <cstdint>
#include <memory>

bool game::Port::fontIsLoaded = false;
sf::Font game::Port::f{};

game::Port::Port(float x, float y) : Wall(x, y)
{
    if (!fontIsLoaded)
    {
        std::cout << "done" << std::endl;
        f.loadFromFile("resources/ThaleahFat.ttf");
        fontIsLoaded = true;
    }

    idxToTeleport.setFont(f);
    idxToTeleport.setFillColor(sf::Color::White);
    idxToTeleport.setCharacterSize(BLOCK_SIZE);
    idxToTeleport.setPosition(getPos() - _outputShape.getOrigin());

    col = {200, 0, 200};
    disabledCol = {100, 0, 100};
    activeType = EntityType::Port;
    changeActiveness(false);
}

void game::Port::setIdx(uint64_t idx) 
{ 
    _idx = idx; 
    idxToTeleport.setString(std::to_string(idx));
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

void game::Port::show(sf::RenderWindow &win) const
{
    win.draw(_outputShape);
    win.draw(idxToTeleport);
}

void game::Port::changeActiveness(bool isActive)
{
    if (isActive)
    {
        _outputShape.setFillColor(col);
        type = activeType;
    }
    else
    {
        _outputShape.setFillColor(disabledCol);
        type = game::EntityType::Wall;
    }
}

// ----

game::LevelPort::LevelPort(float x, float y) : Port(x, y) 
{
    col = {200, 200, 0};
    disabledCol = {100, 100, 0};
    activeType = EntityType::LevelPort;
    changeActiveness(false);
}

game::LevelPort::LevelPort() : game::LevelPort(0, 0) {}

std::unique_ptr<game::Entity> game::LevelPort::copy() const
{
    return std::make_unique<game::LevelPort>(*this);
}

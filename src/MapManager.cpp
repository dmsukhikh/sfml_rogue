#include "../include/MapManager.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <random>
#include <stdexcept>
#include <iostream>

game::Map::Map(const Map &op)
{
    _data.clear();
    for (auto &&i: op._data)
    {
        _data.push_back(i->copy());
    }
}

game::MapManager::MapManager() : _gen(_rd())
{
    std::filesystem::path p("resources/maps"); 
    for (const auto &map: std::filesystem::directory_iterator(p))
    {
        std::fstream mapFile(map.path(), std::ios_base::in);
        Map tempmap;
        float x = 0, y = 0;
        while (!mapFile.eof())
        {
            for (char c = mapFile.get(); c != '\n' && !mapFile.eof(); c = mapFile.get())
            {
                switch (c)
                {
                    case 'X':
                        tempmap._data.push_back(std::make_unique<game::Wall>(x, y));
                        break;
                    case '.':
                        tempmap._data.push_back(std::make_unique<game::Floor>(x, y));
                        break;
                    default:
                        tempmap._data.push_back(std::make_unique<game::Floor>(x, y));
                        break;
                }
                x += game::Entity::BLOCK_SIZE;
            }
            x = 0;
            y += game::Entity::BLOCK_SIZE;
        }
        pool.push_back(std::move(tempmap));
    }
    _randmap = std::uniform_int_distribution<int>(0, pool.size()-1);
}

void game::MapManager::generateNewLevel()
{
    level.clear();
    for (int i = 0; i < LEVELNUM; ++i)
    {
        level.push_back(pool[_randmap(_gen)]); 
    }
}

const game::Map &game::MapManager::getLevel(uint32_t idx)
{
    if (idx >= level.size())
    {
        throw std::range_error("Bad idx is out of range");
    }
    return level[idx];
}

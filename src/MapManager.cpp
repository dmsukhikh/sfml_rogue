#include "../include/MapManager.hpp"
#include <cstddef>
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
    _unlinkedPorts = op._unlinkedPorts;
    _linkedPorts = op._linkedPorts;
    width = op.width;
    height = op.height;
} 

void game::Map::removeUnlinkedPorts()
{
    for (auto &i : _unlinkedPorts)
    {
        _data[i] = std::make_unique<game::Floor>(_data[i]->getPos().x,
                                                 _data[i]->getPos().y);
    }
    _unlinkedPorts.clear();
}

void game::Map::linkPort(uint64_t idx)
{
    if (_unlinkedPorts.empty())
    {
        throw std::range_error("No ports for linking");
    }
    else
    {
        // Мы знаем, что по индексу port доступен объект game::Port, поэтому
        // можно произвести reinterpret_cast. Господи, прости меня за это. Но
        // я знаю, что я делаю
        auto i = _unlinkedPorts.begin();
        reinterpret_cast<Port &>(*_data[*i]).setIdx(idx);
        _linkedPorts.insert(*i);
        _unlinkedPorts.erase(i);
    }
}

game::MapManager::MapManager() : _gen(_rd())
{
    std::filesystem::path p("resources/maps"); 
    for (const auto &map: std::filesystem::directory_iterator(p))
    {
        std::fstream mapFile(map.path(), std::ios_base::in);
        Map tempmap;
        float x = 0, y = 0, maxx = 0;
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
                    case 'P':
                        tempmap._data.push_back(std::make_unique<game::Port>(x, y));
                        tempmap._unlinkedPorts.insert(tempmap._data.size() - 1);
                        break;
                    default:
                        tempmap._data.push_back(std::make_unique<game::Floor>(x, y));
                        break;
                }
                x += game::Entity::BLOCK_SIZE;
            }
            maxx = std::max(maxx, x);
            x = 0;
            y += game::Entity::BLOCK_SIZE;
        }
        tempmap.width = maxx;
        tempmap.height = y;
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
    
    // Генерация, собственно, карты, путем соединения через телепорты разных
    // комнат. Есть часть связная и несвязная: после линковки с некоторой
    // комнатой из связной части, новая комната попадает туда. Потом, все 
    // неслинкованные телепорты заменяются на game::Floor объекты
    for (size_t i = 1; i < LEVELNUM; ++i)
    {
        if (level[i]._unlinkedPorts.empty()) continue;

        auto rand = std::uniform_int_distribution<size_t>(0, i-1); 
        size_t tolink = rand(_gen);
        for (;level[tolink]._unlinkedPorts.empty(); tolink = rand(_gen));
        level[tolink].linkPort(i);
        level[i].linkPort(tolink);
        std::cout << tolink << " " << i << std::endl;
    }
    for (auto &i: level)
    {
        i.removeUnlinkedPorts();
    }
}

const game::Map &game::MapManager::getRoom(uint32_t idx)
{
    if (idx >= level.size())
    {
        throw std::range_error("Bad idx is out of range");
    }
    return level[idx];
}

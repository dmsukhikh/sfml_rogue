#include "../include/MapManager.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <random>
#include <stdexcept>
#include <iostream>
#include <queue>

game::Map::Map(const Map &op)
{
    _data.clear();
    for (auto &&i: op._data)
    {
        _data.push_back(i->copy());
    }
    isLvlPort = op.isLvlPort;
    _lvlPortIdx = op._lvlPortIdx;
    _unlinkedPorts = op._unlinkedPorts;
    _linkedPorts = op._linkedPorts;
    _adjList = op._adjList;
    _mapCoords = op._mapCoords;
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
    if (!isLvlPort)
    {
        _data[_lvlPortIdx] = std::make_unique<game::Floor>(
            _data[_lvlPortIdx]->getPos().x, _data[_lvlPortIdx]->getPos().y);
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
        int x = 0, y = 0, maxx = 0, idx = 0, prevrow = 0, praccum = 0;

        while (!mapFile.eof())
        {
            for (char c = mapFile.get(); c != '\n' && !mapFile.eof(); c = mapFile.get())
            {
                tempmap._adjList.push_back({});
                switch (c)
                {
                    case 'X':
                        tempmap._data.push_back(std::make_unique<game::Wall>(
                            (x+0.5) * Entity::BLOCK_SIZE, (y+0.5) * Entity::BLOCK_SIZE));
                        break;
                    case '.':
                        tempmap._data.push_back(std::make_unique<game::Floor>(
                            (x+0.5) * Entity::BLOCK_SIZE, (y+0.5) * Entity::BLOCK_SIZE));
                        break;
                    case 'P':
                        tempmap._data.push_back(std::make_unique<game::Port>(
                            (x+0.5) * Entity::BLOCK_SIZE, (y+0.5) * Entity::BLOCK_SIZE));
                        tempmap._unlinkedPorts.insert(tempmap._data.size() - 1);
                        break;

                    case 'L':
                        tempmap._data.push_back(std::make_unique<game::LevelPort>(
                            (x+0.5) * Entity::BLOCK_SIZE, (y+0.5) * Entity::BLOCK_SIZE));
                        tempmap._lvlPortIdx = tempmap._data.size() - 1;
                        break;

                    default:
                        tempmap._data.push_back(std::make_unique<game::Floor>(
                            (x+0.5) * Entity::BLOCK_SIZE, (y+0.5) * Entity::BLOCK_SIZE));
                        break;
                }

                tempmap._mapCoords.push_back({Entity::BLOCK_SIZE * (x + 0.5f),
                                              Entity::BLOCK_SIZE * (y + 0.5f)});

                if (x > 0 &&
                    tempmap._data[idx]->getType() == EntityType::None &&
                    tempmap._data[idx - 1]->getType() == EntityType::None)
                {
                    tempmap._adjList[idx].push_back(idx - 1);
                    tempmap._adjList[idx - 1].push_back(idx);
                }

                if (y > 0 && idx-praccum < prevrow &&
                    tempmap._data[idx]->getType() == EntityType::None &&
                    tempmap._data[idx-prevrow]->getType() == EntityType::None)
                {
                    tempmap._adjList[idx].push_back(idx - prevrow);
                    tempmap._adjList[idx - prevrow].push_back(idx);
                }
                
                x += 1;
                idx += 1;
            }
            maxx = std::max(maxx, x);
            prevrow = x;
            praccum += x;
            x = 0;
            y += 1;
        }

        tempmap.width = maxx * Entity::BLOCK_SIZE;
        tempmap.height = y * Entity::BLOCK_SIZE;
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
    std::vector<std::vector<int>> graph(LEVELNUM);
    for (size_t i = 1; i < LEVELNUM; ++i)
    {
        if (level[i]._unlinkedPorts.empty()) continue;

        // Случайный выбор комнаты из связной части
        auto rand = std::uniform_int_distribution<size_t>(0, i-1); 
        size_t tolink = rand(_gen);

        // ! В связной части не должно быть так, чтобы все порталы были
        // заполнены. ! Поэтому важно, чтобы в каждой новой карте было минимум
        // два телепорта ! Также важно, чтобы на каждой карте был один, и только
        // один Level-телепорт

        for (; level[tolink]._unlinkedPorts.empty(); tolink = rand(_gen));
        level[tolink].linkPort(i);
        level[i].linkPort(tolink);

        graph[i].push_back(tolink);
        graph[tolink].push_back(i);
    }
    curGraph = graph;

    // Нахождение самого дальнего уровня от 0-го с помощью bfs. Там будет 
    // телепорт на новый левел
    std::vector<std::pair<int, int>> pathData(level.size(), {-1, true});
    {
        std::queue<size_t> q;
        q.push(0);   
        pathData[0].second = false;
        int cnt = 0;
        pathData[0].first = cnt++;

        while (!q.empty())
        {
            size_t i = q.front();
            q.pop();
            for (auto j : graph[i])
            {
                if (pathData[j].second)
                {
                    pathData[j].first = cnt;
                    pathData[j].second = false;
                    q.push(j);
                }
            }
            cnt++;
            pathData[i].second = false;
        }
    }

    size_t lvlportRoom =
        std::max_element(pathData.begin(), pathData.end()) - pathData.begin();
    level[lvlportRoom].isLvlPort = true;
    endPoint = lvlportRoom;

    for (auto &i: level)
    {
        i.removeUnlinkedPorts();
    }
}

game::Map &game::MapManager::getRoom(uint32_t idx)
{
    if (idx >= level.size())
    {
        throw std::range_error("Bad idx is out of range");
    }
    return level[idx];
}

int game::MapManager::getEndPoint() const { return endPoint; }

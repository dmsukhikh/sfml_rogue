#pragma once
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <vector>
#include <memory>
#include <random>
#include <set>
#include "entities/Entities.hpp"

namespace game {

struct Map
{
    Map() = default;
    Map(const Map &op);

    void removeUnlinkedPorts();
    void linkPort(uint64_t idx);
    bool isCleared = false;

    uint64_t width, height;
    std::set<std::size_t> _unlinkedPorts, _linkedPorts;
    std::vector<std::vector<int>> _adjList;
    std::vector<sf::Vector2f> _mapCoords;
    std::vector<std::unique_ptr<Entity>> _data;
};

class MapManager
{
    std::random_device _rd;
    std::mt19937 _gen;
    std::uniform_int_distribution<int> _randmap;

    const uint32_t LEVELNUM = 10;

    std::vector<Map> level;
    std::vector<Map> pool; 

  public:
    MapManager();
    void generateNewLevel();
    const Map& getRoom(uint32_t idx);
};

}

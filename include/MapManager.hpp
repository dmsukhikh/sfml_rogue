#pragma once
#include "entities/Entities.hpp"
#include <cstdint>
#include <vector>
#include <memory>
#include <random>

namespace game {
struct Map
{
    Map() = default;
    Map(const Map &op);
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
    const Map& getLevel(uint32_t idx);
};

}

#pragma once
#include <cstdint>
#include <utility>
#include <fstream>
#include <SFML/Graphics/Color.hpp>

namespace game
{
struct Settings 
{
    Settings();
    ~Settings();

    std::pair<uint32_t, uint32_t> screenSize;
    sf::Color bgcol;

  private:
    void _writeDefaultToFile(std::fstream &f);
};
};

#pragma once
#include <cstdint>
#include <utility>
#include <fstream>
#include "SFML/Graphics/Color.hpp"

namespace game
{
struct Settings 
{
    Settings();
    ~Settings();

    unsigned videoModeIdx;
    sf::Color bgcol;
    unsigned volume;

  private:
    void _writeDefaultToFile(std::fstream &f);
};
};

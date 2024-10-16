#pragma once
#include <cstdint>
#include <utility>
#include <fstream>

namespace game
{
struct Settings 
{
    Settings();
    ~Settings();

    std::pair<uint32_t, uint32_t> screenSize;

  private:
    void _writeDefaultToFile(std::fstream &f);
};
};

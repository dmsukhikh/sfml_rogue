#include "../include/Settings.hpp"
#include <iostream> 

game::Settings::Settings()
{
    std::fstream file("settings.txt", std::ios_base::in);
    if (file.fail())
    {
        file.open("settings.txt", std::ios_base::in | std::ios_base::out |
                                      std::ios_base::trunc);
        _writeDefaultToFile(file);
    }
    
    // Пропуск предупредительного сообщения
    for (char i = 0; i != '\n'; i=file.get());

    file >> screenSize.first >> screenSize.second;
    {
        int32_t r, g, b;
        file >> r >> g >> b;
        bgcol.r = r;
        bgcol.g = g;
        bgcol.b = b;
    }
}

game::Settings::~Settings()
{
    std::fstream f("settings.txt",
                      std::ios_base::out | std::ios_base::trunc);

    f << "THIS IS SYSTEM FILE: DON'T CHANGE ANYTHING WITHING THIS FILE!" << std::endl;
    f << screenSize.first << " " << screenSize.second << std::endl;
    f << static_cast<int32_t>(bgcol.r) << " " 
      << static_cast<int32_t>(bgcol.g) << " " 
      << static_cast<int32_t>(bgcol.b) << std::endl;
}

void game::Settings::_writeDefaultToFile(std::fstream &f)
{
    f << "THIS IS SYSTEM FILE: DON'T CHANGE ANYTHING WITHING THIS FILE!" << std::endl;
    f << 1200 << " " << 720 << std::endl; // Screen size; 
    f << 50 << " " << 50 << " " << 100 << std::endl;    // Background color
    f.seekg(0);
}

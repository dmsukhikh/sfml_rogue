#include "../include/Settings.hpp"
#include <iostream> 

game::Settings::Settings()
{
    std::fstream file("resources/settings.txt", std::ios_base::in);
    if (file.fail())
    {
        file.open("resources/settings.txt", std::ios_base::in | std::ios_base::out |
                                      std::ios_base::trunc);
        _writeDefaultToFile(file);
    }
    
    // Пропуск предупредительного сообщения
    for (char i = 0; i != '\n'; i=file.get());

    file >> videoModeIdx;
    {
        int32_t r, g, b;
        file >> r >> g >> b;
        bgcol.r = r;
        bgcol.g = g;
        bgcol.b = b;
    }
    file >> volume;
}

game::Settings::~Settings()
{
    std::fstream f("resources/settings.txt",
                      std::ios_base::out | std::ios_base::trunc);

    f << "THIS IS SYSTEM FILE: DON'T CHANGE ANYTHING WITHIN THIS FILE!" << std::endl;
    f << videoModeIdx << std::endl;
    f << static_cast<int32_t>(bgcol.r) << " " 
      << static_cast<int32_t>(bgcol.g) << " " 
      << static_cast<int32_t>(bgcol.b) << std::endl;
    f << volume;
}

void game::Settings::_writeDefaultToFile(std::fstream &f)
{
    f << "THIS IS SYSTEM FILE: DON'T CHANGE ANYTHING WITHIN THIS FILE!" << std::endl;
    f << 0 << std::endl; // Screen size; 
    f << 50 << " " << 50 << " " << 100 << std::endl;    // Background color
    f << 100;
    f.seekg(0);
}

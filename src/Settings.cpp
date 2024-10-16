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
}

game::Settings::~Settings()
{
    std::fstream f("settings.txt",
                      std::ios_base::out | std::ios_base::trunc);

    f << "THIS IS SYSTEM FILE: DON'T CHANGE ANYTHING WITHING THIS FILE!" << std::endl;
    f << screenSize.first << " " << screenSize.second << std::endl; 
}

void game::Settings::_writeDefaultToFile(std::fstream &f)
{
    f << "THIS IS SYSTEM FILE: DON'T CHANGE ANYTHING WITHING THIS FILE!" << std::endl;
    f << 1200 << " " << 720 << std::endl; // Screen size; 
    f.seekg(0);
}

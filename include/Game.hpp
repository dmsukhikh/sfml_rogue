#pragma once
#include "gamegui/GameGUI.hpp"
#include "Settings.hpp"
#include <vector>

namespace game
{
class Game
{
    Settings _settings;
    sf::RenderWindow _curWin;

    std::vector<GridPacker> _menu;

    void _setMenusWindows();
    void _setGreetingWindow();


public:
    Game();
};

};

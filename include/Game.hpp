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
    std::size_t _showingWindowIdx = 0;
    bool _gameIsRunning = false;

    std::string _res, _curvolume, _cursens;

    void _setMenusWindows();
    void _setSettingsWindow();
    void _setGreetingWindow();
    void _gameloop();

  public:
    Game();
    void mainloop();
};

}; // namespace game

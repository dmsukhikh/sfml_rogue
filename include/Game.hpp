#pragma once
#include "gamegui/GameGUI.hpp"
#include "Settings.hpp"
#include "entities/Entities.hpp"
#include "MapManager.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <list>

namespace game
{
class Game
{
    Settings _settings;
    sf::RenderWindow _curWin;
    std::vector<GridPacker> _guiScreens;
    std::size_t _showingWindowIdx = 0;
    std::string _res, _curvolume, _cursens;

    bool _gameIsRunning = false,
         _keyMoveLeftIsPressed = false,
         _keyMoveRightIsPressed = false,
         _keyMoveUpIsPressed = false,
         _keyMoveDownIsPressed = false;

    Gamer gamer;
    MapManager mapManager;
    std::list<std::unique_ptr<Movable>> onMapEntities;

    uint32_t level = 0, room = 0;
    sf::Clock _frameClock;
    sf::View cam;
    sf::Vector2i _view = {0, 0};

    void _setMenusWindows();
    void _setSettingsWindow();
    void _setGreetingWindow();
    void _setGameGui();

    void _gameloop();
    void _inputHandling();
    void _ingameHandling(float delta);
    void _showObjects();
    void _moveCamera();
    void _activatePorts();

  public:
    Game();
    void mainloop();
};

}; // namespace game

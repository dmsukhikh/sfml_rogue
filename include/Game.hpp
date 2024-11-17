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
    sf::Font _hudfont;

    bool _gameIsRunning = false,
         _keyMoveLeftIsPressed = false,
         _keyMoveRightIsPressed = false,
         _keyMoveUpIsPressed = false,
         _keyMoveDownIsPressed = false,
         _isMinimapShowed = false;

    Gamer gamer;
    MapManager mapManager;
    std::list<std::unique_ptr<Movable>> onMapEntities;

    uint32_t level = 1, room = 0, enemies = 1;
    sf::Clock _frameClock;
    sf::View cam, actCam;
    sf::Vector2i _view = {0, 0};
    float gamerScoreClock = 0;

    void _setMenusWindows();
    void _setSettingsWindow();
    void _setGreetingWindow();
    void _setGameGui();
    void _showGameHUD();
    void _showMinimap();

    void _gameloop();
    void _inputHandling();
    void _ingameHandling(float delta);
    void _showObjects();
    void _moveCamera();
    void _activatePorts();
    void _generateEnemies();
    void _initializeRoom(size_t room);

  public:
    Game();
    void mainloop();
};

}; // namespace game

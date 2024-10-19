#include "../include/Game.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>

using Anchor = game::GridPacker::Anchor;

game::Game::Game()
{
    _curWin.create(
        sf::VideoMode(_settings.screenSize.first, _settings.screenSize.second),
        "SFML: Rogue");
    _curWin.setFramerateLimit(75);
    _curWin.setKeyRepeatEnabled(false);
    gamer.setPos(200, 200);
    _setMenusWindows();
}

void game::Game::_setGreetingWindow()
{
    GridPacker greet(_settings.screenSize.first, _settings.screenSize.second),
               btnsTray(200, 200);

    game::Button newGame(200, 300), settings(200, 150), exit(200, 150),
                 centerVoid(10, 10);

    newGame.setText("New game");
    settings.setText("Settings");
    exit.setText("Exit");
    exit.setFunc([this](){ _curWin.close(); });
    settings.setFunc([this]() { _showingWindowIdx = 1; });
    newGame.setFunc(
        [this]()
        {
            _gameIsRunning = true;
            _showingWindowIdx = 2;
            _frameClock.restart();
        });

    btnsTray.putObject(newGame, 0, 0);
    btnsTray.putObject(settings, 0, 1);
    btnsTray.putObject(exit, 0, 2);

    centerVoid.setBgColor(_settings.bgcol);
    centerVoid.setToggleColor(_settings.bgcol);
    centerVoid.setOutlineColor(_settings.bgcol);


    greet.putObject(centerVoid, 0, 0);
    greet.putObject(centerVoid, 1, 0);
    greet.putObject(centerVoid, 2, 0);
    greet.putObject(centerVoid, 4, 0);
    greet.putObject(btnsTray, 3, 0);
    _guiScreens.push_back(greet);
}

void game::Game::_setSettingsWindow()
{
    game::Slider volume(10, 50, 0, 100), sens(10, 50, 0, 100);
    game::Button resolutionLeft(30, 30), resolutionRight(30, 30),
                 apply(10, 10), exit(10, 10), stub(10, 10);

    resolutionLeft.setText("<");
    resolutionRight.setText(">");
    apply.setText("Apply");
    exit.setText("Exit");
    stub.setToggleColor(_settings.bgcol);
    stub.setBgColor(_settings.bgcol);
    stub.setOutlineColor(_settings.bgcol);

    _res = "1200x720", _curvolume = "50", _cursens = "50";
    game::Label outputRes(10, 50, _res), curVolume(10, 50, _curvolume),
        curSens(10, 50, _cursens);

    volume.setFunc(
        [&volume, this](double scale)
        {
            _curvolume =
                std::to_string(static_cast<int>(volume.getBorders().y * scale));
        });

    sens.setFunc(
        [&sens, this](double scale)
        {
            _cursens =
                std::to_string(static_cast<int>(sens.getBorders().y * scale));
        });

    exit.setFunc([this]() { _showingWindowIdx = 0; });

    game::GridPacker main(_settings.screenSize.first, _settings.screenSize.second),
                     volstring(_settings.screenSize.first, 10),
                     sensstr(_settings.screenSize.first, 10),
                     resstr(_settings.screenSize.first, 10),
                     manageBtns(_settings.screenSize.first, 10);

    volstring.putObject(volume, 0, 0, Anchor::EXPAND, Anchor::FIXED);
    volstring.putObject(curVolume, 0, 1, Anchor::EXPAND, Anchor::FIXED);
    sensstr.putObject(sens, 0, 0, Anchor::EXPAND, Anchor::FIXED);
    sensstr.putObject(curSens, 0, 1, Anchor::EXPAND, Anchor::FIXED);
    resstr.putObject(resolutionLeft, 0, 0);
    resstr.putObject(resolutionRight, 0, 2);
    resstr.putObject(outputRes, 0, 1);
    manageBtns.putObject(stub, 0, 0);
    manageBtns.putObject(stub, 0, 2);
    manageBtns.putObject(stub, 0, 3);
    manageBtns.putObject(stub, 0, 5);
    manageBtns.putObject(apply, 0, 1);
    manageBtns.putObject(exit, 0, 4);

    main.putObject(stub, 0, 0);
    main.putObject(stub, 5, 0);
    main.putObject(volstring, 1, 0);
    main.putObject(sensstr, 2, 0);
    main.putObject(resstr, 3, 0);
    main.putObject(manageBtns, 4, 0);

    _guiScreens.push_back(main);
}

void game::Game::_setMenusWindows()
{
    _setGreetingWindow();
    _setSettingsWindow();
    _setGameGui();
}

void game::Game::mainloop()
{
    sf::Event ev;
    while (_curWin.isOpen())
    {
        _curWin.clear(_settings.bgcol);

        if (_gameIsRunning)
        {
            _gameloop();
        }
        else
        {
            _guiScreens[_showingWindowIdx].displayWidgets(_curWin);
            while (_curWin.pollEvent(ev))
            {
                _guiScreens[_showingWindowIdx]._invoke(_curWin, ev);
            }
            _curWin.display();
        }
    }
}

void game::Game::_setGameGui()
{
    game::GridPacker gui(_settings.screenSize.first,
                         _settings.screenSize.second);
    _guiScreens.push_back(gui);
}

void game::Game::_gameloop()
{
    auto delta = _frameClock.restart().asSeconds();

    _showObjects();
    _inputHandling();
    _ingameHandling(delta);

}


void game::Game::_inputHandling()
{
    sf::Event ev;
    while (_curWin.pollEvent(ev))
    {
        if (ev.type == sf::Event::KeyPressed)
        {
            if (ev.key.code == sf::Keyboard::Escape)
            {
                _curWin.close();
            }
            else if (ev.key.code == sf::Keyboard::A)
            {
                _keyMoveLeftIsPressed = true;
                gamer._xmovement = -10;
            }
            else if (ev.key.code == sf::Keyboard::D)
            {
                _keyMoveRightIsPressed = true;
                gamer._xmovement = 10;
            }
            else if (ev.key.code == sf::Keyboard::W)
            {
                _keyMoveUpIsPressed = true;
                gamer._ymovement = -10;
            }
            else if (ev.key.code == sf::Keyboard::S)
            {
                _keyMoveDownIsPressed = true;
                gamer._ymovement = 10;
            }
        }
        
        else if (ev.type == sf::Event::KeyReleased)
        {
            if (ev.key.code == sf::Keyboard::A)
            {
                _keyMoveLeftIsPressed = false;
                gamer._xmovement = _keyMoveRightIsPressed ? 1 : 0;
            }
            else if (ev.key.code == sf::Keyboard::D)
            {
                _keyMoveRightIsPressed = false;
                gamer._xmovement = _keyMoveLeftIsPressed ? -1 : 0;
            }
            else if (ev.key.code == sf::Keyboard::W)
            {
                _keyMoveUpIsPressed = false;
                gamer._ymovement = _keyMoveDownIsPressed ? 1 : 0;
            }
            else if (ev.key.code == sf::Keyboard::S)
            {
                _keyMoveDownIsPressed = false;
                gamer._ymovement = _keyMoveUpIsPressed ? -1 : 0;
            }
        }

        if (ev.type == sf::Event::MouseMoved)
        {
            auto t = _curWin.mapPixelToCoords({ev.mouseMove.x, ev.mouseMove.y});
            gamer.rotate(t.x, t.y);
        }
    }
}

void game::Game::_showObjects()
{
    _curWin.clear(sf::Color::Black);
    gamer.show(_curWin);
    _guiScreens[_showingWindowIdx].show(_curWin);
    _curWin.display();
}

void game::Game::_ingameHandling(float delta)
{
    gamer.move(delta);
}

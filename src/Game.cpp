#include "../include/Game.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "../include/vecmath.hpp"
#include <iostream>
#include <algorithm>
#include <memory>
#include <random>

using Anchor = game::GridPacker::Anchor;

game::Game::Game()
{
    _curWin.create(
        sf::VideoMode(_settings.screenSize.first, _settings.screenSize.second),
        "SFML: Rogue");
    _curWin.setFramerateLimit(75);
    _curWin.setKeyRepeatEnabled(false);
    cam.setSize({_settings.screenSize.first*1.f, _settings.screenSize.second*1.f});
    cam.setCenter({_settings.screenSize.first*0.5f, _settings.screenSize.second*0.5f});
    _curWin.setView(cam);
    gamer.setPos(200, 200);
    _setMenusWindows();
    mapManager.generateNewLevel();
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
        [volume, this](double scale)
        {
            _curvolume =
                std::to_string(static_cast<int>(volume.getBorders().y * scale));
        });

    sens.setFunc(
        [sens, this](double scale)
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

    _initializeRoom(0);

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

    _inputHandling();
    _ingameHandling(delta);
    _showObjects();
    std::cout << 1 / delta << std::endl;
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
            else if (ev.key.code == sf::Keyboard::H)
            {
                _activatePorts();
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
            else if (ev.key.code == sf::Keyboard::Space)
            {
                onMapEntities.push_back(std::make_unique<Shot>(
                    gamer.getPos().x +
                        cos((gamer.getAngle()-90)/RADTODEG) * (50 * sqrt(3) / 4 + 30),
                    gamer.getPos().y +
                        sin((gamer.getAngle()-90)/RADTODEG) * (50 * sqrt(3) / 4 + 30)));

                auto it = onMapEntities.rbegin();
                auto t = _curWin.mapPixelToCoords(_view);
                (*it)->rotate(gamer.getAngle()-90);
                (*it)->setGamerState(true);
            }
        }

        if (ev.type == sf::Event::MouseMoved)
        {
            _view = {ev.mouseMove.x, ev.mouseMove.y};
        }
    }
}

void game::Game::_showObjects()
{
    _curWin.clear(sf::Color::Black);
    gamer.show(_curWin);
    for (const auto &i: mapManager.getRoom(room)._data)
    {
        i->show(_curWin);
    }

    for (const auto &i: onMapEntities)
    {
        i->show(_curWin);
    }

    _moveCamera();
    _guiScreens[_showingWindowIdx].show(_curWin);
    _curWin.display();
}

void game::Game::_ingameHandling(float delta)
{
    auto t = _curWin.mapPixelToCoords(_view);
    gamer.move(delta);
    gamer.rotate(t.x, t.y);
    // Искуственный интеллект
    for (auto &obj: onMapEntities)
    {
        if (obj->getType() == EntityType::Enemy)
        {
            auto &enemy = reinterpret_cast<game::AbstractEnemy &>(*obj);
            enemy.rotate(gamer.getPos().x, gamer.getPos().y);
            enemy.findPathToPlayer(gamer.getPos().x, gamer.getPos().y);
        }
    }

    for (auto &&i: onMapEntities)
    {
        i->move(delta);
    }

    for (auto &&i: mapManager.getRoom(room)._data)
    {
        // Столкновения игрока с тайлами карты
        if (gamer.collide(*i))
        {
            switch (i->getType())
            {
                case game::EntityType::Wall:
                    gamer.stop(delta, i->getPos() - gamer.getPos());
                    break;

                case game::EntityType::Port:
                    _initializeRoom(reinterpret_cast<Port &>(*i).getIdx());
                    std::cout << room << std::endl;

                default:
                    break;
            }
        }

        // Столкновения сущностей с тайлами карты
        for (auto &&obj : onMapEntities)
        {
            obj->collideHandling(*i);
        }
    }

    for (auto it = onMapEntities.begin(); it != onMapEntities.end(); it++)
    {
        if ((*it)->collide(gamer))
        {
            (*it)->collideHandling(gamer);
        }
        for (auto j = it; j != onMapEntities.end(); ++j)
        {
            if ((*it)->collide(*(*j)) && j != it)
            {
                (*it)->collideHandling(*(*j));
            }
        }
    }

    auto it = std::remove_if(onMapEntities.begin(), onMapEntities.end(), 
            [](std::unique_ptr<Movable> &i){ return !i->isExisted; });
    onMapEntities.erase(it, onMapEntities.end());
}

void game::Game::_moveCamera()
{
    sf::Vector2f newview = gamer.getPos();
    newview.x = std::max(_settings.screenSize.first/2.f, newview.x);
    newview.x = std::min(newview.x, mapManager.getRoom(room).width -
                                        _settings.screenSize.first / 2.f);

    newview.y = std::max(_settings.screenSize.second/2.f, newview.y);
    newview.y = std::min(newview.y, mapManager.getRoom(room).height -
                                        _settings.screenSize.second / 2.f);
    cam.setCenter(newview);
    _curWin.setView(cam);
}

void game::Game::_activatePorts()
{
    auto &rm = mapManager.getRoom(room);
    for (auto &idx: rm._linkedPorts)
    {
        reinterpret_cast<Port &>(*rm._data[idx]).changeActiveness(true);
    }
}

void game::Game::_generateEnemies()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> randTile(
        0, mapManager.getRoom(room)._data.size()-1);
    for (int i = 0; i < 3; ++i)
    {
        auto idx = 0;
        do
        {
            idx = randTile(gen);
        } while (mapManager.getRoom(room)._data[idx]->getType() !=
                 EntityType::None);
        auto pos = mapManager.getRoom(room)._data[idx]->getPos();

        auto j = std::make_unique<Striker>(pos.x, pos.y);
        onMapEntities.push_back(std::move(j));
    }
}

void game::Game::_initializeRoom(size_t i)
{
    _curWin.clear();
    room = i;
    AbstractEnemy::setGraph(mapManager.getRoom(room));
    onMapEntities.clear();
    gamer.setPos(Entity::BLOCK_SIZE*3, Entity::BLOCK_SIZE*3);
    if (!mapManager.getRoom(room).isCleared)
    {
        _generateEnemies();
    }
}

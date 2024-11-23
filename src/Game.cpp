#include "../include/Game.hpp"
#include "../include/vecmath.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <memory>
#include <random>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

using Anchor = game::GridPacker::Anchor;

game::Game::Game() : gamer(0, 0, true)
{
    std::vector<std::pair<unsigned int, unsigned int>> m = {
        {540, 955}, {680, 1204}, {800, 1410}, {1024, 1820}};
    for (auto &[x, y]: m)
    {
        mods.push_back({y, x}); 
    }

    Entity::BLOCK_SIZE = 50.f * m[_settings.videoModeIdx].first/1024;
    Movable::VOLUME = _settings.volume;
    gamer = Gamer();
    mapManager = std::move(MapManager());

    _curWin.create(mods[_settings.videoModeIdx], "SFML: Rogue");
    _curWin.setFramerateLimit(75);
    _curWin.setKeyRepeatEnabled(false);
    _hudfont.loadFromFile("resources/ThaleahFat.ttf");
    cam.setSize({mods[_settings.videoModeIdx].width * 1.f,
                 1.f * mods[_settings.videoModeIdx].height});
    cam.setCenter({mods[_settings.videoModeIdx].width * 0.5f,
                   0.5f * mods[_settings.videoModeIdx].height});
    _curWin.setView(cam);
    actCam = cam;
    gamer.setPos(200, 200);
    _setMenusWindows();
    game::AbstractEnemy::setScreenSize(cam.getSize());
    mapManager.generateNewLevel();
}

void game::Game::_setGreetingWindow()
{
    GridPacker greet(cam.getSize().x, cam.getSize().y),
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
    game::Slider volume(10, 50, 0, 100);
    game::Button resolutionLeft(30, 30), resolutionRight(30, 30),
                 apply(10, 10), exit(10, 10), stub(10, 10);

    videoModeIdx = _settings.videoModeIdx;
    resolutionLeft.setText("<");
    resolutionRight.setText(">");
    apply.setText("Apply");
    exit.setText("Exit");
    stub.setToggleColor(_settings.bgcol);
    stub.setBgColor(_settings.bgcol);
    stub.setOutlineColor(_settings.bgcol);

    _res = std::to_string(mods[videoModeIdx].width) + "x" +
           std::to_string(mods[videoModeIdx].height),
    _curvolume = std::to_string(_settings.volume), _volumeLabel = "Volume ";
    _warning = "Settings are applied after restarting the game";
    game::Label outputRes(10, 50, _res), curVolume(10, 50, _curvolume),
        volumeLabel(10, 50, _volumeLabel), warning(10, 50, _warning);

    resolutionLeft.setFunc(
        [this]()
        {
            if (!videoModeIdx)
                videoModeIdx = mods.size();
            videoModeIdx--;
            _res = std::to_string(mods[videoModeIdx].width) + "x" +
                   std::to_string(mods[videoModeIdx].height);
        });

    resolutionRight.setFunc(
        [this]()
        {
            videoModeIdx++;
            if (videoModeIdx == mods.size())
                videoModeIdx = 0;
            _res = std::to_string(mods[videoModeIdx].width) + "x" +
                   std::to_string(mods[videoModeIdx].height);
        });

    apply.setFunc(
        [this]()
        {
            _settings.videoModeIdx = videoModeIdx;
            _settings.volume = std::stoi(_curvolume);
        });

    volume.setFunc(
        [volume, this](double scale)
        {
            _curvolume =
                std::to_string(static_cast<int>(volume.getBorders().y * scale));
        });

    exit.setFunc([this]() { _showingWindowIdx = 0; });

    game::GridPacker main(cam.getSize().x, cam.getSize().y),
                     volstring(cam.getSize().x, 10),
                     resstr(cam.getSize().x, 10),
                     manageBtns(cam.getSize().x, 10);

    volstring.putObject(volumeLabel, 0, 0, Anchor::EXPAND, Anchor::FIXED);
    volstring.putObject(volume, 0, 1, Anchor::EXPAND, Anchor::FIXED);
    volstring.putObject(curVolume, 0, 2, Anchor::EXPAND, Anchor::FIXED);
    resstr.putObject(resolutionLeft, 0, 0);
    resstr.putObject(resolutionRight, 0, 2);
    resstr.putObject(outputRes, 0, 1);
    manageBtns.putObject(stub, 0, 0);
    manageBtns.putObject(stub, 0, 2);
    manageBtns.putObject(stub, 0, 3);
    manageBtns.putObject(stub, 0, 5);
    manageBtns.putObject(apply, 0, 1);
    manageBtns.putObject(exit, 0, 4);

    main.putObject(stub, 5, 0);
    main.putObject(volstring, 1, 0);
    main.putObject(warning, 0, 0);
    main.putObject(resstr, 3, 0);
    main.putObject(manageBtns, 4, 0);

    _guiScreens.push_back(main);
}

void game::Game::_setMenusWindows()
{
    _setGreetingWindow();
    _setSettingsWindow();
    _setEnd();
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
            _curWin.setView(actCam);
            _guiScreens[_showingWindowIdx].displayWidgets(_curWin);
            while (_curWin.pollEvent(ev))
            {
                _guiScreens[_showingWindowIdx]._invoke(_curWin, ev);
            }
            _curWin.display();
        }
    }
}


void game::Game::_gameloop()
{
    auto delta = _frameClock.restart().asSeconds();

    _inputHandling();
    _ingameHandling(delta);
    _showObjects();
}

void bob()
{}

void game::Game::_inputHandling()
{
    sf::Event ev;
    while (_curWin.pollEvent(ev))
    {
        if (ev.type == sf::Event::KeyPressed)
        {
            if (ev.key.code == sf::Keyboard::Escape)
            {
                bob();
                _gameIsRunning = false;
                _showingWindowIdx = 0;
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
            else if (ev.key.code == sf::Keyboard::Space)
            {
                gamer.dash();
            }
            else if (ev.key.code == sf::Keyboard::E)
            {
                gamer.isUlting = true;
            }
            else if (ev.key.code == sf::Keyboard::Tab)
            {
                _isMinimapShowed = true;
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
            else if (ev.key.code == sf::Keyboard::Tab)
            {
                _isMinimapShowed = false;
            }
            else if (ev.key.code == sf::Keyboard::E)
            {
                gamer.isUlting = false;
            }
        }

        if (ev.type == sf::Event::MouseMoved)
        {
            _view = {ev.mouseMove.x, ev.mouseMove.y};
        }

        if (ev.type == sf::Event::MouseButtonPressed)
        {
            if (ev.mouseButton.button == sf::Mouse::Left)
            {
                gamer.isShooting = true;
            }
        }

        if (ev.type == sf::Event::MouseButtonReleased)
        {
            if (ev.mouseButton.button == sf::Mouse::Left)
            {
                gamer.isShooting = false;
            }
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

    for (const auto &i: mapManager.getRoom(room)._itemData)
    {
        i->show(_curWin);
    }

    for (const auto &i: onMapEntities)
    {
        i->show(_curWin);
    }

    _moveCamera();
    // _guiScreens[_showingWindowIdx].show(_curWin);
    _showGameHUD();
    if (_isMinimapShowed) _showMinimap();
    _curWin.display();
}

void game::Game::_ingameHandling(float delta)
{
    bool _needNewLevel = false;
    auto t = _curWin.mapPixelToCoords(_view);
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
                    break;

                case game::EntityType::LevelPort:
                    _needNewLevel = true;
                    enemies += 2;
                    gamer.score += level*1000;
                    break;

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

    // Обработка коллизии с предметами. Можно выбрать только один из нескольких
    for (auto &i: mapManager.getRoom(room)._itemData)
    {
        if (gamer.collide(*i) && i->getType() == EntityType::Item)
        {
            gamer.addItem(i->itemType);
            mapManager.getRoom(room)._itemData.clear();
            break;
        }
    }
    
    gamer.move(delta);
    gamer.rotate(t.x, t.y);
    if (gamer.isShooting)
    {
        auto shot = gamer.shot(delta);
        if (shot.has_value())
        {
            if (shot.value()->hasExplosive)
            {
                auto exp = std::make_unique<Fireball>(shot.value()->getPos().x,
                                                      shot.value()->getPos().y);
                exp->rotate(gamer.getAngle());
                exp->masterType = EntityType::Gamer;
                exp->col = sf::Color::Green;
                onMapEntities.push_back(std::move(exp));
            }
            onMapEntities.push_back(std::move(shot.value()));
        }
    }

    if (gamer.isUlting)
    {
        auto shot = gamer.ult(delta);
        if (shot.has_value())
        {
            onMapEntities.push_back(std::move(shot.value()));
        }

    }

    // Искуственный интеллект
    for (auto &obj: onMapEntities)
    {
        if (obj->getType() == EntityType::Enemy)
        {
            auto &enemy = reinterpret_cast<game::AbstractEnemy &>(*obj);
            enemy.rotate(gamer.getPos().x, gamer.getPos().y);
            enemy.setPanicPoint(mapManager.getRoom(room)._data, gamer.getPos(),
                                delta);
            enemy.processMoving(gamer.getPos());
            auto enshot = enemy.shot(delta);
            if (enshot.has_value())
            {
                onMapEntities.push_back(std::move(enshot.value()));
            }
        }
    }

    for (auto &&i: onMapEntities)
    {
        i->move(delta);
    }

    // Поскольку монстрики могут умереть только от игрока, мы даем награды за
    // тех, у кого isExisted == false
    for (auto &i: onMapEntities)
    {
        if (!i->isExisted)
        {
            gamer.score += i->getBounty();
        }
    }

    auto it = std::remove_if(onMapEntities.begin(), onMapEntities.end(), 
            [](std::unique_ptr<Movable> &i){ return !i->isExisted; });
    onMapEntities.erase(it, onMapEntities.end());

    if (_needNewLevel)
    {
        mapManager.generateNewLevel();
        ++level;
        _initializeRoom(0);
    }

    {
        int enemycnt = 0;
        for (auto &i : onMapEntities)
        {
            if (i->getType() == EntityType::Enemy)
                ++enemycnt;
        }
        if (!enemycnt)
        {
            mapManager.getRoom(room).isCleared = true;
            mapManager.getRoom(room).activateItems();
            _activatePorts();
        }
    }

    {
        gamerScoreClock += delta;
        if (gamerScoreClock >= 0.1) 
        {
            gamer.score += level;
            gamerScoreClock = 0;
        }
    }

    if (gamer.getHp() == 0)
    {
        if (gamer.lives > 0)
        {
            gamer.addItem(Item::HEALING);
            gamer.lives--;
        }
        else
        {
            _gameIsRunning = false;
            _result = "You result: " + std::to_string(gamer.score);
            _showingWindowIdx = 2;
            onMapEntities.clear();
            mapManager.generateNewLevel();
            room = 0;
            level = 1;
            enemies = 1;
            _initializeRoom(0);
            gamer =
                game::Gamer(Entity::BLOCK_SIZE * 3.f, Entity::BLOCK_SIZE * 3.f);
        }
    }
}

void game::Game::_moveCamera()
{
    sf::Vector2f newview = gamer.getPos();
    newview.x = std::max(cam.getSize().x/2.f, newview.x);
    newview.x = std::min(newview.x, mapManager.getRoom(room).width -
                                        cam.getSize().x / 2.f);

    newview.y = std::max(cam.getSize().y/2.f, newview.y);
    newview.y = std::min(newview.y, mapManager.getRoom(room).height -
                                        cam.getSize().y / 2.f);
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
    if (rm.isLvlPort)
    {
        reinterpret_cast<LevelPort &>(*rm._data[rm._lvlPortIdx])
            .changeActiveness(true);
    }
}

void game::Game::_generateEnemies()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> randTile(
        0, mapManager.getRoom(room)._data.size()-1);
    std::uniform_real_distribution<float> randEnemy;

    for (int i = 0; i < enemies; ++i)
    {
        auto idx = 0;
        do
        {
            idx = randTile(gen);
        } while (mapManager.getRoom(room)._data[idx]->getType() !=
                 EntityType::None);
        auto pos = mapManager.getRoom(room)._data[idx]->getPos();

        std::unique_ptr<game::Movable> enemy;

        bool isChosen = false;
        while (!isChosen)
        {
            float prob = randEnemy(gen);
            if (prob <= 0.2)
            {
                enemy = std::make_unique<Striker>(pos.x, pos.y);
                isChosen = true;
            }
            else if (prob > 0.2 && prob <= 0.4 && level > 1)
            {
                enemy = std::make_unique<Sniper>(pos.x, pos.y);
                isChosen = true;
            }
            else if (prob > 0.4 && prob <= 0.6 && level > 1)
            {
                enemy = std::make_unique<Bigboy>(pos.x, pos.y);
                isChosen = true;
            }
            else if (prob > 0.6 && prob <= 0.8 && level > 2)
            {
                enemy = std::make_unique<Wizard>(pos.x, pos.y);
                isChosen = true;
            }
            else if (prob > 0.8 && prob <= 1 && level > 2)
            {
                enemy = std::make_unique<Wolf>(pos.x, pos.y);
                isChosen = true;
            }
        }

        onMapEntities.push_back(std::move(enemy));
    }
}

void game::Game::_initializeRoom(size_t i)
{
    _curWin.clear();
    room = i;
    Lazer::setMap(&mapManager.getRoom(room)._data);
    AbstractEnemy::setGraph(mapManager.getRoom(room));
    onMapEntities.clear();
    gamer.setPos(Entity::BLOCK_SIZE*3, Entity::BLOCK_SIZE*3);

    if (!mapManager.getRoom(room).isCleared)
    {
        // Случайная возможность того, что увеличится число врагов
        std::random_device seed;
        std::mt19937 gen(seed());
        std::uniform_real_distribution<float> randIncEnemy;
        if (randIncEnemy(gen) <= 0.1) ++enemies;
        _generateEnemies();
    }
}


void game::Game::_showGameHUD()
{
    std::string tempstr = std::to_string(gamer.getHp()) + "\\" +
                           std::to_string(gamer.getMaxHp()) + " hp";
    sf::Text hudhp(tempstr, _hudfont, Entity::BLOCK_SIZE*0.6f);
    hudhp.setPosition(
        cam.getCenter() - cam.getSize() / 2.f +
        sf::Vector2f{Entity::BLOCK_SIZE, Entity::BLOCK_SIZE * 0.4f});

    hudhp.setFillColor(sf::Color::Red);
    hudhp.setOutlineColor(sf::Color::Black);
    hudhp.setOutlineThickness(2.f);

    std::stringstream scoreStream;
    scoreStream << "score: " << std::setfill('0') << std::setw(7) << gamer.score;
    tempstr = std::string(std::istreambuf_iterator(scoreStream), {});
    sf::Text hudscore(tempstr, _hudfont, Entity::BLOCK_SIZE*0.6f);
    hudscore.setPosition(
        cam.getCenter() +
        sf::Vector2f{cam.getSize().x / 2.f - 4.6f * Entity::BLOCK_SIZE,
                     -cam.getSize().y / 2.f + 0.4f * Entity::BLOCK_SIZE});

    hudscore.setFillColor(sf::Color::Red);
    hudscore.setOutlineColor(sf::Color::Black);
    hudscore.setOutlineThickness(2.f);

    tempstr = "dash: " + std::to_string(gamer.getDashCharge()) + "%";
    sf::Text huddash(tempstr, _hudfont, Entity::BLOCK_SIZE*0.6f);
    huddash.setPosition(
        cam.getCenter() +
        sf::Vector2f{cam.getSize().x / 2.f - 4.6f * Entity::BLOCK_SIZE,
                     cam.getSize().y / 3.f + 2.f * Entity::BLOCK_SIZE});
    huddash.setFillColor(sf::Color::Red);
    huddash.setOutlineColor(sf::Color::Black);
    huddash.setOutlineThickness(2.f);

    tempstr = "ult: " + std::to_string(gamer.getUltCharge()) + "%";
    sf::Text hudult(tempstr, _hudfont, Entity::BLOCK_SIZE*0.6f);

    hudult.setPosition(
        cam.getCenter() +
        sf::Vector2f{cam.getSize().x / 2.f - 7.6f * Entity::BLOCK_SIZE,
                     cam.getSize().y / 3.f + 2.f * Entity::BLOCK_SIZE});

    hudult.setFillColor(sf::Color::Red);
    hudult.setOutlineColor(sf::Color::Black);
    hudult.setOutlineThickness(2.f);

    _curWin.draw(hudhp);
    _curWin.draw(hudscore);
    _curWin.draw(huddash);
    _curWin.draw(hudult);
}


void game::Game::_showMinimap()
{
    sf::RectangleShape blur(cam.getSize());
    blur.setFillColor({0,0,0,200});
    blur.setPosition(cam.getCenter()-cam.getSize()/2.f);
    _curWin.draw(blur);

    // Рисуем граф
    sf::CircleShape dotsgen(cam.getSize().y/3.f, mapManager.LEVELNUM); 
    dotsgen.setOrigin(dotsgen.getLocalBounds().getSize()/2.f);
    dotsgen.setPosition(cam.getCenter());
    std::vector<sf::Vector2f> dots;

    for (int i = 0; i < mapManager.LEVELNUM; ++i)
    {
        dots.push_back(dotsgen.getPoint(i) + dotsgen.getPosition() -
                       dotsgen.getOrigin());
    }

    for (int i = 0; i < mapManager.LEVELNUM; ++i)
    {
        for (auto &j: mapManager.curGraph[i])
        {
            if (j > i) break;
            sf::RectangleShape link(
                {vabs(dots[j] - dots[i]) + Entity::BLOCK_SIZE / 5.f,
                 Entity::BLOCK_SIZE / 5.f});

            link.setOrigin({0, 5});
            link.setFillColor({50, 50, 50});
            link.setOutlineThickness(2.f);
            link.setOutlineColor(sf::Color::Black);
            link.setPosition(
                dots[i] +
                sf::Vector2f{Entity::BLOCK_SIZE, Entity::BLOCK_SIZE} * 0.5f);
            link.setRotation(360 - vdeg(dots[j] - dots[i], {1, 0}));
            _curWin.draw(link);
        }
    }


    for (int i = 0; i < mapManager.LEVELNUM; ++i)
    {
        sf::RectangleShape roomrect({Entity::BLOCK_SIZE, Entity::BLOCK_SIZE});
        sf::Color roomcol = sf::Color::Magenta;
        if (!mapManager.getRoom(i).isCleared && i != room)
        {
            roomcol.r *= 0.3;
            roomcol.g *= 0.3;
            roomcol.b *= 0.3;
        }

        if (i == mapManager.getEndPoint())
        {
            roomcol = sf::Color::Yellow;
        }

        if (i == room)
        {
            roomcol = sf::Color::Green;
        }

        roomrect.setFillColor(roomcol);
        roomrect.setPosition(dots[i]);
        
        sf::Text number(std::to_string(i), _hudfont, 0.8f*Entity::BLOCK_SIZE);
        number.setPosition(
            roomrect.getPosition() + roomrect.getSize() / 2.f -
            sf::Vector2f{number.getLocalBounds().width * 0.5f,
                         1.75f * number.getGlobalBounds().height});

        // точка основы зависит от шрифта тупого блин((
        number.setOutlineThickness(2.f);
        if (mapManager.getRoom(i).withItems)
        {
            number.setFillColor(sf::Color::Yellow);
        }

        _curWin.draw(roomrect);
        _curWin.draw(number);
    }

    std::string temp = "Level: " + std::to_string(level);
    sf::Text lvlhud(temp, _hudfont);
    lvlhud.setOrigin({lvlhud.getLocalBounds().width * 0.2f, 0});
    lvlhud.setPosition(
        cam.getCenter() -
        sf::Vector2f{0, cam.getSize().y / 2.f - Entity::BLOCK_SIZE * 0.4f});
    _curWin.draw(lvlhud);
}

void game::Game::_setEnd()
{
    GridPacker greet(cam.getSize().x, cam.getSize().y),
               forText(30, 30);

    game::Button exit(200, 150),
                 centerVoid(10, 10);

    game::Label res(10, 10, _result);

    exit.setText("Exit");
    exit.setFunc([this](){ 
            _showingWindowIdx = 0; 
        });

    centerVoid.setBgColor(_settings.bgcol);
    centerVoid.setToggleColor(_settings.bgcol);
    centerVoid.setOutlineColor(_settings.bgcol);

    forText.putObject(centerVoid, 0, 1);
    forText.putObject(res, 0, 0);

    greet.putObject(centerVoid, 0, 0);
    greet.putObject(forText, 1, 0);
    greet.putObject(centerVoid, 2, 0);
    greet.putObject(centerVoid, 4, 0);
    greet.putObject(exit, 3, 0);
    _guiScreens.push_back(greet);
}

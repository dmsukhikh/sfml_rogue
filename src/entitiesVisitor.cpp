#include "Game.hpp"
#include "entitiesVisitor.hpp"

game::DefaultVisitor::DefaultVisitor(game::Game *game) : _game(game) {}

void game::DefaultVisitor::visitWall(Wall &i)
{
    _game->visitWall(i);
}

void game::DefaultVisitor::visitPort(Port &p)
{
    _game->visitPort(p);
}

void game::DefaultVisitor::visitLevelPort()
{
    _game->visitLevelPort();
}

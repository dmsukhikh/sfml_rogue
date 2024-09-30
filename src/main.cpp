#include "../gamegui/GameGUI.hpp"
#include "AbstractPacker.hpp"
#include "GuiObject.hpp"
#include <iostream>
#include <memory>
#include <string>

int main()
{
    sf::RenderWindow j(sf::VideoMode(1024, 720), "SFML: Bebra");

    auto jopa = game::GridPacker(500, 500);
    auto jopasmall = game::GridPacker(400, 400);
    jopa._setPos(50, 50);
    jopa.putObject(game::Button(10, 10, "wawa"), 0, 0);
    jopa.putObject(game::Button(10, 10, "wowe"), 1, 0);
    jopa.putObject(game::Button(10, 10, "sosi"), 0, 1);
    auto a = game::Button(100, 100, "3", 0, sf::Color::Black, sf::Color::Cyan);
    jopasmall.putObject(a, 0, 0);
    jopasmall.putObject(a, 1, 0);
    jopasmall.putObject(a, 0, 1);
    jopasmall.putObject(a, 1, 1);
    jopa.putObject(jopasmall, 1, 1);

    std::unique_ptr<game::GuiObject> jopaptr(jopa.clone());

    uint64_t x = 0, y = 0;
    while (j.isOpen())
    {
        j.clear(sf::Color::White);
        jopa._show(j);
        j.display();
    }
}

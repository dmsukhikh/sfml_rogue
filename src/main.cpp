#include "../gamegui/GameGUI.hpp"
#include <iostream>
#include <string>

void f()
{
    std::cout << "gool" << std::endl;
}

int main()
{
    sf::RenderWindow j(sf::VideoMode(1500, 720), "SFML: Bebra");

    game::GridPacker jopa(600, 400), jopasmall(0, 0);
    game::Button upbtn(100, 100, "click me");
    game::Button dwbtn(100, 100, "click me", 0, sf::Color::Black, sf::Color::Cyan);

    upbtn.setFunc([](){std::cout << "clicked upper button" << std::endl;});
    dwbtn.setFunc([](){std::cout << "clicked lower button" << std::endl;});

    jopasmall.putObject(dwbtn, 0, 0);
    jopasmall.putObject(dwbtn, 1, 0);
    jopasmall.putObject(dwbtn, 0, 1);
    jopasmall.putObject(dwbtn, 1, 1);
    jopa.putObject(upbtn, 0, 0);
    jopa.putObject(upbtn, 1, 0);
    jopa.putObject(upbtn, 0, 1);
    jopa.putObject(jopasmall, 1, 1);

    sf::Event ev;

    while (j.isOpen())
    {
        j.clear(sf::Color::Magenta);
        jopa._show(j);
        j.display();

        while (j.pollEvent(ev))
        {
            jopa._invoke(ev);
        }
    }
}

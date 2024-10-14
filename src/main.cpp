#include "../gamegui/GameGUI.hpp"
#include <SFML/Graphics/Color.hpp>
#include <iostream>
#include <string>

void f()
{
    std::cout << "gool" << std::endl;
}

int main()
{
    sf::RenderWindow j(sf::VideoMode(1500, 720), "SFML: Bebra");
    game::GridPacker bob(1200, 300);
    game::Slider slide(10, 10, 0, 100);

    auto i = slide.getBorders();
    std::string out =
        std::to_string((int)(slide.getBorders().x + slide.getBorders().y) / 2);
    slide.setFunc([i, &out](double scale)
                  { out = std::to_string((int)(i.y * scale)); });

    game::Label boba(10, 10, out);
    boba.setColor({100, 200, 100});

    bob.putObject(slide, 0, 0);
    bob.putObject(boba, 0, 1);


    sf::Event ev;

    int x =0;
    while (j.isOpen())
    {
        j.clear(sf::Color::Magenta);
        bob.show(j);
        j.display();

        while (j.pollEvent(ev))
        {
            bob._invoke(j, ev);
        }
    }
}

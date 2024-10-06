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

    sf::Color bgcol{255/2, 200, 255};
    int left = 0, right = 255;
    auto changeColor = [right, &bgcol](double scale)
    {
        bgcol.r = scale*right;
    };

    game::GridPacker jopa(300, 400), jopasmall(0, 0);
    game::Slider coc(600, 40, left, right);
    game::Button no(30, 30);
    no.setText("bebra");
    coc.setFunc(changeColor);

    jopa.putObject(coc, 1, 0, game::GridPacker::Anchor::EXPAND);
    jopa.putObject(no, 0, 0);


    sf::Event ev;

    int x =0;
    while (j.isOpen())
    {
        j.clear(bgcol);
        jopa._show(j);
        j.display();

        while (j.pollEvent(ev))
        {
            jopa._invoke(j, ev);
        }
    }
}

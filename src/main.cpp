#include <SFML/Graphics.hpp>
#include "../include/gamegui/Button.hpp"


int main()
{
    sf::RenderWindow j(sf::VideoMode(1024, 720), "SFML: Bebra");

    game::Button b(150, 55, "I don't care", 25);
    while (j.isOpen())
    {
        j.clear(sf::Color::Black);
        b.show(j);
        j.display();
    }
}

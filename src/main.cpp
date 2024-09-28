#include "../gamegui/GameGUI.hpp"

int main()
{
    sf::RenderWindow j(sf::VideoMode(1024, 720), "SFML: Bebra");

    game::AbsolutePacker pack(1024, 720);
    pack.putObject(game::Button(200, 100, "malaybilay"), 0, 50);
    pack.putObject(game::Button(300, 150, "malaybilay"), 0, 200);
    pack.putObject(game::Button(400, 200, "malaybilay"), 600, 50);
    game::AbsolutePacker anp(0, 0);
    anp.putObject(pack, 0, 0);

    while (j.isOpen())
    {
        j.clear(sf::Color::Magenta);
        anp.displayWidgets(j);
        j.display();
    }
}

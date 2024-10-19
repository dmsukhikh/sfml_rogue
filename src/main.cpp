#include "../include/Game.hpp"
#include "GameGUI.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <vector>

using Anchor = game::GridPacker::Anchor;

int main()
{
    game::Game game;
    game.mainloop();
    
}

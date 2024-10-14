#include "../../include/gamegui/GuiObject.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

game::GuiObject::GuiObject(uint64_t w, uint64_t h) : _height(h), _width(w) {}

void game::GuiObject::_resize(uint64_t width, uint64_t height)
{
    _height = height;
    _width = width;
}

void game::GuiObject::_setPos(uint64_t x, uint64_t y) 
{
    _x = x;
    _y = y;
}

game::Dot<uint64_t> game::GuiObject::getPos() const
{
    return {_x, _y};
}

game::Dot<uint64_t> game::GuiObject::getSize() const
{
    return {_width, _height};
}

void game::GuiObject::_invoke(const sf::RenderWindow &capture, const sf::Event &ev)
{

}

#include "../../include/gamegui/Image.hpp"
#include <SFML/Graphics/RenderWindow.hpp>


game::Image::Image(const std::string &filename)
    : GuiObject(0, 0)
{
    _tx.loadFromFile(filename);
    _im.setTexture(_tx);
    _width = _im.getGlobalBounds().width;
    _height = _im.getGlobalBounds().height;
}

game::Image::Image(const Image &op)
    : GuiObject(op._width, op._height), _tx(op._tx), _im(op._tx)
{
    _im.setScale(op._im.getScale());
    _im.setPosition(op._im.getPosition());
}

void game::Image::_resize(uint64_t width, uint64_t height)
{
    GuiObject::_resize(width, height);
    _im.setScale(sf::Vector2f(width / _im.getLocalBounds().width,
                              height / _im.getLocalBounds().height));
}

void game::Image::_setPos(uint64_t x, uint64_t y)
{
    GuiObject::_setPos(x, y);
    _im.setPosition(sf::Vector2f(x, y));
}

void game::Image::_show(sf::RenderWindow &rend)
{
    rend.draw(_im);
}

std::unique_ptr<game::GuiObject> game::Image::clone() const
{
    auto temp = std::make_unique<Image>(*this);
    return temp;
}

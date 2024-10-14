#include "../../include/gamegui/AbstractPacker.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <memory>
#include "../../include/gamegui/Button.hpp"

game::AbstractPacker::AbstractPacker(uint64_t width, uint64_t height)
    : game::GuiObject(width, height)
{
}


game::AbstractPacker::AbstractPacker(AbstractPacker &&op) noexcept
    : game::GuiObject(0, 0)
{
    _data = std::move(op._data);
    _width = op._width;
    _height = op._height;

    op._data = std::vector<std::unique_ptr<GuiObject>>();
    _width = 0;
    _height = 0;
}

game::AbstractPacker& game::AbstractPacker::operator=(AbstractPacker &&op) noexcept
{
    if (this != &op)
    {
        _data = std::move(op._data);
        _width = op._width;
        _height = op._height;

        op._data = std::vector<std::unique_ptr<GuiObject>>();
        _width = 0;
        _height = 0;
    }
    return *this;
}

void game::AbstractPacker::show(sf::RenderWindow &op)
{
    displayWidgets(op);
}

void game::AbstractPacker::displayWidgets(sf::RenderWindow &op)
{
    for (auto &&i: _data) 
    {
        i->show(op);
    }
}

game::AbstractPacker::~AbstractPacker() = default;

std::unique_ptr<game::GuiObject> game::AbstractPacker::clone() const
{
    std::vector<std::unique_ptr<GuiObject>> temp(_data.size());
    for (std::size_t i = 0; i < _data.size(); ++i)
    {
        temp[i] = _data[i]->clone();
    }

    auto t = std::make_unique<AbstractPacker>(_width, _height);
    t->_data = std::move(temp);
    t->_x = _x;
    t->_y = _y;
    return t;
}


void game::AbstractPacker::_setPos(uint64_t newx, uint64_t newy)
{
    for (auto &&i: _data)
        i->_setPos(i->getPos().x - _x + newx, i->getPos().y - _y + newy);
    game::GuiObject::_setPos(newx, newy);
}

void game::AbstractPacker::_invoke(const sf::RenderWindow &capture,
                                   const sf::Event &ev)
{
    for (auto &&i: _data) i->_invoke(capture, ev);
}

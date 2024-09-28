#include "../../include/gamegui/Button.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <memory>

class game::Button::_cimpl
{
  public:
    static sf::Font _f;
    static bool _isFontInit;

    sf::Text _text;
    sf::Color _bg;
    uint64_t _font;
    sf::RectangleShape _rawButton;

    void _resizeRawButton(uint64_t newWidth, uint64_t newHeight)
    {
        _rawButton.setSize(
            {static_cast<float>(newWidth), static_cast<float>(newHeight)});
    }

    _cimpl(uint32_t width, uint32_t height, const std::string &text, 
           uint32_t font, sf::Color fg, sf::Color bg) : _font(font)
    {
        // Наверное, так делать нельзя. But I don't care, my life - my rules
        if (!_isFontInit)
        {
            // Указываем путь, как будто мы пишем в main.cpp
            // Наверное, так хардкодить нельзя. But I don't care
            _f.loadFromFile("../resources/ThaleahFat.ttf");
            _isFontInit = true;
        }
        _rawButton = sf::RectangleShape(
            {static_cast<float>(width), static_cast<float>(height)});
        _rawButton.setFillColor(bg);
        _text.setString(text);
        _text.setFont(_f);
        _text.setFillColor(fg);
        _setTextProperties(width, height);
    }

    void _setTextProperties(uint64_t width, uint64_t height)
    {
        std::size_t chsize;
        if (_font)
        {
            chsize = _font;
        }
        else
        {
            chsize = std::min(1.5*width/(_text.getString().getSize()), height/1.5);
        }
        _text.setCharacterSize(chsize);
        _text.setOrigin({_text.getLocalBounds().getSize().x / 2,
                         _text.getLocalBounds().getSize().y / 2});
        _text.setPosition(
            _rawButton.getPosition().x + _rawButton.getSize().x / 2,
            _rawButton.getPosition().y + _rawButton.getSize().y / 2 - chsize/2.);
        //_text.setOrigin(0, 0);
    }

    void show(sf::RenderWindow &target_window)
    {
        target_window.draw(_rawButton); 
        target_window.draw(_text); 
    }
};

bool game::Button::_cimpl::_isFontInit = false;
sf::Font game::Button::_cimpl::_f{ };

game::Button::Button(uint32_t width, uint32_t height, const std::string &txt,
                     uint32_t font, sf::Color fg, sf::Color bg)
    : GuiObject(width, height)
{
    _impl = std::make_unique<_cimpl>(width, height, txt, font, fg, bg);
}

game::Button::Button(const Button &op) noexcept : _impl(nullptr), GuiObject(0, 0)
{
    if (op._impl != nullptr) 
    {
        _height = op._height;
        _width = op._width;
        *_impl = *op._impl;
    }
}

game::Button &game::Button::operator=(const Button &op) noexcept
{
    if (&op != this)
    {
        *_impl = *op._impl;
    }
    return *this;
}

game::Button &game::Button::operator=(Button &&op) noexcept
{
    if (&op != this && op._impl != nullptr)
    {
        _impl = std::move(op._impl);
        op._impl = nullptr;
    }
    return *this;
}

game::Button::Button(Button &&op) noexcept : _impl(nullptr), GuiObject(0, 0)
{
    if (op._impl) 
    {
        _impl = std::move(op._impl);
        _height = op._height;
        _width = op._width;
        op._impl = nullptr;
        op._height = 0;
        op._width = 0;
    }
}


std::unique_ptr<game::GuiObject> game::Button::clone() const
{
    std::string rawtxt = _impl->_text.getString().toAnsiString();
    auto _font = _impl->_font;
    auto _fg = _impl->_text.getFillColor();
    auto _bg = _impl->_rawButton.getFillColor();

    auto temp = std::make_unique<game::Button>(_width, _height, rawtxt, _font,
                                               _fg, _bg);
    temp->_setPos(_impl->_rawButton.getPosition().x,
                  _impl->_rawButton.getPosition().y);
    return temp;
}

game::Button::~Button() = default;


void game::Button::_show(sf::RenderWindow &op)
{
    op.draw(_impl->_rawButton);
    op.draw(_impl->_text);
}

void game::Button::_resize(uint64_t width, uint64_t height)
{
    game::GuiObject::_resize(width, height);
    _impl->_rawButton.setSize(
        {static_cast<float>(width), static_cast<float>(height)});
    // подумать над ресайзом текста
}

void game::Button::_setPos(uint64_t x, uint64_t y)
{
    game::GuiObject::_setPos(x, y);
    _impl->_rawButton.setPosition(x, y);
    _impl->_setTextProperties(_width, _height);
}

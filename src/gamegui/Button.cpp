#include "../../include/gamegui/Button.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstdint>
#include <memory>
// #include <iostream>

class game::Button::_cimpl
{
  public:
    static sf::Font _f;
    static bool _isFontInit;

    bool _isToggled = false;
    sf::Text _text;
    sf::Color _bg = sf::Color::White, 
              _fg = sf::Color::Black, 
              _toggledBg = sf::Color::Green,
              _outlinecol = sf::Color::Black;
    std::function<void()> _invokingFunction;
    uint64_t _font = 0;
    float _BorderThickness = 5.0;
    sf::RectangleShape _rawButton;

    void _resizeRawButton(uint64_t newWidth, uint64_t newHeight)
    {
        _rawButton.setSize(
            {static_cast<float>(newWidth), static_cast<float>(newHeight)});
    }

    // ! _BorderThickness <= width, height, по умолчанию 5
    _cimpl(uint32_t width, uint32_t height)
    {
        // Наверное, так делать нельзя. But I don't care, my life - my rules
        if (!_isFontInit)
        {
            // Указываем путь, как будто мы пишем в main.cpp
            // Наверное, так хардкодить нельзя. But I don't care
            _f.loadFromFile("../resources/ThaleahFat.ttf");
            _isFontInit = true;
        }

        _toggledBg = sf::Color::Green;

        _invokingFunction = [](){};
        _rawButton = sf::RectangleShape(
            {static_cast<float>(width-1*_BorderThickness), 
             static_cast<float>(height-1*_BorderThickness)});
        _rawButton.setFillColor(_bg);
        _rawButton.setOutlineColor(_outlinecol);
        _rawButton.setOutlineThickness(_BorderThickness);
        _text.setFont(_f);
        _text.setFillColor(_fg);
        _setTextProperties(width, height);
    }

    // Нужен при ресайзе кнопки
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

game::Button::Button(uint32_t width, uint32_t height)
    : GuiObject(width, height)
{
    _impl = std::make_unique<_cimpl>(width, height);
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
    auto temp = std::make_unique<game::Button>(_width, _height);
    temp->setBgColor(_impl->_bg);
    temp->setFgColor(_impl->_fg);
    temp->setOutlineColor(_impl->_outlinecol);
    temp->setOutlineThickness(_impl->_BorderThickness);
    temp->setToggleColor(_impl->_toggledBg);
    temp->setFont(_impl->_font);
    temp->setText(_impl->_text.getString());
    temp->setFunc(_impl->_invokingFunction);
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
        {static_cast<float>(width-1*_impl->_BorderThickness), 
         static_cast<float>(height-1*_impl->_BorderThickness)});
    _impl->_setTextProperties(width, height);
}

void game::Button::_setPos(uint64_t x, uint64_t y)
{
    game::GuiObject::_setPos(x, y);
    _impl->_rawButton.setPosition(x, y);
    _impl->_setTextProperties(_width, _height);
}

void game::Button::_invoke(const sf::Event &ev) 
{
    switch (ev.type)
    {
    case sf::Event::EventType::MouseButtonPressed:
        if (_impl->_rawButton.getGlobalBounds().contains(
                sf::Vector2f(ev.mouseButton.x, ev.mouseButton.y)))
        {
            _impl->_invokingFunction();
        }
        break;

    case sf::Event::EventType::MouseMoved:
        if (_impl->_rawButton.getGlobalBounds().contains(
                sf::Vector2f(ev.mouseMove.x, ev.mouseMove.y)) &&
            !_impl->_isToggled)
        {
            _impl->_isToggled = true;
            _impl->_rawButton.setFillColor(_impl->_toggledBg);
        }
        else if (!_impl->_rawButton.getGlobalBounds().contains(
                     sf::Vector2f(ev.mouseMove.x, ev.mouseMove.y)) &&
                 _impl->_isToggled)
        {
            _impl->_isToggled = false;
            _impl->_rawButton.setFillColor(_impl->_bg);
        }
        break;
    

    default:
        break;
    }
}

void game::Button::setFunc(const std::function<void()> &f)
{
    _impl->_invokingFunction = f;
}

void game::Button::setBgColor(const sf::Color &col)
{
    _impl->_bg = col;
    _impl->_rawButton.setFillColor(col);
}

void game::Button::setFgColor(const sf::Color &col)
{
    _impl->_fg = col;
    _impl->_text.setFillColor(col);
}

void game::Button::setOutlineColor(const sf::Color &col)
{
    _impl->_outlinecol = col;
    _impl->_rawButton.setOutlineColor(col);
}

void game::Button::setToggleColor(const sf::Color &col)
{
    _impl->_toggledBg = col;
}

void game::Button::setText(const std::string &txt)
{
    _impl->_text.setString(txt);
    _impl->_setTextProperties(_width, _height);
}

void game::Button::setFont(uint64_t font)
{
   _impl->_font = font;
   _impl->_setTextProperties(_width, _height);
}

void game::Button::setOutlineThickness(float thickness)
{
    _impl->_BorderThickness = thickness;
    _impl->_rawButton.setOutlineThickness(thickness);
}

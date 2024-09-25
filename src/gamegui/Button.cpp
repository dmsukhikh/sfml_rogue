#include "../../include/gamegui/Button.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class game::Button::_cimpl
{
    // Сделать еще операторы копирующего и move-присваивания

  public:
    static sf::Font _f;
    static bool _isFontInit;

    uint32_t _width, _height, _trueWidth, _trueHeight;
    sf::Text _text;
    sf::Color _bg;
    sf::RectangleShape _rawButton;

    void _resizeRawButton()
    {
        _rawButton.setSize(
            {static_cast<float>(_trueWidth), static_cast<float>(_trueHeight)});
    }

    _cimpl(uint32_t width, uint32_t height, const std::string &text, 
           uint32_t font, sf::Color bg, sf::Color fg) : _width(width), _height(height)
    {
        // Наверное, так делать нельзя. But I don't care, my life - my rules
        if (!_isFontInit)
        {
            // Указываем путь, как будто мы пишем в main.cpp
            // Наверное, так хардкодить нельзя. But I don't care
            _f.loadFromFile("../resources/ThaleahFat.ttf");
            _isFontInit = true;
        }

        _trueHeight = _height;
        _trueWidth = _width;
        _rawButton = sf::RectangleShape(
            {static_cast<float>(_trueWidth), static_cast<float>(_trueHeight)});
        _rawButton.setFillColor(bg);
        _text.setString(text);
        _text.setFont(_f);
        _text.setFillColor(fg);
        _text.setCharacterSize(font);
        _text.setPosition(_rawButton.getPosition());
    }

    void resizeButton(uint32_t new_height, uint32_t new_width)
    {
        _trueHeight = new_height;
        _trueWidth = new_width;
        _resizeRawButton();
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
                     uint32_t font, sf::Color bg, sf::Color fg)
{
    _impl = std::make_unique<_cimpl>(width, height, txt, font, bg, fg);
}

game::Button::Button(const Button &op) noexcept : _impl(nullptr)
{
    if (op._impl != nullptr) *_impl = *op._impl;
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

game::Button::Button(Button &&op) noexcept : _impl(nullptr)
{
    if (op._impl) 
    {
        _impl = std::move(op._impl);
        op._impl = nullptr;
    }
}


std::unique_ptr<game::GuiObject> game::Button::clone() const
{
    return std::make_unique<game::Button>(*this);
}

game::Button::~Button() = default;


void game::Button::show(sf::RenderWindow &op)
{
    op.draw(_impl->_rawButton);
    op.draw(_impl->_text);
}

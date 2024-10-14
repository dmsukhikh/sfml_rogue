#include "../../include/gamegui/Slider.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>
#include <memory>

class game::Slider::_cimpl
{
  public:
    bool _isToggled = false;
    const uint16_t switcherSize = 20;
    game::Slider &base;
    sf::RectangleShape leftSide, rightSide, switcher;
    sf::Color lsColor = sf::Color::Green, 
              rsColor = {100, 100, 100}, 
              swColor = {45,  45,  45 }, 
              outlineColor = sf::Color::Black;
    double leftBorder, rightBorder, curCount = 0.5,
           borderThickness = 5.0;

    // Функция, вызывающаяся при движении ползунка. Принимает текущее значение
    // слайдера
    std::function<void(double)> invokingFunc = [](double){};

    // Возможно, здесь происходит RVO
    auto _getDrawVec() { return std::vector{&leftSide, &rightSide, &switcher}; }

    // Активируется при перемещении курсора. Двигает ползунок в позицию x,
    // изменяя прямоугольники leftSide и rightSide
    void _moveRectangles(int x)
    {
        if (x >= base._x && x <= base._x + base._width - switcherSize)
            switcher.setPosition(sf::Vector2f(x, switcher.getPosition().y));
        curCount = (switcher.getPosition().x + switcherSize / 2.0 - base._x) /
                   base._width;
        
        // Нормализация curCount: запихиваем в invokingFunc нормализованное 
        // значение curCount
        double min = (switcherSize/2.)/base._width,
               max = (base._width - switcherSize/2.)/base._width;

        _resizeDrawable();
        invokingFunc((curCount - min) / (max - min));
    }

    void _moveDrawable()
    {
        leftSide.setPosition(
            sf::Vector2f(base._x, base._y + base._height / 4.0));
        rightSide.setPosition(
            sf::Vector2f(base._x + base._width, base._y + base._height / 4.0));
        switcher.setPosition(sf::Vector2f(
            base._x + base._width * curCount - switcherSize / 2.0, base._y));
    }

    void _resizeDrawable()
    {
        leftSide.setSize(
            sf::Vector2f(base._width * curCount, base._height / 2.0));
        rightSide.setSize(
            sf::Vector2f(base._width * (1 - curCount), base._height / 2.0));
        switcher.setSize(sf::Vector2f(switcherSize, base._height));
        rightSide.setOrigin({rightSide.getLocalBounds().width, 0});
    }

    _cimpl(Slider &b, double left, double right, double st = 0) : base(b)
    {
        leftBorder = left;
        rightBorder = right;
        _resizeDrawable();
        _moveDrawable();

        leftSide.setFillColor(lsColor);
        rightSide.setFillColor(rsColor);
        switcher.setFillColor(swColor);

        for (auto &i : _getDrawVec())
        {
            i->setOutlineColor(outlineColor);
            i->setOutlineThickness(borderThickness);
        }
    }


};

game::Slider::Slider(uint64_t width, uint64_t height, double left, double right)
    : GuiObject(width, height)
{
    _impl = std::make_unique<_cimpl>(*this, left, right);
}

game::Slider::Slider(Slider &&op) noexcept
    : GuiObject(op._width, op._height), _impl(nullptr)
{
    if (op._impl)
    {
        _impl = std::move(op._impl);
        _height = op._height;
        _width = op._width;
        _x = op._x;
        _y = op._y;
        op._height = 0;
        op._width = 0;
        op._x = 0;
        op._y = 0;
        op._impl = nullptr;
    }
}

// ЭЭЭ уга буга
game::Slider &game::Slider::operator=(Slider &&op) noexcept
{
    if (&op != this && op._impl != nullptr)
    {
        _impl = std::move(op._impl);
        _height = op._height;
        _width = op._width;
        _x = op._x;
        _y = op._y;
        op._height = 0;
        op._width = 0;
        op._x = 0;
        op._y = 0;
        op._impl = nullptr;
    }
    return *this;
}

void game::Slider::show(sf::RenderWindow &r)
{
    for (auto &i : _impl->_getDrawVec())
    {
        r.draw(*i);
    }
}

std::unique_ptr<game::GuiObject> game::Slider::clone() const
{
    auto t = std::make_unique<game::Slider>(_width, _height, _impl->leftBorder,
                                            _impl->rightBorder);
    t->_impl->invokingFunc = _impl->invokingFunc;
    t->_impl->lsColor = _impl->lsColor;
    t->_impl->rsColor = _impl->rsColor;
    t->_impl->swColor = _impl->swColor;
    t->_impl->outlineColor = _impl->outlineColor;
    t->_impl->borderThickness = _impl->borderThickness;
    return t;
}

game::Slider::~Slider() = default;


void game::Slider::_setPos(uint64_t x, uint64_t y)
{
    GuiObject::_setPos(x, y);
    _impl->_moveDrawable();
}

void game::Slider::_resize(uint64_t width, uint64_t height)
{
    GuiObject::_resize(width, height);
    _impl->_resizeDrawable();
}

void game::Slider::_invoke(const sf::RenderWindow &capture, const sf::Event &ev)
{
    switch (ev.type)
    {
        case sf::Event::EventType::MouseButtonPressed:
            if (_impl->switcher.getGlobalBounds().contains(
                    capture.mapPixelToCoords(
                        {ev.mouseButton.x, ev.mouseButton.y})))
            {
                _impl->_isToggled = true;
            }
            break;

        case sf::Event::EventType::MouseButtonReleased:
            _impl->_isToggled = false;
            break;

        case sf::Event::EventType::MouseMoved:
            if (_impl->_isToggled)
            {
                _impl->_moveRectangles(
                    capture.mapPixelToCoords({ev.mouseMove.x, ev.mouseMove.y})
                        .x);
            }
            break;

    default:
        break;
    }
}


void game::Slider::setFunc(const std::function<void(double)> &f)
{
    _impl->invokingFunc = f;
}

game::Dot<double> game::Slider::getBorders() const
{
    return {_impl->leftBorder, _impl->rightBorder};
}

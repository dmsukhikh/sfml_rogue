#include "../../include/gamegui/Label.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <memory>

class game::Label::_cimpl
{
public:
    static bool _fontLoaded;
    static sf::Font _f;

    sf::Text _outtxt;

    // Не const, потому что объект _cimpl переносим (std::move)
    std::string &_txt;
    float _BorderThickness = 5.0;
    sf::Color fillcol = sf::Color::White;
    uint64_t chsize = 0;

    // Вынести хардкод куда-нибудб :(
  _cimpl(uint64_t width, uint64_t height, std::string &txt) : _txt(txt)
  {
      if (!_fontLoaded)
      {
          _f.loadFromFile("../resources/ThaleahFat.ttf");
          _fontLoaded = true;
      }
      _outtxt.setFont(_f);
      _outtxt.setString(_txt);
      _outtxt.setFillColor(fillcol);
      _outtxt.setOutlineThickness(_BorderThickness);
      _setTextProperties(width, height);
  }

  _cimpl &operator=(_cimpl &&op) 
  {
    _txt = std::move(op._txt);
    _outtxt = std::move(op._outtxt);
    return *this;
  }

  void _setTextProperties(uint64_t width, uint64_t height)
  {
      chsize =
          std::min(1.5 * width / (_outtxt.getString().getSize()), height/1.5);
      _outtxt.setCharacterSize(chsize);
      _outtxt.setOrigin(_outtxt.getLocalBounds().width / 2.,
                        _outtxt.getLocalBounds().height / 2.);
  }
};

sf::Font game::Label::_cimpl::_f{};
bool game::Label::_cimpl::_fontLoaded = false;

game::Label::Label(uint64_t width, uint64_t height, std::string &txt)
    : GuiObject(width, height)
{
    _impl = std::make_unique<_cimpl>(width, height, txt);
}

// Ультрамощни мув с std::move. Теперь компилятор сам сгенерирует конструктор
// копирования для GuiObject
game::Label::Label(game::Label &&op) noexcept : GuiObject(std::move(op))
{
    if (op._impl)
    {
        _impl = std::move(op._impl);
    }
}

game::Label& game::Label::operator=(Label &&op) noexcept
{
    if (op._impl && &op != this)
    {
        _impl = std::move(op._impl);
        // Ультрагол ?????
        dynamic_cast<GuiObject&>(*this) = std::move(op);
    }
    return *this;
}

game::Label::~Label() = default;

void game::Label::show(sf::RenderWindow &sf)
{
    sf.draw(_impl->_outtxt);
    _impl->_outtxt.setString(_impl->_txt);
}

void game::Label::_resize(uint64_t width, uint64_t height)
{
    _width = width;
    _height = height;
    _impl->_setTextProperties(width, height);
}

void game::Label::_setPos(uint64_t x, uint64_t y)
{
    _x = x;
    _y = y;
    _impl->_outtxt.setPosition(x+_width/2., y+_height/2.-_impl->chsize/2.);
}

std::unique_ptr<game::GuiObject> game::Label::clone() const
{
    // Клонирование приводит к тому, что клонированные объекты будут иметь 
    // ссылку на ту же переменную, что и их родитель.
    auto t = std::make_unique<Label>(_width, _height, _impl->_txt);
    t->_setPos(_x, _y);
    return t;
}

void game::Label::setColor(sf::Color new_color)
{
    _impl->fillcol = new_color;
    _impl->_outtxt.setFillColor(new_color);
}

void game::Label::setOutlineThickness(float new_th)
{
    _impl->_BorderThickness = new_th;
    _impl->_outtxt.setOutlineThickness(new_th);
}

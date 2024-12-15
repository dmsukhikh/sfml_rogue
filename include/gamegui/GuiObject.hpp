#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include <memory>

namespace game
{

template<typename T>
struct Dot
{
    T x, y;
};

class GuiObject
{
  protected:
    // Переделать: заменить типы _x и _y на простые int64
    uint64_t _width, _height, _x = 0, _y = 0;

  public:
    GuiObject(uint64_t w, uint64_t h);
    virtual std::unique_ptr<GuiObject> clone() const = 0;
    virtual ~GuiObject() = default;

    // Приходится переносить эти функции в public-раздел по той самой причине,
    // что разрабы с++ "хорошие люди". Написано: protected делает доступным
    // поля и методы в текущем классе и в производных классах. По факту: если
    // мы определим функцию, например, void Derived::func(Base &a), то в func
    // не будут видны protected-поля Base, хотя Derived наследуется от Base.
    // Ну и шизофрения

    // 
    virtual void show(sf::RenderWindow& target_window) = 0;
    virtual void _resize(uint64_t width, uint64_t height);
    virtual void _setPos(uint64_t x, uint64_t y);

    // Передаем окно захвата для грамотной обработки координат мышки в 
    // игровом пространстве
    virtual void _invoke(const sf::RenderWindow& capture, const sf::Event &ev);
    Dot<uint64_t> getPos() const;
    Dot<uint64_t> getSize() const;
};

} // namespace game

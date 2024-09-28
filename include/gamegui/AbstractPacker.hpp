#pragma once
#include "GuiObject.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <vector>

namespace game
{
class AbstractPacker : public GuiObject
{
  protected:
    std::vector<std::unique_ptr<game::GuiObject>> _data;

  public:
    // Класс-интерфейс, основа для других упаковщиков. Сам по себе безполезен
    AbstractPacker(uint64_t width, uint64_t height);
    AbstractPacker(const AbstractPacker& op) = delete;
    AbstractPacker& operator=(const AbstractPacker& op) = delete;
    AbstractPacker(AbstractPacker &&op) noexcept;
    AbstractPacker& operator=(AbstractPacker &&op) noexcept;
    ~AbstractPacker() override;

    void displayWidgets(sf::RenderWindow &op);
    void _show(sf::RenderWindow &op) override;
    std::unique_ptr<GuiObject> clone() const override;
    
};
} // namespace game

// Я буду надеяться, что компилятор достаточно умен для того, чтобы 
// сгенерировать правильные операторы и конструкторы

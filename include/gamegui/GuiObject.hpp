#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace game
{

class GuiObject
{
  public:
    virtual std::unique_ptr<GuiObject> clone() const = 0;
    virtual void show(sf::RenderWindow& target_window) = 0;
    virtual void resize();
    virtual ~GuiObject() = default;
};

} // namespace game

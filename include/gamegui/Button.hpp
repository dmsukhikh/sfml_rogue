#include "GuiObject.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

namespace game
{

class Button : public GuiObject
{
    class _cimpl;
    std::unique_ptr<_cimpl> _impl;

  public:
    Button(uint32_t width, uint32_t height, const std::string &txt,
           uint32_t font = 12, sf::Color bg = sf::Color::White,
           sf::Color fg = sf::Color::Black);

    Button(const Button &op) noexcept;
    Button(Button &&op) noexcept;
    Button &operator=(const Button &op) noexcept;
    Button &operator=(Button &&op) noexcept;

    void show(sf::RenderWindow &op) override;
    std::unique_ptr<GuiObject> clone() const override;
    ~Button() override;
};

} // namespace game

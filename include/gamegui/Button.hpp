#include "GuiObject.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <functional>

namespace game
{

class Button : public GuiObject
{
  private:
    class _cimpl;
    std::unique_ptr<_cimpl> _impl;

  public:
    Button(uint32_t width, uint32_t height, const std::string &txt,
           uint32_t font = 0, sf::Color fg = sf::Color::Black,
           sf::Color bg = sf::Color::White);

    Button(const Button &op) noexcept;
    Button(Button &&op) noexcept;
    Button &operator=(const Button &op) noexcept;
    Button &operator=(Button &&op) noexcept;
    void _show(sf::RenderWindow &op) override;
    void _resize(uint64_t width, uint64_t height) override;
    void _setPos(uint64_t x, uint64_t y) override;
    void _invoke(const sf::Event &ev) override;
    void setFunc(const std::function<void()> &f);

    std::unique_ptr<GuiObject> clone() const override;
    ~Button() override;
};

} // namespace game

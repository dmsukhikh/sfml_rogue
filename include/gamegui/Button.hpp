#include "GuiObject.hpp"
#include "SFML/Graphics.hpp"
#include <memory>
#include <functional>
#include <string>

namespace game
{

class Button : public GuiObject
{
  private:
    class _cimpl;
    std::unique_ptr<_cimpl> _impl;

  public:
    Button(uint32_t width, uint32_t height);
    Button(const Button &op) noexcept;
    Button(Button &&op) noexcept;
    Button &operator=(const Button &op) noexcept;
    Button &operator=(Button &&op) noexcept;

    void show(sf::RenderWindow &op) override;
    void _resize(uint64_t width, uint64_t height) override;
    void _setPos(uint64_t x, uint64_t y) override;
    void _invoke(const sf::RenderWindow &capture, const sf::Event &ev) override;
    
    void setBgColor(const sf::Color &col);
    void setFgColor(const sf::Color &col);
    void setOutlineColor(const sf::Color &col);
    void setToggleColor(const sf::Color &col);
    void setText(const std::string &txt);
    void setFont(uint64_t font);
    void setOutlineThickness(float thickness);



    void setFunc(const std::function<void()> &f);

    std::unique_ptr<GuiObject> clone() const override;
    ~Button() override;
};

} // namespace game

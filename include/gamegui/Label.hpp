#pragma once
#include "GuiObject.hpp"

namespace game
{
class Label : public GuiObject
{
    class _cimpl;
    std::unique_ptr<_cimpl> _impl;

  public:
    Label(uint64_t width, uint64_t height, std::string &txt);
    Label(const Label &op) = delete;
    Label &operator=(const Label &op) = delete;
    Label(Label &&op) noexcept;
    Label &operator=(Label &&op) noexcept;
    ~Label() override;

    void show(sf::RenderWindow &target_window) override;
    void _resize(uint64_t width, uint64_t height) override;
    void _setPos(uint64_t x, uint64_t y) override;
    void setColor(sf::Color new_color);
    void setOutlineThickness(float new_th);
    std::unique_ptr<GuiObject> clone() const override;
};
} // namespace game

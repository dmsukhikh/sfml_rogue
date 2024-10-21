#pragma once
#include "GuiObject.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <memory>
#include <functional>

namespace game
{
class Slider : public GuiObject
{
    class _cimpl;
    std::unique_ptr<_cimpl> _impl;

    public:
        Slider(uint64_t width, uint64_t height, double left, double right);
        Slider(const Slider &op);
        Slider(Slider &&op) noexcept;
        Slider &operator=(const Slider &op);
        Slider &operator=(Slider &&op) noexcept;

        void show(sf::RenderWindow &r) override;
        std::unique_ptr<GuiObject> clone() const override;
        void _setPos(uint64_t x, uint64_t y) override;
        void _resize(uint64_t width, uint64_t height) override;
        void _invoke(const sf::RenderWindow &capture, const sf::Event &ev) override;

        void setFunc(const std::function<void(double)> &f);
        Dot<double> getBorders() const;

        ~Slider() override;

};
} // namespace game

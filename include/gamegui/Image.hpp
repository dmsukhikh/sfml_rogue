#pragma once
#include "GuiObject.hpp"

namespace game
{
class Image : public GuiObject
{
    sf::Sprite _im;
    sf::Texture _tx;

  public:
    Image(const std::string &filename);
    Image(const Image &op);
    Image &operator=(const Image &op) noexcept = delete;
    // Поправить, убрать эти дефолты!!!
    Image(Image &&op) = default;
    Image &operator=(Image &&op) noexcept = default;

    void show(sf::RenderWindow &target_window) override;
    void _resize(uint64_t width, uint64_t height) override;
    void _setPos(uint64_t x, uint64_t y) override;
    std::unique_ptr<GuiObject> clone() const override;
};
} // namespace game

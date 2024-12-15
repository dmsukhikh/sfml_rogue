#include "AbstractPacker.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include <cstdint>
#include <memory>

namespace game
{

class GridPacker final : public AbstractPacker
{
    class _cimpl;
    std::unique_ptr<_cimpl> _impl;

  public:
    enum class Anchor
    {
        EXPAND,
        FIXED 
    };


    GridPacker(uint64_t width, uint64_t height);
    GridPacker(const GridPacker &op);
    GridPacker(GridPacker &&op) noexcept;
    GridPacker &operator=(const GridPacker &op);
    GridPacker &operator=(GridPacker &&op) noexcept;
    ~GridPacker() override;

    std::unique_ptr<GuiObject> clone() const override;
    void displayWidgets(sf::RenderWindow &op) override; 

    void putObject(std::unique_ptr<GuiObject> &&op, uint64_t row,
                   uint64_t column, Anchor anchx = Anchor::EXPAND,
                   Anchor anchy = Anchor::EXPAND);

    void putObject(const GuiObject &op, uint64_t row, uint64_t column,
                   Anchor anchx = Anchor::EXPAND, Anchor anchy = Anchor::EXPAND);

    void _setPos(uint64_t x, uint64_t y) override;
    void _resize(uint64_t x, uint64_t y) override;
};
}

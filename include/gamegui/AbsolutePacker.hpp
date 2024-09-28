#pragma once
#include "GuiObject.hpp"
#include "AbstractPacker.hpp"

namespace game
{
class AbsolutePacker final : public AbstractPacker
{
  public:
    AbsolutePacker(uint64_t width, uint64_t height);
    AbsolutePacker(AbsolutePacker &&op) noexcept;
    AbsolutePacker &operator=(AbsolutePacker &&op) noexcept;
    ~AbsolutePacker() override;

    void putObject(std::unique_ptr<GuiObject> &&op, uint64_t x, uint64_t y);
    void putObject(const GuiObject &op, uint64_t x, uint64_t y);
};
}

#include "../../include/gamegui/AbsolutePacker.hpp"
#include <cstdint>
#include <sys/types.h>

game::AbsolutePacker::AbsolutePacker(uint64_t width, uint64_t height)
    : AbstractPacker(width, height)
{
}

game::AbsolutePacker::AbsolutePacker(AbsolutePacker &&op) noexcept
    : AbstractPacker(std::move(op))
{
}

game::AbsolutePacker &
game::AbsolutePacker::operator=(AbsolutePacker &&op) noexcept
{
    return *this;
}

void game::AbsolutePacker::putObject(const GuiObject &op, uint64_t x,
                                     uint64_t y)
{
    auto temp = op.clone();
    temp->_setPos(x, y);
    _data.push_back(std::move(temp));
}

void game::AbsolutePacker::putObject(std::unique_ptr<GuiObject> &&op,
                                     uint64_t x, uint64_t y)
{
    // Поскольку все объекты хранятся по smart ptr'ам, объекты размещаются
    // на куче. Поэтому просто GuiObject && передавать нельзя: объект из кучи
    // запихивать в смарт-поинтер глупо. Поэтому для перемещения необходимо
    // сразу размещать объект на куче

    op->_setPos(x, y);
    _data.push_back(std::move(op));
}

game::AbsolutePacker::~AbsolutePacker() = default;

#include "../../include/gamegui/GridPacker.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <memory>
#include <vector>
#include <map>

class game::GridPacker::_cimpl
{
  public:
    struct _WidgetProp
    {
        std::size_t idx;
        uint64_t row, col;
        game::GridPacker::Anchor anchx, anchy;
    };
    std::vector<_WidgetProp> _mapData;
    std::size_t _idx = 0;
    const uint64_t _SPAN = 0;

    // Расширение идиомы pimpl: ссылка на класс-интерфейс для 
    // доступа к унаследованным приватным полям
    game::GridPacker &_base;

    _cimpl(game::GridPacker &base) : _base(base) {}
    _cimpl& operator=(const _cimpl &op)
    {
        _idx = op._idx;
        _mapData = op._mapData;
        return *this;
    }

    // Стоит отметить важный момент, относительно работы GridPacker:
    // массив _data не должен меняться, за исключением, быть может,
    // добавления новых элементов. То есть, из _data не должны удаляться
    // виджеты, а также их порядок не должен меняться. Конечно, текущая 
    // реализация GridPacker'a этого всего не позволяет, однако же такой
    // моментик должен быть учтен в будущем. 
    // Также, row и col начинаются с нуля
    void _putObjectProps(uint64_t row, uint64_t col,
                         game::GridPacker::Anchor anchx,
                         game::GridPacker::Anchor anchy)
    {
        _mapData.push_back({_idx++, row, col, anchx, anchy});
    }

    void _computeMapping()
    {
        // Ключ - строка, или столбец; Значение - размер строки (или
        // столбца) в пикселях. Выбирается вычислением максимального
        // размера некоторого элемента
        std::map<uint64_t, uint64_t> _mapWidth, _mapHeight, _mapPosX, _mapPosY;
        _mapWidth[0] = 0;
        _mapHeight[0] = 0;

        // Ищем количество строк/столбцов, чтобы посчитать базовый
        // размер ячейки в таблице
        for (auto &&i: _mapData)
        {
            _mapWidth[i.col];
            _mapHeight[i.row];
        }
        uint64_t _baseWidth = _base._width / _mapWidth.size(),
                 _baseHeight = _base._height / _mapHeight.size();

        for (auto &&i: _mapData)
        {
            uint64_t compWidth, compHeight;
            // Обработка ширины...
            switch (i.anchx)
            {
                case Anchor::EXPAND:
                    compWidth = _baseWidth;
                    break;
                case Anchor::FIXED:
                    compWidth = _base._data[i.idx]->getSize().x;
                    break;
            }

            // Обработка длины...
            switch (i.anchy)
            {
                case Anchor::EXPAND:
                    compHeight = _baseHeight;
                    break;
                case Anchor::FIXED:
                    compHeight = _base._data[i.idx]->getSize().y;
                    break;
            }

            _mapWidth[i.col] = std::max(_mapWidth[i.col], compWidth);
            _mapHeight[i.row] = std::max(_mapHeight[i.row], compHeight);
        }
        
        uint64_t curx = _base._x, cury = _base._y;
        for (auto &[k, v]: _mapWidth)
        {
            _mapPosX[k] = curx;
            curx += v+_SPAN;
        }
        for (auto &[k, v]: _mapHeight)
        {
            _mapPosY[k] = cury;
            cury += v+_SPAN;
        }

        for (auto &&i: _mapData)
        {
            uint64_t width, height;
            switch (i.anchx)
            {
                case Anchor::FIXED:
                    width = _base._data[i.idx]->getSize().x;
                    break;
                case Anchor::EXPAND:
                    width = _mapWidth[i.col];
            }

            switch (i.anchy)
            {
                case Anchor::FIXED:
                    height = _base._data[i.idx]->getSize().y;
                    break;
                case Anchor::EXPAND:
                    height = _mapHeight[i.row];
            }
        
            _base._data[i.idx]->_setPos(_mapPosX[i.col], _mapPosY[i.row]);
            _base._data[i.idx]->_resize(width, height);    
        }
    }
};


game::GridPacker::GridPacker(uint64_t width, uint64_t height)
    : AbstractPacker(width, height), _impl(nullptr)
{
    auto& i = *this;
    _impl = std::make_unique<_cimpl>(i);
}

game::GridPacker::GridPacker(game::GridPacker &&op) noexcept
    : AbstractPacker(std::move(op)), _impl(nullptr)
{
    if (op._impl)
    {
        _impl = std::move(op._impl);
        op._impl = nullptr;
    }
}

game::GridPacker &game::GridPacker::operator=(game::GridPacker &&op) noexcept
{
    game::AbstractPacker::operator=(std::move(op));
    if (op._impl)
    {
        _impl = std::move(op._impl);
        op._impl = nullptr;
    }
    return *this;
}

game::GridPacker::~GridPacker() = default;

void game::GridPacker::putObject(std::unique_ptr<GuiObject> &&op, uint64_t row,
                                 uint64_t column, Anchor anchx, Anchor anchy)
{
    _data.push_back(std::move(op));
    _impl->_putObjectProps(row, column, anchx, anchy);
    _impl->_computeMapping();
}

void game::GridPacker::putObject(const GuiObject &op, uint64_t row,
                                 uint64_t column, Anchor anchx, Anchor anchy)
{
    _data.push_back(op.clone());
    _impl->_putObjectProps(row, column, anchx, anchy);
    _impl->_computeMapping();
}

void game::GridPacker::displayWidgets(sf::RenderWindow &op)
{
    game::AbstractPacker::displayWidgets(op);
}

void game::GridPacker::_setPos(uint64_t x, uint64_t y)
{
    game::GuiObject::_setPos(x, y);
    _impl->_computeMapping();
}

void game::GridPacker::_resize(uint64_t width, uint64_t height)
{
    game::GuiObject::_resize(width, height);
    _impl->_computeMapping();
}


std::unique_ptr<game::GuiObject> game::GridPacker::clone() const
{
    std::vector<std::unique_ptr<GuiObject>> temp(_data.size());
    for (std::size_t i = 0; i < _data.size(); ++i)
    {
        temp[i] = _data[i]->clone();
    }

    auto t = std::make_unique<GridPacker>(_width, _height);
    t->_data = std::move(temp);
    *t->_impl = *_impl;
    t->_x = _x;
    t->_y = _y;
    return t;
}

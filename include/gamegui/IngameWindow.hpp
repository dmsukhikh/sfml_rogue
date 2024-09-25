#pragma once
#include <memory>

namespace game
{

class IngameWindow
{
public:
    IngameWindow();
    IngameWindow(const IngameWindow &from);
    IngameWindow& operator=(const IngameWindow &from);
    IngameWindow(IngameWindow &&from);
    IngameWindow& operator=(IngameWindow &&from);

    ~IngameWindow();

    void display();
    
private:
    class _cimpl;
    std::unique_ptr<_cimpl> _impl;
};


}

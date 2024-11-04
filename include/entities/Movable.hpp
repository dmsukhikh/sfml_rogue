#pragma once
#include "Entity.hpp"
#include <cstdint>

namespace game
{
class Movable : public Entity
{
    // Класс, который наследуют все сущности игры, которые двигаются: игрок,
    // враги, пули. Возможно, нужно было сделать такой же класс для тайлов, но
    // пока что это не нужно

  protected:
    const float _SIZE = 50;
    float _MAXSPEEDABS = 400;
    float _ACCABS = 1500;
    float _SLOWDOWNABS = 600;
    float _angle = 0;
    sf::Vector2f _speed = {0, 0};
    uint16_t _hp = 5;
    bool _isGamerEntity = false;

  public:
    int _xmovement = 0, _ymovement = 0;
    bool isExisted = true;

    Movable(float x, float y);
    Movable();

    void decreaseHp(uint16_t on);
    void setGamerState(bool i);
    float getAngle() const;
    uint16_t getHp() const;

    // Отвечает за обработку столкновения с каким-то объектом
    virtual void collideHandling(Entity &op);
    virtual void collideHandling(Movable &op);

    // Функции, считающие формулы всякие. В перегрузках необходимо сначала
    // исполнять эти функции, а потом как-то двигать спрайты
    virtual void rotate(float x, float y);
    virtual void rotate(float angle);
    virtual void move(float delta);
    virtual void stop(float delta, sf::Vector2f def);
    virtual void setPos(float x, float y);
};

}

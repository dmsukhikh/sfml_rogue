#pragma once
#include "Entity.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstdint>
#include "../SoundManager.hpp"


namespace game
{
class Movable : public Entity
{
    // Класс, который наследуют все сущности игры, которые двигаются: игрок,
    // враги, пули. Возможно, нужно было сделать такой же класс для тайлов, но
    // пока что это не нужно

  protected:
    static SoundManager soundManager;
    static int IDGEN;
    static float SHOCKCD, POISONCD, POISONLIMIT;

    sf::Sound sound;
    void play(const sf::SoundBuffer &op);

    int id;
    int bounty = 0;
    float _SIZE = BLOCK_SIZE;
    float _MAXSPEEDABS = 8*BLOCK_SIZE;
    float _ACCABS = 30*BLOCK_SIZE;
    float _SLOWDOWNABS = 12*BLOCK_SIZE;
    float _angle = 0;
    sf::Vector2f _speed = {0, 0};
    uint16_t _hp = 5;

    float shockClock = 0, poisonClock = 0, poisonDamage = 0;

  public:
    static int VOLUME;
    int _xmovement = 0, _ymovement = 0;
    bool isExisted = true, hasShocked = false, hasExplosive = false,
         hasPoisoned = false;
    EntityType masterType = EntityType::None;

    Movable(float x, float y);
    Movable();

    void decreaseHp(uint16_t on);

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

    int getBounty() const;
    int getId() const;
    float getAngle() const;
    uint16_t getHp() const;
    sf::Vector2f getSpeed() const;

};

}

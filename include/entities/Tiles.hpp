#pragma once
#include "Entity.hpp"
#include <cstdint>
#include <memory>

namespace game
{
class Wall : public Entity
{
protected:
  sf::RectangleShape _outputShape;

  public:
    Wall();
    Wall(float x, float y);

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;
};

class Floor : public Entity
{
    // Класс-заглушка для пустых тайлов на карте. Мб позже тут появится
    // какая-нибудь 
  public:
    Floor();
    Floor(float x, float y);

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;
};

class Port : public Wall
{
    uint64_t _idx;
    bool isActive = false;
    bool isLinked = false;
    static const sf::Color COL;
    static const sf::Color DISABLED_COL;
public:
    Port();
    Port(float x, float y);

    void changeActiveness(bool isActive);
    bool getLinked() const;
    void setIdx(uint64_t idx);
    uint64_t getIdx() const;
    std::unique_ptr<Entity> copy() const override;
};
}

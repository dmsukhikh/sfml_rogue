#pragma once
#include "Entity.hpp"
#include <cstdint>
#include <memory>
#include "entitiesVisitor.hpp"

namespace game
{
class Wall : public Entity
{
protected:

  public:
  sf::RectangleShape _outputShape;
    Wall();
    Wall(float x, float y);

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;

    void accept(EntitiesVisitor &visitor) override;
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
    static sf::Font f; 
    static bool fontIsLoaded;
    sf::Text idxToTeleport;

protected:
    uint64_t _idx;
    bool isActive = false;
    bool isLinked = false;

    // не-статик, чтобы в наследуемых телепортах переопределялись штуки
    EntityType activeType;
    sf::Color col;
    sf::Color disabledCol;

public:
    Port();
    Port(float x, float y);

    void changeActiveness(bool isActive);
    void show(sf::RenderWindow &win) const override;
    bool getLinked() const;
    void setIdx(uint64_t idx);
    uint64_t getIdx() const;
    std::unique_ptr<Entity> copy() const override;

    void accept(EntitiesVisitor &visitor) override;
};

class LevelPort : public Port
{
  public:
    LevelPort();
    LevelPort(float x, float y);
    std::unique_ptr<Entity> copy() const override;

    void accept(EntitiesVisitor &visitor) override;
};
} // namespace game

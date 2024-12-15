#pragma once
#include "Movable.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Bullets.hpp"
#include <memory>
#include <map>

namespace game
{
class Lazer : public AbstractShot 
{
    static const std::vector<std::unique_ptr<Entity>> *map;
    sf::RectangleShape _sprite;
    std::vector<sf::FloatRect> _hitboxes;
    float lifetime = 0;
    std::map<int, float> damageCDs;


  public:
    static void setMap(const std::vector<std::unique_ptr<Entity>> *anmap);
    static Lazer getLazer(sf::Vector2f pos, float angle);
    static const float appearingTime, beatingTime;

    sf::Color appearingCol = sf::Color::Red, damagingCol = sf::Color::White;

    // Иметь публичные конструкторы - эщкере или не эщкере, если есть фабрика?
    Lazer(float x, float y);
    Lazer();

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    std::unique_ptr<Entity> copy() const override;
    void move(float delta) override;
    void rotate(float angle) override;

    void collideHandling(Entity &op) override;
    void collideHandling(Movable &op) override;
};
}

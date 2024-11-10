#include "Movable.hpp"
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <optional>

namespace game
{
class Gamer : public Movable
{
    sf::ConvexShape _sprite, _hitbox;
    float shotCD = 0;

  public:
    bool isShooting = false;

    Gamer(float x, float y);
    Gamer();

    void show(sf::RenderWindow &win) const override;
    std::vector<sf::FloatRect> getHitboxes() const override;
    bool collide(const Entity &op) const override;
    
    void rotate(float x, float y) override;
    void move(float delta) override;
    void stop(float delta, sf::Vector2f def) override;
    void setPos(float x, float y) override;
    std::unique_ptr<Entity> copy() const override;
    std::optional<std::unique_ptr<game::Movable>> shot(float delta);
};
}
